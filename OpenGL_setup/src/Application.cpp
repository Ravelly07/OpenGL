#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

//code by TheCherno https://thecherno.com/opengl

/* Create the Shader*/
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();//apunta al inicio del array
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
/*end of create the shader*/

int main(void)
{
    GLFWwindow* window; //Window es un objeto de tipo GLFwindow

    /* Initialize the library */
    if (!glfwInit()) //inicia la bilioteca, en caso de no estar inicialixado correctamente el programa se cerrará
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Mi primer triangulo", NULL, NULL);
    if (!window) //Si por alguna razón no se crea correctamente la ventana, se cierra el GLFW creado y termina el programa
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
        //---La segunda libreria se utiliza despues de crear el contexto
        //(la instrucción superior)
    if (glewInit() != GLEW_OK) {
        std::cout << "Error in Glew" << std::endl;
        exit(1);
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

        //Reacemos el triangulo --VertexBufer
    float position[6] = { -0.5f,-0.5f, 
                            0.0f, 0.5f, 
                            0.5f, -0.5 };
        //creamos el identificador para el bufer
    unsigned int buffer;
        //creamos el buffer
    glGenBuffers(1, &buffer); // Buffer identifier
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);//6 datos pero debe ser en bytes

        //APUNTADOR a los atributos
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, 2 *sizeof(float),0);
    glEnableVertexAttribArray(0);


        //---Hasta aquí y pasamos al drawCall --CALL

    /*The vertex Shader and fragment shader programs*/
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.0, 0.5, 0.5, 0.0);\n"//RGBa
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /*the end of  vertex Shader and fragment shader programs*/

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
/*
        //INICIAMOS EN EL RENDER A CREAR UN TRIANGULO
            //si hay un glB, debe haber un glE
        glBegin(GL_TRIANGLES);//van todos los parametros de lo que crearemos dentro del  '()'
            //vertices de un triangulo de 2 dimenciones
        glVertex2f(-0.5f,-0.5f);
        glVertex2f(0.0f,0.5f);
        glVertex2f(0.5f,-0.5f);

        glEnd();
*/

        //DRAW CALL
        glDrawArrays(GL_TRIANGLES, 0, 3);//1-Que hará,2-de donde empieza en el array,3-cuantos valores necesita
            //hasta aqui sabe que tiene un buffer condatos que pertenecen aun triangulo
            //ahora le decimos como lo hará

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}