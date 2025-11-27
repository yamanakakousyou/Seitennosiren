#pragma once

class Game;
class BackGround;
class Player;
class Enemy;
class Boss;
class Sord;
class Food;
class Pouse;

class Map :public IGameObject
{
public:
	Map();
	~Map();

	bool Start();

	void Update();

	void Render(RenderContext& rc);

private:

	Player* m_player=nullptr;
	Enemy* m_enemy=nullptr;
	Boss* m_boss=nullptr;
	Sord* m_sord=nullptr;
	Food* m_food=nullptr;
	BackGround* m_backGround=nullptr;

	SpriteRender m_back;
	SpriteRender m_back2;
	SpriteRender m_playerIcon;
	SpriteRender m_enemyIcon;
	SpriteRender m_bossIcon;
	SpriteRender m_sordIcon;
	SpriteRender m_meatIcon;
	SpriteRender m_onigiriIcon;

	//Vector3 m_pos;				// ミニマップの表示位置
	//Vector3 m_playerMapPos;		// プレイヤーの点の位置
	//Vector3 m_enemyMapPos;		// 敵の点の位置
	//Vector3 m_bossMapPos;		//ボスの点の位置
	//Vector3 m_sordMapPos;		//剣の点の位置
	//Vector3 m_meatMapPos;		//肉の点の位置
	//Vector3 m_onigiriMapPos;	//おにぎりの点の位置

};

