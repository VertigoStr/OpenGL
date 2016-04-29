#include "Text.h"

Text::Text(int screenWidth, int screenHeight)
{
	shader = LoadShaders("Shaders/Text/vs.vert", "Shaders/Text/fs.frag");
	fontPath = "Data/Text/font1.ttf";
	WIDTH = screenWidth;
	HEIGHT = screenHeight;
	InitParameters();
	textBox.setSize(ivec2(screenWidth, screenHeight));
}

Text::~Text()
{
}

void Text::RenderText(string txt[], string FPS, ivec2 boxParameters, float step, float scale, int arraySize)
{
	textBox.Draw(boxParameters, vec4(0.f, 0.f, 0.f, 0.5f));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	RenderText("F1 - menu(on/off)", 0.0f, HEIGHT - step, scale, vec3(1.0, 1.0f, 1.0f));	
	RenderText(FPS.c_str(), 0.0f, HEIGHT - 2 * step, scale, vec3(1.0, 1.0f, 1.0f));	
	float currentHeight = HEIGHT - 2 * step;
	for (int i = 0; i < arraySize; i++)
	{
		currentHeight -= step;
		RenderText(txt[i].c_str(), 0.0f, currentHeight, scale, vec3(1.0, 1.0f, 1.0f));
	}
	glDisable(GL_BLEND);
}

void Text::RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color)
{
	glUseProgram(shader);
	mat4 projection = ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, value_ptr(projection));

	glUniform3f(glGetUniformLocation(shader, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVao);

    string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::InitParameters()
{
	FT_Library ft;
    if (FT_Init_FreeType(&ft))
        cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;

    FT_Face face;
		
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
        cout << "ERROR::FREETYPE: Failed to load font" << endl;

    FT_Set_Pixel_Sizes(face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
            continue;
        }
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        Character character = {
            texture,
            ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

	glGenVertexArrays(1, &textVao);
    glGenBuffers(1, &VBO);
    glBindVertexArray(textVao);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}