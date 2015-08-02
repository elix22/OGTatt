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

#include "npc.h"

NPC::NPC(Context *context, MasterControl *masterControl, Vector3 pos):
    SceneObject(context, masterControl),
    maxHealth_{100.0f},
    health_{maxHealth_}
{
    rootNode_->SetName("NPC");
    rootNode_->SetPosition(pos);

    rigidBody_ = rootNode_->CreateComponent<RigidBody>();
    rigidBody_->SetLinearFactor(Vector3::ONE - Vector3::UP);
    rigidBody_->SetLinearDamping(0.9f);
    rigidBody_->SetAngularFactor(Vector3::UP);
    rigidBody_->SetAngularDamping(0.9f);
    rigidBody_->SetLinearRestThreshold(0.023f);

    collisionShape_ = rootNode_->CreateComponent<CollisionShape>();
    collisionShape_->SetCylinder(0.4f, 0.5f);
}
