#pragma once
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <opencv2/opencv.hpp>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mountain.h"

using namespace std;

class Water
{
private:
	GLuint waterVAO;
	GLuint waterVBO;
	GLuint waterEBO;
	GLuint waveVAO;
	GLuint waveVBO;
	GLuint waveEBO;
	GLuint waterTex;
	vector<float> vertices;
	vector<int> indices;
	vector<float> WavePoints;
	vector<int> WaveIndices;
public:
	GLuint waterProg;
	GLuint waveProg;
	Water();
	~Water();
	void Init(const string vs_name, const string fs_name, const char* textfilename);
	void Wave_Init(const string vs_name, const string fs_name, const char* textfilename);
	void CalData();
	void BindBuffer();
	void Wave_BindBuffer();
	void GenTexture(const char* textfilename);
	void SetUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
	void Display(GLFWwindow* window, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
	void WaveDisplay(GLFWwindow* window);
	void WaveDynamic();
	void Cal_Silhouette();
	void LoadSilhouette(slet v0, slet v1);
};
