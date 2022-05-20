#include <utility>

#include "Vertex.h"

namespace Vertex
{
	Vertex::Vertex(const double& x, const double& y, const double& r = 0.0, const double& g = 0.0, const double& b = 0.0)
		: _coordinate(std::make_pair(x, y)), _color{r, g, b}
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
}