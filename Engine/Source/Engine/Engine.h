#pragma once
#include "..\CommonUtilities\Vector4.hpp"

struct SCreateParameters;
class CGraphicsEngine;
class CForwardRenderer;
class CDirectX11Framework;

class CModelFactory;
class CCameraFactory;
class CLightFactory;
class CErrorManager;
class CModelHandler;
class CScene;

class CCamera;
class CSpotLight;
class CModelInstance;

enum class eRenderMode;

namespace CommonUtilities 
{
	class Timer;
	class InputHandler;
}
namespace CU = CommonUtilities;

namespace Editor
{
	class CEditor;
}

class CEngine 
{
private:
	CEngine();
	~CEngine();

	// Singleton Getters/Setters
public:
	static bool Start(SCreateParameters* someCreateParameters);
	static bool Run();

	static CErrorManager*		GetErrorManager() { return myInstance->myErrorManager; };
	static CModelFactory*		GetModelFactory() { return myInstance->myModelFactory; };
	static CCameraFactory*		GetCameraFactory() { return myInstance->myCameraFactory; };
	static CLightFactory*		GetLightFactory() { return myInstance->myLightFactory; };
	static CModelHandler*		GetModelHandler() { return myInstance->myModelHandler; };
	static CScene*				GetScene() { return myInstance->myScene; };

	static CU::InputHandler*	GetInput() { return myInstance->myInput; };
	static CU::Timer*			GetTimer() { return myInstance->myTimer; };

	static void					SetRenderMode(const eRenderMode& aRenderMode);

	static void					SetClearColor(const CU::Vector4f& aClearColor) { myInstance->myClearColor = aClearColor; };
	static const				CU::Vector4f& GetClearColor() { return myInstance->myClearColor; };

	static CEngine* GetInstance();

private:
	bool InternalStart(SCreateParameters* someCreateParameters);
	bool InternalRun();
	void InitConsole();
	void CloseConsole();

private:
	inline static CEngine* myInstance;
	bool myEngineIsRunning = true;

	SCreateParameters* myCreateParameters = nullptr;
	CGraphicsEngine* myGraphicsEngine = nullptr;
	Editor::CEditor* myEditor = nullptr;

private:
	CU::Vector4f myClearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
	
	CU::InputHandler* myInput = nullptr;
	CU::Timer* myTimer = nullptr;

	CModelFactory* myModelFactory = nullptr;
	CCameraFactory* myCameraFactory = nullptr;
	CLightFactory* myLightFactory = nullptr;
	
	CErrorManager* myErrorManager = nullptr;

	CModelHandler* myModelHandler = nullptr;

	CScene* myScene = nullptr;

	CDirectX11Framework* myFramework = nullptr;


	// Move this to game and editor
private:
	CCamera* myEditorCamera = nullptr;
	CCamera* myMainCamera = nullptr;
	
	CSpotLight* mySpotLight = nullptr;
	CModelInstance* myHead = nullptr;
	CModelInstance* myHead2 = nullptr;

};