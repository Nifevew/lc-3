EXECUTABLE = lc3

all: main.obj LC_3.obj
	link main.obj LC_3.obj /OUT:$(EXECUTABLE).exe
	nmake clean

main.obj:
	cl main.cpp /c /Ox /std:c++20 /EHsc /Fo:main

LC_3.obj:
	cl LC_3.cpp /c /Ox /std:c++20 /Fo:LC_3

clean:
	del main.obj
	del LC_3.obj
