
cmake_minimum_required (VERSION 3.8)

project (sicp-scheme)

set (SRC_CORE
	src/obj.c
	src/obj.h
	src/strbldr.c
	src/strbldr.h
	src/obj.c
	src/obj.h
	src/sserror.c
	src/sserror.h
	src/inport.c
	src/inport.h
	src/outport.c
	src/outport.h
	src/lexer.c
	src/lexer.h
	src/parser.c
	src/parser.h
	src/convert.c
	src/convert.h
	src/eval.c
	src/eval.h
	src/output.c
	src/output.h	
	src/windows.h
	src/windows.c
	)

set(BIN_DIR  ${CMAKE_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

add_executable (sicp ${SRC_CORE} src/sicp.c)
