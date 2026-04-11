import os
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser
from dotenv import load_dotenv

load_dotenv()

# 使用 DeepSeek API
llm = ChatOpenAI(
    model="deepseek-chat",
    temperature=0,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


def simple_approach(product_name: str) -> str:
    """
    简单方式：一次性让 LLM 完成所有任务
    缺点：任务复杂时，容易遗漏细节，输出质量不稳定
    """
    prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个专业的产品评测作家。"),
        ("human", """请为产品 '{product_name}' 撰写一篇完整的评测文章，要求：
1. 提取产品的3-5个核心特性
2. 分析每个特性的优缺点
3. 给出综合评分（1-10分）
4. 撰写一段总结性的推荐语

请直接输出完整的评测文章。""")
    ])

    chain = prompt | llm | StrOutputParser()
    return chain.invoke({"product_name": product_name})


def prompt_chaining_approach(product_name: str) -> str:
    """
    Prompt Chaining 方式：将复杂任务拆分为多个简单步骤
    优点：每个步骤专注单一任务，结果更可靠、更稳定
    """

    # 步骤1：提取产品核心特性
    step1_prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个产品分析师，擅长提取产品核心特性。"),
        ("human", "请分析产品 '{product_name}'，列出3-5个核心特性，每个特性用一句话简要描述。只输出特性列表。")
    ])
    step1_chain = step1_prompt | llm | StrOutputParser()
    features = step1_chain.invoke({"product_name": product_name})

    # 步骤2：分析每个特性的优缺点
    step2_prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个产品评论家，擅长客观分析产品优缺点。"),
        ("human", """基于以下产品特性，为每个特性分析其优点和缺点：

产品：{product_name}
特性列表：
{features}

请为每个特性分别列出优点和缺点。""")
    ])
    step2_chain = step2_prompt | llm | StrOutputParser()
    analysis = step2_chain.invoke({
        "product_name": product_name,
        "features": features
    })

    # 步骤3：给出综合评分
    step3_prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个评分专家，根据分析给出公正的评分。"),
        ("human", """基于以下产品分析，给出综合评分（1-10分）：

产品：{product_name}
分析内容：
{analysis}

请给出评分并简要说明评分理由。""")
    ])
    step3_chain = step3_prompt | llm | StrOutputParser()
    rating = step3_chain.invoke({
        "product_name": product_name,
        "analysis": analysis
    })

    # 步骤4：撰写推荐语
    step4_prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个营销文案撰写专家。"),
        ("human", """基于以下信息，撰写一段总结性的推荐语（100字以内）：

产品：{product_name}
特性：{features}
分析：{analysis}
评分：{rating}

请撰写吸引人的推荐语。""")
    ])
    step4_chain = step4_prompt | llm | StrOutputParser()
    recommendation = step4_chain.invoke({
        "product_name": product_name,
        "features": features,
        "analysis": analysis,
        "rating": rating
    })

    # 组合最终结果
    final_result = f"""# {product_name} 评测报告

## 核心特性
{features}

## 详细分析
{analysis}

## 综合评分
{rating}

## 推荐语
{recommendation}
"""
    return final_result


def main():
    product = "iPhone 15 Pro"

    print("=" * 60)
    print("方式一：简单方式（一次性完成）")
    print("=" * 60)
    simple_result = simple_approach(product)
    print(simple_result)

    print("\n" + "=" * 60)
    print("方式二：Prompt Chaining（分步骤完成）")
    print("=" * 60)
    chaining_result = prompt_chaining_approach(product)
    print(chaining_result)

    print("\n" + "=" * 60)
    print("对比总结")
    print("=" * 60)
    print("""
【简单方式】
- 优点：代码简单，一次调用
- 缺点：任务复杂时，LLM容易遗漏要求，输出不稳定
- 适用：简单、单一的任务

【Prompt Chaining 方式】
- 优点：
  1. 每个步骤专注单一任务，结果更可靠
  2. 可以检查中间结果，便于调试
  3. 某一步失败可以重试，不影响其他步骤
  4. 输出结构更清晰、更稳定
- 缺点：需要多次调用 LLM
- 适用：复杂、多步骤的任务

Prompt Chaining 是 Agentic Design Patterns 中的基础模式，
通过任务分解提高 LLM 输出的可靠性和稳定性。
""")


if __name__ == "__main__":
    main()
