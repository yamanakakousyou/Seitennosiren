#include "stdafx.h"
#include "Game.h"
#include "BackGround.h"
#include "Player.h"
#include "Enemy.h"
#include "GameCamera.h"
#include "Sord.h"
#include "Food.h"
#include "Boss.h"
#include "GameClear.h"
#include "Message.h"
#include "Map.h"
#include "Title.h"
#include "Pouse.h"


Game::Game()
{


}
Game::~Game()
{

	//プレイヤーを削除する
	DeleteGO(m_player);
	//敵を削除する
	DeleteGO(m_enemy);
	//ゲームカメラを削除する
	DeleteGO(m_gamecamera);
	//背景を削除する
	DeleteGO(m_backGround);

	DeleteGO(m_boss);

	DeleteGO(m_message);

	DeleteGO(this);
}

bool Game::Start()
{
	m_backGround = NewGO<BackGround>(0, "backGround");
	m_player = NewGO<Player>(0, "player");
	m_enemy = NewGO<Enemy>(0, "enemy");
	m_sord = NewGO<Sord>(0, "item");
	m_message = NewGO<Message>(0, "message");
	m_map = NewGO<Map>(0, "map");

	auto meat = NewGO<Food>(0, "meat");
	auto onigiri = NewGO<Food>(0, "onigiri");

	m_enemy->SetPlayer(m_player);

	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	m_currentTurn = TurnType::Player;

	//ステージIDで判別する
	int stageID = m_backGround->GetStageID();

	if (stageID == 0)
	{
		// プレイヤーとエネミーの初期位置を十分離す
		m_player->SetPosition(Vector3(m_player->m_position));
		m_enemy->SetPosition(Vector3(300.0f, 0.0f, 0.0f));
		m_sord->SetPosition(Vector3(50.0f, 0.0f, 0.0f));
		meat->SetType(FoodType::Meat);
		meat->SetPosition({ 100.0f, 0.0f, 0.0f });
		onigiri->SetType(FoodType::Onigiri);
		onigiri->SetPosition({ 200.0f, 0.0f, 0.0f });
	}

	if (stageID == 1)
	{
		//m_player->SetPosition(Vector3(m_player->m_position));
		//m_enemy->SetPosition(Vector3(400.0f, 0.0f, 0.0f));
		//m_item->SetPosition(Vector3(100.0f, 0.0f, 0.0f));
		//m_boss->SetPosition(Vector3(-300.0f, 0.0f, 0.0f));
	// プレイヤーとエネミーの初期位置を十分離す
		m_player->SetPosition(Vector3(m_player->m_position));
		m_enemy->SetPosition(Vector3(300.0f, 0.0f, 0.0f));
		m_sord->SetPosition(Vector3(50.0f, 0.0f, 0.0f));
		meat->SetType(FoodType::Meat);
		meat->SetPosition({ 100.0f, 0.0f, 0.0f });
		onigiri->SetType(FoodType::Onigiri);
		onigiri->SetPosition({ 200.0f, 0.0f, 0.0f });
	}

	if (stageID == 2)
	{
		m_player->SetPosition(Vector3(m_player->m_position));
		m_enemy->SetPosition(Vector3(300.0f, 0.0f, 0.0f));
		m_sord->SetPosition(Vector3(50.0f, 0.0f, 0.0f));
		meat->SetType(FoodType::Meat);
		meat->SetPosition({ 100.0f, 0.0f, 0.0f });
		onigiri->SetType(FoodType::Onigiri);
		onigiri->SetPosition({ 200.0f, 0.0f, 0.0f });
	}

	return true;
}

void Game::Update()
{
	// STARTでポーズ画面生成
	if (g_pad[0] && g_pad[0]->IsTrigger(enButtonStart))
	{
		if (!FindGO<Pouse>("pouse"))
		{
			NewGO<Pouse>(0, "pouse");
		}
		return; // ゲーム本体停止
	}

	// ポーズ中はゲーム本体更新を止める
	if (FindGO<Pouse>("pouse"))
	{
		return;
	}
}

void Game::Render(RenderContext& rc)
{
	//m_modelRender.Draw(rc);
}

void Game::PlayerTurn()
{
}

void Game::NextTurn() {
	if (m_currentTurn == TurnType::Player) {
		m_currentTurn = TurnType::Enemy;
		SetEnemyTurnStartTimeNow(); // 敵ターン開始時刻を記録
	}
	else {
		m_currentTurn = TurnType::Player;
	}
}

void Game::SpawnBoss()
{
	if (m_boss == nullptr) //雑魚が死んだら
	{
		m_boss = NewGO<Boss>(0, "boss");
		m_boss->SetPlayer(m_player);
		m_boss->SetPosition(Vector3(400.0f, 0.0f, 0.0f));
		m_player->SetBoss(m_boss);
	}
}

void Game::OnEnemyDead(Enemy* enemy)
{
	if (m_enemy == enemy)
	{
		m_enemy = nullptr;
	}
}

TurnType Game::GetCurrentTurn() const {
	return m_currentTurn;
}
