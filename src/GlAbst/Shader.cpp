#include "Shader.h"

std::string ReadFile(const std::string& filenPath){
    std::ifstream file;
    file.open(filenPath.c_str());
    char c;
    std::string sfile;
    while(file.get(c)) sfile += c;
    file.close();
    return sfile;
}
void CheckShader(GLuint shader, const std::string& name)
{
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        std::cout << name << ": " << errorLog.data() << '\n';
        // Provide the infolog in whatever manor you deem best.
        // Exit with failure.
        glDeleteShader(shader); // Don't leak the shader.
    }
}
Shader::Shader(const std::string &vs, const std::string &fs)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string vertexSource = ReadFile(vs.c_str());
    std::string fragSource = ReadFile(fs.c_str());
    const char* vertexSourceS = vertexSource.c_str();
    const char* fragSourceS = fragSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSourceS, NULL);
    glShaderSource(fragShader, 1, &fragSourceS, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragShader);
    CheckShader(vertexShader, vs);
    CheckShader(fragShader, fs);
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragShader);
    glLinkProgram(ID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

Shader::Shader(const std::string &vs, const std::string &gs, const std::string &fs)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    std::string vertexSource = ReadFile(vs.c_str());
    std::string fragSource = ReadFile(fs.c_str());
    std::string geomSource = ReadFile(gs.c_str());
    const char* vertexSourceS = vertexSource.c_str();
    const char* fragSourceS = fragSource.c_str();
    const char* geomSourceS = geomSource.c_str();
    glShaderSource(vertexShader, 1, &vertexSourceS, NULL);
    glShaderSource(fragShader, 1, &fragSourceS, NULL);
    glShaderSource(geometryShader, 1, &geomSourceS, NULL);
    glCompileShader(vertexShader);
    glCompileShader(fragShader);
    glCompileShader(geometryShader);
    CheckShader(vertexShader, vs);
    CheckShader(fragShader, fs);
    CheckShader(geometryShader, gs);
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragShader);
    glAttachShader(ID, geometryShader);
    glLinkProgram(ID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
    glDeleteShader(geometryShader);
}

void Shader::Bind()
{
    glUseProgram(ID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}
int GetUniformLocation(GLuint id, const std::string& name){
    int result = glGetUniformLocation(id, name.c_str());
    if(result == -1){
        // std::cout << "Uniform with name: " << name << " can't be found!" << '\n';
    }
    return result;
}
void Shader::SetUniform(const std::string &name, int data)
{
    glUniform1i(GetUniformLocation(ID, name.c_str()),data);
}

void Shader::SetUniform(const std::string &name, float data)
{
    glUniform1f(GetUniformLocation(ID, name.c_str()),data);
}

void Shader::SetUniform(const std::string &name, const glm::vec3 data)
{
    glUniform3f(GetUniformLocation(ID, name.c_str()),data.x, data.y, data.z);
}

void Shader::SetUniform(const std::string &name, const glm::mat4 &mat)
{
    glUniformMatrix4fv(GetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}
