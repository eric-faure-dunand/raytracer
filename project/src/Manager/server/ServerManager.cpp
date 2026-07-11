/*
** EPITECH PROJECT, 2025
** raytracer²
** File description:
** ServerManager.cpp
*/

#include "ServerManager.hpp"

#include <arpa/inet.h>

namespace raytracer {

void ServerManager::Reply(Client& cl, const std::string& message) {
    _logger.LogSend(message, cl.Uid);
    cl.MsgQueu.push_back(message);
    cl.doWrite = true;
}

void ServerManager::HandleFGet(Client& cl, std::istringstream& iss) {
    std::string path, ip;
    std::uint16_t port = 0;
    if (!(iss >> path >> ip >> port)) {
        Reply(cl, "400 BAD REQUEST");
        return;
    }

    std::string realpath;
    std::string okFilename;
    if (path == "ConfigueFile") {
        realpath = _configFile;
        const auto slash = _configFile.find_last_of('/');
        okFilename = (slash == std::string::npos) ? _configFile : _configFile.substr(slash + 1);
    } else
        realpath = path;
    {
        std::ifstream test(realpath, std::ios::binary);
        if (!test.is_open()) {
            Reply(cl, "404 NOT FOUND");
            return;
        }
    }

    TcpSocket data;
    try {
        data.Connect(ip, port);
    } catch (const IError&) {
        Reply(cl, "503 NO CONNECT");
        return;
    }
    try {
        FileSender::Stream(data, realpath);
    } catch (const IError&) {
        Reply(cl, "500 ERROR");
        return;
    }
    data.Close();
    if (okFilename.empty())
        Reply(cl, "200 OK");
    else
        Reply(cl, "200 OK " + okFilename);
}

void ServerManager::HandleReady(Client& cl, std::istringstream& iss) {
    std::size_t threads = 0;
    if (!(iss >> threads) || threads == 0) {
        Reply(cl, "400 BAD REQUEST");
        return;
    }
    cl.ThreadAviable = threads;
    cl.CanCompute = true;
}

void ServerManager::HandleFPut(Client& cl, std::istringstream& iss) {
    std::string ip;
    std::uint16_t port = 0;
    if (!(iss >> ip >> port)) {
        Reply(cl, "400 BAD REQUEST");
        return;
    }
    if (!_map || _map->empty() || (*_map)[0].empty()) {
        Reply(cl, "500 NO MAP");
        return;
    }

    TcpSocket data;
    try {
        data.Connect(ip, port);
    } catch (const IError&) {
        Reply(cl, "503 NO CONNECT");
        return;
    }

    try {
        std::uint32_t y_net = 0;
        data.ReceiveExact(reinterpret_cast<char*>(&y_net), 4);
        const std::uint32_t y_start = ntohl(y_net);

        std::vector<std::uint8_t> buf;
        char chunk[4096];
        while (true) {
            const std::size_t n = data.ReceiveBlock(chunk, sizeof(chunk));
            if (n == 0)
                break;
            buf.insert(buf.end(),
                       reinterpret_cast<std::uint8_t*>(chunk),
                       reinterpret_cast<std::uint8_t*>(chunk) + n);
        }

        const std::size_t width = (*_map)[0].size();
        const std::size_t height = _map->size();
        const std::size_t pixelCount = buf.size() / 3;
        for (std::size_t i = 0; i < pixelCount; ++i) {
            const std::size_t y = y_start + i / width;
            const std::size_t x = i % width;
            if (y >= height || x >= width)
                break;
            (*_map)[y][x].SetColor({buf[3 * i], buf[3 * i + 1], buf[3 * i + 2]});
            (*_map)[y][x].SetState(COMPUTED);
        }
    } catch (const IError&) {
        Reply(cl, "500 ERROR");
        return;
    }
    if (_outstanding > 0)
        --_outstanding;
    Reply(cl, "200 OK");
}

void ServerManager::HandleCommand(Client& cl, const std::string& line) {
    std::istringstream iss(line);
    std::string cmd;
    std::size_t code = 0;
    iss >> cmd;
    _logger.LogReceive(line, cl.Uid);
    try {
        code = std::stoul(cmd);
    } catch (...) {}
    if (code != 0)
        return;
    if (cmd == "FGET") {
        HandleFGet(cl, iss);
        return;
    }
    if (cmd == "READY") {
        HandleReady(cl, iss);
        return;
    }
    if (cmd == "FPUT") {
        HandleFPut(cl, iss);
        return;
    }
    Reply(cl, "400 BAD REQUEST");
}

void ServerManager::DoPoll() {
    _poller.Clear();
    _poller.Watch(_listen.Fd(), POLLIN);
    for (const auto& c : _clients)
        _poller.Watch(c.socket.Fd(), static_cast<short>(c.doWrite ? (POLLIN | POLLOUT) : POLLIN));

    if (_poller.Wait(0) < 0)
        throw Error("ServerManager: poll failed.");

    std::vector<std::size_t> toRemove;
    for (std::size_t i = 0; i < _clients.size(); ++i) {
        Client& c = _clients[i];
        const short rev = _poller.Revents(i + 1);
        c.doRead = false;
        if (rev & (POLLERR | POLLHUP | POLLNVAL)) {
            toRemove.push_back(i);
            continue;
        }
        if (rev & POLLIN) {
            try {
                const std::string chunk = c.socket.Receive();
                if (chunk.empty()) {
                    toRemove.push_back(i);
                    continue;
                }
                c.cmd.append(chunk);
                c.doRead = true;
            } catch (const IError&) {
                toRemove.push_back(i);
                continue;
            }
        }
    }
    for (auto it = toRemove.rbegin(); it != toRemove.rend(); ++it) {
        _logger.LogReceive("Disconnected.", _clients[*it].Uid);
        _clients.erase(_clients.begin() + static_cast<std::ptrdiff_t>(*it));
    }

    if (_poller.Revents(0) & POLLIN) {
        try {
            Client cl(_listen.Accept(), GetUid());
            _logger.LogReceive("New conexion." , cl.Uid);
            _clients.emplace_back(std::move(cl));
        } catch (const IError& e) {
            std::cerr << "ServerManager: " << e.what();
        }
    }
}

std::size_t ServerManager::GetUid() {
    std::vector<std::size_t> uidTab;

    for (std::size_t i = 1; i <= _clients.size() + 1; i++)
        uidTab.push_back(i);
    for (std::size_t i = 0; i < _clients.size(); i++)
        uidTab[_clients[i].Uid - 1] = 0;
    for (std::size_t i = 0; i < uidTab.size(); i++)
        if (uidTab[i] != 0)
            return uidTab[i];
    return _clients.size() + 1;
}

void ServerManager::AssignWork() {
    if (!_map || _map->empty())
        return;
    const std::size_t height = _map->size();
    for (auto& cl : _clients) {
        if (!cl.CanCompute)
            continue;
        if (_nextRow >= height)
            continue;
        const std::size_t start = _nextRow;
        const std::size_t end = std::min(_nextRow + cl.ThreadAviable, height);
        std::ostringstream oss;
        oss << "COMPUTE " << start << " " << end;
        Reply(cl, oss.str());
        cl.CanCompute = false;
        _nextRow = end;
        ++_outstanding;
    }
}

void ServerManager::SelfCompute(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map) {
    if (_selfComputing) {
        if (!_multiThread.isEnd())
            return;
        for (std::size_t y = _selfStart; y < _selfEnd && y < map.size(); ++y)
            for (std::size_t x = 0; x < map[y].size(); ++x)
                map[y][x].SetState(COMPUTED);
        _selfComputing = false;
    }
    if (_nextRow >= map.size())
        return;
    const std::size_t nthreads = static_cast<std::size_t>(_multiThread.GetTreadNumber());
    _selfStart = _nextRow;
    _selfEnd = std::min(_nextRow + nthreads, map.size());
    _nextRow = _selfEnd;
    _selfComputed = 0;
    _multiThread.Compute(objects, lights, camera, map, _selfStart, _selfEnd, _selfComputed);
    _selfComputing = true;
}

void ServerManager::Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map) {
    _map = &map;
    DoPoll();

    for (auto& cl : _clients) {
        if (cl.doRead) {
            std::string::size_type pos;
            while ((pos = cl.cmd.find('\n')) != std::string::npos) {
                std::string line = cl.cmd.substr(0, pos);
                cl.cmd.erase(0, pos + 1);
                if (!line.empty() && line.back() == '\r')
                    line.pop_back();
                if (!line.empty())
                    HandleCommand(cl, line);
            }
            cl.doRead = false;
        }
        if (cl.doWrite && !cl.MsgQueu.empty()) {
            std::string message;
            for (const auto& entry : cl.MsgQueu) {
                std::string line = entry;
                while (!line.empty() && (line.back() == '\n' || line.back() == '\r'))
                    line.pop_back();
                message += line;
                message += "\r\n";
            }
            if (!message.empty()) {
                try {
                    cl.socket.SendAll(message.data(), message.size());
                } catch (const IError&) {
                    std::cerr << "ServerManager: cannot write to client.\n";
                }
            }
            cl.MsgQueu.clear();
            cl.doWrite = false;
        }
    }

    AssignWork();
    SelfCompute(objects, lights, camera, map);

    if (_nextRow >= map.size() && !_selfComputing && _outstanding == 0) {
        _logger.LogSend("All work done, server finishing.");
        _state = FINISH;
    }
}

}
