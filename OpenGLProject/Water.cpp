#include "Water.h"

Water::Water(int rows, int cols)
{
	program = LoadShaders("Shaders/Water/vs.vert", "Shaders/Water/fs.frag");
	normalMap = LoadTexture("Data/Water/normalwater.png");
	heightMap = LoadTexture("Data/Water/WaterH00.jpg");

	this->rows = rows;
	this->columns = cols;
	waterHeight = -12.0f;
	generateMesh();
}

void Water::Draw(mat4 proj, mat4 view, mat4 model, GLuint reflectionTexture, GLuint environmentMap, vec3 camera, bool geometry, float waterSpeed)
{
	glUseProgram(program);
	glBindVertexArray(vao);
	
	glActiveTexture(GL_TEXTURE0 + 0);
	glUniform1i(glGetUniformLocation(program, "reflectionTexture"), 0);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glBindSampler(0, reflectionTexture);
	glActiveTexture(GL_TEXTURE0 + 1);
	glUniform1i(glGetUniformLocation(program, "WaterNormalMap"), 1);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glBindSampler(1, normalMap);
	glActiveTexture(GL_TEXTURE0 + 2);
	glUniform1i(glGetUniformLocation(program, "environmentMap"), 2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, environmentMap);
	glBindSampler(2, environmentMap);
	glActiveTexture(GL_TEXTURE0 + 3);
	glUniform1i(glGetUniformLocation(program, "heightMap"), 3);
	glBindTexture(GL_TEXTURE_2D, heightMap);
	glBindSampler(3, heightMap);

	float time = 0.001f * glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(glGetUniformLocation(program, "time"), time);	
	glUniform1f(glGetUniformLocation(program, "waterSpeed"), waterSpeed);	

	glUniform3f(glGetUniformLocation(program, "cameraPos"), camera.x, camera.y, camera.z);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, value_ptr(proj));

	glUniformMatrix3fv(glGetUniformLocation(program, "normalMatrix"), 1, GL_FALSE, value_ptr(transpose(inverse(model))));
	glDrawElements(geometry ? GL_LINES : GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
}

Water::~Water()
{
}

float Water::randomInRange(float min, float max)
{
    assert(min < max);
    double n = (double) rand() / (double) RAND_MAX;
    double v = min + n * (max - min);
    return v;
}

void Water::generateMesh()
{	
	int vertexCount = (rows - 1) * (columns - 1) * 6;
    int indexCount = vertexCount;
    vec3 *vertices = new vec3[vertexCount];
	vec3 *normal = new vec3[vertexCount];
	vec2 *tex = new vec2[vertexCount];
	int *indices = new int[indexCount];

    int vi = 0, ii = 0;
		for (int r = 0; r < rows - 1; ++r) {
	        for (int c = 0; c < columns - 1; ++c) {
				normal[vi] = vec3(0.0, 1.0, 0.0);
				tex[vi] = vec2(r + 0.5f, c + 0.5f) / (float)rows;;
                vertices[vi++] = vec3(r, 0.0f, c) + vec3(0, 0, 1);

				normal[vi] = vec3(0.0, 1.0, 0.0);
				tex[vi] = vec2(r + 0.5f, c + 1 + 0.5f) / (float)rows;
                vertices[vi++] = vec3(r, 0.0f, c + 1) + vec3(0, 0, 1);

				normal[vi] = vec3(0.0, 1.0, 0.0);
				tex[vi] = vec2(r + 1 + 0.5f, c + 0.5f) / (float)rows;
                vertices[vi++] = vec3(r + 1, 0.0f, c) + vec3(0, 0, 1);

				normal[vi] = vec3(0.0, 1.0, 0.0);
				tex[vi] = vec2(r + 1 + 0.5f, c + 0.5f) / (float)rows;
                vertices[vi++] = vec3(r + 1, 0.0f,  c) + vec3(0, 0, 1);

				normal[vi] = vec3(0.0, 1.0, 0.0);
				tex[vi] = vec2(r + 0.5f, c + 1 + 0.5f) / (float)rows;
                vertices[vi++] = vec3(r, 0.0f, c + 1) + vec3(0, 0, 1);

				normal[vi] = vec3(0.0, 1.0, 0.0);
				tex[vi] = vec2(r + 1 + 0.5f, c + 1 + 0.5f) / (float)rows;
                vertices[vi++] = vec3(r + 1, 0.0f, c + 1) + vec3(0, 0, 1);

                for (int j = 0; j < 6; ++j) {
                    indices[ii] = ii;
                    ++ii;
                }
			}
		}
    assert(vi == vertexCount && ii == indexCount);
	indices_count = indexCount;

	GLuint vbos[4];
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, (GLfloat*) vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(program, "position"));
	glVertexAttribPointer(glGetAttribLocation(program, "position"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &vbos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 2, (GLfloat*) tex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(program, "texCoords"));
	glVertexAttribPointer(glGetAttribLocation(program, "texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &vbos[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 3, (GLfloat*) normal, GL_STATIC_DRAW);
	glEnableVertexAttribArray(glGetAttribLocation(program, "normal"));
	glVertexAttribPointer(glGetAttribLocation(program, "normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &vbos[3]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(int),(GLuint*) indices, GL_STATIC_DRAW);
	glBindVertexArray(0);
	delete []vertices;
	delete []indices;
	delete []tex;
	delete []normal;
}