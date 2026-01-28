#include "stdafx.h"
#include "Boss.h"
#include "Game.h"
#include "Player.h"
#include "GameClear.h"
#include "Message.h"
#include "Pouse.h"

namespace 
{
    const int BossHP = 9; // ボスの最大HP（雑魚敵より多め）
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
    m_game = FindGO<Game>("game");
    m_player = FindGO<Player>("player");
    m_gameclear = FindGO<GameClear>("gameclear");
    m_message = FindGO<Message>( "message");

    // HP設定
    m_BossHP = BossHP;

    // ボス専用スケールなど0
    m_modelRender.SetScale(5.0f, 5.0f, 5.0f);

    return true;
}

void Boss::Update()
{
    if (FindGO<Pouse>("pouse")) return;

    m_modelRender.Update();

    // クールダウン進行
    if (m_attackCooldown > 0.0f) 
    {
        m_attackCooldown -= dt;
        if (m_attackCooldown < 0.0f) m_attackCooldown = 0.0f;
    }

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
    if (dist <= attackRange) 
    {
        int roll = rand() % 100;

        //ボスの攻撃のみ外れやすい
        if (roll < 80) 
        { 
            // 命中率
            m_player->PlayerTakeDamage(2); // 通常敵より強いダメージ
            // 攻撃が命中したときだけメッセージを表示
            if (m_message) {
                auto se = NewGO<SoundSource>(0);
                se->Init(7);       
                se->Play(false);
                m_message->AddMessage("PlayerDamage");
            }

            else
            {
                m_message->AddMessage("Miss");
            }
        }
        m_attackCooldown = m_AttackCooldown;
    }
}

void Boss::EnemyTakeDamage(int dmg)
{
    // ボス用のダメージ処理に渡す
    BossTakeDamage(dmg);
}

void Boss::BossTakeDamage(int dmg)
{
    m_BossHP -= dmg;

    if (m_BossHP <= 0) 
    {
        m_BossHP = 0;

        //ゲームクリアを表示する
        NewGO<GameClear>(0,"gameclear");
        DeleteGO(this);
        DeleteGO(m_game);
    }

}

void Boss::EnemyTurn()
{
    if (!m_game) return;

    static TurnType lastTurn = TurnType::Enemy;
    if (m_game->GetCurrentTurn() != lastTurn) 
    {
        HasMoved = false; // ターン切替時にリセット
    }

    lastTurn = m_game->GetCurrentTurn();

    if (m_game->GetCurrentTurn() != TurnType::Enemy || HasMoved) return;
    if (!m_game->IsEnemyWaitTimeElapsed()) return;

    Vector3 toPlayer = m_game->GetPlayerPosition() - m_position;
    float dist = toPlayer.Length();
    float attackRange = 100.0f;

    if (dist <= attackRange && m_attackCooldown <= 0.0f) 
    {
        // 攻撃
        float angle = atan2f(toPlayer.x, toPlayer.z);
        Quaternion rot; rot.SetRotationY(angle);
        m_modelRender.SetRotation(rot);

        m_modelRender.PlayAnimation(enAnimationClip_BossAttack);
        EnemyAttack();

        HasMoved = true;
        m_attackCooldown = m_AttackCooldown; // クールダウンを設定
        m_game->NextTurn(); // 攻撃後にターン終了
        return;
    }

    // 攻撃範囲外は移動などを Enemy::EnemyTurn() に任せる
    Enemy::EnemyTurn();
}

void Boss::Render(RenderContext& rc)
{
    Enemy::Render(rc);
}