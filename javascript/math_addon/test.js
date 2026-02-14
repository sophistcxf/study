const MathAddon = require('./index.js');

console.log('=== Node.js原生扩展(.so文件)加载测试 ===\n');

// 测试1: 阶乘计算
console.log('1. 测试阶乘计算:');
try {
    console.log(`5! = ${MathAddon.factorial(5)}`);
    console.log(`10! = ${MathAddon.factorial(10)}`);
    console.log(`0! = ${MathAddon.factorial(0)}`);
} catch (error) {
    console.error('阶乘计算错误:', error.message);
}

console.log('\n2. 测试斐波那契数列:');
try {
    console.log(`Fibonacci(10) = ${MathAddon.fibonacci(10)}`);
    console.log(`Fibonacci(20) = ${MathAddon.fibonacci(20)}`);
    console.log(`Fibonacci(0) = ${MathAddon.fibonacci(0)}`);
} catch (error) {
    console.error('斐波那契计算错误:', error.message);
}

console.log('\n3. 测试数组求和:');
try {
    const numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    console.log(`数组 [${numbers.join(', ')}] 的和 = ${MathAddon.sum(numbers)}`);
    
    const decimals = [1.5, 2.7, 3.2, 4.8];
    console.log(`数组 [${decimals.join(', ')}] 的和 = ${MathAddon.sum(decimals)}`);
} catch (error) {
    console.error('数组求和错误:', error.message);
}

console.log('\n4. 测试矩阵乘法:');
try {
    const matrixA = [
        [1, 2, 3],
        [4, 5, 6]
    ];
    
    const matrixB = [
        [7, 8],
        [9, 10],
        [11, 12]
    ];
    
    console.log('矩阵 A:');
    matrixA.forEach(row => console.log(row.join('\t')));
    
    console.log('\n矩阵 B:');
    matrixB.forEach(row => console.log(row.join('\t')));
    
    const result = MathAddon.multiplyMatrices(matrixA, matrixB);
    console.log('\n矩阵乘积 A × B:');
    result.forEach(row => console.log(row.join('\t')));
    
} catch (error) {
    console.error('矩阵乘法错误:', error.message);
}

console.log('\n5. 错误处理测试:');
try {
    console.log('测试负数阶乘:');
    MathAddon.factorial(-5);
} catch (error) {
    console.log('✓ 正确捕获负数阶乘错误:', error.message);
}

try {
    console.log('测试非整数输入:');
    MathAddon.fibonacci(3.14);
} catch (error) {
    console.log('✓ 正确捕获非整数输入错误:', error.message);
}

try {
    console.log('测试无效数组:');
    MathAddon.sum(['a', 'b', 'c']);
} catch (error) {
    console.log('✓ 正确捕获无效数组元素错误:', error.message);
}

console.log('\n6. 性能对比测试:');
console.time('JavaScript阶乘计算');
let jsResult = 1;
for (let i = 1; i <= 15; i++) {
    jsResult *= i;
}
console.timeEnd('JavaScript阶乘计算');
console.log(`JavaScript结果: ${jsResult}`);

console.time('Native扩展阶乘计算');
const nativeResult = MathAddon.factorial(15);
console.timeEnd('Native扩展阶乘计算');
console.log(`Native扩展结果: ${nativeResult}`);

console.log('\n=== 测试完成 ===');
console.log('\n说明:');
console.log('- 这个示例展示了Node.js如何通过Node-API加载和使用.so文件');
console.log('- 原生C++代码被编译成动态链接库(.so/.node文件)');
console.log('- JavaScript可以通过require()直接加载并调用原生函数');
console.log('- Native扩展通常比纯JavaScript实现有更好的性能');