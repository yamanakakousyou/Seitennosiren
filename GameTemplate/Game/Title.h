#pragma once
class Game;
class Title : public IGameObject
{
public:
    Title();
    ~Title();
    void Update();
    //描画関数。
    void Render(RenderContext& rc);
    //スプライトレンダ―。
    SpriteRender spriteRender;
    Game* m_game;
};