#ifndef ARRAY_JOB_RESUME_HPP
#define ARRAY_JOB_RESUME_HPP

#include "job_resume_struct.hpp"

class ResumeArray
{
private:
    Resume *arr;
    int capacity;
    int size;

public:
    ResumeArray(int cap = 20000);
    ~ResumeArray();
    void insert(const Resume &r);
    int getSize() const;
    void setSize(int newSize);
    Resume *getArray();
    const Resume *getArray() const;
};

class JobArray
{
private:
    Job *arr;
    int capacity;
    int size;

public:
    JobArray(int cap = 20000);
    ~JobArray();

    void insert(const Job &j);
    int getSize() const;
    void setSize(int newSize);
    Job *getArray();
    const Job *getArray() const;
};

#endif