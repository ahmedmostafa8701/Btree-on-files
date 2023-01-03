//
// Created by Dell on 12/28/2022.
//

#ifndef FILEASSIGNMENT2_BTREE_H
#define FILEASSIGNMENT2_BTREE_H

#include "FileManager.h"
#include "Node.h"

class BTree{
private:
    FileManager manager;
    int m;
    int n;
    int recordLength;
    int colomnSize;
    string filePath;
    int rootPos;
    int headerPos;
    const static int POP_LOW = 0;
    const static int POP_HIGH = 1;
    const static int EMPTYCOLOMNS = -2;
public:
    BTree(int m, int n, int colomnSize, int headerPos,int rootPos, const string &filePath = "") : m(m), n(n), colomnSize(colomnSize), filePath(
            filePath) {
        recordLength = (2 * m + 1) * colomnSize + 1;
        this->filePath = filePath;
        if(filePath == ""){
            this->filePath = manager.btree;
        }
        this->rootPos = rootPos;
        this->headerPos = headerPos;
        int size = manager.fileSize(filePath);
        if(size == 0 || size != recordLength * n){
            manager.deleteContent(filePath);
            createIndexFileFile();
        }
    }

    void createIndexFileFile(){
        for (int i = 0; i < n; i++){
            Node node(colomnSize, m, -1);
            if(i == n - 1){
                node.setNext(-1);
            }
            else
            {
                node.setNext(i + 1);
            }
            writeNode(node, i);
        }
    }
    int insert(pair<int, int> p){
        vector<int> parents;
        int pos = getInsertedPos(p.first, parents);
        if(pos == EMPTYCOLOMNS){
            Node node(colomnSize, m, 0);
            node.insert(p);
            return insert(node);
        }
        if(pos == -1){
            return -1;
        }
        Node node = readNode(pos);
        if(!node.isFull()){
            if(node.getLeafStatus() == -1){
                node.setLeafStatus(0);
            }
            int oldMax = node.maxElement();
            node.insert(p);
            if(parents.size() != 0){
                Node parent = readNode(parents.back());
                parent.remove(oldMax);
                parent.insert({node.maxElement(), pos});
                writeNode(parent, parents.back());
            }
            writeNode(node, pos);
            return pos;
        }
        int extra = 1;
        parents.push_back(pos);
        int i = parents.size() - 2;
        while(i > -1) {
            Node p = readNode(parents[i]);
            if(!p.isFull()){
                break;
            }
            extra++;
            if(parents[i] == rootPos){
                extra++;
            }
            i--;
        }
        if(parents.size() == 1){
            extra++;
        }
        vector<int> avail = getAvail(extra);
        if(avail.size() == 0){
            return -1;
        }
        return split(parents, extra, p);
    }
    int split(vector<int> chain, int splited, pair<int, int> p){
        if(chain.size() == 0){
            return -1;
        }
        if(splited == 0){
            Node node = readNode(chain.back());
            int oldMax = node.maxElement();
            node.insert(p);
            writeNode(node, chain.back());
            if(chain.back() != rootPos){
                Node parent = readNode(chain[chain.size() - 2]);
                parent.remove(oldMax);
                parent.insert({node.maxElement(), chain.back()});
                writeNode(parent, chain[chain.size() - 2]);
            }
            return chain.back();
        }
        if(chain.size() == 1){
            // It's guaranteed that chain.size() == root position
            Node node1 = readNode(chain.back());
            node1.insert(p);
            Node node2 = node1.split();
            int pos1 = insert(node1);
            int pos2 = insert(node2);
            Node root(colomnSize, m, 1);
            root.insert({node1.maxElement(), pos1});
            root.insert({node2.maxElement(), pos2});
            writeNode(root, chain.back());
            if(node1.getColomns().find(p.first) != node1.getColomns().end()){
                return pos1;
            }
            return pos2;
        }
        int pos = chain.back();
        Node node = readNode(pos);
        int oldMax = node.maxElement();
        node.insert(p);
        Node node1 = node.split();
        writeNode(node, pos);
        int pos1 = insert(node1);
        Node parent = readNode(chain[chain.size() - 2]);
        parent.remove(oldMax);
        parent.insert({node.maxElement(), chain.back()});
        writeNode(parent, chain[chain.size() - 2]);
        splited--;
        chain.pop_back();
        split(chain, splited, {node1.maxElement(), pos1});
        if(node1.getColomns().find(p.first) != node1.getColomns().end()){
            return pos1;
        }
        return pos;
    }
    int insert(Node node){
        vector<int> v = getAvail();
        if(v.empty()){
            return -1;
        }
        Node avail = readNode(v.front());
        Node header = readNode(headerPos);
        header.setNext(avail.getNext());
        writeNode(header, headerPos);
        writeNode(node,v.front());
        return v.front();
    }
    int getInsertedPos(int key, vector<int> &v){
        return getInsertedPos(rootPos, key, v);
    }
    int getInsertedPos(int pos, int key, vector<int> &v){
        Node node = readNode(pos);
        if(node.getLeafStatus() == -1){
            return EMPTYCOLOMNS;
        }
        if(node.getLeafStatus() == 0){
            if(node.find(key) == -1){
                return pos;
            }
            return -1;
        }
        v.push_back(pos);
        for (auto item: node.getColomns()){
            if(key < item.first){
                return getInsertedPos(item.second, key, v);
            }
            if(key == item.first){
                return -1;
            }
        }
        return getInsertedPos(std::max_element(node.getColomns().begin(), node.getColomns().end())->second, key, v);
    }
    int search(int key){
        return search(rootPos, key);
    }
    int search(int pos, int key){
        Node node = readNode(pos);
        if(node.getLeafStatus() == -1){
            return -1;
        }
        for (auto item: node.getColomns()){
            if(node.getLeafStatus() == 1){
                if(key <= item.first){
                    return search(item.second, key);
                }
            }
            else{
                if(key < item.first){
                    return -1;
                }
                if(key == item.first){
                    return item.second;
                }
            }
        }
        return -1;
    }
    void remove(int key){
        vector<int> parents;
        int pos = getDeletedPos(key, parents);
        if(pos == -1 || pos == EMPTYCOLOMNS){
            return;
        }
        remove(key, pos, parents);
    }
    int remove(int key, int pos, vector<int> &parents){
        Node node = readNode(pos);
        int large = node.maxElement();
        node.remove(key);
        if(node.size() == 0){
            deleteNode(pos);
            if(parents.size() != 0){
                int parent = parents.back();
                parents.pop_back();
                parent = remove(large, parent, parents);
                if(parent != -1){
                    parents.push_back(parent);
                }
            }
            return -1;
        }
        writeNode(node, pos);
        if(parents.size() == 0){
            return pos;
        }
        updateMax(parents, {node.maxElement(), pos}, large);
        if(node.size() < m / 2){
            Node parent = readNode(parents.back());
            auto colomns = parent.getColomns();
            auto it = colomns.find(node.maxElement());
            auto prev = it;
            auto next = it;
            if(prev != colomns.begin() && redistribution(pos, (--prev)->second, parents, POP_HIGH)){
                return pos;
            }
            else if(++next != colomns.end() && redistribution(pos, next->second, parents, POP_LOW)){
                return pos;
            }
            prev = it;
            next = it;
            if(prev != colomns.begin() && merge(pos, (--prev)->second, parents)){
                if(parents.size() == 1){
                    Node parentNew = readNode(parents.back());
                    if(parentNew.size() == 1){
                        node = readNode(prev->second);
                        writeNode(node, parents.back());
                        deleteNode(prev->second);
                        return -1;
                    }
                }
                return prev->second;
            }
            else if(++next != colomns.end() && merge(pos, next->second, parents)){
                if(parents.size() == 1){
                    Node parentNew = readNode(parents.back());
                    if(parentNew.size() == 1){
                        node = readNode(next->second);
                        writeNode(node, parents.back());
                        deleteNode(next->second);
                        return -1;
                    }
                }
                return next->second;
            }
        }
        return pos;
    }
    bool merge(int sourcePos, int destinationPos, vector<int> &parents){
        Node source = readNode(sourcePos);
        Node destination = readNode(destinationPos);
        int largeSource = source.maxElement();
        int largeDestination = destination.maxElement();
        int parent = parents.back();
        if(source.size() == 0){
            deleteNode(sourcePos);
            return true;
        }
        if(source.size() + destination.size() > m){
            return false;
        }
        for (const auto &item: source.getColomns()){
            destination.insert(item);
        }
        writeNode(destination, destinationPos);
        deleteNode(sourcePos);
        parents.pop_back();
        parent = remove(largeSource, parent, parents);
        if(parent != -1){
            parents.push_back(parent);
            updateMax(parents, {destination.maxElement(), destinationPos}, largeDestination);
        }
    }
    bool redistribution(int sourcePos, int destinationPos, vector<int> parents, int pop){
        Node source = readNode(sourcePos);
        Node destination = readNode(destinationPos);
        if(destination.size() - 1 < m / 2 || destination.size() == 1){
            return false;
        }
        if(pop == POP_LOW){
            int oldMax = source.maxElement();
            source.insert(destination.popFront());
            updateMax(parents, {source.maxElement(), sourcePos}, oldMax);
        }
        else if(pop == POP_HIGH){
            int oldMax = destination.maxElement();
            source.insert(destination.popBack());
            updateMax(parents, {destination.maxElement(), destinationPos}, oldMax);
        }
        writeNode(source, sourcePos);
        writeNode(destination, destinationPos);
    }
    int getDeletedPos(int key, vector<int> &v){
        return getDeletedPos(rootPos, key, v);
    }
    int getDeletedPos(int pos, int key, vector<int> &v){
        Node node = readNode(pos);
        if(node.getLeafStatus() == -1){
            return EMPTYCOLOMNS;
        }
        if(node.getLeafStatus() == 0){
            if(node.find(key) != -1){
                return pos;
            }
            return -1;
        }
        v.push_back(pos);
        for (auto item: node.getColomns()){
            if(key <= item.first){
                return getDeletedPos(item.second, key, v);
            }
        }
        return -1;
    }
    void updateMax(vector<int> chain, pair<int, int> updated, int old){
        if(chain.size() == 0){
            return;
        }
        int pos = chain.back();
        Node node = readNode(pos);
        int oldMax = node.maxElement();
        node.remove(old);
        node.insert(updated);
        writeNode(node, pos);
        chain.pop_back();
        updateMax(chain, {node.maxElement(), pos}, oldMax);
    }
    void deleteNode(int pos){
        Node node = readNode(pos);
        Node header = readNode(headerPos);
        node.removeAll();
        node.setLeafStatus(-1);
        node.setNext(header.getNext());
        header.setNext(pos);
        writeNode(header, headerPos);
        writeNode(node, pos);
    }
    vector<Node> getAll(){
        return getAll(rootPos);
    }
    vector<Node> getAll(int pos){
        vector<Node> res;
        Node node = readNode(pos);
        if(node.getLeafStatus() == -1){
            return res;
        }
        res.push_back(node);
        if(node.getLeafStatus() != 0){
            for (const auto &item: node.getColomns()){
                vector<Node> v = getAll(item.second);
                for (auto item: v){
                    res.push_back(item);
                }
            }
        }
        return res;
    }
    vector<int> getAvail(int n = 1){
        vector<int> res;
        Node node= readNode(headerPos);
        while (node.getLeafStatus() == -1 && node.getNext() != -1 && res.size() < n){
            res.push_back(node.getNext());
            node = readNode(node.getNext());
        }
        return res;
    }
    Node readNode(int pos){
        Node node(colomnSize, m);
        node.readFromFile(filePath, pos * recordLength, recordLength);
        return node;
    }
    void writeNode(Node node, int pos){
        node.writeToFile(filePath, pos * recordLength, recordLength);
    }
    bool isAvail(int pos){
        return true;
    }
};
#endif //FILEASSIGNMENT2_BTREE_H
