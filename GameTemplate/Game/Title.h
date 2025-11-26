#pragma once
class Game;
class Describe;
class Title : public IGameObject
{
public:
    Title();
    ~Title();
    bool Start();
    void Update();
    //描画関数。
    void Render(RenderContext& rc);
    //スプライトレンダ―。
    SpriteRender spriteRender;
    Game* m_game;
    SoundSource* m_soundSource = nullptr;
    Describe* m_describe;
};