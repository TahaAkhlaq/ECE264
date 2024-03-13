/* Name: Taha Akhlaq
  Description:
  * This program processes a list of unique creatures named foobars from a
  * given input file. It determines their line positions, calculates their
  * strength based on their type (either an ordinary foobar, a foo, or a bar),
  * and their place in the line. Lastly, it records each foobar's name along
  * with their calculated strength, in the same sequence they appear in the
  * input file, into a designated output file.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// base class for ordinary foobar
class Foobar {
private:
  string name;  // name of the foobar
  int position; // position in the line

protected:
  // gets the foobar's position in the line
  int getPosition() const { return position; }

public:
  // constructor for intializing the foobar with its name
  // the position defaults to 0
  Foobar(string name) : name(name), position(0) {}

  // sets the position of the foobar in the line
  void setPosition(int pos) { position = pos; }

  // gets the name of the foobar
  string getName() const { return name; }

  // the strength of the foobar is based on its position
  virtual int getStrength() const { return position; }
};

// derived class for foo
class Foo : public Foobar {
public:
  Foo(string name) : Foobar(name) {} // constructor for foo

  // strength for foo is its position times 3
  int getStrength() const override { return getPosition() * 3; }
};

// derived class for bar
class Bar : public Foobar {
public:
  Bar(string name) : Foobar(name) {} // constructor for bar

  // strength for bar is its position plus 15
  int getStrength() const override { return getPosition() + 15; }
};

// loads a vector with foobar pointers from the input file
void readInputFile(const string &filename, vector<Foobar *> &foobars) {

  ifstream inFile(filename);
  if (!inFile) {
    cerr << "Can not open the file: " << filename << endl;
    exit(EXIT_FAILURE); // terminates the program if input file can not be opened
  }

  // read each line in the input file to get the foobar's type and name
  string line, type, name;
  while (getline(inFile, line)) {
    stringstream ss(line); // use stringstream to break the line into words
    ss >> type >> name;    // extract the type and name

    // based on the type, create objects and add it the vector
    if (type == "foo") {
      foobars.push_back(new Foo(name));
    } else if (type == "bar") {
      foobars.push_back(new Bar(name));
    } else { // if it's not a foo or bar, it is an ordinary foobar
      foobars.push_back(new Foobar(name));
    }
  }
}

/* assigns positions in reverse order within the vector to correctly calculate
foobar strengths, treating the beginning of the vector as the end of the line */
void updatePositions(vector<Foobar *> &foobars) {
  int size = foobars.size();
  for (int i = 0; i < size; i++) {
    foobars[i]->setPosition(size - i);
  }
}

// saves each foobar's name and strength in the output file
void writeOutputFile(const string &filename, const vector<Foobar *> &foobars) {
  ofstream outFile(filename);

  if (!outFile) {
    cerr << "Can not open the file: " << filename << endl;
    exit(EXIT_FAILURE); // terminates the program if file can not be opened
  }

  // iterates through the vector of foobars
  for (const auto &foobar : foobars) {
    outFile << foobar->getName() << " " << foobar->getStrength() << endl;
  }
}

int main() {

  string inputFileName, outputFileName;

  cout << "Enter the name of the input file: ";
  cin >> inputFileName;
  cout << "Enter the name of the output file: ";
  cin >> outputFileName;

  // vector to hold pointers to the foobar objects
  vector<Foobar *> foobars;

  readInputFile(inputFileName, foobars);
  updatePositions(foobars);
  writeOutputFile(outputFileName, foobars);

  // free up the dynamically allocated foobars from memory
  for (auto &foobar : foobars) {
    delete foobar;
  }

  cout << "Output file has been generated!\n";
  return 0;
}