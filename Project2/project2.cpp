/*
Name: Taha Akhlaq
Description:
  *This program reads commands from an input file for controlling stacks and
  *queues containing integers, doubles, and strings. It performs actions such
  *as their creation, adding (push), and removing (pop) elements. The program
  *tracks and records the execution of these commands, including any errors,
  *into an output file.
*/

#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

// Node template class representing an element in the SimpleList data structure
template <typename T> class Node {
public:
  T data;        // Data stored in the node
  Node *next;    // Pointer to the next node
  Node(T value); // Constructor to initialize the node
};

// Base class for SimpleList to be used in stack and queue implementations
template <typename T> class SimpleList {
protected:
  Node<T> *head; // Pointer to the first node in the list (head)
  Node<T> *tail; // Pointer to the last node in the list (tail)
  string name;   // The name of the list

public:
  SimpleList(string listName); // Constructor
  virtual ~SimpleList();  // Destructor for freeing up dynamically allocated
                          // memory used by the list
  string getName() const; // Getter function for the list's name
  virtual void push(T value) = 0; // Adding elements
  virtual T pop() = 0;            // Removing Elements
  bool isEmpty() const;
};

// Stack class (using the SimpleList as the base class) LIFO
template <typename T> class Stack : public SimpleList<T> {
public:
  Stack(string listName);      // Constructor
  void push(T value) override; // Pushes (adds) a value onto the stack
  T pop() override;            // Pops (removes) a value from the stack
};

// Queue class (using the SimpleList as the base class) FIFO
template <typename T> class Queue : public SimpleList<T> {
public:
  Queue(string listName);      // Constructor
  void push(T value) override; // A value goes into the queue
  T pop() override;            // A value gets removed from the queue
};

// Implementation of the Node constructor
template <typename T> Node<T>::Node(T value) : data(value), next(nullptr) {}

// Implmenttation of the SimpleList destructor
template <typename T>
SimpleList<T>::SimpleList(string listName)
    : head(nullptr), tail(nullptr), name(listName) {}

template <typename T> SimpleList<T>::~SimpleList() {
  while (head != nullptr) {
    Node<T> *temp = head;
    head = head->next;
    delete temp;
  }
}

// Returns the name of the list
template <typename T> string SimpleList<T>::getName() const { return name; }

// Checks if the list is empty
template <typename T> bool SimpleList<T>::isEmpty() const {
  return head == nullptr;
}

// Stack method implementations

// Constructor for Stack class
template <typename T>
Stack<T>::Stack(string listName) : SimpleList<T>(listName) {}

// Push for Stack class
template <typename T> void Stack<T>::push(T value) {
  Node<T> *newNode = new Node<T>(value); // Create a new node
  newNode->next =
      this->head;       // Set the new node's next pointer to the current head
  this->head = newNode; // Update the head pointer to point to the new node
  if (this->tail == nullptr) { // If the list is empty then also update the tail
                               // pointer to point to the new node
    this->tail = newNode;
  }
}

// Pop for Stack Class
template <typename T> T Stack<T>::pop() {
  if (this->isEmpty()) {
    throw logic_error(
        "ERROR: This list is empty!"); // Check if the stack is empty
  }
  Node<T> *temp = this->head; // The temporary pointer points to the head node
  T value = temp->data;       // Gets the value of the head node
  this->head = this->head->next; // Moves the head pointer to the next node
  if (this->head == nullptr) {   // Update the tail pointer to point to null if
                                 // the stack is empty
    this->tail = nullptr;
  }
  delete temp; // Remove (pop) the old head node
  return value;
}

// Queue methods implementation

// Constructor for Queue class
template <typename T>
Queue<T>::Queue(string listName) : SimpleList<T>(listName) {}

// Push for Queue class
template <typename T> void Queue<T>::push(T value) {
  Node<T> *newNode = new Node<T>(value); // Create a new node
  if (this->isEmpty()) { // If the queue is empty then the head and tail
                         // pointers point to the new node
    this->head = this->tail = newNode;
  } else {
    this->tail->next =
        newNode; // Otherwise, set the current tail pointer to the new node
    this->tail = newNode; // Update the tail pointer to the new node
  }
}

// Pop for Queue class
template <typename T> T Queue<T>::pop() {
  if (this->isEmpty()) {
    throw logic_error(
        "ERROR: This list is empty!"); // Check if the queue is empty
  }
  Node<T> *temp = this->head; // The temporary pointer points to the head node
  T value = temp->data;       // Gets the value of the head node
  this->head = this->head->next; // Moves the head pointer to the next node
  if (this->head == nullptr) {   // Update the tail pointer to point to null if
                                 // the queue is empty
    this->tail = nullptr;
  }
  delete temp; // Remove (pop) the old head node
  return value;
}

// Function to find a SimpleList by name in a list of SimpleList pointers
template <typename T>
SimpleList<T> *findList(const string &name, list<SimpleList<T> *> &lists) {
  for (SimpleList<T> *list :
       lists) { // Iterate throrugh the list of SimpleList pointers
    if (list->getName() == name) {
      return list; // If the list is found, return a pointer to the SimpleList
    }
  }
  return nullptr; // If the list is not found then return null
}

int main() {
  string inputFileName, outputFileName; // Store filenames for input and output
  cout << "Enter name of input file: "; // Ask user for the input and output
                                        // file names
  cin >> inputFileName;
  cout << "Enter name of output file: ";
  cin >> outputFileName;

  // Open the input file for reading the commands and the output file for
  // recording the output
  ifstream inputFile(inputFileName);
  ofstream outputFile(outputFileName);

  if (!inputFile.is_open()) {
    cerr << "ERROR: Unable to open the file!" << endl;
    return 1; // Return failure if can not open the file
  }

  // Initialize SimpleList to hold pointers to stack and queues for each data
  // type
  list<SimpleList<int> *> integerLists;
  list<SimpleList<double> *> doubleLists;
  list<SimpleList<string> *> stringLists;

  // Variables for processing the commands
  string dataType, elementValue, command, listName;

  while (inputFile >> command) {
    outputFile << "PROCESSING COMMAND: " << command;

    if (command == "create") { // Create new lists

      // Read the list name and type from the input file
      inputFile >> listName >> dataType;
      outputFile << " " << listName << " " << dataType << endl;

      // Check to see if the name already exists
      if ((findList(listName, integerLists) == nullptr) &&
          (findList(listName, doubleLists) == nullptr) &&
          (findList(listName, stringLists) == nullptr)) {

        // Create a new list based on if its a stack or a queue and the data
        // type
        if (dataType == "stack") {
          if (listName[0] == 'i') {
            // Create a stack of integers if the list name starts with i
            integerLists.push_front(new Stack<int>(listName));
          } else if (listName[0] == 'd') {
            // Create a stack of doubles if the list name starts with d
            doubleLists.push_front(new Stack<double>(listName));
          } else if (listName[0] == 's') {
            // Create a stack of strings if the list name starts with s
            stringLists.push_front(new Stack<string>(listName));
          }
        } else if (dataType == "queue") {
          if (listName[0] == 'i') {
            // Create a queue of integers if the list name starts with i
            integerLists.push_front(new Queue<int>(listName));
          } else if (listName[0] == 'd') {
            // Create a queue of doubles if the list name starts with d
            doubleLists.push_front(new Queue<double>(listName));
          } else if (listName[0] == 's') {
            // Create a queue of strings if the list name starts with s
            stringLists.push_front(new Queue<string>(listName));
          }
        }

      }

      else {
        outputFile << "ERROR: This name already exists!" << endl;
      }
    }

    else if (command == "push") { // Push command
      inputFile >> listName >> elementValue;
      outputFile << " " << listName << " " << elementValue << endl;

      bool found = false;

      // Push an integer element to the list if it exists
      if (listName[0] == 'i') {
        auto list = findList(listName, integerLists);
        if (list) {
          list->push(stoi(elementValue)); // Convert string to integer and push
          found = true;
        }
      }

      // Push a double element to the list if it exists
      else if (listName[0] == 'd') {
        auto list = findList(listName, doubleLists);
        if (list) {
          list->push(stod(elementValue)); // Convert string to double and push
          found = true;
        }
      }

      // Push a string element to the list if it exists
      else if (listName[0] == 's') {
        auto list = findList(listName, stringLists);
        if (list) {
          list->push(elementValue); // Push the string element
          found = true;
        }
      }

      if (!found)
        outputFile << "ERROR: This name does not exist!" << endl;
    }

    else if (command == "pop") { // Pop commmand
      inputFile >> listName;
      outputFile << " " << listName << endl;
      bool found = false;

      // Remove a string element from the list if it exists and is not empty
      if (listName[0] == 'i') {
        auto list = findList(listName, integerLists);
        if (list && !list->isEmpty()) {
          outputFile << "Value popped: " << list->pop()
                     << endl; // Pop the value and output it
          found = true;
        }
      }

      // Remove a string element from the list if it exists and is not empty
      else if (listName[0] == 'd') {
        auto list = findList(listName, doubleLists);
        if (list && !list->isEmpty()) {
          outputFile << "Value popped: " << list->pop()
                     << endl; // Pop the value and output it
          found = true;
        }
      }

      // Remove a string element from the list if it exists and is not empty
      else if (listName[0] == 's') {
        auto list = findList(listName, stringLists);

        if (list && !list->isEmpty()) {
          outputFile << "Value popped: " << list->pop()
                     << endl; // Pop the value and output it
          found = true;
        }
      }

      // Output the correct Error message if a value was not popped (removed)
      if (!found)
        outputFile << (findList(listName, integerLists) ||
                               findList(listName, doubleLists) ||
                               findList(listName, stringLists)
                           ? "ERROR: This list is empty!"
                           : "ERROR: This name does not exist!")
                   << endl;
    }
  }

  // Free up memory by deleting all the lists
  for (auto &list : integerLists)
    delete list;
  for (auto &list : doubleLists)
    delete list;
  for (auto &list : stringLists)
    delete list;

  inputFile.close();
  outputFile.close();

  return 0;
}