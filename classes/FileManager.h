//
// Created by Dell on 12/8/2022.
//
#include<bits/stdc++.h>
#include "filesystem"
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
        filesystem::path path{fileName};
        fstream stream;
        if(!exists(path)){
            stream.open(fileName.c_str(), ios::out);
        }
        else{
            stream.open(fileName.c_str(), ios::in | ios::binary | ios::out);
        }
        stream.seekg(0, ios::end);
        int n = (int) stream.tellg();
        return n;
    }
    bool isEmpty(string fileName){
        return fileSize(fileName) == 0;
    }
    void deleteContent(string fileName){
        fstream stream;
        stream.open(fileName.c_str(), ios::in | ios::binary | ios::out);
        stream.clear();
        stream.close();
    }
};
#endif //FILESASSIGNMENT_FILE_H
