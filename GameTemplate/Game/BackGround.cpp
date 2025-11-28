#include "stdafx.h"
#include "BackGround.h"


BackGround::BackGround()
{
    srand((unsigned)time(nullptr));
    m_stageID = rand() % 3; // 0か1をランダム

    if (m_stageID == 0) {
        modelRender.Init("Assets/modelData/stage1/stage1.tkm");
    }
    if(m_stageID==1) {
        modelRender.Init("Assets/modelData/stage2/stage2.1.tkm");
    }
    if (m_stageID == 2) {
        modelRender.Init("Assets/modelData/stage3/stage3.tkm");
    }

    physicsStaticObject.CreateFromModel(modelRender.GetModel(),
        modelRender.GetModel().GetWorldMatrix());

    // 当たり判定デバッグ用
     //PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
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