#pragma once
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Skybox
{
private:
	GLuint skyVAO;
	GLuint skyVBO;
	GLuint skyEBO;
	GLuint skyTex;
	float edge;
	int number;
	vector<float> vertices;
	vector<int> indices;
public:
	GLuint skyProg;
	Skybox();
	~Skybox();
	void Init(const string vs_name, const string fs_name, int num, const char* textfilename);
	void CalData(int num);
	void BindBuffer();
	void GenTexture(const char* textfilename);
	void SetUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
	void Display(GLFWwindow* window, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
};

