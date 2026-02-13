import torch
import torch.nn as nn
import torch.optim as optim

torch.manual_seed(0)
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# ============================================================
# 1. 构造数据集：从序列中预测最大值
# ============================================================
def generate_batch(batch_size=64, seq_len=10):
    x = torch.rand(batch_size, seq_len, 1)  # (B, T, 1)
    y = x.max(dim=1).values                 # (B, 1)
    return x.to(device), y.to(device)


# ============================================================
# 2. 无注意力的 RNN 基线模型
# ============================================================
class RNNBaseline(nn.Module):
    def __init__(self, input_dim=1, hidden_dim=32):
        super().__init__()
        self.rnn = nn.GRU(input_dim, hidden_dim, batch_first=True)
        self.fc = nn.Linear(hidden_dim, 1)

    def forward(self, x):
        # x: (B, T, 1)
        outputs, h_last = self.rnn(x)   # h_last: (1, B, H)
        h_last = h_last.squeeze(0)      # (B, H)
        out = self.fc(h_last)           # (B, 1)
        return out


# ============================================================
# 3. 单头 Q/K/V 自注意力（点积注意力）
# ============================================================
class RNNAttentionQKV(nn.Module):
    def __init__(self, input_dim=1, hidden_dim=32):
        super().__init__()
        self.rnn = nn.GRU(input_dim, hidden_dim, batch_first=True)

        self.W_Q = nn.Linear(hidden_dim, hidden_dim)
        self.W_K = nn.Linear(hidden_dim, hidden_dim)
        self.W_V = nn.Linear(hidden_dim, hidden_dim)

        self.fc = nn.Linear(hidden_dim, 1)
        self.scale = hidden_dim ** 0.5

    def forward(self, x):
        # x: (B, T, 1)
        H, _ = self.rnn(x)             # H: (B, T, H)

        Q = self.W_Q(H)                # (B, T, H)
        K = self.W_K(H)                # (B, T, H)
        V = self.W_V(H)                # (B, T, H)

        scores = torch.bmm(Q, K.transpose(1, 2)) / self.scale   # (B, T, T)
        attn_weights = torch.softmax(scores, dim=-1)            # (B, T, T)

        O = torch.bmm(attn_weights, V)                          # (B, T, H)
        # 平均池化
        context = O.mean(dim=1)                                 # (B, H)
        out = self.fc(context)                                  # (B, 1)
        return out, attn_weights


# ============================================================
# 4. 多头自注意力（nn.MultiheadAttention）
# ============================================================
class RNNMultiHeadAttn(nn.Module):
    def __init__(self, input_dim=1, hidden_dim=32, num_heads=4):
        super().__init__()
        assert hidden_dim % num_heads == 0, "hidden_dim 必须能被 num_heads 整除"

        self.rnn = nn.GRU(input_dim, hidden_dim, batch_first=True)

        self.mha = nn.MultiheadAttention(
            embed_dim=hidden_dim,
            num_heads=num_heads,
            batch_first=False  # 输入输出用 (T, B, E)
        )

        self.fc = nn.Linear(hidden_dim, 1)

    def forward(self, x):
        # x: (B, T, 1)
        H, _ = self.rnn(x)             # (B, T, H)
        H_tbh = H.transpose(0, 1)      # (T, B, H)

        attn_output, attn_weights = self.mha(
            H_tbh, H_tbh, H_tbh,
            need_weights=True
        )
        O = attn_output.transpose(0, 1)  # (B, T, H)
        context = O.mean(dim=1)          # (B, H)
        out = self.fc(context)           # (B, 1)

        # 将权重整理为 (B, heads, T, T)
        if attn_weights.dim() == 3:
            B = x.size(0)
            num_heads = attn_weights.size(0) // B
            attn_weights = attn_weights.view(
                B, num_heads, attn_weights.size(1), attn_weights.size(2)
            )
        return out, attn_weights


# ============================================================
# 5. Bahdanau（加性）注意力版本
# ============================================================
class RNNBahdanauAttention(nn.Module):
    def __init__(self, input_dim=1, hidden_dim=32, attn_dim=32):
        super().__init__()
        self.rnn = nn.GRU(input_dim, hidden_dim, batch_first=True)

        # 可学习 query 向量（代表“我要从序列中抽取的东西”，这里是“最大值相关信息”）
        self.query = nn.Parameter(torch.randn(hidden_dim))

        # Bahdanau 加性注意力： e_t = v^T tanh(W_h h_t + W_q q)
        self.W_h = nn.Linear(hidden_dim, attn_dim, bias=False)
        self.W_q = nn.Linear(hidden_dim, attn_dim, bias=False)
        self.v   = nn.Linear(attn_dim, 1, bias=False)

        self.fc = nn.Linear(hidden_dim, 1)

    def forward(self, x):
        # x: (B, T, 1)
        H, _ = self.rnn(x)                 # H: (B, T, H)
        B, T, H_dim = H.shape

        # 扩展 query 到 batch 维度
        q = self.query.unsqueeze(0).expand(B, H_dim)     # (B, H)
        q = q.unsqueeze(1).expand(B, T, H_dim)           # (B, T, H)

        # 计算 e_t = v^T tanh(W_h h_t + W_q q)
        Wh = self.W_h(H)                                # (B, T, attn_dim)
        Wq = self.W_q(q)                                # (B, T, attn_dim)
        scores = self.v(torch.tanh(Wh + Wq)).squeeze(-1)  # (B, T)

        attn_weights = torch.softmax(scores, dim=-1)    # (B, T)

        # 加权求和得到 context
        attn_expanded = attn_weights.unsqueeze(-1)      # (B, T, 1)
        context = (H * attn_expanded).sum(dim=1)        # (B, H)

        out = self.fc(context)                          # (B, 1)
        return out, attn_weights


# ============================================================
# 6. Cross-Attention 版本
# ============================================================
class RNNCrossAttention(nn.Module):
    """
    Q 来自一个独立的小 query 序列（可学习参数），
    K, V 来自 RNN 对输入序列的编码 H。
    """
    def __init__(self, input_dim=1, hidden_dim=32, query_len=1):
        super().__init__()
        self.rnn = nn.GRU(input_dim, hidden_dim, batch_first=True)

        # learnable query 序列：形状 (query_len, hidden_dim)
        self.query_seq = nn.Parameter(torch.randn(query_len, hidden_dim))

        # 使用标准的 MultiheadAttention 来实现 cross-attention
        self.mha = nn.MultiheadAttention(
            embed_dim=hidden_dim,
            num_heads=4,
            batch_first=False
        )

        self.fc = nn.Linear(hidden_dim, 1)

    def forward(self, x):
        # x: (B, T, 1)
        H, _ = self.rnn(x)          # H: (B, T, H)
        B, T, H_dim = H.shape

        # K, V: 来自输入序列 H (T, B, H)
        KV = H.transpose(0, 1)      # (T, B, H)

        # Q: 来自可学习 query_seq，扩展到 batch： (query_len, B, H)
        query_len = self.query_seq.size(0)
        Q = self.query_seq.unsqueeze(1).expand(query_len, B, H_dim)  # (Q_len, B, H)

        # Cross-attention: Q 对 KV
        attn_output, attn_weights = self.mha(
            Q,     # query: (Q_len, B, H)
            KV,    # key:   (T, B, H)
            KV,    # value: (T, B, H)
            need_weights=True
        )
        # attn_output: (Q_len, B, H)
        # attn_weights: (B * num_heads, Q_len, T) (旧版) 或 (B, num_heads, Q_len, T) (新版)

        # 将所有 query positions 的输出平均
        attn_output_mean = attn_output.mean(dim=0)      # (B, H)

        out = self.fc(attn_output_mean)                 # (B, 1)

        # 整理权重形状方便查看
        if attn_weights.dim() == 3:
            num_heads = attn_weights.size(0) // B
            attn_weights = attn_weights.view(
                B, num_heads, attn_weights.size(1), attn_weights.size(2)
            )
        return out, attn_weights


# ============================================================
# 7. 训练函数（通用）
# ============================================================
def train_model(model, epochs=400, batch_size=64, lr=1e-3, use_attention=False, name="model"):
    model.to(device)
    optimizer = optim.Adam(model.parameters(), lr=lr)
    criterion = nn.MSELoss()

    for epoch in range(1, epochs + 1):
        x, y = generate_batch(batch_size=batch_size)
        if use_attention:
            preds, attn = model(x)
        else:
            preds = model(x)
        loss = criterion(preds, y)

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        if epoch % 100 == 0:
            with torch.no_grad():
                x_val, y_val = generate_batch(batch_size=256)
                if use_attention:
                    val_preds, _ = model(x_val)
                else:
                    val_preds = model(x_val)
                val_loss = criterion(val_preds, y_val).item()
            print(f"[{name}] Epoch {epoch}/{epochs}, "
                  f"train loss={loss.item():.4f}, val loss={val_loss:.4f}")

    return model


# ============================================================
# 8. 训练并对比几个模型（可按需注释掉）
# ============================================================

epochs = 2000

print("训练无注意力的 RNNBaseline...")
baseline = RNNBaseline()
baseline = train_model(baseline, epochs, use_attention=False, name="Baseline")

print("\n训练 Bahdanau 注意力 RNNBahdanauAttention...")
bahdanau_model = RNNBahdanauAttention()
bahdanau_model = train_model(bahdanau_model, epochs, use_attention=True, name="Bahdanau")

print("\n训练 Cross-Attention 模型 RNNCrossAttention...")
cross_model = RNNCrossAttention()
cross_model = train_model(cross_model, epochs, use_attention=True, name="CrossAttn")

# 也可以继续训练单头、自注意力、多头注意力：
print("\n训练单头点积注意力 RNNAttentionQKV...")
single_attn_model = RNNAttentionQKV()
single_attn_model = train_model(single_attn_model, epochs, use_attention=True, name="DotProd")

# print("\n训练多头自注意力 RNNMultiHeadAttn...")
multi_attn_model = RNNMultiHeadAttn(num_heads=4)
multi_attn_model = train_model(multi_attn_model, epochs, use_attention=True, name="MultiHead")


# ============================================================
# 9. 测试一个样例，对比预测 & 查看注意力（以 Bahdanau 和 Cross 为例）
# ============================================================
with torch.no_grad():
    x_test, y_test = generate_batch(batch_size=1, seq_len=10)
    baseline_pred = baseline(x_test)
    bahdanau_pred, bahdanau_attn = bahdanau_model(x_test)
    cross_pred, cross_attn = cross_model(x_test)
    single_attn_pred, single_attn_attn = single_attn_model(x_test)
    multi_attn_pred, multi_attn_attn = multi_attn_model(x_test)

print("\n示例输入序列：")
print(x_test.cpu().squeeze(-1))  # (1, T) -> (T,)

print("真实最大值：", y_test.item())
print("Baseline 预测：", baseline_pred.item())
print("Bahdanau Attention 预测：", bahdanau_pred.item())
print("Cross-Attention 预测：", cross_pred.item())
print("单头点积注意力 预测：", single_attn_pred.item())
print("多头注意力 预测：", multi_attn_pred.item())

print("\nBahdanau 注意力权重（对每个时间步的标量权重）：")
print(bahdanau_attn.cpu().squeeze(0))   # (T,)

print("\nCross-Attention 权重形状：", cross_attn.shape)   # (B, heads, Q_len, T)
print("第 1 个 head、第 1 个 query 位置的注意力分布：")
print(cross_attn[0, 0, 0].cpu())       # (T,)
