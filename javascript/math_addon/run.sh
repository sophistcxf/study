#!/bin/bash

echo "运行Node.js原生扩展测试..."

# 检查是否已构建
if [ ! -f "build/Release/math_extension.node" ]; then
    echo "未找到编译的扩展文件，正在自动构建..."
    ./build.sh
fi

# 运行测试
echo "执行测试..."
node test.js