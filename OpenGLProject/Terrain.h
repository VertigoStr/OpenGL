#include <glew.h>
#include <glut.h>
#include <fwd.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <time.h>
#include <SOIL.h>
#include "Utils.h"
using namespace glm;
using namespace std;

class Terrain
{
public:
	Terrain(GLuint program, GLuint textures[4], int u, int v);
	Terrain(GLuint program, GLuint textures[4], const char* heightMapName);
	Terrain(float size);
	~Terrain();
	void RenderTerrain(mat4 proj, mat4 view, mat4 model);
	void LoadTerrain(const char *filename);
	float HeightAt(float x, float z);
private:	
	bool fromFile;
	float scale;
	float smoothKoeff;

	int IndicesCount1;
	unsigned int *indices;
	int u;
	int v;
	float **hmap;
	float maxX, minX, maxY, minY, maxZ, minZ;

	vec3* pos;
	vec3* normal;
	vec2* tex;

	GLuint program;
	GLuint terrainVao;
	GLuint texsN[4];
	GLuint texsS[4];

	int iView1;
	int iWorld1;
	int iProjection1;

	int Pos;
	int Normal;
	int Tex;
	
	void TerrainInfo();
	void GenerateHMap();
	void GenerateLandscape();
	void InitSettings();
	void InitElements();
	void Smooth();
};