#pragma once
#include <string>
#include <map>
#include <functional>
#include <thread>
#include <atomic>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

class SimpleHTTPServer {
private:
    int server_fd;
    struct sockaddr_in address;
    int addrlen;
    std::atomic<bool> running;
    std::thread server_thread;
    
    // Route handlers
    std::map<std::string, std::function<std::string(const std::string&)>> routes;
    
public:
    SimpleHTTPServer(int port = 8080);
    ~SimpleHTTPServer();
    
    bool start();
    void stop();
    void add_route(const std::string& path, std::function<std::string(const std::string&)> handler);
    
private:
    void server_loop();
    std::string handle_request(const std::string& request);
    std::string parse_request(const std::string& request, std::string& method, std::string& path);
    std::string create_response(const std::string& body, const std::string& content_type = "text/html");
    std::string url_decode(const std::string& str);
};
