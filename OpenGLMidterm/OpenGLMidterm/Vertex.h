#pragma once
#include <array>
#include <utility>

namespace Vertex
{
	class Vertex
	{
	public:
		explicit Vertex(const double&, const double&, const double& = 0.0, const double& = 0.0, const double& = 0.0);

		double getX() const;
		void setX(const double&);

		double getY() const;
		void setY(const double&);

		double getRed() const;
		void setRed(const double&);

		double getGreen() const;
		void setGreen(const double&);

		double getBlue() const;
		void setBlue(const double&);

	private:
		std::pair<double, double> _coordinate;
		std::array<double, 3> _color;
	};
}