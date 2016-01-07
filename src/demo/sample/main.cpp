///////////////////////////////////////////////////////////////////////
//
// triangles.cpp
//
///////////////////////////////////////////////////////////////////////
 
 
?//--------------------------------------------------------------------
//
// �ڳ���һ��ͷ,���ǰ����������ͷ�ļ�,
// ������һЩȫ�ֱ���(��ͨ���ǲ���ȫ�ֱ���������,����ֻ��Ϊ��˵��һЩ��������)
// �Լ�����һЩ���õĳ���ṹ
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
// init()���������������Ǻ�����õ���һЩ����.���綥����Ϣ,�����
//
 
void init(void) {
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);
 
    // ��������ָ����Ҫ��Ⱦ�����������ε�λ����Ϣ.
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
 
    // Ȼ��ʹ���˱����vertex��fragment shaders
    ShaderInfo  shaders[] = {
            { GL_VERTEX_SHADER, "triangles.vert" },
            { GL_FRAGMENT_SHADER, "triangles.frag" },
            { GL_NONE, NULL }
    };
 
    // LoadShaders()�������Զ���(����û�и���)��һ������,
    // ���ڼ�ΪGPU׼��shaders�Ĺ���,�������ϸ����
    GLuint program = LoadShaders(shaders);
    glUseProgram(program);
    // ����ⲿ�����ǳ�Ϊshader plumbing,
    // ���ǰ���Ҫ�����ݺ�shader�����еı���������һ��,�������ϸ����
    glVertexAttribPointer(vPosition, 2, GL_FLOAT,
                          GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
}
 
//---------------------------------------------------------------------
//
// display
//
// �������������������Ⱦ�ĵط�.������OpenGL�ĺ������������ݽ�����Ⱦ.
// �������е�display������������������������.
//
 
void display(void) {
    // 1. ����glClear()��մ���
    glClear(GL_COLOR_BUFFER_BIT);
 
    // 2. ����OpenGL������������Ⱦ��Ķ���
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
 
    // 3. ����ͼ����Ƶ�����
    glFlush();
}
 
//---------------------------------------------------------------------
//
// main
//
// main()�������ڴ�������,����init()����,�����뵽�¼�ѭ��(event loop).
// �����Իῴ��һЩ��gl��ͷ�ĺ���,���������������ͬ.
// ��Щ�������Ե�������,�Ա����ǿ����ڲ�ͬ��ϵͳ�и������ʹ��OpenGL.
// ��������ʹ�õ���GLUT��GLEW.
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