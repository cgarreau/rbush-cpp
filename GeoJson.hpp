#ifndef GEOJSON_HPP
#define GEOJSON_HPP


#include <vector>
#include <map>
#include <string>


namespace geojson
{
  struct Point
  {
    double x;
    double y;
  };

  struct Bbox
  {
    double minX;
    double minY;
    double maxX;
    double maxY;
  };

  typedef std::vector<Point> MultiPoint;
  typedef std::vector<Point> LineString;     // array of two or more positions
  typedef LineString LinearRing;             // A LinearRing is closed LineString with 4 or more positions. The first and last positions are equivalent
  typedef std::vector<LineString> MultiLineString;
  typedef std::vector<LinearRing> Polygon;   //For Polygons with multiple rings, the first must be the exterior ring and any others must be interior rings or holes
  typedef std::vector<Polygon> MultiPolygon;
  
  typedef std::map<std::string, std::string> Properties;
  
  std::string to_string(const Polygon& polygon);
  std::string to_string(const Properties& properties);
}

bool pointIsInsidePolygon(const geojson::Polygon& polygon, const geojson::Point& point);


#endif // GEOJSON_HPP
