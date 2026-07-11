/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** CfgReader.hpp
*/

#ifndef CFGREADER_HPP
    #define CFGREADER_HPP
    #include <libconfig.h++>
    #include <algorithm>

    #include "AReader.hpp"
    #include "Warning.hpp"
    #include "printer.hpp"

namespace raytracer {

class CfgReader : public AReader {
    libconfig::Config _cfg;

    const libconfig::Setting& CameraExist();
    std::map<std::string, std::string> settingToParams(const libconfig::Setting& cfg, const std::string& prefix = "");
    void TakeOneValue(const libconfig::Setting& cfg, std::map<std::string, std::string>& params, const std::string& prefix);

    template<typename T>
    std::vector<std::unique_ptr<T>> loadSubList(const std::string& section, const std::string& name_list) {
        std::vector<std::unique_ptr<T>> list;
        const libconfig::Setting& root = _cfg.getRoot();
        if (!root.exists(section))
            return list;
        const libconfig::Setting& sec = root[section.c_str()];
        if (!sec.exists(name_list))
            return list;
        const libconfig::Setting& objects = sec[name_list.c_str()];
        for (int i = 0; i < objects.getLength(); ++i) {
            const libconfig::Setting& group = objects[i];
            std::map<std::string, std::string> param = settingToParams(group);
            try {
                list.push_back(ldloader.load<T>(name_list, param));
            } catch (const IError& e) {
                if (e.code() == 0 && std::find(NoOpenFile.begin(), NoOpenFile.end(), name_list) == NoOpenFile.end())
                    NoOpenFile.push_back(name_list);
            }
        }
        return list;
    }

    template<typename T>
    std::vector<std::unique_ptr<T>> loadLists(std::string name_list) {
        std::vector<std::unique_ptr<T>> list;
        const libconfig::Setting& root = _cfg.getRoot();
        if (!root.exists(name_list))
            throw Warning("Reader: No Objets found.");
        const libconfig::Setting& objects = _cfg.lookup(name_list);
        for (int i = 0; i < objects.getLength(); ++i) {
            const libconfig::Setting& group = objects[i];
            if (!group.getName()) {
                std::cerr << "membre without name" << std::endl;
                continue;
            }
            std::string GroupeName = group.getName();
            std::map<std::string, std::string> param;
            if (group.getLength() > 0 && group[0].isGroup()) {
                for (int j = 0; j < group.getLength(); ++j) {
                    param = settingToParams(group[j]);
                    try {
                        list.push_back(ldloader.load<T>(GroupeName, param));
                    } catch (const IError& e) {
                        // If plugin file not found (common when config uses plural names),
                        // try a singular fallback (e.g., "planes" -> "plane").
                        if (e.code() == 0 && !GroupeName.empty() && GroupeName.back() == 's') {
                            std::string alt = GroupeName.substr(0, GroupeName.size() - 1);
                            try {
                                list.push_back(ldloader.load<T>(alt, param));
                                continue;
                            } catch (const IError& e2) {
                                (void)e2;
                            }
                        }
                        if (e.code() == 0 && std::find(NoOpenFile.begin(), NoOpenFile.end(), GroupeName) == NoOpenFile.end()) {
                            std::cout << e;
                            NoOpenFile.push_back("plugins/raytracer_" + GroupeName + ".so");
                        } else if (e.code() == 2 && std::find(NoOpenFile.begin(), NoOpenFile.end(), GroupeName) == NoOpenFile.end()) {
                            std::cout << e;
                            NoOpenFile.push_back(e.what());
                        } else
                            std::cout << e;
                    }
                }
            } else {
                param = settingToParams(group);
                try {
                    list.push_back(ldloader.load<T>(GroupeName, param));
                } catch (const IError& e) {
                    if (e.code() == 0 && !GroupeName.empty() && GroupeName.back() == 's') {
                        std::string alt = GroupeName.substr(0, GroupeName.size() - 1);
                        try {
                            list.push_back(ldloader.load<T>(alt, param));
                            continue;
                        } catch (const IError& e2) {
                            (void)e2;
                        }
                    }
                    if (e.code() == 0 && std::find(NoOpenFile.begin(), NoOpenFile.end(), GroupeName) == NoOpenFile.end()) {
                        std::cout << e;
                        NoOpenFile.push_back("plugins/raytracer_" + GroupeName + ".so");
                    } else if (e.code() == 2 && std::find(NoOpenFile.begin(), NoOpenFile.end(), GroupeName) == NoOpenFile.end()) {
                        std::cout << e;
                        NoOpenFile.push_back(e.what());
                    } else
                        std::cout << e;
                }
            }
        }
        return list;
    }

public:
    CfgReader(std::string NameFile): AReader(NameFile) {SetUp();};
    ~CfgReader() = default;

    void SetUp();
    std::pair<std::size_t, std::size_t> GetCameraResolution();
    std::array<int, 3> GetCameraPosition();
    std::array<int, 3> GetCameraRotation();
    double GetCameraFieldOfView();

    std::vector<std::unique_ptr<IObject>> GetObjects() { return loadLists<IObject>("primitives"); };
    std::vector<std::unique_ptr<ILight>> GetLights();
};

}

#endif