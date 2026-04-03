#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>  
#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"
#include <map>
#include "ast-parse.h"
#include <vector>
#define TRUE 1
#define FALSE 0

class ClassTable;
typedef ClassTable *ClassTableP;
extern std::map<Symbol,std::map<Symbol,Method>> class2MethodsMap;

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.

class ClassTable {
private:
  int semant_errors;
  void install_basic_classes();
  ostream& error_stream;

public:
  //std::unordered_map<int,int> classMap;
  std::map<Symbol,Class_> classMap;
  ClassTable(Classes);
  int errors() { return semant_errors; }
  ostream& semant_error();
  ostream& semant_error(Class_ c);
  ostream& semant_error(Symbol filename, tree_node *t);
  void check();
  void checkMethodOverride(Class_,Symbol);
  void RecursiveCheck(Classes);
  bool TypeGT(Symbol,Symbol);
  bool conform(Symbol,Symbol);
  std::vector<Symbol>* getParents(Symbol);
  Symbol LeastCommonAncestor(Symbol,Symbol);
  Method findMethod(Symbol,Symbol);
};


#endif

