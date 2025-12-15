#include "Server.h"

int main(int argc, char **argv)
{
    Server srv;

    srv.Initialize(argc,argv);
    srv.start();
    
    return 0;
}