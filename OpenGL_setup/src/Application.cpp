#include <GL/glew.h>       
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>  
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())//inicia la bilioteca, en caso de no estar inicialiado correctamente el programa se cerrará
        return -1;

    // to create the vertex array
    /*Especificamos la versión de la API con la que el contexto 
    de la ventana creada será compatible*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /*Especificamos para que perfil de OGL se crea el contexto */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Texture in square", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);  //to synchronize the animation

    //---La segunda libreria se utiliza despues de crear el contexto anterior
    /*inicializamos glew, al igual que con glfw3, si sucede
    algun conflicto, pasará un reporte de este a la consola y cerrará el programa. */
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    //Imprime la versión de OGL con la que estamos trabajando
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        // include the texture positions to be mapped
        float positions[] = {
            -0.5f, -0.5f, 0.f, 0.f,      // 0 the bottom left
             0.5f, -0.5f, 1.f, 0.f,      // 1 the bottom right side
             0.5f,  0.5f, 1.f, 1.f,      // 1 the top right
            -0.5f,  0.5f, 0.f, 1.f,      // 2 the top left
        };
        /*El indice forma parte dle index buffer y nos ayuda a mantener el orden en el que se utilizarán 
        los vertices para imprimir la figura en nuestra ventana, en este caso esta usando los vertices
        0,1,2 para formar un triangulo, y adjunto se forma un segundo triangulo siguiendo los vertices 2, 3 y 0*/
        unsigned int indices[] = { 
            0, 1, 2,
            2, 3, 0
        };
        
        /*Toda función del code utilizada en el presente proyecto se ejecutará
        a travez del metodo GLCall de la clase 'Render', esto nos permitira saber
        donde y cual error se produce en caso de que este exista*/

        //defining how openGL is going to blend alpha
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  //src alpha = 0; dest = 1 - 0 = 0


        //Creamos objeto de tipo VertexArray
        VertexArray va;
        //Creamos objeto de tipo VertexBuffer
        VertexBuffer vb(positions, 4 * 4 * sizeof(float)); // expand the buffer to 4 elements per vertex
        //Se crea objeto de tipo VertexBufferLayout
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);  // adding to more attributes
        va.AddBuffer(vb, layout);
        //Se crea objeto de tipo IndeyBuffer
        IndexBuffer ib(indices, 6);
        //Se crea objeto de tipo Shader
        Shader shader("res/shaders/Basic.shader");//Se obtienen los atributos del shader
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        //Se crea objeto de tipo Texture
        Texture texture("res/textures/texture0.png");//Se obtienen los la textura qeu se utilizará
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);  //the slot is 0

        /*Eliminamos los viculos de los objetos*/
        va.Unbind();    //glBindVertexArray(0)
        vb.Unbind();    //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        ib.Unbind();    //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        shader.UnBind();  //GLCall(glUseProgram(0));

        //Se crea un objeto de tipo renderer para poder 
        Renderer renderer;

        // to create the animation that change the color 
        /*No se usa para este preyecto*/
        //float r = 0.0f;
        //float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();  //GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // use the shader and bind the buffer and ibo each time in case that the buffer change
            shader.Bind();   //GLCall(glUseProgram(shader));
            shader.SetUniform4f("u_Color", 0.0f, 0.3f, 0.8f, 1.0f);   //GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
            //Se dibuja en la ventana a travez de un metodo de la clase Renderer. 
            renderer.Draw(va, ib, shader);

            /*GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));/*Esta función se ejecuta en 
                                                                                el método render.draw*/

            /*part of the animation
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;
            */

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}