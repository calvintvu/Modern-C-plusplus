/**
ConsoleColor.h
Purpose: alows easy coloring of console output

@author Jasper Edbrooke
@version 1.0 17/10/2018
*/


#include <iostream>
using namespace std;
enum color
{
	black,  
	red,    
	green,  
	yellow, 
	blue,   
	purple,
	cyan,   
	white,
	bgblack,  
	bgred,    
	bggreen,  
	bgyellow, 
	bgblue,   
	bgpurple,
	bgcyan,   
	bgwhite,
	clear 
};

ostream& operator<<(ostream& os, color c) {
	switch (c){
		case black	: os << "\x1b[30m"; break;
		case red	: os << "\x1b[31m"; break;
		case green	: os << "\x1b[32m"; break;
		case yellow	: os << "\x1b[33m"; break;
		case blue	: os << "\x1b[34m"; break;
		case purple	: os << "\x1b[35m"; break;
		case cyan	: os << "\x1b[36m"; break;
		case white	: os << "\x1b[37m"; break;
		case bgblack: os << "\x1b[40m"; break;
		case bgred	: os << "\x1b[41m"; break;
		case bggreen: os << "\x1b[42m"; break;
		case bgyellow: os << "\x1b[43m"; break;
		case bgblue	: os << "\x1b[44m"; break;
		case bgpurple: os << "\x1b[45m"; break;
		case bgcyan	: os << "\x1b[46m"; break;
		case bgwhite: os << "\x1b[47m"; break;
		default		: os << "\x1b[0m"; break;
	}
	return os;
} 

