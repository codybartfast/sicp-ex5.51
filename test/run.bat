@echo off
SET TEST_DIR=%~dp0
SET BIN_DIR=%TEST_DIR%..\bin

"%BIN_DIR%\sicp.exe" %TEST_DIR%code\scratch.sicp
