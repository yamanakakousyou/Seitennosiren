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
	for (auto e : m_enemies) {
		DeleteGO(e);
	}
	m_enemies.clear();

	//ゲームカメラを削除する
	DeleteGO(m_gamecamera);
	//背景を削除する
	DeleteGO(m_backGround);

	DeleteGO(m_boss);

	DeleteGO(m_message);

	DeleteGO(m_soundSource);

	DeleteGO(this);
}

bool Game::Start()
{
	m_backGround = NewGO<BackGround>(0, "backGround");
	m_player = NewGO<Player>(0, "player");
	m_sord = NewGO<Sord>(0, "item");
	m_message = NewGO<Message>(0, "message");

	auto meat = NewGO<Food>(0, "meat");
	auto onigiri = NewGO<Food>(0, "onigiri");

	m_gamecamera = NewGO<GameCamera>(0, "gamecamera");
	m_map = NewGO<Map>(0, "map");
	m_currentTurn = TurnType::Player;
	Vector3 bossSpawnPos;
	//ステージIDで判別する
	int stageID = m_backGround->GetStageID();

	if (stageID == 0)
	{
		//ステージ1生成時の位置
		m_player->SetPosition(Vector3(m_player->m_position));
		SpawnEnemy({ 300.0f, 0.0f, 0.0f });
		bossSpawnPos = { 300.0f, 0.0f, 0.0f };
		m_sord->SetPosition(Vector3(50.0f, 0.0f, 0.0f));
		meat->SetType(FoodType::Meat);
		meat->SetPosition({ 100.0f, 0.0f, 0.0f });
		onigiri->SetType(FoodType::Onigiri);
		onigiri->SetPosition({ 200.0f, 0.0f, 0.0f });
	}

	if (stageID == 1)
	{
		//ステージ2生成時の位置
		m_player->SetPosition(Vector3(m_player->m_position));
		SpawnEnemy({ 300.0f, 0.0f, 0.0f });
		bossSpawnPos = { -100.0f, 0.0f, 0.0f };
		m_sord->SetPosition(Vector3(50.0f, 0.0f, 0.0f));
		meat->SetType(FoodType::Meat);
		meat->SetPosition({ -100.0f, 0.0f, 0.0f });
		onigiri->SetType(FoodType::Onigiri);
		onigiri->SetPosition({ 200.0f, 0.0f, 0.0f });
	}

	if (stageID == 2)
	{
		//ステージ3生成時の位置
		m_player->SetPosition(Vector3(m_player->m_position));
		SpawnEnemy({ 300.0f, 0.0f, 0.0f });
		SpawnEnemy({ -60.0f, 0.0f, 50.0f });
		SpawnEnemy({ -200.0f, 0.0f, -100.0f });
		bossSpawnPos = { 50.0f, 0.0f, 0.0f };
		m_sord->SetPosition(Vector3(50.0f, 0.0f, 0.0f));
		meat->SetType(FoodType::Meat);
		meat->SetPosition({ 300.0f, 0.0f, 0.0f });
		onigiri->SetType(FoodType::Onigiri);
		onigiri->SetPosition({ 200.0f, 0.0f, 0.0f });
	}

	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/BGM.wav");
	m_soundSource = NewGO<SoundSource>(0);
	//ResistWaveFileBankで指定した番号。
	m_soundSource->Init(1);
	//BGMは曲をループさせる。
	m_soundSource->Play(true);

	m_bossSpawnPos = bossSpawnPos;
	return true;
}

void Game::Update()
{

	if (FindGO<Pouse>("pouse")) return;

	if (m_currentTurn == TurnType::Enemy)
	{
		bool allMoved = true;

		for (auto enemy : m_enemies)
		{
			if (!enemy->HasMoved)
			{
				enemy->EnemyTurn();
			}

			if (!enemy->HasMoved)
			{
				allMoved = false;
			}
		}

		if (allMoved)
		{
			NextTurn(); //全員動いたら次ターンへ
		}
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
	if (m_currentTurn == TurnType::Player) 
	{
		m_currentTurn = TurnType::Enemy;
		SetEnemyTurnStartTimeNow(); // 敵ターン開始時刻を記録

		for (auto e : m_enemies) 
		{
			e->HasMoved = false;
		}
	}

	else 
	{
		m_currentTurn = TurnType::Player;
	}
}

void Game::SpawnEnemy(const Vector3& pos)
{
	Enemy* e = NewGO<Enemy>(0, "enemy");
	e->SetPlayer(m_player);
	e->SetPosition(pos);
	m_enemies.push_back(e);
}

void Game::SpawnBoss(const Vector3&pos)
{
	Boss* b = NewGO<Boss>(0, "boss");
	b->SetPlayer(m_player);
	b->SetPosition(pos);    
	m_enemies.push_back(b);

	m_boss = b;
}

void Game::OnEnemyDead(Enemy* enemy)
{
	m_enemies.erase
	(
		std::remove(m_enemies.begin(), m_enemies.end(), enemy),
		m_enemies.end()
	);

	// 雑魚が全滅したらボスを出す
	if (m_enemies.empty() && m_boss == nullptr) 
	{
		SpawnBoss(m_bossSpawnPos);
	}
}

TurnType Game::GetCurrentTurn() const {
	return m_currentTurn;
}
