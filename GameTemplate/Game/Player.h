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
    //�X�V����
    void Update();
    //�`�揈��
    void Render(RenderContext& rc);
    //�ړ�����
    void Move();
    //��]����
    void Rotation();
    //�X�e�[�g�Ǘ�
    void ManageState();
    //�A�j���[�V�����̍Đ�
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
        // �G�l�~�[����v���C���[�Ɍ������ĐL�т�x�N�g�����v�Z����B
        Vector3 V1toV2 = v1 - v2;
        return V1toV2;
    }

    bool HasMoved = false;

    enum EnAnimationClip {		//�A�j���[�V�����B
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

    //��]�p�ϐ�
    Quaternion rot;

    ModelRender     modelRender;
    SpriteRender    m_spriteRender;
    FontRender		m_fontRender;
    FontRender      m_satietyFontRender;
    SpriteRender    m_satietyRender;

    AnimationClip animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B	//�A�j���[�V�����N���b�v�B
    CharacterController m_characterController;  //�L�����N�^�[�R���g���[���[�B

    Vector3 moveSpeed; //�ړ����x�B
    Vector3 m_position = Vector3::Zero;//�|�W�V����
    Quaternion m_rotation;//�N�H�[�^�j�I���B
    Transform* m_transform;
    Game* m_game;
    Enemy* m_enemy = nullptr;
    Boss* m_boss = nullptr;

    int playerState = 0;//�v���C���[�̃X�e�[�g

    int m_PlayerHP = 0;
    int m_PlayerMaxHP = 0;
    int m_satiety = 0;
    int m_PlayerAttackDamage = 2;
    int dmg = 0;
    int m_attackPower = 1;
    float m_attackRange = 0;
    float m_attackCooldown = 1.0f;
    const float kAttackCooldown = 0.5f; // 0.5 �b�̃N�[���_�E��
};
