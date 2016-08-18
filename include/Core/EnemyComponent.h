#pragma once

#include <vector>
#include <string>
#include <memory>

#include "../Core/Component.h"

struct EnemyComponent : public Component {
};

using EnemyComponentPtr = std::shared_ptr<EnemyComponent>;