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

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <Urho3D/Urho3D.h>

#include "mastercontrol.h"

namespace Urho3D {
class Node;
class Scene;
class Sprite;
}

using namespace Urho3D;

class SceneObject : public Object
{
    URHO3D_OBJECT(SceneObject, Object);
public:
    SceneObject(Context *context, MasterControl* masterControl);

    Vector3 GetPosition() {return rootNode_->GetWorldPosition();}
protected:
    MasterControl* masterControl_;
    void HandleSceneUpdate(StringHash eventType, VariantMap &eventData);
    SharedPtr<Node> rootNode_;
    void Disable();
    void Set(Vector3 position);
    float randomizer_;
};

#endif // SCENEOBJECT_H
