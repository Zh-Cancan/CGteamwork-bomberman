#include "ball.h"
#include "shader.h"
#include "gui.h"

#include <vector>

unsigned int Ball::genVAO()
{
    /* 生成圆球的顶点数据和索引数据 */
    std::vector<float> sphereVertices;
	std::vector<int> sphereIndices;

	/*2-计算球体顶点*/
	//生成球的顶点
	for (int y=0;y<=Y_SEGMENTS;y++)
	{
		for (int x=0;x<=X_SEGMENTS;x++)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			sphereVertices.push_back(xPos);
			sphereVertices.push_back(yPos);
			sphereVertices.push_back(zPos);
            sphereVertices.push_back(0.5f + 0.2 * std::sin(xSegment * PI * 2));
            sphereVertices.push_back(0.3f + 0.2 * std::sin(ySegment * PI * 2));
            sphereVertices.push_back(0.7f + 0.1 * std::sin(xSegment * PI * 2));
		}
	}

	//生成球的Indices
	for (int i=0;i<Y_SEGMENTS;i++)
	{
		for (int j=0;j<X_SEGMENTS;j++)
		{
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j+1);
			sphereIndices.push_back(i* (X_SEGMENTS + 1) + j);
			sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
			sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
		}
	}

    /* 创建顶点数组对象，记录一个绘制的完整配置 */
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* 配置顶点缓冲对象，将数据一次性送给显卡 */
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), &sphereVertices[0], GL_STATIC_DRAW);

    /* 配置索引缓冲对象，减少数据存储量 */
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(int), &sphereIndices[0], GL_STATIC_DRAW);

    /* 告诉openGL如何解析VBO中的数据(各个参数含义略) */
    // 位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    /* VAO记录完毕，可以解绑当前VAO、VBO防止VAO额外记录配置 */
    /* 额外说一句，GL_ARRAY_BUFFER是媒介，将该VBO绑在它上面，指定Pointer时就默认使用该VBO，并由VAO进行记录，因此之后可以随便解绑VBO */
    /* 相比之下，GL_ELEMENT_ARRAY_BUFFER会记录glBindBuffer函数，所以解绑VAO之前不能解绑VBO */
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return VAO;
}


void Ball::draw()
{
        // 编译生成着色器
        Shader ourShader("resource/sphereShader.ver", "resource/sphereShader.fra");
        ourShader.use();
        
        unsigned int VAO1 = genVAO();

        glm::mat4 view            = glm::mat4(1.0f);
        glm::mat4 projection      = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.3f, 0.0f, -5.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(ourShader.ID, "model");

        int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(VAO1);

        glm::mat4 model      = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, X_SEGMENTS * Y_SEGMENTS * 6, GL_UNSIGNED_INT, 0);
}