#pragma once
#include <array>
#include <utility>

namespace Vertex
{
	class Vertex
	{
	public:
		explicit Vertex(const double& x, const double& y);
		explicit Vertex(const double& x, const double& y, const std::array<double, 4>& color);
		explicit Vertex(const double& x, const double& y, const double& r, const double& g, const double& b, const double& a);

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

		double getAlpha() const;
		void setAlhpa(const double&);

		std::array<double, 4> getRGBA() const;
		void setRGBA(const std::array<double, 4>&);

		friend Vertex operator-(const Vertex&);

		friend Vertex operator+(const Vertex&, const Vertex&);
		friend Vertex operator-(const Vertex&, const Vertex&);

		friend Vertex operator*(const Vertex&, const double&);
		friend Vertex operator/(const Vertex&, const double&);
		friend Vertex operator*(const double&, const Vertex&);
		friend Vertex operator/(const double&, const Vertex&);

		Vertex& operator+=(const Vertex& secondVertex);
		Vertex& operator-=(const Vertex& secondVertex);
		Vertex& operator*=(const double&);
		Vertex& operator/=(const double&);
	private:
		std::pair<double, double> _coordinate;
		std::array<double, 4> _color;
	};
}