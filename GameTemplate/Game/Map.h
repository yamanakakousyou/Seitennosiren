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

public:
	SpriteRender				m_back;
	SpriteRender				m_back2;
	SpriteRender				m_back3;
	SpriteRender				m_playerIcon;
	SpriteRender				m_enemyIcon;
	std::vector<SpriteRender*>	m_enemyIcons;
	SpriteRender				m_bossIcon;
	SpriteRender				m_sordIcon;
	SpriteRender				m_meatIcon;
	SpriteRender				m_onigiriIcon;

private:
	Game*						m_game			= nullptr;
	Player*						m_player		= nullptr;
	Enemy*						m_enemy			= nullptr;
	Boss*						m_boss			= nullptr;
	Sord*						m_sord			= nullptr;
	Food*						m_food			= nullptr;
	BackGround*					m_backGround	= nullptr;
};

