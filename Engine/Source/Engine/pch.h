#pragma once

#define SAFE_DELETE(aPointer) if (aPointer) { delete aPointer; aPointer = nullptr; }

#include <array>
#include <vector>
#include <string>
#include <assert.h>
#include <d3d11.h>

#include "Vector.h"
#include "Matrix.h"
#include "Transform.h"
#include "Mathcu.hpp"
#include "InputHandler.h"

#include "nlohmann/json.hpp"

#include "imgui-docking/imgui.h"
#include "imgui-docking/imgui_impl_dx11.h"
#include "imgui-docking/imgui_impl_win32.h"

#include "Engine.h"
#include "ErrorManager.h"

namespace Utils
{
	static const float PI = atanf(1.f) * 4.f;
}