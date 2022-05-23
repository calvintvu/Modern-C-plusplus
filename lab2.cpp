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
        shared_ptr<Node> left;
        shared_ptr<Node> right;
    public:
        Branch(shared_ptr<Node> n0, shared_ptr<Node> n1){
            left=n0;
            right=n1;
        };
        shared_ptr<Node> getLeft(){return left;}
        shared_ptr<Node> getRight(){return right;}
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
	vector<shared_ptr<Node>> vdata;
public:
    vector<shared_ptr<Node>> get(){return vdata;}
	Priority_Queue() { }
	shared_ptr<Node> top()
	{
		if (vdata.size() > 0)
			return vdata[0];
		return *vdata.end();
	}
	int size() { return vdata.size(); }
	bool empty() { return vdata.size() < 1; }
	void push(shared_ptr<Node> n)
	{
		// vdata.push_back(n);
		// sort(vdata.begin(), vdata.end(),
		// 	[](shared_ptr<Node> a, shared_ptr<Node> b)
		// 	{
		// 		return a->freq() < b->freq();
		// 	});
        vector<shared_ptr<Node>>::iterator itr = vdata.begin();
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
			[](shared_ptr<Node> n)
			{
				cout << n->symbol() << ' ' << n->freq() << endl;
			});
	}
};

class QueueTree{
    private:
        string solu;
    public:
        void priority_tree(Priority_Queue &p);
        void encode(Priority_Queue q, string b);
        string getsolu(){return solu;}
};
void QueueTree::priority_tree(Priority_Queue &p){
    do{
        shared_ptr<Node> QLeft = p.top();
        p.pop();
        shared_ptr<Node> QRight = p.top();
        p.pop();
        shared_ptr<Branch> node = make_shared<Branch>(QLeft, QRight);
        p.push(node);
    }
    while(p.get().size()>1);
}
void QueueTree::encode(Priority_Queue q, string b){
    solu ="";
    shared_ptr<Node> root = q.get()[0];
    for(int i=0;i<b.length();i++){
        if(b[i]=='0'){
            if(root->symbol().length() > 1){
                root = dynamic_pointer_cast<Branch>(root)->getLeft();
            }
            if(root->symbol().length()==1){
                solu+=root->symbol();
                root = q.get()[0];
            }
        }
        if(b[i]=='1'){
            if(root->symbol().length() > 1){
                root = dynamic_pointer_cast<Branch>(root)->getRight();
            }
            if(root->symbol().length()==1){
                solu+=root->symbol();
                root = q.get()[0];
            }
        }
    }
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
        string solu;
    public:
        void setTable(map<string,float> m){table=m;}
        void insertsolution(string s){solu = s;}
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
        void solution(){cout << solu << endl;}
};

int main(){
    File f;
    f.readText("AsciiFrequenciesV3.txt");
    f.readBin("Compress.bin");
    Data d;
    d.setTable(f.getTable());
    d.convert(f.getBin());
    Priority_Queue p;
    for (auto const &pair: d.get()){
        shared_ptr<Leaf> l = make_shared<Leaf>(pair.first,pair.second);
        p.push(l); 
    }
    QueueTree t;
    t.priority_tree(p);
    t.encode(p, d.getBin());
    d.insertsolution(t.getsolu());
    d.solution();
}