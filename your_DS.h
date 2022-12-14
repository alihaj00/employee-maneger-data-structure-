

#include "elements.h"
#include "union_Find.h"
#include "EmployeeKey.h"
#include <climits>


#ifndef MEVNIWET2_YOUR_DS_H
#define MEVNIWET2_YOUR_DS_H
#define INITIAL_EMPOYEES_NUM 0


class your_DS {
public:
    HashTable<Employee> allEmployees;
    UnionFind<Company> companies;
    AVLTree<EmployeeKey, Employee> employeesBySalary;
    int numOfEmployees;
    int num_of_zero_salary;
    int numOfCompanies;
    long long int grade_of_zero;
    int K;

    explicit your_DS(int k) : K(k), numOfEmployees(INITIAL_EMPOYEES_NUM), grade_of_zero(0), numOfCompanies(k),
                              allEmployees(), companies(k), num_of_zero_salary(INITIAL_EMPOYEES_NUM) {

    };


    StatusType addEmployee(int EmployeeID, int CompanyID, int Grade) {
        if (EmployeeID <= 0 || CompanyID <= 0 || CompanyID > K || Grade < 0) return INVALID_INPUT;
        if ((allEmployees.Exist(EmployeeID))) return FAILURE;

        Employee *newEmployee;
        try {
            newEmployee = new Employee(EmployeeID, 0, CompanyID, Grade);
        }
        catch (std::bad_alloc &) {
            return ALLOCATION_ERROR;
        }
        allEmployees.insert(EmployeeID, newEmployee);//O(1)
        auto *temp = companies.Find(CompanyID);
        auto *wanted_company = temp->data; //O(log*(k))
        wanted_company->employeesNoSalary.insert(EmployeeID, newEmployee); //O(1)
        newEmployee->currCompany = wanted_company;
        this->numOfEmployees++;
        num_of_zero_salary++;
        grade_of_zero += Grade;
        wanted_company->zero_grade += Grade;
        wanted_company->employees_number++;
        wanted_company->employees_number_without_salary++;
        return SUCCESS;
    }

    StatusType removeEmployee(int EmployeeID) {
        if (EmployeeID <= 0)return INVALID_INPUT;
        if (!this->allEmployees.Exist(EmployeeID)) {
            return FAILURE;
        }

        auto toRemove = this->allEmployees.find(EmployeeID);
        if (toRemove->get_data()->salary > 0) {
            toRemove->get_data()->currCompany->employeesBySalary.removeNode(
                    EmployeeKey(EmployeeID, toRemove->get_data()->salary));
            this->employeesBySalary.removeNode(EmployeeKey(EmployeeID, toRemove->get_data()->salary));
            toRemove->get_data()->currCompany->employees_number--;

        } else {
            toRemove->get_data()->currCompany->employeesNoSalary.remove(EmployeeID);
            toRemove->get_data()->currCompany->employees_number_without_salary--;
            toRemove->get_data()->currCompany->employees_number--;
            toRemove->get_data()->currCompany->zero_grade -= toRemove->get_data()->grade;
            grade_of_zero -= toRemove->get_data()->grade;
            this->num_of_zero_salary--;
        }
        delete toRemove->get_data();
        this->allEmployees.remove(EmployeeID);
        this->numOfEmployees--;
        return SUCCESS;


    }

    StatusType acquireCompany(int AcquirerID, int TargetID, double Factor) {
        if (AcquirerID <= 0 || AcquirerID > K || TargetID <= 0 || TargetID > K ||
            TargetID == AcquirerID || Factor <= 0.0)
            return INVALID_INPUT;
        if (companies.Find(TargetID)->key == companies.Find(AcquirerID)->key) return INVALID_INPUT;

        auto *acquire_company = companies.Find(AcquirerID);
        auto *target_company = companies.Find(TargetID);
        int sizeAcquire = acquire_company->size;
        int sizeTarget = target_company->size;
        auto rootAcq = acquire_company->top;
        auto rootTarg = target_company->top;

        if (sizeAcquire >= sizeTarget) {
            rootAcq->increase += (companyValueAux(target_company->key)) * Factor;
            rootTarg->decrease += rootAcq->increase - rootAcq->decrease;
        } else {
            rootAcq->increase += (companyValueAux(target_company->key) * Factor);
            rootAcq->decrease += rootTarg->increase - rootTarg->decrease;
        }
        companies.Union(acquire_company->key, target_company->key);


        int c1, c2;
        employeeKeyCompare keyCompare;

        if (acquire_company->data->employeesBySalary.size == 0) {


            acquire_company->data->employeesBySalary = target_company->data->employeesBySalary;

            target_company->data->employeesBySalary.resetNext();
            auto *current = target_company->data->employeesBySalary.getMinInfo();
            while (current) {
                current->currCompany = acquire_company->data;
                current->companyId = acquire_company->key;
                current = target_company->data->employeesBySalary.getNextInfo();
            }


        } else if (target_company->data->employeesBySalary.size > 0 &&
                   acquire_company->data->employeesBySalary.size > 0) {


            auto *infoArr1 = acquire_company->data->employeesBySalary.getInfoInOrder(
                    acquire_company->data->employees_number, &c1);

            auto *infoArr2 = target_company->data->employeesBySalary.getInfoInOrder(
                    target_company->data->employees_number, &c2);

            auto *newInfoArr = mergeArrays<Employee *, employeeKeyCompare>(infoArr1, c1, infoArr2, c2, keyCompare);
            for (int i = 0; i < c1 + c2; i++) {


                if (newInfoArr != nullptr) {
                    newInfoArr[i]->companyId = acquire_company->data->company_id;
                    newInfoArr[i]->currCompany = acquire_company->data;
                }
            }


            EmployeeKey *keyArr;
            long long int *valueArr;

            try {
                keyArr = new EmployeeKey[acquire_company->data->employees_number +
                                         target_company->data->employees_number];
                valueArr = new long long int[acquire_company->data->employees_number +
                                             target_company->data->employees_number];
            }
            catch (std::bad_alloc &) {
                return ALLOCATION_ERROR;
            }


            for (int i = 0; i < c1 + c2; i++) {

                keyArr[i] = EmployeeKey(newInfoArr[i]->id, newInfoArr[i]->salary);
                valueArr[i] = newInfoArr[i]->grade;
                newInfoArr[i]->currCompany = acquire_company->data;
                newInfoArr[i]->companyId = acquire_company->data->company_id;
            }

            target_company->data->employeesBySalary.clear();
            //acquire_company->data->employeesBySalary.clear();
            acquire_company->data->employeesBySalary.arrayToAVL(newInfoArr, keyArr, valueArr, c1 + c2);


            delete[] infoArr1;
            delete[] infoArr2;
            delete[] newInfoArr;
            delete[] keyArr;
            delete[] valueArr;

        }

        auto *target_table = target_company->data->employeesNoSalary.Dyn_arr;

        if (target_table && target_company->data->employees_number_without_salary) {
            for (int p = 0; p < target_company->data->employeesNoSalary.m; p++) {

                if (target_table[p] && target_table[p]->first && target_table[p]->list_size > 0) {
                    auto *current = target_table[p]->first;

                    while (current) {
                        acquire_company->data->employeesNoSalary.insert(current->getKey(), current->get_data());
                        current->get_data()->currCompany = acquire_company->data;
                        current->get_data()->companyId = acquire_company->key;
                        current = current->get_next();

                    }
                }
            }

            target_company->data->employeesNoSalary.clear();

        }


        acquire_company->data->employees_number += target_company->data->employees_number;
        acquire_company->data->employees_number_without_salary += target_company->data->employees_number_without_salary;
        acquire_company->data->zero_grade += target_company->data->zero_grade;
        int temp = target_company->key;
        delete target_company->data;
        delete target_company;
        this->companies.Union_arr[temp - 1] = nullptr;


        return SUCCESS;

    }

    StatusType promoteEmployee(int EmployeeID, int BumpGrade) {
        if (EmployeeID <= 0) return INVALID_INPUT;
        if (!allEmployees.Exist(EmployeeID)) return FAILURE;
        auto *wanted_player = allEmployees.search(EmployeeID);
        if (BumpGrade > 0) {
            wanted_player->grade += BumpGrade;
            if (wanted_player->salary == 0) {
                grade_of_zero += BumpGrade;
                wanted_player->currCompany->zero_grade += BumpGrade;


            } else {
                auto *temp = wanted_player->currCompany->employeesBySalary.searchForNode(
                        EmployeeKey(EmployeeID, wanted_player->salary));
                temp->value += BumpGrade;
                wanted_player->currCompany->employeesBySalary.FixRank(temp);
                auto *temp2 = employeesBySalary.searchForNode(EmployeeKey(EmployeeID, wanted_player->salary));
                temp2->value += BumpGrade;
                employeesBySalary.FixRank(temp2);
            }
        }
        return SUCCESS;

    }

    StatusType SumOfBumpGradeBetweenTopWorkersByGroup(int companyID, int m, long long int *sumBumpGrade) {
        if (m <= 0 || companyID < 0 || companyID > K || sumBumpGrade == nullptr) return INVALID_INPUT;


        if (companyID == 0) {
            if (numOfEmployees - num_of_zero_salary < m) return FAILURE;
            auto *wanted = employeesBySalary.Select((numOfEmployees - num_of_zero_salary) - m + 1);
            auto to_return = employeesBySalary.SumRank(*wanted);
            *sumBumpGrade = to_return;

            return SUCCESS;
        }
        auto *wanted_company = companies.Find(companyID)->data;
        int no_salary = wanted_company->employees_number_without_salary;
        int allEmployeesNum = wanted_company->employees_number;
        if (allEmployeesNum - no_salary < m) return FAILURE;
        auto *wanted = wanted_company->employeesBySalary.Select((allEmployeesNum - no_salary) - m + 1);
        auto to_return = wanted_company->employeesBySalary.SumRank(*wanted);

        *sumBumpGrade = to_return;
        return SUCCESS;


    }

    StatusType
    averageBumpGradeBetweenSalaryByGroup(int CompanyID, int lowerSalary, int higherSalary, double *averageBumpGrade) {
        if (higherSalary < 0 || lowerSalary < 0 || lowerSalary > higherSalary || CompanyID < 0 || CompanyID > K ||
            averageBumpGrade == nullptr)
            return INVALID_INPUT;

        double grade_sum = 0, employee_sum = 0, avg = 0;

        if (CompanyID == 0) {

            if (lowerSalary == 0) {
                grade_sum += grade_of_zero;
                employee_sum += num_of_zero_salary;
            }
            if (employeesBySalary.size == 0 || (higherSalary == 0 && lowerSalary == 0)) {
                if (employee_sum == 0) return FAILURE;
                *averageBumpGrade = grade_sum / employee_sum;
                return SUCCESS;
            }
            if (lowerSalary > employeesBySalary.getMaxInfo()->salary) return FAILURE;
            if (higherSalary < employeesBySalary.getMinInfo()->salary) {
                if (lowerSalary == 0) {
                    if (employee_sum == 0) return FAILURE;
                    *averageBumpGrade = grade_sum / employee_sum;
                    return SUCCESS;
                } else return FAILURE;
            }

            EmployeeKey lowKey(-1, lowerSalary);
            EmployeeKey highKey(-2, higherSalary);//-2 is our inf
            auto *lower = employeesBySalary.searchForNodeUpClose(lowKey);
            auto *higher = employeesBySalary.searchForNodeDownClose(highKey);
            auto wantedLowKey = lower->key;
            auto wantedHighKey = higher->key;

            auto high_rank = (double) employeesBySalary.Rank(wantedHighKey);
            auto low_rank = (double) employeesBySalary.Rank(wantedLowKey);
            auto high_sum = (double) employeesBySalary.SumRank(wantedHighKey);
            auto low_sum = (double) employeesBySalary.SumRank(wantedLowKey);

            if (low_rank == high_rank) {
                grade_sum += lower->value;
                employee_sum++;
                //if (employee_sum == 0) return FAILURE;
                *averageBumpGrade = grade_sum / employee_sum;
                return SUCCESS;

            }

            grade_sum += low_sum - high_sum + higher->value;
            employee_sum += high_rank - low_rank + 1;
            *averageBumpGrade = grade_sum / employee_sum;
            return SUCCESS;


        } else {


            auto *wantedCompany = companies.Find(CompanyID)->data;
            if (lowerSalary == 0) {
                grade_sum += wantedCompany->zero_grade;
                employee_sum += wantedCompany->employees_number_without_salary;
            }
            if (wantedCompany->employeesBySalary.size == 0 || (higherSalary == 0 && lowerSalary == 0)) {
                if (employee_sum == 0) return FAILURE;
                *averageBumpGrade = grade_sum / employee_sum;
                return SUCCESS;
            }
            if (lowerSalary > wantedCompany->employeesBySalary.getMaxInfo()->salary) return FAILURE;
            if (higherSalary < wantedCompany->employeesBySalary.getMinInfo()->salary) {
                if (lowerSalary == 0) {
                    if (employee_sum == 0) return FAILURE;
                    *averageBumpGrade = grade_sum / employee_sum;
                    return SUCCESS;
                } else return FAILURE;
            }

            EmployeeKey lowKey(-1, lowerSalary);
            EmployeeKey highKey(-2, higherSalary);//-2 is our inf
            auto *lower = wantedCompany->employeesBySalary.searchForNodeUpClose(lowKey);
            auto *higher = wantedCompany->employeesBySalary.searchForNodeDownClose(highKey);
            auto wantedLowKey = lower->key;
            auto wantedHighKey = higher->key;

            auto high_rank = (double) wantedCompany->employeesBySalary.Rank(wantedHighKey);
            auto low_rank = (double) wantedCompany->employeesBySalary.Rank(wantedLowKey);
            auto high_sum = (double) wantedCompany->employeesBySalary.SumRank(wantedHighKey);
            auto low_sum = (double) wantedCompany->employeesBySalary.SumRank(wantedLowKey);

            if (low_rank == high_rank) {
                grade_sum += lower->value;
                employee_sum++;
                *averageBumpGrade = grade_sum / employee_sum;
                return SUCCESS;

            }
            grade_sum += low_sum - high_sum + higher->value;
            employee_sum += high_rank - low_rank + 1;
            *averageBumpGrade = grade_sum / employee_sum;
            return SUCCESS;
        }


    }


    StatusType EmployeeSalaryIncrease(int employeeID, int salaryIncrease) {
        if (salaryIncrease <= 0 || employeeID <= 0) return INVALID_INPUT;
        if (!allEmployees.Exist(employeeID)) return FAILURE;
        auto *wantedEmployee = allEmployees.search(employeeID);
        EmployeeKey oldKey(employeeID, wantedEmployee->salary);
        wantedEmployee->salary += salaryIncrease;
        EmployeeKey newKey(employeeID, wantedEmployee->salary);
        if (wantedEmployee->salary - salaryIncrease == 0) {
            wantedEmployee->currCompany->employeesNoSalary.remove(employeeID);
            wantedEmployee->currCompany->employeesBySalary.insertNode(newKey, wantedEmployee, wantedEmployee->grade);
            employeesBySalary.insertNode(newKey, wantedEmployee, wantedEmployee->grade);
            num_of_zero_salary--;
            wantedEmployee->currCompany->employees_number_without_salary--;
            wantedEmployee->currCompany->zero_grade -= wantedEmployee->grade;
            grade_of_zero -= wantedEmployee->grade;

            return SUCCESS;
        } else {
            wantedEmployee->currCompany->employeesBySalary.removeNode(oldKey);
            wantedEmployee->currCompany->employeesBySalary.insertNode(newKey, wantedEmployee, wantedEmployee->grade);
            employeesBySalary.removeNode(oldKey);
            employeesBySalary.insertNode(newKey, wantedEmployee, wantedEmployee->grade);
            return SUCCESS;
        }


    }

    StatusType companyValue(int CompanyID, double *standing) {
        if (CompanyID <= 0 || CompanyID > this->K || !standing) {
            return INVALID_INPUT;
        }
        double temp = companyValueAux(CompanyID);


        *standing = temp;

        return SUCCESS;


    }


    double companyValueAux(int CompanyID) const {
        auto temp = this->companies.Find_arr[CompanyID - 1];
        double value = temp->baseValue;

        while (temp) {
            value += (temp->increase - temp->decrease);
            temp = temp->father;
        }
        return value;

    }


    void Quit() {

        for (int i = 0; i < this->allEmployees.m; i++) {
            if (allEmployees.Dyn_arr[i]) {
                for (auto it = allEmployees.Dyn_arr[i]->begin(); it.get_ptr(); ++it) {
                    if (it.get_data()) {
                        delete it.get_data();
                    }
                    it.get_ptr()->data = nullptr;

                }


            }
        }

        this->allEmployees.clear();

        for (int j = 0; j < this->numOfCompanies; j++) {
            if (this->companies.Union_arr[j]) {
                auto currCompany = this->companies.Union_arr[j]->data;
                if (currCompany) {

                    if (currCompany->employeesBySalary.root) {
                        currCompany->employeesBySalary.clear();
                        currCompany->employeesNoSalary.clear();
                    }


                    delete currCompany;
                    this->companies.Union_arr[j]->data = nullptr;


                }
                delete this->companies.Union_arr[j];


                this->companies.Union_arr[j] = nullptr;


            }
            delete this->companies.Find_arr[j];
            this->companies.Find_arr[j] = nullptr;
        }
        delete[] this->companies.Union_arr;
        delete[] this->companies.Find_arr;
        this->employeesBySalary.clear();


    }


};


#endif //MEVNIWET2_YOUR_DS_H
