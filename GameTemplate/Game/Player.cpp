#include "stdafx.h"
#include "Player.h"
#include "Game.h"
#include "Enemy.h"
#include "Boss.h"
#include "GameOver.h"

namespace {
	const int PLAYERHP = 10;
	const int PLAYERMAXHP = 10;
}

bool Player::Start()
{
	//�A�j���[�V�����N���b�v�����[�h����B
	animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Walk].Load("Assets/animData/Walk.tka");
	animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	animationClips[enAnimationClip_Attack].Load("Assets/animData/Attack.tka");
	animationClips[enAnimationClip_Attack].SetLoopFlag(true);
	animationClips[enAnimationClip_Die].Load("Assets/animData/Die.tka");
	animationClips[enAnimationClip_Die].SetLoopFlag(true);
	//���f��������������
	modelRender.Init("Assets/modelData/Unitychan.tkm", animationClips, enAnimationClip_Num, enModelUpAxisY);

	//�L�����R��������������
	//m_position = Vector3(0.0f, 0.0f, 0.0f); // ��

	m_characterController.Init(25.0f, 75.0f, m_position);

	modelRender.SetScale(0.45f, 0.45f, 0.45f);

	m_rotation.SetRotationDegY(0);
	modelRender.SetRotation(m_rotation);
	//modelRender.SetPosition(m_position);
	m_game = FindGO<Game>("game");
	m_enemy = FindGO<Enemy>("enemy");
	m_boss = FindGO<Boss>("boss");

	m_PlayerHP = PLAYERHP;
	m_PlayerMaxHP = PLAYERMAXHP;
	return true;
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Update()
{
	PlayerMoveTurn();
	ManageState();
	PlayAnimation();
	modelRender.Update();
	m_spriteRender.Update();
	PlayerUI();
	//PlayerAttack();
	PlayerTakeDamage(dmg);
}

void Player::Rotation()
{
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//�L�����N�^�[�̕�����ς���B
		m_rotation.SetRotationYFromDirectionXZ(moveSpeed);
		//�G�`������ɉ�]��������B
		modelRender.SetRotation(m_rotation);
	}
}

void Player::ManageState()
{
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{

		playerState = 1;

		modelRender.PlayAnimation(enAnimationClip_Walk);
	}
	else
	{

		//�X�e�[�g��0(�ҋ@)�ɂ���B
		playerState = 0;
		modelRender.PlayAnimation(enAnimationClip_Idle);
	}
}

void Player::PlayAnimation()
{
	switch (playerState)
	{
		//�v���C���[�X�e�[�g��0(�ҋ@)��������B
	case 0:
		//�ҋ@�A�j���[�V�������Đ�����B
		modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//�v���C���[�X�e�[�g��1(�W�����v��)��������B
	//case 1:
	//	modelRender.PlayAnimation(enAnimationClip_Walk);
	//	break;
	}
}

void Player::PlayerMoveTurn()
{
	static TurnType lastTurn = TurnType::Player;
	if (m_game->GetCurrentTurn() != lastTurn) {
		HasMoved = false;
	}
	lastTurn = m_game->GetCurrentTurn();

	if (m_game->GetCurrentTurn() != TurnType::Player)
		return;

	if (HasMoved) return;

	Vector3 moveDir = Vector3::Zero;

	static bool wasPressed = false;
	bool isPressed = false;

	if (GetAsyncKeyState('W') & 0x8000) { moveDir.z += 1.0f; isPressed = true; }
	if (GetAsyncKeyState('S') & 0x8000) { moveDir.z -= 1.0f; isPressed = true; }
	if (GetAsyncKeyState('D') & 0x8000) { moveDir.x += 1.0f; isPressed = true; }
	if (GetAsyncKeyState('A') & 0x8000) { moveDir.x -= 1.0f; isPressed = true; }


	if (!wasPressed && isPressed && moveDir.Length() > 0.0f) {
		moveDir.Normalize();
		float step = 1000.0f;

		Vector3 moveSpeed = moveDir * step;
		m_position = m_characterController.Execute(moveSpeed, 1.0f / 60.0f);
		modelRender.SetPosition(m_position);

		float angle = atan2f(moveDir.x, moveDir.z);
		Quaternion rot;
		rot.SetRotationY(angle);
		modelRender.SetRotation(rot);

		HasMoved = true;
		m_game->NextTurn();
	}

	wasPressed = isPressed; // ��ԍX�V

	// �U���{�^��
	if (g_pad[0]->IsTrigger(enButtonB)) {
		modelRender.PlayAnimation(enAnimationClip_Attack);
		PlayerAttack();
		HasMoved = true;
		m_game->NextTurn();
	}
}

void Player::PlayerUI()
{

	m_spriteRender.Init("Assets/sprite/HP.DDS", 1024, 128);
	m_spriteRender.SetPosition({ -200.0f, 500.0f, 0.0f });
	m_spriteRender.SetScale({ 0.5f, 0.5f, 0.5f });
	m_spriteRender.SetPivot(Vector2(0.0f, 0.5f));

	// HP�\���p�̃o�b�t�@��p��
	wchar_t hpText[32];
	swprintf_s(hpText, sizeof(hpText) / sizeof(wchar_t), L"HP%d/10", m_PlayerHP);

	m_fontRender.SetText(hpText); // ������SetText���A���ۂ̃e�L�X�g�ݒ�֐��ɍ��킹�Ă�������
	m_fontRender.SetPosition({ -600.0f, 530.0f, 0.0f });
	m_fontRender.SetScale(2.0);
	m_fontRender.SetColor(g_vec4Black);

	float hpRate = (float)m_PlayerHP / 10.0f; // �ő�HP��10�Ƃ��Ċ����v�Z
	m_spriteRender.SetScale({ 0.5f * hpRate, 0.5f, 0.5f });
}

void Player::PlayerAttack()
{
	float attackRange = 70.0f;   // �U������
	float attackAngle = 100.0f;  // ���̍U���p�x�i�x���j

	// �v���C���[�̌����x�N�g�����擾
	Vector3 forward(0, 0, 1);
	m_rotation.Apply(forward); // ��]��K�p���āu�������Ă�����v�ɂ���
	forward.Normalize();

	// --- �G�ւ̍U������ ---
	if (m_enemy)
	{
		Vector3 toEnemy = m_enemy->GetPosition() - m_position;
		float dist = toEnemy.Length();
		toEnemy.Normalize();

		// �˒���������
		if (dist <= attackRange)
		{
			float dot = forward.Dot(toEnemy);
			float angle = acosf(dot) * 180.0f / 3.14159f;

			if (angle < attackAngle)
			{
				int roll = rand() % 100;
				if (roll < 90)
				{
					m_enemy->EnemyTakeDamage(m_attackPower);
					// �q�b�g�G�t�F�N�g
				}
			}
		}
	}

	// --- �{�X�ւ̍U������ ---
	if (m_boss)
	{
		Vector3 toBoss = m_boss->GetPosition() - m_position;
		float distBoss = toBoss.Length();
		toBoss.Normalize();

		if (distBoss <= attackRange)
		{
			float dotB = forward.Dot(toBoss);
			float angleB = acosf(dotB) * 180.0f / 3.14159f;

			if (angleB < attackAngle)
			{
				int roll = rand() % 100;
				if (roll < 90)
				{
					m_boss->BossTakeDamage(m_attackPower);
					// �q�b�g�G�t�F�N�g
				}
			}
		}
	}
}

void Player::PlayerHeal(int recovery)
{
	m_PlayerHP += recovery;
	if (m_PlayerHP > m_PlayerMaxHP) {
		m_PlayerHP = m_PlayerMaxHP; // �ő�HP�𒴂��Ȃ��悤��
	}
}

void Player::SetEnemy(Enemy* enemy)
{
	m_enemy = enemy;
}

void Player::SetBoss(Boss* boss)
{
	m_boss = boss;
}

void Player::AttackPower(int value)
{
	m_attackPower += value;
}

void Player::PlayerTakeDamage(int dmg)
{
	m_PlayerHP -= dmg;

	if (m_PlayerHP <= 0) {
		m_PlayerHP = 0;

		modelRender.PlayAnimation(enAnimationClip_Die);

		NewGO<GameOver>(0, "gameover");
		DeleteGO(this);
		DeleteGO(m_game);
	}
	return;
}

void Player::Render(RenderContext& renderContext)
{
	modelRender.Draw(renderContext);
	m_spriteRender.Draw(renderContext);
	m_fontRender.Draw(renderContext);
}