#include "Sky.h"
#include "LoadShaders.h"

#define EDGE 5000
#define HEIGHTRATE 0.6
#define SEALEVEL 92

using namespace std;

Skybox::Skybox()
{
	glGenVertexArrays(1, &skyVAO);
	glGenBuffers(1, &skyVBO);
	glGenBuffers(1, &skyEBO);
}

Skybox::~Skybox()
{
	glDeleteVertexArrays(1, &skyVAO);
	glDeleteBuffers(1, &skyVBO);
	glDeleteBuffers(1, &skyEBO);
}

void Skybox::Init(const string vs_name, const string fs_name,int num, const char* textfilename)
{
	ShaderInfo shader_info[] =
	{
		{ GL_VERTEX_SHADER, vs_name.c_str() },
	{ GL_FRAGMENT_SHADER, fs_name.c_str() },
	{ GL_NONE, NULL }
	};
	skyProg = LoadShaders(shader_info);

	CalData(num);
	BindBuffer();
	GenTexture(textfilename);
}

void Skybox::CalData(int num)
{
	edge = EDGE;
	number = num;
	switch (number)
	{
	case 1:
		vertices.push_back(edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(1.0);
		
		//
		vertices.push_back(edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(1.0);

	
	case 2:
		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(1.0);

		//
		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(1.0);

	
	case 3:
		vertices.push_back(-edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(1.0);

		//
		vertices.push_back(-edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(1.0);


	case 4:
		vertices.push_back(edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(1.0);

		//
		vertices.push_back(edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(0.0+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(1.0);


	case 5:
		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(edge*HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(1.0);

		//
		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(0.0);

		vertices.push_back(-edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(0.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(-edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(0.0);
		vertices.push_back(1.0);

		vertices.push_back(edge / 2.);
		vertices.push_back(edge / 2.);
		vertices.push_back(-edge * HEIGHTRATE+ SEALEVEL);
		vertices.push_back(1.0);
		vertices.push_back(1.0);
	}

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);
	indices.push_back(5);
	indices.push_back(6);
}

void Skybox::BindBuffer()
{
	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Skybox::GenTexture(const char* textfilename)
{
	glGenTextures(1, &skyTex);

	glBindTexture(GL_TEXTURE_2D, skyTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	cv::Mat img1 = cv::imread(textfilename);
	int Width = img1.cols;
	int Height = img1.rows;
	int channel = img1.channels();

	int pixellength = Width * Height * channel;
	void *pixels = new GLubyte[pixellength];
	memcpy(pixels, img1.data, pixellength * sizeof(char));

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	/*
	cv::namedWindow("show");
	cv::imshow("show", img2);
	cv::waitKey(0);
	*/
}

void Skybox::SetUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	glUseProgram(skyProg);

	GLuint model_matrix_loc = glGetUniformLocation(skyProg, "model_matrix");
	GLuint view_matrix_loc = glGetUniformLocation(skyProg, "view_matrix");
	GLuint projection_matrix_loc = glGetUniformLocation(skyProg, "projection_matrix");

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glUseProgram(0);
}

void Skybox::Display(GLFWwindow* window, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	SetUniforms(model, view, projection);

	glUseProgram(skyProg);

	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skyTex);
	glUniform1i(glGetUniformLocation(skyProg, "Texture"), 0);

	glBindVertexArray(skyVAO);
	glDrawElements(GL_TRIANGLES, sizeof(GLfloat) * indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}