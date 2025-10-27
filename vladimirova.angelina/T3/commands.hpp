#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <vector>
#include "shapes.hpp"

namespace vladimirova
{
	void processArea(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
	void processMax(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
	void processMin(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
	void processCount(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
	void processEcho(std::istream& in, std::ostream& out, std::vector<Polygon>& polygons);
	void processMaxSequence(std::istream& in, std::ostream& out, const std::vector<Polygon>& polygons);
}

#endif
