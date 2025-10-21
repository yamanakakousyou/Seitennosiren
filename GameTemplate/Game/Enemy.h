#pragma once

class Player;
class Game;
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

	bool HasMoved = false;

	bool HasCooldown = false;


	enum EnAnimationClip {		//�A�j���[�V�����B
		enAnimationClip_Idle,
		enAnimationClip_Attack,
		enAnimationClip_Num,
	};

	//�����o�ϐ�	
	Quaternion m_rotation;
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	Game* m_game ;
	Vector3 m_position = { 0.0f,0.0f,0.0f };		//�|�W�V����
	Vector3 m_moveSpeed;				//�ړ����x�B
	Vector3 moveDir;
	ModelRender m_modelRender;
	FontRender m_fontRender;
	SpriteRender	m_EnemyspriteRender;
	Vector3 m_forward = Vector3::AxisZ;	//�O�x�N�g���B
	CharacterController m_characterController;//�L�����N�^�[�R���g���[���[�B
	AnimationClip animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B	

	float m_timer = 0.0f;
	int m_EnemyMaxHP = 0;
	int m_EnemyHP = 0;
	//int m_EnemyAttackDamage = 2;
	int dmg = 0; // dmg���`�B�K�v�ɉ����Ēl��ݒ�
	float dt = 0;

	float m_attackCooldown = 1.0f;
	const float kAttackCooldown = 0.5f; // 0.5 �b�̃N�[���_�E��

	Vector3 RangeCalcV1toV2(const Vector3& v1, const Vector3& v2) {
		// �G�l�~�[����v���C���[�Ɍ������ĐL�т�x�N�g�����v�Z����B
		Vector3 V1toV2 = v1 - v2;
		return V1toV2;
	}
};

