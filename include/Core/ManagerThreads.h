#pragma once

#include <memory>
#include "../Core/WrapperCoroutine.h"

static ManagerCoroutinePtr manager_threads = std::make_shared<ManagerCoroutine>();
