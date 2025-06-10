#include "../../Utility/Utility.h"
#include "../Application.h"
#include "../../Manager/Game/GravityManager.h"
#include "../../Manager/Game/MapEditer.h"
#include "../../Manager/Game/CollisionManager.h"
#include "../../Manager/System/ResourceManager.h"
#include "../../Manager/System/SceneManager.h"
#include "../../Manager/System/Camera.h"
#include "../../Object/Common/Capsule.h"
#include "../../Object/Common/AnimationController.h"
#include "../../Object/Editor/EditController.h"
#include "../../Manager/Game/ItemManager.h"
#include "./Process/PlayerInput.h"
#include "Player.h"

Player::Player(int _playerNum,PlayerInput::CNTL _cntl):playerNum_(_playerNum), cntl_(_cntl)
{
#ifdef DEBUG_ON
	cubeMovePos_=Utility::VECTOR_ZERO;
	cubePos_=Utility::VECTOR_ZERO;
#endif // DEBUG_ON

	trans_ = Transform();
	movedPos_ = Utility::VECTOR_ZERO;


	//デバッグ
	CollisionManager::CreateInstance();




	//初めのJOYPADがkey_padなのでパッドの番号に合わせる
	if (cntl_ == PlayerInput::CNTL::PAD)
	{
		padNum_ = static_cast<InputManager::JOYPAD_NO>(playerNum_);
	}
	else
	{
		padNum_ = InputManager::JOYPAD_NO::INPUT_KEY;
	}
	
	//オブジェクト生成
	//操作関連
	//---------------------------------
	//当たり判定
	isCol_ = false;

	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility::VECTOR_ZERO;
	jumpDeceralation_ = POW_JUMP;

	//パンチ関係の初期化
	punchCnt_ = 0.0f;
	punchCoolCnt_ = 0.0f;
	isPunch_ = false;
	punchPos_ = Utility::VECTOR_ZERO;
	isPunched_ = false;
	punchedCnt_ = PUNCHED_TIME;
	
	itemLocalPos_ = Utility::VECTOR_ZERO;
	hitItemType_ = ItemBase::ITEM_TYPE::NONE;

	input_ = nullptr;
}

Player::~Player(void)
{
}

void Player::Load(void)
{
	//アニメーションでmodelIdを使うので先にモデルセットする
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::CHICKEN));
	//当たり判定の初期化
	trans_.MakeCollider(Collider::COL_TAG::PLAYER);


	//リソースの読み込みなど
	animationController_ = std::make_shared<AnimationController>(trans_.modelId);
	animationController_->Add(static_cast<int>(ANIM_TYPE::IDLE), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::WALK), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::FALL), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::JUMP), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::LAND), DEFAULT_SPD);
	animationController_->Add(static_cast<int>(ANIM_TYPE::PUNCH), DEFAULT_SPD / PUNCH_TIME_MAX);



	//入力
	input_ = std::make_shared<PlayerInput>(padNum_, cntl_);
}

void Player::Init(void)
{
	//Transformの設定
	trans_.quaRot = Quaternion();
	trans_.scl = MODEL_SCL;
	trans_.quaRotLocal =
		Quaternion::Euler({ 0.0f, Utility::Deg2RadF(180.0f), 0.0f });

	trans_.localPos = { 0.0f,-Player::RADIUS,0.0f };
	//操作関連
	//---------------------------------
	//当たり判定
	isCol_ = false;

	isJump_ = false;
	stepJump_ = 0.0f;
	jumpPow_ = Utility::VECTOR_ZERO;
	jumpDeceralation_ = POW_JUMP;

	//パンチ関係の初期化
	punchCnt_ = 0.0f;
	punchCoolCnt_ = 0.0f;
	isPunch_ = false;
	punchPos_ = Utility::VECTOR_ZERO;
	isPunched_ = false;
	punchedCnt_ = PUNCHED_TIME;

#ifdef DEBUG_ON
	cube_.centerPos = Utility::VECTOR_ZERO;
	cubeMovePos_ = Utility::VECTOR_ZERO;
#endif // DEBUG_ON


	itemLocalPos_ = Utility::VECTOR_ZERO;
	trans_.Update();
}

void Player::Update(void)
{
	animationController_->Update();









	if (IsDeath())
	{
		//落ちているアニメーション再生0
		animationController_->Play(static_cast<int>(ANIM_TYPE::FALL), true);
		
		if (animationController_->GetAnimStep() >= FALL_ANIM_START)
		{
			animationController_->SetEndLoop(FALL_ANIM_START, FALL_ANIM_END, 60.0f);
		}
		return;
	}
	//入力更新
	input_->Update();
	
#ifdef DEBUG_ON
	CubeMove();
#endif // DEBUG_ON
	static VECTOR dirDown = trans_.GetDown();
	//重力(各アクションに重力を反映させたいので先に重力を先に書く)
	GravityManager::GetInstance()->CalcGravity(dirDown, jumpPow_,20.0f);


	//アクション関係
	Action();


	//衝突判定
	Collision();

	//回転の同期
	trans_.quaRot = playerRotY_;
	

	trans_.Update();
}

void Player::Draw(void)
{
	MV1DrawModel(trans_.modelId);
#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}


#ifdef DEBUG_ON
void Player::DrawDebug(void)
{
	unsigned int color = 0xffffff;
	const int HIGH = 10;
	const int WIDTH = 200;
	if (playerNum_ == 0) { color = 0xffffff; }
	else if (playerNum_ == 1) { color = 0x550000; }
	else if (playerNum_ == 2) { color = 0x00ff00; }
	else if (playerNum_ == 3) { color = 0x0000ff; }
	if (isCol_) { color = 0xff0000; }
	DrawSphere3D(trans_.pos, RADIUS, 10, color, color, false);
	DrawFormatString(0, 16*(playerNum_*7), 0x000000
		, "角度(%.2f,%.2f,%.2f)\njumpDecel(%f)\nstepJump_(%f)\njumpPow(%f,%f,%f)\nmovedPos(%f,%f,%f)\nhitIType(%d)"
		, trans_.rot.x, trans_.rot.y, trans_.rot.z
		,jumpDeceralation_
		,stepJump_
		,jumpPow_.x,jumpPow_.y,jumpPow_.z
		,movedPos_.x,movedPos_.y,movedPos_.z
		,tag_
	);
	if (IsDeath())
	{
		static int OFFSET = 32;
		//リトライするかEditシーンに戻るか選べるようにする。
		DrawFormatString(Application::SCREEN_HALF_X, Application::SCREEN_HALF_Y+ OFFSET *playerNum_, 0xff0000, "(%d)GameOver", playerNum_);
	}
	

	DrawSphere3D(punchPos_, PUNCH_RADIUS, 4, 0xff0000, 0xff0000, isPunchHitTime_);

	DrawCube3D({ cube_.centerPos.x - CUBE_W,cube_.centerPos.y - CUBE_H,cube_.centerPos.z - CUBE_D }
	, { cube_.centerPos.x + CUBE_W,cube_.centerPos.y + CUBE_H,cube_.centerPos.z + CUBE_D }, 0xff0000, 0xff0000, true);
	//DrawFormatString(0, 80* (playerNum_ + 3), 0, "POS = %f,%f,%f", movedPos_.x, movedPos_.y, movedPos_.z);
	//DrawFormatString(0, 100* (playerNum_ + 3), 0, "jump = %d", static_cast<int>(isJump_));
}
#endif // DEBUG_ON
void Player::Action(void)
{
	Rotate();
	Punch();
	Jump();
	Move();
}

void Player::Move(void)
{
	movePow_ = Utility::VECTOR_ZERO;
	if (!isPunched_)
	{
		dir_ = Utility::VECTOR_ZERO;
	}
	VECTOR getDir = input_->GetDir();
	float deg = 0;
	Quaternion cameraRot = scnMng_.GetCamera().lock()->GetQuaRotOutX();
	Quaternion angle = Quaternion::AngleAxis(Utility::Deg2RadF(deg), Utility::AXIS_Y);
	//吹き飛び中でなかったらカメラ方向に移動したい
	if (input_->CheckAct(PlayerInput::ACT_CNTL::MOVE)&&!isPunched_&&!isPunch_)
	{
		deg = input_->GetMoveDeg();
		dir_ = cameraRot.PosAxis(getDir);
		dir_ = VNorm(dir_);
	}

	if (!Utility::EqualsVZero(dir_))
	{
		//パンチされてぶっ飛んでる時と通常の移動の時のスピード
		if (isPunched_){speed_ = FLY_AWAY_SPEED;}
		else{speed_ = MOVE_SPEED;}
		
		if(!isJump_&&!isPunch_)animationController_->Play(static_cast<int>(ANIM_TYPE::WALK));
		
		moveDir_ = dir_;
		//移動量
		movePow_ = VScale(moveDir_, speed_);
		SetGoalRotate(Utility::Deg2RadF(deg));
	}
	else if(!isJump_&&!isPunch_)
	{
		speed_ = 0.0f;
		animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
	}
}
void Player::Rotate(void)
{
	stepRotTime_ -= PlayerInput::DELTA_TIME;
	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}
void Player::SetGoalRotate(double _deg)
{
	VECTOR cameraRot = scnMng_.GetCamera().lock()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(
		(double)cameraRot.y + _deg, Utility::AXIS_Y);
	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);
	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}
	goalQuaRot_ = axis;
}
bool Player::IsDeath(void)
{
	if (trans_.pos.y <= DEATH_POS_Y)
	{
		return true;
	}
	return false;
}
void Player::Jump(void)
{
	//if (isPunch_)return;
	bool isHit = input_->CheckAct(PlayerInput::ACT_CNTL::JUMP);
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	if (isJump_)return;
	if (isHit)
	{
		if (!isJump_)
		{
			stepJump_ = 0.0f;
		}
		//空中で無理やりアニメーションを切り取ってアニメーションをする
		animationController_->Play(
			(int)ANIM_TYPE::JUMP, false, 10.0f, 60.0f);
		animationController_->SetEndLoop(23.0f, 25.0f, 5.0f);

		isJump_ = true;
		//ジャンプの入力受付時間を減らす
		stepJump_ += deltaTime;

		isPunch_ = false;
		isPunchHitTime_ = false;
		punchCnt_ = 0.0f;
	}

	if (stepJump_ > 0.0f)
	{
		if (!isJump_)
		{
			stepJump_ = 0.0f;
			return;
		}
			
		stepJump_ += deltaTime;
		if (jumpDeceralation_ < 0.0f)
		{
			animationController_->Play(static_cast<int>(ANIM_TYPE::LAND));
		}
		float i = stepJump_ * TIME_JUMP_SCALE;
		jumpDeceralation_ -= i;
		jumpPow_ = VScale(trans_.GetUp(), jumpDeceralation_);
	}

	// ボタンを離したらジャンプ力に加算しない
	if(!isJump_)
	{
		jumpDeceralation_ = POW_JUMP;
		fallCnt_ = 0.0f;
		jumpPow_ = Utility::VECTOR_ZERO;
		stepJump_ = 0.0f;
	} 
}
void Player::Punch(void)
{
	//プレイヤーの手の座標を設定する

	punchPos_ = MV1GetFramePosition(trans_.modelId, 10);
	if (isJump_ || punchCoolCnt_ >= 0.0f)
	{
		punchCoolCnt_ < 0.0f ? punchCoolCnt_ = 0.0f : punchCoolCnt_ -= scnMng_.GetDeltaTime();
		return;
	}

	
	auto isHit = input_->CheckAct(PlayerInput::ACT_CNTL::PUNCH);
	if (punchCnt_ > PUNCH_TIME_MAX)
	{
		punchCnt_ = 0.0f;
		isPunch_ = false;
		punchCoolCnt_ = PUNCH_COOL_TIME;
		return;
	}
	//パンチキーを押されたらパンチ行動をとる
	if (isHit){isPunch_ = true;}
	if (isPunch_)
	{
		punchCnt_ += PlayerInput::DELTA_TIME;
		animationController_->Play((int)ANIM_TYPE::PUNCH,false);
		float animStep = animationController_->GetAnimStep();
		if (animStep > PUNCH_HIT_END_ANIM_STEP)
		{
			isPunchHitTime_ = false;
		}
		else if (animStep > PUNCH_HIT_START_ANIM_STEP)
		{

			isPunchHitTime_ = true;
		}	

	}

	//パンチを受けた時
	if (isPunched_)
	{
		punchedCnt_ -= scnMng_.GetDeltaTime();
	}
	if (punchedCnt_ < 0.0f)
	{
		isPunched_ = false;
		punchedCnt_ = PUNCHED_TIME;
	}
}




VECTOR Player::AddPosRotate(VECTOR _followPos, Quaternion _followRot, VECTOR _localPos)
{
	//座標回転
	VECTOR addPos = _followRot.PosAxis(_localPos);

	//足したものを返す
	return VAdd(_followPos, addPos);
}

void Player::HitItem(const IntVector3 _colPos)
{
	MapEditer& mapEdit = MapEditer::GetInstance();
	ItemManager& itemMng = ItemManager::GetInstance();
	if (mapEdit.IsObjectAtMapPos(_colPos))
	{
		IntVector3 lPos=mapEdit.GetLeaderMapPos(_colPos);
		for (auto& iLPos : itemLPos_)
		{
			if (iLPos == lPos)return;
		}
		
		
		//アイテムタイプ取得
		ItemBase::ITEM_TYPE type = mapEdit.GetItemType(_colPos);

		
		//アイテムのTransform取得
		Transform itemTrans = itemMng.GetItemTransform(lPos,type);

		//UpDownColl();
		//ArroundColl(itemTrans);

		itemLPos_.push_back(lPos);
	}
}

void Player::Collision(void)
{
	movedPos_ = VAdd(trans_.pos, movePow_);
	movedPos_ = VAdd(movedPos_, jumpPow_);

#ifdef DEBUG_ON
	if (CollCube())
	{
		movedPos_ = VAdd(movedPos_, cubeMovePos_);
		jumpPow_ = Utility::VECTOR_ZERO;
		movedPos_.y = cube_.upPos.y + RADIUS;
		stepJump_ = 0.0f;
		isJump_ = false;
		jumpDeceralation_ = POW_JUMP;
	}
	else
	{
		isJump_ = true;
		//if (jumpPow_.y <= LIMIT_GRAVITY)
		//{
		//	jumpPow_.y = LIMIT_GRAVITY;
		//}
	}

#endif // DEBUG_ON

	//UpDownColl();

	//MapEditer& mapEdit = MapEditer::GetInstance();
	//IntVector3 mapPos = mapEdit.WorldToMapPos(movedPos_);
	//for (int x = -COL_RANGE; x <= COL_RANGE; x++)
	//{
	//	for (int y = -COL_RANGE; y <= COL_RANGE; y++)
	//	{
	//		for (int z = -COL_RANGE; z <= COL_RANGE; z++)
	//		{
	//			colPos_ = mapPos + IntVector3{x, y, z};
	//			if (colPos_.x < 0 || colPos_.y < 0 || colPos_.z < 0)continue;
	//			HitItem(colPos_);
	//		}
	//	}
	//}
	//itemLPos_.clear();

#ifdef DEBUG_ON

	//if (movedPos_.y < 0.0f/*||!CollCube()*/)
	//{
	//	movedPos_.y = 0.0f;
	//	isJump_ = false;
	//	stepJump_ = 0.0f;
	//}
#endif // DEBUG_ON
	//移動前の座標を格納する
	moveDiff_ = VSub(movedPos_, trans_.pos);
	// 移動
	trans_.pos = movedPos_;
	// 現在座標を起点に移動後座標を決める
}

void Player::UpDownColl(void)
{
	//if (colInfo_ == ITEM_COL::UP)
	//{
	//	//movedPos_.y = hit.HitPosition.y + RADIUS + POSITION_OFFSET;
	//}








	////移動後と移動前をとる
	//VECTOR prePos = trans_.pos;
	//VECTOR curPos = movedPos_;
	//
	//VECTOR vec = VSub(curPos, prePos);

	//auto hit = MV1CollCheck_Line(_itemTrans.modelId, -1, prePos, curPos);
	//MapEditer& mapEdit = MapEditer::GetInstance();

	////当たったら
	//if (hit.HitFlag > 0)
	//{
	//	//Y座標のみ半径分上に移動させる
	//	movedPos_.y = hit.HitPosition.y + RADIUS + POSITION_OFFSET;
	//	jumpPow_ = Utility::VECTOR_ZERO;
	//	isJump_ = false;
	//	itemLocalPos_ = VSub(movedPos_, _itemTrans.pos);
	//	return;
	//}
	////else
	////{
	////	//当たらなかったら初期化する
	////	itemLocalPos_ = Utility::VECTOR_ZERO;
	////}


	////Lineを引くための上と下の座標をとる
	//VECTOR upPos = movedPos_;
	//upPos.y += (RADIUS);
	//VECTOR downPos = movedPos_;
	//downPos.y -= (RADIUS+ 10.0f);

	//hit = MV1CollCheck_Line(_itemTrans.modelId, -1, upPos, downPos);

	////当たったら
	//if (hit.HitFlag > 0)
	//{
	//	//座標をワールド座標とアイテムローカル座標を足した分移動させる
	//	if (!Utility::EqualsVZero(itemLocalPos_))
	//	{
	//		VECTOR itemLocalPos = VSub(movedPos_, _itemTrans.pos);
	//		movedPos_ = VAdd(itemLocalPos_, _itemTrans.pos);
	//		movedPos_ = VAdd(movedPos_, vec);
	//		hitItemType_ =mapEdit.GetItemType(mapEdit.WorldToMapPos(hit.HitPosition));
	//	}
	//	//Y座標のみ半径分上に移動させる
	//	if (movedPos_.y > hit.HitPosition.y)
	//	{
	//		movedPos_.y = hit.HitPosition.y + RADIUS + POSITION_OFFSET;
	//	}
	//	else
	//	{
	//		movedPos_.y = hit.HitPosition.y - RADIUS - POSITION_OFFSET;
	//	}
	//	jumpPow_ = Utility::VECTOR_ZERO;
	//	isJump_ = false;
	//	itemLocalPos_ = VSub(movedPos_, _itemTrans.pos);
	//}
	//else
	//{
	//	//当たらなかったら初期化する
	//	itemLocalPos_ = Utility::VECTOR_ZERO;
	//	isJump_ = true;
	//	hitItemType_ = ItemBase::ITEM_TYPE::NONE;
	//}
}

void Player::ArroundColl(Transform _itemTrans)
{
	//移動後座標を一回格納し、移動前をとる
	Transform trans = Transform(trans_);
	trans.pos = movedPos_;
	trans.Update();

	auto hits = MV1CollCheck_Sphere(_itemTrans.modelId, -1, trans.pos
		, RADIUS);
	for (int i = 0; i < hits.HitNum; i++)
	{
		auto hit = hits.Dim[i];
		for (int tryCnt = 0; tryCnt < (int)RADIUS; tryCnt++)
		{
			hit.Position[i];
			int pHit = HitCheck_Sphere_Triangle(trans.pos, RADIUS
				, hit.Position[0], hit.Position[1], hit.Position[2]);
			if (pHit)
			{
				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
				// カプセルを移動させる
				trans.pos = movedPos_;
				trans.Update();
				continue;
			}

			break;
		}
		
	}
	MV1CollResultPolyDimTerminate(hits);
}

#ifdef DEBUG_ON
void Player::HitAction(Collider::COL_TAG _tag, bool _isHit, VECTOR _hitPos, VECTOR _itemPos)
{
#ifdef DEBUG_ON
	tag_ = (int)_tag;
#endif // DEBUG_ON

	
	if (_tag == Collider::COL_TAG::PLAYER)return;
	//移動後と移動前をとる
	VECTOR prePos = trans_.pos;
	VECTOR curPos = movedPos_;

	VECTOR vec = VSub(curPos, prePos);

	//当たったら
	if (_isHit)
	{
		//Y座標のみ半径分上に移動させる
		movedPos_.y = _hitPos.y + RADIUS + POSITION_OFFSET;
		jumpPow_ = Utility::VECTOR_ZERO;
		isJump_ = false;
		itemLocalPos_ = VSub(movedPos_, _itemPos);
		return;
	}
	//else
	//{
	//	//当たらなかったら初期化する
	//	itemLocalPos_ = Utility::VECTOR_ZERO;
	//}


	//Lineを引くための上と下の座標をとる
	VECTOR upPos = movedPos_;
	upPos.y += (RADIUS);
	VECTOR downPos = movedPos_;
	downPos.y -= (RADIUS + 10.0f);

	//hit = MV1CollCheck_Line(_itemTrans.modelId, -1, upPos, downPos);

	//当たったら
	if (_isHit )
	{
		//座標をワールド座標とアイテムローカル座標を足した分移動させる
		if (!Utility::EqualsVZero(itemLocalPos_))
		{
			VECTOR itemLocalPos = VSub(movedPos_, _itemPos);
			movedPos_ = VAdd(itemLocalPos_, _itemPos);
			movedPos_ = VAdd(movedPos_, vec);
			//hitItemType_ =mapEdit.GetItemType(mapEdit.WorldToMapPos(hit.HitPosition));
		}
		//Y座標のみ半径分上に移動させる
		if (movedPos_.y > _hitPos.y)
		{
			movedPos_.y = _hitPos.y + RADIUS + POSITION_OFFSET;
		}
		else
		{
			movedPos_.y = _hitPos.y - RADIUS - POSITION_OFFSET;
		}
		jumpPow_ = Utility::VECTOR_ZERO;
		isJump_ = false;
		itemLocalPos_ = VSub(movedPos_, _itemPos);
	}
	else
	{
		//当たらなかったら初期化する
		itemLocalPos_ = Utility::VECTOR_ZERO;
		isJump_ = true;
		hitItemType_ = ItemBase::ITEM_TYPE::NONE;
	}
}
void Player::Onhit(CollisionManager::COL_TAG)
{

}
void Player::OnHitNone(void)
{
}
void Player::OnHitFloor(const IntVector3 _colPos)
{
}
void Player::OnHitDeathItem(const IntVector3 _colPos)
{
}
void Player::CubeMove(void)
{
	auto& input = InputManager::GetInstance();
	const float SPD = 8.0f;
	cubeMovePos_ = Utility::VECTOR_ZERO;
	cube_.upPos = VAdd(cube_.centerPos, { 0.0f,CUBE_H,0.0f });
	if (input.IsNew(KEY_INPUT_UP))cubeMovePos_.z += SPD;
	if (input.IsNew(KEY_INPUT_DOWN))cubeMovePos_.z -= SPD;
	if (input.IsNew(KEY_INPUT_RIGHT))cubeMovePos_.x += SPD;
	if (input.IsNew(KEY_INPUT_LEFT))cubeMovePos_.x -= SPD;
	if (input.IsNew(KEY_INPUT_T))cubeMovePos_.y -= SPD;
	if (input.IsNew(KEY_INPUT_Y))cubeMovePos_.y += SPD;
	cube_.centerPos = VAdd(cube_.centerPos, cubeMovePos_);
}

bool Player::CollCube(void)
{
	VECTOR jumpLine = VSub(movedPos_, trans_.pos);
	VECTOR diff = VSub(cube_.centerPos, movedPos_);
	if (fabsf(diff.x) > CUBE_W + RADIUS
		|| fabsf(diff.y) > CUBE_H + RADIUS
		|| fabsf(diff.z) > CUBE_D + RADIUS)
	{
		return false;
	}
	return true;
}
#endif // DEBUG_ON


