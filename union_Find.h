//
// Created by aliha on 6/4/2022.
//

#ifndef UNION_FIND_H
#define UNION_FIND_H
#include <iostream>
#include "hashTablee.h"
template<class T>
class  Group ;

template <typename T>
class groupData{
public:

int key;
T* data;
Group<T>* top;
int size;
long long value;
groupData():key(0),size(1),top(nullptr),data(nullptr){};
groupData(int key1):key(key1),value(key1),size(1),top(nullptr),data(nullptr){};
groupData(int key1, Group<T>* G):key(key1),size(1),value(key1),top(G),data(nullptr){};

};


template < typename T>
class Group {
public:
    Group *father;
    int key;
    double baseValue;
    double  increase;
    double decrease;
    groupData<T> *main_group;



    Group():key(0),baseValue(0),increase(0),decrease(0),main_group(nullptr),father(nullptr){}

    explicit Group(int key1):key(key1),baseValue(key1),increase(0),decrease(0),main_group(nullptr),father(nullptr){}

~Group()=default;

};
template < typename T>
class UnionFind{
public:
    groupData<T>** Union_arr;
    Group<T>** Find_arr;
    int len;
    explicit UnionFind(int l):len(l){
        Union_arr= new groupData<T>*[len];
        Find_arr=new Group<T>*[len];
        for(int i=0;i<len;i++){
            (Find_arr)[i]= new Group<T> (i+1);//change no 1 (2 changes)
            (Union_arr)[i]= new groupData<T> (i+1,Find_arr[i]);
            (Find_arr)[i]->main_group=Union_arr[i];

        }
    }


    groupData<T> *Find(int to_find){
        to_find-=1;
        if (!Find_arr[to_find]) return nullptr;
        double totalIncreaseSum=0;
        double totalDecreaseSum=0;
        double toSubstractInc=0;
        double toSubstractDec=0;
            Group<T>* searching=Find_arr[to_find];
        Group<T>* wanted;
        while (searching){
            if (searching->father== nullptr) {
                wanted=searching;
                break;
            }
            totalIncreaseSum+=searching->increase;
            totalDecreaseSum+=searching->decrease;

            searching=searching->father;
        }
        searching=Find_arr[to_find];
        while (searching){
            if (searching->father!= nullptr) {
                Group<T> *temp=searching;
                searching=searching->father;
                double tempInc=temp->increase;
                double tempDec=temp->decrease;

                temp->increase=totalIncreaseSum-toSubstractInc;
                temp->decrease=totalDecreaseSum-toSubstractDec;
                toSubstractInc+=tempInc;
                totalDecreaseSum+=tempDec;

                temp->father=wanted;
            }

            break;
        }
        return wanted->main_group;
    }

    StatusType Union (int key1 ,int key2 ) {
        key1-=1;
        key2-=1;
        groupData<T> *group1 = Union_arr[key1];
        groupData<T> *group2 = Union_arr[key2];

        if (!(group2 && group1)) return FAILURE;
        if (group1->size>=group2->size){
            group2->top->father=group1->top;
            group2->top->main_group= nullptr;
            group1->size+=group2->size;



        } else{

            group1->top->father=group2->top;
            group1->top=group1->top->father;
            group2->top->main_group= nullptr;
            group1->top->main_group=group1;
            group1->size+=group2->size;



        }
        return SUCCESS;
    }
};


#endif //UNION_FIND_H
