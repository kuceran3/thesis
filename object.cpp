#include <String>

using namespace std;

template <typename T> class Object {
	T value;
public:
	Object (T arg) {value = arg;}
	int compare (Object o) {
		return value - o.get();
	}
	T get () {return value;}

};
 
