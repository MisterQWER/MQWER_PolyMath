// Author: Gladys Monagan 
// A univariate polynomial term in x
// Course: CPSC 2150
// Copyright 2021

#include <iostream> // std::ostream
#ifndef TERM_H
#define TERM_H
class Term {
   public:
      // create a term with the given coefficient and degree
      Term(int coefficient, int degree);

      // return the coefficient of the term
      int getCoeff();

      // return the degree of the term
      int getDegree(); 

      // set the coefficient of the term to be the given value
      void setCoeff(int coefficient);

      // set the degree of the term to be the given value
      void setDeg(int degree);

      // output the term nicely using the variable 'x'
      friend std::ostream& operator << (std::ostream&, const Term&);
   private:
      int coeff;
      int deg;
};
#endif
