#include "tool.hpp"

Tool::Tool(string name):
    name(name)
{
    device()->Register_tool(this);
}
