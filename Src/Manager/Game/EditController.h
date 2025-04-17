#pragma once
#include<DxLib.h>
#include "../../Common/Vector2.h"
class EditController
{
public:
	EditController() = default;
	~EditController() = default;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
protected:

private:
	Vector2 mousePos2D_;	//2Dのマウス座標
	VECTOR mousePos3D_;		//3Dのマウス座標
	void ChengePos2Dfor3D(void);	//2Dの座標から3Dの座標に変換する
};

