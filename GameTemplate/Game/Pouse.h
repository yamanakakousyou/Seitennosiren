#pragma once

class Game;
class Title;

class Pouse:public IGameObject
{
public:
	Pouse();
	~Pouse();

	bool Start();

	void Update();

	void Render(RenderContext& rc);

private:

	Game* m_game=nullptr;
	Title* m_title=nullptr;
	SpriteRender m_spriteRender;

};

