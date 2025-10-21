#include "stdafx.h"
#include "Boss.h"
#include "Game.h"
#include "Player.h"
#include "GameClear.h"

namespace {
    const int BossHP = 5; // ボスの最大HP（雑魚敵より多め）
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
    // ボス専用のモデルやアニメーションをロードしたいならここで変更
    m_modelRender.Init("Assets/modelData/Boss1/Skeleton/Skeleton.tkm", animationClips, enAnimationClip_Num);

    m_characterController.Init(25.0f, 75.0f, m_position); // ← ここで当たり判定の大きさを指定

    m_modelRender.SetRotation(m_rotation);
    //m_modelRender.SetScale(0.45f, 0.45f, 0.45f);
    m_game = FindGO<Game>("game");
    m_player = FindGO<Player>("player");
    m_gameclear = FindGO<GameClear>("gameclear");

    // HP設定
    m_BossHP = BossHP;

    // ボス専用スケールなど0
    m_modelRender.SetScale(5.0f, 5.0f, 5.0f);

    return true;
}

void Boss::Update()
{
    // Enemyの通常処理
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

    float attackRange = 100.0f; // 通常より広い射程
    if (dist <= attackRange) {
        int roll = rand() % 100;
        if (roll < 80) { // 命中率
            m_player->PlayerTakeDamage(2); // 通常敵より強いダメージ
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

        // --- 攻撃判定 ---
        if (dist < attackRange) {
            float angle = atan2f(toPlayer.x, toPlayer.z);
            Quaternion rot;
            rot.SetRotationY(angle);
            m_modelRender.SetRotation(rot);

            // ★ボス専用攻撃アニメーション
            m_modelRender.PlayAnimation(enAnimationClip_BossAttack);

            EnemyAttack(); // 攻撃処理はEnemyのをそのまま使用
            HasMoved = true;
            m_game->NextTurn();
            return;
        }

        // --- それ以外はEnemyの通常行動をそのまま使用 ---
        Enemy::EnemyTurn();
    }
}

void Boss::Render(RenderContext& rc)
{
    Enemy::Render(rc);
}