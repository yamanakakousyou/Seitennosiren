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
	//ƒ‚ƒfƒ‹‚Ì‰Šú‰»B
	if (m_type == FoodType::Meat) {
		m_modelRender.Init("Assets/modelData/Steak_Cooked.tkm");
	}
	else if (m_type == FoodType::Onigiri) {
		m_modelRender.Init("Assets/modelData/Onigiri.tkm");
	}
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

	float pickupRange = 30.0f; // E‚¦‚é”ÍˆÍ

	if (dist < pickupRange) {
		if (m_type == FoodType::Meat) {
			m_player->AddItem(ItemType::Meat);	// HP‰ñ•œ
			DeleteGO(this); // E‚Á‚½‚çÁ‚¦‚é
		}
		else if (m_type == FoodType::Onigiri) {
			m_player->AddItem(ItemType::Onigiri);	// –• “x‰ñ•œ
			DeleteGO(this); // E‚Á‚½‚çÁ‚¦‚é
		}
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