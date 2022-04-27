// Calvin Vu
// Lab 1: Bitsets
// CIS29
// C++17
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <memory>
#include <utility>
using namespace std;

class File{
    private:
        vector<char> rawdata;
    public:
        void readFile(string f);
        vector<char> getData(){return rawdata;}
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
        unique_ptr<vector<tuple<char, string>>> morseKey;
        string decrypted;
    public:
        Process(){
            morseKey = make_unique<vector<tuple<char, string>>>();
            morseKey->push_back(make_tuple('A', "1001"));
            morseKey->push_back(make_tuple('B', "01101010"));
            morseKey->push_back(make_tuple('C', "01100110"));
            morseKey->push_back(make_tuple('D', "011010"));
            morseKey->push_back(make_tuple('E', "10"));
            morseKey->push_back(make_tuple('F', "10100110"));
            morseKey->push_back(make_tuple('G', "010110"));
            morseKey->push_back(make_tuple('H', "10101010"));
            morseKey->push_back(make_tuple('I', "1010"));
            morseKey->push_back(make_tuple('J', "10010101"));
            morseKey->push_back(make_tuple('K', "011001"));
            morseKey->push_back(make_tuple('L', "10011010"));
            morseKey->push_back(make_tuple('M', "0101"));
            morseKey->push_back(make_tuple('N', "0110"));
            morseKey->push_back(make_tuple('O', "010101"));
            morseKey->push_back(make_tuple('P', "10010110"));
            morseKey->push_back(make_tuple('Q', "01011001"));
            morseKey->push_back(make_tuple('R', "100110"));
            morseKey->push_back(make_tuple('S', "101010"));
            morseKey->push_back(make_tuple('T', "01"));
            morseKey->push_back(make_tuple('U', "101001"));
            morseKey->push_back(make_tuple('V', "10101001"));
            morseKey->push_back(make_tuple('W', "100101"));
            morseKey->push_back(make_tuple('X', "01101001"));
            morseKey->push_back(make_tuple('Y', "01100101"));
            morseKey->push_back(make_tuple('Z', "01011010"));
            morseKey->push_back(make_tuple('0', "0101010101"));
            morseKey->push_back(make_tuple('1', "1001010101"));
            morseKey->push_back(make_tuple('2', "1010010101"));
            morseKey->push_back(make_tuple('3', "1010100101"));
            morseKey->push_back(make_tuple('4', "1010101001"));
            morseKey->push_back(make_tuple('5', "1010101010"));
            morseKey->push_back(make_tuple('6', "0110101010"));
            morseKey->push_back(make_tuple('7', "0101101010"));
            morseKey->push_back(make_tuple('8', "0101011010"));
            morseKey->push_back(make_tuple('9', "0101010110"));
            morseKey->push_back(make_tuple('.', "100110011001"));
            morseKey->push_back(make_tuple(',', "010110100101"));
            morseKey->push_back(make_tuple('"', "100110100110"));
            morseKey->push_back(make_tuple('\'',"100101010110"));
            morseKey->push_back(make_tuple(' ', "11"));
        }
        void decrypt(Database& d);
        string get(){return decrypted;}
        string convert(vector<char> v);
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
            if(temp == "11"){decrypted += " ";}
            else{
                for (auto&& tuple: *morseKey){
                    char letter;
                    string binary;
                    tie(letter, binary) = tuple;
                    if(binary == currentLetterBinary){decrypted += letter;}
                }
            }   
            currentLetterBinary = "";
            temp = "";
        }    
    }
    d.setResult(decrypted);
}
string Process::convert(vector<char> v){
    string binary = "";
    for(int i=0; i<v.size();i++){
        binary.append(bitset<8>(v.at(i)).to_string());
    }
    return binary;
}

class Output{
    private:
    public:
        void print(string s){cout << "Decoded Text:\n" << s;}
};

int main(){
    File f;
    Database d;
    Process p;
    Output o;
    f.readFile("Morse.bin");
    d.setBinary(p.convert(f.getData()));
    p.decrypt(d);
    o.print(d.getResult());
}