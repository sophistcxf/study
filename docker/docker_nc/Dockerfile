# 使用轻量级的基础镜像（Alpine）
FROM alpine:latest
# 安装 netcat（nc）
RUN apk add --no-cache netcat-openbsd
# 声明容器运行时监听的端口
EXPOSE 1234
# 设置容器启动时默认运行的命令
CMD ["nc", "-l", "1234"]
