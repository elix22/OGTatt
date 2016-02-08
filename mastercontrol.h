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

#ifndef MASTERCONTROL_H
#define MASTERCONTROL_H

#include <Urho3D/Urho3D.h>

#include "luckey.h"

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Sprite;
}

using namespace Urho3D;
using namespace LucKey;

class OGTattCam;
class InputMaster;
class Level;
class Player;

typedef struct GameWorld
{
    SharedPtr<OGTattCam> camera;
    SharedPtr<Scene> scene;
    SharedPtr<Node> voidNode;
    SharedPtr<Player> player_;
    struct {
        SharedPtr<Node> sceneCursor;
        SharedPtr<Cursor> uiCursor;
        PODVector<RayQueryResult> hitResults;
    } cursor;
} GameWorld;

typedef struct Resources
{
    struct {
        struct {
            SharedPtr<Model> male;
            SharedPtr<Model> female;
            Vector< SharedPtr<Model> > hairStyles;
        } characters;
        struct {
            SharedPtr<Model> firePit;
            SharedPtr<Model> streetLight;
        } doodads;
        struct {
            SharedPtr<Model> cookieJar;
        } vehicles;
        struct {
            SharedPtr<Model> blockCenter;
            SharedPtr<Model> blockSide;
            SharedPtr<Model> blockTween;
            SharedPtr<Model> blockTweenCorner;
            SharedPtr<Model> blockInCorner;
            SharedPtr<Model> blockOutCorner;
            SharedPtr<Model> blockFillCorner;
            SharedPtr<Model> blockDoubleCorner;
        } tileParts;
    } models;
    struct {
        Vector<SharedPtr<Material> > skin;
        Vector<SharedPtr<Material> > cloth;
        SharedPtr<Material> darkness;
        SharedPtr<Material> metal;
        SharedPtr<Material> leather;
        SharedPtr<Material> sidewalk;
        SharedPtr<Material> wall;
        SharedPtr<Material> hair;
        SharedPtr<Material> pants;
        SharedPtr<Material> paint;
        SharedPtr<Material> glass;
        SharedPtr<Material> amber;
        SharedPtr<Material> headlights;
        SharedPtr<Material> taillights;
        struct {
            SharedPtr<Material> shroomies;
            SharedPtr<Material> squaters;
            SharedPtr<Material> pirates;
            SharedPtr<Material> ninjas;
            SharedPtr<Material> zionists;
            SharedPtr<Material> rastafari;
            SharedPtr<Material> illuminati;
            SharedPtr<Material> discordians;
            SharedPtr<Material> chaotes;
        } decals;
    } materials;
} Resources;

typedef struct HitInfo
{
    Vector3 position_;
    Vector3 hitNormal_;
    Node* hitNode_;
    Drawable* drawable_;
} HitInfo;

namespace {
StringHash const N_PLAYER = StringHash("Player");
StringHash const N_VOID = StringHash("Void");
StringHash const N_CURSOR = StringHash("Cursor");
StringHash const N_TILEPART = StringHash("TilePart");
StringHash const N_SLOT = StringHash("Slot");
}


enum JoyStickButton {JB_SELECT, JB_LEFTSTICK, JB_RIGHTSTICK, JB_START, JB_DPAD_UP, JB_DPAD_RIGHT, JB_DPAD_DOWN, JB_DPAD_LEFT, JB_L2, JB_R2, JB_L1, JB_R1, JB_TRIANGLE, JB_CIRCLE, JB_CROSS, JB_SQUARE};

class MasterControl : public Application
{
    /// Enable type information.
    URHO3D_OBJECT(MasterControl, Application);
    friend class InputMaster;
    friend class OGTattCam;
public:
    /// Constructor.
    MasterControl(Context* context);
    GameWorld world;
    Resources resources;
    SharedPtr<ResourceCache> cache_;
    SharedPtr<Graphics> graphics_;

    /// Setup before engine initialization. Modifies the engine paramaters.
    virtual void Setup();
    /// Setup after engine initialization.
    virtual void Start();
    /// Cleanup after the main loop. Called by Application.
    virtual void Stop();
    void Exit();

    void CreateSineLookupTable();
    float Sine(float x);
    float Sine(float freq, float min, float max, float shift = 0.0f);
    ///Physics
    bool PhysicsRayCast(PODVector<PhysicsRaycastResult> &hitResults, Ray ray, float distance, unsigned collisionMask = M_MAX_UNSIGNED);
    bool PhysicsSphereCast(PODVector<RigidBody *> &hitResults, Vector3 center, float radius, unsigned collisionMask = M_MAX_UNSIGNED);
private:
    SoundSource* musicSource_;
    SharedPtr<UI> ui_;
    SharedPtr<Renderer> renderer_;
    SharedPtr<XMLFile> defaultStyle_;
    SharedPtr<PhysicsWorld> physicsWorld_;

    /// Set custom window title and icon
    void SetWindowTitleAndIcon();
    /// Create console and debug HUD
    void CreateConsoleAndDebugHud();

    /// Construct the scene content.
    void CreateScene();
    /// Construct user interface elements.
    void CreateUI();
    /// Subscribe to application-wide logic update and post-render update events.
    void SubscribeToEvents();

    /// Handle scene update event to control camera's pitch and yaw.
    void HandleSceneUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle the logic update event.
    void HandleUpdate(StringHash eventType, VariantMap& eventData);
    /// Handle the post-render update event.
    void HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData);

    /// Create a mushroom object at position.
    void CreateDungeon(const Vector3 pos);
    void UpdateCursor(float timeStep);
    /// Utility function to raycast to the cursor position. Return true if hit.
    bool CursorRayCast(double maxDistance, PODVector<RayQueryResult> &hitResults);

    /// Pause flag
    bool paused_;

    ///Sine lookup table
    Vector<double> sine_;
    void LoadResources();
};

#endif // MASTERCONTROL_H
