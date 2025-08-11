import os
from openai import OpenAI
from dotenv import load_dotenv
load_dotenv(override=True)
os.environ.setdefault("OPENAI_API_KEY","GpoHwppRIN4zA8r1CtQeaWC9")
os.environ.setdefault("OPENAI_API_KEY",os.getenv("AMAP_API_KEY"))
client = OpenAI(
    base_url=os.getenv("AMAP_EMBEDDING_BASE_URL")
)

completion = client.embeddings.create(
    model="text-embedding-v3",
    input='衣服的质量杠杠的，很漂亮，不枉我等了这么久啊，喜欢，以后还来这里买',
    dimensions=1024,
    encoding_format="float"
)

print(completion.model_dump_json())
