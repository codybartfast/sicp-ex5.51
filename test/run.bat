@echo off
SET TEST_DIR=%~dp0
SET BIN_DIR=%TEST_DIR%..\bin

type %TEST_DIR%\code\scratch.sicp |  "%BIN_DIR%\sicp.exe"
