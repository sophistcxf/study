"""
Chapter 3: Parallelization Pattern (并行化模式)

并行化模式是 Agentic Design Patterns 中的重要模式。
它将任务拆分成多个子任务，并行执行，最后汇总结果。

核心思想：
- 将复杂任务分解为多个独立的子任务
- 并行执行这些子任务，提高效率
- 汇总所有子任务的结果，形成最终输出

两种主要形式：
1. Sectioning（分段）：将任务分成不同部分，每个部分并行处理
2. Voting（投票）：同一任务由多个智能体并行处理，汇总结果

示例场景：文章审核系统，从多个维度并行评估文章质量
"""

import os
import asyncio
from typing import List, Dict
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser
from dotenv import load_dotenv

load_dotenv()

# 初始化 LLM
llm = ChatOpenAI(
    model="deepseek-chat",
    temperature=0,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)


# ============ 1. Sectioning（分段）模式 ============

def create_section_chain(section_name: str, criteria: str):
    """创建一个专门评估特定维度的链"""
    prompt = ChatPromptTemplate.from_messages([
        ("system", f"""你是一个专业的文章审核员，专门负责评估文章的{section_name}。

评估标准：{criteria}

请给出：
1. 评分（1-10分）
2. 简要评价（50字以内）
3. 改进建议（如有）

输出格式：
评分: X分
评价: ...
建议: ..."""),
        ("human", "请评估以下文章：\n\n{article}")
    ])
    return prompt | llm | StrOutputParser()


# 创建多个专业评估链
grammar_chain = create_section_chain(
    "语法和拼写",
    "检查语法错误、拼写错误、标点符号使用"
)

structure_chain = create_section_chain(
    "结构和逻辑",
    "检查文章结构是否清晰、逻辑是否连贯、段落组织是否合理"
)

content_chain = create_section_chain(
    "内容和深度",
    "检查内容是否充实、观点是否有深度、论据是否充分"
)

style_chain = create_section_chain(
    "写作风格",
    "检查语言是否流畅、表达是否清晰、风格是否统一"
)


async def evaluate_article_sectioning(article: str) -> Dict[str, str]:
    """
    使用 Sectioning 模式并行评估文章的多个维度
    """
    # 并行执行所有评估任务
    results = await asyncio.gather(
        asyncio.to_thread(grammar_chain.invoke, {"article": article}),
        asyncio.to_thread(structure_chain.invoke, {"article": article}),
        asyncio.to_thread(content_chain.invoke, {"article": article}),
        asyncio.to_thread(style_chain.invoke, {"article": article}),
        return_exceptions=True
    )

    return {
        "语法和拼写": results[0] if not isinstance(results[0], Exception) else str(results[0]),
        "结构和逻辑": results[1] if not isinstance(results[1], Exception) else str(results[1]),
        "内容和深度": results[2] if not isinstance(results[2], Exception) else str(results[2]),
        "写作风格": results[3] if not isinstance(results[3], Exception) else str(results[3])
    }


# ============ 2. Voting（投票）模式 ============

def create_voter_chain(voter_persona: str):
    """创建一个具有特定视角的评审员"""
    prompt = ChatPromptTemplate.from_messages([
        ("system", f"""你是一个文章评审员。{voter_persona}

请阅读文章并给出：
1. 总体评分（1-10分）
2. 是否推荐发布（是/否）
3. 一句话评价

输出格式：
评分: X分
推荐: 是/否
评价: ..."""),
        ("human", "请评审以下文章：\n\n{article}")
    ])
    return prompt | llm | StrOutputParser()


# 创建多个不同视角的评审员
technical_reviewer = create_voter_chain(
    "你注重技术准确性、专业术语使用和逻辑严谨性。"
)

general_reviewer = create_voter_chain(
    "你代表普通读者，注重文章的可读性和易懂性。"
)

creative_reviewer = create_voter_chain(
    "你注重文章的创意性、独特性和吸引力。"
)


async def evaluate_article_voting(article: str) -> Dict:
    """
    使用 Voting 模式，多个评审员并行评审同一文章
    """
    # 并行执行所有评审
    results = await asyncio.gather(
        asyncio.to_thread(technical_reviewer.invoke, {"article": article}),
        asyncio.to_thread(general_reviewer.invoke, {"article": article}),
        asyncio.to_thread(creative_reviewer.invoke, {"article": article}),
        return_exceptions=True
    )

    reviews = {
        "技术视角": results[0] if not isinstance(results[0], Exception) else str(results[0]),
        "读者视角": results[1] if not isinstance(results[1], Exception) else str(results[1]),
        "创意视角": results[2] if not isinstance(results[2], Exception) else str(results[2])
    }

    # 汇总投票结果
    summary_prompt = ChatPromptTemplate.from_messages([
        ("system", "你是一个评审汇总专家。根据多位评审员的意见，给出最终评审结论。"),
        ("human", """请根据以下评审意见，给出最终结论：

技术视角评审：
{technical_review}

读者视角评审：
{general_review}

创意视角评审：
{creative_review}

请给出：
1. 综合评分
2. 最终建议（通过/修改后通过/拒绝）
3. 主要修改建议""")
    ])

    summary_chain = summary_prompt | llm | StrOutputParser()
    final_decision = await asyncio.to_thread(
        summary_chain.invoke,
        {
            "technical_review": reviews["技术视角"],
            "general_review": reviews["读者视角"],
            "creative_review": reviews["创意视角"]
        }
    )

    return {
        "individual_reviews": reviews,
        "final_decision": final_decision
    }


# ============ 3. 串行方式（对比） ============

def evaluate_article_sequential(article: str) -> Dict[str, str]:
    """
    串行方式：依次评估各个维度
    用于对比并行化的性能优势
    """
    results = {}

    # 依次执行，非并行
    results["语法和拼写"] = grammar_chain.invoke({"article": article})
    results["结构和逻辑"] = structure_chain.invoke({"article": article})
    results["内容和深度"] = content_chain.invoke({"article": article})
    results["写作风格"] = style_chain.invoke({"article": article})

    return results


# ============ 4. 演示 ============

async def main():
    # 测试文章
    test_article = """
人工智能正在改变我们的生活方式。从智能手机到自动驾驶汽车，AI技术已经深入到我们日常生活的方方面面。

在医疗领域，AI可以帮助医生更准确地诊断疾病。通过分析大量的医疗数据，AI系统能够识别出人类难以察觉的模式和关联。这不仅提高了诊断的准确性，还能大大缩短诊断时间。

然而，AI的发展也带来了一些挑战。隐私保护、就业影响、伦理问题等都是我们需要认真思考的议题。如何在享受AI带来便利的同时，妥善应对这些挑战，是我们这个时代的重要课题。

总的来说，人工智能是一把双刃剑。我们应该积极拥抱这项技术，同时也要保持警惕，确保AI的发展符合人类的整体利益。
"""

    print("=" * 70)
    print("Parallelization Pattern 示例 - 文章审核系统")
    print("=" * 70)

    # Sectioning 模式演示
    print("\n" + "─" * 70)
    print("模式一：Sectioning（分段并行评估）")
    print("─" * 70)

    import time
    start_time = time.time()
    section_results = await evaluate_article_sectioning(test_article)
    section_time = time.time() - start_time

    for dimension, result in section_results.items():
        print(f"\n📋 {dimension}:")
        print(result[:300] + "..." if len(result) > 300 else result)

    print(f"\n⏱️  并行评估耗时: {section_time:.2f}秒")

    # Voting 模式演示
    print("\n" + "─" * 70)
    print("模式二：Voting（多视角投票）")
    print("─" * 70)

    start_time = time.time()
    voting_results = await evaluate_article_voting(test_article)
    voting_time = time.time() - start_time

    print("\n👥 各评审员意见:")
    for perspective, review in voting_results["individual_reviews"].items():
        print(f"\n{perspective}:")
        print(review[:300] + "..." if len(review) > 300 else review)

    print("\n📊 最终评审结论:")
    print(voting_results["final_decision"][:500] + "...")

    print(f"\n⏱️  投票评估耗时: {voting_time:.2f}秒")

    # 串行方式对比
    print("\n" + "─" * 70)
    print("对比：串行方式（非并行）")
    print("─" * 70)

    start_time = time.time()
    sequential_results = evaluate_article_sequential(test_article)
    sequential_time = time.time() - start_time

    print(f"\n⏱️  串行评估耗时: {sequential_time:.2f}秒")
    print(f"\n并行加速比: {sequential_time / section_time:.1f}x")

    # 总结
    print("\n" + "=" * 70)
    print("Parallelization Pattern 优势总结")
    print("=" * 70)
    print("""
【核心优势】

1. **性能提升**
   - 多个子任务并行执行，显著减少总耗时
   - 特别适合 I/O 密集型任务（如 LLM API 调用）

2. **模块化设计**
   - 每个子任务独立，易于开发和维护
   - 可以单独优化某个子任务

3. **可靠性增强**
   - Voting 模式通过多个视角减少偏见
   - 某个子任务失败不会影响其他任务

4. **可扩展性**
   - 容易添加新的评估维度或评审员
   - 可以根据需要调整并行度

【两种模式对比】

| 模式 | 适用场景 | 特点 |
|------|----------|------|
| Sectioning | 任务可分解为不同部分 | 各部分评估不同维度 |
| Voting | 需要多视角验证 | 同一任务多评审员评估 |

【适用场景】
- 文章/内容审核（多维度评估）
- 代码审查（多个检查规则并行）
- 数据验证（多个验证规则并行）
- 决策系统（多专家投票）
- 批量数据处理
""")


if __name__ == "__main__":
    asyncio.run(main())
