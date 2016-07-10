#include <string>
#include <iostream>
using namespace std;

struct Position
{
	float x, y, z, w;
	Position(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
	{}
};

struct TexCoord
{
	float u, v, w;
	TexCoord(float u, float v, float w) : u(u), v(v), w(w)
	{}
};

struct Normal
{
	float x, y, z;
	Normal(float x, float y, float z) : x(x), y(y), z(z)
	{}
};

struct Indices
{
	uint64_t index[4];// array of size 4.
};

struct Face
{
	uint8_t indexCount; // Stored bytewise.
	Indices indices[4]; // Array of size 4.
};
struct Data
{
	void* buffer = nullptr;
	size_t allocated = 0;

	Position* positions;
	TexCoord* texCoords;
	Normal* normals;
	Face* faces;
};

struct LoadInfo
{
	size_t PosStart = 0;
	uint64_t NumPos = 0;
	uint64_t PosCap = 0;

	size_t TexStart = 0;
	uint64_t NumTex = 0;
	uint64_t TexCap = 0;

	size_t NormStart = 0;
	uint64_t NumNorm = 0;
	uint64_t NormCap = 0;

	size_t FaceStart = 0;
	uint64_t NumFace = 0;
	uint64_t FaceCap = 0;
};


#define ALLOC_ALL 0
#define ALLOC_POSITION 1
#define ALLOC_TECCOORD 2
#define ALLOC_NORMAL 3
#define ALLOC_FACE 4

class Object
{
private:
	Data _data;
	LoadInfo* _loadInfo;

	void Alloc(int flag);
public:
	Object();
	~Object();
	void AddPosition(const Position& pos);
	void AddTexCoord(const TexCoord& tex);
	void AddNormal(const Normal& norm);
	void Print();
	void GenerateArf(ofstream& of);
};
