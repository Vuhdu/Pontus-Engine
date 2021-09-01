#pragma once
#include "Viewport.h"

class CDirectX11Framework;
class CWindowHandler;

namespace Editor
{
	class CGameViewport : public CViewport
	{
	public:
		CGameViewport(const std::string& aWindowName) : CViewport(aWindowName) { };
		~CGameViewport() = default;

		void Draw() override;

	private:

	};
}