#include "pch.h"

static std::wstring StringToWstring(const std::string& str)
{
	std::wstring wstr;
	wstr.reserve(str.size());
	for (auto it = str.begin(); it != str.end(); ++it)
	{
		wstr.push_back((wchar_t)*it);
	}
	return wstr;
}

Gript::Gript(std::vector<uint8_t>& source)
{
	// Parse source json and build graph

	Json json;
	json.Deserialize(source.data(), source.size());

	// Collect nodes

	Json::Value* realRoot = json.root.children.back();

	Json::Value* sourceNodes = realRoot->ByName("nodes");

	for (auto it = sourceNodes->children.begin(); it != sourceNodes->children.end(); ++it)
	{
		Json::Value* node = *it;

		GriptNode* griptNode = new GriptNode;

		griptNode->name = StringToWstring(node->name);
		Json::Value* text = node->ByName("text");
		if (text)
		{
			griptNode->text = std::wstring(text->value.AsString);
		}

		nodes.push_back(griptNode);
	}

	// Link by edges

	Json::Value* sourceEdges = realRoot->ByName("edges");

	for (auto it = sourceEdges->children.begin(); it != sourceEdges->children.end(); ++it)
	{
		Json::Value* edge = *it;

		if (edge->type != Json::ValueType::Array)
		{
			throw "Invalid edge format!";
		}

		if (edge->children.size() != 2)
		{
			throw "Invalid edge format!";
		}

		Json::Value* from = edge->children.front();
		Json::Value* to = edge->children.back();

		std::wstring fromName = std::wstring(from->value.AsString);
		std::wstring toName = std::wstring(to->value.AsString);

		GriptNode* fromNode = nullptr;
		GriptNode* toNode = nullptr;

		for (auto nit = nodes.begin(); nit != nodes.end(); ++nit)
		{
			GriptNode* node = *nit;
			if (node->name == fromName && !fromNode)
			{
				fromNode = node;
			}
			if (node->name == toName && !toNode)
			{
				toNode = node;
			}
			if (fromNode && toNode)
			{
				break;
			}
		}

		if (fromNode && toNode)
		{
			fromNode->to.push_back(toNode);
			toNode->from.push_back(fromNode);
		}
	}
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
