#include <iostream>
#include <Shlwapi.h>
#include <sstream>
#include <conio.h>
#include "Object.h"
using namespace std;

#pragma comment(lib, "Shlwapi.lib")


int main(int argc,const char* args[])
{
	Object o;
	for (int i = 0; i < 11000; i++)
	{
		o.AddPosition(Position(i, 1, 1, 1));
		o.AddTexCoord(TexCoord(i, 1, 0));
		o.AddNormal(Normal(i, 1, 1));
	}
		




	char name[255];
	if (argc < 2)
	{
		cout << "Enter filename/path: ";
		cin >> name;
	}
	else
		strcpy_s(name, args[1]);

	char* ex = PathFindExtension(name);
	if (ex[0] == '\0')
	{
		ostringstream s;
		s << "OBJParser.exe " << name;
		int i = system((s.str() + ".obj").c_str());
		if (i != 0)
		{
			cout << "File not recognized or not found!" << endl;
		}

		while (!_kbhit());
		return -1;
	}
	ex = (char*)((size_t)ex + sizeof(char));
	
	if (!strcmp(ex, "obj"))
	{
		ostringstream s;
		s << "OBJParser.exe " << name;
		system(s.str().c_str());
	}

	while (!_kbhit());
		
	return 0;
}