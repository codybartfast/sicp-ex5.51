# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("sicp-scheme")

# Add source to this project's executable.
add_executable (sicp 
	"src/obj.c"
	"src/obj.h"
	"src/strbldr.c"
	"src/strbldr.h"
	"src/obj.c"
	"src/obj.h"
	"src/sserror.c"
	"src/sserror.h"
	"src/inport.c"
	"src/inport.h"
	"src/outport.c"
	"src/outport.h"
	"src/lexer.c"
	"src/lexer.h"
	"src/parser.c"
	"src/parser.h"
	"src/convert.c"
	"src/convert.h"
	"src/eval.c"
	"src/eval.h"
	"src/output.c"
	"src/output.h"
	"src/sicp.c"
	"src/windows.h"
	"src/windows.c"
	)

# TODO: Add tests and install targets if needed.
