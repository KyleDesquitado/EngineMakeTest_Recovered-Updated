#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Matrix4x4.h"
#include "InputListener.h"
#include "Cube.h"
#include <vector>

class AppWindow : public Window, public InputListener
{
public:
	AppWindow();

	void updateQuadPosition();

	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;

	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;

	void OnScaleX(int dir);
	void OnScaleY(int dir);
	void OnScaleZ(int dir);
	void OnTranslateX(int dir);
	void OnTranslateY(int dir);
	void OnTranslateZ(int dir);
	void OnRotateX(float dir);
	void OnRotateY(float dir);
	void OnRotateZ(float dir);
	void OnRotateX(int dir);
	void OnRotateY(int dir);
	void OnRotateZ(int dir);
private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vb;
	VertexShader* m_vs;
	PixelShader* m_ps;
	ConstantBuffer* m_cb;
	IndexBuffer* m_ib;
private:
	long m_old_delta;
	long m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;
	float m_delta_rot;

	float m_rot_x = 0.0f;
	float m_rot_y = 0.0f;

	float m_scale_cube = 1;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;

	float xScaleValue = 1;
	float yScaleValue = 1;
	float zScaleValue = 1;
	float xTranslateValue = 0;
	float yTranslateValue = 0;
	float zTranslateValue = 0;
	float xRotateValue = 0;
	float yRotateValue = 0;
	float zRotateValue = 0;


	Matrix4x4 cubeMatrix;
	Matrix4x4 m_world_cam;
	std::vector<Cube*> cubeList;
	

};