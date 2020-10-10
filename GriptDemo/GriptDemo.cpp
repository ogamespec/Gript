#include "pch.h"

/// <summary>
/// Simple implementation that just prints out the name and text of the node.
/// </summary>
class DummyGriptEngine : public Gript
{
public:
    DummyGriptEngine(std::vector<uint8_t>& source) : Gript(source) { }
    ~DummyGriptEngine() { }

    GriptNode* InvokeNode(GriptNode* node)
    {
        // Dump node

        wprintf(L"%s: %s\n", node->name.c_str(), node->text.c_str());

        // Propagate

        if (node->to.size() == 0)
        {
            return nullptr;
        }

        return node->to.front();
    }
};

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Use: GriptDemo <source.gript>\n");
        return 0;
    }

    // Load gript

    FILE* f = fopen(argv[1], "rb");
    if (!f)
    {
        printf("Cannot load %s\n", argv[1]);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    std::vector<uint8_t> source;
    source.resize(size);

    fread(source.data(), 1, size, f);
    fclose(f);

    // Traverse

    DummyGriptEngine gr(source);
    gr.Traverse();

    return 0;
}
