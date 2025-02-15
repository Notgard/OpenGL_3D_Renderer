#include "camera.h"

#include "config.h"

Camera::Camera()
{
	this->fov = glm::radians(51.0f);
	this->aspectRatio = OGL_SCENE_WIDTH / OGL_SCENE_HEIGHT;
	this->near = 0.1f;
	this->far = 200.0f;

	//this->model = glm::mat4(1.0f);
	update_view_matrix();
	update_projection_matrix();
}

void Camera::print_camera()
{
	std::cout << "Camera position: " << glm::to_string(this->position) << std::endl;
	std::cout << "Camera up: " << glm::to_string(this->up) << std::endl;
	std::cout << "Camera right: " << glm::to_string(this->right) << std::endl;
	std::cout << "Camera direction: " << glm::to_string(this->direction) << std::endl;

	std::cout << "Camera pitch: " << this->pitch << std::endl;
	std::cout << "Camera yaw: " << this->yaw << std::endl;

	std::cout << "Camera fov: " << this->fov << std::endl;
	std::cout << "Camera aspect ratio: " << this->aspectRatio << std::endl;
	std::cout << "Camera near: " << this->near << std::endl;
	std::cout << "Camera far: " << this->far << std::endl;

	std::cout << "Camera orientation: " << glm::to_string(this->orientation) << std::endl;
}

void Camera::switch_camera_mode()
{
	free_camera = !free_camera;
	std::string str = free_camera ? "on" : "off";
	std::cout << "Free camera mode: " << str << std::endl;
	if (!free_camera)
	{
		yaw = 0.0f;
		pitch = 0.0f;
		position = {0.0f, 0.0f, 0.0f};
	}
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
		//std::cout << "Aspect ratio changed from " << this->aspectRatio << " to " << aspectRatio << std::endl;
		this->aspectRatio = aspectRatio;
		update_projection_matrix();
	}
	this->aspectRatio = aspectRatio;
}

void Camera::move(CameraMovement movement, float velocity)
{
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
	//std::cout << "Moving camera" << std::endl;
	if (glm::length(move_pos) > 0.0f)
	{
		move_pos = glm::normalize(move_pos);
		this->position += move_pos * velocity;
		//std::cout << "Camera position: " << glm::to_string(this->position) << std::endl;
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

glm::mat4 Camera::get_rotation_matrix()
{
	return glm::toMat4(get_direction());
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
		if (free_camera)
		{
			// Compute the forward vector
			glm::vec3 forward = get_forward();
			// Compute the target position (where the camera is looking)
			glm::vec3 target = this->position + forward;
			// Get the correct up vector
			glm::vec3 up = get_up();
			// Use glm::lookAt to compute the view matrix
			this->view = glm::lookAt(this->position, target, up);
		}
		else
		{
			glm::vec3 focus{0.0f, 0.0f, 0.0f};
			this->position = focus - get_forward() * 5.0f;

			glm::quat r = get_direction();
			view = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(r);
			view = glm::inverse(view);

			this->view = view;
		}
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
		//std::cout << "FOV: " << fov << std::endl;
		//std::cout << "Aspect ratio: " << aspectRatio << std::endl;
		//std::cout << "Near: " << near << std::endl;
		//std::cout << "Far: " << far << std::endl;

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
	//shader->set_model(this->model);
	shader->set_view(get_view_matrix());
	shader->set_projection(get_projection_matrix());
	shader->set_vec3("cameraPosition", this->position);
}

void Camera::reset()
{
	this->position = init_position;
	this->pitch = init_pitch;
	this->yaw = init_yaw;
	this->update_view_matrix();
}

void Camera::on_mouse_wheel(double delta)
{
	this->fov -= delta * zoom_speed;

	// clamp the fov with glm
	this->fov = glm::clamp(this->fov, glm::radians(1.0f), glm::radians(90.0f));

	update_projection_matrix();
}

void Camera::on_mouse_move(double x, double y, int width, int height, MouseButtons button)
{
	glm::vec2 delta = get_mouse_delta(x, y);
	float delta_mag = glm::length(delta);
	//std::cout << "Delta magnitude: " << delta_mag << std::endl;
	if (delta_mag < 0.0f) // Only apply movement when delta is non-zero
	{
		return;
	}

	if (delta_mag > 100.0f) // ignore large deltas to avoid camera jumps
	{
		return;
	}

	if (button == MouseButtons::LEFT_CLICK)
	{
		yaw += delta.x / (float)width * rotation_speed;
		pitch -= delta.y / (float)height * rotation_speed;

		pitch = glm::clamp(pitch, -89.0f, 89.0f);

		set_pitch(pitch);
		set_yaw(yaw);

		update_view_matrix();
	}
	else if (button == MouseButtons::RIGHT_CLICK)
	{
		// TODO: translate the mesh in the scene
		float sensitivity = 0.01f;
		glm::vec3 translation(delta.x * sensitivity, -delta.y * sensitivity, 0.0f);
        //selectedObject->translate(translation);
	}
	else if (button == MouseButtons::MIDDLE_CLICK)
	{
		// translate the mesh in the scene
		glm::vec3 right = get_right();
		glm::vec3 up = get_up();

		// Scale movement based on screen size for consistency
		float scale = move_speed * 0.005f;

		// Move along right and up vectors based on mouse delta
		this->position -= right * delta.x * scale;
		this->position += up * delta.y * scale;

		update_view_matrix();
	}
	this->view = get_view_matrix();
}

void Camera::load_config(Configuration *config)
{
	this->fov = glm::clamp(glm::radians(config->camera_fov), glm::radians(1.0f), glm::radians(90.0f));
	this->near = config->camera_near;
	this->far = config->camera_far;
	//this->position = config->camera_position;
	update_projection_matrix();
	this->free_camera = config->free_camera;
	this->move_speed = config->camera_move_speed;
	update_view_matrix();
}