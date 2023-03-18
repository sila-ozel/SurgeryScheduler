/*
    This file includes all functions related with the system. These functions will be called to do necessary operations in the
    main function. Days are represented as integers in all files, 1 for Monday and 7 for Sunday.
    @author Sıla Özel
*/
#include "SurgerySchedule.h"
#include <string>
#include "Doctor.h"
#include "Patient.h"
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

/*
    This constructor takes 3 arguments: a txt file that includes doctors' information, shift hours, and today as an integer.
    In txt file each line has information about a doctor and it starts with the doctor's name, then their proficiency, and
    finally the days that they do surgeries. For surgery days again integers are used to represent the days and the days are
    seperated by a '/' character. Also, each information is seperated by a comma ',' character.
*/
SurgerySchedule::SurgerySchedule(const string docFileName, const int shiftHours, const int today) {
    fstream file;
    file.open(docFileName.c_str(), ios_base::in);
    if(file.is_open()) {
        string line;
        int k = 0;
        while(getline(file, line)) {
            stringstream s(line);
            string docName, branchName, days;
            getline(s, docName, ',');
            getline(s, branchName, ',');
            getline(s, days);
            string day;
            list<int> dayList;
            stringstream s2(days);
            while(getline(s2, day, '/')) {
                dayList.push_back(stoi(day));
            }
            Doctor d(docName, branchName, dayList);
            docs.push_back(d);
            if(branches.count(branchName)==0) {
                branches.insert({branchName, k});
                k++;
            }
        }
        file.close();
        for(int i = 0; i < (int) (docs.size()); i++) {
            docIds.insert({docs[i].getName(), i});
        }
        for(int i = 0; i < (int) (branches.size()); i++) {
            vector<list<Patient>> v;
            opRooms.push_back(v);
            vector<int> v2;
            hoursLeft.push_back(v2);
            for(int j = 0; j < 7; j++) {
                list<Patient> p;
                opRooms[i].push_back(p);
                hoursLeft[i].push_back(shiftHours);
            }
            list<Patient> p2;
            postponedPats.push_back(p2);
        }
        tod = today;
    }
}

SurgerySchedule::~SurgerySchedule() {
}

void SurgerySchedule::addSurgeryAppointment() {
    string name;
    cout << "Enter doctor's name: ";
    cin.clear();
    cin.sync();
    getline(cin, name);
    if(docIds.count(name) > 0) {
        Doctor d = docs[docIds[name]];
        int roomNo = branches[d.getBranch()];
        string patName;
        int urgency, estTime;
        cout << "Patient's name: ";
        getline(cin, patName);
        cout << "Enter urgency level from 1-6: ";
        cin >> urgency;
        cout << "Enter the estimated time for the operation: ";
        cin >> estTime;
        Patient p(patName,urgency,estTime,name);
        bool done = false;
        list<int> days = d.getSurgDays();
        list<int>::iterator dayIter = days.begin(); //we will add the patient according to the doctor's surgery days
        int i = 0;

        //the patient can be added to the days that are greater than or equal to today
        while(dayIter != days.end() && (*dayIter) < tod) {
            advance(dayIter, 1);
            i++; 
        }
        while(dayIter != days.end() && !done) {
            if(opRooms[roomNo][((*dayIter)-1)].empty()) {
                opRooms[roomNo][((*dayIter)-1)].push_back(p);
                hoursLeft[roomNo][((*dayIter)-1)] -= estTime;
                done = true;
            }
            else {
                list<Patient>::iterator pat = opRooms[roomNo][(*dayIter)-1].begin();
                int sum = 0;
                while(pat != opRooms[roomNo][(*dayIter)-1].end() && pat->getUrgency() < urgency) {
                    sum += pat->getTimeEst();
                    advance(pat,1);
                } 
                if(hoursLeft[roomNo][(*dayIter)-1] - estTime + sum >= 0) {
                    opRooms[roomNo][(*dayIter)-1].insert(pat,p);
                    hoursLeft[roomNo][(*dayIter)-1] -= estTime;
                    done = true;
                }
                else {
                    advance(dayIter, 1);
                    i++;
                }
            }
        }
        if(!done) {
            postponedPats[roomNo].push_back(p);
            return;
        }

        //in this while loop go through the 7 days and re-schedule accordingly
        int day = 0;
        while(day < 7) {
            if(hoursLeft[roomNo][day] >= 0) {
                day++;
            }
            else {
                list<Patient>::iterator patIter = opRooms[roomNo][day].begin();
                
                int sum = 0;
                while(patIter != opRooms[roomNo][day].end() && sum + patIter->getTimeEst() < abs(hoursLeft[roomNo][day])) {
                    sum += patIter->getTimeEst();
                    advance(patIter, 1);
                }
                
                days = docs[docIds[patIter->getDocName()]].getSurgDays();
                list<int>::iterator temp = days.begin();
                while(temp != days.end() && *temp <= day+1) {
                    advance(temp,1);
                }
                
                Patient pat = *patIter;
                opRooms[roomNo][day].remove(*patIter);
                hoursLeft[roomNo][day] += pat.getTimeEst();
                bool inserted = false;
                while(temp != days.end() && !inserted) {
                    if(opRooms[roomNo][(*temp)-1].empty()) {
                        opRooms[roomNo][(*temp)-1].push_back(pat);
                        hoursLeft[roomNo][(*temp)-1] -= pat.getTimeEst();
                        inserted = true;
                    }
                    else {    
                        list<Patient>::iterator pIter = opRooms[roomNo][(*temp)-1].begin();
                        int s = 0;
                        while(pIter != opRooms[roomNo][(*temp)-1].end() && pIter->getUrgency() <= pat.getUrgency()) {
                            s += pIter->getTimeEst();
                            advance(pIter,1);
                        }
                        if(hoursLeft[roomNo][(*temp)-1] - pat.getTimeEst() + s >= 0) {  // check this condition
                            opRooms[roomNo][(*temp)-1].insert(pIter, pat);
                            hoursLeft[roomNo][(*temp)-1] -= pat.getTimeEst();
                            inserted = true;
                        }
                        else {
                            advance(temp, 1);
                        }
                    }
                }
                if(!inserted) {
                    postponedPats[roomNo].push_back(pat);
                }
            }
        }
    }
    else {
        cout << "Doctor " << name << " does not exist." << endl;
    }
}

// assume that they cannot add a new branch with this
bool SurgerySchedule::addDoc() {
    list<int> days;
    string name, branch;
    cout << "Enter the name of the doctor: ";
    cin.clear();
    cin.sync();
    getline(cin, name);
    if(docIds.count(name) > 0) {
        cout << "Doctor " << name << " already exists" << endl;
        return false;
    }
    cout << "Enter the proficiency of the doctor: ";
    getline(cin, branch);
    int day;
    cout << "Enter the surgery days as numbers (1 for Monday and 7 Sunday, -1 to stop): ";
    do{
        cin >> day;
        if(day > 0) {
            days.push_back(day);
        }
    } while(day > 0);
    Doctor d(name, branch, days);
    docs.push_back(d);
    int id = (int) (docs.size()) - 1;
    docIds.insert({name, id});
    return true;
}

bool SurgerySchedule::removeDoc() {
    string name;
    cout << "Enter the name of the doctor to be removed: ";
    cin.clear();
    cin.sync();
    getline(cin, name);
    if(docIds.count(name)>0) {
        int pos = docIds[name];
        docs.erase(docs.begin()+pos);
        docIds.erase(name);
        for(; pos < (int)docs.size(); pos++) {
            docIds[docs[pos].getName()] = pos;
        }
        return true;
    }
    cout << "Doctor " << name << " does not exist" << endl;
    return false;
}

// if there is no surgery that day then it won't pop and if all surgeries are done for that day then they are done for the day
void SurgerySchedule::popSurgeryAppointment() {
    string docname;
    cout << "Enter the name of the doctor that wants to pop an appointment: ";
    cin.clear();
    cin.sync();
    getline(cin, docname);
    string branch = docs[docIds[docname]].getBranch();
    int roomNo = branches[branch];
    if(!opRooms[roomNo][tod].empty()) {
        opRooms[roomNo][tod].pop_back();
    }
    bool done = true;
    for(int i = 0; i < (int) (branches.size()); i++) {
        if(!opRooms[i][tod].empty()) {
            done = false;
        }
    }
    if(done) {
        tod++;
    }
}

void SurgerySchedule::showSurgeriesForRoom() {
    int roomNo;
    cout << "Enter the operating room number: ";
    cin >> roomNo;
    if(roomNo > (int) (branches.size())) {
        cout << "Operating Room " << roomNo << " does not exist." << endl;
        return;
    }
    cout << "Surgeries for room number " << roomNo << ":" << endl;
    for(int i = 0; i < 7; i++) {
        list<Patient>::reverse_iterator pat = opRooms[roomNo-1][i].rbegin();
        cout << "Day " << i+1 << ": " << endl;
        if(opRooms[roomNo-1][i].empty()) {
            cout << "No surgeries for today." << endl;
        }
        else {
            int k = 1;
            while(pat != opRooms[roomNo-1][i].rend()) {
                cout << "Patient " << k << ": ";
                cout << "Name: " << pat->getName() << " Doctor's Name: " << pat->getDocName() << endl;
                k++;
                pat++;
            }
        }
    }
}

void SurgerySchedule::showSurgeriesForDay() {
    int day;
    cout << "Enter the day for which you want to see the surgeries (1 for Monday and 7 for Sunday): ";
    cin >> day;
    cout << "Surgeries for day " << day << ":" << endl;
    for(int i = 0; i < (int) (branches.size()); i++) {
        list<Patient>::reverse_iterator pat = opRooms[i][day-1].rbegin();
        cout << "Room " << i+1 << ":" << endl;
        if(opRooms[i][day].empty()) {
            cout << "No surgeries for room " << i+1 << " today." << endl;
        }
        else{
            int k = 1;
            while(pat != opRooms[i][day-1].rend()) {
                cout << "Patient " << k << ": ";
                cout << "Name: " << pat->getName() << " Doctor's Name: " << pat->getDocName() << endl;
                k++;
                advance(pat, 1);
            }
        }
    }
}

void SurgerySchedule::showDocs() {
    for(int i = 0; i < (int) (docs.size()); i++) {
        cout << "Doctor " << i+1 << ":" << endl;
        cout << "Name: " << docs[i].getName() << " Branch: " << docs[i].getBranch() << endl;
    }
}

void SurgerySchedule::showPostponedPats() {
    for(int i = 0; i < (int) (branches.size()); i++) {
        list<Patient>::iterator iter = postponedPats[i].begin();
        if(iter == postponedPats[i].end()) {
            cout << "There is no postponed patient for operation room " << i+1 << endl;
            continue;
        }
        cout << "Postponed patients for room " << i+1 << endl;
        int k = 1;
        while(iter != postponedPats[i].end()) {
            cout << "Patient " << k << endl;
            cout << "Name: " << iter->getName() << " Doctor's Name: " << iter->getDocName() << endl;
            k++;
            iter++;
        }
    }
}

void SurgerySchedule::removeFromPostponed() {
    string docName;
    cout << "Enter the name of the doctor: ";
    cin.clear();
    cin.sync();
    getline(cin, docName);
    string br = docs[docIds[docName]].getBranch();
    int roomno = branches[br];
    string patName;
    cout << "Enter the name of the patient: ";
    getline(cin, patName);
    list<Patient>::iterator iter = postponedPats[roomno].begin();
    while(iter != postponedPats[roomno].end()) {
        if(iter->getName() == patName) {
            break;
        }
        iter++;
    }
    if(iter == postponedPats[roomno].end()) {
        cout << "There is no patient with name " << patName << endl;
    }
    postponedPats[roomno].erase(iter);
}