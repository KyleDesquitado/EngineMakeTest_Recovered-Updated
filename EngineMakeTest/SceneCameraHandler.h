#pragma once
#include "Camera.h"
class SceneCameraHandler
{
public:
	static SceneCameraHandler* getInstance();
	static void initialize();
	static void destroy();

	void update();

	Matrix4x4 getSceneCameraViewMatrix();
	Matrix4x4 getSceneCameraWorldCamMatrix();
	Camera* getSceneCamera();

private:
	SceneCameraHandler();
	~SceneCameraHandler();
	SceneCameraHandler(SceneCameraHandler const&) {};// copy constructor is private
	SceneCameraHandler& operator=(SceneCameraHandler const&) {};// assignment operator is private*/
	static SceneCameraHandler* sharedInstance;

	Camera* sceneCamera;
};

