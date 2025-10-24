import numpy as np
from scipy.spatial.transform import Rotation
import matplotlib.pyplot as plt
import pyceres as ceres

# 创建 Ceres 问题
problem = ceres.Problem()

# 定义位姿节点 (x, y, theta)
num_poses = 4
poses = np.zeros((num_poses, 3), dtype=np.float64)

# 初始位姿估计 (包含噪声)
poses[0] = [0.0, 0.0, 0.0]          # 起点
poses[1] = [0.9, 0.1, 0.15]         # 添加噪声
poses[2] = [1.9, 0.1, 0.25]         # 添加噪声
poses[3] = [2.8, -0.1, 0.35]        # 添加噪声

# 添加参数块 (每个位姿作为优化变量)
for i in range(num_poses):
    problem.add_parameter_block(poses[i], 3)

    # 固定第一个位姿以消除自由度
    if i == 0:
        problem.set_parameter_block_constant(poses[i])

# 定义残差函数 (用于计算位姿之间的约束)
class PoseGraphResidual:
    def __init__(self, dx, dy, dtheta, weight=1.0):
        self.measurement = np.array([dx, dy, dtheta])
        self.weight = weight

    def __call__(self, pose_i, pose_j):
        # 提取位姿 i 和 j
        x_i, y_i, theta_i = pose_i
        x_j, y_j, theta_j = pose_j

        # 计算相对变换
        cos_i = np.cos(theta_i)
        sin_i = np.sin(theta_i)

        dx = cos_i * (x_j - x_i) + sin_i * (y_j - y_i)
        dy = -sin_i * (x_j - x_i) + cos_i * (y_j - y_i)
        dtheta = theta_j - theta_i

        # 归一化角度差到 [-π, π]
        dtheta = np.arctan2(np.sin(dtheta), np.cos(dtheta))

        # 计算残差
        residuals = np.array([dx, dy, dtheta]) - self.measurement
        residuals *= self.weight  # 应用权重

        return residuals

# 添加里程计约束 (相邻位姿之间的约束)
odometry_constraints = [
    (0, 1, 1.0, 0.0, 0.1, 1.0),   # 从位姿0到位姿1
    (1, 2, 1.0, 0.0, 0.1, 1.0),   # 从位姿1到位姿2
    (2, 3, 1.0, 0.0, 0.1, 1.0)    # 从位姿2到位姿3
]

# 修改：创建残差块的正确方式
for i, j, dx, dy, dtheta, weight in odometry_constraints:
    residual_block = PoseGraphResidual(dx, dy, dtheta, weight)
    # 修改：使用正确的方式创建成本函数
    cost_function = ceres.NumericDiffCostFunction(residual_block, 3, [3, 3])
    problem.add_residual_block(
        cost_function,
        None,  # 没有损失函数
        [poses[i], poses[j]]
    )

# 添加回环约束 (检测到的非相邻位姿之间的约束)
loop_closure = PoseGraphResidual(-3.0, 0.0, -0.3, 5.0)  # 更高的权重
# 修改：使用正确的方式创建回环约束成本函数
loop_cost_function = ceres.CreateCostFunction(loop_closure, 3, [3, 3])
problem.add_residual_block(
    loop_cost_function,
    None,
    [poses[0], poses[3]]
)

# 配置求解器选项
options = ceres.SolverOptions()
options.max_num_iterations = 100
options.linear_solver_type = ceres.LinearSolverType.DENSE_QR
options.minimizer_progress_to_stdout = True

# 运行优化
summary = ceres.Summary()
ceres.Solve(options, problem, summary)

# 打印优化结果
print(summary.BriefReport())
print("\n优化后的位姿:")
for i, pose in enumerate(poses):
    print(f"位姿 {i}: x={pose[0]:.4f}, y={pose[1]:.4f}, θ={pose[2]:.4f}")

# 可视化结果
def plot_poses(poses, color, label):
    x = poses[:, 0]
    y = poses[:, 1]
    plt.plot(x, y, 'o-', color=color, label=label)

    # 绘制方向箭头
    for pose in poses:
        dx = 0.5 * np.cos(pose[2])
        dy = 0.5 * np.sin(pose[2])
        plt.arrow(pose[0], pose[1], dx, dy,
                  head_width=0.1, head_length=0.2, fc=color, ec=color)

# 创建初始位姿的副本用于比较
initial_poses = poses.copy()

# 绘制结果
plt.figure(figsize=(10, 8))
plot_poses(initial_poses, 'red', '优化前')
plot_poses(poses, 'blue', '优化后')

# 添加约束线
for i, j, *_ in odometry_constraints:
    plt.plot([poses[i][0], poses[j][0]],
             [poses[i][1], poses[j][1]], 'g--', alpha=0.3)

# 绘制回环约束
plt.plot([poses[0][0], poses[3][0]],
         [poses[0][1], poses[3][1]], 'm-', linewidth=2, label='回环约束')

plt.xlabel('X')
plt.ylabel('Y')
plt.title('位姿图优化')
plt.legend()
plt.grid(True)
plt.axis('equal')
plt.show()