/* OG Tatt
// Copyright (C) 2015 LucKey Productions (luckeyproductions.nl)
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "bullet.h"

Bullet::Bullet(Context *context, MasterControl *masterControl):
    SceneObject(context, masterControl),
    lifetime_{1.0f},
    damage_{0.25f}
{
    rootNode_->SetName("Bullet");
    rootNode_->SetScale(Vector3(1.0f, 1.0f, 0.1f));
    model_ = rootNode_->CreateComponent<StaticModel>();
    model_->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Bullet.mdl"));
    model_->SetMaterial(masterControl_->cache_->GetResource<Material>("Resources/Materials/Bullet.xml"));

    rigidBody_ = rootNode_->CreateComponent<RigidBody>();
    rigidBody_->SetMass(0.5f);
    rigidBody_->SetLinearFactor(Vector3::ONE - Vector3::UP);
    rigidBody_->SetFriction(0.0f);

    SubscribeToEvent(E_SCENEUPDATE, HANDLER(Bullet, HandleSceneUpdate));
    SubscribeToEvent(E_POSTRENDERUPDATE, HANDLER(Bullet, HandlePostRenderUpdate));
}

void Bullet::HandleSceneUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace SceneUpdate;

    float timeStep = eventData[P_TIMESTEP].GetFloat();

    age_ += timeStep;
    rootNode_->SetScale(Vector3(Max(1.75f - 10.0f*age_, 1.0f),
                                Max(1.75f - 10.0f*age_, 1.0f),
                                Min(Min(150.0f*age_, 7.0f), Max(7.0f-timeSinceHit_*42.0f, 0.1f))
                                ));
    if (age_ > lifetime_) {
        Disable();
    }
}

void Bullet::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
    using namespace PostRenderUpdate;

    float timeStep = eventData[P_TIMESTEP].GetFloat();
    if (timeStep > 0.0f && !fading_) HitCheck(timeStep);
}

void Bullet::Disable()
{
    fading_ = true;
    rootNode_->SetEnabled(false);
}

void Bullet::HitCheck(float timeStep) {
    if (!fading_) {
        PODVector<PhysicsRaycastResult> hitResults;
        Ray bulletRay(rootNode_->GetPosition(), rootNode_->GetDirection());
        if (masterControl_->PhysicsRayCast(hitResults, bulletRay, rigidBody_->GetLinearVelocity().Length()*timeStep*1.1f, M_MAX_UNSIGNED)){
            for (int i = 0; i < hitResults.Size(); i++){
                if (!hitResults[i].body_->IsTrigger() && hitResults[i].body_->GetNode()->GetNameHash() != N_PLAYER){
                    hitResults[i].body_->ApplyImpulse(rigidBody_->GetLinearVelocity()*0.023f - 0.23f*hitResults[i].normal_,
hitResults[i].position_ - hitResults[i].body_->GetNode()->GetWorldPosition());
                    new HitFX(context_, masterControl_, hitResults[i].position_);
                    //Deal damage
                    unsigned hitID = hitResults[i].body_->GetNode()->GetID();
                    Disable();
                }
            }
        }
    }
}
