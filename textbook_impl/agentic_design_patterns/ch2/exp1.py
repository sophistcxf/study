"""
Chapter 2: Routing Pattern (路由模式)

路由模式是 Agentic Design Patterns 中的核心模式之一。
它根据输入内容的类型或特征，将请求路由到不同的专门处理链/智能体。

核心思想：
- 不用一个通用模型处理所有任务
- 而是根据任务类型，路由到专门的子模型
- 每个子模型专注于特定领域，效果更好

示例场景：客户服务平台，根据问题类型路由到不同专家
"""

import os
from typing import Literal
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser
from pydantic import BaseModel, Field
from dotenv import load_dotenv

load_dotenv()

# 初始化 LLM
llm = ChatOpenAI(
    model="deepseek-chat",
    temperature=0,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


# ============ 1. 定义路由分类器 ============

class QueryClassifier(BaseModel):
    """用于分类用户查询类型的数据模型"""
    query_type: Literal["technical", "billing", "general", "complaint"] = Field(
        description="查询类型: technical(技术支持), billing(账单), general(一般咨询), complaint(投诉)"
    )
    urgency: Literal["low", "medium", "high"] = Field(
        description="紧急程度: low(低), medium(中), high(高)"
    )
    reasoning: str = Field(
        description="分类理由的简要说明"
    )


# 使用提示词实现分类（兼容 DeepSeek API）
classifier_prompt = ChatPromptTemplate.from_messages([
    ("system", """你是一个智能客服查询分类器。分析用户查询，确定其类型和紧急程度。

请严格按照以下格式输出（不要添加其他内容）：
query_type: <类型>
urgency: <紧急程度>
reasoning: <分类理由>

类型必须是以下之一：technical, billing, general, complaint
紧急程度必须是以下之一：low, medium, high"""),
    ("human", "请分析以下客户查询：\n\n{query}")
])

classifier_chain = classifier_prompt | llm | StrOutputParser()


def parse_classification(output: str) -> QueryClassifier:
    """解析分类器的文本输出为结构化数据"""
    result = {"query_type": "general", "urgency": "low", "reasoning": ""}

    for line in output.strip().split('\n'):
        if ':' in line:
            key, value = line.split(':', 1)
            key = key.strip()
            value = value.strip()
            if key in result:
                result[key] = value

    return QueryClassifier(**result)


# ============ 2. 定义各个专业处理链 ============

# 技术支持专家
technical_prompt = ChatPromptTemplate.from_messages([
    ("system", """你是技术支持专家，擅长解决产品技术问题。
请提供清晰、分步骤的技术解决方案。如果问题复杂，请提供排查步骤。"""),
    ("human", "客户问题：{query}")
])
technical_chain = technical_prompt | llm | StrOutputParser()

# 账单支持专家
billing_prompt = ChatPromptTemplate.from_messages([
    ("system", """你是账单支持专家，擅长处理账单、付款、退款相关问题。
请清晰解释费用明细，并提供具体的解决方案。"""),
    ("human", "客户问题：{query}")
])
billing_chain = billing_prompt | llm | StrOutputParser()

# 一般咨询专家
general_prompt = ChatPromptTemplate.from_messages([
    ("system", """你是客户服务代表，擅长回答一般性产品咨询。
请提供友好、有帮助的回答，并适当介绍相关产品功能。"""),
    ("human", "客户问题：{query}")
])
general_chain = general_prompt | llm | StrOutputParser()

# 投诉处理专家
complaint_prompt = ChatPromptTemplate.from_messages([
    ("system", """你是投诉处理专家，擅长处理客户不满和投诉。
请先表达理解和歉意，然后提供具体的解决方案或补偿措施。"""),
    ("human", "客户问题：{query}")
])
complaint_chain = complaint_prompt | llm | StrOutputParser()


# ============ 3. 路由函数 ============

def route_query(query: str, classification: QueryClassifier) -> str:
    """
    根据分类结果，将查询路由到相应的处理链
    """
    # 根据紧急程度添加前缀
    urgency_prefix = {
        "high": "【紧急】",
        "medium": "",
        "low": ""
    }.get(classification.urgency, "")

    # 路由到对应的专家
    if classification.query_type == "technical":
        response = technical_chain.invoke({"query": query})
        return f"{urgency_prefix}[技术支持] {response}"

    elif classification.query_type == "billing":
        response = billing_chain.invoke({"query": query})
        return f"{urgency_prefix}[账单支持] {response}"

    elif classification.query_type == "complaint":
        response = complaint_chain.invoke({"query": query})
        return f"{urgency_prefix}[投诉处理] {response}"

    else:  # general
        response = general_chain.invoke({"query": query})
        return f"{urgency_prefix}[一般咨询] {response}"


# ============ 4. 完整的路由系统 ============

def customer_service(query: str) -> dict:
    """
    完整的客服路由系统
    """
    # 步骤1：分类查询
    classification_output = classifier_chain.invoke({"query": query})
    classification = parse_classification(classification_output)

    # 步骤2：路由到对应专家
    response = route_query(query, classification)

    return {
        "query": query,
        "classification": classification,
        "response": response
    }


# ============ 5. 对比：不使用路由的简单方式 ============

def simple_approach(query: str) -> str:
    """
    简单方式：用一个通用模型处理所有查询
    """
    prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个客服代表，回答客户问题。"),
        ("human", "{query}")
    ])
    chain = prompt | llm | StrOutputParser()
    return chain.invoke({"query": query})


# ============ 6. 演示 ============

def main():
    # 测试用例
    test_queries = [
        "我的应用一直崩溃，错误代码是 0x80070005，怎么解决？",
        "为什么我这个月被扣了 299 元？我要申请退款！",
        "你们的产品有哪些主要功能？",
        "你们的服务太差了！我等了3天没人回复，要求全额退款并赔偿！"
    ]

    print("=" * 70)
    print("Routing Pattern 示例 - 智能客服路由系统")
    print("=" * 70)

    for i, query in enumerate(test_queries, 1):
        print(f"\n{'─' * 70}")
        print(f"测试案例 {i}: {query}")
        print('─' * 70)

        # 路由方式
        result = customer_service(query)

        print(f"\n📊 分类结果:")
        print(f"   类型: {result['classification'].query_type}")
        print(f"   紧急度: {result['classification'].urgency}")
        print(f"   理由: {result['classification'].reasoning}")

        print(f"\n🤖 路由响应:\n{result['response'][:500]}...")

    # 对比简单方式
    print("\n" + "=" * 70)
    print("对比：简单方式（不使用路由）")
    print("=" * 70)

    test_query = test_queries[0]
    print(f"\n查询: {test_query}")
    simple_response = simple_approach(test_query)
    print(f"\n简单方式响应:\n{simple_response[:500]}...")

    # 总结
    print("\n" + "=" * 70)
    print("Routing Pattern 优势总结")
    print("=" * 70)
    print("""
【核心优势】

1. **专业化处理**
   - 每个子模型专注于特定领域
   - 回答更专业、更深入

2. **可扩展性**
   - 新增业务类型只需添加新的处理链
   - 不影响现有路由逻辑

3. **灵活控制**
   - 可以根据分类结果做不同处理
   - 如：紧急投诉自动升级给人工

4. **成本优化**
   - 简单问题可以用轻量级模型
   - 复杂问题才用更强的模型

【适用场景】
- 多领域客服系统
- 内容审核（不同违规类型不同处理）
- 多语言翻译（先识别语言再翻译）
- 文档处理（根据类型选择不同解析器）
""")


if __name__ == "__main__":
    main()
