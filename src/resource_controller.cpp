#include <new>
#include "resource_controller.hpp"

std::vector<RESOURCE_CONTROLLER::Base*> ResourceController::RESOURCE;

void ResourceController::add(RESOURCE_CONTROLLER::Base *base)
{
    RESOURCE.push_back(base);
}

void ResourceController::release()
{
    for(auto &&b : RESOURCE)
        delete b;
}

RESOURCE_CONTROLLER::Base::~Base(){}

void *RESOURCE_CONTROLLER::Base::operator new(std::size_t size)
{
    void *alloc = ::operator new(size);
    ResourceController::add(static_cast<Base*>(alloc));
    return alloc;
}