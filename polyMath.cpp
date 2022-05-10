// Program to use (test) univariate polynomials with a user interface. 
// Commands supported 
// -> input two polynomials
// -> calculate their sum
// -> get the coefficients of some terms
// -> get the degree of the polynomial
// -> evaluate the polynomial given a specific value for x.
// -> get the number of terms in the polynomial
// -> possibly differentiate
// usage: polyMath -batch < inFile.txt > outFile.txt
// to run program non-interactively obtaining data from inFile.txt
// putting the output into outFile.txt
//
// usage: polyMath
// renders an interactive session with commands

// Author: Gladys Monagan (with material from Jian hua Pan)
// This code is copyrighted.

#include "Univariate.h"
#include "Commands.h"
#include <functional> // std::greater
#include <map>        // std::map
#include <iostream>   // std::cout std::cin
#include <string>     // std::string
using std::istream, std::ostream, std::cout;
using std::string;


// output a polynomial with its label
// precondition:
//    the operator << has been overloaded for Univariate
void printPoly(const string& label, const Univariate& poly, 
      ostream& out = cout) {
   out << label << " = " << poly << "\n";
}

// output the polynomial's degree with its label
void printDegree(const string& label, const Univariate& poly,
      ostream& out = cout) {
   out << "degree of " << label << " = " << poly.degree() << "\n";
}

// output the polynomials a and b
// precondition:
//    aPoly, bPoly are bona fide polynomials 
//    (including the zero polynomial)
void printPolys(const Univariate& aPoly, const Univariate& bPoly,
      ostream& out = cout) {
   printPoly("a", aPoly, out);
   printPoly("b", bPoly, out);
   printDegree("a", aPoly);
   printDegree("b", bPoly);
}

// differentiate and print the differentiated polynomials
void differentiate(const Univariate& aPoly, const Univariate& bPoly,
      ostream& out = cout) {
   Univariate aDPoly = aPoly.differentiate();
   Univariate bDPoly = bPoly.differentiate();
   printPoly("da/dx", aDPoly, out);
   printPoly("db/dx", bDPoly, out);
}

// print the coefficient and a blurb
void printCoeffDegree(string label, int coefficient, int degree,
      ostream& out = cout) {
   out << "in " << label << ", " << coefficient;
   out << " is the coefficient of the term of degree " << degree << "\n";
}

// print the polynomials a, b and their sum 
// precondition:
//    aPoly, bPoly and sumPoly are bona fide polynomials 
//    (including the zero polynomial)
void printPolys(const Univariate& aPoly, const Univariate& bPoly, 
      const Univariate& sumPoly, ostream& out = cout) {
   printPoly("a", aPoly, out);
   printPoly("b", bPoly, out);
   printPoly("a+b", sumPoly, out);
   printDegree("a", aPoly, out);
   printDegree("b", bPoly, out);
   printDegree("a+b", sumPoly, out);
}

// output the number of terms of the aPoly, bPoly and sumPoly if validSum
// validSum means that sumPoly is a valid polynomial
// precondition:
//    aPoly, bPoly, sumPoly are bona fide polynomials 
//    (including the zero polynomial)
void numberOfTerms(const Univariate& aPoly, const Univariate& bPoly, 
      const Univariate& sumPoly, bool validSum,ostream& out = cout) {
   out << aPoly.numberOfTerms() << " term(s) in a = " << aPoly << "\n";
   out << bPoly.numberOfTerms() << " term(s) in b = " << bPoly << "\n";
   if (validSum) {
      out << sumPoly.numberOfTerms();
      out << " term(s) in sum = " << sumPoly<<"\n";
   }
}

// read in the info on the first polynomial (polynomial a)
// if successful, read in the info on the second polynomial (polynomial b)
// expected for each polynomial is
// -> the number of terms
// -> pairs of numbers corresponding to the coefficient of the term 
//    and the degree
// precondition:
//    aPoly, bPoly, sumPoly are the zero polynomials
//    the operator << has been defined for Univariate
//    reading into a polynomial consists of
//        an integer n for the number of terms
//        coefficient degree pairs n times    
// postcondition: 
//    aPoly, bPoly are bona fide polynomials 
//    the input stream in is cleared in case of having read 
//    non-numeric input
bool inputUnivariates(std::istream& in, 
      Univariate& aPoly, Univariate& bPoly, ostream& out = cout) {
   bool successfulRead = true;
   out << "for polynomial a, enter # of terms,";
   out << " then \"coefficient degree\" pairs:\n";
   if (!(in >> aPoly)) {
      out << "ERROR in reading polynomial a\n";
      successfulRead = false;
   }
   if (successfulRead) {
      out << "for polynomial b, enter # of terms,";
      out << " then \"coefficient degree\" pairs:\n";
      if (!(in >> bPoly)) {
         out << "ERROR in reading polynomial b\n";
         successfulRead = false;
      }
   }
   clearClean(in);
   return successfulRead;
}

// read in the degree of the term whose coefficients will be printed
// for aPoly, bPoly and sumPoly (if validSum is true)
// if validSum is true, that means that sumPoly is valid
// if notInteractive is true, the degree read is echoed out
// postcondition:
//    the input stream in is cleared in case of having read 
//    non-numeric input
void getCoeffs(istream &in, const Univariate& aPoly, const Univariate& bPoly, 
      const Univariate& sumPoly, bool validSum, bool notInteractive,
      ostream& out = cout) {
   do {
      int degree;
      out << "enter degree of polynomial term whose coefficient is needed ";
      in >> degree;
      if (notInteractive) out << degree << "\n";
      printCoeffDegree("a", aPoly.coefficient(degree), degree);
      printCoeffDegree("b", bPoly.coefficient(degree), degree);
      if (validSum) printCoeffDegree("a+b", sumPoly.coefficient(degree), degree);
      out << "continue getting coefficients? (y/n): ";
   } while (yesAnswer(in, notInteractive, out));
   clearClean(in);
}

// read in the value of x to use in evaluating the polynomials
// evaluate aPoly, bPoly and sumPoly (if validSum is true) 
// at the given value of x and output the results
// if validSum is true, that means that sumPoly is valid
// if notInteractive is true, the degree read is echoed out
// postcondition:
//    the input stream in is cleared in case of having read 
//    non-numeric input
void evalPolys(istream &in, const Univariate& aPoly, const Univariate& bPoly, 
               const Univariate& sumPoly, bool validSum, bool notInteractive,
               ostream& out = cout) {
   do {
      double x;
      out << "enter the value of x: ";
      in >> x;
      if (notInteractive) out << x << "\n";
      out << "when x = " << x << "\n";
      out << "a = " << aPoly << " = " << aPoly.evaluate(x) << "\n";
      out << "b = " << bPoly << " = " << bPoly.evaluate(x) << "\n";
      if (validSum) out << "a+b = " << sumPoly << " = " << sumPoly.evaluate(x) << "\n";
      out << "evaluate for another value of x? (y/n) : ";
   } while (yesAnswer(in, notInteractive, out));
   clearClean(in);
}

// process the command 
//   read in aPoly and bPoly 
//   compute the polynomial which is the sum of aPoly and bPoly
//   print aPoly, bPoly and sumPoly
//   evaluate aPoly, bPoly and sumPoly asking the user for the value of x
//   get the coefficients of terms in aPoly, bPoly and sumPoly 
//   get the number of terms in aPoly, bPoly and sumPoly
//   print if an error was made
//   read a comment (or document) and echo it if notInteractive
// postcondition:
//   return true when the command is RESET or QUIT, returns false otherwise
//   validSum is set to true if the command SUM is executed
// 
bool process(istream& in, Commands command, Univariate& aPoly, 
      Univariate& bPoly, Univariate& sumPoly, bool& validSum,
      bool notInteractive, ostream& out = cout) {
   bool doneProcessing = false;
   switch (command) {
      case INPUT:
         // using the overloaded operator << of Univariate to enter info
         if (inputUnivariates(in, aPoly, bPoly, out)) 
            printPolys(aPoly, bPoly, out);
         break;
      case SUM:
         validSum = true;
         sumPoly = aPoly + bPoly;
         printPolys(aPoly, bPoly, sumPoly, out);
         break;
      case PRINT:
         validSum ? printPolys(aPoly, bPoly, sumPoly, out) : 
                     printPolys(aPoly,bPoly, out);
         break;
      case EVALUATE:
         // ask the user for the values of x and evaluate the polynomials
         evalPolys(in, aPoly, bPoly, sumPoly, validSum, notInteractive, out);
         break;
      case GET_COEFF:
         // ask for the degree of the term whose coefficient will be returned
         getCoeffs(in, aPoly, bPoly, sumPoly, validSum, notInteractive, out);
         break;
      case NUMBER_OF_TERMS:
         // output the number of terms of each polynomial
         numberOfTerms(aPoly, bPoly, sumPoly, validSum, out);
         break;
      case RESET:
         // will be used to restart the process eliminating the polynomials
         doneProcessing = true;
         break;
      case MENTION:
         getLineOutputLine(in, notInteractive, out);
         break;
      case DIFFERENTIATE:
         differentiate(aPoly, bPoly, out);
         break;
      case QUIT:
         // the task is done
         doneProcessing = true;
         break;
      case INVALID:
         out << "invalid command, please input again!\n";
         break;
      default:
         std::cerr << "**** ERROR in the switch of process ***\n";
         break;
   }
   return doneProcessing;
}

// determine if the "-batch" option was entered meaning that the
// session is NOT interactive
// return true if the string matches -batch, false otherwise
bool notInteractiveOption(string option) {
   return option == "-batch";
} 

// build the tree so that the degree is in descending order
struct MyFunctor {
   bool operator() (int a, int b) const {
      return a > b;
   }
};

// if the session is interactive, echo the input
// for each pair of polynomials read, process the command and
// finish once the command is quit
// right now input is from std::cin and output from std::out
int main(int argc, char* argv[]) {
   bool notInteractive = false;
   if (argc > 1) {
      notInteractive = notInteractiveOption(argv[1]);
   }
   std::map<char, Commands> commands;
   buildCommands(commands);

   istream& in = std::cin;
   ostream& out = std::cout;
   Commands cmd;
   do {
      if (!notInteractive) printCommandMenu(out);

      // process two polynomials (and their sum) 
      bool doneWithPolys = false;
      // in essence, we want the polynomial in descending order on degree 
      // when traversed
      // with a lambda function
      Univariate aPoly([](int a, int b) -> bool {return a > b;});
      // with a default STL function
      Univariate bPoly((std::greater<int>()));
      // with a functor or function object
      MyFunctor descending;
      Univariate sumPoly(descending); 
      bool validSum = false; // set to true when the command is SUM
      do {
         cmd = getCommand(commands, in, notInteractive, out);
         doneWithPolys = process(in, cmd, aPoly, bPoly, sumPoly, 
                                 validSum, notInteractive, out);
      } while (!doneWithPolys); // calls the destructors
      
   } while (cmd != QUIT); 
   return 0;
}
