// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <vector>
#include <string>
#include <array>

#include "MainSingleton.h"
#include "ErrorManager.h"

#include "Vector.h"
#include "Matrix.h"
#include "Mathcu.hpp"

#include "imgui-docking/imgui.h"
#include "imgui-docking/imgui_impl_dx11.h"
#include "imgui-docking/imgui_impl_win32.h"

#define SAFE_DELETE(aPointer) if (aPointer) { delete aPointer; aPointer = nullptr; }

#endif //PCH_H
