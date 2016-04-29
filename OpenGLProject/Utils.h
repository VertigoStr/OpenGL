#include <glew.h>
#include <glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SOIL.h>
using namespace std;

GLuint LoadShaders(const char *vertexshader, const char *fragmentshader);
GLuint LoadTexture(const char *image);
GLuint LoadCube(GLuint program);
GLuint LoadFrameBufferTexture(int w, int h);