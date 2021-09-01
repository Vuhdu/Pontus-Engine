#pragma once

namespace Editor 
{
	class CEditorInterface;

	class CEditor 
	{
	public:
		CEditor() = default;
		~CEditor() = default;

		bool Init();

		void Update();

		void Destroy();

	private:
		CEditorInterface* myEditorInterface = nullptr;

	};
}