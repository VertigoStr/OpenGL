#include <iostream>
#include <map>
#include <string>
#include "Utils.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
using namespace std;
using namespace glm;

class TextBox
{
public:
	TextBox();
	TextBox(int w, int h);
	void Draw(ivec2 pos, vec4 color);
	~TextBox();
	void setSize(ivec2 size);

private:
	GLuint shader;
	int WIDTH, HEIGHT;
	GLuint VAO, VBO;
	void InitParameters();
};