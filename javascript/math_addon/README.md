# Node.js原生扩展(.so文件)加载示例

这个项目演示了如何在Node.js中创建、编译和加载原生C++扩展(.so文件)。

## 项目结构

```
math_addon/
├── math_extension.cpp    # C++原生扩展源码
├── binding.gyp          # 编译配置文件
├── package.json         # Node.js包配置
├── index.js            # JavaScript封装接口
├── test.js             # 测试文件
├── build.sh            # 构建脚本
├── run.sh              # 运行脚本
└── README.md           # 说明文档
```

## 工作原理

1. **C++代码编写**: 使用Node-API编写原生C++函数
2. **编译过程**: 通过node-gyp将C++代码编译为动态链接库(.so/.node文件)
3. **Node.js加载**: JavaScript通过`require()`加载编译后的.so文件
4. **函数调用**: 直接调用原生C++函数，获得接近原生性能

## 安装和使用

### 前置条件

确保系统已安装：
- Node.js (推荐v16+)
- Python (用于node-gyp)
- C++编译器 (GCC/Clang/msvc)

### 快速开始

```bash
# 克隆或下载项目
cd math_addon

# 方法1: 使用构建脚本(推荐)
chmod +x build.sh run.sh
./build.sh  # 构建扩展
./run.sh    # 运行测试

# 方法2: 手动构建
npm install
npm run build
npm test
```

## API说明

### MathAddon.factorial(n)
计算阶乘
```javascript
const result = MathAddon.factorial(5); // 返回 120
```

### MathAddon.fibonacci(n)
计算斐波那契数列第n项
```javascript
const result = MathAddon.fibonacci(10); // 返回 55
```

### MathAddon.sum(array)
计算数组元素总和
```javascript
const result = MathAddon.sum([1, 2, 3, 4, 5]); // 返回 15
```

### MathAddon.multiplyMatrices(matrixA, matrixB)
矩阵乘法运算
```javascript
const matrixA = [[1, 2], [3, 4]];
const matrixB = [[5, 6], [7, 8]];
const result = MathAddon.multiplyMatrices(matrixA, matrixB);
```

## 性能优势

原生扩展相比纯JavaScript的优势：
- **执行速度**: C++原生代码执行更快
- **内存效率**: 更好的内存管理和使用
- **CPU密集型任务**: 适合数学计算、图像处理等场景
- **系统级操作**: 可以直接调用系统API

## 实际应用场景

1. **数学计算**: 复杂的数学算法和统计计算
2. **图像处理**: 图像变换、滤镜效果
3. **加密解密**: 高性能的加解密算法
4. **数据压缩**: 视频、音频编解码
5. **硬件交互**: 直接与硬件设备通信

## 开发注意事项

1. **类型安全**: C++代码需要严格处理类型转换
2. **内存管理**: 注意避免内存泄漏
3. **异常处理**: 正确处理JavaScript和C++之间的异常
4. **跨平台兼容**: 不同平台可能需要不同的编译配置

## 故障排除

常见问题及解决方案：

1. **编译失败**: 确保安装了正确的编译工具链
2. **找不到.node文件**: 检查构建路径是否正确
3. **版本兼容性**: 确保Node.js版本与node-gyp兼容

## 学习资源

- [Node-API官方文档](https://nodejs.org/api/n-api.html)
- [node-addon-api GitHub](https://github.com/nodejs/node-addon-api)
- [node-gyp文档](https://github.com/nodejs/node-gyp)