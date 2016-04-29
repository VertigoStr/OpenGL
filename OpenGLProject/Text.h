#include <iostream>
#include <map>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Utils.h"
#include "TextBox.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
using namespace std;
using namespace glm;


struct Character {
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class Text
{
public:
	Text(int screenWidth, int screenHeight);
	~Text();
	void RenderText(string txt[], string FPS, ivec2 boxParameters, float step, float scale, int arraySize);

private:
	void RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color);
	void InitParameters();
	GLuint shader;
	string fontPath;
	map<GLchar, Character> Characters;
	GLuint textVao;
	GLuint VBO;
	int WIDTH, HEIGHT;
	TextBox textBox;
};