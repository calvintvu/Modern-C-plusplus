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

class Cart{
    private:
        string cartNum;
        float total_amt;
        vector<string> hex_items;
        vector<string> items_receipt;
    public:
        Cart(string c, vector<string> v){
            cartNum=c;
            hex_items=v;
        }
        string getCartNum(){return cartNum;}
        vector<string> getHexItems(){return hex_items;}

};

class Database{
    private:
        map<string, Product> product_database;
        vector<Cart> carts_line;
    public:
        map<string, Product> getMap(){return product_database;}
        void insertProduct(string b, Product p){
            product_database.insert(make_pair(b, p));
        }
        void printProduct(){
            for (auto pair: product_database){
                cout  << pair.first << "---" << pair.second.getProductName() << "---" << pair.second.getProductPrice() << "\n";
            }
        }
        void insertCart(Cart c){
            carts_line.push_back(c);
        }
        void printCarts(){
            for(int i=0;i<carts_line.size()-1;i++){
                cout << carts_line.at(i).getCartNum() << endl;
                for(int j=0;j<carts_line.at(i).getHexItems().size();j++){
                    cout << carts_line.at(i).getHexItems().at(j) << endl;
                }
                cout << endl;
            }
        }
        Product findProduct(string s){
            return product_database.find(s)->second;
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
                                Product p = Product(name,barcode,price);
                                d.insertProduct(temp, p);
                                temp = "";
                            }
                    }
                    count++;
                    if(count==5){count=0;}
                }
            }
            FileIn.close();
        }
        void readCarts(Database&d, string s){
            ifstream fin;
            fin.open(s, ios::in);
            vector<string> hexes;
            string line, word, temp;
            string cartnum;
            while (true) {
                getline(fin, line);
                cartnum = line;
                getline(fin, line);
                stringstream s(line);
                while (getline(s, word, ',')) {
                    if(word.length()>0 || word != ""){hexes.push_back(word);}
                }
                Cart c = Cart(cartnum,hexes);
                d.insertCart(c);
                hexes.clear();
                if(fin.eof()){break;}
            }
            fin.close();
        }
};


class QueueManager{
    private:
        vector<Cart> carts;
        vector<vector<Cart>> queues;
    public:
};


int main(){
    Database d;
    File f;
    f.readProducts(d, "ProductPrice.xml");
    // d.printProduct();
    f.readCarts(d, "Carts.csv");
    // d.printCarts();

    // Product test = d.findProduct("00D1061090C1190");
    // cout << test.getProductName() <<" "<<test.getProductPrice() <<  endl;

}