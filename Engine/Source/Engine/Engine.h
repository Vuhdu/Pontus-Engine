#pragma once
#include "resource.h"

class CGraphicsEngine;

class CCamera;
class CSpotLight;
class CModelInstance;

struct SCreateParameters;

namespace CommonUtilities 
{
	class InputHandler;
	class Timer;
}
namespace CU = CommonUtilities;

namespace Editor
{
	class CEditor;
}

class CEngine 
{
public:
	CEngine() = default;
	~CEngine() = default;

	bool Init(SCreateParameters* someCreateParameters);
	bool Run();
	void Destroy();

private:
	void InitConsole();
	void CloseConsole();

private:
	bool myEngineIsRunning = true;

	SCreateParameters* myCreateParameters = nullptr;

	Editor::CEditor* myEditor = nullptr;
	CGraphicsEngine* myGraphicsEngine = nullptr;

	CU::InputHandler* myInput = nullptr;
	CU::Timer* myTimer = nullptr;

	CCamera* myEditorCamera = nullptr;
	CCamera* myMainCamera = nullptr;

	CSpotLight* mySpotLight = nullptr;
	CModelInstance* myHead = nullptr;
	CModelInstance* myHead2 = nullptr;
};