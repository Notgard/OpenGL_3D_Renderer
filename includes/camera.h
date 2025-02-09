#pragma once

#include "shader.h"

#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "config.h"

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
	glm::mat4 get_rotation_matrix();

	void update_view_matrix();
	void update_projection_matrix();

	glm::vec3 get_position() const { return this->position; };
	void set_position(const glm::vec3 &position);
	
	void set_near(const float near);
	float get_near() const { return this->near; }

	void set_far(const float far);
	float get_far() const { return this->far; }

	float get_fov() const { return this->fov; }

	void set_aspect_ratio(const float aspectRatio);

	void move(CameraMovement movement, float velocity);
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

	void print_camera();
	void switch_camera_mode();

	float get_move_speed() const { return move_speed; }
	bool is_free_camera() const { return free_camera; }

	void load_config(Configuration *config);

private:
	glm::vec2 get_mouse_delta(double x, double y);

	glm::vec3 init_position{-6.4f, 5.3f, -6.0f};
	glm::vec3 position = init_position;

	glm::vec3 target{0.0f, 0.0f, 0.0f}; // target center
	glm::vec3 up{0.0f, 1.0f, 0.0f};
	glm::vec3 right{ 1.0f, 0.0f, 0.0f };
	glm::vec3 direction{0.0f, 0.0f, -11.0f}; // always looking at Z axis 
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

	float init_pitch = 0.3f;
	float pitch = init_pitch;
	float init_yaw = -4.0f;
	float yaw = init_yaw;
	float roll = 0.0f;

	float rotation_speed = 3.0f;
	float move_speed = 3.5f;
	float zoom_speed = 0.1f;

	float mouse_sensitivity = 0.1f;

	bool update_view = false;
	bool update_projection = false;
	bool free_camera = true;

	// mouse inputs
	double lastX = DEFAULT_MOUSE;
	double lastY = DEFAULT_MOUSE;
};
