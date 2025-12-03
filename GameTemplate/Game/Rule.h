#pragma once

class Game;

class Rule:public IGameObject
{
public:
	Rule();
	~Rule();

	bool Start();

	void Update();

	void Render(RenderContext& rc);
	
public:
	SpriteRender	m_spriteRender;

private:
	Game*			m_game = nullptr;
};

