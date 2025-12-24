// main.cpp
#include "Server.h"

int main(int argc, char **argv)
{
    Server srv;

    if (!srv.Initialize(argc, argv)) {
        // Если инициализация не удалась (справка показана или ошибка)
        return 0;  // Выходим с кодом 0
    }
    
    srv.start();
    
    return 0;
}
