#pragma once

#include "shader.h"

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

enum class CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum class MouseButtons
{
	LEFT_CLICK,
	RIGHT_CLICK,
	MIDDLE_CLICK
};

#define DEFAULT_MOUSE -9999.0

class Camera
{
public:
	Camera();
	glm::mat4 get_view_matrix();
	glm::mat4 get_projection_matrix();
	glm::mat4 get_translation_matrix();

	void update_view_matrix();
	void update_projection_matrix();

	void set_position(const glm::vec3 &position);
	void set_near(const float near);
	void set_far(const float far);
	void set_aspect_ratio(const float aspectRatio);
	void move(CameraMovement movement);
	void update(Shader *shader);
	void reset();

	void set_pitch(float angle);
	void set_yaw(float angle);
	void set_roll(float angle);

	glm::vec3 get_up() const;

	glm::vec3 get_right() const;

	glm::vec3 get_forward() const;

	glm::quat get_direction() const;

	// event handlers
	void on_mouse_wheel(double delta);
	void on_mouse_move(double x, double y, int width, int height, MouseButtons button);

private:
	glm::vec2 get_mouse_delta(double x, double y);

	glm::vec3 position{0.0f, 0.0f, 0.0f};

	glm::vec3 up{0.0f, 1.0f, 0.0f};
	glm::vec3 right{ 1.0f, 0.0f, 0.0f };
	glm::vec3 direction{0.0f, 0.0f, -1.0f}; // defaults to looking down the z axis in OpenGL
	glm::vec3 direction_right = glm::normalize(glm::cross(direction, up)); //strafe vector
	glm::vec3 direction_up = glm::normalize(glm::cross(direction_right, direction));

	glm::vec3 move_pos{0.0f, 0.0f, 0.0f};

	glm::mat4 orientation;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	float fov;
	float aspectRatio;
	float near;
	float far;

	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	float rotation_speed = 4.0;
	float move_speed = 3.0;

	bool update_view = false;
	bool update_projection = false;

	// mouse inputs
	double lastX = DEFAULT_MOUSE;
	double lastY = DEFAULT_MOUSE;
};
