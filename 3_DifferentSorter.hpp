#pragma once

#include <stxxl/sorter>
#include <limits>

template <typename T>
struct my_comparator3
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
class DifferentSorter {
public:
	DifferentSorter(): quartetSorter(my_comparator3<CINT>(),static_cast<size_t>(1)<<30){};
	void insert(CINT number);
	void sort();
	std::vector<int> getSortedValues();
private:
	 stxxl::sorter<CINT, my_comparator3<CINT> > quartetSorter;
	 std::vector<CINT> result;
};

template<typename CINT>
void DifferentSorter<CINT>::insert(CINT number) {

	quartetSorter.push(number);
	
}

template<typename CINT>
void DifferentSorter<CINT>::sort() {

	quartetSorter.sort();
	
}

template<typename CINT>
std::vector<int> DifferentSorter<CINT>::getSortedValues() {
	
	while (!quartetSorter.empty())
    {
        //std::cout << *quartetSorter << " ";
		result.push_back(*quartetSorter);
        ++quartetSorter;
    }
	return result;
}
