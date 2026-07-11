/*
** EPITECH PROJECT, 2025
** G-OOP-400-NCE-4-1-raytracer-8
** File description:
** libld.hpp
*/

#ifndef LIBLD_HPP
    #define LIBLD_HPP
    #define LD_HPP
    #include <dlfcn.h>
    #include <memory>
    #include <iostream>
    #include <map>
    #include <vector>
    #include <string>

    #include "IObject.hpp"
    #include "ILight.hpp"
    #include "SoType.hpp"
    #include "Warning.hpp"
    #include "Error.hpp"
    #include "FileNotFound.hpp"

namespace raytracer {

class ld {
    std::vector<void*> _handles;

public:
    ld() {};
    ~ld() {
        for (auto h : _handles)
            dlclose(h);
    };

    template<typename T>
    std::unique_ptr <T> load(std::string path, std::map<std::string, std::string> param) {
        std::string realpath("plugins/raytracer_" + path + ".so");
        void *handle = dlopen(realpath.c_str(), RTLD_LAZY);
        if (!handle)
            throw Warning("No file match in plugin with name : " + realpath);
        dlerror();
        auto getSoType = reinterpret_cast<SoTypeEnum (*) ()> (dlsym(handle, "getSOType"));
        const char* err = dlerror();
        if (err || !getSoType) {
            dlclose(handle);
            throw Error("libld : getSOType not found in " + realpath + ".\n");
        }
        SoTypeEnum type = getSoType();
        T* (*getObject)(std::map<std::string, std::string>) = nullptr;
        switch (type) {
            case OBJECT:
                getObject = reinterpret_cast<T* (*)(std::map<std::string, std::string>)> (dlsym(handle, "getObject")); break;
            case LIGHT:
                getObject = reinterpret_cast<T* (*)(std::map<std::string, std::string>)> (dlsym(handle, "getLight")); break;
            case Texture:
                getObject = reinterpret_cast<T* (*)(std::map<std::string, std::string>)> (dlsym(handle, "getTexture")); break;
            default:
                dlclose(handle);
                throw Error("libld : unknown SoTypeEnum in " + realpath + ".\n");
        }
        err = dlerror();
        if (err || !getObject) {
            dlclose(handle);
            throw Error("libld : factory function not found in " + realpath + ".\n");
        }
        _handles.push_back(handle);
        return std::unique_ptr<T> (getObject(param));
    }
};

}

#endif