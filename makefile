DEPENDENCIES := lex.cpp parser.cpp parser.hpp 
OBJECTS := parser 

all:
	${MAKE} clean
	${MAKE} lexer
	${MAKE} parser
	${MAKE} llvm

lexer:
	flex -o lex.cpp lex.l

parser:
	bison -v -t -d -o parser.cpp parser.y

llvm: 
	g++ -o parser parser.cpp lex.cpp main.cpp -std=c++11 `llvm-config-7 --cppflags`

clean:
	rm -f $(DEPENDENCIES) $(OBJECTS)