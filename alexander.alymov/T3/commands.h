#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "Polygon.h"
#include "StreamGuard.h"

namespace alymov
{

    void lessarea(std::vector< Polygon >& polygon, std::istream& in, std::ostream& out);
    void maxseq(std::vector< Polygon >& polygons, std::istream& in, std::ostream& out);

    void area(std::vector< Polygon >& polygons, std::string& mode, std::ostream& out);
    void max(std::vector< Polygon >& polygons, std::string& mode, std::ostream& out);
    void min(std::vector< Polygon >& polygons, std::string& mode, std::ostream& out);
    void count(std::vector< Polygon >& polygons, std::string& mode, std::ostream& out);

}

#endif
