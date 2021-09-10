#pragma once
#include "..\CommonUtilities\Vector.h"

struct SCreateParameters;
class CGraphicsEngine;
class CForwardRenderer;
class CDirectX11Framework;

class CModelFactory;
class CCameraFactory;
class CLightFactory;
class CFullscreenTextureFactory;

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

public:
	static bool Start(SCreateParameters* someCreateParameters);
	static bool Run();

	static CErrorManager*				GetErrorManager() { return myInstance->myErrorManager; };
	static CModelFactory*				GetModelFactory() { return myInstance->myModelFactory; };
	static CCameraFactory*				GetCameraFactory() { return myInstance->myCameraFactory; };
	static CLightFactory*				GetLightFactory() { return myInstance->myLightFactory; };
	static CFullscreenTextureFactory*	GetFullscreenTextureFactory() { return myInstance->myFullscreenTextureFactory; };
	static CModelHandler*				GetModelHandler() { return myInstance->myModelHandler; };
	static CScene*						GetScene() { return myInstance->myScene; };

	static CU::InputHandler*	GetInput() { return myInstance->myInput; };
	static CU::Timer*			GetTimer() { return myInstance->myTimer; };

	static CDirectX11Framework* GetFramework() { return myInstance->myFramework; };

	static void					SetRenderMode(const eRenderMode& aRenderMode);

	static void					SetClearColor(const CU::Vector4f& aClearColor) { myInstance->myClearColor = aClearColor; };
	static const				CU::Vector4f& GetClearColor() { return myInstance->myClearColor; };

	static const				CU::Vector2ui& GetResolution() { return myInstance->myResolution; };
	static const				void ToggleEditor(const bool aToggle) { myInstance->myWillUseEditor = aToggle; };
	static const				bool IsUsingEditor() { return myInstance->myWillUseEditor; };

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
	CU::Vector2ui myResolution = { 1280, 720 };
	CU::Vector4f myClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	CU::InputHandler* myInput = nullptr;
	CU::Timer* myTimer = nullptr;

	CModelFactory* myModelFactory = nullptr;
	CCameraFactory* myCameraFactory = nullptr;
	CLightFactory* myLightFactory = nullptr;
	CFullscreenTextureFactory* myFullscreenTextureFactory = nullptr;
	
	CErrorManager* myErrorManager = nullptr;

	CModelHandler* myModelHandler = nullptr;

	CScene* myScene = nullptr;

	CDirectX11Framework* myFramework = nullptr;

	bool myWillUseEditor = false;

	// Move this to game and editor
private:
	CCamera* myEditorCamera = nullptr;
	CCamera* myMainCamera = nullptr;
	
	CSpotLight* mySpotLight = nullptr;

};