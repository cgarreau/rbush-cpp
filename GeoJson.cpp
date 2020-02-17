#include "GeoJson.hpp"

#include <sstream>


static bool rayIntersect(const geojson::Point& p, const geojson::Point& p1, const geojson::Point& p2)
{
  return ((p1.y > p.y) != (p2.y > p.y)) && (p.x < (p2.x - p1.x) * (p.y - p1.y) / (p2.y - p1.y) + p1.x);
}

// ray casting algorithm for detecting if point is in polygon
bool pointIsInsidePolygon(const geojson::Polygon& polygon, const geojson::Point& p)
{
  bool inside = false;
  for (auto &ring: polygon)
  {
    for (size_t j = 0, len2 = ring.size(), k = len2 - 1; j < len2; k = j++)
    {
      if (rayIntersect(p, ring[j], ring[k]))
        inside = !inside;
    }
  }
  return inside;
}


std::string geojson::to_string(const geojson::Polygon& polygon)
{
  std::stringstream ss;
  ss << "[";
  bool firstline = true;
  for (auto& line: polygon)
  {
    if (firstline)
      firstline = false;
    else
      ss << ",";
    ss << "[";
    bool first = true;
    for (auto& point: line)
    {
      if (first)
        first = false;
      else
        ss << ",";
     ss << "[" << point.x << "," << point.y << "]";
    }
    ss << "]";
  }
  ss << "]";
  return ss.str();
}

std::string geojson::to_string(const geojson::Properties& properties)
{
  std::stringstream ss;
  ss << "[";
  bool first = true;
  for (auto& prop: properties)
  {
    if (first)
      first = false;
    else
      ss << ",";
   ss << "\"" << prop.first << "\":\"" << prop.second << "\"";
  }
  ss << "]";
  return ss.str();
}
