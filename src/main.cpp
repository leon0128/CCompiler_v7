#include "controller.hpp"

int main(int argc, char **argv)
{
    Controller controller;
    return controller.execute(argv[1]) ? 0 : 1;
}