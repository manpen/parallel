#pragma once


#include <stxxl/sorter>
#include <limits>
#include <chrono>

template <typename T>
struct my_comparator4
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
class NonParallelSorter {
public:
	NonParallelSorter():quartetSorter(my_comparator4<CINT>(),static_cast<size_t>(1)<<30){};
	std::vector<CINT> computeSorting(std::vector<CINT> numbers);
private:
	stxxl::sorter<CINT, my_comparator4<CINT> > quartetSorter;
	std::vector<CINT> result;
	typedef typename std::vector<CINT>::const_iterator vector_iterator;
};

template<typename CINT>
std::vector<CINT> NonParallelSorter<CINT>::computeSorting(std::vector<CINT> numbers) {
	
	std::vector<CINT> result;
	vector_iterator it;
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	
	for(it = numbers.begin(); it < numbers.end(); it++)
	{
			quartetSorter.push(*it);
	}
		
	std::chrono::steady_clock::time_point insert = std::chrono::steady_clock::now();
	
    quartetSorter.sort();  // sort elements (in ascending order)
	
	std::chrono::steady_clock::time_point sort = std::chrono::steady_clock::now();
	
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
