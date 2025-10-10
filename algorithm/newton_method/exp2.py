import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

class NewtonOptimizer:
    def __init__(self):
        # 目标函数：y=(x-1)^2
        self.func = lambda x: 2*x**2 - 3*x + 7

        # 目标函数的梯度（一阶导数）
        self.grad = lambda x: 4*x - 3

        # 目标函数的二阶导数（Hessian矩阵）
        self.hessian = lambda x: 4  # 常数

        # 优化参数
        self.max_iterations = 20
        self.position_tolerance = 1e-4
        self.gradient_tolerance = 1e-4
        self.positions = []  # 存储优化路径

    def optimize(self, start_pos):
        """执行牛顿法优化"""
        current_pos = np.array([start_pos], dtype=float)  # 转换为数组形式
        self.positions = [current_pos.copy()]

        for i in range(self.max_iterations):
            # 计算当前点的梯度和Hessian矩阵
            g = self.grad(current_pos[0])
            H = self.hessian(current_pos[0])

            # 检查收敛条件
            if abs(g) < self.gradient_tolerance:
                print(f"在迭代 {i} 后收敛（梯度条件）")
                return True

            # 计算牛顿法更新步长
            try:
                # 对于一维情况，直接计算 delta = -g/H
                delta = -g / H
            except ZeroDivisionError:
                print("Hessian矩阵奇异，优化终止")
                return False

            # 更新位置
            new_pos = current_pos[0] + delta

            # 检查位置变化
            if abs(new_pos - current_pos[0]) < self.position_tolerance:
                print(f"在迭代 {i} 后收敛（位置条件）")
                return True

            # 存储并更新位置
            self.positions.append(np.array([new_pos]))
            current_pos[0] = new_pos

        print(f"达到最大迭代次数 {self.max_iterations} 后未收敛")
        return False

    def visualize(self):
        """可视化优化过程"""
        fig, ax = plt.subplots(figsize=(10, 8))

        # 创建函数曲线
        x = np.linspace(-2, 10, 400)
        y = self.func(x)

        # 绘制函数曲线
        ax.plot(x, y, 'b-', linewidth=2, label='y=(x-1)²')

        # 绘制全局最小值点
        ax.plot(1, 0, 'r*', markersize=15, label='global minimum')

        # 准备动画数据
        path_x = np.array([pos[0] for pos in self.positions])
        path_y = np.array([self.func(pos[0]) for pos in self.positions])
        
        line, = ax.plot([], [], 'bo-', lw=1.5, markersize=6, label='optimized path')
        point, = ax.plot([], [], 'ro', markersize=8)

        # 设置图表属性
        ax.set_title('newton optimizer for y=(x-1)²')
        ax.set_xlabel('X')
        ax.set_ylabel('Y')
        ax.legend()
        ax.grid(True)

        def init():
            line.set_data([], [])
            point.set_data([], [])
            return line, point

        def update(frame):
            # 更新路径
            line.set_data(path_x[:frame+1], path_y[:frame+1])
            # 更新当前点
            point.set_data([path_x[frame]], [path_y[frame]])
            return line, point

        ani = FuncAnimation(fig, update, frames=len(path_x),
                            init_func=init, blit=True, interval=800)

        plt.tight_layout()
        plt.show()

# 使用示例
if __name__ == "__main__":
    # 创建优化器实例
    optimizer = NewtonOptimizer()

    # 从起点3.0开始优化
    start_point = 5.0
    success = optimizer.optimize(start_point)

    # 打印最终结果
    final_pos = optimizer.positions[-1][0]
    print(f"\noptimized result: {'success' if success else 'failure'}")
    print(f"start: {start_point:.2f}")
    print(f"end: {final_pos:.6f}")
    print(f"function value: {optimizer.func(final_pos):.6f}")

    # 可视化
    optimizer.visualize()