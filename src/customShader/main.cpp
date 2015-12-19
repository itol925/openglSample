//g++ textfile.cpp main.cpp -framework opengl -lGLEW -lglfw3 -o run1
#include "textfile.h"
#include <gl/glew.h>
#include <glfw/glfw3.h>
#include <iostream>

using namespace std;

GLuint vShader, fShader;

void printGLVersion(){
	const GLubyte *render = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	cout<< "GL vendor:			"<< vendor << endl;
	cout<< "GL Renderer:		"<< render << endl;
	cout<< "GL Version(string):	"<< version << endl;
	cout<< "GL Version(integer):"<< major << "." << minor << endl;
	cout<< "GLSL Version:		"<< glslVersion << endl;
}

void initShader(const char * vShaderFile, const char * fShaderFile){
	GLenum err = glewInit();  
	if( GLEW_OK != err ){  
		cout <<"Error initializing GLEW: " << glewGetErrorString(err) << endl;  
		exit(1);
	}  
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

void Display(GLFWwindow * window, int width, int height){
	float ratio = width / (float) height;
	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(window)){
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
void error_cb(int error, const char* description){
	fputs(description, stderr);
	printf("run error:%s\r", description);
}

void key_cb(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (action == GLFW_PRESS){
		switch(key)  {  
			case GLFW_KEY_ESCAPE :  
				glDeleteShader(vShader);  
				glUseProgram(0); 
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
		}
	}	
}

void initGL(GLFWwindow * window){
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
}

int main(){
	if(!glfwInit()){
		printf("run error: glfwinit error!\r");
		exit(EXIT_FAILURE);	
	}
	glfwSetErrorCallback(error_cb);
	GLFWwindow * window = glfwCreateWindow(200, 200, "fuck my wife", NULL, NULL);
	glfwSetKeyCallback(window, key_cb);
	initGL(window);
	printGLVersion();
	initShader("basic.vert","basic.frag");
	Display(window, 300, 300);
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}
