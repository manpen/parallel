#pragma once


//#include <stxxl/sorter>

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
	NonParallelSorter();
	using Sorter = stxxl::sorter<size_t, my_comparator4<size_t> >;
private:
	void computeSorting(Sorter &quartetSorter);

};

template<typename CINT>
void NonParallelSorter<CINT>::computeSorting(Sorter &quartetSorter) {
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	

		quartetSorter.push(1);
	std::chrono::steady_clock::time_point insert = std::chrono::steady_clock::now();
	
    quartetSorter.sort();  // sort elements (in ascending order)
	
	std::chrono::steady_clock::time_point sort = std::chrono::steady_clock::now();
	
    // walk through sorted values and print them out
    while (!quartetSorter.empty())
    {
		std::cout << *quartetSorter << " ";
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
}

template<typename CINT>
NonParallelSorter<CINT>::NonParallelSorter(){

	Sorter quartetSorter(my_comparator4<size_t>(),static_cast<size_t>(1)<<30);

	computeSorting(quartetSorter);
}