#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"
#include "Message.h"
#include "Pouse.h"

namespace 
{
	const int EnemyHP = 4;
}

Enemy::Enemy()
{

}

Enemy::~Enemy()
{
	DeleteGO(m_damageSE);
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
	m_message = FindGO<Message>("message");
	//当たり判定を描画する。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_EnemyHP = EnemyHP;

	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/Attack.wav");	//音の読み込み。
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/Damage.wav");	//音の読み込み。
	return true;
}

void Enemy::Update()
{
	if (FindGO<Pouse>("pouse")) 
	{
		return;
	}

	// アニメ・その他処理
	m_modelRender.Update();
	m_EnemyspriteRender.Update();
	EnemyTurn();
	EnemyTakeDamage(dmg);
	//EnemyAttack();
	Cooldown(dt);
	EnemyUI();
}



void Enemy::Cooldown(float dt) 
{
	// クールダウンの進行
	HasCooldown = false;

	if (m_attackCooldown > 0.0f) 
	{
		m_attackCooldown -= dt;
		if (m_attackCooldown < 0.0f) m_attackCooldown = 0.0f;
	}

	// 攻撃はクールダウンが0のときのみ実行
	if (m_attackCooldown <= 0.0f) 
	{
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

	if (m_game->GetCurrentTurn() != lastTurn) 
	{
		HasMoved = false;
	}

	lastTurn = m_game->GetCurrentTurn();

	if (m_game->GetCurrentTurn() == TurnType::Enemy && !HasMoved) 
	{
		if (!m_game->IsEnemyWaitTimeElapsed()) return;

		Vector3 toPlayer = m_game->GetPlayerPosition() - m_position;
		float dist = toPlayer.Length();
		float attackRange = 70.0f; // 近接攻撃距離

		// --- 攻撃優先 ---
		if (dist < attackRange) 
		{
			float angle = atan2f(toPlayer.x, toPlayer.z);
			Quaternion rot;
			rot.SetRotationY(angle);
			m_modelRender.SetRotation(rot);

			m_modelRender.PlayAnimation(enAnimationClip_Attack);
			EnemyAttack();
			HasMoved = true;
			return;
		}

		Vector3 move(0, 0, 0);
		float step = 20.0f; // 1マスの見た目距離
		float chaseRange = 150.0f;

		if (dist < chaseRange) 
		{
			// XとZの距離の絶対値を比較して大きい軸に沿って移動
			//常に正方向か負方向かを「正のとき/負のとき」で決める処理,
			// 分岐の回数を減らせる。
			if (fabs(toPlayer.x) > fabs(toPlayer.z)) 
			{
				move.x = (toPlayer.x > 0) ? step : -step;
			}
			else 
			{
				move.z = (toPlayer.z > 0) ? step : -step;
			}
		}
		else {
			// ランダム移動（上下左右）
			int dir = rand() % 4;
			switch (dir) 
			{
			case 0: move.z = step; break;   // 前
			case 1: move.z = -step; break;  // 後
			case 2: move.x = -step; break;  // 左
			case 3: move.x = step; break;   // 右
			}
		}

		// 移動
		m_position = m_characterController.Execute(move, 1.0f);
		m_modelRender.SetPosition(m_position);

		// 向きを更新
		float angle = atan2f(move.x, move.z);
		Quaternion rot;
		rot.SetRotationY(angle);
		m_modelRender.SetRotation(rot);

		m_modelRender.PlayAnimation(enAnimationClip_Idle);

		HasMoved = true;
	}

	// プレイヤーとの接触判定
	Vector3 playerPos = m_game->GetPlayerPosition();
	float distToPlayer = (playerPos - m_position).Length();
}

void Enemy::EnemyAttack()
{
	if (!m_player) return;

	Vector3 toPlayer = m_game->GetPlayerPosition() - m_position;
	float dist = toPlayer.Length();

	float attackRange = 60.0f;
	if (dist <= attackRange) 
	{
		int roll = rand() % 100;

		if (roll < 90) 
		{
			// プレイヤーにダメージを送るのみ。プレイヤー側で HP を管理
			m_player->PlayerTakeDamage(1);
			auto se = NewGO<SoundSource>(0);
			se->Init(7);
			se->Play(false);
			m_message->AddMessage("PlayerDamage");
			// エフェクト/サウンドをここに追加
		}

		m_attackCooldown = m_AttackCooldown;
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

	if (m_EnemyHP <= 0) 
	{
		m_EnemyHP = 0;

		// 自分を消す前に Game の m_enemy を無効化
		if (m_game)
		{
			m_game->OnEnemyDead(this);
		}

		DeleteGO(this); // 実際の削除は次フレーム
	}
}



void Enemy::Render(RenderContext& rc)
{
	//モデルを描画する
	m_modelRender.Draw(rc);
	m_fontRender.Draw(rc);
}