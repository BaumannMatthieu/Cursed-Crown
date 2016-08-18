#pragma once

#include <vector>
#include <string>
#include <memory>

#include <SFML/Graphics.hpp>

#include "../Core/Component.h"
#include "../Core/Resource.h"

struct PlayAnimationComponent : public Component {
	
};

using PlayAnimationComponentPtr = std::shared_ptr<PlayAnimationComponent>;