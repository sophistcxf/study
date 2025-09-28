import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

class ParticleFilter:
    def __init__(self, num_particles, process_noise, measurement_noise):
        self.num_particles = num_particles
        self.process_noise = process_noise
        self.measurement_noise = measurement_noise
        self.particles = None
        self.weights = None
        self.state_dim = 2  # [position, velocity]
        self.initialize_particles()

    def initialize_particles(self):
        # 初始化粒子位置和速度
        self.particles = np.zeros((self.num_particles, self.state_dim))
        self.particles[:, 0] = np.random.uniform(-10, 10, self.num_particles)  # 初始位置
        self.particles[:, 1] = np.random.uniform(-1, 1, self.num_particles)     # 初始速度
        self.weights = np.ones(self.num_particles) / self.num_particles  # 均匀权重

    def predict(self, dt=1.0):
        # 状态预测（恒定速度模型）
        self.particles[:, 1] += np.random.normal(0, self.process_noise, self.num_particles)  # 速度噪声
        self.particles[:, 0] += self.particles[:, 1] * dt  # 位置更新
        self.particles[:, 0] += np.random.normal(0, self.process_noise, self.num_particles)  # 位置噪声

    def update(self, measurement):
        '''
        权重更新（基于测量似然）
        观察 measurement 是位置
        '''
        expected = self.particles[:, 0]  # 预测位置
        # 计算高斯似然
        likelihood = np.exp(-0.5 * ((measurement - expected) ** 2) / (self.measurement_noise ** 2))
        self.weights *= likelihood
        self.weights += 1e-300  # 防止除零
        self.weights /= np.sum(self.weights)  # 归一化

    def resample(self):
        # 系统重采样（Systematic Resampling）
        indices = np.zeros(self.num_particles, dtype=int)
        cumulative_sum = np.cumsum(self.weights)
        step = cumulative_sum[-1] / self.num_particles
        u = np.random.uniform(0, step)

        i = 0
        for j in range(self.num_particles):
            while u > cumulative_sum[i]:
                i += 1
            indices[j] = i
            u += step

        # 重采样粒子
        self.particles = self.particles[indices]
        self.weights = np.ones(self.num_particles) / self.num_particles

    def estimate(self):
        # 计算状态估计（加权平均）
        return np.average(self.particles[:, 0], weights=self.weights), np.average(self.particles[:, 1], weights=self.weights)

# 模拟参数
np.random.seed(42)
num_steps = 50
true_position = 0.0
true_velocity = 0.5
process_noise = 0.1
measurement_noise = 1.0
num_particles = 200

# 创建粒子滤波器
pf = ParticleFilter(num_particles, process_noise, measurement_noise)

# 存储结果用于可视化
true_positions = []
measurements = []
estimates = []
particles_history = []

for step in range(num_steps):
    # 真实状态更新（恒定速度 + 噪声）
    true_velocity += np.random.normal(0, process_noise)
    true_position += true_velocity + np.random.normal(0, process_noise)
    true_positions.append(true_position)

    # 生成带噪声的测量
    measurement = true_position + np.random.normal(0, measurement_noise)
    measurements.append(measurement)

    # 粒子滤波步骤
    pf.predict()
    pf.update(measurement)

    # 估计状态
    pos_est, vel_est = pf.estimate()
    estimates.append(pos_est)

    # 记录粒子状态
    particles_history.append(pf.particles.copy())

    # 重采样（有效粒子数判断）
    effective_particles = 1.0 / np.sum(pf.weights**2)
    if effective_particles < num_particles / 2:
        pf.resample()

# 创建可视化
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), gridspec_kw={'height_ratios': [3, 1]})
plt.subplots_adjust(hspace=0.4)

# 设置坐标轴
ax1.set_xlim(-10, 60)
ax1.set_ylim(-5, 5)
ax1.set_xlabel('Position')
ax1.set_ylabel('Time Step')
ax1.set_title('Particle Filter Tracking')

# 时间轴标签
time_labels = [str(i) for i in range(num_steps)]
ax1.set_yticks(range(num_steps))
ax1.set_yticklabels(time_labels)

# 粒子散点图
scatter = ax1.scatter([], [], s=5, alpha=0.5, c='blue', label='Particles')
ax1.plot([], [], 'r-', linewidth=2, label='True Path')
ax1.plot([], [], 'g--', linewidth=2, label='Estimates')
ax1.scatter([], [], s=50, c='purple', marker='*', label='Measurements')
ax1.legend(loc='upper left')

# 粒子分布直方图
ax2.set_xlim(-10, 60)
ax2.set_ylim(0, num_particles/10)
ax2.set_xlabel('Position')
ax2.set_ylabel('Particle Count')
ax2.set_title('Particle Distribution')

# 初始化绘图元素
true_line, = ax1.plot([], [], 'r-')
est_line, = ax1.plot([], [], 'g--')
meas_scatter = ax1.scatter([], [], s=50, c='purple', marker='*')
hist_bins = np.linspace(-10, 60, 50)
hist_bar = ax2.bar(hist_bins[:-1], np.zeros(len(hist_bins)-1), width=hist_bins[1]-hist_bins[0], alpha=0.6)

# 动画更新函数
def update(frame):
    # 更新轨迹
    true_line.set_data(true_positions[:frame], range(frame))
    est_line.set_data(estimates[:frame], range(frame))

    # 更新测量点
    if frame > 0:
        meas_scatter.set_offsets(np.c_[measurements[:frame], range(frame)])

    # 更新粒子
    scatter.set_offsets(np.c_[particles_history[frame][:, 0], np.full(num_particles, frame)])

    # 更新直方图
    hist, _ = np.histogram(particles_history[frame][:, 0], bins=hist_bins)
    for rect, h in zip(hist_bar, hist):
        rect.set_height(h)

    return scatter, true_line, est_line, meas_scatter, *hist_bar

# 创建动画
ani = animation.FuncAnimation(fig, update, frames=num_steps, interval=200, blit=True)
plt.show()
