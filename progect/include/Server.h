#ifndef __SERVER_H__
#define __SERVER_H__

#include "ServerError.h"
#include "CmdLine.h"
#include "Logger.h"
#include "Communication.h"
#include "UsersParser.h"
/** @file
* @author Макеев В.Д.
* @version 1.0
* @date 20.12.2023
* @copyright ИБСТ ПГУ
* @warning Курсовая работа студента
* @brief Заголовочный файл для модуля Server
*/


/** @brief Класс Server
* @details Класс Server отвечает за инициализацию и запуск сервера.
* @warning Реализация только для типа данных int16_t
*/

class Server{

public:
	/**
     * @brief Инициализация сервера
     * @param argc Количество аргументов командной строки
     * @param argv Массив с аргументами командной строки
     * @return true, если инициализация прошла успешно, false в противном случае
     */
    bool Initialize(int argc,char** argv);
    /**
     * @brief Запуск сервера
    */
    void start();

private:
	/**
     * @brief Выполнение вычислений для клиента
     * @param client Сокет клиента
     */
    void executeCalculations(int client);

    CmdLine m_cmd;
    UsersParser m_usersParser;
    Communication m_com;

    bool m_mustDie = false;
};
#endif // __SERVER_H__
