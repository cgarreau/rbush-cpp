#ifndef WHICHPOLYGON_HPP
#define WHICHPOLYGON_HPP

#include <string>

#include "GeoJson.hpp"
#include "RBush.hpp"


struct TreeData
{
  geojson::Polygon *coords;
  geojson::Properties *props;
};




class WhichPolygon
{
  rbush::RBush<TreeData> *m_tree;
public:
  WhichPolygon(const char *json);
  WhichPolygon(std::string filename);
  ~WhichPolygon();
  
  rbush::RBush<TreeData> *tree() const { return m_tree; }
  
  geojson::Properties *query(const geojson::Point& p);
  
  const std::string serialize() const;

};

#endif // WHICHPOLYGON_HPP
