#include "job_resume_array.hpp"

ResumeArray::ResumeArray(int cap) : capacity(cap), size(0)
{
    arr = new Resume[capacity];
}

ResumeArray::~ResumeArray()
{
    delete[] arr;
}

void ResumeArray::insert(const Resume &r)
{
    if (size < capacity)
        arr[size++] = r;
}

int ResumeArray::getSize() const
{
    return size;
}

void ResumeArray::setSize(int newSize)
{
    size = newSize;
}

Resume *ResumeArray::getArray()
{
    return arr;
}

const Resume *ResumeArray::getArray() const
{
    return arr;
}

JobArray::JobArray(int cap) : capacity(cap), size(0)
{
    arr = new Job[capacity];
}

JobArray::~JobArray()
{
    delete[] arr;
}

void JobArray::insert(const Job &j)
{
    if (size < capacity)
        arr[size++] = j;
}

int JobArray::getSize() const
{
    return size;
}
void JobArray::setSize(int newSize)
{
    size = newSize;
}
Job *JobArray::getArray()
{
    return arr;
}

const Job *JobArray::getArray() const
{
    return arr;
}
