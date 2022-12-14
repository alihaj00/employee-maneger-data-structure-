//
// Created by Mahmu on 08/06/2022.
//

#ifndef MEVNIWET2_EMPLOYEEKEY_H
#define MEVNIWET2_EMPLOYEEKEY_H

class EmployeeKey{
public:
    int id;
    long long salary;
    EmployeeKey(int id,long long  salary):id(id),salary(salary){}
    EmployeeKey():id(-1),salary(0){}


    bool operator >(const EmployeeKey& other)const{
        if (this->salary>other.salary)return true;

        else if(this->salary<other.salary) return false;
        else{
            if (this->id==-2) return true;
            if (other.id==-2) return false;

            if (this->id>other.id) return true;

            else if(this->id<other.id) return false;

            else return false;
        }


    }
    bool operator <(const EmployeeKey& other)const{
        if (this->salary>other.salary)return false;

        else if(this->salary<other.salary) return true;
        else{
            if (this->id==-2) return false;

            if (other.id==-2) return  true;

            if (this->id < other.id) return true;

            else if(this->id > other.id) return false;

            return false;
        }


    }



    bool operator==(const EmployeeKey& other)const{
        if ( this->id==other.id)
            return true;
        else return false;
    }


    bool operator<=(const EmployeeKey& other)const{
        if(*this < other || *this==other){
            return true;
        }
        return false;

    }


    bool operator >=(const EmployeeKey& other)const{
        if(*this > other || *this==other){
            return true;
        }
        return false;

    }



};


#endif //MEVNIWET2_EMPLOYEEKEY_H
