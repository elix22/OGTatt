#pragma once

#include "mastercontrol.h"
#include <Urho3D/Urho3D.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Math/Plane.h>
#include <Urho3D/Graphics/Viewport.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Sprite;
}

using namespace Urho3D;

class Imp : public Object
{
    OBJECT(Imp);
public:
    Imp(Context *context, MasterControl* masterControl, Urho3D::Node *parent, Vector3 pos);

    virtual void Start();
    virtual void Stop();
private:
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    MasterControl* masterControl_;
    Node* rootNode_;
    Node* spinNode_;
    AnimatedModel* impModel_;
    double randomizer_;
    Animation* smokeAnim_;
};
