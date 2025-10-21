#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"
#include <cstdlib> // rand()用

namespace {
	const int EnemyHP = 3;
}

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

bool Enemy::Start()
{
	animationClips[enAnimationClip_Idle].Load("Assets/animData/MM_Idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Attack].Load("Assets/animData/MM_Attack.tka");
	animationClips[enAnimationClip_Attack].SetLoopFlag(true);
	m_modelRender.Init("Assets/modelData/enemy/MushroomMan.tkm", animationClips, enAnimationClip_Num);
	m_characterController.Init(25.0f, 75.0f, m_position); // ← ここで当たり判定の大きさを指定

	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(0.45f, 0.45f, 0.45f);
	m_game = FindGO<Game>("game");
	//当たり判定を描画する。
//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_EnemyHP = EnemyHP;
	return true;
}

void Enemy::Update()
{
	// アニメ・その他処理
	m_modelRender.Update();
	m_EnemyspriteRender.Update();
	EnemyTurn();
	EnemyTakeDamage(dmg);
	//EnemyAttack();
	Cooldown(dt);
	EnemyUI();
}



void Enemy::Cooldown(float dt) {
	// クールダウンの進行
	HasCooldown = false;
	if (m_attackCooldown > 0.0f) {
		m_attackCooldown -= dt;
		if (m_attackCooldown < 0.0f) m_attackCooldown = 0.0f;
	}

	// 攻撃はクールダウンが0のときのみ実行
	if (m_attackCooldown <= 0.0f) {
		//EnemyAttack();
		HasCooldown = true;
	}
}



void Enemy::SetPlayer(Player* player)
{
	m_player = player;
}

void Enemy::EnemyTurn()
{
	if (!m_game) return;

	static TurnType lastTurn = TurnType::Enemy;
	if (m_game->GetCurrentTurn() != lastTurn) {
		HasMoved = false;
	}
	lastTurn = m_game->GetCurrentTurn();

	if (m_game->GetCurrentTurn() == TurnType::Enemy && !HasMoved) {
		// 待ち時間が経過していなければ何もしない
		if (!m_game->IsEnemyWaitTimeElapsed()) {
			return;
		}

		Vector3 toPlayer = m_game->GetPlayerPosition() - m_position;
		float dist = toPlayer.Length();

		float attackRange = 70.0f; // 近接攻撃できる距離

		// --- 攻撃優先 ---
		if (dist < attackRange) {

			float angle = atan2f(toPlayer.x, toPlayer.z);
			Quaternion rot;
			rot.SetRotationY(angle);
			m_modelRender.SetRotation(rot);


			//Cooldown(dt);
			m_modelRender.PlayAnimation(enAnimationClip_Attack);
			EnemyAttack();
			HasMoved = true;
			m_game->NextTurn();
			return;
		}

		Vector3 move(0, 0, 0);
		float step = 20.0f;

		// プレイヤーが近ければ近づく、それ以外はランダム
		float chaseRange = 150.0f; // プレイヤーに近づく範囲

		if (dist < chaseRange) {
			// プレイヤー方向に移動
			toPlayer.Normalize();
			move = toPlayer * step;
		}
		else {
			// ランダム移動
			int dir = rand() % 4;
			switch (dir) {
			case 0: move = Vector3(0, 0, step); break;   // 前
			case 1: move = Vector3(0, 0, -step); break;  // 後
			case 2: move = Vector3(-step, 0, 0); break;  // 左
			case 3: move = Vector3(step, 0, 0); break;   // 右
			}
		}

		m_position = m_characterController.Execute(move, 1.0f);
		m_modelRender.SetPosition(m_position);

		// 向きを更新
		float angle = atan2f(move.x, move.z);
		Quaternion rot;
		rot.SetRotationY(angle);
		m_modelRender.SetRotation(rot);

		// Idle アニメーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Idle);

		HasMoved = true;
		m_game->NextTurn();
	}

	Vector3 playerPos = m_game->GetPlayerPosition();
	float dist = (playerPos - m_position).Length();
	if (dist < 50.0f) { // 50.0fは当たり判定のしきい値
		// 当たった処理
	}
}

void Enemy::EnemyAttack()
{
	if (!m_player) return;

	Vector3 toPlayer = m_game->GetPlayerPosition() - m_position;
	float dist = toPlayer.Length();

	float attackRange = 60.0f;
	if (dist <= attackRange) {
		int roll = rand() % 100;
		if (roll < 90) {
			// プレイヤーにダメージを送るのみ。プレイヤー側で HP を管理
			m_player->PlayerTakeDamage(1);
			// エフェクト/サウンドをここに追加
		}
		m_attackCooldown = kAttackCooldown;
	}
}

void Enemy::EnemyUI()
{
	//// HP表示用のバッファを用意
	//wchar_t hpText[32];
	//swprintf_s(hpText, sizeof(hpText) / sizeof(wchar_t), L"HP%d/10", m_EnemyHP);

	//m_fontRender.SetText(hpText); // ここはSetText等、実際のテキスト設定関数に合わせてください
	//m_fontRender.SetPosition({ 200.0f, 430.0f, 0.0f });
	//m_fontRender.SetScale(2.0);
	//m_fontRender.SetColor(g_vec4Black);
}

void Enemy::EnemyTakeDamage(int dmg)
{
	m_EnemyHP -= dmg;

	if (m_EnemyHP <= 0) {
		m_EnemyHP = 0;
		//m_modelRender.PlayAnimation(enAnimationClip_Die);

		//NewGO<GameOver>(0, "gameover");

		//DeleteGO(m_game);

		if (m_game)
		{
			m_game->SpawnBoss();
		}

		// ★ 自分を消す前に Game の m_enemy を無効化
		if (m_game)
		{
			m_game->OnEnemyDead(this);
		}

		DeleteGO(this); // 実際の削除は次フレーム
	}
	return;
}



void Enemy::Render(RenderContext& rc)
{
	//モデルを描画する
	m_modelRender.Draw(rc);
	m_fontRender.Draw(rc);
}