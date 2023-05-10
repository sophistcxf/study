#include "plugin_points.h"
#include "kernel.h"
#include <fstream>

bool Kernel::setConfig(const std::string &config) {
    std::ifstream ifs("plugin_config.json", std::ios_base::in);
    ifs.seekg(0, std::ios_base::end);
    size_t len = ifs.tellg();
    ifs.seekg(0, std::ios_base::beg);
    char* buf = new char[len+1];
    memset(buf, 0, len+1);
    ifs.read(buf, len);
    return true;
}

void Kernel::setSignal(const Signal* signal) {
    const std::vector<IProcessPlugin*>& serial = graph_.executeSerial();
    for (auto plugin : serial) {
        plugin->setSignal(signal);
    }
}
