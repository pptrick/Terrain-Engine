#include "mountain.h"
#include "LoadShaders.h"

#define BROAD_CONTROLLER 4
#define HEIGHT_CONTROLLER 1.2
#define SEALEVEL 92

using namespace std;

vector<slet> silhouette;

mountain::mountain()
{
	glGenVertexArrays(1, &mountVAO);
	glGenBuffers(1, &mountVBO);
	glGenBuffers(1, &mountEBO);
}

mountain::~mountain()
{
	glDeleteVertexArrays(1, &mountVAO);
	glDeleteBuffers(1, &mountVBO);
	glDeleteBuffers(1, &mountEBO);
}

void mountain::Init(const string vs_name,const string fs_name, const char* heightmap_filename, const char* textfilename, const char* detailfilename)
{
	ShaderInfo shader_info[] =
	{
		{ GL_VERTEX_SHADER, vs_name.c_str() },
	{ GL_FRAGMENT_SHADER, fs_name.c_str() },
	{ GL_NONE, NULL }
	};
	mountProg = LoadShaders(shader_info);

	LoadData(heightmap_filename);
	BindBuffer();
	GenTexture(textfilename, detailfilename);
	FindSilhouette();

}

void mountain::BindBuffer()
{
	glBindVertexArray(mountVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mountVBO);
	glBufferData(GL_ARRAY_BUFFER, Points.size() * sizeof(GLfloat), &Points[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mountEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLint), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void mountain::GenTexture(const char* textfilename, const char* detailfilename)
{
	glGenTextures(1, &mountTex);

	glBindTexture(GL_TEXTURE_2D, mountTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	cv::Mat img1 = cv::imread(textfilename);
	int mWidth = img1.cols;
	int mHeight = img1.rows;
	int mchannel = img1.channels();

	int mpixellength = mWidth * mHeight * mchannel;
	void *mpixels = new GLubyte[mpixellength];
	memcpy(mpixels, img1.data, mpixellength * sizeof(char));

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, mpixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	
	glGenTextures(1, &rockTex);

	glBindTexture(GL_TEXTURE_2D, rockTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	cv::Mat img2 = cv::imread(detailfilename,CV_BGR2GRAY);
	int rWidth = img2.cols;
	int rHeight = img2.rows;
	int rchannel = img2.channels();

	/*
	cv::namedWindow("show");
	cv::imshow("show", img2);
	cv::waitKey(0);
	*/

	int rpixellength = rWidth * rHeight * rchannel;
	void *rpixels = new GLubyte[rpixellength];
	memcpy(rpixels, img2.data, rpixellength * sizeof(char));

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rWidth, rHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, rpixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

void mountain::SetUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	glUseProgram(mountProg);

	GLuint model_matrix_loc = glGetUniformLocation(mountProg, "model_matrix");
	GLuint view_matrix_loc = glGetUniformLocation(mountProg, "view_matrix");
	GLuint projection_matrix_loc = glGetUniformLocation(mountProg, "projection_matrix");

	glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, glm::value_ptr(projection));

	glUseProgram(0);
}

void mountain::Display(GLFWwindow* window,const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetUniforms(model, view, projection);

	glUseProgram(mountProg);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mountTex);
	glUniform1i(glGetUniformLocation(mountProg, "mountTexture"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, rockTex);
	glUniform1i(glGetUniformLocation(mountProg, "rockTexture"), 1);

	int reflect = 0;
	GLuint reflect_loc = glGetUniformLocation(mountProg, "reflect");
	glUniform1i(reflect_loc, reflect);

	glBindVertexArray(mountVAO);
	glDrawElements(GL_TRIANGLES, sizeof(GLfloat) * indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	reflect = 1;
	reflect_loc = glGetUniformLocation(mountProg, "reflect");
	glUniform1i(reflect_loc, reflect);

	glBindVertexArray(mountVAO);
	glDrawElements(GL_TRIANGLES, sizeof(GLfloat) * indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void mountain::LoadData(const char* heightmap_filename)
{
	heightmap = cv::imread(heightmap_filename,0);
	heightmap_rows = heightmap.rows;
	heightmap_cols = heightmap.cols;

	for (int i = 0; i < heightmap.rows; i++) height.push_back(vector<float>(heightmap_cols, 0));
	for (int u = 0; u < heightmap_rows; u++)
	{
		for (int v = 0; v < heightmap_cols; v++)
		{
			height[u][v] = heightmap.at<uchar>(u, v);
			Points.push_back(BROAD_CONTROLLER*(u - 128));
			Points.push_back(BROAD_CONTROLLER*(v - 128));
			Points.push_back(HEIGHT_CONTROLLER*height[u][v]);
			Points.push_back((float)u / 255.);
			Points.push_back((float)v / 255.);
		}
	}

	for (int row = 1; row < heightmap_rows; row++)
	{
		for (int col = 1; col < heightmap_cols; col++)
		{
			indices.push_back((row - 1) * heightmap_cols + col - 1);
			indices.push_back((row - 1) * heightmap_cols + col);
			indices.push_back(row * heightmap_cols + col - 1);
			indices.push_back(row * heightmap_cols + col - 1);
			indices.push_back((row - 1) * heightmap_cols + col);
			indices.push_back(row * heightmap_cols + col);
		}
	}
}

void mountain::FindSilhouette()
{
	int count = 0;
	for (int i = 0; i < Points.size() / 5; i++)
	{
		if (abs(Points[5 * i + 2] - SEALEVEL)<2)
		{
			slet buffer(Points[5 * i], Points[5 * i + 1], Points[5 * i + 2]);
			buffer.num = count;
			silhouette.push_back(buffer);
			count++;
		}
	}
}