#include "Shader.hpp"

namespace raytracer {

std::string Shader::readFile(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open())
        throw Error("Shader: could not open file: " + path);

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader::compile(GLenum type, const std::string &source, const std::string &path) {
    GLuint shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        glDeleteShader(shader);
        throw Error("Shader: compilation failed (" + path + "):\n" + std::string(log));
    }
    return shader;
}

GLuint Shader::link(GLuint shader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, shader);
    glLinkProgram(program);
    glDeleteShader(shader);

    GLint ok = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        glDeleteProgram(program);
        throw Error("Shader: program link failed:\n" + std::string(log));
    }
    return program;
}

GLuint Shader::compileCompute(const std::string &path) {
    std::string source = readFile(path);
    GLuint shader = compile(GL_COMPUTE_SHADER, source, path);
    return link(shader);
}

}
