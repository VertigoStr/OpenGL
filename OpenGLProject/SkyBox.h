#include <glew.h>
#include <glut.h>
#include <fwd.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <SOIL.h>
#include "Utils.h"
using namespace std;
using namespace glm;

class SkyBox
{
public:
	SkyBox(GLuint skybox, int Size, vector<const GLchar*> faces);
	SkyBox(int Size);
	void Draw(mat4 mProjection, mat4 mView, mat4 mModel);
	GLuint getTextureId();
	~SkyBox();

private:
	GLuint cubeMapVAO;
	GLuint textureID;
	GLuint skybox;

	GLuint view;
	GLuint proj;
	GLuint model;
	GLuint texture;

	void GenerateCubeMap(double N);
	void LoadCubeMap(vector<const GLchar*> faces);
};