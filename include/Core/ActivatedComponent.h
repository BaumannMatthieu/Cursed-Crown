#pragma once

#include <vector>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Core/Component.h"

struct ActivatedComponent : public Component {
	
};

using ActivatedComponentPtr = std::shared_ptr<ActivatedComponent>;