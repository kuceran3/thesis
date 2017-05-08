Disk structure
+include
	- *.h files
+scripts
	- *.py files
+src
	- *.cpp files
+text
	- latex files
Makefile
README.txt

Installation:
For installation of Python please visit www.python.org and download distribution at least 3.0+
	when installing follow the instructions on this site
For compilating on linux please use C++ version 11

Usage:

	Python:
		chunking script
			
			python3 script_name.py <INPUT_FILE>

		filter pattern

			python3 script_name.py <INPUT_FILE> <list of starting positions of dimensions> <list of ending positions of dimensions> 0/1

			1 for saving of attribute values

		generate scripts
		generate.py --- generates data with no missing values
			python3 generate.py <NUM_OF_DIMENSIONS> <DIMENSION_LENGTH> <NUM_OF_ATTRIBUTES> <MAX_ATTRIBUTE_VALUE> <OUTPUT_FILE>
		generateGauss.py --- generates 3D data based on Gaussian distribution
			python3 generate_Gauss.py <NUMBER_OF_DIM> <DIM_SIZE> <%_DENSITY> <%_OF_RESULTS> <PATTERN_SIZE> <PATTERN_FILE> <OUTPUT_FILE>
		generateGauss2D.py --- generates 2D data based on Gaussian distribution
			python generate_Gauss2D.py <NUMBER_OF_DIM> <DIM_SIZE> <%_DENSITY> <%_OF_RESULTS> <PATTERN_SIZE> <PATTERN_FILE> <OUTPUT_FILE>
		generateGauss4D.py --- generates 4D data based on Gaussian distribution
			python generate_Gauss4D.py <NUMBER_OF_DIM> <DIM_SIZE> <%_DENSITY> <%_OF_RESULTS> <PATTERN_SIZE> <PATTERN_FILE> <OUTPUT_FILE>
	C++:
		Makefile
		g++ -std=c++11 -Wall -pedantic -O3

		all *_chunks.cpp are able to work with chunked version of data
		all other implementations are not properly debugged and need further work, but are included anyway

	exact pattern matching:
		brute --- brute force solution
		exactSkipDim --- solution with skipping of first dimension
		Usage:
			./executable <INPUT_FILE> <PATTERN>
	approximate pattern matching:
		bruteApp --- brute force approximate solution
		appSplitDyn --- variation of exactSkipDim but splits all dimensions into smaller parts, dynamicaly checks possible results
		stricterFilter --- variation of appSplitDyn but with added preverification phase before dynamic check
		hash * --- solutions using hashed find phase
		Usage:
			./executable <INPUT_FILE> <PATTERN> <MAX_MISTAKES_ALLOWED>

Informations about author:
Anna Kucerova, CTU FIT, 2017
