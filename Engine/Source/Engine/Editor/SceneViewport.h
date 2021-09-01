#pragma once
#include "Viewport.h"

class CDirectX11Framework;
class CWindowHandler;

namespace Editor 
{
	class CSceneViewport : public CViewport
	{
	public:
		CSceneViewport(const std::string& aWindowName) : CViewport(aWindowName) { };
		~CSceneViewport() = default;

		void Draw() override;

	private:

	};
}

