#include "terrain.h"
Terrain::Terrain(float size)
{
	this->program = LoadShaders("Shaders/Terrain/landscape.vert", "Shaders/Terrain/landscape.frag");

	GLuint textures[4];
	string textures_names[] = {"Data/Terrain/Textures/sand.jpg", "Data/Terrain/Textures/rs-grass00.jpg", "Data/Terrain/Textures/rock.jpg", "Data/Terrain/Textures/snow.jpg"};
	for (int i = 0; i < 4; i++)
		textures[i] = LoadTexture(textures_names[i].c_str());

	for (int i = 0; i < 4; i++)
		texsS[i] = textures[i];

	texsN[0] = glGetUniformLocation(this->program, "region1ColorMap");
	texsN[1] = glGetUniformLocation(this->program, "region2ColorMap");
	texsN[2] = glGetUniformLocation(this->program, "region3ColorMap");
	texsN[3] = glGetUniformLocation(this->program, "region4ColorMap");

	iWorld1 = glGetUniformLocation(this->program, "model");
	iView1 = glGetUniformLocation(this->program, "view");
	iProjection1 = glGetUniformLocation(this->program, "proj");

	Pos = glGetAttribLocation(this->program, "inPos");
	Normal = glGetAttribLocation(this->program, "inNormal");
	Tex = glGetAttribLocation(this->program, "inTex");

	this->scale = size;
	smoothKoeff = 0.1f;
	LoadTerrain("Data/Terrain/HeightMaps/4.jpg");
	fromFile = true;
	TerrainInfo();
}
Terrain::Terrain(GLuint program, GLuint textures[4], int u, int v)
{
	this->program = program;
	this->u = u;
	this->v = v;
	InitElements();
	
	scale = 15.5f;
	smoothKoeff = 0.01f;

	for (int i = 0; i < 4; i++)
		texsS[i] = textures[i];

	texsN[0] = glGetUniformLocation(this->program, "region1ColorMap");
	texsN[1] = glGetUniformLocation(this->program, "region2ColorMap");
	texsN[2] = glGetUniformLocation(this->program, "region3ColorMap");
	texsN[3] = glGetUniformLocation(this->program, "region4ColorMap");

	iWorld1 = glGetUniformLocation(this->program, "model");
	iView1 = glGetUniformLocation(this->program, "view");
	iProjection1 = glGetUniformLocation(this->program, "proj");

	Pos = glGetAttribLocation(this->program, "inPos");
	Normal = glGetAttribLocation(this->program, "inNormal");
	Tex = glGetAttribLocation(this->program, "inTex");
	fromFile = false;
	InitSettings();
}

Terrain::Terrain(GLuint program, GLuint textures[4], const char *heightMapName)
{
	this->program = program;

	for (int i = 0; i < 4; i++)
		texsS[i] = textures[i];
	
	scale = 15.5f;
	smoothKoeff = 0.01f;

	texsN[0] = glGetUniformLocation(this->program, "region1ColorMap");
	texsN[1] = glGetUniformLocation(this->program, "region2ColorMap");
	texsN[2] = glGetUniformLocation(this->program, "region3ColorMap");
	texsN[3] = glGetUniformLocation(this->program, "region4ColorMap");

	iWorld1 = glGetUniformLocation(this->program, "model");
	iView1 = glGetUniformLocation(this->program, "view");
	iProjection1 = glGetUniformLocation(this->program, "proj");

	Pos = glGetAttribLocation(this->program, "inPos");
	Normal = glGetAttribLocation(this->program, "inNormal");
	Tex = glGetAttribLocation(this->program, "inTex");

	LoadTerrain(heightMapName);
	fromFile = true;
}


Terrain::~Terrain()
{
	glDeleteTextures(4, texsN);
	glDeleteProgram(program);
	glDeleteVertexArrays(1, &terrainVao);

	delete []this->pos;
	delete []this->normal;
	delete []this->tex;
	delete []this->indices;
	for (int i = 0; i < this->u; i++)
		delete []this->hmap[i];
	delete []this->hmap;
}
void Terrain::GenerateHMap()
{
	srand(NULL);
	for (int i = 0; i<u; i++)
	{
		for (int j = 0; j<v; j++)
		{
			float x = ((float)i / (float)u) - 0.25f;
			float y = ((float)j / (float)v) - 0.25f;
			float xx = ((float)i / (float)u) - 0.5f;
			float yy = ((float)j / (float)v) - 0.5f;
			float h = xx*yy*xx*yy + 0.3f / (1.0f + (x*x + y*y)*50.0f);
			this->hmap[i][j] = h;
		}
	}
}
void Terrain::TerrainInfo()
{
	cout << "TerrainInfo:" << endl;
	cout << "========================================================================================" << endl;
	minY = pos[0].y;
	maxY = pos[0].y;

	minX = pos[0].x;
	maxX = pos[0].x;
	
	minZ = pos[0].z;
	maxZ = pos[0].z;
	for (int i = 0; i < u * v; i++)
	{
		if (minY > pos[i].y)
			minY = pos[i].y;
		
		if (maxY < pos[i].y)
			maxY = pos[i].y;

		if (minX > pos[i].x)
			minX = pos[i].x;
		
		if (maxX < pos[i].x)
			maxX = pos[i].x;

		if (minZ > pos[i].z)
			minZ = pos[i].z;
		
		if (maxZ < pos[i].z)
			maxZ = pos[i].z;
	}
	cout << "Min x:  " << minX << endl;
	cout << "Max x:  " << maxX << endl;
	cout << "Min y:  " << minY << endl;
	cout << "Max y:  " << maxY << endl;
	cout << "Min z:  " << minZ << endl;
	cout << "Max z:  " << maxZ << endl;
	cout << "========================================================================================" << endl;
}
void Terrain::GenerateLandscape()
{
	// Генерация карты высот
	if (!fromFile)
		GenerateHMap();

	// Генерация сетки вершин для вершинного буфера
	for (int i = 0; i < u; i++)
	{
		for (int j = 0; j < v; j++)
		{
			float x = (float)i / (float)u - 0.5f;	
			float y = (float)j / (float)v - 0.5f;
			this->pos[j * u + i] = vec3(x,  hmap[i][j], y) * scale;
			this->normal[j * u + i] = vec3(0, 1, 0);
			this->tex[j * u + i] = vec2(x + 0.5f, y + 0.5f);
		}
	}
	//Генерация  индексного буфера
	for (int i = 0; i < (u - 1); i++)
	{
		for (int j = 0; j < (v - 1); j++)
		{
			unsigned int indexa = j*(u - 1) + i;
			unsigned int indexb = j*u + i;
			indices[indexa * 6 + 0] = indexb;
			indices[indexa * 6 + 1] = indexb + 1 + u;
			indices[indexa * 6 + 2] = indexb + 1;

			indices[indexa * 6 + 3] = indexb;
			indices[indexa * 6 + 4] = indexb + u;
			indices[indexa * 6 + 5] = indexb + u + 1;
		}
	}
}
void Terrain::InitSettings()
{
	GenerateLandscape();
	int verticesCount1 = this->u * this->v;
	int indicesCount1 = this->IndicesCount1;
	GLuint vbos[4];
	glGenVertexArrays(1, &this->terrainVao);
	glGenBuffers(4, vbos);

	glBindVertexArray(vbos[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	glBufferData(GL_ARRAY_BUFFER,  3 * verticesCount1 * sizeof(float), this->pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(Pos);
	glVertexAttribPointer(Pos, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  

	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * verticesCount1 * sizeof(float), this->normal, GL_STATIC_DRAW);
	glEnableVertexAttribArray(Normal);
	glVertexAttribPointer(Normal, 3, GL_FLOAT, GL_FALSE, 0,NULL);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, 2 * verticesCount1 * sizeof(float),this->tex , GL_STATIC_DRAW);
	glEnableVertexAttribArray(Tex);
	glVertexAttribPointer(Tex, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,  indicesCount1*sizeof(unsigned int),this->indices, GL_STATIC_DRAW);

}

void Terrain::RenderTerrain(mat4 proj, mat4 view, mat4 model)
{
	glUseProgram(this->program);
    glEnable(GL_TEXTURE_2D);

	// пересылка нескольких текстур
    glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, texsS[0]);
	glUniform1i(texsN[0], 0);
	glBindSampler(0, texsN[0]);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, texsS[1]);
	glUniform1i(texsN[1], 1);
	glBindSampler(1, texsN[1]);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, texsS[2]);
	glUniform1i(texsN[2], 2);
	glBindSampler(2, texsN[2]);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, texsS[3]);
	glUniform1i(texsN[3], 3);
	glBindSampler(3, texsN[3]);

	glUniform1f(glGetUniformLocation(program, "HEIGHTMAP_SCALE"), 1.0f);

	glUniformMatrix4fv(this->iView1, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(this->iProjection1, 1, GL_FALSE, value_ptr(proj));
	glUniformMatrix4fv(this->iWorld1, 1, GL_FALSE, value_ptr(model));

	glBindVertexArray(this->terrainVao);
	glDrawElements(GL_TRIANGLES, this->IndicesCount1, GL_UNSIGNED_INT, 0);
	glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}
float Terrain::HeightAt(float x, float z)
{	
	float y = 0;
	int count = 0;
	float eps = 0.01f;
	for (int i = 0; i < u * v; i++)
		if (abs(abs(x) - abs(pos[i].x)) <= eps && abs(abs(z) - abs(pos[i].z)) <= eps)
		{
			y += pos[i].y;
			cout << "Found: " << pos[i].x << " " << y << " " << pos[i].z << endl;
			count++;
		}
	cout << "Count: " << count << endl;
	count = count == 0 ? 1 : count;
	return y / count;
}

void Terrain::InitElements()
{	
	this->pos = new vec3[u * v];
	this->normal = new vec3[u * v];
	this->tex = new vec2[u * v];
	
	this->IndicesCount1 = (u - 1)*(v - 1) * 6;
	this->indices = new unsigned int[IndicesCount1];
	this->hmap = new float *[this->u];
	for (int i = 0; i < this->u; i++)
		hmap[i] = new float[this->v];
}
void Terrain::LoadTerrain(const char *filename)
{   
	float height = 1.0f;
	int width, length;
	unsigned char* image = SOIL_load_image(filename, &width, &length, 0, SOIL_LOAD_RGB);

	this->u = width;
	this->v = length;
	InitElements();

	for(unsigned i = 0; i < this->u; i++)
    {
		for(unsigned j = 0; j < this->v; j++)
        {
			unsigned char color =
                (unsigned char)image[3 * (j * width + i)];
            hmap[i][j] = height * ((color / 255.0f) - 0.5f);
        }
    }
	SOIL_free_image_data(image);
	Smooth();
	InitSettings();
}
void Terrain::Smooth()
{
	float k = smoothKoeff;
	// Слева - направо
	for(unsigned i = 1; i < this->u; i++)
    {
		for(unsigned j = 0; j < this->v; j++)
		{
			hmap[i][j] = hmap[i-1][j] * (1-k) + 
			      hmap[i][j] * k;
		}
	}

	// Справа - налево
	for(unsigned i = u - 2; i > -1; i--)
    {
		for(unsigned j = 0; j < this->v; j++)
		{
			hmap[i][j] = hmap[i+1][j] * (1-k) + 
			      hmap[i][j] * k;
		}
	}

	// Снизу - вверх
	for(unsigned i = 0; i < this->u; i++)
    {
		for(unsigned j = 1; j < this->v; j++)
		{
			hmap[i][j] = hmap[i][j-1] * (1-k) + 
			      hmap[i][j] * k;
		}
	}

	// Сверху - вниз
	for(unsigned i = 0; i < this->u; i++)
    {
		for(unsigned j = v; j > -1; j--)
		{
			hmap[i][j] = hmap[i][j+1] * (1-k) + 
			      hmap[i][j] * k;
		}
	}
}