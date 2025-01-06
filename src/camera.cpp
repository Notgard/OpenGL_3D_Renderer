#include "camera.h"

#include "config.h"

Camera::Camera()
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->fov = 50.0f;
	this->aspectRatio = OGL_SCENE_WIDTH / OGL_SCENE_HEIGHT;
	this->near = 0.1f;
	this->far = 400.0f;

	this->model = glm::mat4(1.0f);
	update_view_matrix();
	update_projection_matrix();
}

void Camera::set_near(const float near)
{
	this->near = near;
	update_projection_matrix();
}

void Camera::set_far(const float far)
{
	this->far = far;
	update_projection_matrix();
}

void Camera::set_position(const glm::vec3 &position)
{
	this->position = position;
}

void Camera::set_aspect_ratio(const float aspectRatio)
{
	if (this->aspectRatio != aspectRatio)
	{
		std::cout << "Aspect ratio changed from " << this->aspectRatio << " to " << aspectRatio << std::endl;
		this->aspectRatio = aspectRatio;
		update_projection_matrix();
	}
	this->aspectRatio = aspectRatio;
}

void Camera::move(CameraMovement movement)
{
	std::cout << "Moving camera" << std::endl;
	switch (movement)
	{
	case CameraMovement::FORWARD:
		move_pos += get_forward();
		break;
	case CameraMovement::LEFT:
		move_pos -= get_right();
		break;
	case CameraMovement::BACKWARD:
		move_pos -= get_forward();
		break;
	case CameraMovement::RIGHT:
		move_pos += get_right();
		break;
	case CameraMovement::UP:
		move_pos += get_up();
		break;
	case CameraMovement::DOWN:
		move_pos -= get_up();
		break;
	}
	if (glm::length(move_pos) > 0.0f)
	{
		move_pos = glm::normalize(move_pos);
		this->position += move_pos * move_speed;
		std::cout << "Camera position: " << glm::to_string(this->position) << std::endl;
		update_view_matrix();
	}

	move_pos = glm::vec3(0.0f);
}

glm::vec2 Camera::get_mouse_delta(double x, double y)
{
	if (lastX == DEFAULT_MOUSE && lastY == DEFAULT_MOUSE)
	{
		lastX = x;
		lastY = y;
		// return the vector directly to avoid the first frame delta (spoiler: it's 0)
		return glm::vec2(0.0f, 0.0f);
	}

	double deltaX = x - lastX;
	double deltaY = lastY - y;

	lastX = x;
	lastY = y;

	return glm::vec2(deltaX, deltaY);
}

void Camera::set_pitch(float angle)
{
	orientation = glm::rotate(orientation, angle, direction_right);
}

void Camera::set_yaw(float angle)
{
	orientation = glm::rotate(orientation, angle, direction_up);
}

void Camera::set_roll(float angle)
{
	orientation = glm::rotate(orientation, angle, direction);
}

glm::mat4 Camera::get_translation_matrix()
{
	return glm::translate(glm::mat4(), -position);
}

glm::vec3 Camera::get_up() const
{
	return glm::rotate(get_direction(), up);
}

glm::vec3 Camera::get_right() const
{
	return glm::rotate(get_direction(), right);
}

glm::vec3 Camera::get_forward() const
{
	return glm::rotate(get_direction(), direction);
}

glm::quat Camera::get_direction() const
{
	return glm::quat(glm::vec3(-pitch, -yaw, 0.0f));
}

glm::mat4 Camera::get_view_matrix()
{
	if (update_view)
	{ // cache the view matrix
		glm::mat4 view;
		// TODO: Implement the view matrix
		// view = glm::toMat4(this->orientation) * this->get_translation_matrix();
		glm::vec3 focus{0.0f, 0.0f, 0.0f};
		this->position = focus - get_forward() * 5.0f;

		glm::quat r = get_direction();
		view = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(r);
		view = glm::inverse(view);

		this->view = view;
		// std::cout << "View matrix: " << glm::to_string(view) << std::endl;
	}
	return this->view;
}

void Camera::update_view_matrix()
{
	update_view = true;
	get_view_matrix();
	update_view = false;
}

glm::mat4 Camera::get_projection_matrix()
{
	if (update_projection)
	{ // cache the projection matrix
		glm::mat4 projection;
		// print perspective params
		std::cout << "FOV: " << fov << std::endl;
		std::cout << "Aspect ratio: " << aspectRatio << std::endl;
		std::cout << "Near: " << near << std::endl;
		std::cout << "Far: " << far << std::endl;

		auto glm_projection = glm::perspective(fov, this->aspectRatio, this->near, this->far);
		this->projection = glm_projection;
		// std::cout << "Projection matrix: " << glm::to_string(glm_projection) << std::endl;
	}
	return this->projection;
}

void Camera::update_projection_matrix()
{
	update_projection = true;
	this->projection = get_projection_matrix();
	update_projection = false;
}

void Camera::update(Shader *shader)
{
	shader->set_model(this->model);
	shader->set_view(get_view_matrix());
	shader->set_projection(get_projection_matrix());
	shader->set_vec3("cameraPosition", this->position);
}

void Camera::reset()
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Camera::on_mouse_wheel(double delta)
{
	this->fov -= delta;
	update_projection_matrix();
}

void Camera::on_mouse_move(double x, double y, int width, int height, MouseButtons button)
{
	glm::vec2 delta = get_mouse_delta(x, y);
	float delta_mag = glm::length(delta);
	if (delta_mag < 0.0f) // Only apply movement when delta is non-zero
	{
		return;
	}

	if (delta_mag > 100.0f) //ignore large deltas to avoid camera jumps
	{
		return;
	}

	if (button == MouseButtons::LEFT_CLICK)
	{
		yaw += delta.x / (float)width * rotation_speed;
		pitch -= delta.y / (float)height * rotation_speed;
		set_pitch(pitch);
		set_yaw(yaw);
		// this->rotation.x = this->rotation.x - y * 0.1;
		// this->rotation.y += x * 0.1;
		update_view_matrix();
	}
	else if (button == MouseButtons::RIGHT_CLICK)
	{
		// TODO
	}
	else if (button == MouseButtons::MIDDLE_CLICK)
	{
		// horizon tilting
	}
	this->view = get_view_matrix();
}