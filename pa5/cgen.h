#include <assert.h>
#include <stdio.h>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"
#include <vector>

enum Basicness     {Basic, NotBasic};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
   List<CgenNode> *nds;
   ostream& str;
   int stringclasstag;
   int intclasstag;
   int boolclasstag;


// The following methods emit code for
// constants and global declarations.

   void code_global_data();
   void code_global_text();
   void code_bools(int);
   void code_select_gc();
   void code_constants();
   void code_nameTab(ostream &);
   void code_dispatchTab(ostream &);
   void code_parentTab(ostream &);

   void code_prototypeObjs(ostream &);
   void code_classInits(ostream &);
   void code_classMethods(ostream &);
   void code_objTab(ostream &);

// The following creates an inheritance graph from
// a list of classes.  The graph is implemented as
// a tree of `CgenNode', and class names are placed
// in the base class symbol table.

   void install_basic_classes();
   void install_class(CgenNodeP nd);
   void install_classes(Classes cs);
   void build_inheritance_tree();
   void set_relations(CgenNodeP nd);
public:
   CgenClassTable(Classes, ostream& str);
   void code();
   CgenNodeP root();
};


class CgenNode : public class__class {
private: 
   CgenNodeP parentnd;                        // Parent of class
   List<CgenNode> *children;                  // Children of class
   Basicness basic_status;                    // `Basic' if class is basic
                                              // `NotBasic' otherwise

public:
   CgenNode(Class_ c,
            Basicness bstatus,
            CgenClassTableP class_table);

   void add_child(CgenNodeP child);
   List<CgenNode> *get_children() { return children; }
   void set_parentnd(CgenNodeP p);
   CgenNodeP get_parentnd() { return parentnd; }
   int basic() { return (basic_status == Basic); }
   void dispatch_def(ostream& );
   void prototype(ostream &);
   void classInit(ostream &);
   std::vector<Attr>* getClassAttrs();
   int getAttrIndex(Symbol);
   std::vector<method_class*>* getMethods();
   void code_method(method_class*, ostream&);
   //CgenNodeP get_parentnd(){return parentnd;}
};

class BoolConst 
{
 private: 
  int val;
 public:
  BoolConst(int);
  void code_def(ostream&, int boolclasstag);
  void code_ref(ostream&) const;
};
class Environment{
public:
	Environment(CgenNode* classPtr):classPtr(classPtr){
		enterscope();}
	CgenNode* classPtr;
	Symbol get_classname(){ return classPtr->name;}
	std::vector<std::vector<std::pair<Symbol,int>>> paramlist;
	void enterscope(){
		std::vector<std::pair<Symbol,int>> littleVec;
		paramlist.push_back(littleVec);
	}
	void exitscope(){
		paramlist.pop_back();
	}
	int sp_fp; //  every push causes fp_index to minus 1, fp_index = fp-sp whenever push a local variable onto stack, record -sp_fp+1 as its location.
	void add_param(Symbol name, int i){
		paramlist[paramlist.size()-1].push_back({name,i});
	}
	int find_param(Symbol name){
		for (auto it=paramlist.rbegin();it!=paramlist.rend();it++){
			for (auto jt=it->rbegin();jt!=it->rend();jt++){
				if (jt->first==name){
					return jt->second;
				}
			}
		}
		return -1;
	}
	int find_attr(Symbol name){
		auto ptr = classPtr;
		std::vector<Attr> attrsList;
		while(ptr){
			for (int i=ptr->features->len()-1;i>=0;i--){
				auto curFeature = ptr->features->nth(i);
				auto attr = curFeature->get_attr();
				if (attr)
					attrsList.push_back(attr);
			}
			ptr = ptr->get_parentnd();
		}
		int c = 0;
		for (auto it = attrsList.rbegin(); it!=attrsList.rend();it++){
			if ((*it)->name==name){
				return c;
			}
			c++;
		}
		return -1;
	}
};
