#include <iostream>
#include <Shlwapi.h>
#include <sstream>
#include <conio.h>
//#include "Object.h"
using namespace std;

#pragma comment(lib, "Shlwapi.lib")


int main(int argc,const char* args[])
{
	//Object o;
	//for (int i = 0; i < 10; i++)
	//{
	//	o.AddPosition(Position(i, 1, 1, 1));
	//	o.AddTexCoord(TexCoord(i, 1, 0));
	//	o.AddNormal(Normal(i, 1, 1));
	//}
	//	
	//for (int i = 0; i < 10; i++)
	//{
	//	vector<vector<uint64_t>> temp;
	//	vector<uint64_t> temp2;
	//	temp2.push_back(i);
	//	temp2.push_back(i);
	//	temp2.push_back(i);
	//	temp.push_back(temp2);
	//	temp.push_back(temp2);
	//	temp.push_back(temp2);
	//	o.AddFace(temp);
	//}

	//o.AddSubMesh("TestSub");
	//for (int i = 9; i < 20; i++)
	//{
	//	vector<vector<uint64_t>> temp;
	//	vector<uint64_t> temp2;
	//	temp2.push_back(i);
	//	temp2.push_back(i);
	//	temp2.push_back(i);
	//	temp.push_back(temp2);
	//	temp.push_back(temp2);
	//	temp.push_back(temp2);
	//	temp.push_back(temp2);
	//	o.AddFace(temp);
	//}
	//ofstream file("test.arf", ios::binary | ios::trunc);
	//o.GenerateArf(file);
	//file.close();

	//ifstream file2("out.arf", ios::binary);
	//Object o2(file2);
	//file2.close();
	//o2.Print();

	char name[255];
	if (argc < 2)
	{
		cout << "Enter filename/path: ";
		cin >> name;
	}
	else
		strcpy_s(name, args[1]);
	
	char nename[255];
	strcpy_s(nename, name);
	PathStripPath(nename);
	PathRenameExtension(nename, ".arf");
	char* ex = PathFindExtension(name);
	if (ex[0] == '\0')
	{
		ostringstream s;
		s << "OBJParser.exe " << name << " " << nename;
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
		s << "OBJParser.exe " << name << " " << nename;
		system(s.str().c_str());
	}

	while (!_kbhit());
		
	return 0;
}