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

#ifndef HEMP_H
#define HEMP_H

#include <Urho3D/Urho3D.h>

#include "mastercontrol.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Sprite;
}

using namespace Urho3D;

class Hemp : public Object
{
    URHO3D_OBJECT(Hemp, Object);
public:
    Hemp(Urho3D::Node *parent, Vector3 pos);
    Hemp() : Hemp(MC->world.scene, Vector3::ZERO){}
    virtual void Start();
    virtual void Stop();
private:
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    Node* rootNode_;
    StaticModel* fropModel_;
    Vector3 scale_;

    double growthStart_;

    double age_ = 0.0;
};

#endif // HEMP_H
