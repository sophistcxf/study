import torch
import torch.nn as nn
import torch.optim as optim
from torch.nn import Transformer
from torchtext.vocab import build_vocab_from_iterator
from torchtext.data.utils import get_tokenizer
from torch.utils.data import DataLoader
import math
import spacy
from torchtext.datasets import Multi30k, multi30k

# 安装依赖（确保已执行）：
# pip install torch==2.2.2 torchtext==0.17.2 spacy
# python -m spacy download en_core_web_sm
# python -m spacy download de_core_news_sm

# 配置参数
BATCH_SIZE = 64
NUM_EPOCHS = 10
LEARNING_RATE = 0.0005
D_MODEL = 256
NUM_HEADS = 8
NUM_LAYERS = 3
FFN_DIM = 512
DROPOUT = 0.1
MAX_SEQ_LEN = 50

# 设备配置
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

# 初始化分词器
spacy_de = spacy.load('de_core_news_sm')
spacy_en = spacy.load('en_core_web_sm')

def de_tokenizer(text):
    return [tok.text for tok in spacy_de.tokenizer(text)]

def en_tokenizer(text):
    return [tok.text for tok in spacy_en.tokenizer(text)]

# 加载数据集（新版API）
train_iter = Multi30k(split='train', language_pair=('de', 'en'))
valid_iter = Multi30k(split='valid', language_pair=('de', 'en'))

# 构建词汇表
def yield_tokens(data_iter, is_src):
    for src, tgt in data_iter:
        yield de_tokenizer(src) if is_src else en_tokenizer(tgt)

src_vocab = build_vocab_from_iterator(
    yield_tokens(train_iter, is_src=True),
    min_freq=2,
    specials=['<unk>', '<pad>', '<sos>', '<eos>']
)
tgt_vocab = build_vocab_from_iterator(
    yield_tokens(train_iter, is_src=False),
    min_freq=2,
    specials=['<unk>', '<pad>', '<sos>', '<eos>']
)
src_vocab.set_default_index(src_vocab['<unk>'])
tgt_vocab.set_default_index(tgt_vocab['<unk>'])

# 数据处理函数
def process_data(src_text, tgt_text):
    src_tokens = ['<sos>'] + de_tokenizer(src_text) + ['<eos>']
    tgt_tokens = ['<sos>'] + en_tokenizer(tgt_text) + ['<eos>']

    # 截断或填充
    src_ids = [src_vocab[token] for token in src_tokens[:MAX_SEQ_LEN]] + \
              [src_vocab['<pad>']] * (MAX_SEQ_LEN - len(src_tokens))
    tgt_ids = [tgt_vocab[token] for token in tgt_tokens[:MAX_SEQ_LEN]] + \
              [tgt_vocab['<pad>']] * (MAX_SEQ_LEN - len(tgt_tokens))

    return torch.tensor(src_ids[:MAX_SEQ_LEN], dtype=torch.long), \
           torch.tensor(tgt_ids[:MAX_SEQ_LEN], dtype=torch.long)

# 创建数据加载器
def collate_batch(batch):
    src_batch, tgt_batch = [], []
    for src, tgt in batch:
        src_tensor, tgt_tensor = process_data(src, tgt)
        src_batch.append(src_tensor)
        tgt_batch.append(tgt_tensor)
    return torch.stack(src_batch).T.to(device), torch.stack(tgt_batch).T.to(device)

train_loader = DataLoader(
    list(train_iter),
    batch_size=BATCH_SIZE,
    collate_fn=collate_batch,
    shuffle=True
)

valid_loader = DataLoader(
    list(valid_iter),
    batch_size=BATCH_SIZE,
    collate_fn=collate_batch
)

# Transformer模型定义
class Seq2SeqTransformer(nn.Module):
    def __init__(self):
        super().__init__()
        self.src_embed = nn.Embedding(len(src_vocab), D_MODEL, padding_idx=src_vocab['<pad>'])
        self.trg_embed = nn.Embedding(len(tgt_vocab), D_MODEL, padding_idx=tgt_vocab['<pad>'])
        self.pos_encoder = PositionalEncoding(D_MODEL, DROPOUT)
        self.transformer = Transformer(
            d_model=D_MODEL,
            nhead=NUM_HEADS,
            num_encoder_layers=NUM_LAYERS,
            num_decoder_layers=NUM_LAYERS,
            dim_feedforward=FFN_DIM,
            dropout=DROPOUT
        )
        self.fc_out = nn.Linear(D_MODEL, len(tgt_vocab))

    def forward(self, src, tgt):
        src = self.pos_encoder(self.src_embed(src) * math.sqrt(D_MODEL))
        tgt = self.pos_encoder(self.trg_embed(tgt) * math.sqrt(D_MODEL))

        tgt_mask = self.transformer.generate_square_subsequent_mask(tgt.size(0)).to(device)

        output = self.transformer(
            src, tgt,
            tgt_mask=tgt_mask,
            src_key_padding_mask=(src == src_vocab['<pad>']).T,
            tgt_key_padding_mask=(tgt == tgt_vocab['<pad>']).T
        )

        return self.fc_out(output)

class PositionalEncoding(nn.Module):
    def __init__(self, d_model, dropout, max_len=5000):
        super().__init__()
        self.dropout = nn.Dropout(p=dropout)
        pe = torch.zeros(max_len, d_model)
        position = torch.arange(0, max_len, dtype=torch.float).unsqueeze(1)
        div_term = torch.exp(torch.arange(0, d_model, 2).float() * (-math.log(10000.0) / d_model))
        pe[:, 0::2] = torch.sin(position * div_term)
        pe[:, 1::2] = torch.cos(position * div_term)
        self.register_buffer('pe', pe)

    def forward(self, x):
        x = x + self.pe[:x.size(0), :]
        return self.dropout(x)

# 初始化模型
model = Seq2SeqTransformer().to(device)
optimizer = optim.Adam(model.parameters(), lr=LEARNING_RATE)
criterion = nn.CrossEntropyLoss(ignore_index=tgt_vocab['<pad>'])

# 训练函数
def train_epoch(model, loader, optimizer, criterion):
    model.train()
    total_loss = 0

    for src, tgt in loader:
        optimizer.zero_grad()
        output = model(src, tgt[:-1, :])  # 使用teacher forcing

        loss = criterion(
            output.reshape(-1, output.shape[-1]),
            tgt[1:, :].reshape(-1)  # 预测下一个token
        )

        loss.backward()
        torch.nn.utils.clip_grad_norm_(model.parameters(), max_norm=1)
        optimizer.step()
        total_loss += loss.item()

    return total_loss / len(loader)

# 验证函数
def evaluate(model, loader, criterion):
    model.eval()
    total_loss = 0

    with torch.no_grad():
        for src, tgt in loader:
            output = model(src, tgt[:-1, :])
            loss = criterion(
                output.reshape(-1, output.shape[-1]),
                tgt[1:, :].reshape(-1)
            )
            total_loss += loss.item()

    return total_loss / len(loader)

# 训练循环
for epoch in range(NUM_EPOCHS):
    train_loss = train_epoch(model, train_loader, optimizer, criterion)
    valid_loss = evaluate(model, valid_loader, criterion)

    print(f"Epoch: {epoch+1:02}")
    print(f"\tTrain Loss: {train_loss:.3f}")
    print(f"\t Val. Loss: {valid_loss:.3f}")
