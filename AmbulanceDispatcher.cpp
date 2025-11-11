#include "AmbulanceDispatcher.hpp"

#include <iostream>

const string AmbulanceDispatcher::FILENAME = "ambulance.txt";
const char AmbulanceDispatcher::DELIMITER = '|';

int AmbulanceDispatcher::lastId = 0; // initialize the latest id

AmbulanceDispatcher::AmbulanceDispatcher()
{
    count = 0;
    front = -1; // set as "not found"/"none"
    rear = -1;  // set as "not found"/"none
    loadAmbulancesFromFile();
}

AmbulanceDispatcher::~AmbulanceDispatcher()
{
    saveAmbulancesToFile();
}

bool AmbulanceDispatcher::isFull()
{
    return (count == MAX_AMBULANCES);
}

bool AmbulanceDispatcher::isEmpty()
{
    return (count == 0);
}

int AmbulanceDispatcher::generateAmbulanceId()
{
    return ++lastId; // pre-increment to make sure it returns the updated value
}

void AmbulanceDispatcher::registerAmbulance(Ambulance ambulance, bool saveToFile)
{
    if (isFull())
    {
        cout << "Ambulance Dispatcher is full. Cannot register more ambulances." << endl;
        return;
    }

    if (isEmpty())
    {
        // first ambulance in the queue
        front = 0;
        rear = 0;
    }
    else
    {
        rear = (rear + 1) % MAX_AMBULANCES; // compute the new rear
    }

    ambulances[rear] = ambulance; // add the new ambulance in the right place
    count++;

    // update last id
    if (ambulance.id > lastId)
    {
        lastId = ambulance.id;
    }

    if (saveToFile)
    {
        saveAmbulancesToFile();
    }
}

void AmbulanceDispatcher::rotateShift()
{
    if (isEmpty())
    {
        cout << "Ambulance Dispatcher is empty. Cannot rotate shift." << endl;
        return;
    }
    if (count == 1)
    {
        cout << "Ambulance Dispatcher has only one ambulance. Cannot rotate shift." << endl;
        return; // no need to rotate, one ambulance only
    }

    // save the front ambulance
    Ambulance temp = ambulances[front];
    front = (front + 1) % MAX_AMBULANCES; // new front
    rear = (rear + 1) % MAX_AMBULANCES;   // new rear
    ambulances[rear] = temp;              // reposition the ambulance

    saveAmbulancesToFile();
    cout << "Shift rotated successfully!" << endl;
}

void AmbulanceDispatcher::displaySchedule()
{
    if (isEmpty())
    {
        cout << "Ambulance Dispatcher is empty. No schedule to display." << endl;
        return;
    }

    cout << "Ambulance Schedule:" << endl;
    int index = front;
    for (int i = 0; i < count; i++)
    {
        cout << "Shift Position " << (i + 1) << ":\n";
        cout << "Ambulance ID: " << ambulances[index].id << endl;
        cout << "Plate Number: " << ambulances[index].plateNumber << endl;
        cout << "Driver Name: " << ambulances[index].driverName << endl;
        cout << "------------------------" << endl;
        index = (index + 1) % MAX_AMBULANCES;
    }

    cout << "Total Ambulances: " << count << endl;
}

void AmbulanceDispatcher::loadAmbulancesFromFile()
{
    int fileLineCount = 0;
    string *rawLines = TextFileManager::readLinesFromFile(FILENAME, fileLineCount);

    if (rawLines == nullptr || fileLineCount == 0)
    {
        cout << "No ambulances in the text file.\n";
        return;
    }

    for (int i = 0; i < fileLineCount; ++i)
    {
        string line = rawLines[i];
        string string_id, plateNumber, driverName;
        stringstream ss(line);

        // get values from raw data/line
        getline(ss, string_id, DELIMITER);
        getline(ss, plateNumber, DELIMITER);
        getline(ss, driverName, DELIMITER);

        // create the ambulance
        Ambulance ambulance = {stoi(string_id), plateNumber, driverName};

        // insert the ambulance into the queue
        registerAmbulance(ambulance, false);
    }

    delete[] rawLines;
}

void AmbulanceDispatcher::saveAmbulancesToFile()
{
    if (!isEmpty())
    {
        string *formattedLines = new string[count];
        int index = front; // start writing from the first in queue, not in array

        for (int i = 0; i < count; i++)
        {
            Ambulance ambulance = ambulances[index];
            formattedLines[i] = to_string(ambulance.id) + DELIMITER + ambulance.plateNumber + DELIMITER + ambulance.driverName;
            index = (index + 1) % MAX_AMBULANCES;
        }

        TextFileManager::writeLinesToFile(FILENAME, formattedLines, count);
        delete[] formattedLines;
    }
}

Ambulance AmbulanceDispatcher::createNewAmbulance()
{
    int id;
    string plateNumber, driverName;

    id = generateAmbulanceId();

    bool plateNumComplete = false;
    bool driverNameComplete = false;

    // prompt plate number
    while (!plateNumComplete)
    {
        cout << "Enter Plate Number: ";
        getline(cin, plateNumber);

        // check empty
        if (plateNumber.empty())
        {
            cout << "Plate number cannot be empty." << endl;
            continue;
        }

        bool hasInvalidChar = false;
        bool hasLetter = false;
        bool hasDigit = false;

        for (char c : plateNumber)
        {
            if (!isalnum(static_cast<unsigned char>(c))) // make sure is alphanumeric and no negative char
            {
                hasInvalidChar = true;
                break;
            }
            if (isalpha(static_cast<unsigned char>(c))) // make sure has letter
            {
                hasLetter = true;
            }
            if (isdigit(static_cast<unsigned char>(c))) // make sure has digit
            {
                hasDigit = true;
            }
        }

        if (hasInvalidChar)
        {
            cout << "Plate number can only contain letters and digits. Please try again.\n";
            continue;
        }

        if (!hasLetter || !hasDigit)
        {
            cout << "Plate number must contain BOTH letters and digits. Please try again.\n";
            continue;
        }

        // convert all characters to uppercase
        for (char &c : plateNumber)
        {
            c = toupper(static_cast<unsigned char>(c));
        }

        plateNumComplete = true;
    }

    // prompt driver name
    while (!driverNameComplete)
    {
        cout << "Enter Driver Name: ";
        getline(cin, driverName);

        // check empty
        if (driverName.empty())
        {
            cout << "Driver name cannot be empty." << endl;
            continue;
        }

        bool valid = true;
        for (char c : driverName)
        {
            if (!isalpha(static_cast<unsigned char>(c)) && c != ' ')
            {
                valid = false;
                break;
            }
        }

        if (!valid)
        {
            cout << "Driver name can only contain letters and spaces. Please try again.\n";
            continue;
        }

        bool newWord = true; // true = start of new word

        // capitalize the first letter of each word
        for (char &c : driverName)
        {
            if (isspace(c))
            {
                newWord = true; // next non-space character will be the first character of new word
            }
            else if (newWord)
            {
                c = toupper(c);
                newWord = false; // reset
            }
            else
            {
                c = tolower(static_cast<unsigned char>(c)); // to make sure the following letters are small caps
            }
        }

        driverNameComplete = true;
    }

    Ambulance newAmbulance = {id, plateNumber, driverName};
    cout << "Ambulance registered successfully!" << endl;
    return newAmbulance;
}
