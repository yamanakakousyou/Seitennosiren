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

public:
	SpriteRender	m_spriteRender;

private:
	Game*			m_game = nullptr;
	Rule*			m_rule = nullptr;
};

