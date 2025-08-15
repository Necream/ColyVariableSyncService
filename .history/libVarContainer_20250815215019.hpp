#ifndef __LIBVARCONTAINER_HPP__
#define __LIBVARCONTAINER_HPP__

#include <map>
#include <string>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// 变量结构体
struct Var {
    string Name;
    string Type;
    string Value;
    int Timestamp;

    // 同步方法
    void Sync(Var& other) {
        if (other.Timestamp > Timestamp) {
            *this = other;
        } else {
            other = *this;
        }
    }

    // JSON 序列化
    json to_json() const {
        return json{
            {"Name", Name},
            {"Type", Type},
            {"Value", Value},
            {"Timestamp", Timestamp}
        };
    }
    
    // JSON 反序列化
    Var& operator=(const json& j) {
        Name = j.value("Name", "");
        Type = j.value("Type", "");
        Value = j.value("Value", "");
        Timestamp = j.value("Timestamp", 0);
        return *this;
    }
};

// 进程容器结构体
struct ProcessContainer {
    map<string, Var> Vars;
    int Timestamp;

    // 同步方法
    void Sync(ProcessContainer& other) {
        if (other.Timestamp > Timestamp) {
            *this = other;
        } else {
            other = *this;
        }
    }

    // JSON 序列化
    json to_json() const {
        json vars_json;
        for (const auto& [key, value] : Vars) {
            vars_json[key] = value.to_json();
        }
        return json{
            {"vars", vars_json},
            {"Timestamp", Timestamp}
        };
    }
    
    // JSON 反序列化
    ProcessContainer& operator=(const json& j) {
        Vars.clear();
        if (j.contains("vars") && j["vars"].is_object()) {
            for (const auto& [key, value] : j["vars"].items()) {
                Vars[key] = value;
            }
        }
        Timestamp = j.value("Timestamp", 0);
        return *this;
    }
};

// 内存容器结构体
struct MemoryContainer {
    map<string, ProcessContainer> process_container;  // 修正了拼写错误
    
    // JSON 序列化
    json to_json() const {
        json j;
        for (const auto& [process_key, container] : process_container) {
            j[process_key] = container.to_json();
        }
        return j;
    }
    
    // JSON 反序列化
    MemoryContainer& operator=(const json& j) {
        process_container.clear();
        if (j.is_object()) {
            for (const auto& [key, value] : j.items()) {
                ProcessContainer pc;
                pc = value;
                process_container[key] = pc;
            }
        }
        return *this;
    }

    // 清除所有数据
    void clear() {
        process_container.clear();
    }
};

#endif // __LIBVARCONTAINER_HPP__