#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"

class Transform {
public:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	Shader* shaderProgram;


	void SetPosition(glm::vec3 NewPos) {
		unsigned int transformLoc = glGetUniformLocation(shaderProgram->ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	}
};