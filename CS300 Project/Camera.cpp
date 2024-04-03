#include "Camera.h"

// William Patrick
// Assignment 3
// cs250 Summer2019
//
#include "Camera.h"

Camera::Camera(void)
{
	near_distance = 0.1;
	far_distance = 10;
	eye_point = glm::vec4(0, 0, 0, 1);
	back_vector = glm::vec4(0, 0, 1, 0);

	right_vector = glm::vec4(1, 0, 0, 0);
	up_vector = glm::vec4(0, 1, 0, 0);

	vp_width = 2 * .1 * tan(90 * (3.141592 / 180));
	vp_height = vp_width;
	vp_distance = 0.1;


}
Camera::Camera(const glm::vec4& E, const glm::vec4& look, const glm::vec4& rel,
	float fov, float aspect, float N, float F)
{
	near_distance = N;
	far_distance = F;
	eye_point = E;
	back_vector = -look;

	right_vector = glm::vec4(glm::cross(glm::vec3(look), glm::vec3(rel)),0.0f);
	up_vector = glm::vec4(glm::cross(glm::vec3(right_vector), glm::vec3(look)), 0.0f);

	vp_width = 2 * N * tan(fov * (3.141592 / 180));
	vp_height = vp_width / aspect;
	vp_distance = N;

}
glm::vec4 Camera::viewport(void) const
{
	return glm::vec4(vp_width, vp_height, vp_distance, 0.0f);
}

Camera& Camera::zoom(float factor)
{
	vp_width *= factor;
	vp_height *= factor;
	return *this;
}

Camera& Camera::forward(float distance)
{
	eye_point = glm::translate(distance * (glm::vec3(glm::normalize(back_vector)))) * eye_point;
	return *this;
}

Camera& Camera::pan(float distance)
{
	eye_point = glm::translate(distance * (glm::vec3(glm::normalize(right_vector)))) * eye_point;
	return *this;
}

Camera& Camera::lift(float distance)
{
	eye_point = glm::translate(distance * (glm::vec3(glm::normalize(up_vector)))) * eye_point;
	return *this;
}


Camera& Camera::yaw(float angle)
{
	glm::mat4 old = model(*this);
	glm::mat4 old_inverse = glm::inverse(old);

	right_vector = old_inverse * right_vector;
	up_vector = old_inverse * up_vector;
	back_vector = old_inverse * back_vector;
	eye_point = old_inverse * eye_point;

	glm::mat4 new_model =  glm::rotate(glm::radians(angle), glm::vec3(up_vector)) * old ;
	eye_point = new_model * eye_point;

	right_vector = new_model * right_vector;
	up_vector = new_model * up_vector;
	back_vector = new_model * back_vector;
	return *this;
}

Camera& Camera::pitch(float angle)
{
	glm::mat4 old = model(*this);
	glm::mat4 old_inverse = glm::inverse(old);

	right_vector = old_inverse * right_vector;
	up_vector = old_inverse * up_vector;
	back_vector = old_inverse * back_vector;
	eye_point = old_inverse * eye_point;


	glm::mat4 new_model = glm::rotate(glm::radians(angle), glm::vec3(right_vector)) * old;
	eye_point = new_model * eye_point;

	right_vector = new_model * right_vector;
	up_vector = new_model * up_vector;
	back_vector = new_model * back_vector;
	return *this;
}

Camera& Camera::roll(float angle)
{
	glm::mat4 old = model(*this);
	glm::mat4 old_inverse = glm::inverse(old);

	right_vector = old_inverse * right_vector;
	up_vector = old_inverse * up_vector;
	back_vector = old_inverse * back_vector;
	eye_point = old_inverse * eye_point;


	glm::mat4 new_model = glm::rotate(glm::radians(angle), glm::vec3(back_vector)) * old;

	eye_point = new_model * eye_point;

	right_vector = new_model * right_vector;
	up_vector = new_model * up_vector;
	back_vector = new_model * back_vector;
	return *this;
}

glm::mat4 model(const Camera& cam)
{
	glm::mat4 A(0);
	A[0][0] = cam.right().x;   A[1][0] = cam.up().x; A[2][0] = cam.back().x; A[3][0] = cam.eye().x;
	A[0][1] = cam.right().y;   A[1][1] = cam.up().y; A[2][1] = cam.back().y; A[3][1] = cam.eye().y;
	A[0][2] = cam.right().z;   A[1][2] = cam.up().z; A[2][2] = cam.back().z; A[3][2] = cam.eye().z;
	A[0][3] = 0;      A[1][3] = 0;    A[2][3] = 0;    A[3][3] = 1;
	return A;
}

