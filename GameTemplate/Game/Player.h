#pragma once

class Game;
class Enemy;
class Boss;
class Transform;
class GameOver;
class Message;
class Pouse;

enum class ItemType
{
    Meat,
    Onigiri
};

enum class PlayerState 
{
    Idle,
    Walk,
    Attack
};

class Player :public IGameObject
{
private:

    //アニメーション。
    enum EnAnimationClip
    {
        enAnimationClip_Idle,
        enAnimationClip_Walk,
        enAnimationClip_Attack,
        enAnimationClip_Die,
        enAnimationClip_Num,
    };

    // インベントリ内のアイテム情報
    struct InventoryItem 
    {
        ItemType type;       // アイテムの種類
        std::string name;    // 表示名（例："肉"、"おにぎり"）
    };

public:
    Player();
    ~Player();

    bool Start();
    //更新処理
    void Update();
    //描画処理
    void Render(RenderContext& rc);
    //移動処理
    //void Move();
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

    void PlayerEat(int amount);

    void PlayerTakeDamage(int dmgp);

    void SetEnemy(Enemy* enemy);

    void SetBoss(Boss* boss);

    const Vector3& GetPosition() const { return m_position; }
    void SetPosition(const Vector3& pos) { m_position = pos; modelRender.SetPosition(m_position); }

    Vector3 RangeCalcV1toV2(const Vector3& v1, const Vector3& v2) 
    {
        // エネミーからプレイヤーに向かって伸びるベクトルを計算する。
        Vector3 V1toV2 = v1 - v2;
        return V1toV2;
    }

    void SetRotation(const Quaternion& rotation)
    {
        m_rotation = rotation;
    }

    Transform* GetTransform()
    {
        return m_transform;
    }

    void AttackPower(int value);

    void AddItem(ItemType type);                                // アイテムを拾ったときに追加
    void UseItem(int index);                                    // 指定番号のアイテムを使用
    void Invebtory();                                           //インベントリ
    void DrawInventory(RenderContext& rc);                      // インベントリの描画

public:
    //回転用変数
    Quaternion      m_rot;
    ModelRender     modelRender;
    SpriteRender    m_spriteRender;
    FontRender      m_pouseFontRender;
    FontRender		m_fontRender;
    FontRender      m_satietyFontRender;
    FontRender      m_itemFontRender;
    SpriteRender    m_satietyRender;

    AnimationClip                                           animationClips[enAnimationClip_Num];		//アニメーションクリップ。	//アニメーションクリップ。
    CharacterController                                     m_characterController;                      //キャラクターコントローラー。
    PlayerState                                             m_state = PlayerState::Idle;

    Vector3                                                 moveSpeed; //移動速度。
    Vector3                                                 m_position = Vector3::Zero;                 //ポジション
    std::vector<InventoryItem>                              m_inventory;                                // プレイヤーが持っているアイテム一覧
    Quaternion                                              m_rotation;                                 //クォータニオン。
    Transform*                                              m_transform;

private:
    Game*           m_game          =nullptr;
    Enemy*          m_enemy         = nullptr;
    Boss*           m_boss          = nullptr;
    Message*        m_message       = nullptr;
    Pouse*          m_pouse         = nullptr;
    SoundSource*    m_soundSource   = nullptr;
    SoundSource*    m_attackSE      = nullptr;
    SoundSource*    m_damageSE      = nullptr;

    // 入力の前回状態を記録
    bool            m_prevUp            = false;
    bool            m_prevDown          = false;
    bool            m_prevA             = false;
    bool            m_prevB             = false;
    bool            HasMoved            = false;
    bool            m_isInventoryOpen   = false;                  // インベントリを開いているかどうか

    int             m_PlayerHP = 0;
    int             m_PlayerMaxHP = 0;
    int             m_satiety = 0;
    int             m_PlayerAttackDamage = 2;
    int             dmg = 0;
    int             m_attackPower = 1;
    int             m_playerState = 0;                          //プレイヤーのステート
    int             m_selectedItemIndex = 0;
    int             m_currentAnim = -1;                         // 現在再生中のアニメ番号
    int             m_inventoryCursor = 0;

    float           m_attackRange = 0;
    float           m_attackCooldown = 1.0f;
    float           m_attackTimer = 0.0f;
    float           m_moveTimer = 0.0f;
    const float     m_AttackCooldown = 0.5f;                    // 0.5 秒のクールダウン
};
