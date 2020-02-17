#include <stdio.h>
#include <random>


#include "WhichPolygon.hpp"

#include "RunTimeStatistic.hpp"

#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>



std::string whichCountry(WhichPolygon *tree, const double lat, const double lon)
{
  auto properties = tree->query({ lon, lat });
  if (!properties) {
    return std::string();
  } else {
    return (*properties)["ISO3166-1:alpha2"];
  }
}

int main(int argc, char **argv)
{
  const std::string countries = "./data/countries.json";
  auto countryTree = new WhichPolygon(countries);

  // test on a single geocode (Paris, FR)
  double lat = 48.2;
  double lon = 2.24;
  std::string country = whichCountry(countryTree, lat, lon);
  std::cout << "Reverse geocode lon=" << lon << ", lat=" << lat << ": country=" << (country.empty() ? "not found" : country) << "\n";

  // Test on one geocode per box (center) of the country boundaries
  auto result = countryTree->tree()->all();
  RunTimeStatistic rts("Reverse geocode", false);
  for (auto& r: *result)
  {
    double x = (r->bbox.minX + r->bbox.maxX) / 2;
    double y = (r->bbox.minY + r->bbox.maxY) / 2;
    whichCountry(countryTree, y, x);
    rts++;
    //std::string country = whichCountry(countryTree, y, x);
    //auto origin = (*(r->data->props))["ISO3166-1:alpha2"];
    //std::cout << "Reverse geocode lon=" << y << ", lat=" << x << ": " << (country.empty() ? "not found" : country) << ", origin=" << origin << "\n";
  }

	return 0;
}
