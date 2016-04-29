#include <glfw3.h>
#include <fwd.hpp>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stdio.h>

using namespace glm;

class Camera
{
public:
	Camera();
	~Camera();
	vec3 getNormal();
	vec3 getRight();
	mat4 getViewMatrix();
	vec3 getDirection();
	mat4 getProjectionMatrix();
	vec3 getCameraPosition();
	vec3 getFront();
	void Use();
	void setCameraPosition(vec3 value);
	void setCameraDirection(vec3 value);
private:	
	float horizontalAngle;
	float verticalAngle;
	float initialFoV;
	float speed;
	float mouseSpeed;

	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
	vec3 direction;
	vec3 right;
	vec3 position; 
	vec3 Front;
};