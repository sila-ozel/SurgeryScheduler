/*
    This file includes the implementation of the functions in Patient.h
    @author Sıla Özel
*/
#include "Patient.h"
#include <string>
using namespace std;

Patient::Patient(const string patName, const int patUrgency, const int timeEst, const string docsName) {
    name = patName;
    urgency = patUrgency;
    time = timeEst;
    docName = docsName;
}

Patient::~Patient() {
}

string Patient::getDocName() const {
    return docName;
}

string Patient::getName() const {
    return name;
}

int Patient::getUrgency() const {
    return urgency;
}

int Patient::getTimeEst() const {
    return time;
}

bool Patient::operator==(const Patient& p) {
    return (this->getName() == p.getName()) && (this->getDocName() == p.getDocName());
}