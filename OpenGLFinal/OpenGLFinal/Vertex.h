#pragma once
#include <array>
#include <utility>

namespace Graph2D
{
	class Vertex
	{
	public:
		Vertex();
		Vertex(float x, float y, float z);

		float getX() const;
		void setX(float);

		float getY() const;
		void setY(float);

		float getZ() const;
		void setZ(float);

		friend bool operator==(const Vertex&, const Vertex&);
	private:
		float _x;
		float _y;
		float _z;
	};
}