#pragma once

#include "MainSingleton.h"
#include "ErrorManager.h"

#include "Vector.h"
#include "Matrix.h"
#include "Mathcu.hpp"

#include <array>
#include <vector>
#include <string>
#include <assert.h>

#include "nlohmann/json.hpp"

#define SAFE_DELETE(aPointer) delete aPointer; aPointer = nullptr;
