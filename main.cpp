/*
    This file only includes a main function. It creates a menu for the user and does operations according to the input.
    @author Sıla Özel
*/
#include "SurgerySchedule.h"
#include <iostream>
#include <string>
#include <list>
using namespace std;

int main() {
    SurgerySchedule s("DocFile.txt", 12, 0);
    string answer;
    string menu = "Which operation do you want to do?\n1. Add a doctor to the system\n2. Remove a doctor from the system\n3. Add a surgery appointment\n";
    menu += "4. Pop a surgery appointment from the system\n5. Show surgeries according to the operating room number\n6. Show surgeries according to the day\n";
    menu += "7. Show all doctors\n8. Show all of the postponed patients";
    int op;
    do {
        cout << menu << endl;
        cin >> op;
        if(op == 1) {
            s.addDoc();
        }
        else if(op == 2) {
            s.removeDoc();
        }
        else if(op == 3) {
            s.addSurgeryAppointment();
        }
        else if(op == 4) {
            s.popSurgeryAppointment();
        }
        else if(op == 5) {
            s.showSurgeriesForRoom();
        }
        else if(op == 6) {
            s.showSurgeriesForDay();
        }
        else if(op == 7) {
            s.showDocs();
        }
        else {
            s.showPostponedPats();
        }
        cout << "Do you want to continue? (Y/y for yes, N/n for no): ";
        cin >> answer;
    } while(answer == "y" || answer == "Y");
}