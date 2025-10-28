import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

class KalmanFilter:
    def __init__(self, process_noise, measurement_noise):
        self.process_noise = process_noise
        self.measurement_noise = measurement_noise

        # State vector: [position, velocity]
        self.state = np.zeros(2)

        # State covariance matrix
        self.covariance = np.eye(2) * 10  # Large initial uncertainty

        # 状态转移矩阵
        self.F = np.array([[1, 1],  # position += velocity
                           [0, 1]]) # velocity remains

        # 系统模型的协方差
        self.Q = np.eye(2) * process_noise

        # 观测矩阵
        self.H = np.array([[1, 0]])

        # 观测噪声的协方差
        self.R = np.array([[measurement_noise]])

    def predict(self, dt=1.0):
        '''
        计算先验分布，(state, convariance)
        '''
        # Update state transition matrix with dt
        self.F[0, 1] = dt

        # Predict state
        self.state = self.F @ self.state

        # Predict covariance
        self.covariance = self.F @ self.covariance @ self.F.T + self.Q

    def update(self, measurement):
        # Measurement residual
        y = measurement - self.H @ self.state

        # Residual covariance
        S = self.H @ self.covariance @ self.H.T + self.R

        # Kalman gain
        K = self.covariance @ self.H.T @ np.linalg.inv(S)

        # Update state estimate
        self.state = self.state + K @ y

        # Update covariance estimate
        self.covariance = (np.eye(2) - K @ self.H) @ self.covariance

    def estimate(self):
        return self.state[0], self.state[1]  # position, velocity

# 模拟参数
np.random.seed(42)
num_steps = 50
true_position = 0.0
true_velocity = 0.5
process_noise = 0.1
measurement_noise = 1.0

# 创建卡尔曼滤波器
kf = KalmanFilter(process_noise, measurement_noise)

# 存储结果用于可视化
true_positions = []
measurements = []
estimates = []
covariances = []

for step in range(num_steps):
    true_velocity += np.random.normal(0, process_noise)
    true_position += true_velocity + np.random.normal(0, process_noise)
    true_positions.append(true_position)
    # 生成带噪声的测量
    measurement = true_position + np.random.normal(0, measurement_noise)
    measurements.append(measurement)

for step in range(num_steps):
    measurement = measurements[step]

    # 卡尔曼滤波步骤
    kf.predict()
    kf.update(measurement)

    # 估计状态
    pos_est, vel_est = kf.estimate()
    estimates.append(pos_est)
    covariances.append(kf.covariance.copy())

# 创建可视化
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8), gridspec_kw={'height_ratios': [3, 1]})
plt.subplots_adjust(hspace=0.4)

# 设置坐标轴
ax1.set_xlim(-10, 60)
ax1.set_ylim(-5, 5)
ax1.set_xlabel('Position')
ax1.set_ylabel('Time Step')
ax1.set_title('Kalman Filter Tracking')

# 时间轴标签
time_labels = [str(i) for i in range(num_steps)]
ax1.set_yticks(range(num_steps))
ax1.set_yticklabels(time_labels)

# 绘制真实路径、估计和测量
ax1.plot([], [], 'r-', linewidth=2, label='True Path')
ax1.plot([], [], 'g--', linewidth=2, label='Estimates')
ax1.scatter([], [], s=50, c='purple', marker='*', label='Measurements')
# 添加不确定性椭圆
uncertainty_ellipse = ax1.plot([], [], 'b-', alpha=0.3, label='Uncertainty')[0]
ax1.legend(loc='upper left')

# 协方差可视化
ax2.set_xlim(-10, 60)
ax2.set_ylim(0, 2)
ax2.set_xlabel('Position')
ax2.set_ylabel('Variance')
ax2.set_title('Position Variance')

# 初始化绘图元素
true_line, = ax1.plot([], [], 'r-')
est_line, = ax1.plot([], [], 'g--')
meas_scatter = ax1.scatter([], [], s=50, c='purple', marker='*')
var_line, = ax2.plot([], [], 'b-')

# 动画更新函数
def update(frame):
    # 更新轨迹
    true_line.set_data(true_positions[:frame], range(frame))
    est_line.set_data(estimates[:frame], range(frame))

    # 更新测量点
    if frame > 0:
        meas_scatter.set_offsets(np.c_[measurements[:frame], range(frame)])

    # 更新不确定性椭圆
    if frame > 0:
        # 获取当前协方差
        cov = covariances[frame-1]
        # 创建椭圆
        lambda_, v = np.linalg.eig(cov[:1, :1])  # 只考虑位置方差
        lambda_ = np.sqrt(lambda_)
        theta = np.linspace(0, 2*np.pi, 100)
        x = lambda_[0] * np.cos(theta)
        y = np.zeros_like(x)
        # 变换到当前估计位置
        x += estimates[frame-1]
        y += frame
        uncertainty_ellipse.set_data(x, y)

    # 更新方差图
    if frame > 0:
        vars = [c[0,0] for c in covariances[:frame]]
        var_line.set_data(range(frame), vars)

    return true_line, est_line, meas_scatter, uncertainty_ellipse, var_line

# 创建动画
ani = animation.FuncAnimation(fig, update, frames=num_steps, interval=200, blit=True)
ani.save('kalman_filter_demo.gif', writer='pillow', fps=5)
plt.show()
