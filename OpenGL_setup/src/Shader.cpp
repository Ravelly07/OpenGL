/*Shader.cpp*/
#include "Shader.h"

#include <iostream>
#include <fstream>  
#include <string>
#include <sstream>

#include "Renderer.h"
//se crea el objeto de tipo shader
Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}
//Se elimina el shader (normalmente s eusa tras la compilaci�n)
Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}
/*Ajuste usado para emplear basic.shader*/
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);  //open the file

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    // read line by line and find the beginning of each shader
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;       //set mode to vertex
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;    // set mode to fragment
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);//Crea el shader de acuerdo al tipo, (vertex, fragment)
    const char* src = source.c_str();//apunta al inicio del array [0]
    glShaderSource(id, 1, &src, nullptr);//Empieza a leer el programa 
    glCompileShader(id);//Compila el shader

    /*Error handling -- En caso de presentarse algun error, el programa recibe
    un identificador, lo convierte a cadena y lo imprime en la consola,
    posteriormente cierra el programa. */
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // to identify the error
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}
//creamos el shader del programa, tras compilar los objetos tipo shader y concatenarlos en un Shader 
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();  //create the object for the shader
                                               // and go to Compile Shader function
                                               // and back here
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);    //compilamos el texto del vertexShader
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);//compilamos el texto del fragmentShader

    //Completamos el shader 'program'
    glAttachShader(program, vs); //Concatenamos el vertexSheder al program
    glAttachShader(program, fs);//Concatenamos el FragmentShader al program
    glLinkProgram(program);//hace un link del programa necesario para completar el shader
    glValidateProgram(program);//Validamos el programa

    //borramos los datos almacenados en vs y fs, ya tenemos el shader creado.
    glDeleteShader(vs);  
    glDeleteShader(fs);

    return program;
}


void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
    GLCall(glUseProgram(0));
}

//Set uniforms
void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform ' " << name << " ' doesn't exist " << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}