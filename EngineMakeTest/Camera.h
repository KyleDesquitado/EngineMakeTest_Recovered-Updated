#pragma once
#include "AGameObject.h"
#include "InputListener.h"
#include "Vector3D.h"

class Camera : public AGameObject, public InputListener
{
public:
	Camera(int id);
	~Camera();

	void update(float deltaTime) override;
	Matrix4x4 getViewMatrix();
	Matrix4x4 getWorldCamMatrix();

	float getForward();
	float getRightward();
	float getUpward();

	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& deltaPos) override;
	virtual void onLeftMouseDown(const Point& deltaPos) override;
	virtual void onLeftMouseUp(const Point& deltaPos) override;
	virtual void onRightMouseDown(const Point& deltaPos) override;
	virtual void onRightMouseUp(const Point& deltaPos) override;
private:
	void updateViewMatrix();

	Matrix4x4 world_cam;

	float ticks = 0.0f;
	float mouseDown = false;

	float m_forward = 0;
	float m_rightward = 0;
	float upward = 0;


	// Inherited via AGameObject
	virtual void draw(int width, int height, VertexShader* vertexShader, PixelShader* pixelShader) override;

	// Inherited via AGameObject
	//virtual void draw(int width, int height) override;
};

