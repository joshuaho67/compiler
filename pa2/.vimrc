set tags+=/usr/class/cs143/include/PA2/PA2_tags
au BufWritePost *cc,*h,*.flex silent! !find -name '*.cc' -or -name '*.h' -or -name '*.flex' -exec ctags {} + &
