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
    void Sync(const Var& other) {
        if()
    }
};
struct PorcessContainer{
    map<string, Var> vars;
};
struct MemoryContainer {
    map <string,map<string,PorcessContainer>> porcess_container;
};

#endif // __LIBVARCONTAINER_HPP__