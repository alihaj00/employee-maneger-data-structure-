//
// Created by aliha on 6/3/2022.
//

#ifndef UNTITLED6_HASHTABLEE_H
#define UNTITLED6_HASHTABLEE_H

#include "listt.h"
#include "math.h"
#include "assert.h"

#define START_SIZE 7
#define GOLD_NUMBER 0.6180339
#define ALLOCATION_FACTOR 0.5
#define DEALLOCATION_FACTOR 0.25


template<class T>
class HashTable {
public:
    List<T, int> **Dyn_arr;
    int n;
    int m;

    HashTable() : n(0), m(START_SIZE) {
        Dyn_arr = new List<T, int> *[START_SIZE];
        for (int i = 0; i < START_SIZE; i++) {
            (Dyn_arr)[i] = nullptr;
        }
    }

    ~HashTable() {
        this->clear();
        this->Dyn_arr = nullptr;
    }

    int Hashing(int key, int size) const {
        double i = GOLD_NUMBER * key;
        double frac = i - floor(i);

        return (double) frac * (size);

    }


    double getAlphaVal() {
        return double(n) / m;
    }

    Node<T, int> *find(int key) const {
        List<T, int> *curr = Dyn_arr[Hashing(key, this->m)];
        if (!curr) return nullptr;
        return curr->find(key);

    }

    T *search(int key) {
        if (find(key)) {
            return find(key)->get_data();
        }
        return nullptr;

    }

    bool Exist(int key) {
        if (!find(key)) {
            return false;
        }
        return true;
    }

    StatusType insert(int key, T *data) {
        if (this->find(key)) return FAILURE;

        auto new_node = Node<T, int>(data, key);


        List<T, int> *add_to = this->Dyn_arr[this->Hashing(key, this->m)];
        if (add_to == nullptr) add_to = new List<T, int>();


        StatusType result = add_to->insert(data, key);
        Dyn_arr[this->Hashing(key, this->m)] = add_to;
        if (result == SUCCESS) {
            n++;
            if (getAlphaVal() >= ALLOCATION_FACTOR) {
                this->DoubleTable();
            }

        }
        return result;
    }


    StatusType remove(int key) {
        Node<T, int> *toRemove = this->find(key);
        if (!toRemove) return SUCCESS;
        List<T, int> *hostList = Dyn_arr[Hashing(key, this->m)];
        StatusType result = hostList->remove(key);

        if (result == SUCCESS) {
            this->n--;
            if (getAlphaVal() <= DEALLOCATION_FACTOR) {
                this->DevideTable();
            }

        }

        return result;

    }

    StatusType DoubleTable() {

        int prevTotalSize = this->m;
        int newTotalSize = this->m * 2;
        int newCurrSize = 0;

        auto newTable = new List<T, int> *[newTotalSize];
        for (int i = 0; i < newTotalSize; i++) {
            (newTable)[i] = nullptr;
        }
        for (int i = 0; i < this->m; i++) {
            List<T, int> *oldList = Dyn_arr[i];
            if (!oldList) continue;
            if (oldList->length() == 0)
                continue;

            for (auto it = oldList->begin(); it.get_ptr(); ++it) {
                List<T, int> *currList = newTable[Hashing(it.get_key(), newTotalSize)];
                if (!currList) {
                    currList = new List<T, int>;

                    //currList->list_size = 0;
                }

                StatusType result = currList->insert(*it, it.get_key());
                if (result == SUCCESS) {
                    newCurrSize++;
                } else {
                    assert(0);
                    return result;
                }
                newTable[Hashing(it.get_key(), newTotalSize)] = currList;


                if (it == oldList->end()) {
                    break;
                }

            }
//            oldList->clear();
//            delete oldList;


        }
        assert(newCurrSize == this->n);
        this->clear();
        this->Dyn_arr = newTable;
        this->m = newTotalSize;
        return SUCCESS;

    }

    StatusType DevideTable() {
        int prevTotalSize = this->m;
        int newTotalSize = this->m / 2;
        int newCurrSize = 0;

        auto newTable = new List<T, int> *[newTotalSize];
        for (int i = 0; i < newTotalSize; i++) {
            (newTable)[i] = nullptr;
        }
        for (int i = 0; i < this->m; i++) {
            List<T, int> *oldList = Dyn_arr[i];
            if (!oldList) continue;
            if (oldList->length() == 0)
                continue;


            for (auto it = oldList->begin(); it.get_ptr(); ++it) {
                List<T, int> *currList = newTable[Hashing(it.get_key(), newTotalSize)];
                if (!currList) {
                    currList = new List<T, int>;
                    //currList->list_size=0;
                }
                StatusType result = currList->insert(*it, it.get_key());
                if (result == SUCCESS) {
                    newCurrSize++;
                } else {
                    return result;
                }
                newTable[Hashing(it.get_key(), newTotalSize)] = currList;
                if (it == oldList->end()) {
                    break;
                }

            }


        }
        assert(newCurrSize == this->n);
        this->clear();
        this->Dyn_arr = newTable;
        this->m = newTotalSize;
        return SUCCESS;


    }

    StatusType clear() {
        if (this->Dyn_arr) {

            try {
                for (int i = 0; i < this->m; i++) {
                    if ((Dyn_arr)[i]) {
                        Dyn_arr[i]->clear();
                        delete Dyn_arr[i];
                        Dyn_arr[i] = nullptr;
                    }

                }
                delete[] this->Dyn_arr;
                this->Dyn_arr = nullptr;
            }

            catch (std::bad_alloc &) {
                return ALLOCATION_ERROR;
            }
        }
        return SUCCESS;
    }


};


#endif //UNTITLED6_HASHTABLEE_H
