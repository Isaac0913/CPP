#ifndef TEXT_FILE_MANAGER_HPP
#define TEXT_FILE_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class TextFileManager
{
public:
    // since C++ does not allow returning a full array, we return pointer instead
    // Returns pointer to a dynamically allocated array
    static string *readLinesFromFile(const string &filename, int &lineCount)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error opening file: " << filename << endl;
            lineCount = 0;
            return nullptr;
        }

        // count how many rows in text file
        // purpose to do this is to count how many rows first instead of using a fixed-sized array
        lineCount = 0;
        string line;
        while (getline(file, line))
        {
            if (!line.empty())
            {
                lineCount++;
            }
        }

        // --- Reset file stream to beginning ---
        file.clear();            // Clear any error flags (like EOF)
        file.seekg(0, ios::beg); // Go back to the start of the file

        // return if no lines in text file
        if (lineCount == 0)
        {
            file.close();
            return nullptr;
        }

        // create new array using the count we got earlier
        // assign each element of new array with the lines in the text file
        string *lines = new string[lineCount];
        int index = 0;
        while (getline(file, line))
        {
            if (!line.empty())
            {
                lines[index++] = line;
            }
        }

        file.close();
        return lines;
    }

    // Write lines to a file from a given array
    static void writeLinesToFile(const string &filename, string *lines, int lineCount)
    {
        ofstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error writing to file: " << filename << endl;
            return;
        }

        for (int i = 0; i < lineCount; ++i)
        {
            file << lines[i] << "\n";
        }

        file.close();
    }
};

#endif
