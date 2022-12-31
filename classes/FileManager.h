//
// Created by Dell on 12/8/2022.
//
#include<bits/stdc++.h>
using namespace std;
#ifndef FILESASSIGNMENT_FILE_H
#define FILESASSIGNMENT_FILE_H
class FileManager{
public:
    string btree;

    FileManager() {
        btree = "D:\\FCAI\\Files\\FileAssignment2\\btree.txt";
    }

    int records(string fileName, int len){
        return fileSize(fileName) / len;
    }
    int fileSize(string fileName){
        fstream stream;
        stream.open(fileName.c_str(), ios::in | ios::binary| ios::out);
        stream.seekg(0, ios::end);
        int n = (int) stream.tellg();
        return n;
    }
    bool isEmpty(string fileName){
        fstream stream;
        stream.open(fileName.c_str(), ios::in | ios::binary | ios::out);
        bool empty = stream.peek() == std::ifstream::traits_type::eof();
        stream.close();
        if(stream.peek() == std::ifstream::traits_type::eof()){
            return true;
        }
        return false;
    }
    void deleteContent(string fileName){
        fstream stream;
        stream.open(fileName.c_str(), ios::in | ios::binary | ios::out);
        stream.clear();
        stream.close();
    }
};
#endif //FILESASSIGNMENT_FILE_H
