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
#include <queue>
#include <thread>
#include <mutex>
#include <random>
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
        float total_amt = 0;
        vector<string> hex_items;
        vector<string> items_receipt;
    public:
        Cart(string c, vector<string> v){
            cartNum=c;
            hex_items=v;
        }
        string getCartNum(){return cartNum;}
        vector<string>& getHexItems(){return hex_items;}
        void addToReceipt(Product p){
            stringstream stream;  
            stream.precision(2);
            stream << fixed;
            stream<<p.getProductPrice();  
            string temp = p.getProductName() + ": $" + stream.str();
            items_receipt.push_back(temp);
            total_amt += p.getProductPrice();
        }
        void printReceipt(){
            cout << cartNum << endl;
            cout << "-------" << endl;
            for(int i = 0; i<items_receipt.size();i++){
                cout << items_receipt.at(i) << endl;
            }
            cout << "==============================" << endl;
            cout << "Total: $" << total_amt << endl << endl;
        }
        void popItem(){
            hex_items.erase(hex_items.begin());
        }
};

class Database{
    private:
        map<string, Product> product_database;
        vector<Cart> carts_line;
        vector<queue<Cart>> carts_queue;
    public:
        map<string, Product> getMap(){return product_database;}
        vector<Cart>& getCarts(){return carts_line;}
        vector<queue<Cart>>& getCartQueue(){return carts_queue;}
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
            s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
            return product_database.find(s)->second;
        }
        void fillQueues(){
            srand(time(NULL));
            carts_queue.resize(5);
            int index = 0;
            for(int i = 0; i < carts_line.size()-1;i++){
                if(index == 5){index=0;}
                carts_queue.at(index).push(carts_line.at(i));
                index++;
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
                                Product p = Product(name,barcode,price);
                                temp.erase(remove_if(temp.begin(), temp.end(), ::isspace), temp.end());
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
            string line, word, temp, cartnum;
            while(true){
                regex pattern("[,]+");
                smatch m;
                getline(fin,line);
                cartnum=line;
                getline(fin,line);
                const string s = line;
                sregex_token_iterator iter(s.begin(), s.end(), pattern, -1);
                sregex_token_iterator end;
                vector<string> tokens(iter, end);
                for (auto &s: tokens) {
                    hexes.push_back(s);
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
        mutex mutex;
        bool done = false;
    public:
        void processBarcode(Database& d, int index){
            mutex.lock();
            if(d.getCartQueue().at(index).front().getHexItems().size()>0){
            string s = d.getCartQueue().at(index).front().getHexItems().front();
            Product p = d.findProduct(s);
            d.getCartQueue().at(index).front().addToReceipt(p);
            d.getCartQueue().at(index).front().popItem();
            }
            if(d.getCartQueue().at(index).front().getHexItems().empty()){
                d.getCartQueue().at(index).front().printReceipt();
                d.getCartQueue().at(index).pop();
            }
            mutex.unlock();
        }
        void runThreads(Database& d){
            vector<thread> threads;
            for(int i=0;i<d.getCartQueue().size();i++){
                if(d.getCartQueue().at(i).size()==0){
                    d.getCartQueue().erase(d.getCartQueue().begin()+i);
                }
                if(d.getCartQueue().size()==0){
                    done = true;
                    break;
                }
                threads.push_back(thread(&QueueManager::processBarcode, this, ref(d), i));
            }
            for(thread& t: threads){t.join();}
        }
        void run(Database& d){
            while(done==false){
                runThreads(d); 
            }
        }
};

int main(){
    Database d;
    File f;
    f.readProducts(d, "ProductPrice.xml");
    f.readCarts(d, "Carts.csv");
    QueueManager q;
    d.fillQueues();
    q.run(d);
    return 0;
}