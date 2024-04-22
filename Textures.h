#pragma once

#include <d3d11.h>
#include <D3DX11tex.h>

#include <windows.h>
#include <string> 

#include "Thirdparty/Dear ImGui/imgui.h"

#include "Resources/Textures/icon_ak47.h"
#include "Resources/Textures/icon_deagle.h"
#include "Resources/Textures/icon_minigun.h"
#include "Resources/Textures/icon_mp5.h"
#include "Resources/Textures/icon_shotgun.h"
#include "Resources/Textures/icon_sniper.h"

namespace Textures
{
	void Init(ID3D11Device* pDevice);
	void Unload();

	inline ID3D11ShaderResourceView* tHandgun;
	inline ID3D11ShaderResourceView* tShotgun;
	inline ID3D11ShaderResourceView* tSMGs;
	inline ID3D11ShaderResourceView* tARs;
	inline ID3D11ShaderResourceView* tRifles;
	inline ID3D11ShaderResourceView* tHeavy;

	inline D3DX11_IMAGE_LOAD_INFO info0;
	inline ID3DX11ThreadPump* pump0;
};