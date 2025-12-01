#pragma once

class Game;
class Rule;

class Describe :public IGameObject
{
public:
	Describe();
	~Describe();

	void Update();
	bool Start();

	void Render(RenderContext& rc);

	SpriteRender	m_spriteRender;

	Game*			m_game = nullptr;
	Rule*			m_rule = nullptr;
};

