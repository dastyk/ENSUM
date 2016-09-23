#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
using namespace std;




class Node
{
private:
	vector<Node*> _nodes;
public:
	Node();
	virtual ~Node();
	virtual void Add(Node* node);
	virtual void Print(int depth);
};


class Object;

class Data : public Node
{
public:
	Data();
	virtual ~Data();
	virtual void Add(Object* obj)=0;
	virtual void Add(Data* data)=0;
	virtual void Print(int depth) = 0;
};

class Position : public Data
{
private:
	struct Pos
	{
		float x, y, z, w;
		Pos(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
	};
	vector<Pos> _data;
public:
	Position(string x, string y, string z, string w);
	virtual ~Position();
	void Add(Object* obj);
	void Add(Data* data);
	void Print(int depth);
};

class Normal : public Data
{
private:
	struct Norm
	{
		float x, y, z;
		Norm(float x, float y, float z) : x(x), y(y), z(z) {};
	};
	vector<Norm> _data;
public:
	Normal(string x, string y, string z);
	virtual ~Normal();
	void Add(Object* obj);
	void Add(Data* data);
	void Print(int depth);
};

class TexCoord : public Data
{
private:
	struct TexC
	{
		float u, v, w;
		TexC(float u, float v, float w) : u(u), v(v), w(w) {};
	};
	vector<TexC> _data;
public:
	TexCoord(string u, string v, string w);
	virtual ~TexCoord();
	void Add(Object* obj);
	void Add(Data* data);
	void Print(int depth);
};

class Point : public Data
{
private:
	struct TexC
	{
		float u, v, w;
		TexC(float u, float v, float w) : u(u), v(v), w(w) {};
	};
	vector<TexC> _data;
public:
	Point(string u, string v, string w);
	virtual ~Point();
	void Add(Object* obj);
	void Add(Data* data);
	void Print(int depth);
};
class Face : public Data
{
private:
	struct Indices
	{
		vector<unsigned long> index;
	};
	struct F
	{
		bool quad;
		bool line;
		bool point;
		Indices face[4];
		F(vector<unsigned long> p) : quad(false), line(false), point(true)
		{
			face[0].index = p;
		}
		void Add(vector<unsigned long> p)
		{
			if (face[0].index.size() == 0)
				face[0].index = p;
			else if (face[1].index.size() == 0)
			{
				point = false;
				line = true;
				face[1].index = p;
			}			
			else if (face[2].index.size() == 0)
			{
				line = false;
				face[2].index = p;
			}	
			else if (face[3].index.size() == 0)
			{
				quad = true;
				face[3].index = p;
			}

		}
	};
	vector<F> _data;
public:
	Face(vector<unsigned long> p);
	virtual ~Face();
	void AddIndices(vector<unsigned long> p);
	void Add(Object* obj);
	void Add(Data* data);
	void Print(int depth);
};

class UseMtl : public Data
{
private:
	string _name;
public:
	UseMtl(string name);
	~UseMtl();
	void Add(Object* obj);
	void Add(Data* data);
	void Print(int depth);

};

class Smooth : public Data
{
private:
	bool _smooth;
public:
	Smooth(bool use);
	~Smooth();
	void Add(Object* obj);
	void Add(Data* data);
	void Print(int depth);
};


class Object : public Node
{
private:
	unordered_map<string, Data*> _data;
	string _name;
	string _usemtl;
public:
	Object();
	virtual ~Object();
	virtual void SetName(string name);
	virtual void SetMtlLib(string name);
	virtual void Add(string type, Data* data);
	virtual void Print(int depth);
};

class MtlLib : public Node
{
	string _name;
public:
	MtlLib(string name);
	~MtlLib();
	virtual void Print(int depth);
};