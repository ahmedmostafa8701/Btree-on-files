//
// Created by Dell on 12/28/2022.
//

#ifndef FILEASSIGNMENT2_NODE_H
#define FILEASSIGNMENT2_NODE_H

#include "FileObject.h"
#include "Format.h"
#include<bits/stdc++.h>
#include<bits/stdc++.h>
using namespace std;
using namespace std;
class Node : public FileObject{
private:
    int colomnSize;
    int m;
    int leafStatus;
    int next;
    map<int, int> colomns;
public:
    Node(int colomnSize, int m, int leafStatus = -1) : colomnSize(colomnSize),
                                                                m(m),
                                                                leafStatus(leafStatus) {
        next = -1;
    };

    int getLeafStatus() const {
        return leafStatus;
    }

    void setLeafStatus(int leafStatus) {
        Node::leafStatus = leafStatus;
    }

    const map<int, int> &getColomns() const {
        return colomns;
    }

    void setColomns(const map<int, int> &colomns) {
        Node::colomns = colomns;
    }

    void setNext(int next){
       this->next = next;
    }

    int getNext() const {
        return next;
    }
    bool IsEmpty(){
        return colomns.size() == 0;
    }
    bool isFull(){
        return colomns.size() == m;
    }
    int size(){
        return colomns.size();
    }
    void insert(pair<int, int> p){
        colomns.insert(p);
    }
    int find(int key){
        auto it = colomns.find(key);
        if(it != colomns.end()){
            return it->second;
        }
        return -1;
    }
    int remove(int key){
        map<int, int>::iterator iterator = colomns.find(key);
        if(iterator != colomns.end()){
            int val = iterator->first;
            colomns.erase(iterator);
            return val;
        }
        return -1;
    }
    Node split(){
        Node node1(colomnSize, m, leafStatus);
        int d1 = colomns.size() / 2;
        int i = 0;
        auto it = colomns.begin();
        while (it != colomns.end() && i < d1){
            node1.insert(*it);
            it++;
            i++;
        }
        colomns.erase(colomns.begin(), it);
        swap(colomns, node1.colomns);
        return node1;
    }
    int maxElement(){
        return std::max_element(colomns.begin(), colomns.end())->first;
    }
    string out() override {
        string s  = "";
        int diff = m - colomns.size();
        s += Format::format(leafStatus, colomnSize);
        for (auto [k, v] : colomns){
            s+= Format::format({k, v} , colomnSize);
        }
        if(colomns.size() == 0){
            s+= Format::format({next, -1}, colomnSize);
            diff--;
        }
        s+= Format::format({-1, -1}, colomnSize, diff);
        s += '\n';
        return s;
    }
    void in(string s) override {
        leafStatus = atoi(s.substr(0, colomnSize).c_str());
        if(leafStatus == -1){
            next = atoi(s.substr(colomnSize, colomnSize).c_str());
            return;
        }
        colomns.clear();
        for (int i = 1; i < 2 * m + 1; i += 2) {
            int key = atoi(s.substr(i * colomnSize, colomnSize).c_str());
            int value = atoi(s.substr(i * colomnSize+ colomnSize, colomnSize).c_str());
            if(key == -1){
                break;
            }
            colomns.insert({key, value});
        }
    }
};
#endif //FILEASSIGNMENT2_NODE_H
