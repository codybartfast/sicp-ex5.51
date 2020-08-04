@echo off
SET TEST_DIR=%~dp0
SET BIN_DIR=%TEST_DIR%..\bin

type %TEST_DIR%\source.txt |  "%BIN_DIR%\sicp.exe"
