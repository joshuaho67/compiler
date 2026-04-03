set tags+=/usr/class/cs143/include/PA4/PA4_tags
"au BufWritePost *cc,*h silent! !find -name '*.cc' -or -name '*.h' -exec ctags {} + &
"au BufWritePost *cc,*h silent! !find -name '*.cc' -or -name '*.h' -exec ctags {} + &
au BufWritePost *cc,*h silent! !ctags ./ast-parse.cc ./ast-lex.cc ./symtab_example.cc ./tree.cc ./dumptype.cc ./semant.h ./semant-phase.cc ./cool-tree.cc ./utilities.cc ./cool-tree.handcode.h ./cool-tree.h ./semant.cc ./handle_flags.cc ./stringtab.cc &
