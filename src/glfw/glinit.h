#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

void InitGlfw();
GLFWwindow* CreateWindow(int w, int h, char* title);
int Run(GLFWwindow * window, void (*display)(int, int));
