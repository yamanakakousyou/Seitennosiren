#include "stdafx.h"
#include "Boss.h"
#include "Game.h"
#include "Player.h"
#include "GameClear.h"

namespace {
    const int BossHP = 5; // �{�X�̍ő�HP�i�G���G��葽�߁j
}

Boss::Boss()
{

}

Boss::~Boss()
{

}

bool Boss::Start()
{
    animationClips[enAnimationClip_BossIdle].Load("Assets/animData/SkeletonAnim/SkeletonIdle.tka");
    animationClips[enAnimationClip_BossIdle].SetLoopFlag(true);
    animationClips[enAnimationClip_BossAttack].Load("Assets/animData/SkeletonAnim/SkeletonAttack.tka");
    animationClips[enAnimationClip_BossAttack].SetLoopFlag(true);
    // �{�X��p�̃��f����A�j���[�V���������[�h�������Ȃ炱���ŕύX
    m_modelRender.Init("Assets/modelData/Boss1/Skeleton/Skeleton.tkm", animationClips, enAnimationClip_Num);

    m_characterController.Init(25.0f, 75.0f, m_position); // �� �����œ����蔻��̑傫�����w��

    m_modelRender.SetRotation(m_rotation);
    //m_modelRender.SetScale(0.45f, 0.45f, 0.45f);
    m_game = FindGO<Game>("game");
    m_player = FindGO<Player>("player");
    m_gameclear = FindGO<GameClear>("gameclear");

    // HP�ݒ�
    m_BossHP = BossHP;

    // �{�X��p�X�P�[���Ȃ�0
    m_modelRender.SetScale(5.0f, 5.0f, 5.0f);

    return true;
}

void Boss::Update()
{
    // Enemy�̒ʏ폈��
    Enemy::Update();
    Enemy::EnemyTurn();

    BossTakeDamage(dmg);
}

void Boss::SetPlayer(Player* player)
{

}

void Boss::EnemyAttack()
{
    if (!m_player) return;

    Vector3 toPlayer = m_game->GetPlayerPosition() - m_position;
    float dist = toPlayer.Length();

    float attackRange = 100.0f; // �ʏ���L���˒�
    if (dist <= attackRange) {
        int roll = rand() % 100;
        if (roll < 80) { // ������
            m_player->PlayerTakeDamage(2); // �ʏ�G��苭���_���[�W
        }
        m_attackCooldown = kAttackCooldown;
    }
}

void Boss::EnemyTakeDamage(int dmg)
{
    //m_BossHP -= dmg;

    //if (m_BossHP <= 0) {
    //    m_BossHP = 0;


    //    DeleteGO(this);
    //}
}

void Boss::BossTakeDamage(int dmg)
{
    m_BossHP -= dmg;

    if (m_BossHP <= 0) {
        m_BossHP = 0;

        NewGO<GameClear>(0,"gameclear");
        DeleteGO(this);
        DeleteGO(m_game);
    }

}

void Boss::EnemyTurn()
{
    if (!m_game) return;

    static TurnType lastTurn = TurnType::Enemy;
    if (m_game->GetCurrentTurn() != lastTurn) {
        HasMoved = false;
    }
    lastTurn = m_game->GetCurrentTurn();

    if (m_game->GetCurrentTurn() == TurnType::Enemy && !HasMoved) {
        if (!m_game->IsEnemyWaitTimeElapsed()) {
            return;
        }

        Vector3 toPlayer = m_game->GetPlayerPosition() - m_position;
        float dist = toPlayer.Length();

        float attackRange = 70.0f;

        // --- �U������ ---
        if (dist < attackRange) {
            float angle = atan2f(toPlayer.x, toPlayer.z);
            Quaternion rot;
            rot.SetRotationY(angle);
            m_modelRender.SetRotation(rot);

            // ���{�X��p�U���A�j���[�V����
            m_modelRender.PlayAnimation(enAnimationClip_BossAttack);

            EnemyAttack(); // �U��������Enemy�̂����̂܂܎g�p
            HasMoved = true;
            m_game->NextTurn();
            return;
        }

        // --- ����ȊO��Enemy�̒ʏ�s�������̂܂܎g�p ---
        Enemy::EnemyTurn();
    }
}

void Boss::Render(RenderContext& rc)
{
    Enemy::Render(rc);
}