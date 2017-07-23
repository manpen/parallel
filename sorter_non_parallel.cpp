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

#include "4_NonParallelSorter.hpp"
#include "3_DifferentSorter.hpp"
#include <limits>


int main(int argc, char** argv)
{
	if(argc != 3){
		std::cout << "Please specify the input file and the implemented option to parallize. " << std::endl;
		std::cout << "Optioins: " << std::endl << "1 Synchron" << std::endl << "2 Asynchron" 
			<< std::endl << "3 Different Sorter" << std::endl << "4 Non Parallel" << std::endl;
		return 0;
	}
	
	switch ( argv[2] ) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			#pragma omp parallel
			{	
				std::cout<<"threads="<<omp_get_num_threads()<< std::endl;
				DifferentSorter<uint64_t> nps(argv);
			}
			break;
		case 4:		
			#pragma omp critical
			{	
				std::cout<<"threads="<<omp_get_num_threads()<< std::endl;
				NonParallelSorter<uint64_t> nps(argv);
			}
			break;
	return 0;
}
//! [example]
