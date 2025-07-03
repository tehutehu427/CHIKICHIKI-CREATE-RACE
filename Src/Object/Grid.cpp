#include <DxLib.h>
#include "../Manager/Game/MapEditer.h"
#include "../Utility/Utility.h"
#include "Grid.h"

Grid::Grid(void)
{
}

Grid::~Grid(void)
{
}

void Grid::Init(void)
{
}

void Grid::Update(void)
{

}

void Grid::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);	//îºìßñæÇ…ê›íË
	// Åyó˚èKÅzç≈èâÇÃÇPñ{
	//VECTOR sPos = { 0.0f, 0.0f, 0.0f };
	//VECTOR ePos = { HLEN, 0.0f, 0.0f };
	//VECTOR sPos = { -HLEN, 0.0f, 0.0f };
	//VECTOR ePos = {  HLEN, 0.0f, 0.0f };
	//DrawLine3D(sPos, ePos, 0xff0000);

	// XZäÓñ{é≤(ÉOÉäÉbÉh)
	VECTOR sPos;
	VECTOR ePos;
	IntVector3 lineNum = MapEditer::MAP_SIZE;
	int size = MapEditer::GRID_SIZE;
	for (int i = 0; i <= lineNum.x; i++)
	{
		for (int j = 0; j <= lineNum.y; j++)
		{
			sPos = {static_cast<float>( i * size),static_cast<float>(j * size),0 };
			ePos = {static_cast<float>( i * size),static_cast<float>(j * size),static_cast<float>(lineNum.z * size) };

			DrawLine3D(sPos, ePos,Utility::GRAY);
			//Utility::DrawPointLine3D(sPos, ePos, Utility::GRAY);
		}
	}
	for (int i = 0; i <= lineNum.x; i++)
	{
		for (int j = 0; j <= lineNum.z; j++)
		{
			sPos = {static_cast<float>( i * size),0,static_cast<float>(j * size) };
			ePos = {static_cast<float>( i * size),static_cast<float>(lineNum.y * size),static_cast<float>(j * size) };

			DrawLine3D(sPos, ePos, Utility::GRAY);
			//Utility::DrawPointLine3D(sPos, ePos, Utility::GRAY);
		}
	}
	for (int i = 0; i <= lineNum.y; i++)
	{
		for (int j = 0; j <= lineNum.z; j++)
		{
			sPos = {0,static_cast<float>( i * size),static_cast<float>(j * size) };
			ePos = {static_cast<float>(lineNum.x * size),static_cast<float>( i * size),static_cast<float>(j * size) };

			DrawLine3D(sPos, ePos, Utility::GRAY);
			//Utility::DrawPointLine3D(sPos, ePos, Utility::GRAY);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ï`âÊÉÇÅ[ÉhÇå≥Ç…ñﬂÇ∑
}

void Grid::Release(void)
{
}
