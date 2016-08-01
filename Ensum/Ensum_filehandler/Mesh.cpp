#include "Ensum_filehandler\Mesh.h"
#include "Ensum_utils\ConsoleLog.h"

#ifdef _DEBUG
#pragma comment(lib, "Ensum_utilsD.lib")
#else
#pragma comment(lib, "Ensum_utils.lib")
#endif



namespace Ensum
{
	namespace FileHandler
	{
		Mesh::Mesh(const char * path)
		{
			ifstream file(path, ios::binary);
			if (file.is_open())
			{
				file.read((char*)&_data, sizeof(Data));
				_data.buffer = operator new(_data.allocated);
				file.read((char*)_data.buffer, _data.allocated);
				file.close();
			}
			else
			{
				Utils::ConsoleLog::DumpToConsole("Failed to load mesh: %s. Loading default instead.", path);
				// TODO: Load default model.
			}
		}
		Mesh::Mesh(const Mesh & other)
		{
			_data = other._data;
			_data.buffer = nullptr;
		}
		Mesh::~Mesh()
		{
			operator delete(_data.buffer);
		}
	}
}