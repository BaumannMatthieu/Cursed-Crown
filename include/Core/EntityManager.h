#pragma once

#include <vector>
#include <algorithm>

#include "../Core/Log.h"
#include "../Core/Entity.h"

class EntityManager {
	public:
		EntityManager() {
		}
		~EntityManager() {
		}

		void add(const EntityPtr entity) {
			if(std::find(m_entitys.begin(), m_entitys.end(), entity) != m_entitys.end()) {
				LOG_ERROR("Entity has already been inserted");
				return;
			}
			m_entitys.push_back(entity);
		}

		std::vector<EntityPtr>& getEntitys() {
			return m_entitys;
		}

	private:
		std::vector<EntityPtr>	m_entitys;
};
