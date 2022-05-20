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
using namespace std;

class Node{
    public:
        Node(){}
        virtual float freq()=0;
        virtual string symbol()=0;
};

class Branch: public Node{
    private:
        Node* left;
        Node* right;
    public:
        Branch(Node* n0, Node* n1){
            left=n0;
            right=n1;
        };
        Node* getLeft(){return left;}
        Node* getRight(){return right;}
        float freq(){return left->freq()+right->freq();}
        string symbol(){return left->symbol()+right->symbol();}
};

class Leaf: public Node{
    private:
        float _freq;
        string _symbol;
    public:
        Leaf(string s, float f){
            _freq=f;
            _symbol=s;
        }
        float freq(){return _freq;}
        string symbol(){return _symbol;}
};

class Priority_Queue
{
	vector<Node*> vdata;
public:
    vector<Node*> get(){return vdata;}
	Priority_Queue() { }
	Node* top()
	{
		if (vdata.size() > 0)
			return vdata[0];
		return *vdata.end();
	}
	int size() { return vdata.size(); }
	bool empty() { return vdata.size() < 1; }
	void push(Node* n)
	{
		// vdata.push_back(n);
		// sort(vdata.begin(), vdata.end(),
		// 	[](Node* a, Node* b)
		// 	{
		// 		return a->freq() < b->freq();
		// 	});
        vector<Node*>::iterator itr = vdata.begin();
        while (itr!=vdata.end() && (*itr)->freq() <= n->freq()){
            itr++;}
        vdata.insert(itr, n);
	}
	void pop()
	{
		vdata.erase(vdata.begin());
	}
	void print()
	{
		for_each(vdata.begin(), vdata.end(),
			[](Node* n)
			{
				cout << n->symbol() << ' ' << n->freq() << endl;
			});
	}
};

class QueueTree{
    private:
        vector<Node*> f_tree;
        vector<string> dec;
    public:
        void priority_tree(Priority_Queue &p);
        void encode(Priority_Queue q, string &b);
};
void QueueTree::priority_tree(Priority_Queue &p){
    do{
        Node* QLeft = p.top();
        p.pop();
        Node* QRight = p.top();
        p.pop();
        Branch* node = new Branch(QLeft, QRight);
        p.push(node);
    }
    while(p.get().size()>1);
}
void QueueTree::encode(Priority_Queue q, string &b){
    string sol ="";
    Node* root = q.get()[0];
    for(int i=0;i<b.length();i++){
        if(b[i]=='0'){
            if(root->symbol().length() > 1){
                root = dynamic_cast<Branch*>(root)->getLeft();
            }
            if(root->symbol().length()==1){
                sol+=root->symbol();
                root = q.get()[0];
            }
        }
        if(b[i]=='1'){
            if(root->symbol().length() > 1){
                root = dynamic_cast<Branch*>(root)->getRight();
            }
            if(root->symbol().length()==1){
                sol+=root->symbol();
                root = q.get()[0];
            }
        }
    }
    cout << sol << endl;
}

class File{
    private:
        map<string,float> table;
        vector<char> bindata;
    public:
        map<string,float> getTable(){return table;}
        vector<char> getBin(){return bindata;}
        void readText(string f);
        void readBin(string f);
};
void File::readText(string f){
    ifstream FileIn(f);
    string ascii,frequency;
    if (FileIn.is_open()){  
        string line;
        while(getline(FileIn, line)){ 
            regex pattern("'(.*)',([0-9]+\\.?[0-9]*)");
            smatch m;
            while (regex_search (line,m,pattern)) {
                for (auto x:m){
                    ascii=m[1];
                    if(ascii.length()>1){ascii.erase(0,1);}
                    frequency=m[2];
                }
                table.insert(make_pair(ascii,stod(frequency)));
                line = m.suffix();
            }
        }
    }
}
void File::readBin(string f){
    ifstream in;
    in.open(f, ios::in | ios::binary);
    if(in.is_open()){
        streampos start = in.tellg();
        in.seekg(0, ios::end);
        streampos end = in.tellg();
        in.seekg(0, ios::beg);
        bindata.resize(static_cast<size_t>(end-start));
        in.read(&bindata[0], bindata.size());
    }
    in.close();
}

class Data{
    private:
        map<string,float> table;
        string binary;
    public:
        void setTable(map<string,float> m){table=m;}
        string getBin(){return binary;}
        map<string,float> get(){return table;}
        void print_map(){
            int i=0;
            for (auto const &pair: table){
                cout << i+1 << ") " << pair.first << " " << pair.second << "\n";
                i++;
            }
        }
        void convert(vector<char> v){
            for(int i=0;i<v.size();i++){
                unsigned char temp = (unsigned char)v.at(i);
                binary.append(bitset<8>(temp).to_string());
            }
        }
        void printBin(){cout << binary << endl;}
};

int main(){
    File f;
    f.readText("AsciiFrequenciesV3.txt");
    f.readBin("Compress.bin");
    Data d;
    d.setTable(f.getTable());
    d.convert(f.getBin());
    Priority_Queue p;
    map<string,float> m = d.get();
    for (auto const &pair: m){
        Leaf* l = new Leaf(pair.first,pair.second);
        p.push(l); 
    }
    QueueTree t;
    t.priority_tree(p);
    string str = d.getBin();
    t.encode(p, str);
}