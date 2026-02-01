#pragma once

#include <vector>

#include "Components/PhysicsComp.hpp"
#include "Components/PositionComp.hpp"
#include "Components/RenderableComp.hpp"
#include "Components/SoundComp.hpp"
#include "Entity.hpp"

class ShipEntity : public Entity<PositionComp, RenderableComp, SoundComp, SpherePhysicsComp> {
public:
    static std::vector<ShipEntity> instances;
};