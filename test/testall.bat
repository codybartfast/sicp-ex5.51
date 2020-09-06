@echo off
set test_dir=%~dp0

%test_dir%runall | find "ERROR: ("