

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"
#include <sstream>
#include <map>
#include <set>
#include <vector>



extern int semant_debug;
extern char *curr_filename;
static bool testing = false;
static std::ostringstream strout;
static ostream& log = testing ? std::cout : strout;
std::map<Symbol,std::map<Symbol,Method>> class2MethodsMap;
std::map<Symbol,std::map<Symbol,Attr>> class2AttrsMap;
static SymbolTable<Symbol,Symbol> *ObjectEnv;
static ClassTable* classtable;
static Class_ curClass;
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
static Symbol 
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class,
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    arg         = idtable.add_string("arg");
    arg2        = idtable.add_string("arg2");
    Bool        = idtable.add_string("Bool");
    concat      = idtable.add_string("concat");
    cool_abort  = idtable.add_string("abort");
    copy        = idtable.add_string("copy");
    Int         = idtable.add_string("Int");
    in_int      = idtable.add_string("in_int");
    in_string   = idtable.add_string("in_string");
    IO          = idtable.add_string("IO");
    length      = idtable.add_string("length");
    Main        = idtable.add_string("Main");
    main_meth   = idtable.add_string("main");
    //   _no_class is a symbol that can't be the name of any 
    //   user-defined class.
    No_class    = idtable.add_string("_no_class");
    No_type     = idtable.add_string("_no_type");
    Object      = idtable.add_string("Object");
    out_int     = idtable.add_string("out_int");
    out_string  = idtable.add_string("out_string");
    prim_slot   = idtable.add_string("_prim_slot");
    self        = idtable.add_string("self");
    SELF_TYPE   = idtable.add_string("SELF_TYPE");
    Str         = idtable.add_string("String");
    str_field   = idtable.add_string("_str_field");
    substr      = idtable.add_string("substr");
    type_name   = idtable.add_string("type_name");
    val         = idtable.add_string("_val");
}


/*
   * ClassTable will have an attribute- unorderd_map<symbol, class> classMap
   * each class has a list of methods. 
   * class2MethodsMap : unordered_map<class_name, std::vector<methods>>
   * for each feature, populate into ObjEnv or class2MethodsMap
   * ObjEnv represents the objectIdentifier mapping
   *
   * first 
   */
ClassTable::ClassTable(Classes classes) : semant_errors(0) , error_stream(cerr) {

    /* Fill this in */
	install_basic_classes();
	// construct the inheritance graph.
	for(int i=classes->first(); classes->more(i);i=classes->next(i)){
		Class_ temp = classes->nth(i);
		if (classMap.find(temp->getName())!=classMap.end()){
			semant_error(temp)<<"Class "<<temp->getName()<<" was previously defined."<<endl;return;
		}
		classMap.insert({temp->getName(),temp});
	}
	// SELF_TYPE cannot be used as class name. 
	if (classMap.find(SELF_TYPE)!=classMap.end()){
		semant_error(classMap.find(SELF_TYPE)->second) << "Redefinition of basic class SELF_TYPE."<<endl;	return;
	}

	// check Main
	if (classMap.find(Main)==classMap.end()){
		semant_error() << "Class Main is not defined."<<endl;	return;
	}
	// check acyclic. 
	for(auto it = classMap.begin(); it!=classMap.end(); ++it){
		// check its parent
		Class_ thisClass = it->second;
		std::set<Symbol> parentSet;
		Symbol parentSymbol = thisClass -> getParent();
		// cannot inherits SELF_TYPE
		if (parentSymbol == SELF_TYPE){
			semant_error(thisClass) << "Class "<<it->first<< " cannot inherit class SELF_TYPE." <<endl; break;
		}
		while(parentSymbol!=No_class){
			log << "checking "<<thisClass->getName()<<" "<<"parentSymbol:"<<parentSymbol->get_string()<<std::endl;
			if (parentSet.find(parentSymbol)!=parentSet.end()){
				semant_error(it->second)<< "acyclic"<<std::endl;
			}
			else if (parentSymbol==SELF_TYPE || parentSymbol==Int || parentSymbol==Str || parentSymbol==Bool){
				// std::cerr << "error inherit from basic class"<<std::endl;
				semant_error(it->second) << "Class " <<it->second->getName() <<" cannot inherit class "<<parentSymbol<<"."<<std::endl;return;
			}
			else {
				parentSet.insert(parentSymbol);
			}
			if (classMap.find(parentSymbol)==classMap.end()){
				//std::cerr << "undefined class"<<std::endl;
				semant_error(it->second) << "Class " <<it->second->getName() <<" inherits from an undefined class "<<parentSymbol<<"."<<std::endl;return;

			}
			thisClass = classMap.find(parentSymbol)->second;
			log << "code reached 01"<<std::endl;
			parentSymbol = thisClass -> getParent();
			log<<"code reached 02"<<std::endl;
		}
	}
	
		

}

void ClassTable::install_basic_classes() {

    // The tree package uses these globals to annotate the classes built below.
   // curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");
    
    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.
    
    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    // 
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.


    Class_ Object_class =
	class_(Object, 
	       No_class,
	       append_Features(
			       append_Features(
					       single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
					       single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
			       single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	       filename);

    // 
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    Class_ IO_class = 
	class_(IO, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       single_Features(method(out_string, single_Formals(formal(arg, Str)),
										      SELF_TYPE, no_expr())),
							       single_Features(method(out_int, single_Formals(formal(arg, Int)),
										      SELF_TYPE, no_expr()))),
					       single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
			       single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	       filename);  

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer. 
    //
    Class_ Int_class =
	class_(Int, 
	       Object,
	       single_Features(attr(val, prim_slot, no_expr())),
	       filename);

    //
    // Bool also has only the "val" slot.
    //
    Class_ Bool_class =
	class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename);

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //       
    Class_ Str_class =
	class_(Str, 
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       append_Features(
									       single_Features(attr(val, Int, no_expr())),
									       single_Features(attr(str_field, prim_slot, no_expr()))),
							       single_Features(method(length, nil_Formals(), Int, no_expr()))),
					       single_Features(method(concat, 
								      single_Formals(formal(arg, Str)),
								      Str, 
								      no_expr()))),
			       single_Features(method(substr, 
						      append_Formals(single_Formals(formal(arg, Int)), 
								     single_Formals(formal(arg2, Int))),
						      Str, 
						      no_expr()))),
	       filename);
	classMap.insert({Object,Object_class});
	classMap.insert({IO,IO_class});
	classMap.insert({Int,Int_class});
	classMap.insert({Str,Str_class});
	classMap.insert({Bool,Bool_class});

}

////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()                
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)  
//       print a line number and filename
//
///////////////////////////////////////////////////////////////////

ostream& ClassTable::semant_error(Class_ c)
{                                                             
    return semant_error(c->get_filename(),c);
}    


ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
{
    error_stream << filename << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream& ClassTable::semant_error()                  
{                                                 
    semant_errors++;                            
    return error_stream;
} 




void ClassTable::check(){
	// initialize class2MethodsMap
	for (auto it=classMap.begin();it!=classMap.end();it++){
		auto features = it->second->getFeatures();
		for (int i=features->first();features->more(i);i=features->next(i)) {
			Feature thisFeature = features->nth(i);
			// todo check duplicate method names.
			thisFeature->updateMethodsMap(it->second);
			thisFeature->updateAttrsMap(it->second);
		}
	}
	// print class2MethodsMap
	log << "class2MethodsMap:"<<endl;
	for (auto it=class2MethodsMap.begin();it!=class2MethodsMap.end();it++)
	{
		log << "\tclass " << it->first << ": " <<endl;
		auto methodsMap = it->second;
		for (auto jt=methodsMap.begin();jt!=methodsMap.end();jt++)
		{
			log << "\t\t" << jt->first <<endl;
			auto method = jt->second;
			// check parameter type, name
			auto formalsList = jt->second->getFormals();
			std::set<Symbol> formalNameSet;
			for (auto k = formalsList->first();formalsList->more(k);k=formalsList->next(k))
			{
				auto formal = formalsList->nth(k);
				if (formalNameSet.find(formal->getName())!=formalNameSet.end()) 
				{
					semant_error(classMap[it->first]) << "Formal parameter " << formal->getName() << " is multiply defined." << endl;
					continue;
				}
				formalNameSet.insert(formal->getName());	
				if (formal->getTypeDecl() == SELF_TYPE)
				{
					semant_error(classMap[it->first]) << "Formal parameter " << formal->getName() << " cannot have type SELF_TYPE." << endl; break;
				}
				if (formal->getName() == self)
				{
					semant_error(classMap[it->first]) << "'self' cannot be the name of a formal parameter." << endl; break;
				}
			}	
		}
	}
	log << "code reached 03"<<endl;
	// check method override legitimacy
	for (auto it=classMap.begin();it!=classMap.end();it++){
		Class_ thisClass = it->second;
		// skip 4 classes
		if (it->first==Object||it->first==Int||it->first==Str||it->first==Bool) continue;
		log << "checking class methods of class:"<<it->first<<endl;
		if (class2MethodsMap.find(thisClass->getName())==class2MethodsMap.end()) continue;
		auto methodMap= class2MethodsMap.find(thisClass->getName())->second;
		for (auto methodIt = methodMap.begin();methodIt!=methodMap.end();methodIt++){
			this->checkMethodOverride(thisClass,methodIt->first);
		}
	}
	// print class2AttrsMap
	log << "class2AttrsMap:"<<endl;
	for (auto it=class2AttrsMap.begin();it!=class2AttrsMap.end();it++)
	{
		log << "\tclass " << it->first << ": " <<endl;
		auto attrsMap = it->second;
		for (auto jt=attrsMap.begin();jt!=attrsMap.end();jt++)
		{
			log << "\t\t" << jt->first <<endl;
		}
	}
}
void ClassTable::checkMethodOverride(Class_ cls,Symbol methodName){
	auto parentClassName = cls->getParent();
	while(parentClassName!=No_class && class2MethodsMap[parentClassName].find(methodName)==class2MethodsMap[parentClassName].end()){
		parentClassName = classMap[parentClassName]->getParent();
	}
	if (parentClassName == No_class)
		return;
	Method thisMethod = class2MethodsMap[cls->getName()][methodName];
	Method parentMethod = class2MethodsMap[parentClassName][methodName];
	if (thisMethod->getFormals()->len()!=parentMethod->getFormals()->len()||thisMethod->getReturnType()!=parentMethod->getReturnType())
		semant_error() << cls->get_filename() << ":" <<thisMethod->get_line_number()<<": Incompatible number of formal parameters in redefined method " << thisMethod->getName() << "." << endl;
	else{
		auto i= thisMethod->getFormals()->first();
		while(thisMethod->getFormals()->more(i)){
			auto thisType = thisMethod->getFormals()->nth(i)->getTypeDecl();
			auto originType = parentMethod->getFormals()->nth(i)->getTypeDecl();
			if (thisType != originType) {
				log <<"class name is "<<cls->getName()<<endl;
				semant_error() << cls->get_filename()<<":" <<thisMethod->get_line_number()<<": In redefined method " << thisMethod->getName() << ", parameter type "<< thisType <<" is different from original type "<<originType<<endl;
				break;
			}
			i = thisMethod->getFormals()->next(i);
		}
	}
}

void method_class::updateMethodsMap(Class_ cls){
	//if (class2MethodsMap.find(cls->getName())!=class2MethodsMap.end() && class2MethodsMap[cls->getName()].find(name)!=class2MethodsMap[cls->getName()].end) return 1;
	// if (class2MethodsMap.find(cls->getName())!=class2MethodsMap.end()) return;
	class2MethodsMap[cls->getName()][name]=this;
}
void attr_class::updateMethodsMap(Class_ cls){}
void method_class::updateAttrsMap(Class_ cls){}
void attr_class::updateAttrsMap(Class_ cls){
	auto clsName = cls->getName();
	if (class2AttrsMap.find(clsName)!=class2AttrsMap.end() && class2AttrsMap[clsName].find(name)!=class2AttrsMap[clsName].end()){
		classtable->semant_error(cls) << "duplicate attr name" <<endl; return;
	}
	if (name == self){
		classtable->semant_error(cls) << "'self' cannot be the name of an attribute." << endl; return;
	}
	class2AttrsMap[cls->getName()][name] = this;
}

std::vector<Symbol>* ClassTable::getParents(Symbol className){
	log << "getParents of " << className<<endl;
	auto parents = new std::vector<Symbol>();
	while(className!=No_class){
		auto thisClass = classMap[className];
			parents->push_back(thisClass->getName());
			log << "\t" << thisClass->getName() ;
		className = thisClass->getParent();
	}
	log << endl;
	return parents;
}
/*
 * for every class, throw in all attrs.
 * starting from every features.
 * if it's an attr, start working on expr
 * it must check the type returned by expr.
 * if it's a method, start working on inner context.
 * it must check the return type.
   */
void ClassTable::RecursiveCheck(Classes classes){
	for (int i=classes->first();classes->more(i);i=classes->next(i))
	{
		auto thisClass = classes->nth(i);
		curClass = thisClass;
		log << "Recursive checking class " << thisClass->getName() <<endl;
		// throw in all attrs from all parents
		ObjectEnv->enterscope();
		auto parents = getParents(thisClass->getName());
		for (auto parentIt =parents->begin();parentIt!=parents->end();parentIt++)
		{	
			if (class2AttrsMap.find(*parentIt)==class2AttrsMap.end()) {
				continue;
			}
			auto attrMap = class2AttrsMap[*parentIt];
			for (auto attrIt = attrMap.begin(); attrIt!=attrMap.end(); attrIt++)
			{
				if (ObjectEnv->lookup(attrIt->first)!=NULL){
					semant_error(thisClass) << "attr redefinition" <<endl;
					continue;
				}
				ObjectEnv->addid(attrIt->first, new Symbol(attrIt->second->getTypeDecl()));
			}
		}
		log << "code reached 04"<<endl;

		// for every feature
		auto features = thisClass->getFeatures();
		for (int j=features->first();features->more(j);j=features->next(j))
		{
			features->nth(j)->RecursiveCheck();
		}

		log << "code reached 05"<<endl;


		ObjectEnv->exitscope();
	}		
}

void method_class::RecursiveCheck(){
	log << "--method--checking method "<< name << " of class "<< curClass->getName() << endl;
	// throw in formal parameters in ObjectEnv
	ObjectEnv->enterscope();
	ObjectEnv->addid(self,new Symbol(SELF_TYPE));
	for (int i=formals->first(); formals->more(i); i = formals->next(i))
	{
		auto formal = formals->nth(i);
		ObjectEnv->addid(formal->getName(), new Symbol(formal->getTypeDecl()));	
	}
	if (classtable->classMap.find(return_type)==classtable->classMap.end() && return_type!=SELF_TYPE){
		classtable->semant_error(curClass) << "Undefined return type " << return_type << " in method " << name <<"." << endl;
	}
	log << "code reached 14" << endl;
	auto exprRet = expr->RecursiveCheck();
	log << "code reached 15" << endl;
	// return type check
	if ((return_type==SELF_TYPE && !classtable->conform(exprRet,SELF_TYPE)) || (return_type!=SELF_TYPE && !classtable->conform(exprRet,return_type)))
	{ classtable->semant_error(curClass) << "Inferred return type " << exprRet << " of method " << name << " does not conform to declared return type " << return_type << "." <<endl; }
	ObjectEnv->exitscope();
}

void attr_class::RecursiveCheck(){
	log << "code reached 06" << endl;
	ObjectEnv->enterscope();
	ObjectEnv->addid(self, new Symbol(SELF_TYPE));
	auto ret = init->RecursiveCheck();
	ObjectEnv->exitscope();
	if (ret == NULL){ return;}
	log << "code reached 09" << endl;
	// type check
	if (!classtable->conform(ret,type_decl)) {
		classtable->semant_error(curClass) << "type not conform" << endl;
	}
	log << "code reached 07" <<endl;
}

Symbol dispatch_class::RecursiveCheck(){
	Symbol e0Type = expr->RecursiveCheck();
	Symbol dispatchClassName = e0Type==SELF_TYPE ? curClass->getName() : e0Type;
	Method thisMethod = NULL;
	if (classtable->classMap.find(dispatchClassName)==classtable->classMap.end()){
		classtable->semant_error(curClass) << "dispatch class not exist" << endl;
		return Object;
	} else {
		thisMethod = classtable->findMethod(dispatchClassName, name);
		if (thisMethod==NULL) {
			classtable->semant_error(curClass) << "dispatch method not exist" << endl;
			return Object;
		}
	}
	if (thisMethod->getFormals()->len() != actual->len()){
		classtable->semant_error(curClass) << "dispatch parameter number not match" << endl;
		return Object;
	}
	auto formals = thisMethod->getFormals();
	for (int i=formals->first();  formals->more(i); i=formals->next(i)){
		auto formal = formals->nth(i);
		auto expr = actual->nth(i);
		auto exprType = expr->RecursiveCheck();
		if (!classtable->conform(exprType,formal->getTypeDecl())){
			classtable->semant_error(curClass) << "dispatch parameter "<<exprType <<" doesn't match " << formal->getTypeDecl() << endl;
		}
	}
	type = thisMethod->getReturnType() == SELF_TYPE ? e0Type : thisMethod->getReturnType();
	return type;
}
Symbol static_dispatch_class::RecursiveCheck(){
	Symbol e0Type = expr->RecursiveCheck();
	if (!classtable->conform(e0Type,type_name)){
		classtable->semant_error(curClass) << "Expression type " << e0Type << " does not conform to declared static dispatch type " << type_name <<"."<<endl;
		return Object;
	}
	// Symbol dispatchClassName = e0Type==SELF_TYPE ? curClass->getName() : e0Type;
	Symbol dispatchClassName = type_name;
	Method thisMethod = NULL;
	if (classtable->classMap.find(dispatchClassName)==classtable->classMap.end()){
		classtable->semant_error(curClass) << "dispatch class not exist" << endl;
		return Object;
	} else {
		thisMethod = classtable->findMethod(dispatchClassName, name);
		if (thisMethod==NULL) {
			classtable->semant_error(curClass) << "dispatch method not exist" << endl;
			return Object;
		}
	}
	if (thisMethod->getFormals()->len() != actual->len()){
		classtable->semant_error(curClass) << "dispatch parameter number not match" << endl;
		return Object;
	}
	auto formals = thisMethod->getFormals();
	for (int i=formals->first();  formals->more(i); i=formals->next(i)){
		auto formal = formals->nth(i);
		auto expr = actual->nth(i);
		auto exprType = expr->RecursiveCheck();
		if (!classtable->conform(exprType,formal->getTypeDecl())){
			classtable->semant_error(curClass) << "dispatch parameter "<<exprType <<" doesn't match " << formal->getTypeDecl() << endl;
		}
	}
	type = thisMethod->getReturnType() == SELF_TYPE ? e0Type : thisMethod->getReturnType();
	return type;
}
Method ClassTable::findMethod(Symbol className, Symbol methodName){
	log << "getting method for class "<< className << " method: " << methodName << endl;
	if (className == SELF_TYPE)
		className = curClass->getName();
	auto parents = getParents(className);
	for (auto it=parents->begin();it!=parents->end();it++)
	{
		if (class2MethodsMap.find(*it)!=class2MethodsMap.end()){
			if (class2MethodsMap[*it].find(methodName)!=class2MethodsMap[*it].end()){
				return class2MethodsMap[*it][methodName];
			}
		}
	}
	return NULL;
}
	
// return type
Symbol object_class::RecursiveCheck(){
	log << "code reached 08" <<endl;
	// lookup name in ObjectEnv
	if (ObjectEnv->lookup(name)==NULL){
		log << "code reached 10" << endl;
		classtable->semant_error(curClass) << "Undeclared identifier " << name <<"." <<endl;
		type =  Object;		
	} else {
		log << "code reached 11" << endl;
	type =  *(ObjectEnv->lookup(name));
	}
	return type;
}

Symbol new__class::RecursiveCheck(){
	if (classtable->classMap.find(type_name)==classtable->classMap.end() && type_name!=SELF_TYPE){
		classtable->semant_error(curClass) << "'new' used with undefined class " << type_name <<"."<<endl;
	}
	/*if (type_name==SELF_TYPE)
		type = curClass->getName();
	else
		type = type_name;
		*/
	type = type_name;
	return type;
}

Symbol assign_class::RecursiveCheck(){
	if (name==self){
		classtable->semant_error(curClass) << "Cannot assign to 'self'." <<endl;
	}
	if (ObjectEnv->lookup(name)==NULL) {
		classtable->semant_error(curClass) << "Undeclared identifier " << name <<"." <<endl; return Object;
	}
	auto exprRet = expr->RecursiveCheck();
	auto nameType = *ObjectEnv->lookup(name);
	if (!classtable->conform(exprRet,nameType)){
		log << "code reached 18" <<endl;
		classtable->semant_error(curClass) << "Type " << exprRet <<" of assigned expression does not conform to declared type " << nameType << " of identifier " << name <<"." << endl;
	}
	type = exprRet;
   	return type;	
}

Symbol cond_class::RecursiveCheck(){
	auto predType = pred->RecursiveCheck();
	if (predType!=Bool) {
		classtable->semant_error(curClass) << "cond_class pred type error" << endl;}
	auto then_expType = then_exp->RecursiveCheck();
	auto else_expType = else_exp->RecursiveCheck();
	type = classtable->LeastCommonAncestor(then_expType,else_expType);
	return type;
}
Symbol loop_class::RecursiveCheck(){
	auto predType = pred->RecursiveCheck();
	if (predType!=Bool) {
		classtable->semant_error(curClass) << "Loop condition does not have type Bool" << endl;}
	body->RecursiveCheck();
	type = Object;
	return Object;
}


Symbol typcase_class::RecursiveCheck(){
	expr->RecursiveCheck();
	Symbol tempSymbol = NULL;
	std::set<Symbol> usedCaseType;
	for (int i=cases->first(); cases->more(i); i = cases->next(i))
	{
		auto thisCase = cases->nth(i);
		auto caseType = thisCase->getTypeDecl();
		if (usedCaseType.find(caseType)!=usedCaseType.end()){
			classtable->semant_error(curClass) << "Duplicate branch " << caseType <<" in case statements." << endl;
		} else {
			usedCaseType.insert(caseType);
		}
		auto retSymbol = thisCase->RecursiveCheck();
		if (i!=cases->first()){
			tempSymbol = classtable->LeastCommonAncestor(tempSymbol, retSymbol);
		} else {
			tempSymbol = retSymbol;	
		}
	}
	type = tempSymbol;
	return type;
}
Symbol branch_class::RecursiveCheck(){
	ObjectEnv->enterscope();
	ObjectEnv->addid(name, new Symbol(type_decl));
	auto retType =  expr->RecursiveCheck(); 	
	ObjectEnv->exitscope();
	return retType;
}
Symbol block_class::RecursiveCheck(){
	
	for (int i=body->first(); body->more(i); i = body->next(i))
	{
		auto thisExpr = body->nth(i);
		type = thisExpr->RecursiveCheck();
	}
	return type;	
}

Symbol let_class::RecursiveCheck(){
	if (identifier==self){
		classtable->semant_error(curClass) << "'self' cannot be bound in a 'let' expression."<<endl;
	}
	// Symbol idType = (type_decl == SELF_TYPE) ? curClass->getName() : type_decl;
	Symbol idType = type_decl;
	/*if (type_decl == SELF_TYPE){
		idType = curClass->getName();
	} else { */
	

	// if init expr exists, do conform test.
	auto initRet = init->RecursiveCheck();
	if (initRet != NULL && !classtable->conform(initRet,idType)){
		classtable->semant_error(curClass) << "let init expr doesn't conform" << endl;
	}
	ObjectEnv->enterscope();
	ObjectEnv->addid(identifier, new Symbol(idType));
	type = body->RecursiveCheck();
	ObjectEnv->exitscope();
	return type;
}

Symbol plus_class::RecursiveCheck(){
	auto e1Type = e1->RecursiveCheck();
	auto e2Type = e2->RecursiveCheck();
	if (e1Type!=Int || e2Type!=Int){
		classtable->semant_error(curClass) << "non-Int arguments: " << e1Type << " + " << e2Type << endl;
	}
	type = Int;
	return type;
}

Symbol sub_class::RecursiveCheck(){
	auto e1Type = e1->RecursiveCheck();
	auto e2Type = e2->RecursiveCheck();
	if (e1Type!=Int || e2Type!=Int){
		classtable->semant_error(curClass) << "non-Int arguments: " << e1Type << " - " << e2Type << endl;
	}
	type = Int;
	return type;
}
Symbol mul_class::RecursiveCheck(){
	auto e1Type = e1->RecursiveCheck();
	auto e2Type = e2->RecursiveCheck();
	if (e1Type!=Int || e2Type!=Int){
		classtable->semant_error(curClass) << "non-Int arguments: " << e1Type << " * " << e2Type << endl;
	}
	type = Int;
	return type;
}
Symbol divide_class::RecursiveCheck(){
	auto e1Type = e1->RecursiveCheck();
	auto e2Type = e2->RecursiveCheck();
	if (e1Type!=Int || e2Type!=Int){
		classtable->semant_error(curClass) << "non-Int arguments: " << e1Type << " / " << e2Type << endl;
	}
	type = Int;
	return type;
}
Symbol neg_class::RecursiveCheck(){
	auto e1Type = e1->RecursiveCheck();
	if (e1Type!=Int){
		classtable->semant_error(curClass) << "non-Int arguments: - " << e1Type << endl;
	}
	type = Int;
	return type;
}

Symbol lt_class::RecursiveCheck(){
	auto e1Type = e1->RecursiveCheck();
	auto e2Type = e2->RecursiveCheck();
	if (e1Type!=Int || e2Type!=Int){
		classtable->semant_error(curClass) << "non-Int arguments: " << e1Type << " < " << e2Type << endl;
	}
	type = Bool;
	return type;
}
Symbol leq_class::RecursiveCheck(){
	auto e1Type = e1->RecursiveCheck();
	auto e2Type = e2->RecursiveCheck();
	if (e1Type!=Int || e2Type!=Int){
		classtable->semant_error(curClass) << "non-Int arguments: " << e1Type << " <= " << e2Type << endl;
	}
	type = Bool;
	return type;
}
Symbol eq_class::RecursiveCheck(){
	auto e1Type = e1->RecursiveCheck();
	auto e2Type = e2->RecursiveCheck();
	 // if ((e1Type==Int || e1Type==Str || e1Type==Bool) && e1Type==e2Type){
	bool errorFlag = false; 
	if (e1Type==Int || e1Type==Str || e1Type==Bool || e2Type==Int || e2Type==Str || e2Type==Bool){
		if (e1Type!=e2Type) { 
			classtable->semant_error(curClass) << "wrong arguments for equal: " << e1Type << " = " << e2Type << endl;
		}
	}
	type = Bool;
	return type;
}
Symbol comp_class::RecursiveCheck(){
	auto e1Type = e1->RecursiveCheck();
	if (e1Type!=Bool){
		classtable->semant_error(curClass) << "non-Bool arguments for complement: " << e1Type << endl;
	}
	type = Bool;
	return type;
}
Symbol int_const_class::RecursiveCheck(){
	type = Int;
	return type;
}
Symbol bool_const_class::RecursiveCheck(){
	type = Bool;
	return type;
}
Symbol string_const_class::RecursiveCheck(){
	type = Str;
	return type;
}
Symbol isvoid_class::RecursiveCheck(){
	e1->RecursiveCheck();
	type = Bool;
	return type;
}
Symbol no_expr_class::RecursiveCheck() {
	type = NULL;
	return type;
}


// return true if classA greater than classB
bool ClassTable::TypeGT(Symbol A, Symbol B){
	log << "checking class " << A <<" GT " << B << endl;
	if (A==B || A == Object) return true;
	if (B == Object) return false;
	auto classB = classtable->classMap[B];
	auto parentName = classB->getParent();
	while (parentName!=A && parentName !=Object){
		auto parentCls = classtable->classMap[parentName];
		parentName = parentCls -> getParent();
	}
	if (parentName == Object)
	{
		return false;
	}
	return true;
}
// return true if classA conform to classB (less than, inherits from)
bool ClassTable::conform(Symbol A, Symbol B){
	log << "checking class " << A <<" conforms to/inherits from " << B << endl;
    if (B==SELF_TYPE && A!=SELF_TYPE)
		// B = curClass->getName();	
		return false;
	if (A==B || B == Object) return true;
	if (A==SELF_TYPE)
		A = curClass->getName();
	if (A==B || B == Object) return true;
	if (A == Object) return false;
	log << "code reached 19" << endl;
	auto classA = classtable->classMap[A];
	auto parentName = classA->getParent();
	log << "code reached 20" <<endl;
	while (parentName!=B && parentName !=Object){
		auto parentCls = classtable->classMap[parentName];
		parentName = parentCls -> getParent();
	}
	log << "code reached 21" << endl;
	if (parentName == Object)
	{
		return false;
	}
	return true;
}

Symbol ClassTable::LeastCommonAncestor(Symbol A, Symbol B){
	if (A==SELF_TYPE)
		A = curClass->getName();
	if (B==SELF_TYPE)
		B = curClass->getName();
	log << "checking leastCommonAncestor of "<<A <<" and " << B << endl;
	auto aParents = getParents(A);
	auto bParents = getParents(B);
	auto aIt = aParents->begin();
	auto bIt = bParents->begin();
	if (aParents->size()>bParents->size()){
		for (int i=0;i<(aParents->size()-bParents->size());i++) {
			aIt++;
		}
	} else {
		for (int i=0;i<(bParents->size()-aParents->size());i++) {
			bIt++;
		}
	}
	while (*aIt != *bIt)
	{
		aIt++;
		bIt++;
	}
	log << "Least common ancestor of " << A << " and " << B << " is: " << *aIt <<endl;
	return *aIt;
}
/*   This is the entry point to the semantic checker.

     Your checker should do the following two things:

     1) Check that the program is semantically correct
     2) Decorate the abstract syntax tree with type information
        by setting the `type' field in each Expression node.
        (see `tree.h')

     You are free to first do 1), make sure you catch all semantic
     errors. Part 2) can be done in a second stage, when you want
     to build mycoolc.
 */
void program_class::semant()
{
    initialize_constants();

    /* ClassTable constructor may do some semantic analysis */
    classtable = new ClassTable(classes);

    /* some semantic analysis code may go here */
	if (!classtable->errors())
	{
		classtable->check();
	}
	ObjectEnv = new SymbolTable<Symbol, Symbol>();
	// populate class2AttrsMap
	if (!classtable->errors()){
		classtable->RecursiveCheck(classes);
	}

    if (classtable->errors()) {
	cerr << "Compilation halted due to static semantic errors." << endl;
	exit(1);
    }
}
