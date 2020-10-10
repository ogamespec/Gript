// Gript - Graph Script

#pragma once

#include <vector>
#include <list>
#include <string>

class GriptNode
{
public:
	std::wstring name;		// Node name
	std::wstring text;		// Node inner text

	std::list<GriptNode*> from;			// Input nodes
	std::list<GriptNode*> to;		// Output nodes

	GriptNode()	{ }
	virtual ~GriptNode() { }
};

class Gript
{
	std::list<GriptNode*> nodes;

public:
	Gript(std::vector<uint8_t> & source);
	virtual ~Gript();

	void Traverse();

	virtual GriptNode * InvokeNode(GriptNode *node) = 0;
};
