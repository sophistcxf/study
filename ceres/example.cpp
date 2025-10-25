#include "ceres/ceres.h"
#include "glog/logging.h"

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;

// 一个简单的代价函数，计算残差 r = 10 - x
struct CostFunctor {
  template <typename T>
  bool operator()(const T* const x, T* residual) const {
    residual[0] = T(10.0) - x[0];
    return true;
  }
};

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);

  // 初始值
  double x = 0.5;
  const double initial_x = x;

  // 构建优化问题
  Problem problem;

  // 设置代价函数(使用自动微分)
  CostFunction* cost_function =
      new AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor);
  problem.AddResidualBlock(cost_function, nullptr, &x);

  // 运行求解器
  Solver::Options options;
  options.minimizer_progress_to_stdout = true;
  Solver::Summary summary;
  Solve(options, &problem, &summary);

  // 输出结果
  std::cout << summary.BriefReport() << "\n";
  std::cout << "x : " << initial_x << " -> " << x << "\n";

  return 0;
}
