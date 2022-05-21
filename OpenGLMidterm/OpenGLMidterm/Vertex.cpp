#include <array>
#include <utility>

#include "Vertex.h"

namespace Vertex
{
	Vertex::Vertex(const double& x, const double& y)
		: _coordinate(std::make_pair(x, y)), _color{ 0.0, 0.0, 0.0, 1.0 }
	{
	}

	Vertex::Vertex(const double& x, const double& y, const std::array<double, 4>& color)
		: _coordinate(std::make_pair(x, y)), _color{ color }
	{
	}

	Vertex::Vertex(const double& x, const double& y, const double& r, const double& g, const double& b, const double& a)
		: _coordinate(std::make_pair(x, y)), _color{r, g, b, a}
	{
	}

	double Vertex::getX() const
	{
		return this->_coordinate.first;
	}

	void Vertex::setX(const double& value)
	{
		this->_coordinate.first = value;
	}

	double Vertex::getY() const
	{
		return _coordinate.second;
	}

	void Vertex::setY(const double& value)
	{
		this->_coordinate.second = value;
	}

	double Vertex::getRed() const
	{
		return _color[0];
	}

	void Vertex::setRed(const double& value)
	{
		_color[0] = value;
	}

	double Vertex::getGreen() const
	{
		return _color[1];
	}

	void Vertex::setGreen(const double& value)
	{
		_color[1] = value;
	}

	double Vertex::getBlue() const
	{
		return _color[2];
	}

	void Vertex::setBlue(const double& value)
	{
		_color[2] = value;
	}

	double Vertex::getAlpha() const
	{
		return _color[3];
	}

	void Vertex::setAlhpa(const double& value)
	{
		_color[3] = value;
	}

	std::array<double, 4> Vertex::getRGBA() const
	{
		return _color;
	}

	void Vertex::setRGBA(const std::array<double, 4>& value)
	{
		_color = value;
	}

	Vertex operator-(const Vertex& vertex)
	{
		Vertex newVertex{ vertex };
		newVertex._coordinate.first = -newVertex._coordinate.first;
		newVertex._coordinate.second = -newVertex._coordinate.second;
		return newVertex;
	}

	Vertex operator+(const Vertex& rhl, const Vertex& rhs)
	{
		return Vertex(rhl) += rhs;
	}

	Vertex operator-(const Vertex& rhl, const Vertex& rhs)
	{
		return Vertex(rhl) -= rhs;
	}

	Vertex operator*(const Vertex& rhl, const double& rhs)
	{
		return Vertex(rhl) *= rhs;
	}

	Vertex operator*(const double& rhl, const Vertex& rhs)
	{
		return Vertex(rhs) *= rhl;
	}

	Vertex operator/(const Vertex& rhl, const double& rhs)
	{
		return Vertex(rhl) /= rhs;
	}

	Vertex operator/(const double& rhl, const Vertex& rhs)
	{
		return Vertex(rhs) *= (1.0 / rhl);
	}

	Vertex& Vertex::operator+=(const Vertex& rhv)
	{
		this->_coordinate.first += rhv._coordinate.first;
		this->_coordinate.second += rhv._coordinate.second;

		return *this;
	}

	Vertex& Vertex::operator-=(const Vertex& rhv)
	{
		this->_coordinate.first -= rhv._coordinate.first;
		this->_coordinate.second -= rhv._coordinate.second;

		return *this;
	}

	Vertex& Vertex::operator*=(const double& scalar)
	{
		this->_coordinate.first *= scalar;
		this->_coordinate.second *= scalar;

		return *this;
	}

	Vertex& Vertex::operator/=(const double& scalar)
	{
		this->_coordinate.first /= scalar;
		this->_coordinate.second /= scalar;

		return *this;
	}
}