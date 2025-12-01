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

	SpriteRender	m_spriteRender;

	Game*			m_game = nullptr;
};

