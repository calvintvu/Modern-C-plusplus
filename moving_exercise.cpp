#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <tuple>
#include <cmath>
#include <algorithm>
#include "consolecolor.h"
using namespace std;

struct Opcode{
	int* _bit;
	string* _text;
	Opcode(int* b, string* t) { _bit = b; _text = t; }
	// friend bool operator == (Opcode& a, Opcode& b) { return a._text.compare(b._text); }
	// friend bool operator < (Opcode& a, Opcode& b) { return a._bit < b._bit; }
	template<class T>
	void swap(T& a, T& b){ //move semantics
		T temp(std::move(a));
		a = std::move(b);
		b = std::move(temp);
		cout << blue << "Swap!" << endl;
	}
};

int main(){
    cout << white << "Standard Move Operations in Swap" << endl;
    Opcode o = Opcode(new int(123), new string("first"));
    Opcode p = Opcode(new int(456), new string("second"));
    cout << yellow << "Object 1 Data Addresses: " << o._bit << " " << o._text;
	cout << endl;
	cout << green << "Object 2 Data Addresses: " << p._bit << " " << p._text;
	cout << endl;
    o.swap(o,p);
    cout << yellow << "Object 1 Data Addresses: " << o._bit << " " << o._text;
	cout << endl;
	cout << green << "Object 2 Data Addresses: " << p._bit << " " << p._text;
	cout << endl;
}
