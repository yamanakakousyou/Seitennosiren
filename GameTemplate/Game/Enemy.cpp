#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include "Game.h"
#include <cstdlib> // rand()�p

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
	m_characterController.Init(25.0f, 75.0f, m_position); // �� �����œ����蔻��̑傫�����w��

	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(0.45f, 0.45f, 0.45f);
	m_game = FindGO<Game>("game");
	//�����蔻���`�悷��B
//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_EnemyHP = EnemyHP;
	return true;
}

void Enemy::Update()
{
	// �A�j���E���̑�����
	m_modelRender.Update();
	m_EnemyspriteRender.Update();
	EnemyTurn();
	EnemyTakeDamage(dmg);
	//EnemyAttack();
	Cooldown(dt);
	EnemyUI();
}



void Enemy::Cooldown(float dt) {
	// �N�[���_�E���̐i�s
	HasCooldown = false;
	if (m_attackCooldown > 0.0f) {
		m_attackCooldown -= dt;
		if (m_attackCooldown < 0.0f) m_attackCooldown = 0.0f;
	}

	// �U���̓N�[���_�E����0�̂Ƃ��̂ݎ��s
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
		// �҂����Ԃ��o�߂��Ă��Ȃ���Ή������Ȃ�
		if (!m_game->IsEnemyWaitTimeElapsed()) {
			return;
		}

		Vector3 toPlayer = m_game->GetPlayerPosition() - m_position;
		float dist = toPlayer.Length();

		float attackRange = 70.0f; // �ߐڍU���ł��鋗��

		// --- �U���D�� ---
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

		// �v���C���[���߂���΋߂Â��A����ȊO�̓����_��
		float chaseRange = 150.0f; // �v���C���[�ɋ߂Â��͈�

		if (dist < chaseRange) {
			// �v���C���[�����Ɉړ�
			toPlayer.Normalize();
			move = toPlayer * step;
		}
		else {
			// �����_���ړ�
			int dir = rand() % 4;
			switch (dir) {
			case 0: move = Vector3(0, 0, step); break;   // �O
			case 1: move = Vector3(0, 0, -step); break;  // ��
			case 2: move = Vector3(-step, 0, 0); break;  // ��
			case 3: move = Vector3(step, 0, 0); break;   // �E
			}
		}

		m_position = m_characterController.Execute(move, 1.0f);
		m_modelRender.SetPosition(m_position);

		// �������X�V
		float angle = atan2f(move.x, move.z);
		Quaternion rot;
		rot.SetRotationY(angle);
		m_modelRender.SetRotation(rot);

		// Idle �A�j���[�V�������Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Idle);

		HasMoved = true;
		m_game->NextTurn();
	}

	Vector3 playerPos = m_game->GetPlayerPosition();
	float dist = (playerPos - m_position).Length();
	if (dist < 50.0f) { // 50.0f�͓����蔻��̂������l
		// ������������
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
			// �v���C���[�Ƀ_���[�W�𑗂�̂݁B�v���C���[���� HP ���Ǘ�
			m_player->PlayerTakeDamage(1);
			// �G�t�F�N�g/�T�E���h�������ɒǉ�
		}
		m_attackCooldown = kAttackCooldown;
	}
}

void Enemy::EnemyUI()
{
	//// HP�\���p�̃o�b�t�@��p��
	//wchar_t hpText[32];
	//swprintf_s(hpText, sizeof(hpText) / sizeof(wchar_t), L"HP%d/10", m_EnemyHP);

	//m_fontRender.SetText(hpText); // ������SetText���A���ۂ̃e�L�X�g�ݒ�֐��ɍ��킹�Ă�������
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

		// �� �����������O�� Game �� m_enemy �𖳌���
		if (m_game)
		{
			m_game->OnEnemyDead(this);
		}

		DeleteGO(this); // ���ۂ̍폜�͎��t���[��
	}
	return;
}



void Enemy::Render(RenderContext& rc)
{
	//���f����`�悷��
	m_modelRender.Draw(rc);
	m_fontRender.Draw(rc);
}