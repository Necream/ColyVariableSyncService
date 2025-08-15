#ifndef __LIBVARCONTAINER_HPP__
#define __LIBVARCONTAINER_HPP__

#include <map>
#include <string>
using namespace std;
struct Var{
    string Name;
    string Type;
    string Value;
    string Timestamp;
    void Sync(Var &other) {
        if(other.Timestamp > Timestamp) {
            Name = other.Name;
            Type = other.Type;
            Value = other.Value;
            Timestamp = other.Timestamp;
        }else{
            other.Name = Name;
            other.Type = Type;
            other.Value = Value;
            other.Timestamp = Timestamp;
        }
    }
};
struct PorcessContainer{
    map<string, Var> vars;
    int 
    void Sync(PorcessContainer &other) {
        for(auto &it : other.vars) {
            if(vars.find(it.first) != vars.end()) {
                vars[it.first].Sync(it.second);
            }else{
                vars[it.first] = it.second;
            }
        }
        for(auto &it : vars) {
            if(other.vars.find(it.first) == other.vars.end()) {
                other.vars[it.first] = it.second;
            }
        }
    }
};
struct MemoryContainer {
    map <string,map<string,PorcessContainer>> porcess_container;
};

#endif // __LIBVARCONTAINER_HPP__