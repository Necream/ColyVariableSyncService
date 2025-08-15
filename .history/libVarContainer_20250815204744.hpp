#ifndef __LIBVARCONTAINER_HPP__
#define __LIBVARCONTAINER_HPP__

#include <map>
#include <string>
using namespace std;
struct Var{
    string Name;
    string Type;
    string Value;
    int Timestamp;
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
    int Timestamp;
    void Sync(PorcessContainer &other) {
        if(other.Timestamp > Timestamp) {
            vars.clear();
            
        }else{
            
        }
    }
};
struct MemoryContainer {
    map <string,map<string,PorcessContainer>> porcess_container;
};

#endif // __LIBVARCONTAINER_HPP__