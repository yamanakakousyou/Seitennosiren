#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Sord.h"
#include "Map.h"

#include "Level3DRender/LevelRender.h"
#include <chrono>

class Player;
class BackGround;
class GameCamera;
class Enemy;
class Boss;
class Sord;
class Food;
class Message;
class GameClear;
class Map;
class Title;
class Pouse;

enum class TurnType {
	Player,
	Enemy
};

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void PlayerTurn();
	void EnemyTurn();
	void NextTurn();
	void SpawnEnemy(const Vector3& pos);
	void SpawnBoss();
	void OnEnemyDead(Enemy* enemy);

public:
	void SetEnemyTurnStartTimeNow() {
		m_enemyTurnStartTime = std::chrono::steady_clock::now();
	}
	//待機時間が経過したかを判定する
	bool IsEnemyWaitTimeElapsed() const {
		auto now = std::chrono::steady_clock::now();
		float elapsed = std::chrono::duration<float>(now - m_enemyTurnStartTime).count();
		//ターンが切り替わるまでの時間
		return elapsed >= m_enemyWaitSeconds;
	}

public:
	ModelRender									m_modelRender;
	Vector3										m_pos;
	Vector3 GetPlayerPosition() const { return	m_player->GetPosition(); }
	Vector3 GetItemPosition() const { return	m_sord->GetPosition(); }
	TurnType									m_currentTurn = TurnType::Player;
	TurnType GetCurrentTurn() const;

	//Enemy
	std::vector<Enemy*>							m_enemies;
	const std::vector<Enemy*>& GetEnemies() const { return m_enemies; }
	std::chrono::steady_clock::time_point		m_enemyTurnStartTime;

private:
	BackGround*								m_backGround;
	Player*									m_player;
	Sord*									m_sord;
	Food*									m_food;
	Boss*									m_boss;
	Map*									m_map;
	Pouse*									m_pouse;
	Message*								m_message;
	GameCamera*								m_gamecamera;
	GameClear*								m_gameclear;
	Title*									m_title;
	SoundSource*							m_soundSource = nullptr;
	

	float									m_enemyWaitSeconds = 1.0f; // 例: 1秒待つ
};

