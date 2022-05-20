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
        size = c.size;
        base = make_unique<vector<int>>(size);
        // for (int i = 0; i < size; i++)
        //     base->at(i) = c.base->at(i);
        copy(c.base->begin(), c.base->end(), base->begin());
    }
    void allocate(int c)
    {
        capacity = c;
        base = make_unique<vector<int>>(capacity);
        // for (int i = 0; i < capacity; i++)
        //     base->at(i) = 0;
        fill(base->begin(),base->end(),0);
    }
    void pop()
    {
        // for (int i = 1; i < size; i++)
        //     base->at(i-1) = base->at(i);
        // size--;
        base->erase(base->begin());
        size--;
    }
    void set(int v, int offset) { base->at(offset) = v; size++; }
    int get(int offset) { return base->at(offset); }
    // int length() { return size; }
    // int begin() { return base->at(0); }
    pair<int, int> length_begin() const{
        return make_pair(size, base->at(0));
    }
    int end() { return base->at(size - 1); }
    void push(int t) { set(t, size); }
    int operator[] (int offset) { return get(offset); }
};

class Trescence
{
    // int out[MAXS];
    vector<int> out;
    // int f[MAXS];
    vector<int> f;
    // int g[MAXS][MAXC];
    vector<vector<int>> g;
    // string* arr;
    unique_ptr<vector<string>> arr;
    int key;
    int states = 1;
public:
    Trescence(tuple<vector<string>,int> t)
    {
        out.reserve(MAXS);
        f.reserve(MAXS);
        g = vector<vector<int>>(MAXC , vector<int> (MAXS, -1));
        arr = make_unique<vector<string>>(get<0>(t));
        key = get<1>(t);
        memset(&out[0], 0, (int)sizeof out);
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
            out[currentState] |= (1 << i);
        }
        // for (int ch = 0; ch < MAXC; ++ch)
        //     if (g[0][ch] == -1)
        //         g[0][ch] = 0;
        replace_if(g[0].begin(), g[0].end(), [&](auto i){return i == -1;}, 0);
        memset(&f[0], -1, sizeof f);
        Minimum min(MAXS);
        for (int ch = 0; ch < MAXC; ++ch) 
        {
            if (g[0][ch] != 0) 
            {
                f[g[0][ch]] = 0;
                min.push(std::move(g[0][ch]));
            }
        }
        while (min.length_begin().first) 
        {
            int state = min.length_begin().second;
            min.pop();
            for (int ch = 0; ch <= MAXC; ++ch) 
            {
                if (g[state][ch] != -1) 
                {
                    int failure = f[state];
                    while (g[failure][ch] == -1)
                        failure = f[failure];
                    failure = g[failure][ch];
                    f[g[state][ch]] = failure;
                    out[g[state][ch]] |= out[failure];
                    min.push(std::move(g[state][ch]));
                }
            }
        }

    }
    int Next(int currentState, char nextInput)
    {
        int answer = currentState;
        int ch = nextInput - 'a';
        while (g[answer][ch] == -1)
            answer = f[answer];
        return g[answer][ch];
    }
    void Parse(string text)
    {
        int currentState = 0;
        for (int i = 0; i < text.size(); ++i) 
        {
            currentState = Next(currentState, text[i]);
            if (out[currentState] == 0)
                continue;
            for (int j = 0; j < key; ++j) 
            {
                if (out[currentState] & (1 << j)) 
                {
                    cout << "Word " << arr->at(j) << " appears from "
                        << i - arr->at(j).size() + 1 << " to " << i << endl;
                }
            }
        }
    }
};

int main()
{
    // string arr[] = { "do","go","dog","god","good","gold" };
    vector<string> arr = { "do","go","dog","god","good","gold" };
    string text = "agodogold";
    // int size = (int)sizeof(arr) / (int)sizeof(arr[0]);
    int size = (int)(sizeof(arr)/4.0);
    Trescence putre(make_tuple(arr,size));
    putre.Parse(text);
}