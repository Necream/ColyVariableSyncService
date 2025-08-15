#ifndef __LIBVARCONTAINER_HPP__
#define __LIBVARCONTAINER_HPP__

#include <map>
#include <string>
struct memory_var_container {
    std::map<std::string, std::string> vars;

    // 添加或更新变量
    void set(const std::string& key, const std::string& value) {
        vars[key] = value;
    }

    // 获取变量值
    std::string get(const std::string& key) const {
        auto it = vars.find(key);
        if (it != vars.end()) {
            return it->second;
        }
        return {};
    }

    // 检查变量是否存在
    bool exists(const std::string& key) const {
        return vars.find(key) != vars.end();
    }

    // 删除变量
    void remove(const std::string& key) {
        vars.erase(key);
    }

}

#endif // __LIBVARCONTAINER_HPP__