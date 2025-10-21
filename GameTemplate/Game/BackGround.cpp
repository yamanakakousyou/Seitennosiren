#include "stdafx.h"
#include "BackGround.h"


BackGround::BackGround()
{
    srand((unsigned)time(nullptr));
    m_stageID = rand() % 2; // 0か1をランダム

    if (m_stageID == 0) {
        modelRender.Init("Assets/modelData/stage3/stage1.tkm");
    }
    if(m_stageID==1) {
        modelRender.Init("Assets/modelData/stage4/stage2.1.tkm");
    }
    //if (stageID == 2) {
    //    modelRender.Init("Assets/modelData/stage4/stage2.tkm");
    //}
    //if (stageID == 3) {
    //    modelRender.Init("Assets/modelData/stage4/stage2.tkm");
    //}

    physicsStaticObject.CreateFromModel(modelRender.GetModel(),
        modelRender.GetModel().GetWorldMatrix());

    // 当たり判定デバッグ用
    // PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
}

BackGround::~BackGround()
{
}

void BackGround::Update()
{

}
void BackGround::Render(RenderContext& rc)
{
    modelRender.Draw(rc);
}