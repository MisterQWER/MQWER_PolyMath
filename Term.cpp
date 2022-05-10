// Author: Gladys Monagan 
// A univariate polynomial term in x
// Course: CPSC 2150
// Copyright 2021

#include "Term.h"
#include <iostream> // std::ostream


Term::Term(int coefficient, int degree): coeff(coefficient), deg(degree) {
}

int Term::getCoeff() {
   return coeff;
}

int Term::getDegree() {
   return deg;
} 

void Term::setCoeff(int coefficient) {
   coeff = coefficient;
}

void Term::setDeg(int degree) {
   deg = degree;
}

std::ostream& operator << (std::ostream& out, const Term& t) {
   int c = t.coeff;
   if (c > 0) {
      out << " + ";
   }
   else if (c < 0) {
      out << " - ";
      c *= -1;
   }
   if (c != 1 || t.deg == 0) {
      out << c;
   }
   if (t.deg == 1) {
      out << "x";
   }
   else if (t.deg != 0) {
      out << "x^" << t.deg;
   }
   return out;
}

