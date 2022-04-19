// Calvin Vu
// Lab 0: Decryption
// CIS29
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>
using namespace std;

class File{
    private:
        vector<char> rawdata;
    public:
        File(){}
        void setData(vector<char> v){rawdata = v;}
        vector<char> getData(){return rawdata;}
        void readFileToVector(string f);
};
void File::readFileToVector(string f){
    char c = 0;
    ifstream FileIn(f);
    if (!FileIn.is_open()) {
        cerr << "Error" << endl;
    }
    while (FileIn.get(c)) {
        rawdata.push_back(c);
    }
    FileIn.close();
}

class Database{
    private:
        int initR1;
        int initR2;
        vector<char> result;
        vector<char> data;
    public:
        Database(){}
        void setR1(int r){initR1=r;}
        void setR2(int r){initR2=r;}
        void setResult(vector<char> r){result=r;}
        void setData(vector<char> r){data=r;}
        int getR1(){return initR1;}
        int getR2(){return initR2;}
        vector<char> getResult(){return result;}
        vector<char> getData(){return data;}
};

class Process{
    private:
        vector<char> decrypted;
    public:
        void decrypt(Database& d);
};
void Process::decrypt(Database& d){
    int spaces = 0;
    vector<char> encrypted = d.getData();
    vector<char> rotor = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9',' ','.',',',';','!','?','(',')','-','\'','"'};
    for (int i = 0; i < rotor.size(); i++) {
        for (int j = 0; j < rotor.size(); j++) {   
            int r1 = i, r2 = j;
            int dec;
            int size = rotor.size();
            for(int i=0; i<encrypted.size(); i++){
                vector <char>::iterator itr;
                itr = find(rotor.begin(), rotor.end(), encrypted.at(i));
                int chr = distance(rotor.begin(), itr);
                dec = (chr-(r1+r2)+size)%size;
                if(dec < 0){dec += size;}
                decrypted.push_back(rotor.at(dec));
                r1++;
                if(r1 == size){
                    r2++;
                    r1 = 0;
                }
                else if(r2 == size){
                    r2 = 0;
                }
            }
            int count = 0;
            for(int i=0; i < decrypted.size(); i++){
                if(decrypted.at(i)==' '){count++;}
            }
            if(count > spaces){
                spaces = count;
                tuple<vector<char>, int, int> entry; //Tuple Exercise
                entry = make_tuple(decrypted, i, j);
                d.setResult(get<0>(entry));
                d.setR1(get<1>(entry));
                d.setR2(get<2>(entry));
            }
            decrypted.clear();
        }
    }
}

class Output{
    public:
        void printToConsole(Database& d){
            cout << "Decrypted Text: \n" << endl;
            for(int x=0;x<d.getResult().size();x++){
                cout << d.getResult().at(x);
            }
            vector<char> rotor = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9',' ','.',',',';','!','?','(',')','-','\'','"'};
            cout<< endl << "\nRotor 1 Initial Setting: "<<d.getR1()<<" "<<rotor.at(d.getR1())<<endl;
            cout<<"Rotor 2 Initial Setting: "<<d.getR2()<<" "<<rotor.at(d.getR2())<<endl;
        }
};

int main(){
    File f;
    Database d;
    Process p;
    Output o;
    f.readFileToVector("encrypt.txt");
    d.setData(f.getData());
    p.decrypt(d);
    o.printToConsole(d);
}