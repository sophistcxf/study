"""
PDF 向量数据库构建工具

用于将 Agentic_Design_Patterns_Complete.pdf 转换为向量数据库，
方便后续检索书中内容。
"""

import os
from langchain_community.document_loaders import PyPDFLoader
from langchain_text_splitters import RecursiveCharacterTextSplitter
from langchain_community.embeddings import HuggingFaceEmbeddings
from langchain_community.vectorstores import FAISS
from dotenv import load_dotenv

load_dotenv()


def build_pdf_vector_db(
    pdf_path: str = "Agentic_Design_Patterns_Complete.pdf",
    db_path: str = "pdf_vector_db",
    chunk_size: int = 1000,
    chunk_overlap: int = 200
):
    """
    构建 PDF 向量数据库
    
    Args:
        pdf_path: PDF 文件路径
        db_path: 向量数据库保存路径
        chunk_size: 文本块大小
        chunk_overlap: 文本块重叠大小
    """
    print(f"📚 开始处理 PDF 文件: {pdf_path}")
    
    # 步骤1：加载 PDF
    print("📖 正在加载 PDF...")
    loader = PyPDFLoader(pdf_path)
    documents = loader.load()
    print(f"✅ 加载完成，共 {len(documents)} 页")
    
    # 步骤2：添加元数据（页码）
    for i, doc in enumerate(documents):
        doc.metadata["page"] = i + 1
        doc.metadata["source"] = pdf_path
    
    # 步骤3：文本分割
    print("✂️  正在分割文本...")
    text_splitter = RecursiveCharacterTextSplitter(
        chunk_size=chunk_size,
        chunk_overlap=chunk_overlap,
        length_function=len,
        separators=["\n\n", "\n", "。", ".", "!", "?", " ", ""]
    )
    chunks = text_splitter.split_documents(documents)
    print(f"✅ 分割完成，共 {len(chunks)} 个文本块")
    
    # 步骤4：创建嵌入模型（使用 HuggingFace 本地模型）
    print("🔤 正在初始化嵌入模型...")
    try:
        embeddings = HuggingFaceEmbeddings(
            model_name="sentence-transformers/all-MiniLM-L6-v2",
            model_kwargs={'device': 'cpu'}  # 使用 CPU
        )
        print("✅ 使用 HuggingFace 本地嵌入模型")
    except ImportError:
        print("⚠️  HuggingFace 不可用，请安装依赖")
        raise ImportError("请安装: uv pip install sentence-transformers")
    
    # 步骤5：创建向量数据库
    print("🗄️  正在创建向量数据库...")
    vectorstore = FAISS.from_documents(chunks, embeddings)
    
    # 步骤6：保存向量数据库
    print(f"💾 正在保存到: {db_path}")
    vectorstore.save_local(db_path)
    
    print(f"\n✨ 向量数据库构建完成！")
    print(f"   - 文档数量: {len(documents)} 页")
    print(f"   - 文本块数量: {len(chunks)}")
    print(f"   - 保存路径: {db_path}")
    
    return vectorstore


def search_pdf(query: str, db_path: str = "pdf_vector_db", k: int = 5):
    """
    在 PDF 向量数据库中搜索
    
    Args:
        query: 搜索查询
        db_path: 向量数据库路径
        k: 返回结果数量
    
    Returns:
        搜索结果列表
    """
    # 加载嵌入模型
    embeddings = HuggingFaceEmbeddings(
        model_name="sentence-transformers/all-MiniLM-L6-v2",
        model_kwargs={'device': 'cpu'}
    )
    
    # 加载向量数据库
    vectorstore = FAISS.load_local(
        db_path, 
        embeddings, 
        allow_dangerous_deserialization=True
    )
    
    # 执行搜索
    results = vectorstore.similarity_search_with_score(query, k=k)
    
    return results


def print_search_results(results, query: str):
    """
    打印搜索结果
    
    Args:
        results: 搜索结果
        query: 原始查询
    """
    print(f"\n{'='*80}")
    print(f"🔍 搜索查询: {query}")
    print(f"{'='*80}\n")
    
    for i, (doc, score) in enumerate(results, 1):
        print(f"结果 {i} (相似度得分: {score:.4f})")
        print(f"📄 页码: {doc.metadata.get('page', 'N/A')}")
        print(f"📝 内容预览:")
        content_preview = doc.page_content[:500].replace('\n', ' ')
        print(f"   {content_preview}")
        if len(doc.page_content) > 500:
            print(f"   ...")
        print(f"\n{'-'*80}\n")


if __name__ == "__main__":
    import sys
    
    # 如果命令行参数包含 "build"，则构建数据库
    if len(sys.argv) > 1 and sys.argv[1] == "build":
        build_pdf_vector_db()
    else:
        # 默认行为：先构建，然后演示搜索
        print("🚀 PDF 向量数据库工具\n")
        
        # 检查数据库是否已存在
        if not os.path.exists("pdf_vector_db"):
            print("📦 向量数据库不存在，开始构建...\n")
            vectorstore = build_pdf_vector_db()
        else:
            print("✅ 向量数据库已存在\n")
        
        # 演示搜索功能
        print("\n" + "="*80)
        print("🎯 演示搜索功能")
        print("="*80)
        
        test_queries = [
            "Goal Setting and Monitoring",
            "Chapter 11",
            "prompt chaining",
            "multi-agent systems"
        ]
        
        for query in test_queries:
            try:
                results = search_pdf(query, k=3)
                print_search_results(results, query)
            except Exception as e:
                print(f"❌ 搜索失败: {e}\n")
