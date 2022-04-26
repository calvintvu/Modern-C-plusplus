#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
using namespace std;

class File{
    private:
        vector<char> rawdata;
        string binary;
    public:
        void readFile(string f);
        string convert();
        vector<char> getData(){return rawdata;}
        string getBinary(){return binary;}
};
void File::readFile(string f){ 
    ifstream in;
    in.open(f, ios::in | ios::binary);
    if(in.is_open()){
        streampos start = in.tellg();
        in.seekg(0, ios::end);
        streampos end = in.tellg();
        in.seekg(0, ios::beg);
        rawdata.resize(static_cast<size_t>(end-start));
        in.read(&rawdata[0], rawdata.size());
    }
    in.close();
}
string File::convert(){
    binary = "";
    for(int i = 0; i<rawdata.size();i++){
        binary.append(bitset<8>(rawdata.at(i)).to_string());
    }
    return binary;
}

class Database{
    private:
        string result;
        string binary;
    public:
        void setBinary(string b){binary = b;}
        string getBinary(){return binary;}
        void setResult(string r){result = r;}
        string getResult(){return result;}
};

class Process{
    private:
        vector<tuple<char, string>> charList;
        string decrypted;
    public:
        Process(){
            charList.push_back(make_tuple('A', "1001"));
            charList.push_back(make_tuple('B', "01101010"));
            charList.push_back(make_tuple('C', "01100110"));
            charList.push_back(make_tuple('D', "011010"));
            charList.push_back(make_tuple('E', "10"));
            charList.push_back(make_tuple('F', "10100110"));
            charList.push_back(make_tuple('G', "010110"));
            charList.push_back(make_tuple('H', "10101010"));
            charList.push_back(make_tuple('I', "1010"));
            charList.push_back(make_tuple('J', "10010101"));
            charList.push_back(make_tuple('K', "011001"));
            charList.push_back(make_tuple('L', "10011010"));
            charList.push_back(make_tuple('M', "0101"));
            charList.push_back(make_tuple('N', "0110"));
            charList.push_back(make_tuple('O', "010101"));
            charList.push_back(make_tuple('P', "10010110"));
            charList.push_back(make_tuple('Q', "01011001"));
            charList.push_back(make_tuple('R', "100110"));
            charList.push_back(make_tuple('S', "101010"));
            charList.push_back(make_tuple('T', "01"));
            charList.push_back(make_tuple('U', "101001"));
            charList.push_back(make_tuple('V', "10101001"));
            charList.push_back(make_tuple('W', "100101"));
            charList.push_back(make_tuple('X', "01101001"));
            charList.push_back(make_tuple('Y', "01100101"));
            charList.push_back(make_tuple('Z', "01011010"));
            charList.push_back(make_tuple('0', "0101010101"));
            charList.push_back(make_tuple('1', "1001010101"));
            charList.push_back(make_tuple('2', "1010010101"));
            charList.push_back(make_tuple('3', "1010100101"));
            charList.push_back(make_tuple('4', "1010101001"));
            charList.push_back(make_tuple('5', "1010101010"));
            charList.push_back(make_tuple('6', "0110101010"));
            charList.push_back(make_tuple('7', "0101101010"));
            charList.push_back(make_tuple('8', "0101011010"));
            charList.push_back(make_tuple('9', "0101010110"));
            charList.push_back(make_tuple('.', "100110011001"));
            charList.push_back(make_tuple(',', "010110100101"));
            charList.push_back(make_tuple('"', "100110100110"));
            charList.push_back(make_tuple('\'', "100101010110"));
            charList.push_back(make_tuple(' ', "11"));
        }
        void decrypt(Database& d);
        string get(){return decrypted;}
};
void Process::decrypt(Database& d){
    string b = d.getBinary();
    string temp, currentLetterBinary;
    for(int i = 0; i < b.length(); i++){
        temp += b[i];
        if((temp.length() >= 2) && (temp != "00") && (temp != "11")){
            currentLetterBinary += temp;
            temp = "";
        }
        else if(temp == "00" || temp == "11"){
            if(temp == "11"){decrypted+=" ";}
            else{
                for (auto&& tuple: charList){
                    char X;
                    string Y;
                    tie(X, Y) = tuple;
                    if( Y == currentLetterBinary){decrypted += X;}
                }
            }   
            currentLetterBinary = "";
            temp = "";
        }    
    }
    d.setResult(decrypted);
}

class Output{
    private:
    public:
        void print(string s){
            cout << s;
        }
};

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

int main(){
    File f;
    Database d;
    Process p;
    Output o;
    f.readFile("morse.bin");
    // hexdump(f.getData().data(),f.getData().size());
    d.setBinary(f.convert());
    p.decrypt(d);
    o.print(d.getResult());
}