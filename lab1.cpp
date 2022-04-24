#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

void hexdump(void *ptr, int buflen)
{
   unsigned char *buf = (unsigned char*)ptr;
   int i, j;
   for (i=0; i<buflen; i+=16) {
      printf("%06x: ", i);
      for (j=0; j<16; j++) { 
         if (i+j < buflen)
            printf("%02x ", buf[i+j]);
         else
            printf("   ");
      }
      printf(" ");
      for (j=0; j<16; j++) {
         if (i+j < buflen)
            printf("%c", isprint(buf[i+j]) ? buf[i+j] : '.');
      }
      printf("\n");
   }
}

int main()
{
   ifstream in;

   in.open("morse.bin", ios::in | ios::binary);

   if(in.is_open())
   {
      // get the starting position
      streampos start = in.tellg();

      // go to the end
      in.seekg(0, std::ios::end);

      // get the ending position
      streampos end = in.tellg();

      // go back to the start
      in.seekg(0, std::ios::beg);

      // create a vector to hold the data that
      // is resized to the total size of the file    
      std::vector<char> contents;
      contents.resize(static_cast<size_t>(end - start));

      // read it in
      in.read(&contents[0], contents.size());

      // print it out (for clarity)
      hexdump(contents.data(), contents.size());
   }
}