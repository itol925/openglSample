#include "glinit.h"

void InitGlfw(){
	if(!glfwInit()){
		printf("run error: glfwinit error!\r");
		exit(EXIT_FAILURE);	
	}
}
GLFWwindow* CreateWindow(int w, int h, char* title){	
	GLFWwindow* window = glfwCreateWindow(w, h, title, NULL, NULL);

	return window;
}

int Run(GLFWwindow * window, void (*display)(int, int)){
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	while (!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		if(display){
			display(width, height);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
