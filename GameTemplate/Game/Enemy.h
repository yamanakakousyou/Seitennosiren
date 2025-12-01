#pragma once

class Player;
class Game;
class Message;
class Pouse;

enum EnAnimationClip 
{	//アニメーション。
	enAnimationClip_Idle,
	enAnimationClip_Attack,
	enAnimationClip_Num,
};

class Enemy :public IGameObject
{
public:
	Enemy();
	~Enemy();

	bool Start();

	void Update();

	void Cooldown(float dt);

	void Render(RenderContext& rc);

	void Rotation();

	virtual void EnemyTurn();

	virtual void EnemyAttack();

	void EnemyUI();

	virtual void EnemyTakeDamage(int dmg);

	virtual void SetPlayer(Player* player);

	void SetGame(Game* game) { m_game = game; }

	const Vector3& GetPosition() const { return m_position; }

	void SetPosition(const Vector3& pos) { m_position = pos; m_modelRender.SetPosition(m_position); }

	Vector3 RangeCalcV1toV2(const Vector3& v1, const Vector3& v2) 
	{
		// エネミーからプレイヤーに向かって伸びるベクトルを計算する。
		Vector3 V1toV2 = v1 - v2;
		return V1toV2;
	}

	//アニメーション
	AnimationClip animationClips[enAnimationClip_Num];		//アニメーションクリップ。

	//メンバ変数	
	Quaternion				m_rotation;
	Player*					m_player = nullptr;
	Enemy*					m_enemy = nullptr;
	Game*					m_game ;
	Message*				m_message=nullptr;
	CharacterController		m_characterController;//キャラクターコントローラー。	
	ModelRender				m_modelRender;
	FontRender				m_fontRender;
	SpriteRender			m_EnemyspriteRender;
	SoundSource*			m_attackSE = nullptr;
	SoundSource*			m_damageSE = nullptr;

	//Vector
	Vector3					m_position = { 0.0f,0.0f,0.0f };		//ポジション
	Vector3					m_moveSpeed;				//移動速度。
	Vector3					moveDir;
	Vector3					m_forward = Vector3::AxisZ;	//前ベクトル。

	bool					HasMoved = false;
	bool					HasCooldown = false;

	int						m_EnemyMaxHP = 0;
	int						m_EnemyHP = 0;
	//int					m_EnemyAttackDamage = 2;
	int						dmg = 0; // dmgを定義。必要に応じて値を設定

	float					dt = 0;
	float					m_timer = 0.0f;
	float					m_attackCooldown = 1.0f;
	const float				m_AttackCooldown = 0.5f; // 0.5 秒のクールダウン
};

