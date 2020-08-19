@echo off
SET TEST_DIR=%~dp0
SET BIN_DIR=%TEST_DIR%..\bin

type %TEST_DIR%\code\ex1.01__text.sicp |  "%BIN_DIR%\sicp.exe"
type %TEST_DIR%\code\ex1.01_question.sicp |  "%BIN_DIR%\sicp.exe"
type %TEST_DIR%\code\ex1.46_cbf.sicp |  "%BIN_DIR%\sicp.exe"
