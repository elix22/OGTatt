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

#include "tile.h"
#include "hemp.h"
#include "grass.h"
#include "firepit.h"
#include "streetlight.h"

Tile::Tile(TileInfo info, Level *level):
    Object(MC->GetContext()),
    info_{info}
{
    level_ = level;

    rootNode_ = level_->rootNode_->CreateChild("Tile");
    rootNode_->SetPosition(Vector3(static_cast<float>(info_.coords_.x_),
                                   static_cast<float>(info_.coords_.y_),
                                   static_cast<float>(-info_.coords_.z_)));
//    rootNode_->SetScale(Vector3(1.0f, 4.0f, 1.0f));

    if (info_.obstacle_){
        CollisionShape* collider{level_->rootNode_->CreateComponent<CollisionShape>()};
        collider->SetBox(Vector3::ONE, rootNode_->GetPosition());
    }
    if (info_.modelName_.Length()){
        StaticModel* model{rootNode_->CreateComponent<StaticModel>()};
        model->SetModel(MC->cache_->GetResource<Model>("Models/"+info.modelName_+".mdl"));
        model->SetCastShadows(true);

        if (info_.materialName_.Length()){
            model->SetMaterial(MC->cache_->GetResource<Material>("Materials/"+info.materialName_+".xml"));
        }
    }
}
