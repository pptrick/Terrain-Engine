#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>
#include "mountain.h"
#include "Sky.h"
#include "water.h"
using namespace std;

#define Height 600
#define Width 600

double xpos, ypos;
double viewx = 400;
double viewy = 0;
double viewz = 0;
float nearz =1.0f;
float farz = 20000.0f;
bool closeloop = 0;

glm::mat4 Model = glm::mat4(1.0);
glm::mat4 View = glm::mat4(1.0);
glm::mat4 Projection = glm::mat4(1.0);
glm::vec3 center = glm::vec3(0, 0, 400);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		center.x -= 15.f;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		center.x += 15.f;
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		center.y -= 15.f;
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		center.y += 15.f;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		center.z += 15.f;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		center.z -= 15.f;
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		closeloop = 1;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(Width, Height, "Terrain Engine", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = GL_TRUE;
	glewInit();

	cout << "OpenGL version:" << glGetString(GL_VERSION) << endl;

	glfwSetKeyCallback(window, key_callback);
	glViewport(0, 0, Width, Height);

	mountain mount;
	mount.Init("../glsl/vs_mount.glsl", "../glsl/fs_mount.glsl", "../data/heightmap.bmp","../data/terrain-texture3.bmp","../data/detail.png");
	Skybox box1;
	box1.Init("../glsl/vs_sky.glsl", "../glsl/fs_sky.glsl", 1, "../data/SkyBox3.bmp");
	Skybox box2;
	box2.Init("../glsl/vs_sky.glsl", "../glsl/fs_sky.glsl", 2, "../data/SkyBox0.bmp");
	Skybox box3;
	box3.Init("../glsl/vs_sky.glsl", "../glsl/fs_sky.glsl", 3, "../data/SkyBox1.bmp");
	Skybox box4;
	box4.Init("../glsl/vs_sky.glsl", "../glsl/fs_sky.glsl", 4, "../data/SkyBox2.bmp");
	Skybox box5;
	box5.Init("../glsl/vs_sky.glsl", "../glsl/fs_sky.glsl", 5, "../data/SkyBox4.bmp");
	Water sea;
	sea.Init("../glsl/vs_water.glsl", "../glsl/fs_water.glsl", "../data/SkyBox5.bmp");
	sea.Wave_Init("../glsl/vs_wave.glsl", "../glsl/fs_wave.glsl", "../data/SkyBox5.bmp");


	while (1)
	{
		glfwPollEvents();
		static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, black);

		float aspect = (float)Height / (float)Width;
		View = glm::mat4(glm::lookAt(center + glm::vec3(viewx, viewy, viewz), center, glm::vec3(0.0f, 0.0f, 1.0f)));
		Projection = glm::mat4(glm::perspective(glm::radians(100.0f), 1.0f / aspect, nearz, farz));

		mount.Display(window, Model, View, Projection);
		box1.Display(window, Model, View, Projection);
		box2.Display(window, Model, View, Projection);
		box3.Display(window, Model, View, Projection);
		box4.Display(window, Model, View, Projection);
		box5.Display(window, Model, View, Projection);
		sea.Display(window, Model, View, Projection);
		sea.WaveDisplay(window);
		
		glfwSwapBuffers(window);
	}

	return 0;
}


/*
///////////TERRAIN ENGINE/////////////
1. 山体重建： 已完成· 通过opencv灰度图设置解决了高度错误问题
2. 规范坐标： 已完成· 笛卡尔坐标系，高度坐标与z轴重合
3. 设置多级纹理： 已完成·强行修图
4. 设置天空: 已完成·
5. 设置海水: 已完成·
6. 设置倒影: 已完成·
7. 设置海水波动:已完成·
8. 光影优化；
9. 波动优化：已完成·
*/
