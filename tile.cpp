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

#include <Urho3D/Urho3D.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Input/Input.h>

#include "tile.h"
#include "hemp.h"
#include "grass.h"
#include "firepit.h"
#include "streetlight.h"

Tile::Tile(Context *context, const IntVector2 coords, Level *platform):
Object(context)
{
    masterControl_ = platform->masterControl_;
    dungeon_ = platform;
    coords_ = coords;

    rootNode_ = dungeon_->rootNode_->CreateChild("Tile");
    rootNode_->SetPosition(Vector3((double)coords_.x_, 0.0f, -(double)coords_.y_));

    //Create random tile addons
    int extraRandomizer = Random(23);
    //Create lamps
    if (coords_.x_%6 == 0 && coords.y_%5 == 0){
        new StreetLight(context_, masterControl_, this);
    }
    //Create random plants
    else if (coords_.y_%10 == 2 || coords_.y_%10 == 3){
        for (int i = 0; i < extraRandomizer/2; i++){
            Vector3 randomPosition = Vector3(Random(-0.4f, 0.4f), 0.0f, Random(-0.4f, 0.4f));
            new Hemp(context_, masterControl_, rootNode_, randomPosition);
        }
    }

    /*for (int i = 0; i < (23-extraRandomizer)+16; i++){
        Vector3 randomPosition = Vector3(Random(-0.4f, 0.4f), 0.0f, Random(-0.4f, 0.4f));
        new Grass(context_, masterControl_, rootNode_, randomPosition);
    }*/

    //Set up center and edge nodes.
    for (int i = 0; i <= TE_LENGTH; i++){
        elements_[i] = rootNode_->CreateChild("TilePart");
        int nthOfType = ((i-1)%4);
        if (i > 0) elements_[i]->Rotate(Quaternion(0.0f, 90.0f-nthOfType*90.0f, 0.0f));
        //Add the right model to the node
        StaticModel* model = elements_[i]->CreateComponent<StaticModel>();
        switch (i){
        case TE_CENTER:    model->SetModel(masterControl_->resources.models.tileParts.blockCenter);
            model->SetMaterial(masterControl_->resources.materials.floor);
            break;
        case TE_NORTH:case TE_EAST:case TE_SOUTH:case TE_WEST:
            model->SetModel(masterControl_->resources.models.tileParts.blockTween);
            model->SetMaterial(0, masterControl_->resources.materials.floor);
            break;
        case TE_NORTHEAST:case TE_SOUTHEAST:case TE_SOUTHWEST:case TE_NORTHWEST:
            model->SetModel(masterControl_->resources.models.tileParts.blockOutCorner);
            model->SetMaterial(0, masterControl_->resources.materials.wall);
            if (model->GetNumGeometries() > 2)
            model->SetMaterial(1, masterControl_->resources.materials.darkness);
            break;
        default:break;
        }


        model->SetCastShadows(true);
    }

    //SubscribeToEvent(E_UPDATE, HANDLER(Tile, HandleUpdate));
}

void Tile::Start()
{
}
void Tile::Stop()
{
}

void Tile::HandleUpdate(StringHash eventType, VariantMap &eventData)
{

}

void Tile::SetBuilding(TileType type)
{
    Vector<SharedPtr<Node> > children = rootNode_->GetChildren();
    for (unsigned i = 0; i < children.Size(); i++)
        if (children[i]->GetNameHash() != N_TILEPART)
            children[i]->SetEnabledRecursive(false);

    buildingType_ = type;
    if (buildingType_ > TT_EMPTY) dungeon_->DisableSlot(coords_);
    StaticModel* model = elements_[0]->GetComponent<StaticModel>();
    switch (buildingType_)
    {
    case TT_ENGINE: {
        //model->SetModel(masterControl_->cache_->GetResource<Model>("Resources/Models/Engine_center.mdl"));
        //model->SetMaterial(0,masterControl_->resources.materials.floor);
    } break;
    default: break;
    }
}

TileType Tile::GetBuilding()
{
    return buildingType_;
}

//Fix this tile's element models and materials according to
void Tile::FixFringe()
{
    for (int element = 1; element < TE_LENGTH; element++)
    {
        StaticModel* model = elements_[element]->GetComponent<StaticModel>();
        //Fix sides
        if (element <= 4){
            //If corresponding neighbour is empty
            if (dungeon_->CheckEmptyNeighbour(coords_, (TileElement)element, true))
            {
                model->SetModel(masterControl_->resources.models.tileParts.blockSide);
                model->SetMaterial(0, masterControl_->resources.materials.wall);
                model->SetMaterial(1, masterControl_->resources.materials.darkness);
            }
            //If neighbour is not empty
            else {
                if (element == 1) {
                    switch (GetBuilding())
                    {
                    case TT_ENGINE : if (dungeon_->GetNeighbourType(coords_, (TileElement)element) == TT_ENGINE) model->SetModel(SharedPtr<Model>()); break;
                    default :
                        model->SetModel(masterControl_->resources.models.tileParts.blockTween);
                        model->SetMaterial(0, masterControl_->resources.materials.floor);
                        break;
                    }
                }
                else if (element == 4) {
                    model->SetModel(masterControl_->resources.models.tileParts.blockTween);
                    model->SetMaterial(0, masterControl_->resources.materials.floor);
                }
                else model->SetModel(SharedPtr<Model>());
            }
        }
        //Fix corners
        else {
            switch (dungeon_->PickCornerType(coords_, (TileElement)element)){
            case CT_NONE:   model->SetModel(SharedPtr<Model>()); break;
            case CT_IN:
                model->SetModel(masterControl_->resources.models.tileParts.blockInCorner);
                model->SetMaterial(0, masterControl_->resources.materials.wall);
                model->SetMaterial(1, masterControl_->resources.materials.darkness);
                break;
            case CT_OUT:
                model->SetModel(masterControl_->resources.models.tileParts.blockOutCorner);
                model->SetMaterial(0, masterControl_->resources.materials.wall);
                break;
            case CT_TWEEN:
                model->SetModel(masterControl_->resources.models.tileParts.blockTweenCorner);
                model->SetMaterial(0, masterControl_->resources.materials.wall);
                model->SetMaterial(1, masterControl_->resources.materials.darkness);
                break;
            case CT_DOUBLE:
                model->SetModel(masterControl_->resources.models.tileParts.blockDoubleCorner);
                model->SetMaterial(0, masterControl_->resources.materials.wall);
                break;
            case CT_FILL:
                model->SetModel(masterControl_->resources.models.tileParts.blockFillCorner);
                model->SetMaterial(0, masterControl_->resources.materials.floor);
                break;
            default: break;
            }
        }
    }
}






















