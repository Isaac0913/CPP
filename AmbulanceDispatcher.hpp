#ifndef AMBULANCE_DISPATCH_HPP
#define AMBULANCE_DISPATCH_HPP

// for file I/O
#include <sstream>
#include "TextFileManager.hpp"
#include "Ambulance.hpp"

class AmbulanceDispatcher
{
private:
    TextFileManager fileManager;
    static const string FILENAME; // the filename where ambulances are stored
    static const char DELIMITER;  // the delimiter

    // ambulances
    static constexpr int MAX_AMBULANCES = 10;
    Ambulance ambulances[MAX_AMBULANCES];

    int count; // how many ambulances in the queue
    int front; // index of the first in queue
    int rear;  // index of the last in queue

    // queue full or empty check
    bool isFull();
    bool isEmpty();

    // to get new unique ID
    static int lastId;
    int generateAmbulanceId();

public:
    // constructor & desturctor
    AmbulanceDispatcher();
    ~AmbulanceDispatcher();

    // prompt for user input and create new ambulance
    Ambulance createNewAmbulance();

    // core functionalities
    void registerAmbulance(Ambulance ambulance, bool saveToFile);
    void rotateShift();
    void displaySchedule();

    // file/IO
    void loadAmbulancesFromFile();
    void saveAmbulancesToFile();
};

#endif
