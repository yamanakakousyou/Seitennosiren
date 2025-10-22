#pragma once
#pragma once
class Player;
class Game;

// �H�ו��̎��
enum class FoodType {
	Meat,     // HP��
	Onigiri   // �����x��
};

class Food :public IGameObject
{
public:
	Food();
	~Food();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Rotation();
	void UpdateModelRender();

	void SetType(FoodType type) { m_type = type; }



	const Vector3& GetPosition() const { return m_position; }
	void SetPosition(const Vector3& pos) { m_position = pos; m_modelRender.SetPosition(m_position); }


	Player* m_player = nullptr;
	Game* m_game = nullptr;
	ModelRender			m_modelRender;
	Vector3 m_position = Vector3::Zero;//�|�W�V����

	FoodType m_type = FoodType::Meat; // �f�t�H���g�͓�
	bool			m_isGet = false;
	bool			m_isCollected = false;	//�A�C�e���擾�ς݂��ǂ����̃t���O
};

