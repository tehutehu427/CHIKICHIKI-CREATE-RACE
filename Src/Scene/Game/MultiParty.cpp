#include "MultiParty.h"

MultiParty::MultiParty(void)
{
}

MultiParty::~MultiParty(void)
{
}

void MultiParty::Load(void)
{
	//親クラスの読み込み処理を呼ぶ
	GameScene::Load();
}

void MultiParty::Init(void)
{
	//親クラスの初期化処理を呼ぶ
	GameScene::Init();
}

void MultiParty::NormalDraw(void)
{
	//親クラスの描画処理を呼ぶ
	GameScene::NormalDraw();
}

void MultiParty::UpdateAction(void)
{
	//親クラスのアクション時の更新処理を呼ぶ
	GameScene::UpdateAction();
}

void MultiParty::UpdateEdit(void)
{
	//親クラスのエディット時の更新処理を呼ぶ
	GameScene::UpdateEdit();
}
