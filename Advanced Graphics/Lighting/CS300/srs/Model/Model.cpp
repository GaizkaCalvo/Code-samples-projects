#include "Model.h"

#include <glm/gtc/constants.hpp>
#include <math.h>
#include <glm/gtx/normal.hpp>
#include <algorithm>

Model::Model() : mHeight(1.0f), mShowNormal(false), mShowAverageNormal(false), 
	mRadius(0.5f), mPrecision(5), mStack(2), mTextured(true), mShowAxis(false), Deleted(false)
{
	mRed.push_back(1.0f);
	mRed.push_back(0.0f);
	mRed.push_back(0.0f);
	mRed.push_back(0.0f);

	mTexture.LoadGeneralTexture();
}

Model::~Model()
{
	Clear();
}

///
// Brief description: Some Easy forms we will compute by hand
///
namespace ModelForms
{
	std::vector<float> Plane = {
	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f   // top left 
	};

	std::vector<float> PaneTextCoord = {
		1.0f, 1.0f,   // 
		1.0f, 0.0f,   // 
		0.0f, 0.0f,   // 
		0.0f, 1.0f    // 
	};

	std::vector<unsigned int> PaneIndices = {// note that we start from 0!
		0, 3, 1,   // first triangle
		3, 2, 1    // second triangle
	};

	std::vector<float> Cube = {
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//0 
	 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//1
	 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//2
	-0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//3
													 //
	-0.5f, -0.5f, -0.5, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//4
	 0.5f, -0.5f, -0.5, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//5
	 0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//6
	-0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//7
													 //
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//8--
	 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//9--
	 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//10--
	-0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//11--
													 //
	-0.5f, -0.5f, -0.5, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//12--
	 0.5f, -0.5f, -0.5, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//13--
	 0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//14--
	-0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//15--
													 //
	-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//16--
	 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//17--
	 0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//18--
	-0.5f,  0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//19--
													 //
	-0.5f, -0.5f, -0.5, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,//20--
	 0.5f, -0.5f, -0.5, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//21
	 0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//22--
	-0.5f,  0.5f, -0.5, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,//23--
	};

	std::vector<float> CubeTextCoord = {
		1.0f, 1.0f,   //0 
		1.0f, 0.0f,   // 1
		0.0f, 0.0f,   // 2
		0.0f, 1.0f,   // 3

		1.0f, 0.0f,   // 4
		0.0f, 0.0f,   // 5
		0.0f, 1.0f,   // 6
		1.0f, 1.0f,   // 7

		1.0f, 0.0f,   // 8
		1.0f, 0.0f,   // 9
		1.0f, 1.0f,   // 10
		1.0f, 1.0f,   // 11

		0.0f, 0.0f,   // 12
		1.0f, 0.0f,   // 13
		1.0f, 0.0f,   // 14
		0.0f, 1.0f,   // 15

		0.0f, 0.0f,   // 16
		0.0f, 0.0f,   // 17
		0.0f, 1.0f,   // 18
		0.0f, 1.0f,   // 19

		0.0f, 1.0f,   // 20
		1.0f, 1.0f,   // 21
		1.0f, 1.0f,   // 22
		0.0f, 0.0f    // 23
	};

	std::vector<unsigned int> CubeIndices = {
		0, 1, 2,  
		2, 3, 0,
		7, 6, 5,
		5, 4, 7,
		15,12, 8,
		15,8,11,
		14,23,19,
		14,19,10,
		18,17,13,
		18,13,22,
		9,16,20,
		9,20,21
	};
};

///
// Brief description: Clear all the VAOs and VBOs of the model
///
void Model::Clear()
{
	if (mForm == Model::NONE)
		return;

	// Delete the VAO
	glDeleteVertexArrays(1, &vao);
	glDeleteVertexArrays(1, &mNormalVao);
	glDeleteVertexArrays(1, &mAverageNormalVao);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	//delete the shaders that we created
	// Delete the VBOs
	glDeleteBuffers(1, &positionBufferObject);
	glDeleteBuffers(1, &mTextureBufferObject); 
	glDeleteBuffers(1, &mNormalBufferObject);
	glDeleteBuffers(1, &mAverageNormalBufferObject);
	glDeleteBuffers(1, &mNormals);
	glDeleteBuffers(1, &mAverageNormals);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	

	//Clear vectors
	mModelIndices.clear();
	mModelNormals.clear();
	mModel.clear();
	mModelTextCoord.clear();
	mModelVertexNormal.clear();
	mNormal.clear();
	mModelAverageNormals.clear();
	mAveNormal.clear();
	mModelVertexAveNormal.clear();
}

///
// Brief description: Put the model that the object will use
///
void Model::Initialize(Model::Forms form)
{
	mForm = form;

	//we check for the model that we will use and we create it
	if (form == Model::NONE)
		return;

	if (form == Model::PLANE)
	{
		mModel = ModelForms::Plane;
		mModelIndices = ModelForms::PaneIndices;
		mModelTextCoord = ModelForms::PaneTextCoord;
	}
	else if (form == Model::CUBE)
	{
		mModel = ModelForms::Cube;
		mModelIndices = ModelForms::CubeIndices;
		mModelTextCoord = ModelForms::CubeTextCoord;
	}
	else if (form == Model::CONE)
	{
		MakeCone();
	}
	else if (form == Model::CYLINDER)
	{
		MakeCylinder();
	}
	else if (form == Model::SPHERE)
	{
		MakeSphere();
	}
	MakeNormals();

	// VAO, 
	glGenVertexArrays(1, &vao);

	// VBO
	glGenBuffers(1, &positionBufferObject);
	// VBO
	glGenBuffers(1, &mTextureBufferObject);
	// VBO
	glGenBuffers(1, &mNormalBufferObject);
	// VBO
	glGenBuffers(1, &mAverageNormalBufferObject);
	//EBO
	glGenBuffers(1, &EBO);

	glBindVertexArray(vao);

	//we bind the VBO to use it with the vertices that we need
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	//here we put the vertices in the VBO, and as its binded we dont need to specify anything
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mModel.size(), &mModel[0], GL_STATIC_DRAW);

	//put the vbo into the vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mModelIndices.size(), &mModelIndices[0], GL_STATIC_DRAW);

	// Insert the VBO into the VAO
	// position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//// color attribute
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, mAverageNormalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mAveNormal.size(), &mAveNormal[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//Put the text coordinates
	glBindBuffer(GL_ARRAY_BUFFER, mTextureBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mModelTextCoord.size(), &mModelTextCoord[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	//normals
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mNormal.size(), &mNormal[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(3);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//NORMALS
#pragma region NORMALS
	//vao
	glGenVertexArrays(1, &mNormalVao);
	//vbo
	glGenBuffers(1, &mNormals);
	//bind vao
	glBindVertexArray(mNormalVao);
	//bind normals
	glBindBuffer(GL_ARRAY_BUFFER, mNormals);
	//put nomrmals
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mModelNormals.size(), &mModelNormals[0], GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

#pragma endregion NORMALS

//AVERAGE NORMALS
#pragma region AVERAGENORMALS
	//vao
	glGenVertexArrays(1, &mAverageNormalVao);
	//vbo
	glGenBuffers(1, &mAverageNormals);
	//bind vao
	glBindVertexArray(mAverageNormalVao);
	//bind normals
	glBindBuffer(GL_ARRAY_BUFFER, mAverageNormals);
	//put nomrmals
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mModelAverageNormals.size(), &mModelAverageNormals[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

#pragma endregion AVERAGENORMALS
}

///
// Brief description: Function we will call to change the model 
///
void Model::ChangeModel(Model::Forms form, int precision, int Stack, float radius)
{
	Clear();
	mPrecision = precision;
	mStack = Stack;
	mRadius = radius;
	Initialize(form);
}

///
// Brief description: Change to the ligt model (white cube)
///
void Model::LightModel()
{
	Clear();

	mModel = ModelForms::Cube;
	mModelIndices = ModelForms::CubeIndices;
	mModelTextCoord = ModelForms::CubeTextCoord;

	// VAO, 
	glGenVertexArrays(1, &vao);

	// VBO
	glGenBuffers(1, &positionBufferObject);
	//EBO
	glGenBuffers(1, &EBO);

	glBindVertexArray(vao);

	//we bind the VBO to use it with the vertices that we need
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	//here we put the vertices in the VBO, and as its binded we dont need to specify anything
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mModel.size(), &mModel[0], GL_STATIC_DRAW);

	//put the vbo into the vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mModelIndices.size(), &mModelIndices[0], GL_STATIC_DRAW);

	// Insert the VBO into the VAO
	// position attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//// color attribute
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


GLuint Model::GetModelVao() const
{
	return vao;
}

GLuint Model::GetNormalsVao() const
{
	return mNormalVao;
}

GLuint Model::GetAverageNormalsVao() const
{
	return mAverageNormalVao;
}

int Model::GetPrecision() const
{
	return mPrecision;
}

Model::Forms Model::GetForm() const
{
	return mForm;
}

void Model::ClearLights()
{
	// Delete the VBOs
	glDeleteBuffers(1, &positionBufferObject);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	// Delete the VAO
	glDeleteVertexArrays(1, &vao);
	error = glGetError();
	if (error != GL_NO_ERROR)
		abort();

	//Clear vectors
	mModel.clear();
	mModelIndices.clear();
	mModelTextCoord.clear();
}

///
// Brief description: Make the cone with the parameters given
///
void Model::MakeCone()
{

	float VerticeDistance = 2 * glm::pi<float>() / mPrecision;

	unsigned int indices = 0;
	for (int i = 0; i < mPrecision; i++)
	{
		//calculate the points in the circle
		float x = cosf(VerticeDistance * i) * mRadius;
		float z = sinf(VerticeDistance * i) * mRadius;
		float x2 = cosf(VerticeDistance * (i+1)) * mRadius;
		float z2 = sinf(VerticeDistance * (i+1)) * mRadius;
		//calculate the uv texture coodinates
		float u = ((static_cast<float>(i)/ mPrecision) + (static_cast<float>(i + 1) / mPrecision)) / 2;
		//if(mPrecision)
		float u1 = (static_cast<float>(i) / mPrecision);
		float u2 = (static_cast<float>(i+1) / mPrecision);

		//we put the top
		PushPointToModel(0.0f, mHeight/2, 0.0f);

		//we put its texture coord
		mModelTextCoord.push_back(u);
		mModelTextCoord.push_back(1.0f);
		//push the indices
		mModelIndices.push_back(indices++);


		//bottom right vertex
		PushPointToModel(x2, -mHeight/2, z2);
		//push uv
		mModelTextCoord.push_back(u2);
		mModelTextCoord.push_back(0.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//bottom left vertex
		PushPointToModel(x, -mHeight / 2, z);
		//push uv
		mModelTextCoord.push_back(u1);
		mModelTextCoord.push_back(0.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//push the indices
		//bottom right vertex
		PushPointToModel(x2, -mHeight / 2, z2);
		//push uv
		mModelTextCoord.push_back(u2);
		mModelTextCoord.push_back(0.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//we put the bot
		PushPointToModel(0.0f, -mHeight / 2, 0.0f);
		//we put its texture coord
		mModelTextCoord.push_back(u);
		mModelTextCoord.push_back(0.0f);
		mModelIndices.push_back(indices++);

		//bottom left vertex
		PushPointToModel(x, -mHeight / 2, z);
		//push uv
		mModelTextCoord.push_back(u1);
		mModelTextCoord.push_back(0.0f);
		//push the indices
		mModelIndices.push_back(indices++);

	}
}

///
// Brief description: Make the cylinders with the parameters given
///
void Model::MakeCylinder()
{

	float VerticeDistance = 2 * glm::pi<float>() / mPrecision;

	unsigned int indices = 0;
	for (int i = 0; i < mPrecision; i++)
	{
		//calculate the points in the circle
		float x = cosf(VerticeDistance * i) * mRadius;
		float z = sinf(VerticeDistance * i) * mRadius;
		float x2 = cosf(VerticeDistance * (i + 1)) * mRadius;
		float z2 = sinf(VerticeDistance * (i + 1)) * mRadius;
		//calculate the uv texture coodinates
		float u = ((static_cast<float>(i) / mPrecision) + (static_cast<float>(i + 1) / mPrecision)) / 2;
		float u1 = (static_cast<float>(i) / mPrecision);
		float u2 = (static_cast<float>(i + 1) / mPrecision);

		//we put the top
		PushPointToModel(0.0f, mHeight/2, 0.0f);
		//we put its texture coord
		mModelTextCoord.push_back(u);
		mModelTextCoord.push_back(1.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//top right vertex
		PushPointToModel(x2, mHeight / 2, z2);
		//push uv
		mModelTextCoord.push_back(u2);
		mModelTextCoord.push_back(1.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//top left vertex
		PushPointToModel(x, mHeight / 2, z);
		//push uv
		mModelTextCoord.push_back(u1);
		mModelTextCoord.push_back(1.0f);
		//push the indices
		mModelIndices.push_back(indices++);



		//we make the square
		PushPointToModel(x2, mHeight / 2, z2);
		//push uv
		mModelTextCoord.push_back(u2);
		mModelTextCoord.push_back(1.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//bottom left vertex
		PushPointToModel(x, -mHeight / 2, z);
		//push uv
		mModelTextCoord.push_back(u1);
		mModelTextCoord.push_back(0.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//top left vertex
		PushPointToModel(x, mHeight / 2, z);
		//push uv
		mModelTextCoord.push_back(u1);
		mModelTextCoord.push_back(1.0f);
		//push the indices
		mModelIndices.push_back(indices++);




		//top right vertex
		PushPointToModel(x2, mHeight / 2, z2);
		//push uv
		mModelTextCoord.push_back(u2);
		mModelTextCoord.push_back(1.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//bottom right vertex
		PushPointToModel(x2, -mHeight / 2, z2);
		//push uv
		mModelTextCoord.push_back(u2);
		mModelTextCoord.push_back(0.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//bottom left vertex
		PushPointToModel(x, -mHeight / 2, z);
		//push uv
		mModelTextCoord.push_back(u1);
		mModelTextCoord.push_back(0.0f);
		//push the indices
		mModelIndices.push_back(indices++);


		//bottom right vertex
		PushPointToModel(x2, -mHeight / 2, z2);
		//push uv
		mModelTextCoord.push_back(u2);
		mModelTextCoord.push_back(0.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//we put the bot
		PushPointToModel(0.0f, -mHeight / 2, 0.0f);
		//we put its texture coord
		mModelTextCoord.push_back(u);
		mModelTextCoord.push_back(0.0f);
		//push the indices
		mModelIndices.push_back(indices++);

		//bottom left vertex
		PushPointToModel(x, -mHeight / 2, z);
		//push uv
		mModelTextCoord.push_back(u1);
		mModelTextCoord.push_back(0.0f);
		//push the indices
		mModelIndices.push_back(indices++);

	}

}

///
// Brief description: Make the cylinders with the parameters given
// Reference: http://www.songho.ca/opengl/gl_sphere.html
///
void Model::MakeSphere()
{
#if 1
	//data we need to increment the circles
	float PI = glm::pi<float>();
	float PreciInc = 2 * PI / mPrecision;
	float StackInc = PI / mStack;
	
	bool DoubleVertex = false;
	//go throught all the stacs
	for (int i = 0; i <= mStack; )
	{
		// starting from pi/2 to -pi/2
		float stackAngle = PI/2 - i*StackInc;
	
		//sphere formula , we start on top
		float z = mRadius * sinf(stackAngle);
		

		if (i == 0 || i == mStack)
		{
			for (int j = 0; j < mPrecision; ++j)
			{
				// starting from 0 to 2pi
				float sectorAngle = j * PreciInc;

				//sphere formula 
				float x = mRadius * cosf(stackAngle) * cosf(sectorAngle);
				float y = mRadius * cosf(stackAngle) * sinf(sectorAngle);

				//push the points
				PushPointToModel(x, y, z);

				//TextCoordinates
				mModelTextCoord.push_back(static_cast<float>(j) / mPrecision);
				mModelTextCoord.push_back(static_cast<float>(i) / mStack);
			}
			++i;
		}
		else
		{
			// Taking into acount the precisions
			for (int j = 0; j <= mPrecision; ++j)
			{
				// starting from 0 to 2pi
				float sectorAngle = j * PreciInc;

				//sphere formula 
				float x = mRadius * cosf(stackAngle) * cosf(sectorAngle);
				float y = mRadius * cosf(stackAngle) * sinf(sectorAngle);

				//push the points
				PushPointToModel(x, y, z);
				//TextCoordinates
				mModelTextCoord.push_back(static_cast<float>(j) / mPrecision);
				mModelTextCoord.push_back(static_cast<float>(i) / mStack);

				if ((j != 0 && j != mPrecision))
				{
					//push the points
					PushPointToModel(x, y, z);

					//TextCoordinates
					mModelTextCoord.push_back(static_cast<float>(j) / mPrecision);
					mModelTextCoord.push_back(static_cast<float>(i) / mStack);
				}
			}
			if (DoubleVertex)
			{
				++i;
				DoubleVertex = !DoubleVertex;
			}
			else
				DoubleVertex = !DoubleVertex;
		}
	}
	
	
	////bottom part
	int j = 0;
	for (int i = 0; i < mPrecision; i++)
	{
		mModelIndices.push_back(j + mPrecision + 1);
		mModelIndices.push_back(i);
		mModelIndices.push_back(j + mPrecision);
		j += 2;
	}


	//middle part
	j = 0;
	int k = 1;
	//for (int i = 1; i < mStack-1; ++i, k+= 2)
	for (int i = 1; i < mStack-1; ++i, k+= 2)
	{
		// beginning of current stack
		int k1 = k * (mPrecision * 2) + mPrecision;
		// beginning of next stack
		int k2 = k1 + mPrecision * 2;
	
		for (int j = 0; j < mPrecision; ++j, k1+=2, k2+=2)
		{
			mModelIndices.push_back(k1);
			mModelIndices.push_back(k2);
			mModelIndices.push_back(k1 + 1);
	
			// second triangle
			mModelIndices.push_back(k1 + 1);
			mModelIndices.push_back(k2);
			mModelIndices.push_back(k2 + 1);
		}
	}

	////top part
	int TotalVerex = static_cast<int>(mModel.size() / 8);
	j = TotalVerex - mPrecision * 3;
	for (int i = 0; i < mPrecision; i++)
	{
		mModelIndices.push_back(j);
		mModelIndices.push_back(TotalVerex - (mPrecision - i));
		mModelIndices.push_back(j + 1);
		j += 2;
	}

#else
	//data we need to increment the circles
	float PI = glm::pi<float>();
	float PreciInc = 2 * PI / mPrecision;
	float StackInc = PI / mStack;
	
	//go throught all the stacs
	for (int i = 0; i <= 1; ++i)
	{
		// starting from pi/2 to -pi/2
		float stackAngle = PI/2 - i*StackInc;
	
		//sphere formula 
		float z = mRadius * sinf(stackAngle);
	
		// Taking into acount the precisions
		for (int j = 0; j <= mPrecision; ++j)
		{
			// starting from 0 to 2pi
			float sectorAngle = j * PreciInc;
	
			//sphere formula 
			float x = mRadius * cosf(stackAngle) * cosf(sectorAngle);
			float y = mRadius * cosf(stackAngle) * sinf(sectorAngle);
	
			//push the points
			PushPointToModel(x, y, z);
	
			//TextCoordinates
			mModelTextCoord.push_back(static_cast<float>(j) / mPrecision);
			mModelTextCoord.push_back(static_cast<float>(i) / mStack);
		}
	}
	
	int k1, k2;
	for (int i = 0; i < 1; ++i)
	{
		// beginning of current stack
		k1 = i * (mPrecision + 1);
		// beginning of next stack
		k2 = k1 + mPrecision + 1;
	
		for (int j = 0; j < mPrecision; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks,
			//coz they dont need 2 triangle
	
			// k1 => k2 => k1+1
			if (i != 0)
			{
				mModelIndices.push_back(k1);
				mModelIndices.push_back(k2);
				mModelIndices.push_back(k1 + 1);
			}
	
			//k1+1 => k2 => k2+1
			if (i != (mStack - 1))
			{
				mModelIndices.push_back(k1 + 1);
				mModelIndices.push_back(k2);
				mModelIndices.push_back(k2 + 1);
			}
		}
	}
#endif
}

///
// Brief description: Make the Normals of the model that we got
///
void Model::MakeNormals()
{
	//we go from all the triangles that we got and we compute the normals to get them 
	size_t size = mModelIndices.size();
	mNormal.resize(mModel.size() / 8);
	mModelVertexAveNormal.resize(mModel.size() / 8);
	for (size_t i = 0; i < size; i += 3)
	{
		//copute normals
		glm::vec3 normal = glm::triangleNormal(
			glm::vec3(mModel[mModelIndices[i] * 8], mModel[mModelIndices[i] * 8 + 1], mModel[mModelIndices[i] * 8 + 2]),
			glm::vec3(mModel[mModelIndices[i + 1] * 8], mModel[mModelIndices[i + 1] * 8 + 1], mModel[mModelIndices[i + 1] * 8 + 2]),
			glm::vec3(mModel[mModelIndices[i + 2] * 8], mModel[mModelIndices[i + 2] * 8 + 1], mModel[mModelIndices[i + 2] * 8 + 2]));
		//we save the normal 
		mModelVertexNormal.push_back(normal);

		mNormal[mModelIndices[i]] = normal;
		mNormal[mModelIndices[i + 1]] = normal;
		mNormal[mModelIndices[i + 2]] = normal;

		//we save all the normals that could appear in the indices
		mModelVertexAveNormal[mModelIndices[i]].push_back(normal);
		mModelVertexAveNormal[mModelIndices[i + 1]].push_back(normal);
		mModelVertexAveNormal[mModelIndices[i + 2]].push_back(normal);
	}

	int k = 0;
	//comnpute the normals pushing two points into the normal vector
	//we make a line that we will show if the user want
	for (size_t i = 0; i < size; i += 3, k++)
	{
		for (int j = 0; j < 3; j++)
		{
			mModelNormals.push_back(mModel[mModelIndices[i + j] * 8]);
			mModelNormals.push_back(mModel[mModelIndices[i + j] * 8 + 1]);
			mModelNormals.push_back(mModel[mModelIndices[i + j] * 8 + 2]);
			mModelNormals.push_back(mModel[mModelIndices[i + j] * 8]     + mModelVertexNormal[k].x/2);
			mModelNormals.push_back(mModel[mModelIndices[i + j] * 8 + 1] + mModelVertexNormal[k].y/2);
			mModelNormals.push_back(mModel[mModelIndices[i + j] * 8 + 2] + mModelVertexNormal[k].z/2);
		}
	}

	size = mModelVertexAveNormal.size();
	//we get the identical points togheter, and we get all the normals that he can get
	float epsilon = 0.01f;
	for (size_t i = 0; i < size; i++)
	{
		//compare all the points
		for (size_t j = i+1; j < size; j++)
		{
			//check the diff that they can have coz precision problems
			float XDiff = std::abs(mModel[i * 8] - mModel[j * 8]);
			float YDiff = std::abs(mModel[i * 8 + 1] - mModel[j * 8 + 1]);
			float ZDiff = std::abs(mModel[i * 8 + 2] - mModel[j * 8 + 2]);
			//if they are considered the saame point
			if (XDiff < epsilon && YDiff < epsilon && ZDiff < epsilon)
			{
				//copy all the normals that he have
				size_t length = mModelVertexAveNormal[j].size();
				for (size_t k = 0; k < length; k++)
				{
					mModelVertexAveNormal[i].push_back(mModelVertexAveNormal[j][k]);
				}
				mModelVertexAveNormal[j].clear();
			}
		}
	}

	for (size_t i = 0; i < size; i++)
	{
		//first we will clear any duplicates, not needed for now 
		auto last = std::unique(mModelVertexAveNormal[i].begin(), mModelVertexAveNormal[i].end());
		mModelVertexAveNormal[i].erase(last, mModelVertexAveNormal[i].end());

		size_t length = mModelVertexAveNormal[i].size();
		//we sum all the normals that we found in the point
		glm::vec3 AverageNormal(0.0f, 0.0f, 0.0f);
		for (size_t j = 0; j < length; j++)
		{
			AverageNormal.x += mModelVertexAveNormal[i][j].x;
			AverageNormal.y += mModelVertexAveNormal[i][j].y;
			AverageNormal.z += mModelVertexAveNormal[i][j].z;
		}
		//we divide by all the normlas found and normalize it
		AverageNormal /= length;
		AverageNormal = glm::normalize(AverageNormal);
		mAveNormal.push_back(AverageNormal);
		AverageNormal /= 2;

		//we save them to put the lines if the users wants
		mModelAverageNormals.push_back(mModel[i * 8]);
		mModelAverageNormals.push_back(mModel[i * 8 + 1]);
		mModelAverageNormals.push_back(mModel[i * 8 + 2]);

		mModelAverageNormals.push_back(mModel[i * 8]     + AverageNormal.x);
		mModelAverageNormals.push_back(mModel[i * 8 + 1] + AverageNormal.y);
		mModelAverageNormals.push_back(mModel[i * 8 + 2] + AverageNormal.z);

	}

	//put all the averages in each vertex
	for (size_t i = 0; i < size; i++)
	{
		//compare all the points
		for (size_t j = i + 1; j < size; j++)
		{
			//check the diff that they can have coz precision problems
			float XDiff = std::abs(mModel[i * 8] - mModel[j * 8]);
			float YDiff = std::abs(mModel[i * 8 + 1] - mModel[j * 8 + 1]);
			float ZDiff = std::abs(mModel[i * 8 + 2] - mModel[j * 8 + 2]);
			//if they are considered the saame point
			if (XDiff < epsilon && YDiff < epsilon && ZDiff < epsilon)
			{
				mAveNormal[j] = mAveNormal[i];
			}
		}
	}
}

///
// Brief description: Function we will use alone the creations of the models
///
inline void Model::PushPointToModel(float x, float y, float z, float w)
{
	mModel.push_back(x);
	mModel.push_back(y);
	mModel.push_back(z);
	mModel.push_back(w);
	//push the color that we can use to render de model 
	for (int j = 0; j < 4; j++)
		mModel.push_back(mRed[j]);
}
