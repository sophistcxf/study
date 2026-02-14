#!/bin/bash

echo "开始构建Node.js原生扩展..."

# 清理之前的构建
echo "清理旧的构建文件..."
npm run clean

# 安装依赖
echo "安装依赖..."
npm install

# 构建扩展
echo "编译原生扩展..."
npm run build

echo "构建完成！"
echo "生成的.so文件位于: build/Release/math_extension.node"