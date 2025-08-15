#include <iostream>
#include <memory>
#include <set>
#include <functional>
#include <string>
#include <cstring>
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
void OperationInit(){
    operations.push_back({"set ",1});
    operations.push_back({"get ",2});
    operations.push_back({"del ",3});
    operations.push_back({"sync ",4});
}
void CommandExecutor(string command){
    int operation_id=0;
    for(const auto op:operations){
        if(GetPrefix(command,op.OperationValue.size())==op.OperationValue){
            operation_id*=10;
            operation_id+=op.id;
            command.erase(0,op.OperationValue.size()); // 去掉操作前缀
        }
    }
    
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
                    cout << "Received: " << msg << "\n";

                    // 回显固定消息
                    send_message("111");

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
