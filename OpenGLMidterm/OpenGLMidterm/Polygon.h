#pragma once
#include <vector>

#include "Vertex.h"

namespace Polygon
{
	class Polygon
	{
	public:
		explicit Polygon(const std::vector<Vertex::Vertex>& vertices);

		const std::vector<Vertex::Vertex>& getVertices() const;
	private:
		std::vector<Vertex::Vertex> _vertices;
	};
}