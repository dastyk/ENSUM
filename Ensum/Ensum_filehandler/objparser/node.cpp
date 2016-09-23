#include "node.h"

Node::Node()
{

}
Node::~Node()
{
	for (auto n : _nodes)
		delete n;
}

void Node::Add(Node * node)
{
	_nodes.push_back(node);
}

void Node::Print(int depth)
{
	cout << "Node" << endl;
	for (auto& n : _nodes)
		n->Print(depth + 1);
}


Object::Object()
{

}

Object::~Object()
{
	for (auto& d : _data)
		delete d.second;
}

void Object::SetName(string name)
{
	_name = name;
}

void Object::SetMtlLib(string name)
{
	_usemtl = name;
}

void Object::Add(string type, Data * data)
{
	if (_data.find(type) == _data.end ())
		_data[type] = data;
	else
		_data[type]->Add(data);
}

void Object::Print(int depth)
{
	for (int i = 0; i < depth; i++)
		cout << "\t";
	cout << _name << endl;

	for (auto& d : _data)
		d.second->Print(depth + 1);
}



Data::Data()
{
}

Data::~Data()
{
}


Position::Position(string x, string y, string z, string w)
{
	_data.push_back(Pos(stof(x), stof(y), stof(z), stof(w)));
}

Position::~Position()
{
}

void Position::Add(Object * obj)
{
	obj->Add("Position", this);
}

void Position::Add(Data * data)
{
	Position* d = (Position*)data;
	_data.insert(_data.end(), d->_data.begin(), d->_data.end());
	delete data;
}

void Position::Print(int depth)
{
	for (int i = 0; i < depth; i++)
		cout << "\t";
	cout << "Positions" << endl;
	for (auto& p : _data)
	{
		for (int i = 0; i < depth+1; i++)
			cout << "\t";
		cout << p.x << " " << p.y << " " << p.z << " " << p.w << endl;
	}
}

Normal::Normal(string x, string y, string z)
{
	_data.push_back(Norm(stof(x), stof(y), stof(z)));
}

Normal::~Normal()
{
}

void Normal::Add(Object * obj)
{
	obj->Add("Normal", this);
}

void Normal::Add(Data * data)
{
	Normal* d = (Normal*)data;
	_data.insert(_data.end(), d->_data.begin(), d->_data.end());
	delete data;
}

void Normal::Print(int depth)
{
	for (int i = 0; i < depth; i++)
		cout << "\t";
	cout << "Normals" << endl;
	for (auto& p : _data)
	{
		for (int i = 0; i < depth + 1; i++)
			cout << "\t";
		cout << p.x << " " << p.y << " " << p.z << endl;
	}
}

TexCoord::TexCoord(string u, string v, string w)
{
	_data.push_back(TexC(stof(u), stof(v), stof(w)));
}

TexCoord::~TexCoord()
{
}

void TexCoord::Add(Object * obj)
{
	obj->Add("TexCoord", this);
}

void TexCoord::Add(Data * data)
{
	TexCoord* d = (TexCoord*)data;
	_data.insert(_data.end(), d->_data.begin(), d->_data.end());
	delete data;
}

void TexCoord::Print(int depth)
{
	for (int i = 0; i < depth; i++)
		cout << "\t";
	cout << "TexCoords" << endl;
	for (auto& p : _data)
	{
		for (int i = 0; i < depth + 1; i++)
			cout << "\t";
		cout << p.u << " " << p.v << " " << p.w << endl;
	}
}

Point::Point(string u, string v, string w)
{
	_data.push_back(TexC(stof(u), stof(v), stof(w)));
}

Point::~Point()
{
}

void Point::Add(Object * obj)
{
	obj->Add("Parameter Space Point", this);
}

void Point::Add(Data * data)
{
	Point* d = (Point*)data;
	_data.insert(_data.end(), d->_data.begin(), d->_data.end());
	delete data;
}

void Point::Print(int depth)
{
	for (int i = 0; i < depth; i++)
		cout << "\t";
	cout << "Parameter Space Points" << endl;
	for (auto& p : _data)
	{
		for (int i = 0; i < depth + 1; i++)
			cout << "\t";
		cout << p.u << " " << p.v << " " << p.w << endl;
	}
}

Face::Face(vector<unsigned long> p)
{
	_data.push_back(F(p));
}

Face::~Face()
{
}

void Face::AddIndices(vector<unsigned long> p)
{
	_data[0].Add(p);
}

void Face::Add(Object * obj)
{
	obj->Add("Face", this);
}

void Face::Add(Data * data)
{
	Face* d = (Face*)data;
	_data.insert(_data.end(), d->_data.begin(), d->_data.end());
	delete data;
}

void Face::Print(int depth)
{
	for (int i = 0; i < depth; i++)
		cout << "\t";
	cout << "Faces" << endl;
	for (auto& p : _data)
	{
		for (int i = 0; i < depth + 1; i++)
			cout << "\t";
		for (int i = 0; i < 4; i++)
		{
			for (vector<unsigned long>::iterator it = p.face[i].index.begin(); it != p.face[i].index.end(); it++)
			{
				cout << (*it);
				if ((it + 1) != p.face[i].index.end())
					cout << "/";
			}
			cout << " ";
		}
		cout << endl;
	}
}

MtlLib::MtlLib(string name) : _name(name)
{
}

MtlLib::~MtlLib()
{
}

void MtlLib::Print(int depth)
{

}

UseMtl::UseMtl(string name) : _name(name)
{
}

UseMtl::~UseMtl()
{
}

void UseMtl::Add(Object * obj)
{
	obj->Add("MtlLib", this);
}

void UseMtl::Add(Data * data)
{

}

void UseMtl::Print(int depth)
{
	for (int i = 0; i < depth; i++)
		cout << "\t";
	cout << "MtlLib " << endl;
	for (int i = 0; i < depth + 1; i++)
		cout << "\t";
	cout << _name << endl;
}

Smooth::Smooth(bool use) : _smooth(use)
{
}

Smooth::~Smooth()
{
}

void Smooth::Add(Object * obj)
{
	obj->Add("Smoothing", this);
}

void Smooth::Add(Data * data)
{
}

void Smooth::Print(int depth)
{
	for (int i = 0; i < depth; i++)
		cout << "\t";
	cout << "Smoothing " << endl;
	for (int i = 0; i < depth + 1; i++)
		cout << "\t"; 
	cout << (_smooth ? "true" : "false") << endl;
}
