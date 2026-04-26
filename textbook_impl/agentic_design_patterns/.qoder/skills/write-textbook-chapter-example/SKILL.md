---
name: write-textbook-chapter-example
description: Write runnable Python examples for Agentic Design Patterns textbook chapters. Use when the user asks to write a chapter example, create exp1.py for a chapter, or implement a design pattern from the textbook. Guides through PDF analysis, code structure, DeepSeek API integration, and validation.
---

# Write Textbook Chapter Example

## Overview

This skill standardizes the workflow for implementing runnable Python examples for the *Agentic Design Patterns* textbook. Each chapter demonstrates a specific agentic design pattern using LangChain and DeepSeek API.

## Prerequisites

- Project uses `uv` for Python environment management
- API keys in `.env`: `DEEPSEEK_API_KEY`, `TAVILY_API_KEY`
- Textbook PDF: `Agentic_Design_Patterns_Complete.pdf`
- Existing chapters in `ch1/`, `ch5/`, `ch7/`, `ch10/` serve as style references

## Workflow

### Step 1: Determine Chapter Topic

Extract chapter information from the PDF:

```bash
# Ensure pypdf is installed
uv add pypdf

# Extract chapter titles
uv run python -c "
from pypdf import PdfReader
import re
reader = PdfReader('Agentic_Design_Patterns_Complete.pdf')
for i, page in enumerate(reader.pages):
    pt = page.extract_text()
    if pt:
        matches = re.findall(r'(?i)chapter\s+(\d+)[:.\s]+([^\n]+)', pt)
        for num, title in matches:
            t = title.strip()
            if 3 < len(t) < 150:
                print(f'Page {i+1}: Chapter {num}: {t}')
"
```

Then extract the target chapter's content (adjust page range):

```bash
uv run python -c "
from pypdf import PdfReader
reader = PdfReader('Agentic_Design_Patterns_Complete.pdf')
for i in range(START_PAGE-1, END_PAGE):
    pt = reader.pages[i].extract_text()
    if pt:
        print(f'===== PAGE {i+1} =====')
        print(pt)
" > /tmp/chN.txt
```

### Step 2: Analyze Existing Code Style

Read 1-2 existing chapter examples to understand conventions:

```bash
# Typical structure to observe:
# - ch1/exp1.py: Prompt Chaining (simple chain pattern)
# - ch5/exp1.py: Tool Use + ReAct (agent with tools)
# - ch7/exp1.py: Multi-Agent (coordinator + specialized agents)
# - ch10/exp1.py: MCP integration (async patterns)
```

Key conventions to follow:
- File: `chN/exp1.py` (N = chapter number)
- Docstring header explaining the pattern
- Section comments: `# ============ N. Section Name ============`
- DeepSeek LLM setup with `ChatOpenAI(base_url="https://api.deepseek.com/v1")`
- `main()` function with demo + summary
- If using multiple files, main entry is `exp1.py`, helpers named descriptively

### Step 3: Create Directory and File

```bash
mkdir -p chN
```

Create `chN/exp1.py` with this boilerplate:

```python
"""
Chapter N: Pattern Name (中文名)

Pattern description (2-3 sentences).

Core concepts:
- Key idea 1
- Key idea 2

Example scenario: ...
"""

import os
from dotenv import load_dotenv
from langchain_openai import ChatOpenAI
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser

load_dotenv()

# Initialize LLM
llm = ChatOpenAI(
    model="deepseek-chat",  # or "deepseek-reasoner" for reasoning tasks
    temperature=0,
    base_url="https://api.deepseek.com/v1",
    api_key=os.getenv("DEEPSEEK_API_KEY")
)

# ============ 1. Core Components ============

# ... implementation ...

# ============ 2. Main System ============

# ... implementation ...

# ============ 3. Demo ============

def main():
    print("=" * 70)
    print("Chapter N: Pattern Name")
    print("=" * 70)
    # ... demos ...

if __name__ == "__main__":
    main()
```

### Step 4: Adapt Textbook Code to DeepSeek

The textbook uses OpenAI models (GPT-4o, o4-mini). Map to DeepSeek equivalents:

| Textbook Model | DeepSeek Equivalent | Use Case |
|---|---|---|
| GPT-4o-mini / Flash | `deepseek-chat` (temp=0.7) | Fast, cheap, simple tasks |
| GPT-4o | `deepseek-chat` (temp=0.3) | Standard quality |
| o4-mini / o3 / Pro | `deepseek-reasoner` | Complex reasoning |

If the textbook uses `google_search()`, replace with `TavilySearchResults`:

```python
from langchain_community.tools import TavilySearchResults

search = TavilySearchResults(max_results=3, api_key=os.getenv("TAVILY_API_KEY"))
results = search.invoke(query)
```

### Step 5: Critical Pitfalls

**Pitfall 1: JSON in ChatPromptTemplate**

JSON braces `{` `}` are interpreted as template variables. Escape them:

```python
# WRONG - LangChain tries to find variable "classification"
"Respond with JSON: {\"classification\": \"simple\"}"

# CORRECT - double braces escape
"Respond with JSON: {{\"classification\": \"simple\"}}"
```

**Pitfall 2: TavilySearchResults naming conflict**

Do not name the instance attribute the same as the method:

```python
# WRONG - self.search shadows the search() method
class SearchTool:
    def __init__(self):
        self.search = TavilySearchResults(...)  # shadows method!
    def search(self, query): ...

# CORRECT
class SearchTool:
    def __init__(self):
        self.search_engine = TavilySearchResults(...)
    def search(self, query):
        results = self.search_engine.invoke(query)
```

**Pitfall 3: Async patterns**

If the textbook example uses async (like MCP), wrap in `asyncio.run()`:

```python
async def async_main():
    ...

def main():
    asyncio.run(async_main())
```

### Step 6: Validation Checklist

Before finishing, verify:

```bash
# 1. Syntax check
uv run python -m py_compile chN/exp1.py

# 2. Run the example
uv run python chN/exp1.py

# 3. Check for runtime errors
#    - API connectivity
#    - Tool invocations
#    - Prompt template parsing
```

Common fixes:
- Missing imports → add them
- `LangChainDeprecationWarning` for Tavily → install `langchain-tavily` if needed (usually not blocking)
- API errors → check `.env` and model name

### Step 7: Output Summary

After successful validation, report:
1. Chapter number and pattern name
2. File location (`chN/exp1.py`)
3. Key components implemented
4. Run command: `uv run python chN/exp1.py`

## Code Style Rules

1. **Use Chinese for all user-facing output** (docstrings, print statements, comments)
2. **Use English for code identifiers** (classes, functions, variables)
3. **Section headers**: `# ============ N. Name ============`
4. **Demo structure**: `demo_xxx()` functions called from `main()`, plus comparison/contrast where applicable
5. **Summary block**: ASCII-art summary at end of `main()` explaining advantages, use cases, and pattern combinations

## Progressive Disclosure

- For complex multi-file chapters, see `reference.md` (if created)
- For DeepSeek API quirks, see `langchain-deepseek-notes.md` (if created)
