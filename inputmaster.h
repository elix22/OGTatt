#pragma once

#include <Urho3D/Input/Input.h>

#include "mastercontrol.h"
#include "level.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Sprite;
}

using namespace Urho3D;

class InputMaster : public Object
{
    OBJECT(InputMaster);
public:
    InputMaster(Context* context, MasterControl* masterControl);
    WeakPtr<Node> firstHit_;

    void DeselectAll();
private:
    MasterControl* masterControl_;

    Input* input_;
    void HandleMouseDown(StringHash eventType, VariantMap &eventData);
    void HandleKeyDown(StringHash eventType, VariantMap &eventData);
    void HandleMouseUp(StringHash eventType, VariantMap &eventData);

    Vector<SharedPtr<Level> > selectedDungeons_;
    void SetSelection(SharedPtr<Level> platform);
};