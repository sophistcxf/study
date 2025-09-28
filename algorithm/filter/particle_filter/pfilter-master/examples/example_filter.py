# %%
# press ESC to exit the demo!
from pfilter import (
    ParticleFilter,
    gaussian_noise,
    cauchy_noise,
    t_noise,
    squared_error,
    independent_sample,
)
import numpy as np

# testing only
from scipy.stats import norm, gamma, uniform
import skimage.draw
import cv2


img_size = 48


'''
生成图像的二进制
'''
def blob(x):
    """Given an Nx3 matrix of blob positions and size, 
    create N img_size x img_size images, each with a blob drawn on 
    them given by the value in each row of x
    
    One row of x = [x,y,radius]."""
    y = np.zeros((x.shape[0], img_size, img_size))
    for i, particle in enumerate(x):
        rr, cc = skimage.draw.ellipse(
            particle[0], particle[1], max(particle[2], 1), max(particle[2], 1), shape=(img_size, img_size)
        )
        y[i, rr, cc] = 1
    return y


# %%

# names (this is just for reference for the moment!)
columns = ["x", "y", "radius", "dx", "dy"]


# prior sampling function for each variable
# (assumes x and y are coordinates in the range 0-img_size)
prior_fn = independent_sample(
    [
        norm(loc=img_size / 2, scale=img_size / 2).rvs,
        norm(loc=img_size / 2, scale=img_size / 2).rvs,
        gamma(a=1, loc=0, scale=10).rvs,
        norm(loc=0, scale=0.5).rvs,
        norm(loc=0, scale=0.5).rvs,
    ]
)

# 对x进行矩阵运算
# x是N*5的矩阵，每一行是(x,y,s,dx,dy)
# 返回的xp是(x+dx,y+dy,s,dx,dy)
def velocity(x):
    dt = 1.0
    xp = (
        x
        @ np.array(
            [
                [1, 0, 0, dt, 0],
                [0, 1, 0, 0, dt],
                [0, 0, 1, 0, 0],
                [0, 0, 0, 1, 0],
                [0, 0, 0, 0, 1],
            ]
        ).T
    )

    return xp


def example_filter():
    # create the filter
    # observer_fn是从internal state转为sensor state
    # dynamics_fn，每个粒子按此函数运行
    # dynamics_fn，粒子的运动函数
    # noise_fn，粒子按dynamics_fn运行后，添加的噪声
    # resample_proportion，多少比例的粒子，是从原始的prior_fn中进行采样的
    pf = ParticleFilter(
        prior_fn=prior_fn,
        observe_fn=blob,
        n_particles=100,
        dynamics_fn=velocity,
        noise_fn=lambda x: t_noise(x, sigmas=[0.15, 0.15, 0.05, 0.05, 0.15], df=100.0),
        weight_fn=lambda x, y: squared_error(x, y, sigma=2),
        resample_proportion=0.05,
        column_names=columns,
    )

    # np.random.seed(2018)
    # start in centre, random radius
    s = np.random.uniform(2, 8)

    # 被追踪的目标，一开始在中心，向dx,dy方向移动
    x = img_size // 2
    y = img_size // 2
    #dx = np.random.uniform(-0.25, 0.25)
    #y = np.random.uniform(-0.25, 0.25)
    dx = 0.1
    dy = 0.1
    scale_factor = 20

    # create window
    cv2.namedWindow("samples", cv2.WINDOW_NORMAL)
    cv2.resizeWindow("samples", scale_factor * img_size, scale_factor * img_size)

    for i in range(1000):
        # low_res_img是目标的位置，以此为观测，更新滤波器
        low_res_img = blob(np.array([[x, y, s]]))
        pf.update(low_res_img)

        # resize for drawing onto
        img = cv2.resize(
            np.squeeze(low_res_img), (0, 0), fx=scale_factor, fy=scale_factor
        )

        cv2.putText(
            img,                            # 图片
            "ESC to exit",                  # 添加的文字
            (50, 50),                       # 左上角的坐标
            cv2.FONT_HERSHEY_SIMPLEX,       # 字体
            1,                              # 字体大小
            (255, 255, 255),                # 字体颜色
            2,                              # 字体粗细
            cv2.LINE_AA,
        )

        # 转换颜色空间
        color = cv2.cvtColor(img.astype(np.float32), cv2.COLOR_GRAY2RGB)

        # 获取估计结果
        x_hat, y_hat, s_hat, dx_hat, dy_hat = pf.mean_state

        blue = (1, 0, 0)
        green = (0,1,0)
        red = (0,0,1)

        # draw individual particles
        # 绘制粒子
        for particle in pf.original_particles:

            xa, ya, sa, _, _ = particle
            sa = np.clip(sa, 1, 100)
            cv2.circle(
                color,          # 圆画在哪里
                (int(ya * scale_factor), int(xa * scale_factor)),   # 圆心
                max(int(sa * scale_factor), 1),                     # 半径
                blue,                                          # 颜色
                1,                                                  # 线的宽度
            )

        # x,y exchange because of ordering between skimage and opencv
        # 绘制估计位置
        cv2.circle(
            color,
            (int(y_hat * scale_factor), int(x_hat * scale_factor)),
            max(int(sa * scale_factor), 1),
            green,
            1,
            lineType=cv2.LINE_AA,   # 反锯齿线条
        )

        #
        cv2.line(
            color,
            (int(y_hat * scale_factor), int(x_hat * scale_factor)),         # 起点，估计的圆心
            (
                int(y_hat * scale_factor + 5 * dy_hat * scale_factor),
                int(x_hat * scale_factor + 5 * dx_hat * scale_factor),
            ),                                                              # 终点，估计的方向
            red,
            lineType=cv2.LINE_AA,
        )

        cv2.imshow("samples", color)
        result = cv2.waitKey(20)
        # break on escape
        if result == 27:
            break
        x += dx
        y += dy

    cv2.destroyAllWindows()


if __name__ == "__main__":
    example_filter()

# %%
