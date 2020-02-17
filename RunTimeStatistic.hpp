#ifndef RUNTIMESTATISTIC_HPP
#define RUNTIMESTATISTIC_HPP

#include <inttypes.h>
#include <string>
#include <chrono>

class RunTimeStatistic
{
  std::string title;
  std::chrono::high_resolution_clock::time_point ns_start;
  std::size_t mem_start;
  std::size_t blocks_start;
  std::chrono::high_resolution_clock::time_point ns_stop;
  std::size_t mem_stop;
  std::size_t blocks_stop;
  uint64_t count;
  bool verbose;
  
public:
  RunTimeStatistic( const std::string& title, bool verbose = true );
  ~RunTimeStatistic();
  
  RunTimeStatistic &operator++(int)
  {
    count++;
    //  if ((count%1000)==0)
    //    dump();
    return *this;
  }
  RunTimeStatistic &operator+=(uint64_t c)
  {
    count += c;
    return *this;
  }
  RunTimeStatistic &operator+=(std::string text)
  {
    title += text;
    return *this;
  }
  
  void setTitle(const std::string atitle) { title = atitle; };

  std::chrono::high_resolution_clock::duration elapsed_ns()
  {
    return std::chrono::high_resolution_clock::now() - ns_start;
  }
  
  uint64_t elapsed()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - ns_start).count();
  }
  
  uint64_t cnt()
  {
    return count;
  }

  void dump();
};


#endif // RUNTIMESTATISTIC_HPP
