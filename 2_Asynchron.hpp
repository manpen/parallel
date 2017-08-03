#pragma once

#include <stxxl/stats>
#include <stxxl/parallel_sorter_asynchron>
#include <chrono>

// ManPen: C++ allows you to nest classes/struct. So you CAN
// declare this comparator within AsynchronSorter. This will not
// pollute your namespace
template <typename T>
struct my_comparator2
{
   bool operator () (const T& a, const T& b) const
   {
       return a < b;
   }

   T min_value() const
   {
       return std::numeric_limits<T>::min();
   }

   T max_value() const
   {
       return std::numeric_limits<T>::max();
   }
};

template<typename CINT>
class AsynchronSorter {
public:
	AsynchronSorter():quartetSorter(my_comparator2<CINT>(),static_cast<size_t>(1)<<30){};

   // ManPen: You should never pass complex structures (such as vectors) by value,
   // as (most likely) will trigger a copy of data (needs time and memory). Rather,
   // pass it as reference (and if you are just reading as const reference). 
   // by value:      foo(std::vector<>  v) 
   // by ref:        foo(std::vector<>& v) 
   // by const ref:  foo(const std::vector<>& v) 
  	std::vector<CINT> computeSorting(std::vector<CINT> numbers, int nthreads);
private:
	stxxl::parallel_sorter_asynchron<CINT, my_comparator2<CINT> > quartetSorter;
	std::vector<CINT> result;

};

template<typename CINT>
std::vector<CINT> AsynchronSorter<CINT>::computeSorting(std::vector<CINT> numbers, int nthreads) {
	
	std::vector<CINT> result;
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		
		quartetSorter.push_bulk(numbers, nthreads);
			
	std::chrono::steady_clock::time_point insert = std::chrono::steady_clock::now();
	
	//std::cout << "Finished pushing" << std::endl;
	
    quartetSorter.sort();  // sort elements (in ascending order)
	
	std::chrono::steady_clock::time_point sort = std::chrono::steady_clock::now();

   // ManPen: A vector is guaranteed to use a consecutive chunk of memory. If you push
   // beyond this limit, a new, larger section needs to be allocated, and the existing
   // data is copied into the new larger chunk. Then the old one is deallocated. This
   // is slow and fragments your memory. So if you know beforehand, how many elements
   // need to be stored, tell the vector beforehand:
   // result.reserve(quartetSorter.size())

	
    // walk through sorted values and print them out
    while (!quartetSorter.empty())
    {
		//std::cout << *quartetSorter << " ";
		result.push_back(*quartetSorter);
        ++quartetSorter;
    }
    
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << std::endl << "Insertion took: " << std::chrono::duration_cast<std::chrono::microseconds>(insert - begin).count()
				<< " microseconds." << std::endl;
	std::cout << "Sorting took: " << std::chrono::duration_cast<std::chrono::microseconds>(sort - insert).count()
				<< " microseconds." << std::endl;
	std::cout << "Writing took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - sort).count()
				<< " microseconds." << std::endl;
	std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
				<< " microseconds." << std::endl;
	
	return result;
}

