#include "Cube.h"
#include "GraphicsEngine.h"
#include "InputSystem.h"
#include "DeviceContext.h"
#include "SceneCameraHandler.h"
#include <iostream>
Cube::Cube(int id, void* shader_byte_code, size_t size_shader):AGameObject(id)
{
	vertex vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1, 1,0),  Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,0), Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0), Vector3D(0,0.2f,0) },
		{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(0,1,1), Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,1,0), Vector3D(0,0.2f,0) }

	};

	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		1,6,5,
		5,2,1,
		//BOTTOM SIDE
		7,0,3,
		3,4,7,
		//RIGHT SIDE
		3,2,5,
		5,4,3,
		//LEFT SIDE
		7,6,1,
		1,0,7
	};


	// INDEX BUFFER
	indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	indexBuffer->load(index_list, ARRAYSIZE(index_list));

	// VERTEX SHADER
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	//vertexShader = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	// VERTEX BUFFER
	vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	vertexBuffer->load(vertex_list, sizeof(vertex), ARRAYSIZE(vertex_list), shader_byte_code, size_shader);
	

	// PIXEL SHADER
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	//pixelShader = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	// CONSTANT BUFFER
	constant cc;
	cc.m_time = 0;
	constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	constantBuffer->load(&cc, sizeof(constant));
}

Cube::~Cube()
{
	this->vertexBuffer->release();
	this->indexBuffer->release();
	AGameObject::~AGameObject();
}

void Cube::update(float deltaTime)
{
	this->deltaTime = deltaTime;

}

void Cube::draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader)
{
	GraphicsEngine* graphicsEngine = GraphicsEngine::get();
	DeviceContext* deviceContext = graphicsEngine->getImmediateDeviceContext();

	constant cbData = {};
	SceneCameraHandler* sch = SceneCameraHandler::getInstance();

	/*Matrix4x4 allMatrix; allMatrix.setIdentity();
	Matrix4x4 translationMatrix; translationMatrix.setTranslation(this->getLocalPosition());
	Matrix4x4 scaleMatrix; scaleMatrix.setScale(this->getLocalScale());
	Vector3D rot = this->getLocalRotation();
	Matrix4x4 xRotMatrix; xRotMatrix.setRotationX(rot.m_x);
	Matrix4x4 yRotMatrix; yRotMatrix.setRotationX(rot.m_y);
	Matrix4x4 zRotMatrix; zRotMatrix.setRotationX(rot.m_z);
	
	allMatrix *= scaleMatrix;

	Matrix4x4 rotMatrix;
	rotMatrix *= zRotMatrix; rotMatrix *= yRotMatrix; rotMatrix *= xRotMatrix;

	allMatrix *= rotMatrix;

	allMatrix *= scaleMatrix;*/

	constant cc;

	Matrix4x4 temp;

	//cc.m_world.setScale(this->getLocalScale());
	//

	//temp.setIdentity();
	//temp.setRotationZ(this->localRotation.m_z);
	//cc.m_world *= temp;
	//temp.setIdentity();
	//temp.setRotationY(this->localRotation.m_y);
	//cc.m_world *= temp;
	//temp.setIdentity();
	//temp.setRotationX(this->localRotation.m_x);
	//cc.m_world *= temp;

	//temp.setTranslation(this->localPosition);
	//cc.m_world *= temp;

	/*cbData.m_world.setScale(this->localScale);

	temp.setIdentity();
	temp.setRotationZ(this->localRotation.m_z);
	cbData.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(this->localRotation.m_y);
	cbData.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(this->localRotation.m_x);
	cbData.m_world *= temp;*/

	temp.setIdentity();

	//TRANSLATION
	Matrix4x4 translationMatrix;
	translationMatrix.setIdentity();
	translationMatrix.setTranslation(this->getLocalPosition());

	//SCALE
	Matrix4x4 scaleMatrix;
	scaleMatrix.setIdentity();
	scaleMatrix.setScale(this->getLocalScale());

	//ROTATION
	Matrix4x4 xMatrix, yMatrix, zMatrix, rotMatrix;

	xMatrix.setIdentity();
	yMatrix.setIdentity();
	zMatrix.setIdentity();
	Vector3D rotation = this->getLocalRotation();

	xMatrix.setRotationZ(rotation.m_x);
	yMatrix.setRotationX(rotation.m_y);
	zMatrix.setRotationY(rotation.m_z);

	rotMatrix.setIdentity();

	rotMatrix *= xMatrix;
	rotMatrix *= yMatrix;
	rotMatrix *= zMatrix;

	// APPLICATION
	temp *= scaleMatrix;
	temp *= rotMatrix;
	temp *= translationMatrix;

	cc.m_world = temp;

	m_world_cam.inverse();
	cc.m_view = sch->getSceneCameraWorldCamMatrix();

	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);

	this->constantBuffer->update(deviceContext, &cc);
	// SET CONSTANT BUFFER
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(vertexShader, constantBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(pixelShader, constantBuffer);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(pixelShader);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(vertexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(indexBuffer);

	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
	//std::cout << id << std::endl;
}

void Cube::setAnimSpeed(float speed)
{
}
