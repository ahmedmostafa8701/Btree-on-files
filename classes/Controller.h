//
// Created by Dell on 12/8/2022.
//
/*
 D:\FCAI\Files\FileAssignment2\btree.txt
 10
 5
 1
 3
 12
 1
 7
 24
 1
 10
 48
 1
 24
 60
 1
 14
 72

 */
#include<bits/stdc++.h>
#include "FileManager.h"

using namespace std;
#ifndef FILESASSIGNMENT_BACKEND_H
#define FILESASSIGNMENT_BACKEND_H
class Controller{
private:
    FileManager manager;
public:
    Controller(const FileManager &file) : manager(file) {}
        BTree *btree;
    void CreateIndexFileFile(char* filename, int numberOfRecords, int m){
        btree = new BTree(m, numberOfRecords, 5, 0, 1, filename);
    }
    int InsertNewRecordAtIndex(int RecordID, int reference){
        pair<int, int> p{RecordID, reference};
        return btree->insert(p);
    }

    void DeleteRecordFromIndex(int RecordID){

    }
    void DisplayIndexFileContent(){
        vector<Node> nodes = btree->getAll();
        for (auto item: nodes){
            cout << item.out();
        }
    }
    int SearchARecord(int RecordID){
        return btree->search(RecordID);
    }
};
#endif //FILESASSIGNMENT_BACKEND_H
