# SurgeryScheduler
A mini menu-based project that I implemented in C++ to remember the concepts that were covered in CS201 course.
## User Guide
- The system provides a menu for choices and prompts the user for inputs.
- DocFile.txt includes the information about the doctors.
- Days are represented as numbers (1 for Monday and 7 for Sunday).
## Information About the Files
#### DocFile.txt
- This file includes the information about the doctors.
- Each information about a doctor is seperated by a comma (',') character and each doctor's information must start in a new line.
- First information must be the name of the doctor, second information must be the doctor's branch/proficiency, and last information must be the days that the doctor performs surgeries.
- Days must be integers from 1 to 7 and they must be seperated by a '/' character.
#### main.cpp
- This file includes the main function and it constructs a SurgerySchedule object and then provides a menu of choices for the user. The user can choose the operation by entering the operation number. According to the operation, additional inputs might be necessary, in that case system prints prompts for the user.
