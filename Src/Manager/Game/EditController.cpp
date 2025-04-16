#include "../System/InputManager.h"
#include "EditController.h"

void EditController::Init(void)
{
}

void EditController::Update(void)
{
	mousePos2D_ = InputManager::GetInstance().GetMousePos();
	ChengePos2Dfor3D();

}

void EditController::Draw(void)
{
}

void EditController::Release(void)
{
}

void EditController::ChengePos2Dfor3D(void)
{

}
