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
};
struct MemoryContainer {
    map <string,map<string,PorcessContainer>> porcess_container;

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
};

#endif // __LIBVARCONTAINER_HPP__