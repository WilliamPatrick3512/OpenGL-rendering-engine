#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
class Camera
{
	public:
		Camera(void);
		Camera(const glm::vec4& E, const glm::vec4& look, const glm::vec4& rel,
			float fov, float aspect, float N, float F);
		glm::vec4 eye(void) const { return eye_point; }
		glm::vec4 right(void) const { return right_vector; }
		glm::vec4 up(void) const { return up_vector; }
		glm::vec4 back(void) const { return back_vector; }
		glm::vec4 viewport(void) const;

		
		glm::mat4 matrix();

		Camera& zoom(float factor);
		Camera& forward(float distance);
		Camera& pan(float distance);
		Camera& lift(float distance);
		Camera& yaw(float angle);
		Camera& pitch(float angle);
		Camera& roll(float angle);

		glm::vec4 eye_point;

	private:
		glm::vec4 right_vector, up_vector, back_vector;
		float vp_width, vp_height, vp_distance,
			near_distance, far_distance;
};

glm::mat4 model(const Camera& cam);

