//
// Created by Dell on 12/28/2022.
//
#include<bits/stdc++.h>
using namespace std;
#ifndef FILEASSIGNMENT2_FORMAT_H
#define FILEASSIGNMENT2_FORMAT_H
class Format{
public:
    static string format(int x, int size, int n = 1){
        if(n == 0){
            return "";
        }
        string res = to_string(x);
        while(res.size() < size){
            res += " ";
        }
        return res + format(x, size, --n);
    }
    static string format(pair<int, int> p, int size, int n = 1){
        if(n == 0){
            return "";
        }
        string res = to_string(p.first);
        while(res.size() < size){
            res += " ";
        }
        res += to_string(p.second);
        while(res.size() < size * 2){
            res += " ";
        }
        return res + format(p, size, --n);
    }
    static list<int> splitFixed(string s, int size){
        list<int> res;
        for (int i = 0, length = s.length(); i < length; i += size) {
            string sub = s.substr(i, size);
            res.push_back(atoi(sub.c_str()));
        }
        return res;
    }
};
#endif //FILEASSIGNMENT2_FORMAT_H
