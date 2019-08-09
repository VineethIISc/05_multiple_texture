#include "Shader.h"
#include "Texture.h"
#include "TestQuad.h"




TestQuad::TestQuad():VBO(0), VAO(0), objectShader(NULL)
{
	//draw 2 quad, one in first quadrant and one in third quadrant
	numVertices = 12;

	//create vertices and color
	initVertexData();

	//initialise the first texture
	modelTexture1 = new Texture("container.jpg");

	//initialise the second texture 
	modelTexture2 = new Texture("face.png");
}


TestQuad::~TestQuad()
{
	if (VAO)
		glDeleteVertexArrays(1, &VAO);

	if (VBO)
		glDeleteBuffers(1, &VBO);

	if (textureObject)
		glDeleteTextures(1, &textureObject);

	if (modelTexture1)
		delete modelTexture1;

	if (modelTexture2)
		delete modelTexture2;
}

GLuint TestQuad::getVBO()
{
	return VBO;
}

GLuint TestQuad::getVAO()
{
	return VAO;
}

void TestQuad::initVertexData()
{
	//one quad in first quadrant
	//triangle 1
	vertices.push_back(Vector3f(0.0f, 0.0f, 0.0f));
	vertices.push_back(Vector3f(1.0f, 0.0f, 0.0f));
	vertices.push_back(Vector3f(1.0f, 1.0f, 0.0f));

	//triangle 2
	vertices.push_back(Vector3f(0.0f, 0.0f, 0.0f));
	vertices.push_back(Vector3f(1.0f, 1.0f, 0.0f));
	vertices.push_back(Vector3f(0.0f, 1.0f, 0.0f));

	//create three vertices for the triangle
	//color for triangle 1
	Vector3f redColor(1.0f, 0.0f, 0.0f);
	colors.push_back(redColor);
	colors.push_back(redColor);
	colors.push_back(redColor);

	//color for triangle 2
	colors.push_back(redColor);
	colors.push_back(redColor);
	colors.push_back(redColor);

	//texture coordinate for each vertices
	//triangle 1
	textCoord.push_back(Vector2f(0.0f, 0.0f));
	textCoord.push_back(Vector2f(1.0f, 0.0f));
	textCoord.push_back(Vector2f(1.0f, 1.0f));

	//triangle 2
	textCoord.push_back(Vector2f(0.0f, 0.0f));
	textCoord.push_back(Vector2f(1.0f, 1.0f));
	textCoord.push_back(Vector2f(0.0f, 1.0f));



	//--------------------------------------
	//drawing another quad in third quadrant
	//triangle 1
	vertices.push_back(Vector3f(0.0f, 0.0f, 0.0f));
	vertices.push_back(Vector3f(-1.0f, 0.0f, 0.0f));
	vertices.push_back(Vector3f(-1.0f, -1.0f, 0.0f));

	//triangle 2
	vertices.push_back(Vector3f(0.0f, 0.0f, 0.0f));
	vertices.push_back(Vector3f(-1.0f, -1.0f, 0.0f));
	vertices.push_back(Vector3f(0.0f,-1.0f, 0.0f));

	//create three vertices for the triangle
	//color for triangle 1
	Vector3f whiteColor(1.0f, 1.0f, 1.0f);
	colors.push_back(whiteColor);
	colors.push_back(whiteColor);
	colors.push_back(whiteColor);

	//color for triangle 2
	colors.push_back(whiteColor);
	colors.push_back(whiteColor);
	colors.push_back(whiteColor);

	//texture coordinate for each vertices
	//triangle 1
	textCoord.push_back(Vector2f(0.0f, 0.0f));
	textCoord.push_back(Vector2f(1.0f, 0.0f));
	textCoord.push_back(Vector2f(1.0f, 1.0f));

	//triangle 2
	textCoord.push_back(Vector2f(0.0f, 0.0f));
	textCoord.push_back(Vector2f(1.0f, 1.0f));
	textCoord.push_back(Vector2f(0.0f, 1.0f));
}

void TestQuad::loadData()
{
	//create vertex array objects
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//bind VAO, VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//allocate memory in GPU
	GLsizeiptr VertexSize = sizeof(Vector3f) * numVertices;
	GLsizeiptr ColorSize = sizeof(Vector3f) * numVertices;
	GLsizeiptr TextureSize = sizeof(Vector2f) * numVertices;
	GLsizeiptr TotalSize = VertexSize + ColorSize + TextureSize;
	glBufferData(GL_ARRAY_BUFFER, TotalSize, NULL, GL_STATIC_DRAW);

	//pass vertex position and color data
	GLintptr offset = 0;
	glBufferSubData(GL_ARRAY_BUFFER, offset, VertexSize, vertices[0]);
	offset += VertexSize;
	glBufferSubData(GL_ARRAY_BUFFER, offset, ColorSize, colors[0]);
	offset += ColorSize;
	glBufferSubData(GL_ARRAY_BUFFER, offset, TextureSize, textCoord[0]);

	//enable vertex attrib pointer and information about data
	glEnableVertexAttribArray(0);		//position
	glEnableVertexAttribArray(1);		//color
	glEnableVertexAttribArray(2);		//texture

	//assign attribute pointer
	GLuint currBufferOffset = 0;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(currBufferOffset));
	currBufferOffset += VertexSize;		
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(currBufferOffset));
	currBufferOffset += ColorSize;
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(currBufferOffset));

	//unbind VAO, VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	//create shader do after 
	objectShader = new Shader(SHADER_PATH"vsTestQuad.txt", SHADER_PATH"fsTestQuad.txt");
	
	//load texture data to GPU
	modelTexture1->loadTexture();
	modelTexture2->loadTexture();
}

void TestQuad::render()
{
	//bind the shader program
	objectShader->Bind();

	//assign uniform variable and texture object
	objectShader->setTextureVariable("ourTexture1", 0);
	objectShader->setTextureVariable("ourTexture2", 1);

	//bind the VAO and call draw function
	//bind the texture object to target : 2D texture
	modelTexture1->UseTexture(GL_TEXTURE0);	//bind the first texture to texture unit 1
	modelTexture2->UseTexture(GL_TEXTURE1);	//bind the second texture to texture unit 2

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 0.5 *numVertices);		//draw the first quad
	glDrawArrays(GL_TRIANGLES, 0.5 * numVertices, 0.5 * numVertices);	//draw the second quad

	glBindVertexArray(0);
	modelTexture1->unbindTexture();
	modelTexture2->unbindTexture();

	//unbind the shader
	objectShader->Unbind();
}




