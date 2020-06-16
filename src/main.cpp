#include "controller.hpp"

int main(int argc, char **argv)
{
    Controller controller;
    return controller.execute(argv[0]) ? 0 : 1;
}