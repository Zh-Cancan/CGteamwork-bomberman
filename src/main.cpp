#include <iostream>
#include <fstream>
#include <windows.h>

//#include "../include/shader.h"
//#include "../include/gui.h"
#include "../include/bomberman.h"
#include "../include/usingOpengl.h"
#include "../include/utils/FileUtils.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
//unsigned int genVAO();
GLFWwindow* initOpengl();

int main()
{
    /* 重点在图形学效果，可以不做记录存储与读取 */

    // GLFW或GLAD初始化，失败返回NULL
    GLFWwindow* window;
    if (nullptr == (window = initOpengl()))
        return -1;

    

    // 循环绘制图像窗口(渲染循环) 
    /*while(!glfwWindowShouldClose(window))
    {
        // 输入
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // 渲染指令
        

        // 处理事件、交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();*/
    bomberman();
    return 0;
}

 void framebuffer_size_callback(GLFWwindow* window, int width, int height)
 {
     glViewport(0, 0, width, height);
 }

 void processInput(GLFWwindow *window)
 {
     if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
         glfwSetWindowShouldClose(window, true);
 }


 GLFWwindow* initOpengl()
 {
 /* 设置opengl绘图窗口参数 */
     glfwInit();
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
     //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

     /* 绘制GLFW窗口 */
     GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
     if (window == NULL)
     {
         std::cout << "Failed to create GLFW window" << std::endl;
         glfwTerminate();
         return nullptr;
     }

     /* 设置多个回调函数，相当于注册函数关联 */
     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

     /* 更换当前内容窗口 */
     glfwMakeContextCurrent(window);

     /* 将进程地址传给GLAD，方便GLAD获取openGL函数指针 */
     if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
     {
         std::cout << "Failed to initialize GLAD" << std::endl;
         return nullptr;
     }

     return window;
 }



