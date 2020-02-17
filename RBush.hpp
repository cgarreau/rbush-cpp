#ifndef RBUSH_HPP
#define RBUSH_HPP


//
// RBush is a high-performance library for 2D spatial indexing of points and rectangles. It's based on an optimized R-tree data structure with bulk insertion support.
//
// See https://github.com/mourner/rbush for original javascript version
// See also this article from the autor: https://blog.mapbox.com/a-dive-into-spatial-search-algorithms-ebd0c5e39d2a
//
// This file is a C++11 transcription of original js code writen by Vladimir Agafonkin
//


#include <vector>
#include <limits>
#include <algorithm>
#include <functional>
#include <cmath>

#include <iostream>

namespace rbush
{
  struct Bbox
  {
    double minX;
    double minY;
    double maxX;
    double maxY;
  };

  template <class T>
  struct TreeNode;

  template <class T>
  struct TreeNode
  {
    Bbox bbox;
    int height;
    bool leaf;
    std::vector<TreeNode *> *children;
    T *data;
  };


  template <class T>
  class RBush
  {
    int _maxEntries;
    int _minEntries;
    TreeNode<T> *rootNode;


   // calculate node's bbox from bboxes of its children
    static void calcBBox(TreeNode<T>& node)
    {
      distBBox(node, 0, node.children->size(), &node);
    };

    // min bounding rectangle of node children from k to p-1
    static TreeNode<T> *distBBox(TreeNode<T>& node, int k, int p, TreeNode<T> *destNode)
    {
      if (!destNode)
        destNode = createNode(NULL);
      destNode->bbox.minX = std::numeric_limits<int>::max();
      destNode->bbox.minY = std::numeric_limits<int>::max();
      destNode->bbox.maxX = std::numeric_limits<int>::lowest();
      destNode->bbox.maxY = std::numeric_limits<int>::lowest();
      for (auto i = k; i < p; i++)
      {
        auto child = (*node.children)[i];
        extend(destNode->bbox, child->bbox);
      }
      return destNode;
    };

    static void extend(Bbox& a, const Bbox& b)
    {
      a.minX = std::min(a.minX, b.minX);
      a.minY = std::min(a.minY, b.minY);
      a.maxX = std::max(a.maxX, b.maxX);
      a.maxY = std::max(a.maxY, b.maxY);
    };

    static bool compareNodeMinX(const TreeNode<T> *a, const TreeNode<T> *b) { return a->bbox.minX < b->bbox.minX; };
    static bool compareNodeMinY(const TreeNode<T> *a, const TreeNode<T> *b) { return a->bbox.minY < b->bbox.minY; };
    
    static double bboxArea(const Bbox& a) { return (a.maxX - a.minX) * (a.maxY - a.minY); };
    static double bboxMargin(const Bbox& a) { return (a.maxX - a.minX) + (a.maxY - a.minY); };

    static double enlargedArea(const Bbox& a, const Bbox& b)
    {
      return (std::max(b.maxX, a.maxX) - std::min(b.minX, a.minX)) *
             (std::max(b.maxY, a.maxY) - std::min(b.minY, a.minY));
    };

    static double intersectionArea(const Bbox& a, const Bbox& b)
    {
      auto minX = std::max(a.minX, b.minX);
      auto minY = std::max(a.minY, b.minY);
      auto maxX = std::min(a.maxX, b.maxX);
      auto maxY = std::min(a.maxY, b.maxY);

      return std::max(0.0, maxX - minX) *
             std::max(0.0, maxY - minY);
    };

    static bool contains(const Bbox& a, const Bbox& b)
    {
      return a.minX <= b.minX &&
             a.minY <= b.minY &&
             b.maxX <= a.maxX &&
             b.maxY <= a.maxY;
    };

    static bool intersects(const Bbox& a, const Bbox& b)
    {
      return b.minX <= a.maxX &&
             b.minY <= a.maxY &&
             b.maxX >= a.minX &&
             b.maxY >= a.minY;
    };

    static TreeNode<T> *createNode(std::vector<TreeNode<T> *> *children)
    {
      auto node = new TreeNode<T>();
      node->height = 1;
      node->leaf = true;
      node->children = children,
      node->bbox.minX = std::numeric_limits<int>::max();
      node->bbox.minY = std::numeric_limits<int>::max();
      node->bbox.maxX = std::numeric_limits<int>::lowest();
      node->bbox.maxY = std::numeric_limits<int>::lowest();
      return node;
    };


    // Floyd–Rivest algorithm (https://en.wikipedia.org/wiki/Floyd%E2%80%93Rivest_algorithm)
    // left is the left index for the interval
    // right is the right index for the interval
    // k is the desired index value, where array[k] is the (k+1)th smallest element when left = 0
    static void quickselect(std::vector<TreeNode<T> *>array, int left, int right, int k, std::function<double(const TreeNode<T> *, const TreeNode<T> *)> compare)
    {
      //std::cerr << "quickselect([0-"<< array.size()-1 << "]," << k << "," << left << "," << right << ")\n";
      #define sign(x) ((x > 0.0) ? 1 : ((x < 0.0) ? (-1) : 0))
      #define F_SWAP(a,b) { TreeNode<T> *temp=(a);(a)=(b);(b)=temp; }
      while (right > left)
      {
        // use select recursively to sample a smaller set of size s
        // the arbitrary constants 600 and 0.5 are used in the original
        // version to minimize execution time
        if (right - left > 600)
        {
          int n = right - left + 1;
          int i = k - left + 1;
          double z = std::log(n);
          double s = 0.5 * std::exp(2.0 * z / 3.0);
          double sd = 0.5 * std::sqrt(z * s * (n - s) / n) * (i - n / 2.0 < 0 ? -1 : 1);
          int newLeft = std::max(left, (int)std::floor(k - i * s / n + sd));
          int newRight = std::min(right, (int)std::floor(k + (n - i) * s / n + sd));
          //std::cerr << "quickselect_r([0-"<< array.size()-1 << "]," << k << "," << newLeft << "," << newRight << ")\n";
          quickselect(array, k, newLeft, newRight, compare);
        }
        // partition the elements between left and right around t
        auto t = array[k];
        int i = left;
        int j = right;
        F_SWAP(array[left], array[k]); //swap array[left] and array[k]
        if (compare(array[right], t) > 0) //array[right] > t
           F_SWAP(array[left], array[right]); //swap array[right] and array[left]
        while (i < j)
        {
          F_SWAP(array[i], array[j]); //swap array[i] and array[j]
          i++;
          j--;
          while (compare(array[i], t) < 0) //array[i] < t
            i++;
          while (compare(array[j], t) > 0) // array[j] > t
            j--;
        }
        if (compare(array[left], t) == 0) //array[left] = t
        {
          F_SWAP(array[left], array[j]); //swap array[left] and array[j]
        }
        else
        {
          j++;
          F_SWAP(array[j], array[right]); //swap array[j] and array[right]
        }
        // adjust left and right towards the boundaries of the subset
        // containing the (k - left + 1)th smallest element
        if (j <= k)
           left = j + 1;
        if (k <= j)
           right = j - 1;
     }
   };

    // sort an array so that items come in groups of n unsorted items, with groups sorted between each other;
    // combines selection algorithm with binary divide & conquer approach

    static void multiSelect(std::vector<TreeNode<T> *>arr, int ileft, int iright, int n, std::function<double(const TreeNode<T> *, const TreeNode<T> *)> compare)
    {
      //std::cerr << "multiSelect([0-"<< arr.size()-1 << "]," << ileft << "," << iright << "," << n << ")\n";
      
      std::vector<int> stack;
      stack.push_back(ileft);
      stack.push_back(iright);

      while (!stack.empty())
      {
        int right = stack.back();
        stack.pop_back();
        int left = stack.back();
        stack.pop_back();

        if (right - left <= n)
          continue;

        int mid = left + std::ceil((right - left) / n / 2.0) * n;

        quickselect(arr, left, right, mid, compare);

        stack.push_back(left);
        stack.push_back(mid);
        stack.push_back(mid);
        stack.push_back(right);
      }
    };


    TreeNode<T> *_chooseSubtree(Bbox bbox, TreeNode<T> *node, int level, std::vector<TreeNode<T> *>& path)
    {
      int minArea;
      int minEnlargement;
      TreeNode<T> *targetNode = NULL;

      while (true)
      {
        path.push_back(node);

        if (node->leaf || (int)path.size() - 1 == level)
          break;

        minArea = std::numeric_limits<int>::max();
        minEnlargement = std::numeric_limits<int>::max();

        for (auto& child: *node->children)
        {
          auto area = bboxArea(child->bbox);
          auto enlargement = enlargedArea(bbox, child->bbox) - area;

          // choose entry with the least area enlargement
          if (enlargement < minEnlargement)
          {
            minEnlargement = enlargement;
            minArea = area < minArea ? area : minArea;
            targetNode = child;
          }
          else if (enlargement == minEnlargement)
          {
            // otherwise choose one with the smallest area
            if (area < minArea)
            {
              minArea = area;
              targetNode = child;
            }
          }
        }
        node = targetNode ? targetNode : (*node->children)[0];
      }
      return node;
    };

    // total margin of all possible split distributions where each node is at least m full
    int _allDistMargin(TreeNode<T> &node, int m, int M, std::function<bool(const TreeNode<T> *, const TreeNode<T> *)> compare)
    {
      std::sort(node.children->begin(), node.children->end(), compare);
      
      auto leftBBox = distBBox(node, 0, m, NULL);
      auto rightBBox = distBBox(node, M - m, M, NULL);
      auto margin = bboxMargin(leftBBox->bbox) + bboxMargin(rightBBox->bbox);

      for (int i = m; i < M - m; i++)
      {
        auto child = (*node.children)[i];
        extend(leftBBox->bbox, child->bbox);
        margin += bboxMargin(leftBBox->bbox);
      }

      for (int i = M - m - 1; i >= m; i--)
      {
        auto child = (*node.children)[i];
        extend(rightBBox->bbox, child->bbox);
        margin += bboxMargin(rightBBox->bbox);
      }

      return margin;
    };

    // sorts node children by the best axis for split
    void _chooseSplitAxis(TreeNode<T> *node, int m, int M)
    {
      auto xMargin = this->_allDistMargin(*node, m, M, compareNodeMinX);
      auto yMargin = this->_allDistMargin(*node, m, M, compareNodeMinY);

      // if total distributions margin value is minimal for x, sort by minX,
      // otherwise it's already sorted by minY
      if (xMargin < yMargin)
        std::sort(node->children->begin(), node->children->end(), compareNodeMinX);
    };


    // split overflowed node into two
    void _split(std::vector<TreeNode<T> *>& insertPath, int level)
    {
      auto node = insertPath[level];
      int M = node->children->size();
      int m = this->_minEntries;

      this->_chooseSplitAxis(node, m, M);
      int splitIndex = this->_chooseSplitIndex(*node, m, M);
      
      //node->children.splice(splitIndex, node.children.length - splitIndex)
      std::vector<TreeNode<T> *> spliced;
      spliced.insert(spliced.end(), node->children->begin() + splitIndex, node->children->begin() + node->children->size() - splitIndex);
      node->children->erase(node->children->begin() + splitIndex, node->children->begin() + node->children->size() - splitIndex);
      auto newNode = createNode(&spliced);
      newNode->height = node->height;
      newNode->leaf = node->leaf;

      calcBBox(*node);
      calcBBox(*newNode);

      if (level) insertPath[level - 1]->children->push_back(newNode);
      else this->_splitRoot(node, newNode);
    };

    int _chooseSplitIndex(TreeNode<T> &node, int m, int M)
    {
      int index = 0;

      double minOverlap = std::numeric_limits<double>::max();
      double minArea = std::numeric_limits<double>::max();

      for (int i = m; i <= M - m; i++)
      {
        auto bbox1 = distBBox(node, 0, i, NULL);
        auto bbox2 = distBBox(node, i, M, NULL);

        auto overlap = intersectionArea(bbox1->bbox, bbox2->bbox);
        auto area = bboxArea(bbox1->bbox) + bboxArea(bbox2->bbox);

        // choose distribution with minimum overlap
        if (overlap < minOverlap)
        {
          minOverlap = overlap;
          index = i;
          minArea = area < minArea ? area : minArea;
        }
        else if (overlap == minOverlap) {
          // otherwise choose distribution with minimum area
          if (area < minArea)
          {
            minArea = area;
            index = i;
          }
        }
      }
      return index;
    };


    void insert(TreeNode<T> *item)
    {
      this->_insert(item, this->rootNode->height - 1, false);
    };
    
    void _insert(TreeNode<T> *item, int level, bool isNode)
    {
      Bbox bbox = item->bbox;
      std::vector<TreeNode<T> *> insertPath;
      
      // find the best node for accommodating the item, saving all nodes along the path too
      auto node = this->_chooseSubtree(bbox, this->rootNode, level, insertPath);

      // put the item into the node
      node->children->push_back(item);
      extend(node->bbox, bbox);

      // split on node overflow; propagate upwards if necessary
      while (level >= 0)
      {
        if ((int)insertPath[level]->children->size() > this->_maxEntries)
        {
          this->_split(insertPath, level);
          level--;
        }
        else
          break;
      }

      // adjust bboxes along the insertion path
      this->_adjustParentBBoxes(bbox, insertPath, level);
    };


    void _adjustParentBBoxes(Bbox &bbox, std::vector<TreeNode<T> *>& path, int level)
    {
      // adjust bboxes along the given tree path
      for (int i = level; i >= 0; i--)
      {
        extend(path[i]->bbox, bbox);
      }
    };


    void _splitRoot(TreeNode<T> *node, TreeNode<T> *newNode)
    {
      std::cerr << "_splitRoot" << "\n";
      // split root node
      auto children = new std::vector<TreeNode<T> *>();
      children->push_back(node);
      children->push_back(newNode);
      this->rootNode = createNode(children);
      this->rootNode->height = node->height + 1;
      this->rootNode->leaf = false;
      calcBBox(*this->rootNode);
    };


    TreeNode<T> *_build(std::vector<TreeNode<T> *>& items, int left, int right, int height)
    {
      int N = right - left + 1;
      int M = this->_maxEntries;

      if (N <= M)
      {
        // reached leaf level; return leaf
        auto childrens = new std::vector<TreeNode<T> *>();
        childrens->insert(childrens->end(), items.begin() + left, items.begin() + right + 1); //items.slice(left, right + 1)
        auto node = createNode(childrens);
        calcBBox(*node);
        return node;
      }

      if (!height)
      {
        // target height of the bulk-loaded tree
        height = std::ceil(std::log(N) / std::log(M));

        // target number of root entries to maximize storage utilization
        M = std::ceil(N / std::pow(M, height - 1));
      }

      auto node = createNode(new std::vector<TreeNode<T> *>());
      node->leaf = false;
      node->height = height;

      // split the items into M mostly square tiles

      int N2 = std::ceil(N / M);
      int N1 = N2 * std::ceil(std::sqrt(M));
      
      multiSelect(items, left, right, N1, [](const TreeNode<T> *a, const TreeNode<T> *b) { return a->bbox.minX - b->bbox.minX; });

      for (int i = left; i <= right; i += N1)
      {
        int right2 = std::min(i + N1 - 1, right);

        multiSelect(items, i, right2, N2, [](const TreeNode<T> *a, const TreeNode<T> *b) { return a->bbox.minY - b->bbox.minY; });

        for (int j = i; j <= right2; j += N2)
        {
          int right3 = std::min(j + N2 - 1, right2);

          // pack each entry recursively
          node->children->push_back(this->_build(items, j, right3, height - 1));
        }
      }

      calcBBox(*node);

      return node;
    };

    void _all(TreeNode<T> *node, std::vector<TreeNode<T> *>& result)
    {
      std::vector<TreeNode<T> *> nodesToSearch;
      while (node)
      {
        if (node->leaf)
          result.insert(result.end(), node->children->begin(), node->children->end());
        else
          nodesToSearch.insert(nodesToSearch.end(), node->children->begin(), node->children->end());
        if (!nodesToSearch.empty())
        {
          node = nodesToSearch.back();
          nodesToSearch.pop_back();
        }
        else
        {
          node = NULL;
        }
      }
      //return result;
    };

    void load(std::vector<TreeNode<T> *> &data)
    {
      if (!data.size())
        return;

      if ((int)data.size() < this->_minEntries)
      {
        for (auto& item: data)
        {
          this->insert(item);
        }
      }

      // recursively build the tree with the given data from scratch using OMT algorithm
      std::vector<TreeNode<T> *> dataslice;
      dataslice.insert(dataslice.end(), data.begin(), data.end());
      auto node = this->_build(dataslice, 0, data.size() - 1, 0);

      if (!this->rootNode->children->size())
      {
        // save as is if tree is empty
        this->rootNode = node;
      }
      else if (this->rootNode->height == node->height)
      {
        // split root if trees have the same height
        this->_splitRoot(this->rootNode, node);
      }
      else
      {
        if (this->rootNode->height < node->height)
        {
          // swap trees if inserted one is bigger
          auto tmpNode = this->rootNode;
          this->rootNode = node;
          node = tmpNode;
        }
        // insert the small tree into the large tree at appropriate level
        this->_insert(node, this->rootNode->height - node->height - 1, true);
      }
    };



  public:
  
    const TreeNode<T> *root() const { return rootNode; };
  
    RBush(std::vector<TreeNode<T> *> &data, int maxEntries = 9)
    {
      this->_maxEntries = std::max(4, maxEntries);
      this->_minEntries = std::max(2, (int)std::ceil(this->_maxEntries * 0.4));
      this->rootNode = createNode(new std::vector<TreeNode<T> *>());
      load(data);
    };

    ~RBush()
    {
      // TODO
      delete rootNode;
    };
    
    std::vector<TreeNode<T> *> *all()
    {
      auto result = new std::vector<TreeNode<T> *>();
      this->_all(this->rootNode, *result);
      return result;
    }
    
    std::vector<TreeNode<T> *> *search(const Bbox& bbox)
    {
      auto node = this->rootNode;
      auto result = new std::vector<TreeNode<T> *>();

      if (!intersects(bbox, node->bbox))
        return result;

      std::vector<TreeNode<T> *> nodesToSearch;
      while (node)
      {
        for (auto& child: *node->children)
        {
          if (intersects(bbox, child->bbox))
          {
            if (node->leaf)
              result->push_back(child);
            else if (contains(bbox, child->bbox))
              this->_all(child, *result);
            else
              nodesToSearch.push_back(child);
          }
        }
        if (!nodesToSearch.empty())
        {
          node = nodesToSearch.back();
          nodesToSearch.pop_back();
        }
        else
        {
          node = NULL;
        }
      }
      return result;
    };
    
  };

}


#endif // RBUSH_HPP
