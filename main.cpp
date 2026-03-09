/* =====IMPORTANT!!!!!=====*/ 
/* I needed to use some features of c++ libraries we haven't seen before in the class 
for making these helper functions. Otherwise i might needed to code x2 longer of this version.
Also Mr. Gorur wanted from us to pay attention to the syntax in the previous lab lesson. 
This lab project made on that purpose as well.*/ 

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// helper functions to avoid extra library usage

char myToUpper(char c) {
  if (c >= 'a' && c <= 'z')
    return c - 'a' + 'A'; // adding extra amount to letter A for uppercase
  return c;
}

char myToLower(char c) {
  if (c >= 'A' && c <= 'Z')
    return c - 'A' + 'a'; // adding extra amount to letter a for lowercase
  return c;
}

bool myIsDigit(char c) { return c >= '0' && c <= '9'; }

bool myIsAlpha(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// same uppercase and lowercase operation done to strings below

string toUpper(const string &s) {
  string result = s;
  for (int i = 0; i < (int)result.size(); i++)
    result[i] = myToUpper(result[i]);
  return result;
}

string toLower(const string &s) {
  string result = s;
  for (int i = 0; i < (int)result.size(); i++)
    result[i] = myToLower(result[i]);
  return result;
}

void printPadded(const string &text, int width) {
  cout << text;
  for (int i = (int)text.size(); i < width; i++) // fit the string into the width we want
    cout << ' ';
}

void printPaddedInt(int num, int width) { printPadded(to_string(num), width); } // same operation for int

void printRepeat(char c, int count) {
  for (int i = 0; i < count; i++)
    cout << c;
}

void clearCin() {
  cin.clear(); // prevent input errors and memory leaks (user-based errors are included)
  string discard;
  getline(cin, discard);
}

string readLine(const string &prompt) {
  string input;
  cout << prompt;
  getline(cin, input);
  while (!input.empty() && input.front() == ' ') // erase spaces in the beginning (trimmer)
    input.erase(input.begin());
  while (!input.empty() && input.back() == ' ') // erase spaces in the end
    input.pop_back();
  return input;
}

class Passenger {
private:
  string name;
  string surname;
  char gender;

public:
  Passenger() : name(""), surname(""), gender(' ') {}

  Passenger(const string &name, const string &surname, char gender)
      : name(name), surname(surname), gender(myToUpper(gender)) {}

  string getName() const { return name; }
  string getSurname() const { return surname; }
  char getGender() const { return gender; }
  string getFullName() const { return name + " " + surname; }

  void setName(const string &name) { this->name = name; }
  void setSurname(const string &surname) { this->surname = surname; }
  void setGender(char gender) { this->gender = myToUpper(gender); }

  bool operator==(const Passenger &other) const {
    return name == other.name && surname == other.surname && gender == other.gender; // if equal return true
  }

  bool operator!=(const Passenger &other) const { return !(*this == other); } // if equal return false 

  friend ostream &operator<<(ostream &os, const Passenger &p) {
    os << p.name << " " << p.surname << " (" << p.gender << ")";
    return os;
  }
};

class Flight {
private:
  string flightNo;
  string destination;
  int maxSeats;
  int numPassengers;
  vector<bool> seatOccupied; // i needed to use an extra vector to keep in memory seat occupation statement
  vector<Passenger> seatPassenger;

  int seatLabelToIndex(const string &label) const {
    if (label.size() < 2)
      return -1; // if string size < 2, such as "A" or "6"
    string rowStr = label.substr(0, label.size() - 1); // gets row 
    char col = myToUpper(label[label.size() - 1]); // gets column 

    if (col < 'A' || col > 'D')
      return -1; // check for between A and D

    for (int i = 0; i < (int)rowStr.size(); i++) {
      if (!myIsDigit(rowStr[i]))
        return -1; // check if row is a number
    }
    int row = stoi(rowStr); // turn row from string to int 
    int totalRows = maxSeats / 4;
    if (row < 1 || row > totalRows)
      return -1;

    return (row - 1) * 4 + (col - 'A'); // calculates the index for the vector i created
  }

  string indexToSeatLabel(int index) const {
    int row = index / 4 + 1;
    char col = 'A' + (index % 4);
    return to_string(row) + col; // does the opposite operation above with suming row and column up
  }

public:
  Flight(const string &flightNo, const string &destination, int maxSeats = 40)
  /* by the way, i consciously used the reference opereator "&" in the constructor for a better memory usage and 
  performance optimization. */ 
    : flightNo(flightNo), destination(destination), numPassengers(0) {
    if (maxSeats <= 0)
      maxSeats = 40;
    if (maxSeats % 2 != 0)
      maxSeats--;
    if (maxSeats > 80)
      maxSeats = 80;
    this->maxSeats = maxSeats;

    seatOccupied.resize(maxSeats, false); // defaultly it makes 40 seats not occupied 
    seatPassenger.resize(maxSeats); // makes 40 passenger object
  }

  string getFlightNo() const { return flightNo; }
  string getDestination() const { return destination; }
  int getMaxSeats() const { return maxSeats; }
  int getNumPassengers() const { return numPassengers; }
  int availableSeats() const { return maxSeats - numPassengers; }

  void displaySeatingPlan() const {
    cout << "\n Legend:" << endl;
    cout << " X - Occupied Seat" << endl;
    cout << " O - Vacant Seat" << endl;
    cout << "\nSeating Plan:" << endl;
    cout << "-------------Front------------" << endl;

    int totalRows = maxSeats / 4;
    for (int r = 0; r < totalRows; r++) {
      int row = r + 1;
      cout << "| ";
      for (int c = 0; c < 4; c++) {
        int idx = r * 4 + c;
        char status = seatOccupied[idx] ? 'X' : 'O'; // checks the status of the seat (occupied or not)
        // ternary makes it shorter and way faster i love it
        cout << row << (char)('A' + c) << " " << status;
        if (c == 1)
          cout << " | |";
        else if (c < 3)
          cout << " | ";
      }
      cout << " |" << endl;
    }
  }

  bool reserveSeat(const Passenger &passenger, const string &seatLabel) {
    int idx = seatLabelToIndex(seatLabel);
    if (idx == -1) {
      cout << "Error: Invalid seat label \"" << seatLabel << "\"." << endl;
      return false;
    }
    if (numPassengers >= maxSeats) {
      cout << "Error: Flight " << flightNo << " is full. No available seats."
           << endl;
      return false;
    }
    if (seatOccupied[idx]) {
      cout << "Error: Seat " << seatLabel << " is already occupied." << endl;
      return false;
    }
	// error handling made above 
	
    seatOccupied[idx] = true;
    seatPassenger[idx] = passenger;
    numPassengers++;
    cout << "Seat " << seatLabel << " reserved successfully for "
         << passenger.getFullName() << "." << endl;
    return true;
  }

  bool cancelReservation(const Passenger &passenger) {
    for (int i = 0; i < maxSeats; i++) {
      if (seatOccupied[i] && seatPassenger[i] == passenger) {
        seatOccupied[i] = false;
        seatPassenger[i] = Passenger();
        numPassengers--;
        cout << "Reservation for " << passenger.getFullName() << " at seat "
             << indexToSeatLabel(i) << " has been cancelled." << endl;
        return true;
      }
    }
    cout << "Error: No reservation found for " << passenger.getFullName() << "."
         << endl;
    return false;
  }

  int numberOfPassengers() const { return numPassengers; }

  void printPassengers() const {
    cout << "\nPassenger List for Flight " << flightNo << ":" << endl;
    printPadded("Seat", 4);
    cout << "|";
    printPadded("Passenger Name", 15);
    cout << "|";
    cout << "Gender" << endl;
    printRepeat('-', 4);
    cout << "|";
    printRepeat('-', 15);
    cout << "|";
    printRepeat('-', 6);
    cout << endl;

    for (int i = 0; i < maxSeats; i++) { // it makes fullName fit to the blank given for the printPassengers
      if (seatOccupied[i]) {
        string seatLabel = indexToSeatLabel(i);
        string fullName = seatPassenger[i].getFullName();
        if ((int)fullName.length() > 15)
          fullName = fullName.substr(0, 14) + ".";

        printPadded(seatLabel, 4);
        cout << "|";
        printPadded(fullName, 15);
        cout << "|";
        cout << seatPassenger[i].getGender() << endl;
      }
    }

    if (numPassengers == 0)
      cout << "No passengers on this flight." << endl;
  }

  bool isFlyingTo(const string &dest) const {
    return toLower(destination) == toLower(dest);
  }
};

class FlightManager {
private:
  vector<Flight> flights;

public:
  bool addFlight(const Flight &flight) {
    for (int i = 0; i < (int)flights.size(); i++) {
      if (flights[i].getFlightNo() == flight.getFlightNo()) {
        cout << "Error: Flight " << flight.getFlightNo() << " already exists."
             << endl;
        return false;
      }
    }
    flights.push_back(flight);
    cout << "Flight " << flight.getFlightNo() << " to "
         << flight.getDestination() << " added successfully." << endl;
    return true;
  }

  bool removeFlight(const string &flightNumber) {
    for (int i = 0; i < (int)flights.size(); i++) {
      if (flights[i].getFlightNo() == flightNumber) {
        flights.erase(flights.begin() + i);
        cout << "Flight " << flightNumber << " removed successfully." << endl;
        return true;
      }
    }
    cout << "Error: Flight " << flightNumber << " not found." << endl;
    return false;
  }

  void listAllFlights() const {
    if (flights.empty()) {
      cout << "\nNo flights available." << endl;
      return;
    }
    cout << "\n--- All Flights ---" << endl;
    printPadded("Flight No", 12);
    printPadded("Destination", 20);
    printPadded("Available Seats", 15);
    cout << endl;
    printRepeat('-', 47);
    cout << endl;
    for (int i = 0; i < (int)flights.size(); i++) {
      printPadded(flights[i].getFlightNo(), 12);
      printPadded(flights[i].getDestination(), 20);
      printPaddedInt(flights[i].availableSeats(), 15);
      cout << endl;
    }
  }

  // returns the index of the flight, or -1 if not found
  int findFlightIndex(const string &flightNumber) {
    for (int i = 0; i < (int)flights.size(); i++) {
      if (flights[i].getFlightNo() == flightNumber)
        return i;
    }
    return -1;
  }

  Flight &getFlightAt(int index) { return flights[index]; }
};

// validation functions for misusage or errors made by user

bool isValidFlightNumber(const string &fn) {
  if (fn.size() < 3 || fn.size() > 6)
    return false;

  string airlineUpper = toUpper(fn.substr(0, 2));
  if (airlineUpper != "TK" && airlineUpper != "PG")
    return false;

  string digits = fn.substr(2);
  if (digits.empty() || digits.size() > 4)
    return false;

  for (int i = 0; i < (int)digits.size(); i++) {
    if (!myIsDigit(digits[i]))
      return false;
  }
  return true;
}

bool isValidName(const string &name) {
  if (name.empty())
    return false;
  for (int i = 0; i < (int)name.size(); i++) {
    if (!myIsAlpha(name[i]) && name[i] != ' ')
      return false;
  }
  return true;
}

// functions for the menus that will presented to passenger


void passengerMenu(Flight &flight) {
  int choice = 0;
  while (true) {
    cout << "\n===== Passenger Management: Flight " << flight.getFlightNo()
         << " (" << flight.getDestination() << ") =====" << endl;
    cout << "1. Reserve a Seat" << endl;
    cout << "2. Cancel a Reservation" << endl;
    cout << "3. View Passenger List" << endl;
    cout << "4. Back to Flight Management Menu" << endl;
    cout << "Enter your choice: ";

    if (!(cin >> choice)) {
      clearCin();
      cout << "Invalid input. Please enter a number between 1 and 4." << endl;
      continue;
    }
    clearCin();

    switch (choice) { // normally i would use if else structure but i choose switch case instead 
    case 1: {
      flight.displaySeatingPlan();

      if (flight.availableSeats() == 0) {
        cout << "This flight is full. No seats available for reservation."
             << endl;
        break;
      }

      string seatLabel = readLine("Choose a seat to reserve (e.g., 1A): ");
      seatLabel = toUpper(seatLabel);

      string name;
      do { // purpose of the do-while usage is to trigger validation functions for repeating the operation
        name = readLine("Enter passenger first name: ");
        if (!isValidName(name))
          cout << "Error: Name must contain only letters. Please try again."
               << endl;
      } while (!isValidName(name));

      string surname;
      do { // purpose of the do-while usage is to trigger validation functions for repeating the operation
        surname = readLine("Enter passenger surname: ");
        if (!isValidName(surname))
          cout << "Error: Surname must contain only letters. Please try again."
               << endl;
      } while (!isValidName(surname));

      char gender;
      do { // purpose of the do-while usage is to trigger validation functions for repeating the operation
        string genderStr = readLine("Enter passenger gender (M/F): ");
        if (genderStr.empty()) {
          cout << "Error: Please enter M or F." << endl;
          gender = ' ';
        } else {
          gender = myToUpper(genderStr[0]);
          if (gender != 'M' && gender != 'F')
            cout << "Error: Gender must be M or F. Please try again." << endl;
        }
      } while (gender != 'M' && gender != 'F');

      Passenger p(name, surname, gender);
      flight.reserveSeat(p, seatLabel);
      break;
    }
    case 2: {
      if (flight.numberOfPassengers() == 0) {
        cout << "No passengers on this flight to cancel." << endl;
        break;
      }

      string name;
      do { // same do while usage here
        name = readLine("Enter passenger first name: ");
        if (!isValidName(name))
          cout << "Error: Name must contain only letters. Please try again."
               << endl;
      } while (!isValidName(name));

      string surname;
      do { // same do while usage here
        surname = readLine("Enter passenger surname: ");
        if (!isValidName(surname))
          cout << "Error: Surname must contain only letters. Please try again."
               << endl;
      } while (!isValidName(surname));

      char gender;
      do { // same do while usage here
        string genderStr = readLine("Enter passenger gender (M/F): ");
        if (genderStr.empty()) {
          cout << "Error: Please enter M or F." << endl;
          gender = ' ';
        } else {
          gender = myToUpper(genderStr[0]);
          if (gender != 'M' && gender != 'F')
            cout << "Error: Gender must be M or F. Please try again." << endl;
        }
      } while (gender != 'M' && gender != 'F');

      Passenger p(name, surname, gender);
      flight.cancelReservation(p);
      break;
    }
    case 3: {
      flight.printPassengers();
      break;
    }
    case 4: {
      return;
    }
    default: {
      cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
      break;
    }
    }
  }
}


void flightMenu(FlightManager &manager) {
  int choice = 0;
  while (true) {
    cout << "\n===== Flight Management System =====" << endl;
    cout << "1. Add a Flight" << endl;
    cout << "2. Remove a Flight" << endl;
    cout << "3. List All Flights" << endl;
    cout << "4. Select a Flight and Manage Passengers" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";

    if (!(cin >> choice)) {
      clearCin();
      cout << "Invalid input. Please enter a number between 1 and 5." << endl;
      continue;
    }
    clearCin();

    switch (choice) {
    case 1: {
      string flightNo;
      do { 
        flightNo = readLine("Enter flight number (e.g., TK101, PG202): ");
        flightNo = toUpper(flightNo);
        if (!isValidFlightNumber(flightNo))
          cout << "Error: Invalid flight number. Must be TK or PG followed by "
                  "1-4 digits."
               << endl;
      } while (!isValidFlightNumber(flightNo));

      string destination = readLine("Enter destination: ");
      if (destination.empty()) {
        cout << "Error: Destination cannot be empty." << endl;
        break;
      }

      Flight newFlight(flightNo, destination);
      manager.addFlight(newFlight);
      break;
    }
    case 2: {
      string flightNo = readLine("Enter flight number to remove: ");
      flightNo = toUpper(flightNo);
      manager.removeFlight(flightNo);
      break;
    }
    case 3: {
      manager.listAllFlights();
      break;
    }
    case 4: {
      string flightNo = readLine("Enter flight number to manage: ");
      flightNo = toUpper(flightNo);
      int index = manager.findFlightIndex(flightNo);
      if (index == -1) {
        cout << "Error: Flight " << flightNo << " not found." << endl;
      } else {
        passengerMenu(manager.getFlightAt(index));
      }
      break;
    }
    case 5: {
      cout << "Exiting the Flight Management System. Goodbye!" << endl;
      return;
    }
    default: {
      cout << "Invalid choice. Please enter a number between 1 and 5." << endl;
      break;
    }
    }
  }
}


int main() {
  FlightManager manager;
  flightMenu(manager);
  return 0;
}