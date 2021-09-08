#pragma once
class CFullscreenRenderer
{
public:
	CFullscreenRenderer() = default;
	~CFullscreenRenderer() = default;

	enum Shader
	{
		COPY,
		LUMINANCE,
		GAUSSIANHORIZONTAL,
		GAUSSIANVERTICAL,
		BLOOM,
		COUNT
	};

	bool Init();
	void Render(Shader aMode);

private:
	struct ID3D11VertexShader* myVertexShader;
	std::array<struct ID3D11PixelShader*, Shader::COUNT> myPixelShaders;
};

