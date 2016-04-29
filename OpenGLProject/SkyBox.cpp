#include "SkyBox.h"

SkyBox::SkyBox(int Size)
{
	this->skybox = LoadShaders("Shaders/SkyBox/SkyBox.vert", "Shaders/SkyBox/SkyBox.frag");

	vector<const GLchar*> cube;
	cube.push_back("Data/SkyBox/Night/left.jpg");
	cube.push_back("Data/SkyBox/Night/right.jpg");
	cube.push_back("Data/SkyBox/Night/top.jpg");
	cube.push_back("Data/SkyBox/Night/top.jpg");
	cube.push_back("Data/SkyBox/Night/back.jpg");
	cube.push_back("Data/SkyBox/Night/front.jpg");
	view = glGetUniformLocation(skybox, "view");
	proj = glGetUniformLocation(skybox, "projection");
	model = glGetUniformLocation(skybox, "model");
	texture = glGetUniformLocation(skybox, "skybox");

	LoadCubeMap(cube);
	GenerateCubeMap(Size);
}

SkyBox::SkyBox(GLuint skybox, int Size, vector<const GLchar*> faces)
{
	this->skybox = skybox;

	view = glGetUniformLocation(skybox, "view");
	proj = glGetUniformLocation(skybox, "projection");
	model = glGetUniformLocation(skybox, "model");
	texture = glGetUniformLocation(skybox, "skybox");

	LoadCubeMap(faces);
	GenerateCubeMap(Size);
}

SkyBox::~SkyBox()
{
	glDeleteProgram(skybox);
	glDeleteVertexArrays(1, &cubeMapVAO);
	glDeleteTextures(1, &textureID);
}

void SkyBox::Draw(mat4 mProjection, mat4 mView, mat4 mModel)
{
	glUseProgram(skybox);	

	glUniformMatrix4fv(view, 1, GL_FALSE, value_ptr(mView));
	glUniformMatrix4fv(proj, 1, GL_FALSE, value_ptr(mProjection));
	glUniformMatrix4fv(model, 1, GL_FALSE, value_ptr(mModel));
	
	glBindVertexArray(cubeMapVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(texture, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glActiveTexture(0);

	glUseProgram(0);
}

GLuint SkyBox::getTextureId()
{
	return textureID;
}

void SkyBox::GenerateCubeMap(double N)
{
	GLfloat cubeMapVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
   
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
	for (int i = 0; i < sizeof(cubeMapVertices)/sizeof(*cubeMapVertices); i++)
	{
		cubeMapVertices[i] *= N;
	}
	GLuint cubeMapVBO;
    glGenVertexArrays(1, &cubeMapVAO);
    glGenBuffers(1, &cubeMapVBO);
    glBindVertexArray(cubeMapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeMapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeMapVertices), &cubeMapVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
}
void SkyBox::LoadCubeMap(vector<const GLchar*> faces)
{
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width,height;
    unsigned char* image;
	
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
        );
		SOIL_free_image_data(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}