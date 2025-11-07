#include <iostream>
#include "Eigen/Core"
int main() {
    Eigen::MatrixXd m = Eigen::MatrixXd::Random(5,5);
    std::cout << m << std::endl;
    return 0;
}
