#pragma once
#include "CameraEntity.hpp"
#include "LightEntity.hpp"
#include "ShipEntity.hpp"
#include "PropEntity.hpp"
#include "SkyboxEntity.hpp"

template <typename... EntityTs> class EntityRegistry {};

using EntityRegistryDefinition = EntityRegistry<CameraEntity, LightEntity, ShipEntity, PropEntity, SkyboxEntity>;
