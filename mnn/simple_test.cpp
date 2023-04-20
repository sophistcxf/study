#include <fstream>
#include <iostream>
#include "MNN/Interpreter.hpp"

int main() {
    std::ifstream ifs("saved_model.mnn", std::ios_base::in | std::ios_base::binary);
    if (!ifs) {
        return -1;
    }
    ifs.seekg(0, std::ios_base::end);
    size_t size = ifs.tellg();
    ifs.seekg(0, std::ios_base::beg);
    char* buf = new char[size+1];
    memset(buf, 0, size);
    ifs.read(buf, size);
    MNN::Interpreter* interpreter = MNN::Interpreter::createFromBuffer(buf, size);
    if (interpreter == nullptr) {
        return -1;
    }
    std::cout << "load model successfully" << std::endl;
    MNN::ScheduleConfig netConfig;
    netConfig.type      = MNN_FORWARD_CPU;
    netConfig.numThread = 1;
    MNN::Session* session = interpreter->createSession(netConfig);
    std::cout << "create session successfully" << std::endl;
    MNN::Tensor* pos = interpreter->getSessionInput(session, "pos");;
    MNN::Tensor* prj_pos = interpreter->getSessionInput(session, "prj_pos");
    double pos_arr[2] = {1.0, 2.0};
    double prj_pos_arr[2] = {3.0, 10.0};
    std::copy(pos_arr, pos_arr+2, pos->host<double>());
    std::copy(prj_pos_arr, prj_pos_arr+2, prj_pos->host<double>());
    
    const std::map<std::string, MNN::Tensor*>& all_out_tensors = interpreter->getSessionOutputAll(session);
    for (auto k : all_out_tensors) {
        std::cout << k.first << std::endl;
    }
    
    const std::map<std::string, MNN::Tensor*>& all_in_tensors = interpreter->getSessionInputAll(session);
    for (auto k : all_in_tensors) {
        std::cout << k.first << std::endl;
    }
    
    MNN::Tensor* is_yaw_tensor = interpreter->getSessionOutput(session, "is_yaw");
    if (is_yaw_tensor == nullptr) {
        return -1;
    }
    bool is_yaw = (*is_yaw_tensor->host<int>()) == 1;
    std::cout << "is_yaw: " << is_yaw << std::endl;
    MNN::Tensor* dist_error_tensor = interpreter->getSessionOutput(session, "dist_error");
    if (dist_error_tensor == nullptr) {
        return -1;
    }
    double dist_error = (*dist_error_tensor->host<double>());
    std::cout << "dist_error: " << std::endl;
    return 0;
}
