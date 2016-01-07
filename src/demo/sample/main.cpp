///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////
 
 
?//--------------------------------------------------------------------
//
// 在程序一开头,我们包含了所需的头文件,
// 声明了一些全局变量(但通常是不用全局变量在做的,这里只是为了说明一些基本问题)
// 以及其他一些有用的程序结构
//
 
#include <iostream>
using namespace std;
 
#include "vgl.h"
#include "LoadShaders.h"
 
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };
 
GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
 
const GLuint NumVertices = 6;
 
?//---------------------------------------------------------------------
//
// init
//
// init()函数用于设置我们后面会用到的一些数据.例如顶点信息,纹理等
//
 
void init(void) {
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
 
    // 我们首先指定了要渲染的两个三角形的位置信息.
    GLfloat  vertices[NumVertices][2] = {
        { -0.90, -0.90 },  // Triangle 1
        {  0.85, -0.90 },
        { -0.90,  0.85 },
        {  0.90, -0.85 },  // Triangle 2
        {  0.90,  0.90 },
        { -0.85,  0.90 }
    };
 
    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                     vertices, GL_STATIC_DRAW);
 
    // 然后使用了必需的vertex和fragment shaders
    ShaderInfo  shaders[] = {
            { GL_VERTEX_SHADER, "triangles.vert" },
            { GL_FRAGMENT_SHADER, "triangles.frag" },
            { GL_NONE, NULL }
    };
 
    // LoadShaders()是我们自定义(这里没有给出)的一个函数,
    // 用于简化为GPU准备shaders的过程,后面会详细讲述
    GLuint program = LoadShaders(shaders);
    glUseProgram(program);
    // 最后这部分我们成为shader plumbing,
    // 我们把需要的数据和shader程序中的变量关联在一起,后面会详细讲述
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
}
 
//---------------------------------------------------------------------
//
// display
//
// 这个函数是真正进行渲染的地方.它调用OpenGL的函数来请求数据进行渲染.
// 几乎所有的display函数都会进行下面的三个步骤.
//
 
void display(void) {
    // 1. 调用glClear()清空窗口
    glClear(GL_COLOR_BUFFER_BIT);
 
    // 2. 发起OpenGL调用来请求渲染你的对象
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
 
    // 3. 请求将图像绘制到窗口
    glFlush();
}
 
//---------------------------------------------------------------------
//
// main
//
// main()函数用于创建窗口,调用init()函数,最后进入到事件循环(event loop).
// 这里仍会看到一些以gl开头的函数,但和上面的有所不同.
// 这些函数来自第三方库,以便我们可以在不同的系统中更方便地使用OpenGL.
// 这里我们使用的是GLUT和GLEW.
//
 
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(512, 512);
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow(argv[0]);
 
    if (glewInit()) {
        cerr << "Unable to initialize GLEW ... exiting" << endl; exit(EXIT_FAILURE);
    }
    init();
 
    glutDisplayFunc(display);
 
    glutMainLoop();
}