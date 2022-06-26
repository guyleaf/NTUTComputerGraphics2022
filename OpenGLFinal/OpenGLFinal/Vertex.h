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

		double getX() const;
		void setX(float);

		double getY() const;
		void setY(float);

		double getZ() const;
		void setZ(float);

		friend bool operator==(const Vertex&, const Vertex&);
	private:
		float _x;
		float _y;
		float _z;
	};
}