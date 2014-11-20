set doxygenFolder="C:\Program Files\doxygen\bin\"
set doxDocFolder=%CD%\docs\dev\html\
set doxConfigFile=simpleLog_DoxyConfigfile

echo %doxygenFolder%
echo %doxDocFolder%
rmdir /s/q %doxDocFolder%
REM pause
%doxygenFolder%doxygen.exe %doxConfigFile%
REM pause
%doxDocFolder%index.html