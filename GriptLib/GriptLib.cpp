#include "pch.h"

Gript::Gript(std::vector<uint8_t>& source)
{
	// Parse source json and build graph

}

Gript::~Gript()
{
	while (!nodes.empty())
	{
		GriptNode* node = nodes.back();
		nodes.pop_back();
		delete node;
	}
}

void Gript::Traverse()
{
	// Find first node with no input edges

	GriptNode* root = nullptr;

	for (auto it = nodes.begin(); it != nodes.end(); ++it)
	{
		GriptNode* node = *it;

		if (node->from.size() == 0)
		{
			root = node;
			break;
		}
	}

	// Perform as long as the node has the output edges

	GriptNode* next = root;

	while (next)
	{
		next = InvokeNode(next);
	}
}
