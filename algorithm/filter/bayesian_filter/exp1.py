import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm

class BayesianFilter:
    def __init__(self, state_space, initial_belief, transition_std, observation_std):
        """
        初始化贝叶斯滤波器

        参数:
        state_space: 状态空间数组 (1D numpy数组)
        initial_belief: 初始信念 (概率分布，与state_space同长度)
        transition_std: 状态转移噪声的标准差
        observation_std: 观测噪声的标准差
        """
        self.state_space = state_space
        self.belief = initial_belief
        self.transition_std = transition_std
        self.observation_std = observation_std

        # 确保信念是概率分布
        self.normalize_belief()

    def normalize_belief(self):
        """归一化信念分布"""
        self.belief /= np.sum(self.belief)

    def predict(self, motion):
        """
        预测步骤 - 根据系统模型更新信念

        参数:
        motion: 系统运动量 (标量)
        """
        # 创建新的信念数组
        new_belief = np.zeros_like(self.belief)

        # 对于状态空间中的每个可能状态
        for i, state in enumerate(self.state_space):
            # 计算系统模型预测的状态
            predicted_state = state + motion

            # 计算转移概率 (高斯分布)
            transition_prob = norm.pdf(self.state_space, predicted_state, self.transition_std)

            # 更新新状态的概率
            new_belief += self.belief[i] * transition_prob

        # 更新信念
        self.belief = new_belief
        self.normalize_belief()

    def update(self, observation):
        """
        更新步骤 - 根据观测数据修正信念

        参数:
        observation: 观测值 (标量)
        """
        # 计算观测似然 (高斯分布)
        likelihood = norm.pdf(self.state_space, observation, self.observation_std)

        # 贝叶斯更新: 后验 ∝ 似然 × 先验
        self.belief *= likelihood
        self.normalize_belief()

    def get_estimate(self):
        """获取状态估计 (最大后验概率估计)"""
        return self.state_space[np.argmax(self.belief)]

    def get_mean_estimate(self):
        """获取状态估计 (均值估计)"""
        return np.sum(self.state_space * self.belief)

# 模拟参数设置
np.random.seed(42)
num_steps = 50
state_min, state_max = 0, 100
state_space = np.linspace(state_min, state_max, 500)  # 离散状态空间

# 真实系统参数
true_position = 50
true_velocity = 0.5
process_noise_std = 1.0
measurement_noise_std = 5.0

# 滤波器参数
filter_transition_std = 1.2  # 滤波器对系统模型的不确定性
filter_observation_std = 5.5  # 滤波器对观测模型的不确定性

# 初始化均匀分布的信念
initial_belief = np.ones_like(state_space) / len(state_space)

# 创建贝叶斯滤波器
bf = BayesianFilter(state_space, initial_belief,
                   filter_transition_std, filter_observation_std)

# 存储结果用于可视化
true_positions = []
measurements = []
map_estimates = []
mean_estimates = []
belief_history = []

for step in range(num_steps):
    # 1. 真实系统动态更新 (恒定速度模型 + 噪声)
    true_velocity += np.random.normal(0, process_noise_std/5)
    true_position += true_velocity + np.random.normal(0, process_noise_std)
    true_position = np.clip(true_position, state_min, state_max)
    true_positions.append(true_position)

    # 2. 生成带噪声的观测
    measurement = true_position + np.random.normal(0, measurement_noise_std)
    measurements.append(measurement)

    # 3. 贝叶斯滤波预测步骤 (使用真实速度作为运动输入)
    bf.predict(true_velocity)

    # 4. 贝叶斯滤波更新步骤
    bf.update(measurement)

    # 5. 记录结果
    map_estimates.append(bf.get_estimate())
    mean_estimates.append(bf.get_mean_estimate())
    belief_history.append(bf.belief.copy())

# 创建可视化
plt.figure(figsize=(15, 10))
plt.rcParams['font.sans-serif'] = ['PingFang SC']

# 1. 轨迹对比图
plt.subplot(2, 1, 1)
plt.plot(true_positions, 'g-', linewidth=2, label='true_positions')
plt.plot(measurements, 'ro', markersize=4, alpha=0.6, label='observations')
plt.plot(map_estimates, 'b--', linewidth=2, label='map_estimate')
plt.plot(mean_estimates, 'm-.', linewidth=2, label='mean_estimate')
plt.title('bayesian_filter - positions estimates')
plt.xlabel('time_step')
plt.ylabel('position')
plt.legend()
plt.grid(True)

# 2. 信念分布热力图
plt.subplot(2, 1, 2)
# 创建网格: 时间步 vs 状态空间
time_steps = np.arange(num_steps)
X, Y = np.meshgrid(state_space, time_steps)

# 准备信念数据 (每个时间步的信念分布)
belief_data = np.array(belief_history)

# 绘制热力图
plt.pcolormesh(X, Y, belief_data, shading='auto', cmap='viridis')
plt.colorbar(label='pdf')
plt.plot(true_positions, time_steps, 'r-', linewidth=2, label='true_positions')
plt.plot(map_estimates, time_steps, 'b--', linewidth=1.5, label='map_estimate')
plt.title('信念分布演化')
plt.xlabel('位置')
plt.ylabel('时间步')
plt.legend()

plt.tight_layout()
plt.show()

# 3. 关键时间步的信念分布
key_steps = [0, 10, 20, 30, 40, 49]
plt.figure(figsize=(12, 8))
for i, step in enumerate(key_steps):
    plt.subplot(2, 3, i+1)
    plt.plot(state_space, belief_history[step], 'b-', linewidth=1.5)
    plt.axvline(x=true_positions[step], color='r', linestyle='-', label='真实位置')
    plt.axvline(x=map_estimates[step], color='g', linestyle='--', label='MAP估计')
    plt.axvline(x=mean_estimates[step], color='m', linestyle='-.', label='均值估计')
    plt.title(f'时间步 {step}')
    plt.xlabel('位置')
    plt.ylabel('概率密度')
    if i == 0:
        plt.legend()
    plt.grid(True)

plt.suptitle('关键时间步的信念分布')
plt.tight_layout()
plt.show()
