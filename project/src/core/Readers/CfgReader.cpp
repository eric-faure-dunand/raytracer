
#include "CfgReader.hpp"

namespace raytracer {

void CfgReader::SetUp() {
    try {
        _cfg.readFile(_sceneFile.c_str());
    } catch (const libconfig::FileIOException&) {
        throw Error("Core: cannot read scene file '\"" + _sceneFile + "\"'.");
    } catch (const libconfig::ParseException& ex) {
        throw Error("Core: parse error in scene file '\"" + _sceneFile + "\"' at line "
            + std::to_string(ex.getLine()) + ": " + ex.getError() + "");
    }
}

const libconfig::Setting& CfgReader::CameraExist() {
    const libconfig::Setting& root = _cfg.getRoot();
    if (!root.exists("camera"))
        throw Error("Core: missing 'camera' section in scene file.");
    return root;
}

std::pair<std::size_t, std::size_t> CfgReader::GetCameraResolution() {
    const libconfig::Setting& root = CameraExist();
    const libconfig::Setting& camera = root["camera"];
    if (!camera.exists("resolution"))
        throw Error("Core: missing 'camera.resolution' section in scene file.");
    const libconfig::Setting& resolution = camera["resolution"];

    int width = 0, height = 0;
    if (!resolution.lookupValue("width", width) || !resolution.lookupValue("height", height))
        throw Error("Core: resolution width/height are missing in scene file.");
    if (width <= 0 || height <= 0)
        throw Error("Core: resolution width/height must be positive values.");

    return {static_cast<std::size_t>(width), static_cast<std::size_t>(height)};
}

Vector3 CfgReader::GetCameraPosition() {
    const libconfig::Setting& root = CameraExist();
    const libconfig::Setting& camera = root["camera"];
    if (!camera.exists("position"))
        throw Error("Core: missing 'camera.position' section in scene file.");
    const libconfig::Setting& position = camera["position"];

    float x = 0, y = 0, z = 0;
    if (!position.lookupValue("x", x) || !position.lookupValue("y", y) || !position.lookupValue("z", z))
        throw Error("CfgReader: Camera position are missing or with bad value, in scene file.");
    return {x, y, z};
}

Vector3 CfgReader::GetCameraRotation() {
    const libconfig::Setting& root = CameraExist();
    const libconfig::Setting& camera = root["camera"];
    if (!camera.exists("rotation"))
        throw Error("Core: missing 'camera.rotation' section in scene file.");
    const libconfig::Setting& position = camera["rotation"];

    float x = 0, y = 0, z = 0;
    if (!position.lookupValue("x", x) || !position.lookupValue("y", y) || !position.lookupValue("z", z))
        throw Error("CfgReader: Camera rotation are missing or with bad value, in scene file.");
    return {x, y, z};
}

double CfgReader::GetCameraFieldOfView() {
    const libconfig::Setting& root = CameraExist();
    const libconfig::Setting& camera = root["camera"];
    double fov = 0;
    if (!camera.lookupValue("fieldOfView", fov))
        throw Error("Core: missing 'camera.fieldOfView' section in scene file.");
    return fov;
}
/*
std::vector<std::unique_ptr<ILight>> CfgReader::GetLights() {
    std::vector<std::unique_ptr<ILight>> lights;

    const std::vector<std::string> lightTypes = {"ambient", "directional", "point"};

    for (const auto& type : lightTypes) {
        auto sub = loadSubList<ILight>("lights", type);
        for (auto& l : sub)
            lights.push_back(std::move(l));
    }

    const libconfig::Setting& root = _cfg.getRoot();
    if (root.exists("lights")) {
        const libconfig::Setting& sec = root["lights"];
        double ambientVal = 0.0;
        if (sec.exists("ambient") && sec.lookupValue("ambient", ambientVal)) {
            bool hasAmbient = false;
            if (!hasAmbient) {
                std::map<std::string, std::string> params;
                params["intensity"] = std::to_string(ambientVal);
                try {
                    lights.push_back(ldloader.load<ILight>("ambient", params));
                } catch (const IError& e) {}
            }
        }
    }
    return lights;
}*/

void CfgReader::TakeOneValue(const libconfig::Setting& cfg, std::map<std::string, std::string>& params, const std::string& prefix) {
    switch (cfg.getType()) {
        case libconfig::Setting::TypeString: {
            const char* strVal = cfg.c_str();
            std::string strValue = strVal ? strVal : "";
            params[prefix] = strValue;
            break;
        }
        case libconfig::Setting::TypeInt:
            params[prefix] = std::to_string(static_cast<int>(cfg));
            break;
        case libconfig::Setting::TypeFloat:
            params[prefix] = std::to_string(static_cast<double>(cfg));
            break;
        case libconfig::Setting::TypeBoolean:
            params[prefix] = static_cast<bool>(cfg) ? "true" : "false";
            break;
        default:
            break;
    }
}
/*
std::map<std::string, std::string> CfgReader::settingToParams(const libconfig::Setting& cfg, const std::string& prefix) {
    std::map<std::string, std::string> params;
    if (!cfg.isGroup() && !cfg.isArray()) {
        TakeOneValue(cfg, params, prefix);
        return params;
    }
    for (int i = 0; i < cfg.getLength(); ++i) {
        const libconfig::Setting& field = cfg[i];
        const char* rawName = field.getName();
        std::string key = rawName ? (prefix.empty() ? std::string(rawName) : prefix + "." + std::string(rawName)) : prefix;
        if (field.isGroup() || field.isArray()) {
            for (auto& kv : settingToParams(field, key))
                params.emplace(kv.first, kv.second);
            continue;
        }
        TakeOneValue(field, params, key);
    }
    return params;
}*/

}