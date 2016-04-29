#include <glew.h>
#include <glut.h>
#include <fwd.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <time.h>
#include <SOIL.h>
#include "Utils.h"
using namespace glm;
using namespace std;

class Water
{
public:
	Water(int rows, int cols);
	void Draw(mat4 proj, mat4 view, mat4 model, GLuint reflectionTexture, GLuint environmentMap, vec3 camera, bool geometry, float waterSpeed);
	~Water();

private:
	float waterHeight;
	int rows;
	int columns;

	GLuint vao;
	GLuint program;
	int indices_count;
	GLuint normalMap, heightMap;

	void generateMesh();
	float randomInRange(float min, float max);
};