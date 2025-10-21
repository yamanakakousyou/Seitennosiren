#include "stdafx.h"
#include "Sord.h"
#include "Player.h"
#include "Game.h"

namespace {
	const float BUFF_MULTIPIER = 2.0f;		//バフの倍率。
}

Sord::Sord()
{
}

Sord::~Sord()
{

}

bool Sord::Start()
{
	//モデルの初期化。
	m_modelRender.Init("Assets/modelData/sord.tkm");
	m_modelRender.SetScale(1.0f, 1.0f, 1.0f);
	m_game = FindGO<Game>("game");
	m_player = FindGO<Player>("player");

	return true;
}

void Sord::Update()
{
	if (!m_player) return;

	Vector3 toPlayer = m_player->GetPosition() - m_position;
	float dist = toPlayer.Length();

	float pickupRange = 30.0f; // 拾える範囲

	if (dist < pickupRange) {
		m_player->AttackPower(2); // 攻撃力+2
		DeleteGO(this);              // アイテムを消す
	}
}

void Sord::Rotation()
{

}

void Sord::UpdateModelRender()
{

}

void Sord::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


