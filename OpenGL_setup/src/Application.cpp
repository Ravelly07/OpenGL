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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())//inicia la bilioteca, en caso de no estar inicialiado correctamente el programa se cerrar�
        return -1;

    // to create the vertex array
    /*Especificamos la versi�n de la API con la que el contexto 
    de la ventana creada ser� compatible*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /*Especificamos para que perfil de OGL se crea el contexto */
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "pruebas", NULL, NULL);
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
    algun conflicto, pasar� un reporte de este a la consola y cerrar� el programa. */
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;
    //Imprime la versi�n de OGL con la que estamos trabajando
    glEnable(GL_DEPTH_TEST);
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        // include the texture positions to be mapped
        float positions[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,//0
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//1
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//1
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,//2
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,//0

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//3
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,//4
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,//5
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,//5
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//3

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//6
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//1
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//7
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//7
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//3
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//6

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//8
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//1
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//9
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//9
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//8

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//7
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,//4
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,//4
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//3
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//7

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,//2
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//1
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//8
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//8
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f //2
        };        
        /*Toda funci�n del code utilizada en el presente proyecto se ejecutar�
        a travez del metodo GLCall de la clase 'Render', esto nos permitira saber
        donde y cual error se produce en caso de que este exista*/

        //defining how openGL is going to blend alpha
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  //src alpha = 0; dest = 1 - 0 = 0


        //Creamos objeto de tipo VertexArray
        VertexArray va;
        //Creamos objeto de tipo VertexBuffer
        VertexBuffer vb(positions, sizeof(positions)); // expand the buffer to 4 elements per vertex
        //Se crea objeto de tipo VertexBufferLayout
        VertexBufferLayout layout;
        layout.Push<float>(3); //coordenadas
        layout.Push<float>(2);  // textura corrdenadas
        va.AddBuffer(vb, layout);
        
        /*
        //Se crea objeto de tipo IndeyBuffer
        IndexBuffer ib(indices, 6);
        */

        //Se crea objeto de tipo Shader
        Shader shader("res/shaders/Basic.shader");//Se obtienen los atributos del shader
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
        //Se crea objeto de tipo Texture
        Texture texture("res/textures/texture6.jpg");//Se obtienen los la textura qeu se utilizar�
        texture.Bind(0);
        shader.SetUniform1i("u_Texture", 0);  //the slot is 0
        /*
        Texture texture1("res/textures/texture7.png");//Se obtienen los la textura qeu se utilizar�
        texture1.Bind(1);
        shader.SetUniform1i("u_Texture", 1);  //the slot is 1
        */

        /*Eliminamos los viculos de los objetos*/
        va.Unbind();    //glBindVertexArray(0)
        vb.Unbind();    //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        //ib.Unbind();    //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        shader.UnBind();  //GLCall(glUseProgram(0));

        //Se crea un objeto de tipo renderer para poder 
        Renderer renderer;

        // to create the animation that change the color 
        
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();  //GLCall(glClear(GL_COLOR_BUFFER_BIT));


            // create transformations
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);

            model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            glm::mat4 mvp = projection * view * model;
            //model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
             
            // use the shader and bind the buffer and ibo each time in case that the buffer change
            shader.Bind();   //GLCall(glUseProgram(shader));
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);   //GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
            //Se dibuja en la ventana a travez de un metodo de la clase Renderer. 
            

            shader.SetMat4("u_MVP", mvp);

            renderer.Draw(va, shader);
            /*GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));/*Esta funci�n se ejecuta en 
                                                                                el m�todo render.draw*/
        
            //part of the animation
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;
    

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}
