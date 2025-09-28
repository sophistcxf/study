import numpy as np
import open3d as o3d
import matplotlib.pyplot as plt
from scipy.spatial import KDTree

def draw_registration_result(source, target, transformation=None, window_name="Registration Result"):
    """可视化配准结果"""
    source_temp = source.clone()
    target_temp = target.clone()

    # 为点云着色以便区分
    source_temp.paint_uniform_color([1, 0.706, 0])  # 源点云 - 橙色
    target_temp.paint_uniform_color([0, 0.651, 0.929])  # 目标点云 - 蓝色

    if transformation is not None:
        source_temp.transform(transformation)

    o3d.visualization.draw_geometries(
        [source_temp, target_temp],
        window_name=window_name,
        width=800,
        height=600
    )

def icp_point_to_point(source, target, max_iterations=50, tolerance=1e-6, distance_threshold=0.05):
    """
    ICP 算法实现 (点对点版本)

    参数:
    source: 源点云 (Open3D PointCloud)
    target: 目标点云 (Open3D PointCloud)
    max_iterations: 最大迭代次数
    tolerance: 收敛容差 (误差变化量阈值)
    distance_threshold: 距离阈值 (剔除距离过大的点对)

    返回:
    transformation: 最终变换矩阵 (4x4)
    history: 包含每次迭代信息的字典
    """
    # 初始化变换矩阵
    transformation = np.identity(4)
    source_points = np.asarray(source.points)

    # 准备目标点云的KDTree用于快速最近邻搜索
    target_tree = KDTree(np.asarray(target.points))

    # 存储历史信息
    history = {
        'error': [],
        'transformation': []
    }

    prev_error = 0
    for i in range(max_iterations):
        # 应用当前变换
        transformed_points = np.dot(
            np.hstack((source_points, np.ones((len(source_points), 1)))),
            transformation.T
        )[:, :3]

        # 寻找最近点
        distances, indices = target_tree.query(transformed_points)

        # 剔除距离过大的点对
        valid = distances < distance_threshold
        if np.sum(valid) < 3:  # 至少需要3个点才能求解变换
            print("警告: 有效对应点太少")
            break

        q = np.asarray(target.points)[indices[valid]]
        p = transformed_points[valid]

        # 计算当前误差 (MSE)
        error = np.mean(distances[valid]**2)
        history['error'].append(error)
        history['transformation'].append(transformation.copy())

        # 检查收敛条件
        if np.abs(prev_error - error) < tolerance:
            print(f"在 {i+1} 次迭代后收敛")
            break
        prev_error = error

        # 计算质心
        centroid_p = np.mean(p, axis=0)
        centroid_q = np.mean(q, axis=0)

        # 计算去质心坐标
        p_centered = p - centroid_p
        q_centered = q - centroid_q

        # 计算协方差矩阵
        H = np.dot(p_centered.T, q_centered)

        # 奇异值分解
        U, _, Vt = np.linalg.svd(H)
        R = np.dot(Vt.T, U.T)

        # 处理反射情况
        if np.linalg.det(R) < 0:
            Vt[2, :] *= -1
            R = np.dot(Vt.T, U.T)

        # 计算平移
        t = centroid_q - np.dot(R, centroid_p)

        # 更新当前变换
        current_transformation = np.identity(4)
        current_transformation[:3, :3] = R
        current_transformation[:3, 3] = t

        # 更新累积变换
        transformation = np.dot(current_transformation, transformation)

        # 每5次迭代可视化一次
        if (i % 5 == 0) or (i == max_iterations - 1):
            draw_registration_result(
                source,
                target,
                transformation,
                window_name=f"Iteration {i+1}"
            )

    return transformation, history

# 主程序
if __name__ == "__main__":
    print("1. 加载点云数据...")
    # 加载源点云和目标点云 (这里使用Open3D内置示例)
    source = o3d.data.DemoICPPointClouds().point_clouds[0]
    target = o3d.data.DemoICPPointClouds().point_clouds[1]

    print("2. 初始位置可视化...")
    draw_registration_result(source, target, np.identity(4), "Initial Position")

    print("3. 运行ICP算法...")
    transformation, history = icp_point_to_point(
        source,
        target,
        max_iterations=30,
        distance_threshold=0.05
    )

    print("4. 最终变换矩阵:")
    print(transformation)

    print("5. 最终配准结果...")
    draw_registration_result(source, target, transformation, "Final Registration")

    # 绘制误差收敛曲线
    plt.figure(figsize=(10, 6))
    plt.plot(history['error'], 'o-', markersize=4)
    plt.title('ICP Convergence')
    plt.xlabel('Iteration')
    plt.ylabel('Mean Squared Error (MSE)')
    plt.grid(True)
    plt.show()
