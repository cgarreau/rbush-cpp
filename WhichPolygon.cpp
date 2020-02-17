#include "WhichPolygon.hpp"

#include <string>
#include <limits>
#include <vector>
#include <cstdio>

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>


#include "RunTimeStatistic.hpp"



static rbush::TreeNode<TreeData> *treeItem(geojson::Polygon *polygon, std::map<std::string, std::string> *props)
{
  auto item = new rbush::TreeNode<TreeData>();
  item->bbox = { std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), std::numeric_limits<double>::lowest() };
  item->data = new TreeData({ polygon, props });
  auto line = (*polygon)[0];
  for (auto &p: line)
  {
    item->bbox.minX = std::min(item->bbox.minX, p.x);
    item->bbox.minY = std::min(item->bbox.minY, p.y);
    item->bbox.maxX = std::max(item->bbox.maxX, p.x);
    item->bbox.maxY = std::max(item->bbox.maxY, p.y);
  }
  return item;
}

static geojson::Polygon *toPolygon(rapidjson::Value& coordinates)
{
  auto polygon = new geojson::Polygon();
  for (auto& line: coordinates.GetArray())
  {
    geojson::LinearRing ring;
    for (auto& point: line.GetArray())
    {
      auto p = point.GetArray();
      ring.push_back({ p[0].GetDouble(), p[1].GetDouble() });
    }
    polygon->push_back(ring);
  }
  return polygon;
}

static geojson::Properties *toProperties(rapidjson::Value& feature)
{
  if (!feature.HasMember("properties"))
    return NULL;
  auto& properties = feature["properties"];
  if (properties.IsNull())
    return NULL;

  auto result = new geojson::Properties();
  for (auto itr = properties.MemberBegin(); itr != properties.MemberEnd(); itr++)
  {
    if (itr->value.IsString())
    {
      std::string name = itr->name.GetString();
      std::string value = itr->value.GetString();
      (*result)[name] = value;
    }
  }
  return result;
}


static rbush::RBush<TreeData> *buildIndex(rapidjson::Document& geojson, RunTimeStatistic& rts)
{
  std::vector<rbush::TreeNode<TreeData> *> bboxes;

  int index = 1;
  for (auto& feature: geojson["features"].GetArray())
  {
    auto& geometry = feature["geometry"];
    std::string type = geometry["type"].GetString();

    auto properties = toProperties(feature);
    if (!properties) properties = new geojson::Properties();
    (*properties)["_index"] = std::to_string(index++);

    if (type == "Polygon")
    {
      bboxes.push_back(treeItem(toPolygon(geometry["coordinates"]), properties));
      rts++;
    }
    else if (type == "MultiPolygon")
    {
      for (auto& poly: geometry["coordinates"].GetArray())
      {
        bboxes.push_back(treeItem(toPolygon(poly), properties));
        rts++;
      }
    }
  }
  return new rbush::RBush<TreeData>(bboxes);
}


WhichPolygon::WhichPolygon(std::string filename)
{
  RunTimeStatistic rts("WhichPolygon(" + filename + ")", false);
  FILE* fp = fopen(filename.c_str(), "rb"); // non-Windows use "r"
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document geojson;
  geojson.ParseStream(is);
  fclose(fp);
  m_tree = buildIndex(geojson, rts);
}

WhichPolygon::WhichPolygon(const char *json)
{
  RunTimeStatistic rts("WhichPolygon", false);
  rapidjson::Document geojson;
  geojson.Parse(json);
  m_tree = buildIndex(geojson, rts);
}

WhichPolygon::~WhichPolygon()
{
  delete m_tree;
}

geojson::Properties *WhichPolygon::query(const geojson::Point& p)
{
  auto result = m_tree->search({ p.x, p.y, p.x, p.y });
  for (auto& r: *result)
  {
    if (r->data && pointIsInsidePolygon(*r->data->coords, p))
      return r->data->props;
  }
  return NULL;
}
