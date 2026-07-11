/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ServerManager.hpp
*/

#ifndef SERVERMANAGER_HPP
    #define SERVERMANAGER_HPP
    #include <cstdint>
    #include <sstream>
    #include <string>
    #include <vector>
    #include <iostream>
    #include <fstream>
    #include <utility>

    #include "AManager.hpp"
    #include "Error.hpp"
    #include "Warning.hpp"

namespace raytracer {

struct Client {
    TcpSocket socket;
    std::string cmd = "";
    bool doWrite = false;
    bool doRead = false;
    bool CanCompute = false;
    std::size_t ThreadAviable = 0;
    std::size_t Uid;
    std::vector<std::string> MsgQueu;

    explicit Client(TcpSocket sock, std::size_t uid): socket(std::move(sock)), Uid(uid) {};
};

class ServerManager : public AManager {
    std::string _configFile;

    TcpSocket _listen;
    Poller _poller;
    std::vector<Client> _clients;

    std::vector<std::vector<Tile>>* _map = nullptr;
    std::size_t _nextRow = 0;

    bool _selfComputing = false;
    std::size_t _selfStart = 0;
    std::size_t _selfEnd = 0;
    std::size_t _selfComputed = 0;
    std::size_t _outstanding = 0;

    void HandleCommand(Client& cl, const std::string& line);
    void HandleFGet(Client& cl, std::istringstream& iss);
    void HandleReady(Client& cl, std::istringstream& iss);
    void HandleFPut(Client& cl, std::istringstream& iss);
    void Reply(Client& cl, const std::string& message);
    void DoPoll();
    void AssignWork();
    void SelfCompute(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map);

    std::size_t GetUid(void);
public:
    ServerManager(int port, std::string configFile): _configFile(configFile) {
        _listen.Listen(static_cast<std::uint16_t>(port));
    };
    ~ServerManager() = default;

    void Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map);
};

}

#endif
