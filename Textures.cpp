#include "StdAfx.h"
#include "Textures.h"

void Textures::Init(ID3D11Device* pDevice)
{
	pump0 = nullptr;


	tHandgun = nullptr;
	tShotgun = nullptr;
	tSMGs = nullptr;
	tARs = nullptr;
	tRifles = nullptr;
	tHeavy = nullptr;

	D3DX11CreateShaderResourceViewFromMemory(pDevice, deagle_bytes, sizeof(deagle_bytes), &info0, pump0, &tHandgun, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, shotgun_bytes, sizeof(shotgun_bytes), &info0, pump0, &tShotgun, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, mp5_bytes, sizeof(mp5_bytes), &info0, pump0, &tSMGs, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, ak47_bytes	, sizeof(ak47_bytes), &info0, pump0		, &tARs, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, sniper_bytes, sizeof(sniper_bytes), &info0, pump0, &tRifles, 0);
	D3DX11CreateShaderResourceViewFromMemory(pDevice, minigun_bytes	, sizeof(minigun_bytes), &info0, pump0	, &tHeavy, 0);
}

void Textures::Unload()
{
	tHandgun->Release();
	tShotgun->Release();
	tSMGs->Release();
	tARs->Release();
	tRifles->Release();
	tHeavy->Release();
}
