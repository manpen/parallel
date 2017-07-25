#pragma once

//#include <stxxl/sorter>

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
	DifferentSorter(): quartetSorter(my_comparator3<size_t>(),static_cast<size_t>(1)<<30){};
	void insert(CINT number);
	void sort();
	std::vector<size_t> getSortedValues(size_t size);
private:
	 stxxl::sorter<size_t, my_comparator3<size_t> > quartetSorter;
	 std::vector<size_t> result;
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
std::vector<size_t> DifferentSorter<CINT>::getSortedValues(size_t size) {

    result.resize(size);
	
	while (!quartetSorter.empty())
    {
        std::cout << *quartetSorter << " ";
		result.push_back(*quartetSorter);
        ++quartetSorter;
    }
	return result;
}
