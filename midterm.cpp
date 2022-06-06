// Calvin Vu
#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <memory>
using namespace std;
const int MAXS = 500;
const int MAXC = 26;

class Minimum
{
    // int* base;
    unique_ptr<vector<int>> base;
    int size = 0;
    int capacity = 0;
public:
    Minimum(int c = 1) : capacity(c) { allocate(capacity); }
    Minimum(Minimum& c)
    {
        size = static_cast<int>(c.size);
        base = move(make_unique<vector<int>>(size));
        // for (int i = 0; i < size; i++)
        //     base->at(i) = c.base->at(i);
        copy(c.base->begin(), c.base->end(), base->begin()); 
        /*
        Opportunity to copy the vector using a stl algorithm
        */
    }
    void allocate(int c)
    {
        capacity = c;
        base = move(make_unique<vector<int>>(capacity));
        // for (int i = 0; i < capacity; i++)
        //     base->at(i) = 0;
        // fill(base->begin(),base->end(),0);
        generate(base->begin(), base->end(), []() {return 0;});
        /*
        Opportunity to fill the vector using a stl algorithm with a lambda expression
        */

    }
    void pop()
    {
        // for (int i = 1; i < size; i++)
        //     base->at(i-1) = base->at(i);
        // size--;
        // base->erase(base->begin());
        remove_if(base->begin(), base->end(), [&](auto i) { return i == base->at(0);});
        /*
        Opportunity to remove an element from the vector using a stl algorithm with a lambda expression
        */
        size--;
    }
    void set(int v, int offset) { base->at(offset) = v; size++; }
    int get(int offset) { return base->at(offset); }
    // int length() { return size; }
    // int begin() { return base->at(0); }
    pair<int, int> length_begin() const{
        return make_pair(size, base->at(0));
        /*
        Opportunity to pair two accessors with a tuple
        */
    }
    int end() { return base->at(size - 1); }
    void push(int t) { set(t, size); }
    int operator[] (int offset) { return get(offset); }
};

class Trescence
{
    // int out[MAXS];
    // vector<int> out;
    // int f[MAXS];
    // vector<int> f;
    tuple<vector<int>,vector<int>> out_f;
    /*
    Opportunity to pair vectors in a tuple
    */
    // int g[MAXS][MAXC];
    vector<vector<int>> g;
    /*
    Modify array into vectors
    */
    // string* arr;
    unique_ptr<vector<string>> arr;
    /*
    Usage of smart pointers
    */
    int key;
    int states = 1;
public:
    Trescence(tuple<vector<string>,int> t)
    {
        get<0>(out_f).reserve(MAXS);
        get<1>(out_f).reserve(MAXS);
        g = vector<vector<int>>(MAXC , vector<int> (MAXS, -1));
        arr =  move(make_unique<vector<string>>(get<0>(t)));                    // move pointer
        key = static_cast<int>(get<1>(t));                                      // casting
        memset(&get<0>(out_f)[0], 0, static_cast<int>(sizeof get<0>(out_f)));
        // memset(g, -1, sizeof g);
        for (int i = 0; i < key; ++i) 
        {
            const string& word = arr->at(i);
            int currentState = 0;
            for (int j = 0; j < word.size(); ++j) 
            {
                int ch = word[j] - 'a';
                if (g[currentState][ch] == -1)
                    g[currentState][ch] = states++;
                currentState = g[currentState][ch];
            }
            get<0>(out_f)[currentState] |= (1 << i);
        }
        // for (int ch = 0; ch < MAXC; ++ch)
        //     if (g[0][ch] == -1)
        //         g[0][ch] = 0;
        replace_if(g[0].begin(), g[0].end(), [&](auto i){return i == -1;}, 0); 
        /*
        Opportunity to replace elements using a stl algorithm and lambda
        */
        memset(&get<1>(out_f)[0], -1, static_cast<int>(sizeof get<1>(out_f)));
        Minimum min(MAXS);
        for (int ch = 0; ch < MAXC; ++ch) 
        {
            if (g[0][ch] != 0) 
            {
                get<1>(out_f)[g[0][ch]] = 0;
                min.push(move(g[0][ch]));
            }
        }
        while (min.length_begin().first) 
        {
            int state = static_cast<int>(min.length_begin().second);
            min.pop();
            for (int ch = 0; ch <= MAXC; ++ch) 
            {
                if (g[state][ch] != -1) 
                {
                    int failure = get<1>(out_f)[state];
                    while (g[failure][ch] == -1)
                        failure = get<1>(out_f)[failure];
                    failure = g[failure][ch];
                    get<1>(out_f)[g[state][ch]] = failure;
                    get<0>(out_f)[g[state][ch]] |= get<0>(out_f)[failure];
                    min.push(move(g[state][ch]));
                }
            }
        }

    }
    int Next(int currentState, char nextInput)
    {
        int answer = currentState;
        int ch = nextInput - 'a';
        while (g[answer][ch] == -1)
            answer = get<1>(out_f)[answer];
        return g[answer][ch];
    }
    void Parse(string text)
    {
        int currentState = 0;
        for (int i = 0; i < text.size(); ++i) 
        {
            currentState = Next(currentState, text[i]);
            if (get<0>(out_f)[currentState] == 0)
                continue;
            for (int j = 0; j < key; ++j) 
            {
                if (get<0>(out_f)[currentState] & (1 << j)) 
                {
                    auto word = find_if(arr->begin(), arr->end(),
                    [&](string s) -> bool{return s.compare(arr->at(j))==0;}); 
                    //stl algorithm and lambda
                    cout << "Word " << word->data() << " appears from "
                        << i - word->size() + 1 << " to " << i << endl;
                }
            }
        }
    }
};

int main()
{
    // string arr[] = { "do","go","dog","god","good","gold" };
    vector<string> arr = {"do","go","dog","god","good","gold"};
    string text = "agodogold";
    // int size = (int)sizeof(arr) / (int)sizeof(arr[0]);
    int size = static_cast<int>(sizeof(arr)/4.0);
    Trescence putre(make_tuple(arr,size));
    putre.Parse(text);
}