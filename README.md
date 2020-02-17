# rbush-cpp
RBush is a high-performance C++11 library for 2D spatial indexing of points and rectangles. It's based on an optimized R-tree data structure with bulk insertion support.


Based on [this](https://github.com/mourner/rbush) JavaScript implementation by [Vladimir Agafonkin](http://agafonkin.com/)


Spatial index is a special data structure for points and rectangles that allows you to perform queries like "all items within this bounding box" very efficiently (e.g. hundreds of times faster than looping over all items). It's most commonly used in maps and data visualizations.


## Usage

See WichPolygon.cpp and main.cpp for a demonstration featuring an implementation of a reverse country function.
The program loads country geojson boudaries data into an RBush tree.

Then the search for the country code of a random geocode takes 25 µs in average.


Huge thanks to Vladimir Agafonkin for his original implementation in javascript.

