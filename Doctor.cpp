/*
    This file includes the implementation of the functions in Doctor.h.
    @author Sıla Özel
*/
#include "Doctor.h"
#include <string>
#include <list>
using namespace std;

Doctor::Doctor(const string docName, const string docBranch, const list<int> days) {
    name = docName;
    branch = docBranch;
    surgDays = days;
}

Doctor::~Doctor() {
}

string Doctor::getBranch() {
    return branch;
}

string Doctor::getName() {
    return name;
}

list<int> Doctor::getSurgDays() {
    return surgDays;
}