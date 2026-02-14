// 加载编译后的原生扩展
const mathExtension = require('./build/Release/math_extension.node');

// 封装更好的API接口
class MathAddon {
    /**
     * 计算阶乘（同步）
     * @param {number} n - 要计算阶乘的数字
     * @returns {number} 阶乘结果
     */
    static factorial(n) {
        if (!Number.isInteger(n) || n < 0) {
            throw new Error('参数必须是非负整数');
        }
        return mathExtension.factorialSync(n);
    }

    /**
     * 计算斐波那契数列（同步）
     * @param {number} n - 斐波那契数列位置
     * @returns {number} 斐波那契数值
     */
    static fibonacci(n) {
        if (!Number.isInteger(n) || n < 0) {
            throw new Error('参数必须是非负整数');
        }
        return mathExtension.fibonacciSync(n);
    }

    /**
     * 计算数组元素总和
     * @param {number[]} array - 数字数组
     * @returns {number} 数组元素总和
     */
    static sum(array) {
        if (!Array.isArray(array)) {
            throw new Error('参数必须是数组');
        }
        // 验证数组元素都是数字
        const validArray = array.map(item => {
            if (typeof item !== 'number') {
                throw new Error('数组元素必须都是数字');
            }
            return item;
        });
        return mathExtension.arraySum(validArray);
    }

    /**
     * 矩阵乘法
     * @param {number[][]} matrixA - 第一个矩阵
     * @param {number[][]} matrixB - 第二个矩阵
     * @returns {number[][]} 乘积矩阵
     */
    static multiplyMatrices(matrixA, matrixB) {
        // 验证输入
        if (!Array.isArray(matrixA) || !Array.isArray(matrixB)) {
            throw new Error('参数必须是二维数组');
        }

        if (matrixA.length === 0 || matrixB.length === 0) {
            throw new Error('矩阵不能为空');
        }

        // 验证矩阵格式
        const colsA = matrixA[0].length;
        const rowsB = matrixB.length;

        if (colsA !== rowsB) {
            throw new Error('矩阵维度不匹配：第一个矩阵的列数必须等于第二个矩阵的行数');
        }

        return mathExtension.matrixMultiply(matrixA, matrixB);
    }
}

module.exports = MathAddon;