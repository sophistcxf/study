# 使用 Ubuntu 作为基础镜像
FROM ubuntu:latest

# 设置维护者信息（可选）
#LABEL maintainer="yourname@example.com"

# 更新包索引并安装 GCC
RUN apt-get update \
  && apt-get install -y gcc \
  && apt-get clean \
  && rm -rf /var/lib/apt/lists/*

# 设置容器内工作目录（可选）
WORKDIR /code

# 预期工作目录中的源代码（这里不实际复制文件，只是示例）
# COPY ./src /code

# 可以将一个简单的编译命令作为默认行为
# CMD ["gcc", "your-source-file.c", "-o", "your-output-file"]

# 或者让容器启动后默认进入交互模式（bash shell）
CMD ["/bin/bash"]
