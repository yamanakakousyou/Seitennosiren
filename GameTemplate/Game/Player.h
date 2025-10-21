#pragma once

class Game;
class Enemy;
class Boss;
class Transform;
class GameOver;
class Player :public IGameObject
{
public:
    Player();
    ~Player();

    bool Start();
    //更新処理
    void Update();
    //描画処理
    void Render(RenderContext& rc);
    //移動処理
    void Move();
    //回転処理
    void Rotation();
    //ステート管理
    void ManageState();
    //アニメーションの再生
    void PlayAnimation();

    void PlayerMoveTurn();

    void PlayerUI();

    void PlayerAttack();

    void PlayerHeal(int recovery);

    void PlayerTakeDamage(int dmgp);

    void SetEnemy(Enemy* enemy);

    void SetBoss(Boss* boss);

    const Vector3& GetPosition() const { return m_position; }
    void SetPosition(const Vector3& pos) { m_position = pos; modelRender.SetPosition(m_position); }

    Vector3 RangeCalcV1toV2(const Vector3& v1, const Vector3& v2) {
        // エネミーからプレイヤーに向かって伸びるベクトルを計算する。
        Vector3 V1toV2 = v1 - v2;
        return V1toV2;
    }

    bool HasMoved = false;

    enum EnAnimationClip {		//アニメーション。
        enAnimationClip_Idle,
        enAnimationClip_Walk,
        enAnimationClip_Attack,
        enAnimationClip_Die,
        enAnimationClip_Num,
    };

    void SetRotation(const Quaternion& rotation)
    {
        m_rotation = rotation;
    }

    Transform* GetTransform()
    {
        return m_transform;
    }

    void AttackPower(int value);

    //回転用変数
    Quaternion rot;

    ModelRender     modelRender;
    SpriteRender    m_spriteRender;
    FontRender		m_fontRender;
    FontRender      m_satietyFontRender;
    SpriteRender    m_satietyRender;

    AnimationClip animationClips[enAnimationClip_Num];		//アニメーションクリップ。	//アニメーションクリップ。
    CharacterController m_characterController;  //キャラクターコントローラー。

    Vector3 moveSpeed; //移動速度。
    Vector3 m_position = Vector3::Zero;//ポジション
    Quaternion m_rotation;//クォータニオン。
    Transform* m_transform;
    Game* m_game;
    Enemy* m_enemy = nullptr;
    Boss* m_boss = nullptr;

    int playerState = 0;//プレイヤーのステート

    int m_PlayerHP = 0;
    int m_PlayerMaxHP = 0;
    int m_satiety = 0;
    int m_PlayerAttackDamage = 2;
    int dmg = 0;
    int m_attackPower = 1;
    float m_attackRange = 0;
    float m_attackCooldown = 1.0f;
    const float kAttackCooldown = 0.5f; // 0.5 秒のクールダウン
};
