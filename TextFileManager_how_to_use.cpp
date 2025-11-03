// WARNING: DO NOT RUN THIS CODE, THIS IS JUST NOTES FOR REFERENCE

// First step: include the neccessary directives & hpp file
#include <sstream>
#include "TextFileManager.hpp"
// <string> and <iostream> already included in TextFileManager.hpp
// using namespace std; is also already in TextFileManager.hpp

// ====================================================================

// How to use readLinesFromFile
// Remember: readLinesFromFile returns a pointer of the dynamic array, so must release/delete after use
// when reading the raw strings and inserting them into your own constructors:
int size = 0;                                                             // this will the number of rows/records in your container later,
string *lines = TextFileManager::readLinesFromFile("yourfile.txt", size); // the size will be updated based on how many lines it read in the text file
if (lines != nullptr)
{
    for (int i = 0; i < size; ++i)
    {
        // for example patient has ID, name, condition
        string line = lines[i];      // first, access the string from the returned array
        stringstream ss(line);       // e.g. "ID001,name,XXX"
        string id, name, condition;  // initialize the variables
        getline(ss, id, ',');        // cursor starting from the start of the string until a comma is encountered and assign the string to the variable id
        getline(ss, name, ',');      // cursor continues from where it stopped, until the next comma and assign the string to the variable name
        getline(ss, condition, ','); // same thing
        // here, write your own custom "addToQueue()" or "addToStack()" function with your own cnstructor or whatever
        addPatientToMyQueue(Patient(id, name, condition));
    }

    delete[] lines; // after done loading all data from text file to your container, delete the array
}

// ====================================================================

// How to use writeLinesToFile
// pass the text file name, the array of strings you have (after formatting), and the size/number of rows or records currently in your container
int size = 3; // assuming this is the number of records you have, you should have a field/attribute in your container class to store the size number
// then do whatever process to format each record to raw strings to be stored in the text file
// you can do a loop, then create a dynamic array to temporary store the strings for example:
string *lines = new string[size];
for (int i = 0; i < size; i++)
{
    // suppose you have an array or linked list containing items, either access by index or use while loop
    // just an example (assuming "patients" is the variable for your list):
    Patient p = this->patients[i];
    string formattedLine = p.getId() + "," + p.getName() + "," + p.getConditionType(); // e.g. ID001,NAME,TERMINALLY_ILL
    lines[i] = formattedLine;
}
TextFileManager::writeLinesToFile("yourfile.txt", lines, size);

if (size > 0)
{
    delete[] lines; // after done writing all data to text file, delete the array
}
