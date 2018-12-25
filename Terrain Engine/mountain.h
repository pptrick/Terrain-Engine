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

class mountain
{
private:
	GLuint mountVAO;
	GLuint mountVBO;
	GLuint mountEBO;
	GLuint mountTex;
	GLuint rockTex;
	int heightmap_cols;
	int heightmap_rows;
	cv::Mat heightmap;
	vector<vector<float>> height;
	vector<float> Points;
	vector<int> indices;

public:
	GLuint mountProg;
	mountain();
	~mountain();
	void Init(const string vs_name, const string fs_name, const char* heightmap_filename, const char* textfilename, const char* detailfilename);
	void LoadData(const char* heightmap_filename);
	void BindBuffer();
	void GenTexture(const char* textfilename, const char* detailfilename);
	void SetUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
	void Display(GLFWwindow* window, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
	void FindSilhouette();
};

struct slet
{
	float x;
	float y;
	float z;
	int num;
	bool visited;
	slet()
	{
		visited = 0;
	}
	slet(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
		visited = 0;
	}
};