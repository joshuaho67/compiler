set tags+=/usr/class/cs143/include/PA5/PA5_tags
au BufWritePost *cc,*h silent! !ctags ./ast-parse.cc ./ast-lex.cc ./tree.cc ./dumptype.cc ./cgen-phase.cc ./cgen.cc ./cool-tree.cc ./utilities.cc ./cool-tree.handcode.h ./cool-tree.h ./emit.h ./cgen.h ./cgen_supp.cc ./handle_flags.cc ./stringtab.cc &
