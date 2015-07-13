#pragma once

#include "mastercontrol.h"
#include <Urho3D/Core/CoreEvents.h>

namespace Urho3D {
class Drawable;
class Node;
class Scene;
class Sprite;
}

using namespace Urho3D;

class Tile;
class WallCollider;

enum TileElement {TE_CENTER = 0, TE_NORTH, TE_EAST, TE_SOUTH, TE_WEST, TE_NORTHWEST, TE_NORTHEAST, TE_SOUTHEAST, TE_SOUTHWEST, TE_LENGTH};
enum CornerType {CT_NONE, CT_IN, CT_OUT, CT_TWEEN, CT_DOUBLE, CT_FILL};
enum TileType {TT_SPACE, TT_EMPTY, TT_ENGINE};

class Level : public Object
{
    OBJECT(Level);
    friend class InputMaster;
public:
    Level(Context *context, Vector3 position, MasterControl* masterControl);

    MasterControl* masterControl_;
    Node* rootNode_;

    bool CheckEmpty(Vector3 coords, bool checkTiles) const { return CheckEmpty(IntVector2(round(coords.x_), round(coords.z_)), checkTiles); }
    bool CheckEmpty(IntVector2 coords, bool checkTiles) const;
    bool CheckEmptyNeighbour(IntVector2 coords, TileElement element, bool tileMap) const;
    IntVector2 GetNeighbourCoords(IntVector2 coords, TileElement element) const;
    CornerType PickCornerType(IntVector2 tileCoords, TileElement element) const;
    TileType GetTileType(IntVector2 coords);
    TileType GetNeighbourType(IntVector2 coords, TileElement element);


    virtual void Start();
    virtual void Stop();
    void AddMissingColliders();
    void FixFringe();
    void FixFringe(IntVector2 coords);

    void AddTile(IntVector2 newTileCoords);
    bool DisableSlot(IntVector2 coords);
    bool EnableSlot(IntVector2 coords);
    void EnableSlots();
    void DisableSlots();
private:
    RigidBody* rigidBody_;
    HashMap<IntVector2, SharedPtr<Tile> > tileMap_;
    HashMap<IntVector2, SharedPtr<WallCollider> > collisionMap_;
    HashMap<IntVector2, TileType> buildingMap_;

    void HandleUpdate(StringHash eventType, VariantMap& eventData);

    bool selected_;

    void Select();
    void Deselect();
    void SetSelected(bool selected);
    bool IsSelected() const;

    void SetTileType(IntVector2 coords, TileType type = TT_ENGINE);
    void EmptyTile(IntVector2 coords) {SetTileType(coords, TT_EMPTY);}
};
