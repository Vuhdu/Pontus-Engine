#pragma once
#include "Vector4.hpp"

namespace CommonUtilities
{
	class Timer;
	class InputHandler;
}
namespace CU = CommonUtilities;

class CModelFactory;
class CCameraFactory;
class CLightFactory;
class CErrorManager;

class CModelHandler;

class CScene;

class CGameObject;

class CForwardRenderer;
class CDirectX11Framework;

enum class eRenderMode;

class MainSingleton
{
private:
	MainSingleton() = default;
	~MainSingleton();

public:

	static bool			Start();
	static void			Destroy();

	void				InitFramework(CDirectX11Framework* aFramework);

	CErrorManager*		GetErrorManager();

	CModelFactory*		GetModelFactory();
	CCameraFactory*		GetCameraFactory();
	CLightFactory*		GetLightFactory();
	CModelHandler*		GetModelHandler();
	CScene*				GetScene();

	CU::InputHandler*	GetInput();
	CU::Timer*			GetTimer();

	CGameObject*		GetSelectedGameObject();
	void				SetSelectedGameObject(CGameObject* aGameObject);

	void					SetRenderMode(const eRenderMode& aRenderMode);

	void					SetClearColor(const CU::Vector4f& aClearColor);
	const CU::Vector4f&		GetClearColor() const;

public:
	static MainSingleton* GetInstance();

private:
	bool				InternalStart();

private:
	CU::Vector4f myClearColor = { .25f, .5, 1.0f, 1.0f };
	
	CU::InputHandler* myInput = nullptr;
	CU::Timer* myTimer = nullptr;

	CModelFactory* myModelFactory = nullptr;
	CCameraFactory* myCameraFactory = nullptr;
	CLightFactory* myLightFactory = nullptr;

	CErrorManager* myErrorManager = nullptr;

	CModelHandler* myModelHandler = nullptr;

	CScene* myScene = nullptr;

	CDirectX11Framework* myFramework = nullptr;

	CGameObject* mySelectedGameObject = nullptr;

	inline static MainSingleton* myInstance;
};

