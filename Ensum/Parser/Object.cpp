#include "Object.h"

void Object::Alloc(int flag)
{
	Data ndat;


	switch (flag)
	{
	case ALLOC_ALL:
	{
		_data.buffer = operator new(sizeof(LoadInfo));
		memset(_data.buffer, 0, sizeof(LoadInfo));
		_loadInfo = (LoadInfo*)_data.buffer;

		_loadInfo->PosCap = 10000;
		_loadInfo->TexCap = 10000;
		_loadInfo->NormCap = 10000;
		_loadInfo->FaceCap = 10000;
		break;
	}
	case ALLOC_POSITION:
	{
		_loadInfo->PosCap *= 2;
		break;
	}
	case ALLOC_TECCOORD:
	{
		_loadInfo->TexCap *= 2;
		break;
	}
	case ALLOC_NORMAL:
	{
		_loadInfo->NormCap *= 2;
		break;
	}
	case ALLOC_FACE:
	{
		break;
	}
	default:
		break;
	}

	ndat.allocated = sizeof(LoadInfo) + _loadInfo->PosCap * sizeof(Position)
		+ _loadInfo->TexCap * sizeof(TexCoord) +
		_loadInfo->NormCap * sizeof(Normal) +
		_loadInfo->FaceCap * sizeof(Face);
	ndat.buffer = operator new(ndat.allocated);

	memcpy(ndat.buffer, _data.buffer, sizeof(LoadInfo));
	_loadInfo = (LoadInfo*)ndat.buffer;

	_loadInfo->PosStart = sizeof(LoadInfo);
	ndat.positions = (Position*)((size_t)ndat.buffer + _loadInfo->PosStart);

	_loadInfo->TexStart = _loadInfo->PosStart + _loadInfo->PosCap * sizeof(Position);
	ndat.texCoords = (TexCoord*)((size_t)ndat.buffer + _loadInfo->TexStart);

	_loadInfo->NormStart = _loadInfo->TexStart + _loadInfo->TexCap * sizeof(TexCoord);
	ndat.normals = (Normal*)((size_t)ndat.buffer + _loadInfo->NormStart);

	_loadInfo->FaceStart = _loadInfo->NormStart + _loadInfo->NormCap * sizeof(Normal);
	ndat.faces = (Face*)((size_t)ndat.buffer + _loadInfo->FaceStart);


	memcpy(ndat.positions, _data.positions, _loadInfo->NumPos * sizeof(Position));
	memcpy(ndat.texCoords, _data.texCoords, _loadInfo->NumTex * sizeof(TexCoord));
	memcpy(ndat.normals, _data.normals, _loadInfo->NumNorm * sizeof(Normal));
	memcpy(ndat.faces, _data.faces, _loadInfo->NumFace * sizeof(Face));

	operator delete(_data.buffer);
	_data = ndat;

}

Object::Object()
{
	Alloc(ALLOC_ALL);
}
Object::~Object()
{
	operator delete(_data.buffer);
}

void Object::AddPosition(const Position & pos)
{
	if (_loadInfo->NumPos >= _loadInfo->PosCap)
	{
		// Allocate more space
		Alloc(ALLOC_POSITION);
	}

	_data.positions[_loadInfo->NumPos] = pos;
	_loadInfo->NumPos++;
}

void Object::AddTexCoord(const TexCoord & tex)
{
	if (_loadInfo->NumTex >= _loadInfo->TexCap)
	{
		// Allocate more space
		Alloc(ALLOC_TECCOORD);
	}

	_data.texCoords[_loadInfo->NumTex] = tex;
	_loadInfo->NumTex++;
}

void Object::AddNormal(const Normal & norm)
{
	if (_loadInfo->NumPos >= _loadInfo->PosCap)
	{
		// Allocate more space
		Alloc(ALLOC_NORMAL);
	}

	_data.normals[_loadInfo->NumNorm] = norm;
	_loadInfo->NumNorm++;
}


void Object::Print()
{

}

void Object::GenerateArf(ofstream & of)
{
}

