@REM compiles all .c files in the folder and builds library. Used when .c files are changed ouside of KULT or Eclipse.
@set libName=simpleLogTst
@set depends=simpleLog
echo Rebuilding %libName%...
cd %KI_KULT_PATH%\%libName%\src
for %%f in ( *.c ) do call kult compile_mod -l%libName% %%f
call kult bld_lib -l%libName% -dep %depends%
