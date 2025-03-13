#include <fstream>
#include <iostream>
#include "MNN_generated.h"

void decode() {
    std::ifstream in("pos_fusion_gru.mnn", std::ios::binary);
    in.seekg(0, std::ios::end);
    auto size = in.tellg();
    in.seekg(0, std::ios::beg);
    uint8_t* out_buf = new uint8_t[size];
    in.read(reinterpret_cast<char*>(out_buf), size);
    auto net = MNN::GetNet(out_buf);
    std::cout << net->oplists()->size() << std::endl;
    for (auto op : *(net->oplists())) {
        std:: cout << "op type " << op->type() << std::endl;
    }
}
int main() {
    decode();
    return 0;
}