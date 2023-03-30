REM How to Use this bat file 
REM Follow step by step below
:: Install or Download MingGW and save it in a specific folder
:: Download Freeglut and config it below
::   -copy include and bin folder into MinGW
::   -go to "bin folder" and copy freeglut.dll in to System32 and SysWOW64
:: Extract the bat.(zip) and copy it into your folder
:: set your project name like this "main.c"
:: Run This.bat 
:: Done

cls
REM
echo "Running \m"
g++ -Wall -g -I"C:\MinGW\include" -c "1705096_main.cpp" -o 1705096_main.o

echo "Compiling Graphic"
g++ -L"C:\MinGW\lib" -o 1705096_main.exe 1705096_main.o -lfreeglut -lopengl32 -lglu32 -lwinmm -lgdi32

echo "App Running"
1705096_main.exe