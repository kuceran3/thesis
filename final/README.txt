When running the created C++ executables it can be done by command:

./executable_name input_data input_pattern.csv errors  

where input data have to be chunked and name of the meta file is expected, input pattern is a standard csv file with the specified dimensions and attributes, errors is a parameter used only in executables of approximate searches and defines maximum number of errors allowed for the result to still be valid.

Python scripts can be run using:

chunking
python3 script_name.py 
