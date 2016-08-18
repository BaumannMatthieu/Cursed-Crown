#pragma once

#include <SFML/Graphics.hpp>

#include "../Core/Log.h"

template<typename ResourceT>
class Resource {
	public:
		Resource() {
		}

		~Resource() {
		}

		bool load(const std::string& name) {
			return m_resource.loadFromFile(name);
		}

		const ResourceT& get() const {
			return m_resource;
		}

	private:
		ResourceT		m_resource;
};