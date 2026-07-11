/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Net.hpp
*/

#ifndef NET_HPP
    #define NET_HPP
    #include <cstdint>
    #include <string>
    #include <vector>
    #include <poll.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <cstring>
    #include <fstream>
    #include <sstream>

    #include "Error.hpp"
    #include "printer.hpp"

namespace raytracer {

class TcpSocket {
    int _fd = -1;

public:
    TcpSocket() = default;
    TcpSocket(int fd): _fd(fd) {};
    ~TcpSocket();

    TcpSocket(TcpSocket&& other) noexcept;
    TcpSocket& operator=(TcpSocket&& other) noexcept;

    void Connect(const std::string& ip, std::uint16_t port);
    void Listen(std::uint16_t port, int backlog = 10);
    TcpSocket Accept() const;

    std::size_t Send(const std::string& message) const;
    void SendAll(const char *data, std::size_t len) const;
    std::string Receive() const;
    std::size_t ReceiveBlock(char *buf, std::size_t cap) const;
    void ReceiveExact(char *buf, std::size_t n) const;

    std::uint16_t LocalPort() const;
    std::string LocalIp() const;

    int Fd() const {return _fd;};
    bool IsOpen() const {return _fd != -1;};
    void Close();
};

class Poller {
    std::vector<pollfd> _polls;

public:
    Poller() = default;

    void Clear();
    void Watch(int fd, short events);
    int Wait(int timeout);
    short Revents(std::size_t index) const;
    std::size_t Size() const {return _polls.size();};
};

class FileSender {
public:
    static void Stream(const TcpSocket& dataSocket, const std::string& filePath);
};

class FileReceiver {
    TcpSocket _listen;
    std::uint16_t _port = 0;

public:
    FileReceiver();

    std::uint16_t Port() const {return _port;};
    void Receive(const std::string& outPath);
};

class NetLog {
public:
    NetLog() = default;
    ~NetLog() = default;

    void LogReceive(const std::string& msg, std::size_t uid=0);
    void LogSend(const std::string& msg, std::size_t uid=0);
};

}

#endif
