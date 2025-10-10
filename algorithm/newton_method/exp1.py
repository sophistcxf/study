import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

class NewtonOptimizer:
    def __init__(self):
        # 目标函数：Rosenbrock函数（常用于优化算法测试）
        self.func = lambda x, y: (1 - x)**2 + 100*(y - x**2)**2

        # 目标函数的梯度
        self.grad = lambda x, y: np.array([
            -2*(1 - x) - 400*x*(y - x**2),
            200*(y - x**2)
        ])

        # 目标函数的Hessian矩阵（二阶导数）
        self.hessian = lambda x, y: np.array([
            [2 - 400*y + 1200*x**2, -400*x],
            [-400*x, 200]
        ])

        # 优化参数
        self.max_iterations = 20
        self.position_tolerance = 1e-4
        self.gradient_tolerance = 1e-4
        self.positions = []  # 存储优化路径

    def optimize(self, start_pos):
        """执行牛顿法优化"""
        current_pos = np.array(start_pos, dtype=float)
        self.positions = [current_pos.copy()]

        for i in range(self.max_iterations):
            # 计算当前点的梯度和Hessian矩阵
            g = self.grad(*current_pos)
            H = self.hessian(*current_pos)

            # 检查收敛条件
            if np.linalg.norm(g) < self.gradient_tolerance:
                print(f"在迭代 {i} 后收敛（梯度条件）")
                return True

            # 计算牛顿法更新步长
            try:
                # 解线性方程 H * delta = -g
                delta = np.linalg.solve(H, -g)
            except np.linalg.LinAlgError:
                print("Hessian矩阵奇异，优化终止")
                return False

            # 更新位置
            new_pos = current_pos + delta

            # 检查位置变化
            if np.linalg.norm(new_pos - current_pos) < self.position_tolerance:
                print(f"在迭代 {i} 后收敛（位置条件）")
                return True

            # 存储并更新位置
            self.positions.append(new_pos.copy())
            current_pos = new_pos

        print(f"达到最大迭代次数 {self.max_iterations} 后未收敛")
        return False

    def visualize(self):
        """可视化优化过程"""
        fig, ax = plt.subplots(figsize=(10, 8))

        # 创建函数等高线
        x = np.linspace(-1.5, 2, 400)
        y = np.linspace(-0.5, 3, 400)
        X, Y = np.meshgrid(x, y)
        Z = self.func(X, Y)

        # 绘制等高线
        levels = np.logspace(-1, 3, 20)
        contour = ax.contour(X, Y, Z, levels=levels, cmap='viridis', alpha=0.6)
        plt.colorbar(contour, ax=ax, label='function value')

        # 绘制全局最小值点
        ax.plot(1, 1, 'r*', markersize=15, label='global minimum')

        # 准备动画数据
        path = np.array(self.positions)
        line, = ax.plot([], [], 'bo-', lw=1.5, markersize=6, label='optimized path')
        point, = ax.plot([], [], 'ro', markersize=8)

        # 设置图表属性
        ax.set_title('newton optimizer')
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
            line.set_data(path[:frame+1, 0], path[:frame+1, 1])
            # 更新当前点
            point.set_data([path[frame, 0]], [path[frame, 1]])
            return line, point

        ani = FuncAnimation(fig, update, frames=len(path),
                            init_func=init, blit=True, interval=800)

        plt.tight_layout()
        plt.show()

# 使用示例
if __name__ == "__main__":
    # 创建优化器实例
    optimizer = NewtonOptimizer()

    # 从起点(-0.5, 2.5)开始优化
    start_point = [-0.5, 2.5]
    success = optimizer.optimize(start_point)

    # 打印最终结果
    final_pos = optimizer.positions[-1]
    print(f"\noptimized result: {'success' if success else 'failure'}")
    print(f"start: ({start_point[0]:.2f}, {start_point[1]:.2f})")
    print(f"end: ({final_pos[0]:.6f}, {final_pos[1]:.6f})")
    print(f"function value: {optimizer.func(*final_pos):.6f}")

    # 可视化优化过程
    optimizer.visualize()
