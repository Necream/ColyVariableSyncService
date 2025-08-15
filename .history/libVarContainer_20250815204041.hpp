#ifndef __LIBVARCONTAINER_HPP__
#define __LIBVARCONTAINER_HPP__

#include <map>
#include <string>
using namespace std;
struct Var{
    string Name;
    string Value;
    string Timestamp;
};
struct PorcessContainer{
    map<string, Var> vars;
};
struct MemoryContainer {
    map <string,map<string,PorcessContainer>> porcess_container;
    void 
};

#endif // __LIBVARCONTAINER_HPP__