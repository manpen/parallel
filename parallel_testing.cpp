/***************************************************************************
 *  examples/containers/sorter1.cpp
 *
 *  Part of the STXXL. See http://stxxl.sourceforge.net
 *
 *  Copyright (C) 2013 Daniel Feist <daniel.feist@student.kit.edu>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *  (See accompanying file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 **************************************************************************/

//! [example]

//#include "4_NonParallelSorter.hpp"
//#include "3_DifferentSorter.hpp"
#include "1_Synchron.hpp"
#include <limits>
#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <vector>
#include <algorithm>

int main(int argc, char** argv)
{
	
	if(argc != 4){
		std::cout << "Please specify the input file and the implemented option to parallize and number of threads. " << std::endl;
		std::cout << "Optioins: " << std::endl << "1 Synchron" << std::endl << "2 Asynchron" 
			<< std::endl << "3 Different Sorter" << std::endl << "4 Non Parallel" << std::endl;
		return 0;
	}
	
	int nthreads;
	std::stringstream(argv[3]) >> nthreads;
	
	std::vector<std::vector<size_t>> results(nthreads);
	
   // Read the file
    std::ifstream ifs(argv[1] , std::ios::in);
	//size_t line_count = std::count(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), '\n');
	//std::cout << "Number of elements: " << line_count << std::endl;
	
	ifs.seekg(0);
	std::string line;
	int number;
    int nlines = 0;
    std::vector<int> numbers;
	
	while(ifs >> line){
		std::stringstream(line) >> number;
		numbers.push_back(number);
		nlines = nlines + 1 ;
	}
    ifs.close();
	
	// Prepare outputfile
	std::ofstream outputfile;
	std::vector<int> final_result;
	
	int option;
	std::stringstream(argv[2]) >> option;
	
	switch (option) {
		case 1:
		{
			outputfile.open("../1_result_non_parallel.txt");
			
			std::cout<<"threads="<<omp_get_num_threads()<< std::endl;
			SynchronSorter<int> s;
			final_result = s.computeSorting(numbers);
			std::vector<int>::const_iterator it;
			
			for(it = final_result.begin(); it < final_result.end(); it++)
			{
				outputfile << *it << std::endl;
			}
			
			break;
		}
		case 2:
		{
			break;
		}
		case 3:
		{
			outputfile.open("../3_result_non_parallel.txt");
			
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			#pragma omp parallel num_threads(nthreads)
			{	
				std::cout<<"threads="<<omp_get_num_threads()<< std::endl;
				int t=omp_get_thread_num();
				
				//DifferentSorter<uint64_t> ds;
				

				
				#pragma omp for
					for ( int i = 0; i < nlines; ++i )
					{
						//ds.insert(numbers[i]);
					}
				std::chrono::steady_clock::time_point insert = std::chrono::steady_clock::now();
				std::cout << std::endl << "Insertion took: " << std::chrono::duration_cast<std::chrono::microseconds>(insert - begin).count()
							<< " microseconds." << std::endl;
				//ds.sort();
				std::chrono::steady_clock::time_point sort = std::chrono::steady_clock::now();
				std::cout << "Sorting took: " << std::chrono::duration_cast<std::chrono::microseconds>(sort - insert).count()
							<< " microseconds." << std::endl;
				
				//results[t] = ds.getSortedValues(static_cast<size_t>(1000000/omp_get_num_threads()));
			}
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

			std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
						<< " microseconds." << std::endl;
			
			break;
		}
		case 4:		
		{
			#pragma omp critical
			{	
				std::cout<<"threads="<<omp_get_num_threads()<< std::endl;
				//NonParallelSorter<uint64_t> nps;
			}
			break;
		}
	}
	outputfile.close();
	return 0;
}
//! [example]
