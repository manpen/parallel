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
#include "2_Asynchron.hpp"
//#include "1_Synchron.hpp"
#include <limits>
#include <iostream>
#include <fstream>
#include <chrono>
#include <sstream>
#include <vector>
#include <algorithm>

std::vector<int> merge_runs(std::vector<std::vector<int>> results)
{
	std::vector<int> final_result;
	std::vector<int> it(results.size()); //vector with iterators
	size_t length = 0;

	for(unsigned int i = 0; i < results.size(); i++)
	{
		length = length + results[i].size();
		results[i].push_back(std::numeric_limits<int>::max());		
	}

	for(size_t i = 0; i < length; i++){
		int min_value = std::numeric_limits<int>::max();
		int min_run = length + 1;
		
		for(unsigned int j = 0; j < it.size(); j++){
			if(min_value > results[j][it[j]]){
				min_value = results[j][it[j]];
				min_run = j;
			}
		}
		final_result.push_back(min_value);	
		++it[min_run];
	}
	//std::cout << "test 6"  << std::endl;
	return final_result;
}

int main(int argc, char** argv)
{
	
	if(argc != 4){
		std::cout << "Please specify the input file and the implemented option to parallize and number of threads. " << std::endl;
		std::cout << "Options: " << std::endl << "1 Synchron" << std::endl << "2 Asynchron" 
			<< std::endl << "3 Different Sorter" << std::endl << "4 Non Parallel" << std::endl;
		return 0;
	}
	
	int nthreads;
	std::stringstream(argv[3]) >> nthreads;
	
	std::vector<std::vector<int>> results(nthreads);
	
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
	std::ofstream measures;
	std::vector<int> final_result(nlines);
	
	int option;
	std::stringstream(argv[2]) >> option;
	std::vector<int>::const_iterator it;
	//measures.open("../0_measures_big.csv", std::ios_base::app);
	
	//for(int i = 0; i < 20; i++)
	//{
	
	switch (option) {
		case 1:
		{
			outputfile.open("../1_result_parallel_testing.txt");
			
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			//SynchronSorter<int> s;
			//final_result = s.computeSorting(numbers);
			
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			
			measures << "1," << nthreads << "," << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
			
			break;
		}
		case 2:
		{
			outputfile.open("../2_result_parallel_testing.txt");
			
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			AsynchronSorter<int> as;
			final_result = as.computeSorting(numbers, nthreads);
			//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			
			//measures << "2," << nthreads << "," << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
			
			break;
		}
		case 3:
		{
			outputfile.open("../3_result_parallel_testing.txt");
			
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			#pragma omp parallel num_threads(nthreads)
			{	
				std::cout<<"threads="<<omp_get_num_threads()<< std::endl;
				int t=omp_get_thread_num();
				
				//DifferentSorter<int> ds;

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
				
				//results[t] = ds.getSortedValues();
			}
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

			std::cout << "Total time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()
						<< " microseconds." << std::endl;
			
			//measures << "3," << nthreads << "," << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
			
			final_result = merge_runs(results);
			
			break;
		}
		case 4:		
		{
			outputfile.open("../4_result_parallel_testing.txt");
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			#pragma omp critical
			{	
				std::cout<<"threads="<<omp_get_num_threads()<< std::endl;
				//NonParallelSorter<int> nps;
				//final_result = nps.computeSorting(numbers);
			}
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			
			measures << "4," << nthreads << "," << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
			
			break;
		}
	}
	//}//end for loop for measures
	std::cout << "Finished calculation" << std::endl;
	for(it = final_result.begin(); it < final_result.end(); it++)
	{
		outputfile << *it << std::endl;
	}
	outputfile.close();
	//measures.close();
	return 0;
}
//! 
