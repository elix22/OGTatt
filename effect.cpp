/* OG Tatt
// Copyright (C) 2016 LucKey Productions (luckeyproductions.nl)
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

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/ParticleEffect.h>

#include "effect.h"

Effect::Effect(Context *context, MasterControl *masterControl, Vector3 position, String name):
    SceneObject(context, masterControl)
{
    rootNode_->SetName(name);
    rootNode_->SetPosition(position);
    particleEmitter_ = rootNode_->CreateComponent<ParticleEmitter>();

    //Subscribe to update
    SubscribeToEvent(E_SCENEUPDATE, URHO3D_HANDLER(Effect, HandleSceneUpdate));
}

void Effect::HandleSceneUpdate(StringHash eventType, VariantMap &eventData)
{
    using namespace Update;
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    age_ += timeStep;
    if (age_ > emitTime_ + particleEmitter_->GetEffect()->GetMaxTimeToLive()){
        Disable();
    }
    else if (age_ > emitTime_){
        particleEmitter_->SetEmitting(false);
    }
}
