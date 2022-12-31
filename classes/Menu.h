//
// Created by Dell on 12/8/2022.
//
#include<bits/stdc++.h>

using namespace std;
#ifndef FILESASSIGNMENT_FRONTEND_H
#define FILESASSIGNMENT_FRONTEND_H
class Menu{
private:
    list<string> menu_list;
public:
    Menu(){
        menu_list.emplace_back("Insert new record");
        menu_list.emplace_back("Delete record");
        menu_list.emplace_back("Display content");
        menu_list.emplace_back("Search a record");
        menu_list.emplace_back("Exit");
    }
    int show_menu(){
        int cnt = 1;
        for(auto & i : menu_list){
            cout<<cnt++;
            cout<<")";
            cout<<i+"\n";
        }
        int choice = -1;
        cout<<"Enter your Choice: ";
        cin>>choice;
        while(!(0<choice&&choice<=menu_list.size())){
            cout<<"*************************************\n";
            cout<<"Wrong choice number, please try again: ";
            cin>>choice;
        }
        return choice;
    }
};
#endif //FILESASSIGNMENT_FRONTEND_H
