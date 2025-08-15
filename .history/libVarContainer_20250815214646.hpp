#ifndef __LIBVARCONTAINER_HPP__
#define __LIBVARCONTAINER_HPP__

#include <map>
#include <string>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

struct Var{
    string Name;
    string Type;
    string Value;
    int Timestamp;
    void Sync(Var &other) {
        if(other.Timestamp>Timestamp){
            Name      = other.Name;
            Type      = other.Type;
            Value     = other.Value;
            Timestamp = other.Timestamp;
        }else{
            other.Name      = Name;
            other.Type      = Type;
            other.Value     = Value;
            other.Timestamp = Timestamp;
        }
    }

    // 添加 to_json 方法
    json to_json() const {
        json j;
        j["Name"] = Name;
        j["Type"] = Type;
        j["Value"] = Value;
        j["Timestamp"] = Timestamp;
        return j;
    }
    
    // 从 JSON 赋值的运算符重载
    Var& operator=(const json& j) {
        Name = j.value("Name", "");
        Type = j.value("Type", "");
        Value = j.value("Value", "");
        Timestamp = j.value("Timestamp", 0);
        return *this;
    }
};

struct PorcessContainer{
    map<string, Var> vars;
    int Timestamp;
    void Sync(PorcessContainer &other) {
        if(other.Timestamp > Timestamp) {
            vars.clear();
            vars=other.vars;
        }else{
            other.vars.clear();
            other.vars=vars;
        }
    }

    // 添加 to_json 方法
    json to_json() const {
        json j;
        json vars_json;
        for(const auto& [key, value] : vars) {
            vars_json[key] = value.to_json();
        }
        j["vars"] = vars_json;
        j["Timestamp"] = Timestamp;
        return j;
    }
    
    // 从 JSON 赋值的运算符重载
    PorcessContainer& operator=(const json& j) {
        vars.clear();
        if(j.contains("vars") && j["vars"].is_object()) {
            for(const auto& [key, value] : j["vars"].items()) {
                Var var;
                var = value; // 使用 Var 的赋值运算符
                vars[key] = var;
            }
        }
        Timestamp = j.value("Timestamp", 0);
        return *this;
    }
};

struct MemoryContainer {
    map <string,PorcessContainer> porcess_container;

    // 添加 to_json 方法
    json to_json() const {
        json j;
        for(const auto& [process_key, process_map] : porcess_container) {
            json process_json;
            for(const auto& [container_key, container] : process_map) {
                process_json[container_key] = container.to_json();
            }
            j[process_key] = process_json;
        }
        return j;
    }
    
    // 从 JSON 赋值的运算符重载
    MemoryContainer& operator=(const json& j) {
        porcess_container.clear();
        if(j.is_object()) {
            for(const auto& [process_key, process_map] : j.items()) {
                map<string, PorcessContainer> container_map;
                if(process_map.is_object()) {
                    for(const auto& [container_key, container] : process_map.items()) {
                        PorcessContainer pc;
                        pc = container; // 使用 PorcessContainer 的赋值运算符
                        container_map[container_key] = pc;
                    }
                }
                porcess_container[process_key] = container_map;
            }
        }
        return *this;
    }
};

#endif // __LIBVARCONTAINER_HPP__