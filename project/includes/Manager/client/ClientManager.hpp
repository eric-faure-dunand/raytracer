/*
** EPITECH PROJECT, 2025
** raytracer
** File description:
** ClientManager.hpp
*/

#ifndef CLIENTMANAGER_HPP
    #define CLIENTMANAGER_HPP
    #include <cstdint>
    #include <memory>
    #include <queue>
    #include <sstream>
    #include <string>
    #include <vector>
    #include <iostream>

    #include "AManager.hpp"
    #include "Net.hpp"
    #include "Error.hpp"
    #include "Warning.hpp"
    #include "printer.hpp"

namespace raytracer {

class ClientManager : public AManager {
public:
    typedef enum {
        NOTCONNECTED = 0,
        ASKCONFIGUE = 1,
        WAITCONFIGUE = 2,
        TRYINIT = 3,
        ASKPLUGINS = 4,
        WAITPLUGINS = 5,
        INITCORE = 6,
        COMPUTE = 7,
    } ClientStage;

private:
    ClientStage _stage = NOTCONNECTED;
    TcpSocket _socket;
    Poller _poller;
    std::queue<std::string> _outgoing;
    std::queue<std::string> _incoming;
    std::string _recvBuf;
    std::string _ip;
    std::uint16_t _port;
    bool _connected = false;

    std::unique_ptr<FileReceiver> _receiver;
    std::string _configFilename;
    std::vector<std::string> _pluginsToFetch;
    std::size_t _pluginIndex = 0;

    std::size_t start = 0;
    std::size_t end = 0;
    std::size_t Computed = 0;

    void ConnectIfNeeded();
    void DoPoll();
    void SendMsg(const std::string& str);
    void Advance();
    void ProcessActiveStates(Core& core);
    void ProcessWaitingStates(Core& core);
    void StreamResults(std::vector<std::vector<Tile>>& map);

public:
    ClientManager(const std::string& ip, std::uint16_t port);
    ~ClientManager() = default;

    void QueueMessage(const std::string& message);
    bool HasMessage() const;
    std::string PopMessage();

    void Update(const std::vector<std::unique_ptr<IObject>>& objects, const std::vector<std::unique_ptr<ILight>>& lights, const render::Camera& camera, std::vector<std::vector<Tile>>& map);
    void InitCore(Core& core) override;
};

}

#endif
