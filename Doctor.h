/*
    This class represents a doctor. A doctor has a name, a branch/proficiency, and surgery days.
    @author Sıla Özel
*/
#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <list>
using namespace std;

class Doctor {
    private:
        string name;
        string branch;
        list<int> surgDays;
    public:
        Doctor(const string docName, const string docBranch, const list<int> days);
        ~Doctor();
        string getName();
        string getBranch();
        list<int> getSurgDays();
};

#endif