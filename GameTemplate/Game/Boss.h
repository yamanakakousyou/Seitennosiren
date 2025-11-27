#pragma once
#include "Enemy.h"
class Player;
class Game;
class Enemy;
class GameClear;
class Message;
class Pouse;
class Boss : public Enemy
{
public:
    Boss();
    ~Boss();

    bool Start() override;
    void Update() override;
    void EnemyAttack() override;
    void EnemyTakeDamage(int dmg) override;

    void BossTakeDamage(int dmg);
    void EnemyTurn() override;
    void Render(RenderContext& rc)override;

    void SetPlayer(Player* player)override;

    const Vector3& GetPosition() const { return m_position; }
    void SetPosition(const Vector3& pos) { m_position = pos; m_modelRender.SetPosition(m_position); }

    enum EnAnimationClip {		//アニメーション。
        enAnimationClip_BossIdle,
        enAnimationClip_BossAttack,
        enAnimationClip_Num,
    };

private:

    GameClear* m_gameclear = nullptr;
    Message* m_message;
    int m_BossHP = 0;
};
