/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** Net.cpp
*/

#include "Net.hpp"

#include <cerrno>
#include <cstring>

namespace raytracer {

static std::string Errno() {
    return std::string(" (") + std::strerror(errno) + ")";
}

TcpSocket::~TcpSocket() {
    Close();
}

TcpSocket::TcpSocket(TcpSocket&& other) noexcept : _fd(other._fd) {
    other._fd = -1;
}

TcpSocket& TcpSocket::operator=(TcpSocket&& other) noexcept {
    if (this != &other) {
        Close();
        _fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

void TcpSocket::Close() {
    if (_fd != -1) {
        ::close(_fd);
        _fd = -1;
    }
}

void TcpSocket::Connect(const std::string& ip, std::uint16_t port) {
    Close();
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw Error("TcpSocket: cannot create socket" + Errno() + ".");

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1) {
        Close();
        throw Error("TcpSocket: invalid ip address '" + ip + "'.");
    }
    if (::connect(_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        const std::string e = Errno();
        Close();
        throw Error("TcpSocket: cannot connect to " + ip + ":" + std::to_string(port) + e + ".");
    }
}

void TcpSocket::Listen(std::uint16_t port, int backlog) {
    Close();
    _fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw Error("TcpSocket: cannot create socket" + Errno() + ".");

    const int opt = 1;
    if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        const std::string e = Errno();
        Close();
        throw Error("TcpSocket: cannot configure socket reuse option" + e + ".");
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (::bind(_fd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
        const std::string e = Errno();
        Close();
        throw Error("TcpSocket: cannot bind port " + std::to_string(port) + e + ".");
    }
    if (::listen(_fd, backlog) < 0) {
        const std::string e = Errno();
        Close();
        throw Error("TcpSocket: cannot listen on port " + std::to_string(port) + e + ".");
    }
}

TcpSocket TcpSocket::Accept() const {
    if (_fd == -1)
        throw Error("TcpSocket: cannot accept on a closed socket.");

    sockaddr_in clientAddr{};
    socklen_t clientLen = sizeof(clientAddr);
    const int clientFd = ::accept(_fd, reinterpret_cast<sockaddr *>(&clientAddr), &clientLen);
    if (clientFd < 0)
        throw Error("TcpSocket: cannot accept a new client" + Errno() + ".");
    return TcpSocket(clientFd);
}

std::size_t TcpSocket::Send(const std::string& message) const {
    if (_fd == -1)
        throw Error("TcpSocket: socket is not connected.");
    const ssize_t size = ::send(_fd, message.c_str(), message.size(), 0);
    if (size < 0)
        throw Error("TcpSocket: cannot send data" + Errno() + ".");
    return static_cast<std::size_t>(size);
}

void TcpSocket::SendAll(const char *data, std::size_t len) const {
    if (_fd == -1)
        throw Error("TcpSocket: socket is not connected.");
    std::size_t sent = 0;
    while (sent < len) {
        const ssize_t n = ::send(_fd, data + sent, len - sent, 0);
        if (n < 0)
            throw Error("TcpSocket: cannot send data" + Errno() + ".");
        if (n == 0)
            throw Error("TcpSocket: connection closed during send.");
        sent += static_cast<std::size_t>(n);
    }
}

std::string TcpSocket::Receive() const {
    if (_fd == -1)
        throw Error("TcpSocket: socket is not connected.");

    char buffer[4096];
    const ssize_t size = ::recv(_fd, buffer, sizeof(buffer), 0);
    if (size < 0)
        throw Error("TcpSocket: cannot receive data" + Errno() + ".");
    if (size == 0)
        return std::string();
    return std::string(buffer, static_cast<std::size_t>(size));
}

std::size_t TcpSocket::ReceiveBlock(char *buf, std::size_t cap) const {
    if (_fd == -1)
        throw Error("TcpSocket: socket is not connected.");
    const ssize_t n = ::recv(_fd, buf, cap, 0);
    if (n < 0)
        throw Error("TcpSocket: cannot receive data" + Errno() + ".");
    return static_cast<std::size_t>(n);
}

void TcpSocket::ReceiveExact(char *buf, std::size_t n) const {
    if (_fd == -1)
        throw Error("TcpSocket: socket is not connected.");
    std::size_t got = 0;
    while (got < n) {
        const ssize_t r = ::recv(_fd, buf + got, n - got, 0);
        if (r < 0)
            throw Error("TcpSocket: cannot receive data" + Errno() + ".");
        if (r == 0)
            throw Error("TcpSocket: connection closed before " + std::to_string(n) + " bytes received.");
        got += static_cast<std::size_t>(r);
    }
}

std::uint16_t TcpSocket::LocalPort() const {
    if (_fd == -1)
        throw Error("TcpSocket: cannot query local port on closed socket.");
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);
    if (::getsockname(_fd, reinterpret_cast<sockaddr *>(&addr), &len) < 0)
        throw Error("TcpSocket: getsockname failed" + Errno() + ".");
    return ntohs(addr.sin_port);
}

std::string TcpSocket::LocalIp() const {
    if (_fd == -1)
        throw Error("TcpSocket: cannot query local ip on closed socket.");
    sockaddr_in addr{};
    socklen_t len = sizeof(addr);
    if (::getsockname(_fd, reinterpret_cast<sockaddr *>(&addr), &len) < 0)
        throw Error("TcpSocket: getsockname failed" + Errno() + ".");
    char buf[INET_ADDRSTRLEN] = {0};
    if (::inet_ntop(AF_INET, &addr.sin_addr, buf, sizeof(buf)) == nullptr)
        throw Error("TcpSocket: inet_ntop failed" + Errno() + ".");
    return std::string(buf);
}

void Poller::Clear() {
    _polls.clear();
}

void Poller::Watch(int fd, short events) {
    _polls.push_back(pollfd{fd, events, 0});
}

int Poller::Wait(int timeout) {
    if (_polls.empty())
        return 0;
    return ::poll(_polls.data(), static_cast<nfds_t>(_polls.size()), timeout);
}

short Poller::Revents(std::size_t index) const {
    return _polls.at(index).revents;
}

void FileSender::Stream(const TcpSocket& dataSocket, const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
        throw Error("FileSender: cannot open file '" + filePath + "'.");

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        const std::streamsize got = file.gcount();
        if (got > 0)
            dataSocket.SendAll(buffer, static_cast<std::size_t>(got));
    }
}

FileReceiver::FileReceiver() {
    _listen.Listen(0);
    _port = _listen.LocalPort();
}

void FileReceiver::Receive(const std::string& outPath) {
    TcpSocket data = _listen.Accept();

    std::ofstream out(outPath, std::ios::binary | std::ios::trunc);
    if (!out.is_open())
        throw Error("FileReceiver: cannot open '" + outPath + "' for writing.");

    char buffer[4096];
    while (true) {
        const std::size_t n = data.ReceiveBlock(buffer, sizeof(buffer));
        if (n == 0)
            break;
        out.write(buffer, static_cast<std::streamsize>(n));
    }
}

void NetLog::LogReceive(const std::string& msg, std::size_t uid) {
    std::istringstream iss(msg);
    int code = 0;
    auto color = Color::WHITE;

    if ((iss >> code)) {
        if (code >= 400)
            color = Color::RED;
        else
            color = Color::GREEN;
    }
    std::cout << Color::BG_MAGENTA << "Receive" << Color::RESET;
    if (uid == 0)
        std::cout << " : ";
    else
        std::cout << " from user " << uid << " : ";
    std::cout << color << msg << Color::RESET << std::endl;
}

void NetLog::LogSend(const std::string& msg, std::size_t uid) {
    std::istringstream iss(msg);
    int code = 0;
    auto color = Color::WHITE;

    if ((iss >> code)) {
        if (code >= 400)
            color = Color::RED;
        else
            color = Color::GREEN;
    }
    std::cout << Color::BG_CYAN << "Send" << Color::RESET;
    if (uid == 0)
        std::cout << " at server : ";
    else
        std::cout << " at user " << uid << " : ";
    std::cout << color << msg << Color::RESET << std::endl;
}

}
