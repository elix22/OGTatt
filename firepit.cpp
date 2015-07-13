#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Resource/ResourceCache.h>

#include "firepit.h"

FirePit::FirePit(Context* context, MasterControl* masterControl, Tile* tile):
    Deco(context, masterControl, tile, "FirePit")
{
    rootNode_->SetRotation(Quaternion(0.0f, randomizer_ * 360.0f, 0.0f));

    StaticModel* model_ = rootNode_->CreateComponent<StaticModel>();
    model_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/FirePit.mdl"));
    model_->SetMaterial(0, masterControl_->cache_->GetResource<Material>("Resources/Materials/Metal.xml"));
    model_->SetMaterial(1, masterControl_->resources.materials.darkness);
    model_->SetMaterial(2, masterControl_->cache_->GetResource<Material>("Resources/Materials/Amber.xml"));
    model_->SetCastShadows(true);

    rootNode_->CreateComponent<RigidBody>();
    CollisionShape* collider = rootNode_->CreateComponent<CollisionShape>();
    collider->SetCylinder(0.4f, 0.5f);

    Node* particleNode = rootNode_->CreateChild("Fire");
    particleNode->SetPosition(Vector3::UP * 0.16f);
    ParticleEmitter* flameEmitter = particleNode->CreateComponent<ParticleEmitter>();
    flameEmitter->SetEffect(masterControl_->cache_->GetResource<ParticleEffect>("Resources/Particles/fire1.xml"));
    ParticleEmitter* sparkEmitter = particleNode->CreateComponent<ParticleEmitter>();
    sparkEmitter->SetEffect(masterControl_->cache_->GetResource<ParticleEffect>("Resources/Particles/fire_sparks.xml"));

    lightNode_ = rootNode_->CreateChild("LightNode");
    light_ = lightNode_->CreateComponent<Light>();
    light_->SetColor(Color(1.0f, 0.6f, 0.4f));
    light_->SetRange(5.0f);
    light_->SetCastShadows(true);
    light_->SetShadowBias(BiasParameters(0.00005f, 0.5f));    
    light_->SetShadowCascade(CascadeParameters(1.0f, 2.0f, 3.0f, 5.0f, 0.5f));
    light_->SetShadowResolution(0.25f);

    SubscribeToEvent(E_SCENEUPDATE, HANDLER(FirePit, HandleSceneUpdate));
}

void FirePit::HandleSceneUpdate(StringHash eventType, VariantMap& eventData)
{
    float range = 0.001f;
    float x = 0.0f;
    for (int i = 1; i < 9; i++)
        x += masterControl_->Sine(4.0f + i, -range, range, i+(i*randomizer_ * 1.0f*M_PI))/(i*0.666f);
    float y = 0.5f;
    for (int i = 1; i < 9; i++)
        y += masterControl_->Sine(5.0f + i, -range, range, i+(i*randomizer_ * 1.5f*M_PI))/(i*0.666f);
    float z = 0.0f;
    for (int i = 1; i < 9; i++)
        z += masterControl_->Sine(6.0f + i, -range, range, i+(i*randomizer_ * 2.0f*M_PI))/(i*0.666f);
    lightNode_->SetPosition(x, y, z);
    float brightness = 1.0f;
    for (int i = 1; i < 5; i++)
    {
        brightness += masterControl_->Sine(randomizer_ + 7.123f + i, -0.001f, 0.023f, (randomizer_ * 2.0f*M_PI) + i*(0.2f+randomizer_));//((5+i)*0.2f);
    }
    light_->SetBrightness(brightness);
}
