#pragma once

#include <string>

namespace burda
{
namespace function_extractor
{
class library_loader_base
{
public:
    explicit library_loader_base(const std::string & /*library_path*/)
    {
    }

    virtual ~library_loader_base()
    {
    }
};
}
}
