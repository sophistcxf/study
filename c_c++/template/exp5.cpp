#include <iostream>

enum class EventType {
    kUndefined = 0,
    kGnss = 1,
    kRtk = 2,
    kGyr = 3,
    kAcc = 4,
    kMgc = 5,
    kScene = 6
};

class InputEvent {
public:
    int event_type;
    int64_t event_born_ticktime;
};

int main() {
    static_assert(std::is_pod<InputEvent>::value, "dd");
    return 0;
}