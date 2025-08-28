
            socket.close();
            // 删除该会话对应的子进程映射
            if(subprocess_map.find(session_map[client]) != subprocess_map.end()){
                for(const string& subpid:subprocess_map[session_map[client]]){
                    proof_map.erase(subpid); // 删除子进程凭证
                }
                subprocess_map.erase(session_map[client]); // 删除子进程映射
            }
            memory_container.process_container.erase(session_map[client]); // 删除进程容器