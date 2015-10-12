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

#ifndef PEDESTRIAN_H
#define PEDESTRIAN_H

#include <Urho3D/Urho3D.h>

#include "mastercontrol.h"
#include "npc.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Sprite;
}

using namespace Urho3D;

class Pedestrian : public NPC
{
    OBJECT(Pedestrian);
public:
    Pedestrian(Context *context, MasterControl* masterControl, Vector3 pos);
protected:
    Node* modelNode_;
    AnimatedModel* bodyModel_;
    AnimationController* animCtrl_;
    bool male_;
    Vector<Color> colors_;
private:
    void HandleUpdate(StringHash eventType, VariantMap &eventData);

    float sinceLastTurn_;
    float turnInterval_;
    Vector3 direction_;
    float walkSpeed_;
};

#endif //PEDESTRIAN_H
