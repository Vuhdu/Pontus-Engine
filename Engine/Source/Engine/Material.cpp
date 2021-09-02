#include "pch.h"
#include "Material.h"

#include <d3d11.h>

bool CMaterial::Init(unsigned int anID, ID3D11VertexShader* aVertexShader, ID3D11PixelShader* aPixelShader)
{
    myID = anID;
    myVertexShader = aVertexShader;
    myPixelShader = aPixelShader;
    
    return true;
}
