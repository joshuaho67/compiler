set tags+=/usr/class/cs143/include/PA3/PA3_tags
"au BufWritePost *cc,*h,*y silent! !find -name '*.cc' -or -name '*.h' -or -name '*.y' -exec ctags {} + &
au BufWritePost *cc,*h,*y silent! !ctags ./tree.cc ./dumptype.cc ./cool-tree.cc ./utilities.cc ./cool-tree.handcode.h ./parser-phase.cc ./cool.tab.h ./cool-parse.cc ./cool.y ./tokens-lex.cc ./handle_flags.cc ./stringtab.cc &
" there's some kind of bug that prevents me from using the following command.
"function! CreateTags()
"		let cmd = 'ctags -R *'
"		system(cmd)
"endfunction
"au BufWritePost *cc,*h,*y call CreateTags() 
