//#include <boost/chrono.hpp>
#include <chrono>
#include <cmath>
#include <iostream>

using namespace std;

int main()
{
    chrono::system_clock::time_point start = chrono::system_clock::now();

    for ( long i = 0; i < 1000000000; ++i )
    	std::sqrt( 123.456L ); // burn some time

    chrono::duration<double> sec = chrono::system_clock::now() - start;
    cout << "took " << sec.count() << " seconds\n";
    return 0;
}
