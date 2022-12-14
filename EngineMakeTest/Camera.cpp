#include "Camera.h"
#include "InputSystem.h"
#include "EngineTime.h"

Camera::Camera(int id) : AGameObject(id)
{
	//preset camera values
	this->setPosition(0.0f, 0.0f, -2.0f);
	this->localMatrix.setTranslation(this->localPosition);

	InputSystem::get()->addListener(this);
}

Camera::~Camera()
{
	InputSystem::get()->removeListener(this);
}

void Camera::update(float deltaTime)
{
	//// keu input for camerae
	//if (InputSystem::get()->isKeyDown('W'))
	//{
	//	forward = 1; //forward
	//}
	//else if (InputSystem::get()->isKeyDown('S'))
	//{
	//	forward = -1; //backwards
	//}
	//else if (InputSystem::getInstance()->isKeyDown('A'))
	//{
	//	rightward = -1; //sideward (left)
	//}
	//else if (InputSystem::getInstance()->isKeyDown('D'))
	//{
	//	rightward = 1; //sidewards (right)
	//}
	//else if (InputSystem::getInstance()->isKeyDown('E'))
	//{
	//	upward = 1; //upwards
	//}
	//else if (InputSystem::getInstance()->isKeyDown('Q'))
	//{
	//	upward = -1;//downwards
	//}

	updateViewMatrix();
}

Matrix4x4 Camera::getViewMatrix()
{
	return this->localMatrix;
}

Matrix4x4 Camera::getWorldCamMatrix()
{
	return this->world_cam;
}

float Camera::getForward()
{
	return m_forward;
}

float Camera::getRightward()
{
	return m_rightward;
}

float Camera::getUpward()
{
	return upward;
}

void Camera::onKeyDown(int key)
{
	//std::cout << "Camera";
	if (key == 'W')
	{
		//m_rot_x += 3.14f*m_delta_time;
		m_forward = 1.0f;
	}
	else if (key == 'S')
	{
		//m_rot_x -= 3.14f*m_delta_time;
		m_forward = -1.0f;
	}
	else if (key == 'A')
	{
		//m_rot_y += 3.14f*m_delta_time;
		m_rightward = -1.0f;
	}
	else if (key == 'D')
	{
		//m_rot_y -= 3.14f*m_delta_time;
		m_rightward = 1.0f;
	}

	//if (key == 'W')
	//{
	//	//cube[0]->setRotation(Vector3D( v.m_x+ 3.14f * EngineTime::getDeltaTime(),v.m_y,v.m_z));
	//	forward = 1.0f;
	//}
	//else if (key == 'S')
	//{
	//	//cube[0]->setRotation(Vector3D(v.m_x - 3.14f * EngineTime::getDeltaTime(), v.m_y, v.m_z));
	//	forward = -1.0f;

	//}
	//else if (key == 'A')
	//{
	//	//cube[0]->setRotation(Vector3D(v.m_x, v.m_y + 3.14 *EngineTime::getDeltaTime(), v.m_z));
	//	rightward = -1.0f;

	//}
	//else if (key == 'D')
	//{
	//	//cube[0]->setRotation(Vector3D(v.m_x, v.m_y - 3.14 * EngineTime::getDeltaTime(), v.m_z));
	//	 rightward= 1.0f;

	//}
}

void Camera::onKeyUp(int key)
{
	m_forward = 0;
	m_rightward = 0;
	upward = 0;
}

void Camera::onMouseMove(const Point& deltaPos)
{
	int width = 1004;
	int height = 725;



	localRotation.m_x += (deltaPos.m_y - (height / 2.0f)) *0.01f;
	localRotation.m_y += (deltaPos.m_x - (width / 2.0f)) * 0.01f;



	//InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));

	if (this->mouseDown)
	{
		Vector3D v = this->getLocalRotation();
		// may add speed factor
		v.m_x += (deltaPos.m_y * EngineTime::getDeltaTime() * 0.1f);
		v.m_y += (deltaPos.m_x * EngineTime::getDeltaTime() * 0.1f);

		// update rotation
		this->setRotation(v);

		// centering mouse pos
		//InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));
	}
}

void Camera::onLeftMouseDown(const Point& deltaPos)
{
}

void Camera::onLeftMouseUp(const Point& deltaPos)
{
}

void Camera::onRightMouseDown(const Point& deltaPos)
{
	this->mouseDown = true;
}

void Camera::onRightMouseUp(const Point& deltaPos)
{
	this->mouseDown = false;
}

void Camera::updateViewMatrix()
{
	constant cc;
	Matrix4x4 temp;

	//UPDATE FOR CAM ROTATION
	world_cam.setIdentity();
	temp.setIdentity();
	temp.setRotationX(this->getLocalRotation().m_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(this->getLocalRotation().m_y);
	world_cam *= temp;


	// UPDATE FOR CAM TRANSLATION
	Vector3D new_pos = this->localMatrix.getTranslation() + (world_cam.getZDirection() * (this->m_forward * 0.1f));
	//new_pos = new_pos + (world_cam.get() * (this->upward * 0.1f));
	new_pos = new_pos + (world_cam.getXDirection() * (this->m_rightward * 0.1f));

	world_cam.setTranslation(new_pos);
	world_cam.setTranslation(new_pos);

	//update local matrix
	this->localMatrix = world_cam;

	// application
	world_cam.inverse();

	//std::cout << "x: "<<this->getLocalPosition().m_x <<" y: "<< this->getLocalPosition().m_y<<" z: "<<this->getLocalPosition().m_z <<"\n";
}

void Camera::draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader)
{
}
