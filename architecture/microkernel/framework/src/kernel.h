//
//  kernel.hpp
//  microkernel
//
//  Created by xiangfei.cui on 2023/5/10.
//

#ifndef kernel_hpp
#define kernel_hpp

#include <stdio.h>

#ifndef KERNEL_H
#define KERNEL_H

#include <unordered_map>
#include <queue>
#include <unordered_set>
#include "plugin_points.h"

class Signal;

class IKernel {
public:
    virtual ~IKernel() {}
    virtual void setSignal(const Signal* signal) = 0;
    virtual bool setConfig(const std::string& config) = 0;
};


template <typename T, typename E>
class Vertex {
public:
    typedef Vertex<T, E> VertexType;
public:
    Vertex(std::string i, T* value = nullptr) : identifier_(i), value_(value) {}
    bool addAdj(VertexType* adj, E* edge, bool update = true) {
        auto it = adj_vertexes_.find(adj);
        if (it != adj_vertexes_.end()) {
            if (!update) {
                return false;
            }
            if (it->second != nullptr) {
                delete it->second;
                it->second = nullptr;
            }
        }
        adj_vertexes_[adj] = edge;
        return true;
    }
    
    T* value() {
        return value_;
    }
    
    // 本节点的出结点及边指向的结构体
    T* value_;
    std::unordered_map<VertexType*, E*> adj_vertexes_;
    std::string identifier_;
};

template <typename T, typename E>
class Graph {
public:
    typedef Vertex<T, E> VertexType;
public:
    Graph() {}
    void addVertex(T* vertex) {
        if (vertexes_.find(vertex) == vertexes_.end()) {
            vertexes_[vertex] = new Vertex<T, E>("", vertex);
        }
        bfs_.clear();
    }
    bool addEdge(T* v1, T* v2, E* edge, bool add_vertex = true, bool update = true) {
        auto v1_it = vertexes_.find(v1);
        if (v1_it == vertexes_.end()) {
            if (!add_vertex) {
                return false;
            }
            addVertex(v1);
            v1_it = vertexes_.find(v1);
        }
        auto v2_it = vertexes_.find(v2);
        if (v2_it == vertexes_.end()) {
            if (!add_vertex) {
                return false;
            }
            addVertex(v2);
            v2_it = vertexes_.find(v2);
        }
        bfs_.clear();
        return v1_it->second->addAdj(v2_it->second, edge);
    }
    /**
     * 生成多起点广度优先的序列
     */
    const std::vector<VertexType*>& getBFS() {
        if (!bfs_.empty()) {
            return bfs_;
        }
        std::unordered_map<VertexType*, std::unordered_set<VertexType*> > in_not_accessed;
        std::unordered_map<VertexType*, bool> is_accessed;
        std::queue<VertexType*> q;
        
        for (const auto& vertex : vertexes_) {
           in_not_accessed[vertex.second] = std::unordered_set<VertexType*>();
        }

        for (const auto& vertex : vertexes_) {
            VertexType* v = vertex.second;
            is_accessed.insert(std::make_pair(v, false));
            q.push(v);
            for (const auto& out_vertex : v->adj_vertexes_) {
                in_not_accessed[out_vertex.first].insert(v);
            }
        }
        bool is_empty = q.empty();
        while (!is_empty) {
           VertexType* t = q.front();
           q.pop();
            is_empty = q.empty();
           if (is_accessed[t]) {
             continue;
           }
           if (!in_not_accessed[t].empty()) {
             q.push(t);
             continue;
           }
            bfs_.push_back(t);
           is_accessed[t] = true;
            for (const auto& out_vertex : t->adj_vertexes_) {
                in_not_accessed[out_vertex.first].erase(t);
                q.push(out_vertex.first);
            }
        }
        return bfs_;
    }
private:
    std::unordered_map<T*, VertexType*> vertexes_;
    std::vector<VertexType*> bfs_;
};

// 多起点有向图
// 调用的过程，即是广度优先遍历的过程
class ExecuteGraph {
public:
    void addExecuter(IProcessPlugin* plugin) {
        graph_.addVertex(plugin);
    }
    
    void addDependency(IProcessPlugin* dependent, IProcessPlugin* to) {
        graph_.addEdge(to, dependent, nullptr);
    }
    const std::vector<IProcessPlugin*>& executeSerial() {
        if (!serial_.empty()) {
            return serial_;
        }
        const std::vector<GraphType::VertexType*>& serial = graph_.getBFS();
        for (auto v : serial) {
            serial_.push_back(v->value());
        }
        return serial_;
    }
private:
    typedef Graph<IProcessPlugin, void> GraphType;
    GraphType graph_;
    std::vector<IProcessPlugin*> serial_;
};

class AsynPluginWrapper {
public:
    virtual void setSignal(const Signal* signal);
};

struct PluginMetaInfo {
    std::string name;
    std::vector<std::string> dependency;
    std::string thread_name = "main";
};

class Kernel : public IKernel {
public:
    virtual void setSignal(const Signal* signal);
    virtual bool setConfig(const std::string& config);
private:
    ExecuteGraph graph_;
    
    IPathManager* path_manager_ = nullptr;
    std::vector<IYawJudgePlugin*> yaw_judgers_;
    IYawMergePlugin* yaw_merger_ = nullptr;
    std::vector<IRoadMatchSDPlugin*> road_matchers_;
    IRouteMatchSDPlugin* route_matcher_ = nullptr;
    std::vector<IPositionPlugin*> position_provider_;
    
    std::vector<PluginMetaInfo> meta_infos_;
};

#endif


#endif /* kernel_hpp */
