#include "water.h"
#include "LoadShaders.h"

#define EDGE 5000
#define SEALEVEL 92
#define WaveRange 1.07
#define FREQUENCY 0.4
#define WAVENUM 50

using namespace std;

vector<slet> silhouette_buf;
extern vector<slet> silhouette;

Water::Water()
{
	glGenVertexArrays(1, &waterVAO);
	glGenBuffers(1, &waterVBO);
	glGenBuffers(1, &waterEBO);

	glGenVertexArrays(1, &waveVAO);
	glGenBuffers(1, &waveVBO);
	glGenBuffers(1, &waveEBO);
}

Water::~Water()
{
	glDeleteVertexArrays(1, &waterVAO);
	glDeleteBuffers(1, &waterVBO);
	glDeleteBuffers(1, &waterEBO);

	glDeleteVertexArrays(1, &waveVAO);
	glDeleteBuffers(1, &waveVBO);
	glDeleteBuffers(1, &waveEBO);
}

void Water::Init(const string vs_name, const string fs_name, const char* textfilename)
{
	ShaderInfo shader_info[] =
	{
		{ GL_VERTEX_SHADER, vs_name.c_str() },
	{ GL_FRAGMENT_SHADER, fs_name.c_str() },
	{ GL_NONE, NULL }
	};
	waterProg = LoadShaders(shader_info);

	CalData();
	BindBuffer();
	GenTexture(textfilename);
}

void Water::Wave_Init(const string vs_name, const string fs_name, const char* textfilename)
{
	ShaderInfo shader_info[] =
	{
		{ GL_VERTEX_SHADER, vs_name.c_str() },
	{ GL_FRAGMENT_SHADER, fs_name.c_str() },
	{ GL_NONE, NULL }
	};
	waveProg = LoadShaders(shader_info);

	Cal_Silhouette();
	Wave_BindBuffer();
}

void Water::CalData()
{
	float edge = EDGE;
	float SeaLevel = SEALEVEL;

	for (int u = 0; u < edge/ WAVENUM; u++)
	{
		for (int v = 0; v < edge/ WAVENUM; v++)
		{
			vertices.push_back(u*WAVENUM -edge/2);
			vertices.push_back(v*WAVENUM -edge/2);
			vertices.push_back(SeaLevel);
			vertices.push_back((float)u / (edge / WAVENUM - 1));
			vertices.push_back((float)v / (edge / WAVENUM - 1));
		}
	}

	for (int row = 1; row < edge / WAVENUM; row++)
	{
		for (int col = 1; col < edge / WAVENUM; col++)
		{
			indices.push_back((row - 1) * edge / WAVENUM + col - 1);
			indices.push_back((row - 1) * edge / WAVENUM + col);
			indices.push_back(row * edge / WAVENUM + col - 1);
			indices.push_back(row * edge / WAVENUM + col - 1);
			indices.push_back((row - 1) * edge / WAVENUM + col);
			indices.push_back(row * edge / WAVENUM + col);
		}
	}
}

void Water::BindBuffer()
{
	glBindVertexArray(waterVAO);
	glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Water::Wave_BindBuffer()
{
	glBindVertexArray(waveVAO);
	glBindBuffer(GL_ARRAY_BUFFER, waveVBO);
	glBufferData(GL_ARRAY_BUFFER, WavePoints.size() * sizeof(GLfloat), &WavePoints[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waveEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, WaveIndices.size() * sizeof(GLint), &WaveIndices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Water::GenTexture(const char* textfilename)
{
	glGenTextures(1, &waterTex);

	glBindTexture(GL_TEXTURE_2D, waterTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	cv::Mat img1 = cv::imread(textfilename,-1);
	int Width = img1.cols;
	int Height = img1.rows;
	int channel = img1.channels();

	int pixellength = Width * Height * channel;
	void *pixels = new GLubyte[pixellength];
	memcpy(pixels, img1.data, pixellength * sizeof(char));

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	/*
	cv::namedWindow("show");
	cv::imshow("show", img2);
	cv::waitKey(0);
	*/
}

void Water::SetUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	glUseProgram(waterProg);

	GLuint model_matrix_loc = glGetUniformLocation(waterProg, "model_matrix");
	GLuint view_matrix_loc = glGetUniformLocation(waterProg, "view_matrix");
	GLuint projection_matrix_loc = glGetUniformLocation(waterProg, "projection_matrix");
	GLuint Time_loc = glGetUniformLocation(waterProg, "Time");

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint TimeCount_loc = glGetUniformLocation(waterProg, "TimeCount");
	float t = glfwGetTime();
	float T = 0.1*t;
	glUniform1f(TimeCount_loc, T);
	glUniform1f(Time_loc, T);

	glUseProgram(0);

	glUseProgram(waveProg);

	model_matrix_loc = glGetUniformLocation(waveProg, "model_matrix");
	view_matrix_loc = glGetUniformLocation(waveProg, "view_matrix");
	projection_matrix_loc = glGetUniformLocation(waveProg, "projection_matrix");

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glUseProgram(0);
}

void Water::WaveDynamic()
{
	glUseProgram(waveProg);

	GLuint TimeCount_loc = glGetUniformLocation(waveProg, "TimeCount");
	float t = glfwGetTime();
	float T = 0.5*t;
	glUniform1f(TimeCount_loc, T);

}

void Water::Display(GLFWwindow* window, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetUniforms(model, view, projection);

	glUseProgram(waterProg);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glUniform1i(glGetUniformLocation(waterProg, "Texture"), 0);

	glBindVertexArray(waterVAO);
	glDrawElements(GL_TRIANGLES, sizeof(GLfloat) * indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void Water::WaveDisplay(GLFWwindow* window)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(waveProg);

	WaveDynamic();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);
	glUniform1i(glGetUniformLocation(waveProg, "Texture"), 0);

	glBindVertexArray(waveVAO);
	glDrawElements(GL_TRIANGLES, sizeof(GLfloat) * WaveIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}


bool comp(slet a, slet b)
{
	float dist_a = a.x*a.x + a.y*a.y + a.z*a.z;
	float dist_b = b.x*b.x + b.y*b.y + b.z*b.z;
	return dist_a < dist_b;
}

void Water::Cal_Silhouette()
{
	for (int i = 0; i < silhouette.size(); i++)
	{
		int count=0;
		silhouette_buf.assign(silhouette.begin(), silhouette.end());
		for (int j = 0; j < silhouette_buf.size(); j++)
		{
			silhouette_buf[j].x -= silhouette[i].x;
			silhouette_buf[j].y -= silhouette[i].y;
			silhouette_buf[j].z -= silhouette[i].z;
		}
		sort(silhouette_buf.begin(), silhouette_buf.end(), comp);
		for (int j = 1; j < silhouette_buf.size(); j++)
		{
			if (abs(silhouette_buf[j].x) > 100|| abs(silhouette_buf[j].y) > 100)break;
			if (silhouette_buf[j].visited == 0)
			{
				silhouette_buf[j].visited = 1;
				silhouette[silhouette_buf[j].num].visited = 1;
				silhouette_buf[j].x += silhouette[i].x;
				silhouette_buf[j].y += silhouette[i].y;
				silhouette_buf[j].z += silhouette[i].z;
				count = j;
				break;
			}
		}
		LoadSilhouette(silhouette[i], silhouette_buf[count]);
	}

	for (int i = 0; i < 2*silhouette.size(); i++)
	{
		WaveIndices.push_back(4 * i);
		WaveIndices.push_back(4 * i+1);
		WaveIndices.push_back(4 * i+2);
		WaveIndices.push_back(4 * i + 1);
		WaveIndices.push_back(4 * i + 2);
		WaveIndices.push_back(4 * i+3);
	}
}

void Water::LoadSilhouette(slet v0, slet v1)
{
	static float phase = 0;
	float factor = sin(phase);
	float dist0 = sqrt(v0.x*v0.x+v0.y*v0.y);
	float dist1 = sqrt(v1.x*v1.x + v1.y*v1.y);
	float dist = sqrt((v0.x - v1.x)*(v0.x - v1.x) + (v0.y - v1.y)*(v0.y - v1.y));
	float rate = 50 / dist;
	float rate0 = WaveRange * dist0;
	float rate1 = WaveRange * dist1;
	float SeaLevel = SEALEVEL;
	WavePoints.push_back(v0.x);
	WavePoints.push_back(v0.y);
	WavePoints.push_back(SeaLevel);
	WavePoints.push_back(0.0);
	WavePoints.push_back(0.0);
	WavePoints.push_back(factor);

	WavePoints.push_back(v1.x);
	WavePoints.push_back(v1.y);
	WavePoints.push_back(SeaLevel);
	WavePoints.push_back(1.0);
	WavePoints.push_back(0.0);
	WavePoints.push_back(factor);

	WavePoints.push_back(rate0*v0.x/dist0);
	WavePoints.push_back(rate0*v0.y/dist0);
	WavePoints.push_back(SeaLevel-6);
	WavePoints.push_back(0.0);
	WavePoints.push_back(1.0);
	WavePoints.push_back(factor);

	WavePoints.push_back(rate1*v1.x / dist1);
	WavePoints.push_back(rate1*v1.y / dist1);
	WavePoints.push_back(SeaLevel-6);
	WavePoints.push_back(1.0);
	WavePoints.push_back(1.0);
	WavePoints.push_back(factor);

	//
	WavePoints.push_back(v0.x);
	WavePoints.push_back(v0.y);
	WavePoints.push_back(SeaLevel);
	WavePoints.push_back(0.0);
	WavePoints.push_back(0.0);
	WavePoints.push_back(factor);

	WavePoints.push_back(v1.x);
	WavePoints.push_back(v1.y);
	WavePoints.push_back(SeaLevel);
	WavePoints.push_back(1.0);
	WavePoints.push_back(0.0);
	WavePoints.push_back(factor);

	WavePoints.push_back(v0.x + (v1.y - v0.y)*rate);
	WavePoints.push_back(v0.y - (v1.x - v0.x)*rate);
	WavePoints.push_back(SeaLevel-6);
	WavePoints.push_back(0.0);
	WavePoints.push_back(1.0);
	WavePoints.push_back(factor);

	WavePoints.push_back((- v0.y + v1.y)*rate + v1.x);
	WavePoints.push_back(v1.y + (- v1.x + v0.x)*rate);
	WavePoints.push_back(SeaLevel-6);
	WavePoints.push_back(1.0);
	WavePoints.push_back(1.0);
	WavePoints.push_back(factor);

	phase+=30;
}