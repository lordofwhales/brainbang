#include <iostream>
#include <vector>
using namespace std;

template <class K, class V>
class map {
	vector<K> keys;
	vector<V> values;

 public:
	map<K,V>() {}
 	map<K,V>(int i) {}
	void put(K key, V value) {
		keys.push_back(key);
		values.push_back(value);
	}

	V get(K key) {
		for(int i=0; i<keys.size(); i++) {
			if(keys.at(i)==key) return values[i];
		}
		V p;
		return p;
	}
};
