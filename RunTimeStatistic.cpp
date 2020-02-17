#include "RunTimeStatistic.hpp"


#include <iostream>


size_t memory_usage();
size_t memory_blocks();

//size_t memory_usage()
//{
//	struct mach_task_basic_info info;
//	mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
//	if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
//			   (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
//		return (size_t)0L;		/* Can't access? */
//	return (size_t)info.resident_size;
//}


#ifdef MEMTRACK

size_t s_memory_allocated = 0;
size_t s_memory_blocks = 0;

void* operator new( size_t size ) throw ( std::bad_alloc )
{
  s_memory_allocated += size;
  s_memory_blocks++;
//	std::cout << "+" << std::flush;
  size_t *ps = (size_t *)malloc( sizeof( size_t )+size );
  ps[0] = size;
  return ps+1;
}

void* operator new[]( size_t size ) throw ( std::bad_alloc )
{
  return (operator new)( size );
}

void operator delete( void *p ) throw ()
{
  size_t *ps = (size_t *)p;
  
  s_memory_allocated -= ps[-1];
  s_memory_blocks--;
//	std::cout << "-" << std::flush;
  free( ps-1 );
}

void operator delete[]( void *p ) throw ()
{
  return (operator delete)(p);
}

size_t memory_usage()
{
  return s_memory_allocated;
}

size_t memory_blocks()
{
  return s_memory_blocks;
}

#else

size_t memory_usage()
{
  return 0;
}

size_t memory_blocks()
{
  return 0;
}

#endif





RunTimeStatistic::RunTimeStatistic( const std::string& title, bool verbose ) : title(title), verbose(verbose)
{
	ns_start = std::chrono::high_resolution_clock::now();
	mem_start = memory_usage();
	blocks_start = memory_blocks();
	count = 0u;
	if (verbose)
		std::cerr << "\n\nStarting task: " << title << std::endl;
}
	
RunTimeStatistic::~RunTimeStatistic()
{
	dump();
}

void RunTimeStatistic::dump()
{
	ns_stop = std::chrono::high_resolution_clock::now();
	mem_stop = memory_usage();
	blocks_stop = memory_blocks();
	
	auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(ns_stop - ns_start).count();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(ns_stop - ns_start).count();
	
	if (verbose)
	{
		std::cerr << std::endl << "End of task: " << title << std::endl;
		if (count)
		{
			std::cerr << "  " << count << " steps completed in " << ms << " ms using " << (mem_stop-mem_start)/1024 << " Kbytes in " << blocks_stop-blocks_start << " blocks" << std::endl;
			std::cerr << "  Each steps completed in " << ns/count << " ns using " << (mem_stop-mem_start)/count << " bytes in " << (blocks_stop-blocks_start)*1.0/count << " blocks" << std::endl;
		}
		else
		{
			std::cerr << "  Operation completed in " << ms << " ms (" << ns << " ns) using " << (mem_stop-mem_start)/1024 << " Kbytes in " << blocks_stop-blocks_start << " blocks" << std::endl;
		}
	}
	else
	{
		if (count)
    {
		  std::cerr << title << ": " << count << " steps completed in " << ms << " ms, " << ns << " ns" << std::endl;
			std::cerr << "  Each steps completed in " << ms/count << " ms, " << ns/1000/count << " µs, " << ns/count << " ns" << std::endl;
    }
    else
    {
		  std::cerr << title << ": " << count << " steps, " << ms << " ms, " << ns << " ns" << std::endl;
    }
	}
}
