@echo off
set test_dir=%~dp0
set bin_dir=%test_dir%..\bin\
set source_dir=%test_dir%code\

echo %test_dir%
echo %bin_dir%
echo %source_dir%

cd %source_dir%
call :doDir
goto :eof

:doDir
	for %%f in (*.sicp) do call :runit %%f
	for /D %%d in (*) do (
	    cd %%d
	    call :doDir
	    cd ..
	)
exit /b

:runit
	set file=%~f1
	echo running: %file%
	%bin_dir%sicp  %file%
	echo.
exit /b
