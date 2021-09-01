#pragma once

#include "Engine.h"
#include "ErrorManager.h"

#include "Vector.h"
#include "Matrix.h"
#include "Mathcu.hpp"
#include "InputHandler.h"

#include <array>
#include <vector>
#include <string>
#include <assert.h>

#include "nlohmann/json.hpp"

#include "imgui-docking/imgui.h"
#include "imgui-docking/imgui_impl_dx11.h"
#include "imgui-docking/imgui_impl_win32.h"

#define SAFE_DELETE(aPointer) if (aPointer) { delete aPointer; aPointer = nullptr; }
