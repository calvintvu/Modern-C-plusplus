// Calvin Vu
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <memory>
#include <utility>
#include <regex>
#include <map>
#include <bitset>
#include <sstream>
using namespace std;

class Product{
    private:
        string name;
        string barcode;
        float price;
    public:
        Product(string n, string b, float f){
            name = n;
            barcode = b;
            price = f;
        }
        void setProductName(string s){}
        void setProductBarcode(string s){}
        void setProductPrice(float f){}
        string getProductName(){return name;}
        float getProductPrice(){return price;}
};

class Database{
    private:
        map<string, Product> product_database;
        vector<Cart> carts_line;
    public:
        map<string, Product> getMap(){return product_database;}
        void insert(string b, Product p){
            product_database.insert(make_pair(b, p));
        }
        void print(){
            for (auto pair: product_database){
                cout  << pair.first << "---" << pair.second.getProductName() << "---" << pair.second.getProductPrice() << "\n";
            }
        }
};

class File{
    public:
        void readProducts(Database& d, string s){
        int count = 0;
        string name, barcode;
        float price;
        ifstream FileIn(s);
            if (FileIn.is_open()){  
                string line;
                while(getline(FileIn, line)){ 
                    regex pattern(">(.*)<");
                    smatch m;
                    const string s = line;
                    if (regex_search(s.begin(), s.end(), m, pattern)){
                            if(count==1){name = m[1];}
                            if(count==2){barcode = m[1];}
                            if(count==3){
                                price = stod(m[1]);
                                string temp = "";
                                for(int i = 0; i < 5;i++){
                                    bitset<9> set(barcode.substr(0,9));  
                                    stringstream ss;
                                    ss << hex << setfill('0') << setw(3) << set.to_ulong();
                                    string hexVal(ss.str());
                                    transform(hexVal.begin(), hexVal.end(), hexVal.begin(), ::toupper);
                                    temp += hexVal;
                                    barcode = barcode.erase(0,9);
                                }
                                // cout << temp << endl;
                                Product p = Product(name,barcode,price);
                                d.insert(temp, p);
                                temp = "";
                            }
                    }
                    count++;
                    if(count==5){count=0;}
                }
            }
        }
        void readCarts(Database&d, string s){}
};

class Cart{
    private:
        string cartNum;
        float total_amt;
        vector<string> hex_items;
        vector<string> items_receipt;
    public:

};

class QueueManager{
    private:
        vector<Cart> carts;
    public:
};


int main(){
    Database d;
    File f;
    f.readProducts(d, "ProductPrice.xml");
    d.print();


    // string binary_str("011000001");
    // bitset<9> set(binary_str);  
    // stringstream ss;
    // ss << hex << setfill('0') << setw(3) << set.to_ulong();
    // string hexVal(ss.str());
    // transform(hexVal.begin(), hexVal.end(), hexVal.begin(), ::toupper);
    // cout << hexVal << endl;
}