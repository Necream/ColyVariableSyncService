#include <iostream>
#include <memory>
#include <set>
#include <functional>
#include <string>
#include <cstring>
// 添加ASIO的独立模式定义
#define ASIO_STANDALONE
#include "asio.hpp"
#include "libVarContainer.hpp" // 包含 Var 和 PorcessContainer 的定义

using asio::ip::tcp;
using namespace std;

string GetPrefix(const string& str,size_t length){
    // 如果 length 大于字符串长度，就直接返回整个字符串
    if (length>=str.size()){
        return str;
    }
    return str.substr(0,length);
}
struct Operation{
    string OperationValue;
    int id;
};
vector<Operation> operations;
MemoryContainer memory_container; // 全局内存容器
void OperationInit(){
    operations.push_back({"set",1});
    operations.push_back({"get",2});
    operations.push_back({"del",3});
    operations.push_back({"sync",4});
    operations.push_back({"reg",5});
    operations.push_back({"login",6});
    operations.push_back({"process",1});
    operations.push_back({"var",2});
    operations.push_back({"subprocess",3});
}
string CommandExecutor(string command){
    int operation_id=0;
    for(const auto op:operations){
        if(GetPrefix(command,op.OperationValue.size())==op.OperationValue){
            operation_id*=10;
            operation_id+=op.id;
            command.erase(0,min(command.size(),op.OperationValue.size()+1)); // 去掉操作前缀
        }
    }
    if(operation_id==0){
        cout<<"[ERROR]Unknown command: "<<command<<endl;
        return "[ERROR]Unknown command"+command;
    }
    if(operation_id==1){ // set
        return "[ERROR]Can't set memory directly, use specific commands like set process or set var.";
    }
    if(operation_id==2){ // get
        return "[ERROR]Can't get memory directly, use specific commands like get process or get var.";
    }
    if(operation_id==3){ // del
        return "[ERROR]Can't delete memory directly, use specific commands like del process or del var.";
    }
    if(operation_id==4){ // sync
        return "[ERROR]Can't sync memory directly, use specific commands like sync process or sync var.";
    }
    if(operation_id==5){ // reg
        return "[ERROR]";
    }
    if(operation_id==11){ // set process
        string processid="";
        for(char c:command){
            if(c==' '){
                break;
            }
            processid+=c;
        }
        command.erase(0,processid.size()+1);
        json j = json::parse(command);
        ProcessContainer pc;
        pc.from_json(j);  // 使用 from_json 替代赋值
        memory_container.process_container[processid] = pc;
        return "Process operation completed";
    }
    if(operation_id==12){ // set var
        string processid="";
        for(char c:command){
            if(c==' '){
                break;
            }
            processid+=c;
        }
        command.erase(0,processid.size()+1);
        string varid="";
        for(char c:command){
            if(c==' '){
                break;
            }
            varid+=c;
        }
        command.erase(0,varid.size()+1);
        json j = json::parse(command);
        Var v;
        v.from_json(j);  // 使用 from_json 替代赋值
        memory_container.process_container[processid].Vars[varid] = v;
        return "Var operation completed";
    }
    if(operation_id==21){ // get process
        string processid=command;
        if(memory_container.process_container.find(processid)==memory_container.process_container.end()){
            return "[ERROR]Process not found";
        }
        json j=memory_container.process_container[processid].to_json();
        return j.dump();
    }
    if(operation_id==22){ // get var
        string processid="";
        for(char c:command){
            if(c==' '){
                break;
            }
            processid+=c;
        }
        command.erase(0,processid.size()+1);
        string varid=command;
        if(memory_container.process_container.find(processid)==memory_container.process_container.end()){
            return "[ERROR]Process not found";
        }
        if(memory_container.process_container[processid].Vars.find(varid)==memory_container.process_container[processid].Vars.end()){
            return "[ERROR]Var not found";
        }
        json j=memory_container.process_container[processid].Vars[varid].to_json();
        return j.dump();
    }
    if(operation_id==31){ // del process
        string processid=command;
        if(memory_container.process_container.find(processid)==memory_container.process_container.end()){
            return "[ERROR]Process not found";
        }
        memory_container.process_container.erase(processid);
        return "Process deleted";
    }
    if(operation_id==32){ // del var
        string processid="";
        for(char c:command){
            if(c==' '){
                break;
            }
            processid+=c;
        }
        command.erase(0,processid.size()+1);
        string varid=command;
        if(memory_container.process_container.find(processid)==memory_container.process_container.end()){
            return "[ERROR]Process not found";
        }
        if(memory_container.process_container[processid].Vars.find(varid)==memory_container.process_container[processid].Vars.end()){
            return "[ERROR]Var not found";
        }
        memory_container.process_container[processid].Vars.erase(varid);
        return "Var deleted";
    }
    if(operation_id==41){ // sync process
        string processid="";
        for(char c:command){
            if(c==' '){
                break;
            }
            processid+=c;
        }
        command.erase(0,processid.size()+1);
        if(memory_container.process_container.find(processid)==memory_container.process_container.end()){
            return "[ERROR]Process not found";
        }
        json j = json::parse(command);
        ProcessContainer new_pc;
        new_pc.from_json(j);  // 使用 from_json 替代赋值
        memory_container.process_container[processid].Sync(new_pc);
        return "Process sync completed";
    }
    if(operation_id==42){ // sync var
        string processid="";
        for(char c:command){
            if(c==' '){
                break;
            }
            processid+=c;
        }
        command.erase(0,processid.size()+1);
        string varid="";
        for(char c:command){
            if(c==' '){
                break;
            }
            varid+=c;
        }
        command.erase(0,varid.size()+1);
        if(memory_container.process_container.find(processid)==memory_container.process_container.end()){
            return "[ERROR]Process not found";
        }
        if(memory_container.process_container[processid].Vars.find(varid)==memory_container.process_container[processid].Vars.end()){
            return "[ERROR]Var not found";
        }
        json j = json::parse(command);
        Var new_var;
        new_var.from_json(j);  // 使用 from_json 替代赋值
        memory_container.process_container[processid].Vars[varid].Sync(new_var);
        return "Var sync completed";
    }
    cout<<"[ERROR]Unknown command: "<<command<<endl;
    return "[ERROR]Unknown command"+command;
}

// 会话类
struct ServerSession : enable_shared_from_this<ServerSession>{
    tcp::socket socket;
    char read_buf[512];
    set<shared_ptr<ServerSession>>& clients;

    ServerSession(tcp::socket sock, set<shared_ptr<ServerSession>>& all_clients)
        : socket(move(sock)), clients(all_clients) {}

    // 启动会话
    void start() {
        read_message();
    }

    // 读取数据
    void read_message() {
        auto self = shared_from_this();
        socket.async_read_some(asio::buffer(read_buf),
            [this, self](error_code ec, size_t length) {
                if (!ec) {
                    string msg(read_buf, length);

                    // 回显固定消息
                    send_message(CommandExecutor(msg));

                    // 继续读取
                    read_message();
                } else {
                    cout << "Client disconnected\n";
                    close();
                }
            });
    }

    // 发送数据
    void send_message(const string& msg) {
        auto self = shared_from_this();
        asio::async_write(socket, asio::buffer(msg),
            [this, self](error_code ec, size_t /*length*/) {
                if (ec) {
                    close();
                }
            });
    }

    // 关闭连接
    void close() {
        clients.erase(shared_from_this());
        socket.close();
    }
};

// 接受新连接
void start_accepting(asio::io_context& io, tcp::acceptor& acceptor, set<shared_ptr<ServerSession>>& clients) {
    acceptor.async_accept(
        [&](error_code ec, tcp::socket socket) {
            if (!ec) {
                cout << "New client connected\n";
                auto session = make_shared<ServerSession>(move(socket), clients);
                clients.insert(session);
                session->start();
            }
            start_accepting(io, acceptor, clients); // 继续等待
        });
}

int main() {
    OperationInit(); // 初始化操作列表
    try{
        asio::io_context io;
        int port = 12345; // 监听端口
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), port));
        set<shared_ptr<ServerSession>> clients;

        cout<<"Server running on port "<<port<<"...\n";
        start_accepting(io, acceptor, clients);

        io.run();

    }catch(exception& e){
        cerr<<"Exception: "<<e.what()<<"\n";
    }
}
