#include "TextBox.h"

TextBox::TextBox(int w, int h)
{
	shader = LoadShaders("Shaders/Text/textBox.vert", "Shaders/Text/textBox.frag");
	WIDTH = w;
	HEIGHT = h;
	InitParameters();
}


TextBox::TextBox()
{
	shader = LoadShaders("Shaders/Text/textBox.vert", "Shaders/Text/textBox.frag");
}

void TextBox::setSize(ivec2 size)
{
	WIDTH = size.x;
	HEIGHT = size.y;
	InitParameters();
}

void TextBox::Draw(ivec2 pos, vec4 color)
{
	glViewport(0, 0, WIDTH, HEIGHT);
	if (VAO == NULL)
	{
		cout << "Failed to open VAO!" << endl;
		exit(-1);
	}
	ivec2 size = ivec2(WIDTH, HEIGHT);
    if(color.a < 1.f)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    glUseProgram(shader);
	glUniform4f(glGetUniformLocation(shader, "color"), color.r, color.g, color.b, color.a);
	mat4 proj = ortho(0.0f, static_cast<GLfloat>(size.x), 0.0f, static_cast<GLfloat>(size.y));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, value_ptr(proj));
    glBindVertexArray(VAO);
    GLfloat vertices[6][4] = {
        { pos.x,     pos.y + size.y,   0.0, 0.0 },            
        { pos.x,     pos.y,       0.0, 1.0 },
        { pos.x + size.x, pos.y,       1.0, 1.0 },

        { pos.x,     pos.y + size.y,   0.0, 0.0 },
        { pos.x + size.x, pos.y,       1.0, 1.0 },
        { pos.x + size.x, pos.y + size.y,   1.0, 0.0 }           
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(NULL);
    if(color.a < 1.f)
    {
        glDisable(GL_BLEND);
    }
}

void TextBox::InitParameters()
{
	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextBox::~TextBox()
{
}