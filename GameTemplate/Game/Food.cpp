#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "Food.h"


Food::Food()
{
}

Food::~Food()
{
}

bool Food::Start()
{
	//モデルの初期化。
	m_modelRender.Init("Assets/modelData/Steak_Cooked.tkm");
	m_modelRender.SetScale(7.0f, 7.0f, 7.0f);
	m_game = FindGO<Game>("game");
	m_player = FindGO<Player>("player");

	return true;
}

void Food::Update()
{
	if (!m_player) return;

	Vector3 toPlayer = m_player->GetPosition() - m_position;
	float dist = toPlayer.Length();

	float pickupRange = 30.0f; // 拾える範囲

	if (dist < pickupRange) {
		m_player->PlayerHeal(2);	//HPを2回復
		DeleteGO(this);              // アイテムを消す
	}
	m_modelRender.Update();
}

void Food::Rotation()
{

}

void Food::UpdateModelRender()
{

}

void Food::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}