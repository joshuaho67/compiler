
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;
static std::map<Symbol,int> classNameTagMap;
std::map<int, Symbol> classTagNameMap;
std::map<Symbol,CgenNode*> classMap;

int glabel = 0;
#define CLASS_PARENT_TAB "class_parentTab"
#define NO_CLASS_TAG -2
int testing=0;
std::string dummy;
std::stringstream ss(dummy);
ostream& log = testing? std::cout :ss; 

std::vector<std::pair<Symbol, Symbol>> getMethodTable(Symbol classname);
int getMethodIndex(Symbol classname, Symbol methodname);
//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
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

static char *gc_init_names[] =
  { "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
static char *gc_collect_names[] =
  { "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };


//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);
IntEntry* defaultInt;
StringEntry* defaultString;

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************

void program_class::cgen(ostream &os) 
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  CgenClassTable *codegen_classtable = new CgenClassTable(classes,os);

  os << "\n# end of generated code\n";
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_load(char *dest_reg, int offset, char *source_reg, ostream& s)
{
  s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")" 
    << endl;
}

static void emit_store(char *source_reg, int offset, char *dest_reg, ostream& s)
{
  s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
      << endl;
}

static void emit_load_imm(char *dest_reg, int val, ostream& s)
{ s << LI << dest_reg << " " << val << endl; }

static void emit_load_address(char *dest_reg, const char *address, ostream& s)
{ s << LA << dest_reg << " " << address << endl; }

static void emit_partial_load_address(char *dest_reg, ostream& s)
{ s << LA << dest_reg << " "; }

static void emit_load_bool(char *dest, const BoolConst& b, ostream& s)
{
  emit_partial_load_address(dest,s);
  b.code_ref(s);
  s << endl;
}

static void emit_load_string(char *dest, StringEntry *str, ostream& s)
{
  emit_partial_load_address(dest,s);
  str->code_ref(s);
  s << endl;
}

static void emit_load_int(char *dest, IntEntry *i, ostream& s)
{
  emit_partial_load_address(dest,s);
  i->code_ref(s);
  s << endl;
}

static void emit_move(char *dest_reg, char *source_reg, ostream& s)
{ s << MOVE << dest_reg << " " << source_reg << endl; }

static void emit_neg(char *dest, char *src1, ostream& s)
{ s << NEG << dest << " " << src1 << endl; }

static void emit_add(char *dest, char *src1, char *src2, ostream& s)
{ s << ADD << dest << " " << src1 << " " << src2 << endl; }

static void emit_addu(char *dest, char *src1, char *src2, ostream& s)
{ s << ADDU << dest << " " << src1 << " " << src2 << endl; }

static void emit_addiu(char *dest, char *src1, int imm, ostream& s)
{ s << ADDIU << dest << " " << src1 << " " << imm << endl; }

static void emit_div(char *dest, char *src1, char *src2, ostream& s)
{ s << DIV << dest << " " << src1 << " " << src2 << endl; }

static void emit_mul(char *dest, char *src1, char *src2, ostream& s)
{ s << MUL << dest << " " << src1 << " " << src2 << endl; }

static void emit_sub(char *dest, char *src1, char *src2, ostream& s)
{ s << SUB << dest << " " << src1 << " " << src2 << endl; }

static void emit_sll(char *dest, char *src1, int num, ostream& s)
{ s << SLL << dest << " " << src1 << " " << num << endl; }

static void emit_jalr(char *dest, ostream& s)
{ s << JALR << "\t" << dest << endl; }

static void emit_jal(const char *address,ostream &s)
{ s << JAL << address << endl; }

static void emit_return(ostream& s)
{ s << RET << endl; }

static void emit_gc_assign(ostream& s)
{ s << JAL << "_GenGC_Assign" << endl; }

static void emit_disptable_ref(Symbol sym, ostream& s)
{  s << sym << DISPTAB_SUFFIX; }

static void emit_init_ref(Symbol sym, ostream& s)
{ s << sym << CLASSINIT_SUFFIX; }

static void emit_label_ref(int l, ostream &s)
{ s << "label" << l; }

static void emit_protobj_ref(Symbol sym, ostream& s)
{ s << sym << PROTOBJ_SUFFIX; }

static void emit_method_ref(Symbol classname, Symbol methodname, ostream& s)
{ s << classname << METHOD_SEP << methodname; }

static void emit_label_def(int l, ostream &s)
{
  emit_label_ref(l,s);
  s << ":" << endl;
}

static void emit_beqz(char *source, int label, ostream &s)
{
  s << BEQZ << source << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_beq(char *src1, char *src2, int label, ostream &s)
{
  s << BEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bne(char *src1, char *src2, int label, ostream &s)
{
  s << BNE << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bleq(char *src1, char *src2, int label, ostream &s)
{
  s << BLEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blt(char *src1, char *src2, int label, ostream &s)
{
  s << BLT << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blti(char *src1, int imm, int label, ostream &s)
{
  s << BLT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bgti(char *src1, int imm, int label, ostream &s)
{
  s << BGT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_branch(int l, ostream& s)
{
  s << BRANCH;
  emit_label_ref(l,s);
  s << endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
static void emit_push(char *reg, ostream& str)
{
  emit_store(reg,0,SP,str);
  emit_addiu(SP,SP,-4,str);
}

//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source into the register dest
//
static void emit_fetch_int(char *dest, char *source, ostream& s)
{ emit_load(dest, DEFAULT_OBJFIELDS, source, s); }

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
static void emit_store_int(char *source, char *dest, ostream& s)
{ emit_store(source, DEFAULT_OBJFIELDS, dest, s); }


static void emit_test_collector(ostream &s)
{
  emit_push(ACC, s);
  emit_move(ACC, SP, s); // stack end
  emit_move(A1, ZERO, s); // allocate nothing
  s << JAL << gc_collect_names[cgen_Memmgr] << endl;
  emit_addiu(SP,SP,4,s);
  emit_load(ACC,0,SP,s);
}

static void emit_gc_check(char *source, ostream &s)
{
  if (source != (char*)A1) emit_move(A1, source, s);
  s << JAL << "_gc_check" << endl;
}


///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << STRCONST_PREFIX << index;
}

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream& s, int stringclasstag)
{
  IntEntryP lensym = inttable.add_int(len);

  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s  << LABEL                                             // label
      << WORD << stringclasstag << endl                                 // tag
      << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len+4)/4) << endl // size
      << WORD;


 /***** Add dispatch information for class String ******/
	  s << Str << DISPTAB_SUFFIX;
      s << endl;                                              // dispatch table
      s << WORD;  lensym->code_ref(s);  s << endl;            // string length
  emit_string_constant(s,str);                                // ascii string
  s << ALIGN;                                                 // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s, int stringclasstag)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,stringclasstag);
}

//
// Ints
//
void IntEntry::code_ref(ostream &s)
{
  s << INTCONST_PREFIX << index;
}

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream &s, int intclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                // label
      << WORD << intclasstag << endl                      // class tag
      << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl  // object size
      << WORD; 

 /***** Add dispatch information for class Int ******/
	  s << Int << DISPTAB_SUFFIX;
      s << endl;                                          // dispatch table
      s << WORD << str << endl;                           // integer value
}


//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream &s, int intclasstag)
{
  for (List<IntEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,intclasstag);
}


//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream& s) const
{
  s << BOOLCONST_PREFIX << val;
}
  
//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream& s, int boolclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                  // label
      << WORD << boolclasstag << endl                       // class tag
      << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl   // object size
      << WORD;

 /***** Add dispatch information for class Bool ******/
	  s << Bool << DISPTAB_SUFFIX;
      s << endl;                                            // dispatch table
      s << WORD << val << endl;                             // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//***************************************************
//
//  Emit code to start the .data segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_data()
{
  Symbol main    = idtable.lookup_string(MAINNAME);
  Symbol string  = idtable.lookup_string(STRINGNAME);
  Symbol integer = idtable.lookup_string(INTNAME);
  Symbol boolc   = idtable.lookup_string(BOOLNAME);

  str << "\t.data\n" << ALIGN;
  //
  // The following global names must be defined first.
  //
  str << GLOBAL << CLASSNAMETAB << endl;
  str << GLOBAL; emit_protobj_ref(main,str);    str << endl;
  str << GLOBAL; emit_protobj_ref(integer,str); str << endl;
  str << GLOBAL; emit_protobj_ref(string,str);  str << endl;
  str << GLOBAL; falsebool.code_ref(str);  str << endl;
  str << GLOBAL; truebool.code_ref(str);   str << endl;
  str << GLOBAL << INTTAG << endl;
  str << GLOBAL << BOOLTAG << endl;
  str << GLOBAL << STRINGTAG << endl;
  str << GLOBAL << CLASS_PARENT_TAB << endl;
  //
  // We also need to know the tag of the Int, String, and Bool classes
  // during code generation.
  //
  str << INTTAG << LABEL
      << WORD << intclasstag << endl;
  str << BOOLTAG << LABEL 
      << WORD << boolclasstag << endl;
  str << STRINGTAG << LABEL 
      << WORD << stringclasstag << endl;    
}


//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text()
{
  str << GLOBAL << HEAP_START << endl
      << HEAP_START << LABEL 
      << WORD << 0 << endl
      << "\t.text" << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Main"), str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Int"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("String"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Bool"),str);
  str << endl << GLOBAL;
  emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
  str << endl;
}

void CgenClassTable::code_bools(int boolclasstag)
{
  falsebool.code_def(str,boolclasstag);
  truebool.code_def(str,boolclasstag);
}

void CgenClassTable::code_select_gc()
{
  //
  // Generate GC choice constants (pointers to GC functions)
  //
  str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
  str << "_MemMgr_INITIALIZER:" << endl;
  str << WORD << gc_init_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
  str << "_MemMgr_COLLECTOR:" << endl;
  str << WORD << gc_collect_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_TEST" << endl;
  str << "_MemMgr_TEST:" << endl;
  str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
}


//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************

void CgenClassTable::code_constants()
{
  //
  // Add constants that are required by the code generator.
  //
  stringtable.add_string("");
  inttable.add_string("0");

  stringtable.code_string_table(str,stringclasstag);
  inttable.code_string_table(str,intclasstag);
  code_bools(boolclasstag);
}


CgenClassTable::CgenClassTable(Classes classes, ostream& s) : nds(NULL) , str(s)
{
   stringclasstag = 0 /* Change to your String class tag here */;
   intclasstag =    1 /* Change to your Int class tag here */;
   boolclasstag =   2 /* Change to your Bool class tag here */;

   defaultInt = inttable.add_int(0);
   defaultString = stringtable.add_string("");

   enterscope();
   if (cgen_debug) cout << "Building CgenClassTable" << endl;
   install_basic_classes();
   install_classes(classes);
  for(List<CgenNode> *l = nds; l; l = l->tl()){
	  CgenNode* thisClass = l->hd();
	  classMap.insert({thisClass->name, thisClass});
  }

   build_inheritance_tree();

   code();
   exitscope();
}

void CgenClassTable::install_basic_classes()
{

// The tree package uses these globals to annotate the classes built below.
  //curr_lineno  = 0;
  Symbol filename = stringtable.add_string("<basic class>");

//
// A few special class names are installed in the lookup table but not
// the class list.  Thus, these classes exist, but are not part of the
// inheritance hierarchy.
// No_class serves as the parent of Object and the other special classes.
// SELF_TYPE is the self class; it cannot be redefined or inherited.
// prim_slot is a class known to the code generator.
//
  addid(No_class,
	new CgenNode(class_(No_class,No_class,nil_Features(),filename),
			    Basic,this));
  addid(SELF_TYPE,
	new CgenNode(class_(SELF_TYPE,No_class,nil_Features(),filename),
			    Basic,this));
  addid(prim_slot,
	new CgenNode(class_(prim_slot,No_class,nil_Features(),filename),
			    Basic,this));

// 
// The Object class has no parent class. Its methods are
//        cool_abort() : Object    aborts the program
//        type_name() : Str        returns a string representation of class name
//        copy() : SELF_TYPE       returns a copy of the object
//
// There is no need for method bodies in the basic classes---these
// are already built in to the runtime system.
//
  install_class(
   new CgenNode(
    class_(Object, 
	   No_class,
	   append_Features(
           append_Features(
           single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
           single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
           single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	   filename),
    Basic,this));

// 
// The IO class inherits from Object. Its methods are
//        out_string(Str) : SELF_TYPE          writes a string to the output
//        out_int(Int) : SELF_TYPE               "    an int    "  "     "
//        in_string() : Str                    reads a string from the input
//        in_int() : Int                         "   an int     "  "     "
//
   install_class(
    new CgenNode(
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
	   filename),	    
    Basic,this));

//
// The Int class has no methods and only a single attribute, the
// "val" for the integer. 
//
   install_class(
    new CgenNode(
     class_(Int, 
	    Object,
            single_Features(attr(val, prim_slot, no_expr())),
	    filename),
     Basic,this));

//
// Bool also has only the "val" slot.
//
    install_class(
     new CgenNode(
      class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename),
      Basic,this));

//
// The class Str has a number of slots and operations:
//       val                                  ???
//       str_field                            the string itself
//       length() : Int                       length of the string
//       concat(arg: Str) : Str               string concatenation
//       substr(arg: Int, arg2: Int): Str     substring
//       
   install_class(
    new CgenNode(
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
	     filename),
        Basic,this));

}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_class(CgenNodeP nd)
{
  Symbol name = nd->get_name();

  if (probe(name))
    {
      return;
    }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds = new List<CgenNode>(nd,nds);
  addid(name,nd);
}

void CgenClassTable::install_classes(Classes cs)
{
  for(int i = cs->first(); cs->more(i); i = cs->next(i))
    install_class(new CgenNode(cs->nth(i),NotBasic,this));
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree()
{
  for(List<CgenNode> *l = nds; l; l = l->tl())
      set_relations(l->hd());
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNodeP nd)
{
  CgenNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n)
{
  children = new List<CgenNode>(n,children);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}



void CgenClassTable::code()
{
  if (cgen_debug) cout << "coding global data" << endl;
  code_global_data();

  if (cgen_debug) cout << "choosing gc" << endl;
  code_select_gc();

  if (cgen_debug) cout << "coding constants" << endl;
  code_constants();
  
  if (cgen_debug) cout << "coding class_nameTab" << endl;
  code_nameTab(str);

  if (cgen_debug) cout << "coding dispatchTab" << endl;
  code_dispatchTab(str);

  if (cgen_debug) cout << "coding parentTab" << endl;
  code_parentTab(str);
//                 Add your code to emit
//                   - prototype objects
//                   - class_nameTab
//                   - dispatch tables
//
  if (cgen_debug) cout << "coding prototype objects" << endl;
  code_prototypeObjs(str);


  if (cgen_debug) cout << "coding global text" << endl;
  code_global_text();

//                 Add your code to emit
//                   - object initializer
//                   - the class methods
//                   - etc...
  if (cgen_debug) cout << "coding object initializer" << endl;
  code_classInits(str);

  if (cgen_debug) cout << "coding class methods" << endl;
  code_classMethods(str);
  
  
  if (cgen_debug) cout << "coding class objTab" << endl;
  code_objTab(str);

}

void CgenClassTable::code_nameTab(ostream &s){
	s << CLASSNAMETAB << LABEL;	
	classNameTagMap.insert({Str,0});
	classNameTagMap.insert({Int,1});
	classNameTagMap.insert({Bool,2});

	classTagNameMap.insert({0,Str});
	classTagNameMap.insert({1,Int});
	classTagNameMap.insert({2,Bool});
	s << WORD; stringtable.lookup_string(Str->get_string())->code_ref(s); s << endl;
	s << WORD;stringtable.lookup_string(Int->get_string())->code_ref(s); s << endl;
	s << WORD;stringtable.lookup_string(Bool->get_string())->code_ref(s); s << endl;
	
	int c = 3; // start tag from 3
	for(auto *l = nds; l; l = l->tl()){
		auto curCgenNode = l->hd();
		auto curName = curCgenNode->get_name();
		if (curName!=Str && curName!=Int && curName!=Bool){
			classNameTagMap.insert({curName,c});
			classTagNameMap.insert({c, curName});
			c++;
			s << WORD;stringtable.lookup_string(curName->get_string())->code_ref(s); s << endl;
		}
	}
}

void CgenClassTable::code_objTab(ostream& s){
	s << CLASSOBJTAB << LABEL;
	int n = classTagNameMap.size();	
	for (int i=0;i<n;i++){
		Symbol name = classTagNameMap[i];
		s << WORD << name << PROTOBJ_SUFFIX << endl;
		s << WORD << name << CLASSINIT_SUFFIX << endl;
	}
		
}
void CgenClassTable::code_dispatchTab(ostream &s){
	// for class a, a.dispatchTab:
	for(List<CgenNode> *l = nds; l; l = l->tl()) {
		auto thisCgenNode = l->hd();
		thisCgenNode->dispatch_def(s);
	}
}

void class_dispatch_ref(){
}
/*void CgenNode::dispatch_def(ostream& s){
	s << name <<  DISPTAB_SUFFIX << LABEL;
	std::vector<std::pair<Symbol,Symbol>> classmethodList;
	std::set<Symbol> methodSet;	
	auto ptr = this;
	while(ptr){
		for (int i = ptr->features->len()-1; i >= 0 ; i--){
			auto curFeature = ptr->features->nth(i);
			if (!curFeature->is_method())
				continue;
			if (methodSet.find(curFeature->get_name())!=methodSet.end())
				continue;
			methodSet.insert(curFeature->get_name());
			classmethodList.push_back({ptr->get_name(),curFeature->get_name()});
		}
		ptr = ptr-> parentnd;
	}	
	for (auto it = classmethodList.rbegin(); it!=classmethodList.rend(); it++){
		s << WORD <<  it->first << METHOD_SEP << it->second << endl;
	}
}*/
void CgenNode::dispatch_def(ostream& s){
	s << name <<  DISPTAB_SUFFIX << LABEL;
	std::vector<std::pair<Symbol,Symbol>> classmethodTable = getMethodTable(name);
	for (auto it = classmethodTable.begin(); it!=classmethodTable.end(); it++){
		s << WORD <<  it->first << METHOD_SEP << it->second << endl;
	}
}
void CgenClassTable::code_parentTab(ostream& s){
	// create class_parentTab 
	// using classMap classNameTagMap classTagNameMap
	s << CLASS_PARENT_TAB << LABEL;
	// std::vector<std::pair<int, int>> classParentVec;
	int n = classMap.size();
	for(int i=0;i<n;i++){
		Symbol name = classTagNameMap[i];
		CgenNode* thisNode = classMap[name];
		Symbol parentName = thisNode->get_parent();
		int parenti = NO_CLASS_TAG;
		if (parentName!=No_class)
			parenti = classNameTagMap[parentName];
		s << WORD << parenti << endl;
	}
}


			

void CgenClassTable::code_prototypeObjs(ostream& s){
	for(List<CgenNode> *l = nds; l; l = l->tl()) {
		auto thisCgenNode = l->hd();
		thisCgenNode->prototype(s);
	}
}
void CgenNode::prototype(ostream& s){
	s << WORD << -1 << endl;
	s << name << PROTOBJ_SUFFIX << LABEL;
	s << WORD << classNameTagMap[name] << endl; // class tag
	std::vector<Attr> attrsList;
	auto ptr = this;
	while (ptr){
		for (int i = ptr->features->len()-1; i>=0;i--){
			auto curFeature = ptr->features->nth(i);
			if (curFeature->is_method())
				continue;
			auto attr = curFeature->get_attr();
			attrsList.push_back(attr);
		}
		ptr = ptr->parentnd;
	}
	int len = attrsList.size()+3;
	s << WORD << len << endl; // object size
	s << WORD << name << DISPTAB_SUFFIX << endl; // dispatch pointer
	for (auto it = attrsList.rbegin(); it!=attrsList.rend(); it++){
		auto type = (*it)->type_decl;
		if (type == Str) {
			s << WORD;
			defaultString->code_ref(s);
			s << endl;
		}
		else if (type == Int){
			s << WORD;
			defaultInt->code_ref(s);
			s << endl;
		} else if (type == Bool){
			s << WORD;
			falsebool.code_ref(s);
			s << endl;
		} else {
			s << WORD << 0 << endl;
		}	
	}
}
void CgenClassTable::code_classInits(ostream& s){
	for(List<CgenNode> *l = nds; l; l = l->tl()) {
		auto thisCgenNode = l->hd();
		thisCgenNode->classInit(s);
	}
}

/*
   input has acc, old_self

   acc should be used as the new self
   */
void CgenNode::classInit(ostream& s){
	s << name << CLASSINIT_SUFFIX << LABEL;
	emit_push(FP, s);
	emit_push(RA, s);
	emit_push(SELF, s); // save 

	emit_move(FP, SP, s);
	int sp_fp = 0;
	// emit_push(SELF, s);
	// emit_move(ACC,SELF);
	 emit_move(SELF, ACC, s); // env self object is changed.
	if(parentnd && parentnd->name!=No_class){
		s << JAL;
		emit_init_ref(parentnd->name, s);
		s << endl;
	}
	// for every attr, execute init expr
	std::vector<Attr>* attrs = getClassAttrs();
	for (auto it = attrs->begin(); it!=attrs->end();it++)
	{
		auto thisAttr = *it;
		if (!thisAttr->init->is_nil()) {
			Environment env(this);
			env.sp_fp = 0;
			thisAttr->init->code(s, env);
			int index = getAttrIndex(thisAttr->name);
			//emit_load(T1, 1, SP, s); // fetch self
			// emit_store(ACC, index+3, T1, s);
			 emit_store(ACC, index+3, SELF, s);
		}
	}
	// return new self
	emit_move(ACC, SELF, s);
	// restore old self
	emit_load(SELF, 1,SP,s);
	emit_load(RA,2,SP,s);
	emit_load(FP,3,SP,s);
	emit_addiu(SP,SP,12,s);
	emit_return(s);
}
std::vector<Attr>* CgenNode::getClassAttrs(){
	auto attrs = new std::vector<Attr>;
	for (int i=features->first();features->more(i); i=features->next(i)){
		auto thisFeature = features->nth(i);
		auto thisAttr = thisFeature->get_attr();
		if (thisAttr){
			attrs->push_back(thisAttr);
		}
	}
	return attrs;
}
int CgenNode::getAttrIndex(Symbol name){
	std::vector<Attr> attrList;
	auto ptr = this;
	while(ptr){
		for (int j=ptr->features->len()-1;j>=0;j--){
			auto thisFeature = ptr->features->nth(j);
			auto thisAttr = thisFeature->get_attr();
			if (thisAttr)
				attrList.push_back(thisAttr);
		}
		ptr = ptr->parentnd;
	}
	int i=0;
	for (auto it=attrList.rbegin();it!=attrList.rend();it++){
		if ((*it)->name == name)
			return i;
		i++;
	}
	return -1;
}

void CgenClassTable::code_classMethods(ostream& str){
	for(List<CgenNode> *l = nds; l; l = l->tl()) {
		auto thisCgenNode = l->hd();
		if (thisCgenNode->basic())
			continue;
		std::vector<method_class*>* methods = thisCgenNode->getMethods();
		for(auto it=methods->begin();it!=methods->end();it++){
			thisCgenNode->code_method(*it, str);
		}
	}
}
std::vector<method_class*>* CgenNode::getMethods(){
	auto methods = new std::vector<method_class*>;
	for(int i=features->first();features->more(i);i=features->next(i)){
		auto thisMethod = features->nth(i)->getMethod();
		if(thisMethod){
			methods->push_back(thisMethod);
		}
	}
	return methods;
}

/*
   
   */
void CgenNode::code_method(method_class* method, ostream& s){
	s << name << METHOD_SEP << method->get_name() << LABEL;
	emit_push(FP, s);
	emit_addiu(FP,SP,8,s); // fp points to first parameter.
	emit_push(RA, s);
	emit_push(SELF, s);

	emit_move(SELF, ACC, s); // after dispatch, method self is changed. CAUTION, acc cannot be 0.
	// add attr, param into env
	Environment env(this);
	env.sp_fp = 4; // fp->first para fp-1=>old_fp fp-2=>ra fp-3=>self
	for (int i=0;i<method->formals->len();i++){
		auto thisFormal = method->formals->nth(i);
		env.add_param(thisFormal->get_name(),method->formals->len()-1-i);
	}
	// todo
	(method->expr)->code(s, env);
	emit_load(SELF,-3,FP,s);
	emit_load(RA,-2,FP,s);
	emit_load(FP,-1,FP,s);
	// emit_addiu(SP,SP,12,s);
	emit_addiu(SP,SP,12+4*method->formals->len(),s); // pop all saved registers and parameters on the stack
	emit_return(s);
}	
CgenNodeP CgenClassTable::root()
{
   return probe(Object);
}


///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct) :
   class__class((const class__class &) *nd),
   parentnd(NULL),
   children(NULL),
   basic_status(bstatus)
{ 
   stringtable.add_string(name->get_string());          // Add class name to string table
}


//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

void assign_class::code(ostream &s, Environment env) {
	expr->code(s, env);
	int idx;
	if ((idx=env.find_param(name))!=-1){
		emit_store(ACC, idx, FP, s);
		return;
	}
	if ((idx=env.find_attr(name))!=-1){
		emit_store(ACC, idx+3 , SELF, s);
		return;
	}
	if (cgen_debug)
	cout<<"assign error, symbol not found."<<endl;

}

void static_dispatch_class::code(ostream &s, Environment env) {
	Symbol exprType;
	/*if (expr->get_type()==SELF_TYPE){
		exprType = env.get_classname();
	} else if (expr->get_type()==No_type){
		if (cgen_debug)
			cout << "dispatch on void" << endl;
	} else
		exprType = expr->get_type();
		*/
	// static dispatch
	exprType = type_name;

	// todo void check
	int label = glabel;
	glabel++;

	emit_load_imm(T1, 0, s);
	emit_bne(ACC, T1, label, s);

	emit_load_address(ACC, "str_const0", s);
	emit_load_imm(T1,1, s);

	emit_jal("_dispatch_abort", s);
	emit_label_def(label, s);
	

	int methodIndex = getMethodIndex(exprType,name);
	// evaluate and push parameters.	
	for (int i=0; i<actual->len(); i++){
		auto thisExpr = actual->nth(i);
		thisExpr->code(s, env);
		emit_push(ACC,s); // callee will pop all arguments.
		// env.sp_fp++;
	}
	//emit_move(ACC, SELF, s);
	expr->code(s, env);
	if (expr->is_nil())
		emit_move(ACC, SELF, s);
	// find method from specified class.
	emit_load_address(T1, CLASSOBJTAB, s);
	s << "#code reached 01" << endl;
	emit_load_imm(T2, classNameTagMap[type_name], s);
	emit_sll(T2, T2, 3, s);
	// find protobj addr in CLASSOBJTAB
	emit_add(T1, T1, T2, s);
	// find actual protobj addr
	emit_load(T1, 0, T1, s);
	// find virtualtable
	emit_load(T1, 2, T1, s);
	// find function address
	emit_load(T1, methodIndex, T1, s);
	emit_jalr(T1, s);


	// emit_load(T1, DISPTABLE_OFFSET, ACC, s);
	// emit_load(T2, methodIndex, T1, s);
	// emit_jalr(T2, s);
}

Symbol getClassWithMethod(Symbol classname, Symbol methodname){
	CgenNode* ptr = classMap[classname];
	while(ptr){
		for(int i=0;i<ptr->features->len();i++){
			auto thisFeature = ptr->features->nth(i);
			if(thisFeature->is_method() && thisFeature->get_name() == methodname)
				return ptr->name;
		}
		ptr = ptr->get_parentnd();
	}
}
std::vector<Symbol> getParents(Symbol classname){
	std::vector<Symbol> res;
	CgenNode* ptr = classMap[classname];
	while(ptr->name!=No_class){
		res.push_back(ptr->name);
		ptr = ptr->get_parentnd();
	}
	std::reverse(res.begin(), res.end());
	return res;
}

int getMethodIndex(Symbol classname, Symbol methodname){
	auto classMethodTable = getMethodTable(classname);
	for (int i=0;i<classMethodTable.size();i++){
		if (classMethodTable[i].second==methodname){
			return i;
		}
	}
	return -1;
}
std::vector<std::pair<Symbol, Symbol>> getMethodTable(Symbol classname){
	std::vector<std::pair<Symbol, Symbol>> classMethodTable;	
	auto parents = getParents(classname);
	log << "code reached 2" << endl;
	log << " classMap size "<<classMap.size() << endl;
	for (auto it=parents.begin();it!=parents.end();it++){
		log << "*it" << *it << endl;
		auto thisNode = classMap[*it];
		auto features = thisNode->features;
		auto curClassName = thisNode->name;
		for (int i=0;i<(features->len());i++){
			auto thisMethod = features->nth(i)->getMethod();
			if (!thisMethod)
				continue;
			auto curMethodName = thisMethod->name;
			bool exist_flag = false;
			for (auto jt=classMethodTable.begin();jt!=classMethodTable.end();jt++){
				if (jt->second == curMethodName){
					
					jt->first = curClassName;
					exist_flag = true;
				}
			}
			if (!exist_flag)
				classMethodTable.push_back({curClassName, curMethodName});
		}
	}
	return classMethodTable;
}
	
int getMethodIndex2(Symbol classname, Symbol methodname){
	CgenNode* ptr = classMap[classname];
	std::vector<method_class*> methodList;
	std::set<Symbol> methodSet;
	while(ptr){
		for(int i=ptr->features->len()-1;i>=0;i--){
			auto thisFeature = ptr->features->nth(i);
			auto thisMethod = thisFeature->getMethod();
			if (thisMethod){
				if (methodSet.find(thisMethod->get_name())!=methodSet.end())
					continue;

				methodList.push_back(thisMethod);
				methodSet.insert(thisMethod->get_name());
			}
		}
		ptr = ptr->get_parentnd();
	}
	int c=0;
	for(auto it=methodList.rbegin();it!=methodList.rend();it++){
		if((*it)->get_name()==methodname)
			return c;
		c++;
	}
	return -1;
}

void dispatch_class::code(ostream &s, Environment env) {
	Symbol exprType;
	if (expr->get_type()==SELF_TYPE){
		exprType = env.get_classname();
	} else if (expr->get_type()==No_type){
		if (cgen_debug)
			cout << "dispatch on void" << endl;
	} else
		exprType = expr->get_type();
	// todo void check
	

	int methodIndex = getMethodIndex(exprType,name);
	// evaluate and push parameters.	
	for (int i=0; i<actual->len(); i++){
		auto thisExpr = actual->nth(i);
		thisExpr->code(s, env);
		emit_push(ACC,s); // callee will pop all arguments.
		// env.sp_fp++;
	}
	//emit_move(ACC, SELF, s);
	expr->code(s, env);
	/*if (expr->is_nil())
		emit_move(ACC, SELF, s);
		*/
	int label = glabel;
	glabel++;
	s << "#code reached 02" << endl;
	emit_load_imm(T1, 0, s);
	emit_bne(ACC, T1, label, s);

	emit_load_address(ACC, "str_const0", s);
	emit_load_imm(T1,1, s);

	emit_jal("_dispatch_abort", s);
	emit_label_def(label, s);
	emit_load(T1, DISPTABLE_OFFSET, ACC, s);
	emit_load(T2, methodIndex, T1, s);
	emit_jalr(T2, s);
	
	//emit_addiu(SP, SP, 4*(actual->len()), s);
	// env.sp_fp -= (actual->len());
	 
	// emit_addiu(SP, SP, 4*(actual->len()),s);
	// wrong, should look up virtual function table.
/*	Symbol classWithMethod = getClassWithMethod(exprType,name);
	s << JAL << classWithMethod << METHOD_SEP << name << endl;
	*/
}

void cond_class::code(ostream &s, Environment env) {
	int label1 = glabel;
	int label2 = glabel+1;
	glabel+=2;
	pred->code(s, env);
	emit_load(T1, 3, ACC, s);
	s << "#code reached 03" << endl;
	emit_load_imm(T2, 0, s);
	emit_beq(T1, T2, label2, s);

	then_exp->code(s, env);
	emit_branch(label1, s);

	emit_label_def(label2, s);
	else_exp->code(s, env);
	emit_label_def(label1, s);
}

void loop_class::code(ostream &s, Environment env) {
	int label1 = glabel;
	int label2 = glabel+1;
	glabel+=2;
	emit_label_def(label1, s);
	pred->code(s, env);
	emit_load(T1,3,ACC,s);

	emit_beqz(T1,label2, s);
	body->code(s, env);
	emit_branch(label1, s);
	
	emit_label_def(label2, s);
	emit_load_imm(ACC, 0, s);


}


void typcase_class::code(ostream &s, Environment env) {
	int startLabel = glabel;
	int n = cases->len();
	int finishLabel = glabel+n+1;
	int voidLabel = glabel+n+2;
	int no_match_label = glabel+n+3;
	glabel+= (n+4);
	expr->code(s, env);
	emit_beqz(ACC, voidLabel, s);	
	// load class tag
	emit_load(T1, 0, ACC, s);

	s << "# start" << endl;
	emit_label_def(startLabel, s);

	for (int i=0;i<cases->len();i++){
		branch_class* thisCase = cases->nth(i)->get_case();
		int caseLabel = startLabel+1+i;
		int caseTypeTag = classNameTagMap[thisCase->type_decl];
		emit_load_imm(T2, caseTypeTag, s);
		s << "# branch decision " << i << endl;
		emit_beq(T1, T2, caseLabel, s);
	}

	// load class - parent mapping tab
	emit_load_address(T2, CLASS_PARENT_TAB, s);
	// load parent tag
	emit_sll(T1,T1, 2, s);
	emit_add(T2, T1, T2, s);
	emit_load(T1,0, T2, s); 
	emit_load_imm(T2, NO_CLASS_TAG, s);
	// parent is no_class, then no match.
	emit_beq(T1, T2, no_match_label, s); 

	emit_branch(startLabel, s);
	// branches
	for (int i=0;i<cases->len();i++){
		branch_class* thisCase = cases->nth(i)->get_case();
		int caseLabel = startLabel+1+i;
		int caseTypeTag = classNameTagMap[thisCase->type_decl];
		s << "# branch actual logic " << i << endl;
		emit_label_def(caseLabel, s);
		env.enterscope();
		emit_push(ACC, s);
		env.sp_fp++;
		env.add_param(thisCase->name, -env.sp_fp+1);
		thisCase->expr->code(s, env);
		env.exitscope();
		emit_addiu(SP, SP, 4, s);
		env.sp_fp--;
		emit_branch(finishLabel, s);

	}

	// void
	emit_label_def(voidLabel, s);
	emit_load_address(ACC, "str_const0", s);
	emit_load_imm(T1, 1, s);
	emit_jal("_case_abort2", s);

	// no_match
	emit_label_def(no_match_label, s);
	emit_move(ACC, SELF, s);
	emit_jal("_case_abort", s);

	// finish
	s << "# finish" << endl;
	emit_label_def(finishLabel, s);
	s << endl;
}

void block_class::code(ostream &s, Environment env) {
	for (int i=0;i<body->len();i++){
		body->nth(i)->code(s, env);
	}
}

void let_class::code(ostream &s, Environment env) {
	init->code(s, env);
	// new type_decl
	if (init->is_nil()){
		if (type_decl == Int){
			emit_load_int(ACC, defaultInt,s);
		} else if(type_decl == Str){
			emit_load_string(ACC, defaultString, s);
		} else if(type_decl == Bool){
			emit_load_bool(ACC, falsebool, s);
		} else {
			emit_load_imm(ACC,0,s);
		}
	}
	emit_push(ACC, s);
	env.sp_fp++;
	env.enterscope();
	env.add_param(identifier, -env.sp_fp+1);
	body->code(s, env);	

	env.exitscope();
	emit_addiu(SP,SP,4,s);
}

void plus_class::code(ostream &s, Environment env) {
	e1->code(s, env);
	emit_push(ACC, s);
	env.sp_fp++;

	e2->code(s, env);
	emit_jal("Object.copy", s);
	emit_load(T1, 1, SP, s);

	emit_load(T2, 3, T1, s); // e1 val
	emit_load(T3, 3, ACC, s); // e2 val	

	emit_add(T2, T2, T3, s);
	emit_store(T2, 3, ACC, s);

	emit_addiu(SP, SP, 4, s);
	env.sp_fp--;
}

void sub_class::code(ostream &s, Environment env) {
	e1->code(s, env);
	emit_push(ACC, s);
	env.sp_fp++;

	e2->code(s, env);
	emit_jal("Object.copy", s);
	emit_load(T1, 1, SP, s);

	emit_load(T2, 3, T1, s); // e1 val
	emit_load(T3, 3, ACC, s); // e2 val	

	emit_sub(T2, T2, T3, s);
	emit_store(T2, 3, ACC, s);

	emit_addiu(SP, SP, 4, s);
	env.sp_fp--;
}

void mul_class::code(ostream &s, Environment env) {
	e1->code(s, env);
	emit_push(ACC, s);
	env.sp_fp++;

	e2->code(s, env);
	emit_jal("Object.copy", s);
	emit_load(T1, 1, SP, s);

	emit_load(T2, 3, T1, s); // e1 val
	emit_load(T3, 3, ACC, s); // e2 val	

	emit_mul(T2, T2, T3, s);
	emit_store(T2, 3, ACC, s);

	emit_addiu(SP, SP, 4, s);
	env.sp_fp--;
}

void divide_class::code(ostream &s, Environment env) {
	e1->code(s, env);
	emit_push(ACC, s);
	env.sp_fp++;

	e2->code(s, env);
	emit_jal("Object.copy", s);
	emit_load(T1, 1, SP, s);

	emit_load(T2, 3, T1, s); // e1 val
	emit_load(T3, 3, ACC, s); // e2 val	

	emit_div(T2, T2, T3, s);
	emit_store(T2, 3, ACC, s);

	emit_addiu(SP, SP, 4, s);
	env.sp_fp--;
}

void neg_class::code(ostream &s, Environment env) {
	e1->code(s, env);
	emit_jal("Object.copy", s);
	emit_load(T1, 3, ACC, s);
	emit_neg(T1, T1, s);
	emit_store(T1, 3, ACC, s);
}

void lt_class::code(ostream &s, Environment env) {
	e1->code(s, env);
	emit_push(ACC, s);
	env.sp_fp++;

	e2->code(s, env);
	emit_load(T1, 1, SP, s);
	emit_move(T2, ACC, s);
	// fetch real numbers for comparison
	emit_load(T1, 3, T1, s);
	emit_load(T2, 3, T2, s);
	emit_load_bool(ACC, truebool, s);
	emit_blt(T1, T2, glabel, s);
	emit_load_bool(ACC, falsebool, s);
	emit_label_def(glabel, s);
	glabel++;

	emit_addiu(SP, SP, 4, s);
	env.sp_fp--;
}

void eq_class::code(ostream &s, Environment env) {
	e1->code(s, env);
	emit_push(ACC, s);
	env.sp_fp++;

	e2->code(s, env);
	emit_load(T1, 1, SP, s);
	emit_move(T2, ACC, s);
	emit_addiu(SP, SP, 4, s);
	env.sp_fp--;
	Symbol type1 = e1->get_type();
	Symbol type2 = e2->get_type();
	if (type1==Str||type1==Int||type1==Bool){
		if (type2==Str||type2==Int||type2==Bool){
			emit_load_bool(ACC, truebool, s);
			emit_load_bool(A1, falsebool, s);
			emit_jal("equality_test", s);
			return;
		}
	}
	emit_load_bool(ACC, truebool, s);
	emit_beq(T1, T2, glabel, s);
	emit_load_bool(ACC, falsebool, s);
	emit_label_def(glabel, s);
	glabel++;

}

void leq_class::code(ostream &s, Environment env) {
	e1->code(s, env);
	emit_push(ACC, s);
	env.sp_fp++;

	e2->code(s, env);
	emit_load(T1, 1, SP, s);
	emit_move(T2, ACC, s);
	// fetch real numbers for comparison
	emit_load(T1, 3, T1, s);
	emit_load(T2, 3, T2, s);
	emit_load_bool(ACC, truebool, s);
	emit_bleq(T1, T2, glabel, s);
	emit_load_bool(ACC, falsebool, s);
	emit_label_def(glabel, s);
	glabel++;

	emit_addiu(SP, SP, 4, s);
	env.sp_fp--;
}

void comp_class::code(ostream &s, Environment env) {
	e1->code(s, env);
	emit_jal("Object.copy", s);
	emit_load(T1, 3, ACC, s);
	emit_load_imm(T2, 1, s);
	emit_sub(T2, T2, T1, s);
	emit_store(T2, 3, ACC, s);
}

void int_const_class::code(ostream& s, Environment env)  
{
  //
  // Need to be sure we have an IntEntry *, not an arbitrary Symbol
  //
  emit_load_int(ACC,inttable.lookup_string(token->get_string()),s);
}

void string_const_class::code(ostream& s, Environment env)
{
  emit_load_string(ACC,stringtable.lookup_string(token->get_string()),s);
}

void bool_const_class::code(ostream& s, Environment env)
{
  emit_load_bool(ACC, BoolConst(val), s);
}

void new__class::code(ostream &s, Environment env) {
	std::string cls;
	log << "type_name is "<< type_name << endl;
	if(type_name==SELF_TYPE){
		emit_move(ACC, SELF, s);
		// get class tag
		emit_load(T1, 0, ACC, s);
		emit_load_address(ACC, CLASSOBJTAB, s);
		emit_sll(T1, T1, 3, s);
		emit_add(T1, T1, ACC, s);
		emit_load(ACC, 0, T1, s); // get protobj

		emit_addiu(T1, T1, 4, s);
		emit_load(T3, 0, T1, s); // get class_init
		emit_push(T3, s);
		env.sp_fp++;

		emit_jal("Object.copy", s);
		emit_load(T1, 1, SP, s);
		emit_addiu(SP, SP, 4, s);
		env.sp_fp--;
		emit_jalr(T1, s);
		return;
	} else {
		cls = type_name->get_string();
	}
	std::string cls_proto = cls + PROTOBJ_SUFFIX;
	emit_load_address(ACC, cls_proto.c_str(), s);
	emit_jal("Object.copy", s);
	std::string cls_init = cls + CLASSINIT_SUFFIX;
	emit_jal(cls_init.c_str(), s);
}

void isvoid_class::code(ostream &s, Environment env) {
	emit_move(T1, ACC, s);	
	emit_load_bool(ACC, falsebool, s);
	int label = glabel;
	glabel++;
	emit_beqz(T1, label, s);
	emit_load_bool(ACC, truebool, s);
	emit_label_def(label, s);
}

void no_expr_class::code(ostream &s, Environment env) {
}

void object_class::code(ostream &s, Environment env) {
	log << "coding object_class with name:"<<name << endl;
	if (name == self) {
		log << "outputing self" << endl;
		s << "# outputing self" << endl;
		emit_move(ACC, SELF, s);
		return;
	} 
	int idx;
	if ( (idx = env.find_param(name)) != -1){
		emit_load(ACC,idx,FP,s);
	} else if((idx=env.find_attr(name))!=-1){
		emit_load(ACC,idx+DEFAULT_OBJFIELDS,SELF,s);
	} else {
		cout << " error object_class name not found";
		return;
	}
}


