#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<iostream>

/* Create the Shader
Un sder es un programa que ouede interpretar los datos del vertexbuffer*/
static unsigned int CompileShader(unsigned int type, const std::string& source)
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
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id); //borra el shader creado. 
        return 0;
    }

    return id;
}
/*El vertexShader es texto que el programa recibe como un apuntador
similar ocurre con el fragmentShader*/
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();/*Crea un objeto para el shader llamado program*/
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
/*end of create the shader*/

int main(void)
{
    GLFWwindow* window; //Window es un objeto de tipo GLFwindow

    /* Initialize the library */
    if (!glfwInit()) //inicia la bilioteca, en caso de no estar inicialiado correctamente el programa se cerrará
        return -1;

    /* Creamos una ventana en el contexto OGL de 640 * 480, con el nombre ‘Mi primer triángulo’ */
    window = glfwCreateWindow(640, 480, "Mi primer triangulo", NULL, NULL);
    //Si por alguna razón no se crea correctamente la ventana, se cierra el GLFW creado y termina el programa
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Se genera el contexto del objeto para poder utilizarlo*/
    glfwMakeContextCurrent(window);
    
        //---La segunda libreria se utiliza despues de crear el contexto anterior
        /*inicializamos glew, al igual que con glfw3, si sucede
        algun conflicto, pasará un reporte de este a la consola y cerrará el programa. */
    if (glewInit() != GLEW_OK) {
        std::cout << "Error in Glew" << std::endl;
        exit(1);
    }
        //Imprime la versión de OGL con la que estamos trabajando
    std::cout << glGetString(GL_VERSION) << std::endl;
        
        //Creamos el buffer
        //Reacemos el triangulo --Datos del VertexBuffer
    float position[6] = { -0.5f,-0.5f, 
                            0.0f, 0.5f, 
                            0.5f, -0.5 };
    unsigned int buffer; //creamos el identificador para el bufer
    glGenBuffers(1, &buffer); // Generamos el buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);//Conectamos el buffer 
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);/*Cargamos los 6 datos al buffer 
                                                                               pero debe ser en bytes. */

        //Apuntador a los atributos
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, 2 *sizeof(float),0);//Toma 2 valores por vertice. 
    glEnableVertexAttribArray(0);
        
    /*Podemos pasar al DRAW CALL en el loop
    sin embargo a o continuación veremos otros atributos
    que nos permiran manipular más nuestros shaders*/

    /*The vertex Shader and fragment shader programs*/
    //lenguaje glsl
    /*Crea la cadena para el vertexShader (crea los vertices):
    indicamos la versión del lenguaje, indica a partir
    de que dato en el array empieza a leer,
    inicia el programa principal para el vs, 
    por ultimo mandamos las posiciones*/
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    /*Crea la cadena para el fragmentShader (pinta los pixeles): 
    indicamos la versión del lenguaje,
    inicia el programa principal para el fs,
    mandamos el color*/
    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.0, 1.0, 1.0, 1.0);\n"//RGBa
        "}\n";
    
    /*llamamos a la función CreateShader enviando como parametros
    los programas del vertexShader y fragmentShader
    el cual retorna el objeto del shader tipo program*/
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /*the end of  vertex Shader and fragment shader programs*/

    /* Loop que se ejecuta siempre que el programa este corriendo, 
    aquí ocurre todo lo que visualizamos en la pantalla. */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //DRAW CALL
            //hasta aqui sabe que tiene un buffer con datos que pertenecen aun triangulo
            //ahora le decimos como lo hará
        glDrawArrays(GL_TRIANGLES, 0, 3);/*1-Que hará,
                                         2-de donde empieza en el array,
                                         3-cuantos valores necesita*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}