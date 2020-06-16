#include "controller.hpp"

int main(int argc, char **argv)
{
    return Controller::execute(argv[1]) ? 0 : 1;
}
