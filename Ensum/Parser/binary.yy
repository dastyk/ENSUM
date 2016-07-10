%skeleton "lalr1.cc" 
%defines
%define api.value.type variant
%define api.token.constructor
%code requires{
	#include "node.h"
}
%code{
  #define YY_DECL yy::parser::symbol_type yylex()
   Node* root = nullptr;
  YY_DECL;
}

%type <Node*> line
%type <Node*> statement
%type <Node*> createobject
%type <Node*> data
%type <Node*> indices
%type <Node*> optindices

%token <std::string> REAL
%token <std::string> INTEGER
%token <std::string> POSITION
%token <std::string> TEXCOORD
%token <std::string> FACE
%token <std::string> OBJECT
%token <std::string> S
%token <std::string> MTLLIB
%token <std::string> USEMTL
%token <std::string> NAME
%token <std::string> BOOLEAN
%token <std::string> SEP
%token END 0 "end of file"

%%


line			: statement									{ $$ = new Node(); std::cout << "line: statement" << std::endl;}
				| line statement							{ $$ = $1; std::cout << "line: line statement" << std::endl;}
				;

statement		: OBJECT NAME createobject					{ $$ = new Node(); std::cout << "statement: OBJECT NAME createobject" << std::endl;}
				| MTLLIB NAME								{ $$ = new Node(); std::cout << "statement: MTLLIB NAME" << std::endl;}
				;

createobject	: data										{ $$ = new Node(); std::cout << "createobject: data" << std::endl;}
				| createobject data							{ $$ = new Node(); std::cout << "createobject: createobject data" << std::endl;}
				;

data			: POSITION REAL REAL REAL					{ $$ = new Node(); std::cout << "data: POSITION REAL REAL REAL" << std::endl;}
				| TEXCOORD REAL REAL						{ $$ = new Node(); std::cout << "data: TEXCOORD REAL REAL" << std::endl;}
				| USEMTL NAME								{ $$ = new Node(); std::cout << "data: USEMTL NAME" << std::endl;}
				| S BOOLEAN									{ $$ = new Node(); std::cout << "data: S BOOLEAN" << std::endl;}
				| FACE indices indices indices optindices	{ $$ = new Node(); std::cout << "data: S BOOLEAN" << std::endl;}
				;

indices			: INTEGER									{ $$ = new Node(); std::cout << "indices: S INTEGER" << std::endl;}
				| indices SEP INTEGER						{ $$ = new Node(); std::cout << "indices: indices SEP INTEGER" << std::endl;}		
				;

optindices		: indices									{ $$ = new Node(); std::cout << "optindices: indices" << std::endl;}
				| /*empty*/									{ $$ = new Node(); std::cout << "optindices: empty" << std::endl;}		