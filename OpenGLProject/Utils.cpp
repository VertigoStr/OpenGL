#include "Utils.h"
GLuint LoadShaders(const char *vertexshader, const char *fragmentshader)
{
	cout << "Use shaders: " << vertexshader << " " << fragmentshader << endl;
	cout << "========================================================================================" << endl;
	GLint compile = GL_FALSE;
	int errorInfo = 0;
	GLuint vsID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsID = glCreateShader(GL_FRAGMENT_SHADER);

	string vsCode = "";
	ifstream vsStream(vertexshader, ios::in);
	if (vsStream.is_open())
	{
		string line = "";
		while (getline(vsStream, line))
		{
			vsCode += "\n" + line;
		}
		vsStream.close();
	}
	else 
	{
		cout << "Impossible to open vertex shader file!" << endl;
	}

	string fsCode = "";
	ifstream fsStream(fragmentshader, ios::in);
	if (fsStream.is_open())
	{
		string line = "";
		while (getline(fsStream, line))
		{
			fsCode += "\n" + line;
		}	
		fsStream.close();
	}

	cout << "Try to compile vertex shader..." << endl;

	const char *vsInCode = vsCode.c_str();

	glShaderSource(vsID, 1, &vsInCode, NULL);
	glCompileShader(vsID);
	glGetShaderiv(vsID, GL_COMPILE_STATUS, &compile);
	glGetShaderiv(vsID, GL_INFO_LOG_LENGTH, &errorInfo);
	if (errorInfo > 0 || !compile)
	{
		vector<char> vsError(errorInfo + 1);
		glGetShaderInfoLog(vsID, errorInfo, NULL, &vsError[0]);
		cout << &vsError[0] << endl;
	}
	compile = GL_FALSE; 
	errorInfo = 0;

	cout << "Try to compile fragment shader..." << endl;

	const char *fsInCode = fsCode.c_str();

	glShaderSource(fsID, 1, &fsInCode, NULL);
	glCompileShader(fsID);
	glGetShaderiv(fsID, GL_COMPILE_STATUS, &compile);
	glGetShaderiv(fsID, GL_INFO_LOG_LENGTH, &errorInfo);
	if (errorInfo > 0 || !compile)
	{
		vector<char> fsError(errorInfo + 1);
		glGetShaderInfoLog(fsID, errorInfo, NULL, &fsError[0]);
		cout << &fsError[0] << endl;
	}
	
	compile = GL_FALSE; 
	errorInfo = 0;

	cout << "Try to link program..." << endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vsID);
	glAttachShader(program, fsID);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &compile);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &errorInfo);
	if (errorInfo > 0  || !compile) 
	{
		vector<char> programError(errorInfo + 1);
		glGetProgramInfoLog(program, errorInfo, NULL, &programError[0]);
		cout << &programError[0] << endl;
	}

	glDeleteShader(vsID);
	glDeleteShader(fsID);
	cout << "========================================================================================" << endl;
	return program;
}

GLuint LoadTexture(const char *image)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* img = SOIL_load_image(image, &width, &height, 0, SOIL_LOAD_RGB);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);	
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(img);
    return textureID;
}

GLuint LoadCube(GLuint program)
{
	cout << "Load cube" << endl;
	GLfloat vertices[] = {
        // Positions          // Normals           // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
	GLuint VBO, containerVAO;
    glGenVertexArrays(1, &containerVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(containerVAO);
	glVertexAttribPointer(glGetAttribLocation(program, "position"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(glGetAttribLocation(program, "position"));
    glVertexAttribPointer(glGetAttribLocation(program, "inNormal"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(glGetAttribLocation(program, "inNormal"));
    glVertexAttribPointer(glGetAttribLocation(program, "texCoords"), 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(glGetAttribLocation(program, "texCoords"));
    glBindVertexArray(0);

	return containerVAO;
}
GLuint LoadFrameBufferTexture(int w, int h)
{	
	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	return renderedTexture;
}