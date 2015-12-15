#include "textfile.h"
#include <iostream>
#include <glut/glut.h>

#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArrays glBindVertexArraysAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#endif

using namespace std;

GLuint vShader, fShader;

float positionData[] = {
	-0.8f, -0.8f, 0.0f,
	0.8f, -0.8f, 0.0f,
	0.0f, 0.8f, 0.0f
};

float colorData[] = {
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,  
	0.0f, 0.0f, 1.0f
};

GLuint vaoHandle;

void printGLVersion(){
	//const GLubyte *render = glGetString(GL_RENDERER);
	//const GLubyte *vendor = glGetString(GL_VENDOR);
	//const GLubyte *version = glGetString(GL_VERSION);
	//const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	//GLint major, minor;
	//glGetIntegerv(GL_MAJOR_VERSION, &major);
	//glGetIntegerv(GL_MINOR_VERSION, &minor);

	//cout<< "GL vendor:			"<< vendor << endl;
	//cout<< "GL Renderer:		"<< render << endl;
	//cout<< "GL Version(string):	"<< version << endl;
	//cout<< "GL Version(integer):"<< major << "." << minor << endl;
	//cout<< "GLSL Version:		"<< glslVersion << endl;
}

void initShader(const char * vShaderFile, const char * fShaderFile){
	vShader = glCreateShader(GL_VERTEX_SHADER);
	if(0 == vShader){
		cerr << "ERROR: create vertex shader failed!"<< endl;
		exit(1);
	}
	// 把着色器源代码和着色器相关联
	const GLchar *vShaderCode = textFileRead(vShaderFile);
	const GLchar *vCodeArray[1] = { vShaderCode };

	//编译着色器对象
	glCompileShader(vShader);

	//检查编译是否成功
	GLint compileResult;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileResult);
	if(GL_FALSE == compileResult){
		GLint logLen;
		//得到编译日志长度
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0){
			char * log = (char*)malloc(logLen);
			GLsizei written;
			//得到日志并输出
			glGetShaderInfoLog(vShader, logLen, &written, log);
			cerr << "Vertex shader compile log :"<< endl;
			cerr << log << endl;
			free(log);
		}
	}

	// 创建着色器对象，片段着色器
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	if(0 == fShader){
		cerr << "ERROR: Create fragment shader failed!" << endl;
		exit(1);
	}
	const GLchar *fShaderCode = textFileRead(fShaderFile);
	const GLchar *fCodeArray[1] = { fShaderCode };
	glShaderSource(fShader, 1, fCodeArray, NULL);

	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileResult);
	if(GL_FALSE == compileResult){
		GLint logLen;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0){
			char *log = (char*)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(fShader, logLen, &written, log);
			cerr << "fragment shader compile log: "<< endl;
			cerr << log << endl;
			free(log);
		}
	}

	// 创建着色器对象
	GLuint programHandle = glCreateProgram();
	if(!programHandle){
		cerr << "ERROR : create program failed!" << endl;
		exit(1);
	}
	//添加着色器
	glAttachShader(programHandle, vShader);
	glAttachShader(programHandle, fShader);
	//链接
	glLinkProgram(programHandle);
	//查询链接的结果
	GLint linkStatus;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
	if(GL_FALSE == linkStatus){
		cerr << "ERROR: link shader program failed!" << endl;
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0){
			char * log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen, &written, log);
			cerr << "program log:" << endl;
			cerr << log << endl;
		}
	}else{
		glUseProgram(programHandle);
	}
}

void initVBO(){
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint posBuffer = vboHandles[0];
	GLuint colorBuffer = vboHandles[1];

	// 绑定vbo position
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	// 加载数据到vbo
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

	//vbo color
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

	// VAO
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);// 顶点坐标？？
	glEnableVertexAttribArray(1);//顶点颜色

	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
}

void display(){  
	glClear(GL_COLOR_BUFFER_BIT);  

	//绘制一个三角形(使用普通方法)  
	//使用VAO、VBO绘制  
	glBindVertexArray(vaoHandle);  
	glDrawArrays(GL_TRIANGLES,0,3);  
	glBindVertexArray(0);  

	glutSwapBuffers();  
}  

void keyboard(unsigned char key,int x,int y)  {  
	switch(key)  {  
		case 27:  
			glDeleteShader(vShader);  
			glUseProgram(0);  
			break;  
	}  
}  
void init(){  
	//初始化glew扩展库  
	GLenum err = glewInit();  
	if( GLEW_OK != err )  {  
		cout <<"Error initializing GLEW: " << glewGetErrorString(err) << endl;  
	}  

	initShader("basic.vert","basic.frag");  
	initVBO();
	glClearColor(0.0,0.0,0.0,0.0);  
	//glShadeModel(GL_SMOOTH);  
}  
int main(int argc,char** argv){
	glutInit(&argc,argv);  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  
	glutInitWindowSize(600,600);  
	glutInitWindowPosition(100,100);  
	glutCreateWindow("GLSL Test : Draw a triangle");  
	init();  
	glutDisplayFunc(display);  
	glutKeyboardFunc(keyboard);  

	glutMainLoop();  
	return 0; 
}
