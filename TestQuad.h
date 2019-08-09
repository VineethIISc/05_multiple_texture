#ifndef TEST_QUAD_H
#define TEST_QUAD_H

#include "CommonHeader.h"

class Shader;
class Texture;

class TestQuad
{
public:
	TestQuad();
	~TestQuad();

	//debug
	GLuint getVBO();
	GLuint getVAO();

	//rendering purpose
	void loadData();
	void render();

private:
	GLuint VBO, VAO, textureObject;
	std::vector<Vector3f> vertices;
	std::vector<Vector3f> colors;
	std::vector<Vector2f> textCoord;
	int numVertices;

	//triangle vertices;
	void initVertexData();

	//create a shader for the current object from source file
	Shader* objectShader;

	//Texture object
	Texture* modelTexture1;	//first texture
	Texture* modelTexture2;	//second texture


	
	
};
#endif // !TEST_QUAD_H



