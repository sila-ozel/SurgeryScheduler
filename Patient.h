/*
    This class represents a patient. Each patient has a name, an urgency level from 1 to 6, an estimated time for their surgery,
    and the name of the doctor that will do the surgery.
    @author Sıla Özel
*/
#ifndef PATIENT_H
#define PATIENT_H
#include <string>
using namespace std;

class Patient {
    private:
        string name;
        int urgency;
        int time;
        string docName;
    public:
        Patient(const string patName, const int patUrgency, const int timeEst, const string docsName);
        ~Patient();
        string getName() const;
        int getUrgency() const;
        int getTimeEst() const;
        string getDocName() const;
        bool operator==(const Patient& p);
};
#endif