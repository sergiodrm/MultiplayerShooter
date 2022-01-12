@echo off

FOR /f %%I in ('dir *.uproject /b') do set uprojectName=%%~I

IF EXIST gen_temp.txt ( del /Q gen_temp.txt )
powershell -command "& { (Get-ItemProperty 'Registry::HKEY_CLASSES_ROOT\Unreal.ProjectFile\shell\rungenproj' -Name 'Icon' ).'Icon' }" > gen_temp.txt
set /P gen_bin=<gen_temp.txt
%gen_bin% /projectfiles %cd%\%uprojectName%
del /Q gen_temp.txt
