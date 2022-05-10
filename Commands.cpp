// Author: Gladys Monagan 
// A univariate polynomial term in x
// Course: CPSC 2150
// Copyright 2020

#include "Commands.h"
#include <sstream>    // std::stringstream
#include <map>        // map
#include <string>     // std::string
#include <iostream>   // std::cout std::cin
#include <fstream>    // std::ifstream std::ofstream

using std::istream, std::ostream, std::string;
using std::pair;

// output the commands available
void printCommandMenu(ostream& out) {
   out << "\n ------------------ command list ----------------------\n";
   out << "(i) - input the polynomials a and b\n";
   out << "(s) - sum up the polynomials a and b the polynomial a + b\n";
   out << "(p) - print the polynomials a and b and a + b\n";
   out << "(n) - number of terms in the polynomials a and b and a+b\n";
   out << "(e) - evaluate a, b and a + b at a specific x value\n";
   out << "(c) - coefficient of a term when the term's degree is supplied\n";
   out << "(r) - reset or restart the process with two new polynomials\n";
   out << "(m) - mention or comment, the subsequent line is ignored\n";
   out << "(d) - differentiate the polynomials a and b\n";
   out << "(q) - quit the program altogether\n";
   out << " --------------------------------------------------------\n"; 
}

// make an associative array of the character (that will be obtained from input)
// and its associated command
void buildCommands(std::map<char, Commands>& commands) {
   commands.insert(pair('i', INPUT));
   commands.insert(pair('s', SUM));
   commands.insert(pair('p', PRINT));
   commands.insert(pair('n', NUMBER_OF_TERMS));
   commands.insert(pair('e', EVALUATE));
   commands.insert(pair('c', GET_COEFF));
   commands.insert(pair('r', RESET));
   commands.insert(pair('m', MENTION));
   commands.insert(pair('d', DIFFERENTIATE));
   commands.insert(pair('q', QUIT));
}

// read from input stream in  a command and obtain the first letter of that 
// command to match it to a corresponding value from the enum type Commands
// if notInteractive, the command read is echoed out
// postcondition:
//    returns a command from Commands (including INVALID)
Commands getCommand(std::map<char, Commands>& commands, 
      istream& in, bool notInteractive, ostream& out) {
   out << "\ncommand: ";
   string commandStr;
   in >> commandStr;
   std::stringstream ss(commandStr);
   char ch;
   ss >> ch;
   if (notInteractive) out << commandStr << "\n";  // echo what was read
   Commands cmd = INVALID;
   if (commands.find(ch) != commands.end()) cmd = commands[ch];
   return cmd;
}

// read from input stream in a single word and check if the first
// letter corresponds to 'y' or 'Y' (implying a yes)
// return true if the string read starts with 'y' or 'Y', false otherwise
// if notInteractive, the answer read is echoed out
bool yesAnswer(istream &in, bool notInteractive, ostream& out) {
   string answerStr;
   in >> answerStr;
   if (notInteractive) out << answerStr << "\n";
   return (answerStr[0] == 'Y' || answerStr[0] == 'y');
}

// reset the input stream (possibly out of its fail state) and clear the buffer
// postcondition:
//    in is ready to read again
void clearClean(istream &in) {
   static const int MAX_CHARACTERS_IN_BUFFER = 250;
   in.clear();
   in.ignore(MAX_CHARACTERS_IN_BUFFER, '\n');
}

// consume the end-of-line of the previous input stream in
// read a line using in and output it to out if notInteractive
void getLineOutputLine(istream& in, bool notInteractive, ostream& out) {
   string line;
   std::getline(in, line); // consume the end-of-line
   std::getline(in, line);
   if (notInteractive) out << line << "\n";
}