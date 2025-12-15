#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "UsersParser.h"
/** @file
* @author Макеев В.Д.
* @version 1.0
* @date 20.12.2023
* @copyright ИБСТ ПГУ
* @warning Курсовая работа студента
* @brief Заголовочный файл для модуля Communication
*/


/** @brief Класс Communication
* @details Класс Communication отвечает за связь между клиентами и сервером.
* @warning Реализация только для типа данных int16_t
*/

class Communication{
public:
	/**
     * @brief Создает сокет на указанном порту.
     * @param port Номер порта, на котором будет создан сокет.
     */
    void createSocket(int port);
    /**
     * @brief Ожидает подключения клиента и принимает его
     * @return Дескриптор клиентского сокета.
     */
    int waitClient();
	/**
     * @brief Авторизует клиента, сравнивая его учетные данные с информацией пользователя.
     * @param clientSocket Дескриптор сокета клиента.
     * @paramusers Экземпляр класса UsersParser, хранящий информацию о пользователях.
     */
    void autorize(int clientSocket,UsersParser users);

private:
    int m_socket = 0;
};

#endif // __COMMUNICATION_H__
