"""
Chapter 8: Memory Management Pattern (记忆管理模式)

记忆管理是 Agentic Design Patterns 中至关重要的基础设施。
它使智能体能够记住过去的交互、学习用户偏好，并在长期对话中保持一致性。

核心概念：
- **Short-term Memory (短期记忆)**: 当前对话的上下文，随会话结束而消失
- **Long-term Memory (长期记忆)**: 跨会话持久化存储的知识和用户信息

两种记忆的区别：
| 特性 | 短期记忆 | 长期记忆 |
|------|----------|----------|
| 存储时长 | 会话期间 | 永久/长期 |
| 存储内容 | 对话历史 | 用户画像、事实知识 |
| 检索方式 | 完整加载 | 语义检索 |
| 实现方式 | 列表/Buffer | 向量数据库 |

本示例实现：
1. ConversationBufferMemory - 基于列表的短期记忆
2. VectorStoreMemory - 基于向量数据库的长期记忆
3. HybridMemory - 结合两种记忆的混合记忆系统
"""

import os
import json
import hashlib
from typing import List, Dict, Optional, Any, Callable
from dataclasses import dataclass, field, asdict
from datetime import datetime
from enum import Enum
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate, MessagesPlaceholder
from langchain_core.output_parsers import StrOutputParser
from langchain_core.messages import HumanMessage, AIMessage, SystemMessage, BaseMessage
from dotenv import load_dotenv

load_dotenv()

# 初始化 LLM
llm = ChatOpenAI(
    model="deepseek-chat",
    temperature=0,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


def simple_hash_embedding(text: str, dim: int = 128) -> List[float]:
    """
    基于哈希的简单文本向量化（用于演示）
    
    生产环境应使用：
    - OpenAIEmbeddings
    - HuggingFaceEmbeddings
    - 或其他专业 embedding 模型
    
    这里使用简单的哈希方法，仅用于演示长期记忆的语义检索概念。
    """
    import hashlib
    import math
    
    # 使用多个哈希函数生成向量
    vector = [0.0] * dim
    
    # 基于字符 n-gram 的哈希
    for i in range(len(text) - 1):
        bigram = text[i:i+2].encode('utf-8')
        hash_val = int(hashlib.md5(bigram).hexdigest(), 16)
        vector[hash_val % dim] += 1.0
    
    # 归一化
    norm = math.sqrt(sum(x * x for x in vector))
    if norm > 0:
        vector = [x / norm for x in vector]
    
    return vector


# ============ 1. 短期记忆 (Short-term Memory) ============

@dataclass
class ConversationTurn:
    """对话回合"""
    role: str  # "human" 或 "ai"
    content: str
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())
    metadata: Dict[str, Any] = field(default_factory=dict)


class ConversationBufferMemory:
    """
    对话缓冲区记忆 - 短期记忆实现
    
    特点：
    - 保存当前会话的完整对话历史
    - 支持窗口限制（防止上下文过长）
    - 可直接用于 LangChain 的 prompt 构建
    
    适用场景：
    - 当前对话的上下文理解
    - 多轮问答的连贯性
    """

    def __init__(self, max_turns: int = 10):
        self.buffer: List[ConversationTurn] = []
        self.max_turns = max_turns
        self.session_id: str = self._generate_session_id()

    def _generate_session_id(self) -> str:
        """生成会话ID"""
        return hashlib.md5(datetime.now().isoformat().encode()).hexdigest()[:8]

    def add_human_message(self, content: str, metadata: Dict = None):
        """添加用户消息"""
        self.buffer.append(ConversationTurn(
            role="human",
            content=content,
            metadata=metadata or {}
        ))
        self._trim_buffer()

    def add_ai_message(self, content: str, metadata: Dict = None):
        """添加 AI 回复"""
        self.buffer.append(ConversationTurn(
            role="ai",
            content=content,
            metadata=metadata or {}
        ))
        self._trim_buffer()

    def _trim_buffer(self):
        """修剪缓冲区，保持最大回合数"""
        if len(self.buffer) > self.max_turns * 2:  # *2 因为每轮包含 human + ai
            self.buffer = self.buffer[-self.max_turns * 2:]

    def get_history(self, last_n: int = None) -> List[ConversationTurn]:
        """获取对话历史"""
        if last_n:
            return self.buffer[-last_n:]
        return self.buffer

    def to_langchain_messages(self) -> List[BaseMessage]:
        """转换为 LangChain 消息格式"""
        messages = []
        for turn in self.buffer:
            if turn.role == "human":
                messages.append(HumanMessage(content=turn.content))
            elif turn.role == "ai":
                messages.append(AIMessage(content=turn.content))
        return messages

    def clear(self):
        """清空记忆"""
        self.buffer = []

    def get_summary(self) -> str:
        """获取对话摘要"""
        if not self.buffer:
            return "(无对话历史)"
        return f"当前会话 {self.session_id}: 共 {len(self.buffer)} 条消息"


# ============ 2. 长期记忆 (Long-term Memory) ============

@dataclass
class MemoryEntry:
    """记忆条目"""
    content: str                    # 记忆内容
    memory_type: str               # 记忆类型: "fact", "preference", "event", "knowledge"
    source: str                    # 来源: 会话ID或系统
    timestamp: str = field(default_factory=lambda: datetime.now().isoformat())
    importance: float = 1.0        # 重要性评分 0-1
    tags: List[str] = field(default_factory=list)
    id: str = field(default_factory=lambda: hashlib.md5(
        datetime.now().isoformat().encode()
    ).hexdigest()[:12])


class SimpleVectorStore:
    """
    简易向量存储 - 用于演示长期记忆
    
    生产环境应使用：
    - ChromaDB
    - Pinecone
    - Weaviate
    - Milvus
    """

    def __init__(self, collection_name: str = "long_term_memory"):
        self.collection_name = collection_name
        self.entries: List[MemoryEntry] = []
        self.vectors: List[List[float]] = []
        self._embedding_cache: Dict[str, List[float]] = {}

    def add(self, entry: MemoryEntry) -> str:
        """添加记忆条目"""
        # 生成向量
        vector = self._get_embedding(entry.content)
        
        self.entries.append(entry)
        self.vectors.append(vector)
        
        return entry.id

    def _get_embedding(self, text: str) -> List[float]:
        """获取文本的向量表示（带缓存）"""
        cache_key = hashlib.md5(text.encode()).hexdigest()
        if cache_key not in self._embedding_cache:
            # 使用简单的哈希 embedding（演示用）
            self._embedding_cache[cache_key] = simple_hash_embedding(text)
        return self._embedding_cache[cache_key]

    def search(self, query: str, top_k: int = 3) -> List[MemoryEntry]:
        """语义搜索相关记忆"""
        if not self.entries:
            return []
        
        query_vector = self._get_embedding(query)
        
        # 计算余弦相似度
        similarities = []
        for i, vec in enumerate(self.vectors):
            similarity = self._cosine_similarity(query_vector, vec)
            similarities.append((i, similarity))
        
        # 按相似度排序
        similarities.sort(key=lambda x: x[1], reverse=True)
        
        # 返回 top_k 结果
        results = []
        for idx, score in similarities[:top_k]:
            entry = self.entries[idx]
            results.append(entry)
        
        return results

    def _cosine_similarity(self, a: List[float], b: List[float]) -> float:
        """计算余弦相似度"""
        import math
        dot_product = sum(x * y for x, y in zip(a, b))
        norm_a = math.sqrt(sum(x * x for x in a))
        norm_b = math.sqrt(sum(x * x for x in b))
        if norm_a == 0 or norm_b == 0:
            return 0
        return dot_product / (norm_a * norm_b)

    def get_by_type(self, memory_type: str) -> List[MemoryEntry]:
        """按类型获取记忆"""
        return [e for e in self.entries if e.memory_type == memory_type]

    def get_all(self) -> List[MemoryEntry]:
        """获取所有记忆"""
        return self.entries

    def delete(self, entry_id: str) -> bool:
        """删除记忆"""
        for i, entry in enumerate(self.entries):
            if entry.id == entry_id:
                self.entries.pop(i)
                self.vectors.pop(i)
                return True
        return False


class LongTermMemory:
    """
    长期记忆管理器
    
    功能：
    - 存储跨会话的用户信息
    - 语义检索相关记忆
    - 自动提取和存储关键信息
    
    记忆类型：
    - fact: 用户相关的事实信息
    - preference: 用户偏好设置
    - event: 重要事件记录
    - knowledge: 领域知识
    """

    def __init__(self, user_id: str):
        self.user_id = user_id
        self.vector_store = SimpleVectorStore(f"user_{user_id}_memory")
        self.extraction_prompt = ChatPromptTemplate.from_messages([
            ("system", """你是一个信息提取专家。从对话中提取值得长期保存的信息。

提取规则：
1. 用户的基本信息（姓名、职业、所在地等）
2. 用户的明确偏好（喜欢/不喜欢）
3. 用户提到的关键事实
4. 重要的约定或计划

输出格式（JSON）：
{{
    "extracted_facts": [
        {{
            "content": "提取的信息",
            "type": "fact|preference|event|knowledge",
            "importance": 0.8
        }}
    ]
}}

如果没有值得提取的信息，返回空数组。"""),
            ("human", "对话内容:\n{conversation}")
        ])
        self.extraction_chain = self.extraction_prompt | llm | StrOutputParser()

    def remember(self, content: str, memory_type: str = "fact", 
                 importance: float = 1.0, tags: List[str] = None):
        """主动存储记忆"""
        entry = MemoryEntry(
            content=content,
            memory_type=memory_type,
            source=f"user_{self.user_id}",
            importance=importance,
            tags=tags or []
        )
        return self.vector_store.add(entry)

    def recall(self, query: str, top_k: int = 3) -> List[MemoryEntry]:
        """回忆相关记忆"""
        return self.vector_store.search(query, top_k)

    def extract_and_store(self, conversation: str, session_id: str = None):
        """从对话中提取并存储信息"""
        try:
            result = self.extraction_chain.invoke({"conversation": conversation})
            
            # 尝试解析 JSON
            import re
            json_match = re.search(r'\{.*\}', result, re.DOTALL)
            if json_match:
                data = json.loads(json_match.group())
                facts = data.get("extracted_facts", [])
                
                stored_ids = []
                for fact in facts:
                    entry = MemoryEntry(
                        content=fact["content"],
                        memory_type=fact.get("type", "fact"),
                        source=session_id or f"user_{self.user_id}",
                        importance=fact.get("importance", 0.5),
                        tags=["auto_extracted"]
                    )
                    stored_ids.append(self.vector_store.add(entry))
                
                return stored_ids
        except Exception as e:
            print(f"提取记忆时出错: {e}")
            return []

    def get_user_profile(self) -> Dict:
        """获取用户画像摘要"""
        facts = self.vector_store.get_by_type("fact")
        preferences = self.vector_store.get_by_type("preference")
        
        return {
            "user_id": self.user_id,
            "total_memories": len(self.vector_store.get_all()),
            "facts": [f.content for f in facts],
            "preferences": [p.content for p in preferences]
        }


# ============ 3. 混合记忆系统 ============

class HybridMemoryAgent:
    """
    混合记忆智能体
    
    结合短期记忆和长期记忆的优势：
    - 短期记忆：保持当前对话的连贯性
    - 长期记忆：提供跨会话的个性化体验
    
    工作流程：
    1. 接收用户输入
    2. 从长期记忆中检索相关信息
    3. 结合短期对话历史生成回复
    4. 将新信息存入短期记忆
    5. 定期提取重要信息存入长期记忆
    """

    def __init__(self, user_id: str):
        self.user_id = user_id
        self.short_term = ConversationBufferMemory(max_turns=10)
        self.long_term = LongTermMemory(user_id)
        self._init_prompt()

    def _init_prompt(self):
        """初始化提示词模板"""
        self.prompt = ChatPromptTemplate.from_messages([
            ("system", """你是一个有记忆的 AI 助手。你可以记住用户的信息，提供个性化的回复。

【用户画像】
{user_profile}

【相关记忆】
{relevant_memories}

请基于以上信息，结合对话历史，提供有帮助的回复。"""),
            MessagesPlaceholder(variable_name="chat_history"),
            ("human", "{input}")
        ])
        self.chain = self.prompt | llm | StrOutputParser()

    def chat(self, user_input: str, verbose: bool = False) -> str:
        """
        与用户对话
        
        Args:
            user_input: 用户输入
            verbose: 是否打印记忆检索过程
        
        Returns:
            AI 回复
        """
        # 1. 从长期记忆中检索相关信息
        relevant_memories = self.long_term.recall(user_input, top_k=3)
        memories_text = "\n".join([
            f"- [{m.memory_type}] {m.content}" 
            for m in relevant_memories
        ]) if relevant_memories else "(无相关记忆)"

        # 2. 获取用户画像
        profile = self.long_term.get_user_profile()
        profile_text = f"用户ID: {profile['user_id']}\n"
        if profile['facts']:
            profile_text += f"已知信息: {', '.join(profile['facts'][:3])}"
        else:
            profile_text += "(新用户，暂无画像)"

        if verbose:
            print(f"\n[记忆检索] 查询: '{user_input}'")
            print(f"[记忆检索] 找到 {len(relevant_memories)} 条相关记忆")

        # 3. 生成回复
        chat_history = self.short_term.to_langchain_messages()
        
        response = self.chain.invoke({
            "user_profile": profile_text,
            "relevant_memories": memories_text,
            "chat_history": chat_history,
            "input": user_input
        })

        # 4. 更新短期记忆
        self.short_term.add_human_message(user_input)
        self.short_term.add_ai_message(response)

        # 5. 定期提取长期记忆（每5轮对话）
        if len(self.short_term.buffer) % 10 == 0:  # 每5轮（10条消息）
            self._extract_to_long_term()

        return response

    def _extract_to_long_term(self):
        """将对话提取到长期记忆"""
        # 获取最近的对话
        recent = self.short_term.get_history(last_n=6)
        conversation = "\n".join([
            f"{'用户' if t.role == 'human' else 'AI'}: {t.content}" 
            for t in recent
        ])
        
        # 提取并存储
        stored = self.long_term.extract_and_store(conversation, self.short_term.session_id)
        if stored:
            print(f"[记忆提取] 已保存 {len(stored)} 条长期记忆")

    def actively_remember(self, content: str, memory_type: str = "fact", 
                          importance: float = 1.0):
        """主动记住某些信息"""
        self.long_term.remember(content, memory_type, importance)
        print(f"[主动记忆] 已记录: {content}")

    def get_memory_status(self) -> Dict:
        """获取记忆状态报告"""
        return {
            "user_id": self.user_id,
            "session_id": self.short_term.session_id,
            "short_term": {
                "message_count": len(self.short_term.buffer),
                "max_turns": self.short_term.max_turns,
                "summary": self.short_term.get_summary()
            },
            "long_term": self.long_term.get_user_profile()
        }


# ============ 4. 演示场景 ============

def demo_short_term_memory():
    """演示短期记忆功能"""
    print("=" * 80)
    print("演示 1: 短期记忆 (Conversation Buffer Memory)")
    print("=" * 80)

    memory = ConversationBufferMemory(max_turns=3)

    # 模拟多轮对话
    conversations = [
        ("human", "你好，我叫张三"),
        ("ai", "你好张三！很高兴认识你。有什么我可以帮助你的吗？"),
        ("human", "我想学习 Python 编程"),
        ("ai", "Python 是一门非常优秀的编程语言。我可以帮你制定学习计划。"),
        ("human", "你能推荐一些学习资源吗？"),
        ("ai", "当然可以！推荐《Python编程：从入门到实践》这本书。"),
        ("human", "好的，我会去看看"),
        ("ai", "太好了！有任何问题随时问我。"),
    ]

    print("\n模拟对话（max_turns=3，保留最近3轮）：")
    for role, content in conversations:
        if role == "human":
            memory.add_human_message(content)
            print(f"用户: {content}")
        else:
            memory.add_ai_message(content)
            print(f"AI: {content}")

    print(f"\n{memory.get_summary()}")
    print("\n当前记忆内容（应只保留最近3轮）：")
    for turn in memory.get_history():
        print(f"  [{turn.role}] {turn.content}")


def demo_long_term_memory():
    """演示长期记忆功能"""
    print("\n" + "=" * 80)
    print("演示 2: 长期记忆 (Long-term Memory with Vector Store)")
    print("=" * 80)

    ltm = LongTermMemory(user_id="demo_user")

    # 手动添加一些记忆
    memories = [
        ("用户叫李四，是一名软件工程师", "fact", 0.9),
        ("用户喜欢使用 Python 和 TypeScript", "preference", 0.8),
        ("用户住在上海", "fact", 0.7),
        ("用户对机器学习很感兴趣", "preference", 0.85),
        ("用户上周完成了一个 LLM 项目", "event", 0.6),
    ]

    print("\n添加长期记忆：")
    for content, mtype, importance in memories:
        mid = ltm.remember(content, mtype, importance)
        print(f"  [{mtype}] {content} (重要性: {importance})")

    # 测试语义检索
    queries = [
        "用户叫什么名字？",
        "用户喜欢什么编程语言？",
        "用户的工作是什么？",
        "用户对什么技术感兴趣？"
    ]

    print("\n语义检索测试：")
    for query in queries:
        results = ltm.recall(query, top_k=2)
        print(f"\n  查询: '{query}'")
        for r in results:
            print(f"    → [{r.memory_type}] {r.content}")

    # 显示用户画像
    print("\n用户画像：")
    profile = ltm.get_user_profile()
    print(f"  用户ID: {profile['user_id']}")
    print(f"  总记忆数: {profile['total_memories']}")
    print(f"  事实: {profile['facts']}")
    print(f"  偏好: {profile['preferences']}")


def demo_hybrid_memory():
    """演示混合记忆系统"""
    print("\n" + "=" * 80)
    print("演示 3: 混合记忆系统 (Hybrid Memory Agent)")
    print("=" * 80)

    agent = HybridMemoryAgent(user_id="wangwu_001")

    # 先预设一些长期记忆
    agent.actively_remember("用户叫王五", "fact", 0.95)
    agent.actively_remember("用户是一名产品经理", "fact", 0.9)
    agent.actively_remember("用户喜欢简洁的界面设计", "preference", 0.8)

    print("\n开始多轮对话（观察记忆如何工作）：")
    print("-" * 80)

    # 模拟对话
    inputs = [
        "你好，我是王五",
        "我在设计一个新的产品功能",
        "你能帮我分析一下用户需求吗？",
    ]

    for user_input in inputs:
        print(f"\n用户: {user_input}")
        response = agent.chat(user_input, verbose=True)
        print(f"AI: {response[:200]}..." if len(response) > 200 else f"AI: {response}")

    # 显示记忆状态
    print("\n" + "-" * 80)
    print("当前记忆状态：")
    status = agent.get_memory_status()
    print(f"  会话ID: {status['session_id']}")
    print(f"  短期记忆消息数: {status['short_term']['message_count']}")
    print(f"  长期记忆总数: {status['long_term']['total_memories']}")


def demo_memory_comparison():
    """对比有无记忆的效果（概念演示）"""
    print("\n" + "=" * 80)
    print("演示 4: 对比 - 有记忆 vs 无记忆（概念演示）")
    print("=" * 80)

    # 有记忆的 Agent
    agent_with_memory = HybridMemoryAgent(user_id="comparison_demo")
    agent_with_memory.actively_remember("用户是素食主义者", "preference", 0.95)
    agent_with_memory.actively_remember("用户对坚果过敏", "fact", 1.0)

    # 测试场景：餐厅推荐
    question = "我想找一家适合我的餐厅，有什么推荐吗？"

    print(f"\n用户问题: '{question}'")
    
    print("\n【无记忆 Agent 的行为】")
    print("  - 不知道用户的饮食限制")
    print("  - 可能推荐含肉类或坚果的餐厅")
    print("  - 每次都需要用户重复说明")

    print("\n【有记忆 Agent 的行为】")
    # 检索相关记忆
    relevant = agent_with_memory.long_term.recall(question, top_k=2)
    print(f"  - 自动检索到 {len(relevant)} 条相关记忆:")
    for m in relevant:
        print(f"    → [{m.memory_type}] {m.content}")
    print("  - 会主动避开含坚果的食物")
    print("  - 推荐素食友好的餐厅")
    print("  - 提供个性化的用餐建议")


# ============ 5. 主函数 ============

def main():
    """主函数：运行所有演示"""
    print("=" * 80)
    print("Chapter 8: Memory Management Pattern (记忆管理模式)")
    print("=" * 80)

    # 运行各个演示
    demo_short_term_memory()
    demo_long_term_memory()
    demo_hybrid_memory()
    demo_memory_comparison()

    # 总结
    print("\n" + "=" * 80)
    print("Memory Management Pattern 总结")
    print("=" * 80)
    print("""
【核心概念】

1. **短期记忆 (Short-term Memory)**
   - 实现: ConversationBufferMemory
   - 作用: 保持当前对话的上下文连贯性
   - 特点: 会话结束即消失，快速访问
   - 适用: 多轮对话、上下文理解

2. **长期记忆 (Long-term Memory)**
   - 实现: VectorStoreMemory
   - 作用: 跨会话存储用户信息和知识
   - 特点: 持久化存储，语义检索
   - 适用: 用户画像、个性化服务、知识积累

3. **混合记忆 (Hybrid Memory)**
   - 结合短期和长期记忆的优势
   - 短期记忆提供即时上下文
   - 长期记忆提供历史知识
   - 实现真正的"有记忆"智能体

【最佳实践】

1. **短期记忆管理**
   - 设置合理的窗口大小（5-10轮）
   - 定期总结压缩历史
   - 区分系统消息和用户消息

2. **长期记忆优化**
   - 定期提取和存储关键信息
   - 为记忆设置重要性评分
   - 分类管理不同类型的记忆
   - 定期清理过期或低价值记忆

3. **检索策略**
   - 使用语义检索而非关键词匹配
   - 结合多种检索方式（相似度 + 时间 + 类型）
   - 设置检索结果数量上限
   - 对检索结果进行相关性排序

【生产环境建议】

1. **向量数据库选择**
   - ChromaDB: 轻量级，适合本地开发
   - Pinecone: 托管服务，易于扩展
   - Weaviate: 支持复杂查询
   - Milvus: 高性能，适合大规模数据

2. **记忆提取策略**
   - 使用 LLM 自动提取关键信息
   - 设置提取触发条件（轮数/时间）
   - 人工审核重要记忆
   - 支持用户主动添加记忆

3. **隐私与安全**
   - 敏感信息加密存储
   - 支持用户删除记忆
   - 遵守数据保留政策
   - 提供记忆透明度

【与其他模式结合】

- + Reflection: 基于记忆进行自我反思
- + Planning: 利用历史信息制定更好计划
- + Multi-Agent: 共享长期记忆实现协作
- + Tool Use: 记忆增强的工具选择
""")


if __name__ == "__main__":
    main()
