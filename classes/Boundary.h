//
// Created by Dell on 12/8/2022.
//
#include<bits/stdc++.h>
#include "Menu.h"
#include "Controller.h"
#include "FileManager.h"
using namespace std;
#ifndef FILESASSIGNMENT_MANAGEMENTSYSTEM_H
#define FILESASSIGNMENT_MANAGEMENTSYSTEM_H
class Boundary{
private:
    string read_data;

public:
    void run(){
        Menu menu;
        FileManager manager;
        Controller controller(manager);
        int numberOfRecords;
        int numberOfComlomns;
        string filename;
        cout << "Enter filename: ";
        cin >> filename;
        cout << "Enter number of records: ";
        cin >> numberOfRecords;
        cout << "Enter number of colomns: ";
        cin >> numberOfComlomns;
        controller.CreateIndexFileFile((char *)filename.c_str(), numberOfRecords, numberOfComlomns);
        while(true){
            int choice = menu.show_menu();
            if(choice == 1){
                int recordID;
                int reference;
                cout << "Enter record id: ";
                cin >> recordID;
                cout << "Enter record reference: ";
                cin >> reference;
                int pos = controller.InsertNewRecordAtIndex(recordID, reference);
                if(pos == -1){
                    cout << "Can't insert" << endl;
                }
                else{
                    cout << pos << endl;
                }
            }else if(choice == 2){
                int recordID;
                cout << "Enter record id: ";
                cin >> recordID;
                controller.DeleteRecordFromIndex(recordID);
            }else if(choice == 3){
                controller.DisplayIndexFileContent();
            }else if(choice == 4){
                int recordID;
                cout << "Enter record id: ";
                cin >> recordID;
                int reference = controller.SearchARecord(recordID);
                if(reference == -1){
                    cout << "Not found";
                }
                else{
                    cout << reference;
                }
                cout << endl;
            }else{
                cout<<"****************************************\n";
                cout<<"Program finished execution successfully\n";
                cout<<"****************************************\n";
                break;
            }
        }
    }
};
#endif //FILESASSIGNMENT_MANAGEMENTSYSTEM_H
