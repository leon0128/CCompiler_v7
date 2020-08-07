#ifndef RESOURCE_CONTROLLER_HPP
#define RESOURCE_CONTROLLER_HPP

#include <vector>
#include <cstddef>

namespace RESOURCE_CONTROLLER
{
    class Base;
}

class ResourceController
{
public:
    ResourceController() = delete;

    static void add(RESOURCE_CONTROLLER::Base*);
    static void release();

private:
    static std::vector<RESOURCE_CONTROLLER::Base*> RESOURCE;
};

namespace RESOURCE_CONTROLLER
{

class Base
{
public:
    void *operator new(std::size_t);
    void *operator new[](std::size_t) = delete;
    virtual ~Base() noexcept = 0;
};

}

#endif