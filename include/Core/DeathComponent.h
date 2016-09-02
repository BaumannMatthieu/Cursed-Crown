#pragma once

#include <vector>
#include <string>
#include <memory>

#include "../Core/Component.h"

struct DeathComponent : public Component {
	
};

using DeathComponentPtr = std::shared_ptr<DeathComponent>;