#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "InputSystem.h"
#include "EngineTime.h"
#include "SceneCameraHandler.h"

struct vertex
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};


__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};


AppWindow::AppWindow()
{
}

void AppWindow::updateQuadPosition()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;
	if (m_delta_pos > 1.0f)
		m_delta_pos = 0;


	Matrix4x4 temp;

	m_delta_scale += m_delta_time / 0.55f;

	//cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));

	//temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f,1.5f, 0), m_delta_pos));

	//cc.m_world *= temp;

	/*cc.m_world.setScale(Vector3D(m_scale_cube, m_scale_cube, m_scale_cube));
	temp.setIdentity();
	temp.setRotationZ(0.0f);
	cc.m_world *= temp;
	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	cc.m_world *= temp;
	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	cc.m_world *= temp;*/

	cc.m_world.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setRotationX(m_rot_x);
	world_cam *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rot_y);
	world_cam *= temp;


	/*Vector3D new_pos = m_world_cam.getTranslation() + world_cam.getZDirection() * (m_forward * 0.1f);

	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 0.1f);
	

	world_cam.setTranslation(new_pos);

	m_world_cam = world_cam;

	

	world_cam.inverse();*/


	//cc.m_view = world_cam;

	cc.m_view = SceneCameraHandler::getInstance()->getSceneCameraWorldCamMatrix();
	/*cc.m_proj.setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left)/300.0f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top)/300.0f,
		-4.0f,
		4.0f
	);*/

	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);


	cc.m_proj.setPerspectiveFovLH(1.57f, ((float)width / (float)height), 0.1f, 100.0f);


	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}


AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	EngineTime::initialize();
	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);
	SceneCameraHandler::initialize();

	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_world_cam.setTranslation(Vector3D(0, 0, -2));

	//vertex vertex_list[] =
	//{
	//	//X - Y - Z
	//	//FRONT FACE
	//	{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0),  Vector3D(0.2f,0,0) },
	//	{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,0), Vector3D(0.2f,0.2f,0) },
	//	{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
	//	{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },

	//	//BACK FACE
	//	{ Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0), Vector3D(0,0.2f,0) },
	//	{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(0,1,1), Vector3D(0,0.2f,0.2f) },
	//	{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
	//	{ Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,1,0), Vector3D(0,0.2f,0) }

	//};

	m_vb = GraphicsEngine::get()->createVertexBuffer();
	//UINT size_list = ARRAYSIZE(vertex_list);


	//unsigned int index_list[] =
	//{
	//	//FRONT SIDE
	//	0,1,2,  //FIRST TRIANGLE
	//	2,3,0,  //SECOND TRIANGLE
	//	//BACK SIDE
	//	4,5,6,
	//	6,7,4,
	//	//TOP SIDE
	//	1,6,5,
	//	5,2,1,
	//	//BOTTOM SIDE
	//	7,0,3,
	//	3,4,7,
	//	//RIGHT SIDE
	//	3,2,5,
	//	5,4,3,
	//	//LEFT SIDE
	//	7,6,1,
	//	1,0,7
	//};


	m_ib = GraphicsEngine::get()->createIndexBuffer();
	//UINT size_index_list = ARRAYSIZE(index_list);

	m_ib->load(NULL, 0);




	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);

	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);
	m_vb->load(NULL, sizeof(vertex), 0, shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);
	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));


	//for (int i = 0; i < 1; i++)
	//{
	//	/*float randx = -5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5.0f - (-5.0f))));
	//	float randy = -5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5.0f - (-5.0f))));
	//	float randz = -5.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5.0f - (-5.0f))));*/
	//	Cube* newCube = new Cube(0, shader_byte_code, size_shader);
	//	newCube->setPosition(0, 0, 0);
	//	newCube->setScale(0.5f, 0.5f, 0.5f);
	//	cubeList.push_back(newCube);
	//}

	Cube* newCube;
	//newCube = new Cube(0, shader_byte_code, size_shader);
	//newCube->setPosition(-0.75, 0, 0);
	//newCube->setScale(0.007f, 0.5f, 0.5f);
	//newCube->setRotation(-0.5f, 0.0f, 0.0f);
	//cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(-0.375, 0, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(-0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(-0.250, 0, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(-0.125f, 0, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(-0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(0.0, 0, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(0.125, 0, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(-0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(0.250, 0, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(0.25, 0.25, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(1.55f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(-0.25, 0.25, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(1.6f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(-0.250, 0.5f, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(-0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(-0.125f, 0.5f, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(0.0, 0.5f, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(-0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(0.125, 0.5f, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(-0.0625f, 0.75, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(1.55f, 0.0f, 0.0f);
	cubeList.push_back(newCube);
	
	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(-0.125f, 1.0f, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(-0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);

	newCube = new Cube(0, shader_byte_code, size_shader);
	newCube->setPosition(0.0, 1.0f, 0);
	newCube->setScale(0.0085f, 0.5f, 0.35f);
	newCube->setRotation(0.25f, 0.0f, 0.0f);
	cubeList.push_back(newCube);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();
	SceneCameraHandler::getInstance()->update();
	//CLEAR THE RENDER TARGET 
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		0, 0.3f, 0.4f, 1);
	//SET VIEWPORT OF RENDER TARGET IN WHICH WE HAVE TO DRAW
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);




	updateQuadPosition();




	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);


	//SET THE VERTICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vb);
	//SET THE INDICES OF THE TRIANGLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_ib);


	// FINALLY DRAW THE TRIANGLE
	//GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);

	
	for (int i = 0; i < cubeList.size(); i++)
	{
		if (zScaleValue > 0.01f)
		{
			zScaleValue = max(0.1f, zScaleValue - EngineTime::getDeltaTime());
		}

		if (xScaleValue < 1.0f)
		{
			xScaleValue = min(1.0f, xScaleValue + EngineTime::getDeltaTime());
		}

		if (yScaleValue < 1.0f)
		{
			yScaleValue = min(1.0f, yScaleValue + EngineTime::getDeltaTime());
		}
		cubeList[i]->m_forward = m_forward;
		cubeList[i]->m_rightward = m_rightward;
		cubeList[i]->m_world_cam = m_world_cam;
		//cubeList[i]->setPosition(Vector3D(xTranslateValue, yTranslateValue, 0.0f));
		//cubeList[i]->setScale(Vector3D::lerp(Vector3D(1, 1, 0.001f), Vector3D(0.5, 0.5f, 0.5f), (sin(m_delta_scale) + 1.0f) / 2.0f));
		//cubeList[i]->setScale((Vector3D(xScaleValue, yScaleValue, zScaleValue)));

		/*OnRotateX(0.2f);
		OnRotateY(0.2f);
		OnRotateZ(0.2f);
		*/
		//cubeList[i]->setRotation(xRotateValue, yRotateValue, zRotateValue);
		cubeList[i]->update(EngineTime::getDeltaTime());
		cubeList[i]->draw(
			(this->getClientWindowRect().right - this->getClientWindowRect().left),
			(this->getClientWindowRect().bottom - this->getClientWindowRect().top),
			this->m_vs,
			this->m_ps
			);
	}
	m_swap_chain->present(true);


	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vb->release();
	m_ib->release();
	m_cb->release();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	GraphicsEngine::get()->release();
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onKeyDown(int key)
{
	//if (key == 'W')
	//{
	//	//m_rot_x += 3.14f*m_delta_time;
	//	m_forward = 1.0f;
	//}
	//else if (key == 'S')
	//{
	//	//m_rot_x -= 3.14f*m_delta_time;
	//	m_forward = -1.0f;
	//}
	//else if (key == 'A')
	//{
	//	//m_rot_y += 3.14f*m_delta_time;
	//	m_rightward = -1.0f;
	//}
	//else if (key == 'D')
	//{
	//	//m_rot_y -= 3.14f*m_delta_time;
	//	m_rightward = 1.0f;
	//}


	if (key == 'I')
	{
		//m_rot_x += 3.14f*m_delta_time;
		OnTranslateY(1);
	}
	else if (key == 'J')
	{
		//m_rot_x -= 3.14f*m_delta_time;
		OnTranslateX(-1);
	}
	else if (key == 'K')
	{
		//m_rot_y += 3.14f*m_delta_time;
		OnTranslateY(-1);
	}
	else if (key == 'L')
	{
		//m_rot_y -= 3.14f*m_delta_time;
		OnTranslateX(1);
	}

}

void AppWindow::onKeyUp(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void AppWindow::onMouseMove(const Point& mouse_pos)
{
	int width = (this->getClientWindowRect().right - this->getClientWindowRect().left);
	int height = (this->getClientWindowRect().bottom - this->getClientWindowRect().top);



	m_rot_x += (mouse_pos.m_y - (height / 2.0f)) * m_delta_time * 0.1f;
	m_rot_y += (mouse_pos.m_x - (width / 2.0f)) * m_delta_time * 0.1f;
	std::cout << width << " " << height << std::endl;


	InputSystem::get()->setCursorPosition(Point((int)(width / 2.0f), (int)(height / 2.0f)));


}

void AppWindow::onLeftMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 0.5f;
}

void AppWindow::onLeftMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}

void AppWindow::onRightMouseDown(const Point& mouse_pos)
{
	m_scale_cube = 2.0f;
}

void AppWindow::onRightMouseUp(const Point& mouse_pos)
{
	m_scale_cube = 1.0f;
}
void AppWindow::OnScaleX(int dir)
{
	this->xScaleValue += dir * m_delta_time;
}

void AppWindow::OnScaleY(int dir)
{
	yScaleValue += dir * m_delta_time;
}

void AppWindow::OnScaleZ(int dir)
{
	zScaleValue += dir * m_delta_time;
}

void AppWindow::OnTranslateX(int dir)
{
	this->xTranslateValue += dir * EngineTime::getDeltaTime();
	
}

void AppWindow::OnTranslateY(int dir)
{
	yTranslateValue += dir * EngineTime::getDeltaTime();
}

void AppWindow::OnTranslateZ(int dir)
{
	zTranslateValue += dir * EngineTime::getDeltaTime();
}

void AppWindow::OnRotateX(float dir)
{
	this->xRotateValue += dir * EngineTime::getDeltaTime();;
}

void AppWindow::OnRotateY(float dir)
{
	yRotateValue += dir * EngineTime::getDeltaTime();;
}

void AppWindow::OnRotateZ(float dir)
{
	zRotateValue += dir * EngineTime::getDeltaTime();;

}