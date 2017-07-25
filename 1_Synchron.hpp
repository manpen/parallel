#pragma once

#include <stxxl/stats>
#include <stxxl/parallel_sorter_synchron>
#include <chrono>

template <typename T>
struct my_comparator1
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
class SynchronSorter {
public:
	SynchronSorter():quartetSorter(my_comparator1<size_t>(),static_cast<size_t>(1)<<30){};
	std::vector<CINT> computeSorting(std::vector<CINT> numbers);
private:
	stxxl::parallel_sorter_synchron<CINT, my_comparator1<size_t> > quartetSorter;
	std::vector<CINT> result;

};

template<typename CINT>
std::vector<CINT> SynchronSorter<CINT>::computeSorting(std::vector<CINT> numbers) {
	
	std::vector<CINT> results(numbers.size());
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		
		quartetSorter.push_bulk(numbers);
			
	std::chrono::steady_clock::time_point insert = std::chrono::steady_clock::now();
	
	//std::cout << "Finished pushing" << std::endl;
	
    quartetSorter.sort();  // sort elements (in ascending order)
	
	std::chrono::steady_clock::time_point sort = std::chrono::steady_clock::now();
	
    // walk through sorted values and print them out
    while (!quartetSorter.empty())
    {
		std::cout << *quartetSorter << " ";
		results.push_back(*quartetSorter);
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
	
	return results;
}

