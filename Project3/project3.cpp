// Name: Taha Akhlaq

// THIS IS THE PROVIDED CODE FOR PROGRAM #3, DSA 1, SPRING 2024

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
  string lastName;
  string firstName;
  string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

  ifstream input(filename);
  if (!input) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // The first line indicates the size
  string line;
  getline(input, line);
  stringstream ss(line);
  int size;
  ss >> size;

  // Load the data
  for (int i = 0; i < size; i++) {
    getline(input, line);
    stringstream ss2(line);
    Data *pData = new Data();
    ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
    l.push_back(pData);
  }

  input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

  ofstream output(filename);
  if (!output) {
    cerr << "Error: could not open " << filename << "\n";
    exit(1);
  }

  // Write the size first
  int size = l.size();
  output << size << "\n";

  // Write the data
  for (auto pData : l) {
    output << pData->lastName << " " << pData->firstName << " " << pData->ssn
           << "\n";
  }

  output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
  string filename;
  cout << "Enter name of input file: ";
  cin >> filename;
  list<Data *> theList;
  loadDataList(theList, filename);

  cout << "Data loaded.\n";

  cout << "Executing sort...\n";
  clock_t t1 = clock();
  sortDataList(theList);
  clock_t t2 = clock();
  double timeDiff = ((double)(t2 - t1)) / CLOCKS_PER_SEC;

  cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

  cout << "Enter name of output file: ";
  cin >> filename;
  writeDataList(theList, filename);

  return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.

// Additional Header Files
#include <iterator>
#include <map>
#include <set>

// Global Arrays and Pointers for Radix Sort:
// Bins for Radix Sorting based on the First Letter of the Last Name
Data *bin[26][100000];
string ssnlist[1100000] = {}; // Array to Store SSNs for Sorting (up to 1,100,000 entries)
int ptrl[26]; // Pointers to Track the Current Index of Data Each of the Bins

int i;

// Function Declarations
bool compare(Data *a, Data *b); // Comparison Function for Sorting
int determineSortStrategy(list<Data *> &l); // Determines the Appropriate Sorting Strategy
void insertionSort(list<Data *> &l); // Performs an Insertion Sort on the List
void radixSort(list<Data *> &l);  // Performs a Radix Sort on the List

int point[256] = {0}; // Helper Array for Sorting Uses

// Compares Two Data Pointers for Sorting Appropriately
bool compare(Data *a, Data *b) {
  int i = strcmp((a->lastName).c_str(), (b->lastName).c_str()); // Compares Last Names
  if (i < 0) {
    return 1;
  }

  else if (i > 0) {
    return 0;
  }

  else {
    // When the Last Names are the Same, then sort by First Name
    return strcmp((a->firstName).c_str(), (b->firstName).c_str()) < 0;
  }
}

// Determines the Appropriate Sorting Strategy
int determineSortStrategy(list<Data *> &l) {
  list<string> testcase; // Create a List to Store the First Names
  int i = 0;

  for (auto iter = l.begin(); i != 15;
       iter++) { // Takes the First Names from the List
    testcase.push_back((*iter)->firstName);
    i++;
  }

  testcase.unique(); // Remove Duplicates from the First Name Data

  // Logic for Sorting Strategy
  if (testcase.size() == 1) { // All First Names Are Identical
    return 4;
  }

  else if (testcase.size() < 5) { // For Few Unique First Names
    return 3;
  }

  else if (l.size() <= 110000) {
    return 1;
  }

  else { // For Radix Sort
    return 2;
  }
}

string ss[1100000];

// Insertion Sort 
void insertionSort(list<Data *> &l) {
  int n = 0;
  auto iter = l.begin();
  for (auto iter1 = l.begin(); iter1 != l.end();) {
    string firstn = (*iter1)->firstName;

    // Group by First Name and Sort Each Group
    while (n != l.size() && (*iter1)->firstName == firstn) {
      ss[i] = (*iter1)->ssn;
      iter1++;
      i++;
      n++;
    }

    // Sort Each Group using Insertion Sort on SSNs
    for (int j = 1; j <= i - 1; j++) {
      string a = ss[j];
      int k = j - 1;
      while ((a < ss[k]) && (k >= 0)) {
        ss[k + 1] = ss[k];
        k--;
      }
      ss[k + 1] = a;
    }

    // Update list with Sorted SSNs
    for (int q = 0; q < i; q++) {
      (*iter)->ssn = ss[q];
      iter++;
    }
    i = 0; // Reset Index for the Next Group
  }
}

// Radix Sort
void radixSort(list<Data *> &l) {

  auto iter = l.begin();
  for (auto &iter1 : l) {
    int i = ((iter1->lastName)[0] - 65);
    bin[i][ptrl[i]] = iter1;
    ptrl[i]++;
  }
  
  // For each bin using the custom compare function:
  for (int j = 0; j < 26; j++) {
    sort(&bin[j][0], &bin[j][0] + ptrl[j], compare);
    for (int k = 0; k < ptrl[j]; k++) {
      *iter = bin[j][k];
      iter++;
    }
  }
  insertionSort(l); //Performs Insertion Sort
}

// Loads the SSNs from list to array
void inputssn(list<Data *> &l) {
  int i = 0;
  for (list<Data *>::iterator it = l.begin(); it != l.end(); it++) {
    ssnlist[i] = (*it)->ssn;
    i++;
  }
}

// Function to Output sorted SSNs back into the list
void outputssn(list<Data *> &l, string A[]) {
  int i = 0;
  for (list<Data *>::iterator it = l.begin(); it != l.end(); it++) {
    (*it)->ssn = A[i];
    i++;
  }
}

// Main Function for Sorting Data based on the Strategy
void sortDataList(list<Data *> &l) {
  int size = l.size();
  switch (determineSortStrategy(l)) {
  case 1:
  case 2:
    radixSort(l);
    break;
  case 3:
    insertionSort(l);
    break;
  case 4:
    // Directly sort based on SSNs when all first names are the same
    inputssn(l);
    sort(ssnlist, ssnlist + size);
    outputssn(l, ssnlist);
    break;
  }
}