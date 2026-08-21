
#ifndef SHADER_HPP
    #define SHADER_HPP
    #include <string>
    #include <glad/gl.h>
    #include <fstream>
    #include <sstream>

    #include "Error.hpp"

namespace raytracer {

class Shader {
public:
    static GLuint compileCompute(const std::string &path);

private:
    static std::string readFile(const std::string &path);
    static GLuint compile(GLenum type, const std::string &source, const std::string &path);
    static GLuint link(GLuint shader);
};

}

#endif
