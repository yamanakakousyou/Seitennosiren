#pragma once
#pragma once
class Player;
class Game;

// 食べ物の種類
enum class FoodType {
	Meat,     // HP回復
	Onigiri   // 満腹度回復
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
	Vector3 m_position = Vector3::Zero;//ポジション

	FoodType m_type = FoodType::Meat; // デフォルトは肉
	bool			m_isGet = false;
	bool			m_isCollected = false;	//アイテム取得済みかどうかのフラグ
};

