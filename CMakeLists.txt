
cmake_minimum_required (VERSION 3.8)

project (sicp-scheme)

set (SRC_CORE
	src/convert.c
	src/convert.h
	src/error.c
	src/error.h
	src/eval.c
	src/eval.h
	src/inport.c
	src/inport.h
	src/lexer.c
	src/lexer.h
	src/obj.c
	src/obj.h
	src/outport.c
	src/outport.h
	src/output.c
	src/output.h	
	src/parser.c
	src/parser.h
	src/strbldr.c
	src/strbldr.h
	src/windows.h
	src/windows.c
	)

set(BIN_DIR  ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_executable (sicp ${SRC_CORE} src/sicp.c)
