#include "Camera.h"
extern GLFWwindow* window;
extern const int SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;

Camera::Camera()
{
	horizontalAngle = 3.14f;
	verticalAngle = 0.0f;
	initialFoV = 45.0f;
	speed = 1.5f;
	mouseSpeed = 0.005f;

	direction = vec3(0.0f, 0.0f, 0.0f);
	right = vec3(0.0f, 0.0f, 0.0f);
	position = vec3( 0, 0, 5 ); 
	Front = vec3(0.0f, 0.0f, -1.0f);
}

vec3 Camera::getNormal(){
	vec3 Z = normalize(position - direction);
	return normalize(cross(vec3(0.0, 1.0, 0.0), Z));
}
vec3 Camera::getDirection()
{
	return direction;
}
void Camera::setCameraPosition(vec3 value)
{
	position = value;
}
void Camera::setCameraDirection(vec3 value)
{
	direction = value;
}
vec3 Camera::getRight(){
	return right;
}
mat4 Camera::getViewMatrix(){
	return ViewMatrix;
}
mat4 Camera::getProjectionMatrix(){
	return ProjectionMatrix;
}
vec3 Camera::getCameraPosition(){
	return position;
}
vec3 Camera::getFront(){
	return Front;
}

void Camera::Use(){
	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	glfwSetCursorPos(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

	horizontalAngle += mouseSpeed * float(SCREEN_WIDTH/2 - xpos );
	verticalAngle   += mouseSpeed * float(SCREEN_HEIGHT/2 - ypos );

	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	Front = normalize(direction);

	right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	glm::vec3 up = glm::cross( right, direction );

	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	if (glfwGetKey( window, GLFW_KEY_S) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}
	if (glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS){
		position += up * deltaTime * speed;
	}
	if (glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS){
		position -= up * deltaTime * speed;
	}
	if (glfwGetKey( window, GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS){
		speed += 2;
	}
	else
	{
		speed = 3;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel();

	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 1000.0f);
	ViewMatrix = glm::lookAt(position, position+direction, up);

	lastTime = currentTime;
}

Camera::~Camera()
{
}