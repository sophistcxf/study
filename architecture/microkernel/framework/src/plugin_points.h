#ifndef PLUGIN_POINTS_H
#define PLUGIN_POINTS_H

#include <string>
#include <vector>

class Signal;

/**
 * 需要被信号调度的插件
 */
class IProcessPlugin {
public:
    virtual void setSignal(const Signal* signal) = 0;
};

/**
 * 所有的信息都必须实现 ticktime_，ticktime_ 是信息对齐的唯一方式，精度到毫秒
 */
class CommonMessage {
public:
    int64_t ticktime() {
        return ticktime_;
    }
protected:
    int64_t ticktime_ = -1;
};

class YawJudgeResult : public CommonMessage {
public:
    bool is_yaw;
    std::string reason;
};

class IYawJudgePlugin : public IProcessPlugin {
public:
    virtual ~IYawJudgePlugin() {}
    virtual const YawJudgeResult& yawResult() const = 0;
};

class IYawMergePlugin {
public:
    virtual ~IYawMergePlugin() {}
    virtual const YawJudgeResult& merge() = 0;
};

class Position {
public:
    double lon;
    double lat;
    double alt;
    float speed;
    float course;
};

class IPositionPlugin {
public:
    virtual const Position& getPosition() const = 0;
};

class RoadMatchResult {
public:
    uint64_t linkid;
};

class IRoadMatchSDPlugin {
public:
    virtual ~IRoadMatchSDPlugin() {}
    virtual const RoadMatchResult& getMatchResult() const = 0;
};

class RouteMatchResult {
public:
    uint64_t linkid = 0;
    int32_t segm_idx = 0;
    int32_t link_idx = 0;
    int32_t coor_idx = 0;
};

class RouteMatchResultAllRoutes {
public:
    std::vector<RouteMatchResult> match_results;
};

class IRouteMatchSDPlugin : public IProcessPlugin {
public:
    virtual ~IRouteMatchSDPlugin() {}
    virtual const RouteMatchResultAllRoutes& getRouteMatchResult() const = 0;
};

class IPathManager : public IProcessPlugin{
public:
    virtual ~IPathManager() {}
};

class InsertResult {
public:
};

class IInsertPlugin {
    
};

class IPositionOutput {
public:
    void addObserver();
    void removeObserver();
private:
    IInsertPlugin* insert_ = nullptr;
};

class ISignalManager {
public:
    
};

class DataManager {
public:
    
};

#endif
