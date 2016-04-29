#include <glew.h>
#include <glfw3.h>
#include <fwd.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iostream>
#include <SOIL.h>
#include "Utils.h"
#include "Camera.h"
#include "terrain.h"
#include "SkyBox.h"
#include "Model.h"
#include "Water.h"
#include "Text.h"
#include <irrKlang.h>
using namespace glm;
using namespace std;
using namespace irrklang;

#pragma comment(lib, "irrKlang.lib")

GLFWwindow* window;
const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
Camera camera;	
bool drawObjects = true;
bool drawWater = true;
bool drawTerrain = true;
bool drawSkyBox = true;
bool waterGeometry = false;
bool sound = true;
bool showMenu = true;
float waterSpeed = 0.01f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
int main(int argc, char** argv)
{
	if (!glfwInit())
	{
		cout << "Failed to initialize GLFW" << endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Island", NULL, NULL);
	if( window == NULL ){
		cout << "Failed to open GLFW window.\n" << endl;
		glfwTerminate();
		return -1;
	}

	glfwSetWindowPos(window, 270, 90);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) {
		cout << "Failed to initialize GLEW\n" << endl;
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetKeyCallback(window, key_callback);

	glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	glClearColor(.2, .3, .3, 1.0);

	camera.setCameraPosition(vec3(-64.0, 23.0, -25.0));
	Terrain terrain(205.5f);
	float worldParameters = 350;
	Water water(worldParameters * 2, worldParameters * 2);
	SkyBox cubemap(worldParameters);
	vec3 waterPosition = vec3(-worldParameters, -13, -worldParameters);
	
	Text text(SCREEN_WIDTH, SCREEN_HEIGHT);
	ivec2 boxSize = ivec2(-500, 350);

	GLuint program = LoadShaders("Shaders/Objects/vs.vert", "Shaders/Objects/fs.frag");
	GLuint mvp = glGetUniformLocation(program, "MVP");
	cout << "Load objects:" << endl;
	cout << "========================================================================================" << endl;
	Model tent1 ("Data/Objects/Tent_1/Tent_1.obj", program);
	Model tent ("Data/Objects/Tent/tent.obj", program);
	Model barrel ("Data/Objects/Barrel/Barrel.obj", program);
	Model cannon ("Data/Objects/Cannon/cannon_mesh.obj", program);
	Model house ("Data/Objects/Farmhouse/Farmhouse OBJ.obj", program);
	Model ship ("Data/Objects/Ship/ship3.obj", program);
	Model tree ("Data/Objects/Tree/needle01.obj", program);
	Model lamp ("Data/Objects/Lamp/Petroleum_Lamp.obj", program);
	GLuint cubeVao = LoadCube(program);
	cout << "========================================================================================" << endl;
	mat4 scaleObject[] = {
		scale(mat4(1.0f), vec3(70.15f)), //ship
		scale(mat4(1.0f), vec3(1.3f)),   //cannon
		scale(mat4(1.0f), vec3(0.20f)),  //house
		scale(mat4(1.0f), vec3(0.65f)),  //tent1
		scale(mat4(1.0f), vec3(3.5f)),   //tent
		scale(mat4(1.0f), vec3(0.15f)),  //barrel
		scale(mat4(1.0f), vec3(5.0f)),   //tree
		scale(mat4(1.0f), vec3(7.0f)),   //tree
		scale(mat4(1.0f), vec3(1.0f)),   //lamp
		scale(mat4(1.0f), vec3(1.2f))    //cube
	};

	vec3 positionObject[] = {
		vec3(-62.3381, -11.5, -104.831),	 //ship
		
		vec3(-35.164, -6.9, 44.328),		 //cannon
		vec3(-35.4699, -6.9, 39.8163),
		vec3(-35.7412, -6.35, 35.7012),
		vec3(-36.0708, -5.87, 30.8904),
		vec3(-36.3983, -5.87, 26.4434),
		vec3(-36.7953, -6.16, 22.3239),
		vec3(-37.1787, -6.10, 18.3257),

		vec3(-8.21483, -0.991692, 42.3217),	 //house
		vec3(-8.46408, -8.4955, -34.2052),
		vec3(12.2195, -10.00723, -34.7814),

		vec3(-28.41, -4.85463, 3.3742),   //tent1
		vec3(-29.1099, -6.5071, 58.6885),   //tent  

		vec3(-35.164, -6.9, 43.328),		 //barrel
		vec3(-35.4699, -6.9, 38.8163),
		vec3(-35.7412, -6.35, 34.7012),
		vec3(-36.0708, -5.87, 29.8904),
		vec3(-36.3983, -5.87, 25.4434),
		vec3(-36.7953, -6.16, 21.3239),
		vec3(-37.1787, -6.10, 17.3257),

		vec3(-5.6854, -9.92242, 68.0452),  //tree
		vec3(39.747, -9.95482, 56.9772),
		vec3(72.9695, -9.95482, 59.22),
		vec3(69.9275, -12.0354, 27.3984),
		vec3(44.2057, -11.082, 14.3814),

		vec3(-37.9309, -3.65, 8.99188), //cube
		vec3(-24.4361, -8.0, 35.8615),
		vec3(-24.4361, -6.8, 35.0105),
		vec3(-24.5105, -8.0, 33.9723),
		vec3(-10.0298, -8.90054, -39.4486),
		vec3(17.2729, -10.07074, -35.8718),
		vec3(-7.13707, -0.4, 46.5167),
		vec3(-24.4361, -8.0, 35.0105)
	};
	
	vec3 pointLightPositions[] = {
		vec3(-24.5585, -7.4, 33.7913),
		vec3(-24.4391, -7.4, 36.2676),
		vec3(-7.06688, 0.2, 46.5081),
		vec3(-29.1083, -5.84039, 58.6775),
		vec3(-37.9507, -3.1441, 9.01717),
		vec3(-10.2281, -8.34472, -39.3235),
		vec3(17.4582, -9.44262, -35.7065),
		vec3(-35.48, -6.15831, 38.8325)
    };


	GLuint reflectionFrameBuffer = 0;
	glGenFramebuffers(1, &reflectionFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);
	
	GLuint reflectionTexture = LoadFrameBufferTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	#pragma region cube texture
	GLuint diffuseMap, specularMap;
    glGenTextures(1, &diffuseMap);
    glGenTextures(1, &specularMap);
    int width, height;
    unsigned char* image;
    
    image = SOIL_load_image("Data/Cube/container2.png", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    
    image = SOIL_load_image("Data/Cube/container2_specular.png", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "mdiffuse1"),  0);
	glUniform1i(glGetUniformLocation(program, "mspecular1"), 1);
	#pragma endregion

	ISoundEngine* engine = createIrrKlangDevice();
	if (!engine)
	{
		cout << "Sound library initialization error!" << endl;
		return 0;
	}
	string txt[] = 
	{
		"1 - objects(on/off)",
		"2 - terrain(on/off)",
		"3 - water(on/off)",
		"4 - skybox(on/off)",
		"5 - water geometry(on/off)",
		"6 - sound(on/off)",
		"+/- - change water speed"
	};
	mat4 mWorld = mat4(1.0f);
	mat4 mView = mat4(1.0f);
	mat4 mProjection = mat4(1.0f);

	double time = 0.0;
	double timeDiff = 0.0;
	time = glfwGetTime();
	string FPS;

	std::stringstream sstm;	
	bool soundStart = false;

	do
	{
		timeDiff = glfwGetTime() - time;
		timeDiff = 1 / timeDiff;
		sstm.str(std::string());		
		sstm << "FPS: " << timeDiff;
		FPS = sstm.str();
		sstm.str(std::string());		
		if (sound && !soundStart)
		{
			cout << "Sound on!" << endl;
			engine->play2D("Data/Sound/geldern_night.mp3", true);
			engine->play2D("Data/Sound/3D_River.mp3", true);
			soundStart = true;
		}
		if (!sound && soundStart)
		{
			cout << "Sound off!" << endl;
			engine->setAllSoundsPaused(true);
			soundStart = false;
		}

		camera.Use();
		mProjection = camera.getProjectionMatrix();

		#pragma region reflection
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, reflectionFrameBuffer);
		mWorld = mat4(1.0f);
		mView = lookAt(vec3(-129.523, 624.225, -18.263), vec3(-129.246, 623.264, -18.2517), glm::vec3(0.0, 1.0, 0.0));
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);
		cubemap.Draw(mProjection, mView, mWorld);	
		glDisable(GL_CULL_FACE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		#pragma endregion

		mView = camera.getViewMatrix();
		mWorld = mat4(1.0f);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		if (drawSkyBox)
			cubemap.Draw(mProjection, mView, mWorld);	
			
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		if (drawObjects)
		{
			
			#pragma region cubes
			glUseProgram(program);
			glUniform3f(glGetUniformLocation(program, "viewPos"), camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);
			glUniform1f(glGetUniformLocation(program, "shininess"), 32.0f);
			glUniform1i(glGetUniformLocation(program, "cube"), true);

			glUniform3f(glGetUniformLocation(program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
			glUniform3f(glGetUniformLocation(program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
			glUniform3f(glGetUniformLocation(program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
			glUniform3f(glGetUniformLocation(program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

			for (int i = 0; i < 8; i++)
			{
				sstm.str(std::string());
				sstm << "pointLights[" << i << "].position";
				glUniform3f(glGetUniformLocation(program, sstm.str().c_str()), pointLightPositions[i].x, pointLightPositions[i].y + 0.6f, pointLightPositions[i].z);
				sstm.str(std::string());
				sstm << "pointLights[" << i << "].ambient";
				glUniform3f(glGetUniformLocation(program, sstm.str().c_str()), 0.05f, 0.05f, 0.05f);
				sstm.str(std::string());
				sstm << "pointLights[" << i << "].diffuse";
				glUniform3f(glGetUniformLocation(program, sstm.str().c_str()), 0.8f, 0.8f, 0.8f);
				sstm.str(std::string());
				sstm << "pointLights[" << i << "].specular";
				glUniform3f(glGetUniformLocation(program, sstm.str().c_str()), 1.0f, 1.0f, 1.0f);
				sstm.str(std::string());
				sstm << "pointLights[" << i << "].constant";
				glUniform1f(glGetUniformLocation(program, sstm.str().c_str()), 1.0f);
				sstm.str(std::string());
				sstm << "pointLights[" << i << "].linear";
				glUniform1f(glGetUniformLocation(program, sstm.str().c_str()), 0.09);
				sstm.str(std::string());
				sstm << "pointLights[" << i << "].quadratic";
				glUniform1f(glGetUniformLocation(program, sstm.str().c_str()), 0.032);
			}

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);		
			glBindVertexArray(cubeVao);
			mat4 pv = mProjection * mView;
			for (int i = 25; i < 35; i++)
			{
				mWorld = translate(mat4(1.0f), positionObject[i]);
				glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(mWorld));
				glUniformMatrix3fv(glGetUniformLocation(program,  "TIM"), 1, GL_FALSE, value_ptr(mat3(transpose(inverse(mWorld)))));
				glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(pv * mWorld * scaleObject[9]));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, 0);		
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			#pragma endregion 
			glUniform1i(glGetUniformLocation(program, "cube"), false);
			mWorld = mat4(1.0f);
			mWorld = translate(mWorld, positionObject[0]);
			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(mWorld));
			glUniformMatrix3fv(glGetUniformLocation(program,  "TIM"), 1, GL_FALSE, value_ptr(mat3(transpose(inverse(mWorld)))));
			glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(pv * mWorld * scaleObject[0]));
			ship.Draw(program);
		
			mWorld = mat4(1.0f);
			for(int i = 1; i < 8; i++)
			{
				mWorld = translate(mWorld, positionObject[i]);
				mWorld = rotate(mWorld, 200.0f, vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(mWorld));
				glUniformMatrix3fv(glGetUniformLocation(program,  "TIM"), 1, GL_FALSE, value_ptr(mat3(transpose(inverse(mWorld)))));
				glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(pv * mWorld * scaleObject[1]));
				cannon.Draw(program);
				mWorld = mat4(1.0f);
			}	
			mWorld = mat4(1.0f);
			mWorld = translate(mWorld, positionObject[8]);
			mWorld = rotate(mWorld, 210.0f, vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(mWorld));
			glUniformMatrix3fv(glGetUniformLocation(program,  "TIM"), 1, GL_FALSE, value_ptr(mat3(transpose(inverse(mWorld)))));
			glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(pv * mWorld * scaleObject[2]));
			house.Draw(program);
			mWorld = mat4(1.0f);
			for (int i = 9; i < 11; i++)
			{
				mWorld = translate(mWorld, positionObject[i]);
				mWorld = rotate(mWorld, 5.0f * i, vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(mWorld));
				glUniformMatrix3fv(glGetUniformLocation(program,  "TIM"), 1, GL_FALSE, value_ptr(mat3(transpose(inverse(mWorld)))));
				glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(pv * mWorld * scaleObject[2]));
				house.Draw(program);
				mWorld = mat4(1.0f);
			}

			mWorld = mat4(1.0f);
			mWorld = translate(mWorld, positionObject[11]);
			mWorld = rotate(mWorld, -45.0f, vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(mWorld));
			glUniformMatrix3fv(glGetUniformLocation(program,  "TIM"), 1, GL_FALSE, value_ptr(mat3(transpose(inverse(mWorld)))));
			glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(pv * mWorld * scaleObject[3]));
			tent1.Draw(program);

			mWorld = mat4(1.0f);
			mWorld = translate(mWorld, positionObject[12]);
			mWorld = rotate(mWorld, 180.0f, vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(mWorld));
			glUniformMatrix3fv(glGetUniformLocation(program,  "TIM"), 1, GL_FALSE, value_ptr(mat3(transpose(inverse(mWorld)))));
			glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(pv * mWorld * scaleObject[4]));
			tent.Draw(program);

			for (int i = 12; i < 20; i++)
			{
				mWorld = mat4(1.0f);
				mWorld = translate(mWorld, positionObject[i]);
				mWorld = rotate(mWorld, 180.0f, vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(mWorld));
				glUniformMatrix3fv(glGetUniformLocation(program,  "TIM"), 1, GL_FALSE, value_ptr(mat3(transpose(inverse(mWorld)))));
				glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(pv * mWorld * scaleObject[5]));
				barrel.Draw(program);
			}
			for (int i = 20; i < 25; i++)
			{
				mWorld = mat4(1.0f);
				mWorld = translate(mWorld, positionObject[i]);
				mWorld = rotate(mWorld, 180.0f, vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(mWorld));
				glUniformMatrix3fv(glGetUniformLocation(program,  "TIM"), 1, GL_FALSE, value_ptr(mat3(transpose(inverse(mWorld)))));
				glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(pv * mWorld * (i > 20 ? scaleObject[7] : scaleObject[6])));
				tree.Draw(program);
			}
			for (int i = 0; i < 8; i++)
			{
				mWorld = mat4(1.0f);
				mWorld = translate(mWorld, pointLightPositions[i]);
				glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, value_ptr(mWorld));
				glUniformMatrix3fv(glGetUniformLocation(program,  "TIM"), 1, GL_FALSE, value_ptr(mat3(transpose(inverse(mWorld)))));
				glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_FALSE, value_ptr(pv * mWorld * scaleObject[8]));
				lamp.Draw(program);
			}
		}
		mWorld = mat4(1.0f);
		glEnable(GL_CULL_FACE);
		if (drawTerrain)
			terrain.RenderTerrain(mProjection, mView, mWorld);
		glDisable(GL_CULL_FACE);
		if (drawWater)
		{
			mWorld = translate(mWorld, waterPosition);
			water.Draw(mProjection, mView, mWorld, reflectionTexture, cubemap.getTextureId(), camera.getCameraPosition(), waterGeometry, waterSpeed);
		}
		glDisable(GL_DEPTH_TEST);
		time = glfwGetTime();
		if (showMenu)
			text.RenderText(txt, FPS, boxSize, 25.0f, .3f, 7);

        glfwPollEvents();
		glfwSwapBuffers(window);
	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
	glfwTerminate();
	engine->drop();
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		drawObjects = !drawObjects;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		drawTerrain = !drawTerrain;
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		drawWater = !drawWater;
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		drawSkyBox = !drawSkyBox;
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		waterGeometry = !waterGeometry;
	if (glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS)
	{
		vec3 r = vec3(camera.getCameraPosition() + camera.getDirection());
		cout << "vec3(" << camera.getCameraPosition().x 
			<< ", " << camera.getCameraPosition().y 
			<< ", " << camera.getCameraPosition().z 
			<< "), vec3(" << r.x << ", " << r.y << ", " << r.z << ") " << endl;
	}
	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
	{
		waterSpeed += waterSpeed >= 0.09 ? 0.0f : 0.01f;
	}
	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
	{
		waterSpeed -= waterSpeed <= 0.01f ? 0.0f : 0.01f;
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
		sound = !sound;
	}
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
	{
		showMenu = !showMenu;
	}
}