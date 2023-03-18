/*
    This file includes the prototypes for the functions and the data required to create a schedule for each operating room.
    This system holds all doctors in a vector called docs and the map docIds gives each doctor a changeable id number.
    The branches map maps each branch to an operating room. opRooms is the vector that holds every surgery appointment.
    Postponed patients are hold in the postponedPats.
    @author Sıla Ozel
*/
#ifndef SURGERYSCHEDULE_H
#define SURGERYSCHEDULE_H
#include <string>
#include "Doctor.h"
#include "Patient.h"
#include <vector>
#include <list>
#include <map>
using namespace std;

class SurgerySchedule {
    private:
        vector<Doctor> docs;
        map<string, int> docIds;
        map<string, int> branches;
        vector<vector<int>> hoursLeft; //first dim -> size of branches second dim -> 7 days, Holds how much time left for each day's shift
        vector<vector<list<Patient>>> opRooms; //first dim -> num of branches, sec dim -> num of days(7), third dim -> changeable
        vector<list<Patient>> postponedPats; //vector length must be the number of operating rooms
        int tod;
    public:
        SurgerySchedule(const string docFileName, const int shiftHours, const int today);
        ~SurgerySchedule();
        bool addDoc();
        bool removeDoc();
        void addSurgeryAppointment();
        void popSurgeryAppointment();
        void showSurgeriesForRoom();
        void showSurgeriesForDay();
        void showDocs();
        void showPostponedPats();
        void removeFromPostponed();
};
#endif