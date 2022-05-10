// @Author: Parham Khadem

// Represents a univariate polynomial with a binary search tree
// Course: CPSC 2150

#include <iostream>
#include <functional>   // std::function
#include <math.h>
#include "Term.h"
#include "Univariate.h"




// create the zero polynomial 
Univariate::Univariate(std::function<bool(int, int)> fct): cmpFct(fct)  {

   root = nullptr;
}




// precondition:
//    c is the coefficient of the only term in the polynomial
//    d is the degree of the only term in the polynomial
// postcondition: 
//    a Univariate with one term c x^d is created
Univariate::Univariate(int c, int d, std::function<bool(int, int)> fct) :
   cmpFct(fct) {

      Term* new_term = new Term{c, d};
      root = new Node{nullptr, new_term, nullptr};
}





Univariate::Node* Univariate::copyTree(const Node* root)  {

   if(root == nullptr)  {
      return nullptr;
   }
   Term* term = new Term{root->term->getCoeff(), root->term->getDegree()};
   Node* newNode = new Node{nullptr, term, nullptr};
   newNode->left = copyTree(root->left);
   newNode->right = copyTree(root->right);
   return newNode;
}






// precondition:
//    p is the Univariate we are copying from
// postcondition: 
//    a Univariate which is a copy of p is created
Univariate::Univariate(const Univariate& p) {
   
   cmpFct = p.cmpFct;
   if(p.root->term != nullptr)     {
      root = copyTree(p.root);
   }
   else{
      root = new Node{nullptr, nullptr, nullptr};
   }
}






// postcondition: 
//    returns the highest degree of the polynomial
//    in the special case of the zero polynomial, return -1 
int Univariate::degree() const {
   Node* temp = root;
   if(temp == nullptr) return 0;

   while(temp->right != nullptr) {
      temp = temp->right;
   }
   return temp->term->getDegree();
}






// postcondition: 
//    returns the coefficient of term that has degree n
int Univariate::coefficient(int n) const {

   Node* temp = root;

   while(true)  {

      if(temp == nullptr)  return 0;
      if(temp->term->getDegree() == n)       return temp->term->getCoeff();
      else if(temp->term->getDegree() > n)   temp = temp->left;
      else if(temp->term->getDegree() < n)    temp = temp->right;
   }
}






double Univariate::eval(const Node* root, double x) {
   
   if(root == nullptr)  return 0;

   double value = root->term->getCoeff() * pow(x, root->term->getDegree());
   return value + eval(root->left, x) + eval(root->right, x);
}






// postcondition: 
//    returns the value of evaluating the polynomial at x
double Univariate::evaluate(double x) const {

   return eval(root, x);
}






// determine if the polynomial is the zero polynomial
bool Univariate::isZeroPolynomial() const { 

   return (root->term == nullptr) ?  true:  false;

}


void Univariate::deleter(Univariate::Node*& root)  {

   if(root->left == nullptr && root->right == nullptr)   {
      delete root;
      root = nullptr;
   }

   else if(root->left != nullptr || root->right != nullptr) {
      Node* temp = root;
      if(root->left !=nullptr)   root = root->left;
      else                       root = root->right;
      delete temp;
   }
   else{
      Node* successer = root;
      successer = successer->left;
      while(successer->right != nullptr)  {
         successer = successer->right;
      }
      root->term->setCoeff(successer->term->getCoeff());
      root->term->setDeg(successer->term->getDegree());
      deleter(successer);      
   }
}




void Univariate::insertHelper(Univariate::Node*& root, Term* t) {
   if(root == nullptr)  {
      root = new Node{nullptr, t, nullptr};
   }
   else if(root->term->getDegree() == t->getDegree()) {
      root->term->setCoeff(root->term->getCoeff() + t->getCoeff());
      if(root->term->getCoeff() == 0)  deleter(root);
   }
   else if(t->getDegree() > root->term->getDegree())   {
      insertHelper(root->right, t);
   }
   else  {
      insertHelper(root->left, t);
   }
}






// precondition:
//   the term pointed to by t has been allocated 
//   t != nullptr
// postcondition: 
//    
void Univariate::insert(Term* t) {

   insertHelper(root, t);
}






int Univariate::numOfTerms(const Node* root) {

   if(root == nullptr)  return 0;
   return 1 + numOfTerms(root->left) + numOfTerms(root->right);
}






// postcondition: 
//    returns the number of terms in the tree
//    in the special case of the zero polynomial return 1
int Univariate::numberOfTerms() const {

   if(root == nullptr)  return 1;
   return numOfTerms(root);
}







Univariate::Node* Univariate::adderHelper(Term* term, Univariate::Node* root)  {
   if(root == nullptr)  {
      Term* t = new Term{term->getCoeff(), term->getDegree()};
      Node* newNode = new Node{nullptr, t, nullptr};
      return newNode;
   }
   if(root->term->getDegree() == term->getDegree())   {
      root->term->setCoeff(root->term->getCoeff() + term->getCoeff());
   }
   else if(term->getDegree() > root->term->getDegree())  {
      root->right = adderHelper(term, root->right); 
   }
   else{
      root->left = adderHelper(term, root->left);
   }
   return root;
}







// each element of the other univariate will be added to the root univariate
void Univariate::adder(Univariate::Node* root, const Univariate::Node* other)  {
   if(other == nullptr)  return;
   //the next line will add a single term in the other node to the root Univariate tree
   root = adderHelper(other->term, root);
   adder(root, other->left);
   adder(root, other->right);
}







// precondition:
//    a and b are the two polynomials to sum up,
// postcondition: 
//    create a new polynomial which is the sum of two given polynomials
//    only the non-zero terms will be stored in the sum polynomial 
const Univariate Univariate::operator + (const Univariate& b) {
   Univariate u([](int a, int b) -> bool {return a > b;});
   if(b.isZeroPolynomial()) {
      u = *this;
   }  
   else if(isZeroPolynomial()) {
      u = b;
   }
   else {
      u = *this;
      adder(u.root, b.root);
   }
   return u;
}






void Univariate::derivative(Univariate::Node*& root)   {
   if(root == nullptr) return;
   else if(root->term->getDegree() == 0) {
      delete root;
      root = nullptr;
   }
   else{
      root->term->setCoeff(root->term->getCoeff() * root->term->getDegree());
      root->term->setDeg(root->term->getDegree() - 1);
      derivative(root->left);
      derivative(root->right);
   }
}




// differentiate
// postcondition:
//    returns a new polynomial
const Univariate Univariate::differentiate() const {

   Univariate dx([](int a, int b) -> bool {return a > b;});
   dx = *this;
   if(dx.root != nullptr)  {
      derivative(dx.root);
   }
   return dx;
}





Univariate::Node* Univariate::destroyTree(Univariate::Node* root) {
   if(root == nullptr)  return nullptr;
   root->left = destroyTree(root->left);
   root->right = destroyTree(root->right);
   delete root;
   return root;
}

// destructor frees up the memory 
Univariate::~Univariate() {

   root = destroyTree(root);
}






// postcondition:
//    deep copy of the rtSide was made      
Univariate& Univariate::operator = (const Univariate& rtSide) {
   root = destroyTree(root);
   root = copyTree(rtSide.root);
   return *this;
}




// postcondition: 
//    if non numeric input is given, the istream enters a fail state
//    only non-zero terms will be inserted into the polynomial
std::istream& operator >> (std::istream& in, Univariate& poly) {
   int n;
   // input the number of non-zero terms n of the polynomial: ";
   in >> n;
   //get the degree and coefficient of every term
   for (int i = 0; i < n; i++) {

      int coeff, deg;
      in >> coeff >> deg;
      
      if(coeff != 0)      {
         Term* term = new Term{coeff, deg};
         
         poly.insert(term);
      }
   }
   return in;
}




void Univariate::display(std::ostream& out, const Univariate::Node* root) {
   if(root == nullptr)  {
      return;
   }
   display(out, root->right);
   out << *(root->term);
   display(out, root->left);   
}



// postcondition:     
//    outputs the polynomial, starting with the highest degree
std::ostream& operator << (std::ostream& out, const Univariate& p) {

   if(p.root != nullptr) Univariate::display(out, p.root);
   else out << 0;  
   
   return out;
}
