Hello! Here is the complete list of items/files in this folder.

.hpp files contain the declarations, while .cpp files contain definitions.

The raw data itself:
1. job_description.csv
2. resume.csv

To pre-process raw data:
1. cleaner_loader.hpp
2. cleaner_loader.cpp

The Job and Resume structs:
1. job_resume_struct.hpp

The Job and Resume nodes:
1. job_resume_node.hpp

Linked list containers for Job and Resume:
1. job_resume_linked_list.hpp
2. job_resume_linked_list.cpp

To display Job and Resume output:
1. display_output.hpp
2. display_output.cpp

Linked list insertion sort:
1. linked_list_insertion_sort.hpp
2. linked_list_insertion_sort.cpp

Functions to get user input:
1. user_promt.cpp
2. user_promt.hpp

Structs to store scores:
1. score_struct.hpp

All search operations (search jobs, search resumes, internal sorting):
1. linked_list_search.hpp
2. linked_list_search.cpp
NOTE: i havent buat the linear and the search resumes

Some util operations for strings:
1. string_utils.hpp
2. string_utils.cpp

To measure performance:
1. measure_performance.hpp
2. measure_performance.cpp

The main program to test and execute the functions:
1. main.cpp



Other notes:

When executing main.cpp remember to include {g++ *.cpp -o main -lpsapi}

Notes on lambdas (lambdas are basically just like callbacks imo):
- #include <functional>
- using namespace std;

- When defining:
    returnType functionName(const function<returnType(params with explicit types)> &lambdaName)
    <!-- const function<> func restricts modification -->
    <!-- function<> &func uses reference to the original function object instead of creating a copy -->
    <!-- So: const function<> &func restricts copy and modification -->

- When calling:
    functionName([&](args with explicit types){code to execute here});
    <!-- [&] is to refer to the variables in the scope outside of the lamba -->

- If has other params: 
    Defining: 
    returnType functionName(type param1, type param2, const function<returnType(params with types)> &lambdaName)

    Calling: functionName(arg1, arg2, [&](args with types){code to execute here});
