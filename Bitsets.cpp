	

#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <tuple>
#include <cmath>
#include <algorithm>
#include "consolecolor.h"
using namespace std;

// bitsets forward and backward
// 0-8 Big endian (MSB at 8)
// 8-0 Little endian (LSB at 0)
void Example8(char letter)
{
	bitset<8> fbits;
	bitset<8> bbits;
	cout << cyan << letter << endl;
	for (int i = 0; i<8; i++)
	{
		unsigned char shift = 1 << i;
		fbits.set(7 - i, letter & shift);  // forward 0-8
		bbits.set(i, letter & shift);   // backwards  8-0
	}
	cout << green <<  "Big edian    " << fbits << endl;
	cout << yellow << "Little edian "<< bbits << endl;
}

// alphabet in bitset
void Example7()
{
	string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	// don't need upper bit for lower ASCII
	vector<bitset<7>> bset(alphabet.length());
	for (int c = 0; c < alphabet.length(); c++)
		bset[c] = alphabet[c];
	for (int c = 0; c < alphabet.length(); c++)
	{
		cout << cyan << '\t' << alphabet[c] << '\t';
		cout << purple << bset[c] << '\t';
		cout << green << hex << int(alphabet[c]) << '\t';
		cout << yellow << dec << int(alphabet[c]) << endl;
		cout << white;
	}
}

// Character MSB to LSB
void Example6()
{
	bitset<8> bset('E');  // 'E' = 01000101
	cout << blue << "E bits = " << bset.to_string() << endl;
	for (int i = 0; i < bset.size(); i++)
	{
		cout << yellow << bset[i]; // lsb to msb
		cout << green << bset[(bset.size() - 1) - i]; // msb to lsb
	}
	cout << endl;
}

struct Opcode{
	int _bit;
	string _text;

	Opcode(int b = 0, string t = "") { _bit = b; _text = t; }
	friend bool operator == (Opcode& a, Opcode& b) { return a._text.compare(b._text); }
	friend bool operator < (Opcode& a, Opcode& b) { return a._bit < b._bit; }

	template<class T>
	void swap(T& a, T& b){ //move semantics
		T temp(std::move(a));
		a = std::move(b);
		b = std::move(temp);
		cout << blue << "Swap!" << endl;
	}
	
};

struct Robot
{
	bitset<8> command;
	vector<Opcode> opcodes;
	vector<string> scmds =
	{
		"ON","OFF","FORWARD","BACKWARD","UP","DOWN","LEFT","RIGHT"
	};
	Robot()
	{
		opcodes.resize(scmds.size());
		opcodes.clear();
		for (int op = 0; op < scmds.size(); op++)
		{
			Opcode opcode(pow(2, op), scmds[op] );
			opcodes.push_back(opcode);
		}
	}
	Opcode search(string s)
	{
		Opcode oreturn(-1, "");
		for (int x = 0; x < opcodes.size(); x++)
		{
			if (opcodes[x]._text.compare(s) == 0)
			{
				oreturn = opcodes[x];
				break;
			}
		}
		return oreturn;
	}
	bool set(string scmd)
	{
		Opcode cmd = search(scmd);
		command.set(log(cmd._bit) / log(2));
		return cmd._bit > -1;
	}
	void execute()
	{
		for (int x = 0; x < opcodes.size(); x++)
			if (command.test(x))
				cout << yellow << "opcode: " << red << opcodes[x]._text << endl;
	}
};

// using bits to make decisions
void exampleRobot()
{
	Robot robot;
	robot.set("ON");
	robot.set("LEFT");
	robot.set("BACKWARD");
	robot.execute();
}

// color individual bits
void Example3()
{
	bitset<8> bset(170);
	cout << green << bset << endl;
	for (int i = 7; i > -1; i--) //lsb to msb
	{
		if (bset.test(i))
			cout << green << bset[i];
		else
			cout << yellow << bset[i];
	}
	cout << white << endl;
}

// bitset shifting
void Example2()
{
	bitset<4> bits(15);
	bitset<4> shift(1);
	for (int i = 0; i < 4; i++)
	{
		bitset<4> result = bits & shift;
		cout << green << result.to_string() << '\t'
			<< yellow << result.to_ulong() << endl;
		shift = shift << 1;
	}
}

// bitwise operators
void Example1()
{
	unsigned char first = 'A';
	unsigned char second = 'Z';
	cout << cyan;
	cout << "first & second " << (first & second) << endl;
	cout << "bitset first & second " << bitset<8>(first & second) << endl;
	cout << green;
	cout << "first | second " << (first | second) << endl;
	cout << "bitset first | second " << bitset<8>(first | second) << endl;
	cout << yellow;
	cout << "first ^ second " << (first ^ second) << endl;
	cout << "bitset first ^ second " << bitset<8>(first ^ second) << endl;
	cout << white;
}

// chars with bitsets
void Example0()
{
	for (unsigned char c = 'A'; c <= (unsigned char)('Z'); c++)
	{
		bitset<8> bset(c);
		cout << yellow << c << '\t';
		cout << cyan << '\t' << bset << '\t';
		cout << purple << dec << bset.to_ullong() << '\t';
		cout << green << hex << bset.to_ullong() << '\t';
		cout << white << endl;
	}
}

int main(){
	// cout << white << "---0---\n";
	// Example0();
	// cout << white << "---1---\n";
	// Example1();
	// cout << white << "---2---\n";
	// Example2();
	// cout << white << "---3---\n";
	// Example3();
	// cout << white << "---Robot---\n";
	// exampleRobot();
	cout << white << "Standard Move Operations in Swap" << endl;
	Opcode o = Opcode(123, "first");
	Opcode p = Opcode(456, "second");
	cout << yellow << "Object 1: " << o._bit << " " << o._text;
	cout << endl;
	cout << green << "Object 2: " << p._bit << " " << p._text;
	cout << endl;
	o.swap(o, p);
	cout << yellow << "Object 1: " << o._bit << " " << o._text;
	cout << endl;
	cout << green << "Object 2: " << p._bit << " " << p._text;

}

