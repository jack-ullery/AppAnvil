#flex -B -v -+ parser_lex.l
flex -B -v parser_lex.l
bison -d parser_yacc.y
