//
// Created by aliha on 4/20/2022.
//

#ifndef MAIN_CPP_EMPLOYEE_H
#define MAIN_CPP_EMPLOYEE_H

#include "RankTree.h"
#include "hashTablee.h"
#include "EmployeeKey.h"


#include <stdlib.h>
#include <memory>

class Company;

class Employee {
public:
    int id;
    long long int salary;
    long long int grade;
    int companyId;
    Company *currCompany;


    Employee(int id, long long initSalary, int companyId, long long int grade,
             Company *currCompany) : id(id), companyId(companyId),
                                     salary(initSalary), grade(grade), currCompany(currCompany) {


    }

    Employee(const Employee &other) {
        this->id = other.id;
        this->salary = other.salary;
        this->grade = other.grade;
        this->companyId = other.companyId;
        this->currCompany = other.currCompany;

    }

    Employee(int Employeeid, long long initSalary, int companyId, long long int EmployeeGrade) : id(Employeeid),
                                                                                                 companyId(companyId),
                                                                                                 salary(initSalary),
                                                                                                 grade(EmployeeGrade),
                                                                                                 currCompany(nullptr) {


    }

    Employee() : id(-1), salary(0), grade(0), companyId(0), currCompany(nullptr) {
    }


    Employee *clone() {
        return new Employee(*this);
    }


    ~Employee() { currCompany = nullptr; };


};


class Company {
public:
    int company_id;
    double value;
    long long int zero_grade;
    int employees_number;
    int employees_number_without_salary;
    HashTable<Employee> employeesNoSalary;
    AVLTree<EmployeeKey, Employee> employeesBySalary;


    Company(int id, double value1) : company_id(id), value(value1), zero_grade(0), employees_number(0),
                                     employees_number_without_salary(0), employeesBySalary(), employeesNoSalary() {}

    ~Company() = default;


};

class intcompare {
public:
    bool operator()(long long int x1, long long int x2) {
        if (x1 > x2)return true;
        return false;
    }
};

class employeeIntCompare {
public:
    bool operator()(const Employee *first,
                    const Employee *second) {
        if (first->id > second->id) {
            return true;
        }
        return false;

    }

};


class employeeKeyCompare {
public:

    bool operator()(const Employee *first,
                    const Employee *second) {

        EmployeeKey firstKey(first->id, first->salary);
        EmployeeKey secondKey(second->id, second->salary);
        return firstKey > secondKey;


    }
};


template<class elements, class Compare>
elements *mergeArrays(elements *arr1, int firstLength, elements *arr2, int secondLength, Compare compare) {
    elements *arr3;
    try {
        arr3 = new elements[firstLength + secondLength];
    }
    catch (std::bad_alloc &) {

    }

    if (firstLength == 0 && secondLength == 0) {
        delete[] arr3;
        return nullptr;
    } else if (firstLength == 0) {
        for (int j = 0, l = 0; j < secondLength && l < secondLength;) {
            arr3[l] = arr2[j];
            l++, j++;
        }
    } else if (secondLength == 0) {
        for (int i = 0, l = 0; i < firstLength && l < firstLength;) {
            arr3[l] = arr1[i];
            l++, i++;
        }
    } else {
        for (int i = 0, j = 0, l = 0; i < firstLength && j < secondLength && l < firstLength + secondLength;) {
            if (compare(arr1[i], arr2[j])) {
                arr3[l] = arr2[j];
                l++;
                j++;
            } else {
                arr3[l] = arr1[i];
                l++;
                i++;
            }


            if (i == firstLength && l < firstLength + secondLength) {
                while (j < secondLength) {
                    arr3[l] = arr2[j];
                    l++;
                    j++;
                }
            } else if (j == secondLength && l < firstLength + secondLength) {
                while (i < firstLength) {
                    arr3[l] = arr1[i];
                    i++;
                    l++;
                }
            }
        }
    }
    return arr3;

}


#endif //MAIN_CPP_EMPLOYEE_H
