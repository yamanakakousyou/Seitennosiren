#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Sord.h"

#include "Level3DRender/LevelRender.h"
#include <chrono>

class Player;
class BackGround;
class GameCamera;
class Enemy;
class Boss;
class Sord;
class Food;
class GameClear;
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
	void SpawnBoss();
	void OnEnemyDead(Enemy* enemy);
	TurnType GetCurrentTurn() const;
	Vector3 GetPlayerPosition() const { return m_player->GetPosition(); }
	Vector3  GetEnemyPosition() const { return m_enemy->GetPosition(); }
	Vector3 GetItemPosition() const { return m_sord->GetPosition(); }

	//private:
	ModelRender m_modelRender;
	BackGround* m_backGround;
	Player* m_player;
	Enemy* m_enemy;
	Sord* m_sord;
	Food* m_food;
	Boss* m_boss;
	GameCamera* m_gamecamera;
	GameClear* m_gameclear;
	Vector3 m_pos;
	TurnType m_currentTurn = TurnType::Player;
	std::chrono::steady_clock::time_point m_enemyTurnStartTime;
	float m_enemyWaitSeconds = 1.0f; // 例: 2秒待つ


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
};

