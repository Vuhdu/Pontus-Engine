#pragma once
#include <functional>
#include <array>
#include <string>

struct SCreateParameters
{
	std::function<void(void)> InitCallback;
	std::function<void(void)> UpdateCallback;

	std::array<int, 2> WindowResolution = { 1280, 720 };
	std::array<int, 2> WindowPosition = { 100, 100 };

	std::array<float, 4> ClearColor = { 0.25f, 0.5f, 1.0f, 1.0f };

	std::wstring ApplicationName = L"My Awesome Game";

	bool UseEditorInterface = false; // TO-DO: Fix a working toggle for editor
};

