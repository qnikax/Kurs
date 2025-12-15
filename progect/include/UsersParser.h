#ifndef __USERSPARSER_H__
#define __USERSPARSER_H__

#include <iostream>
#include <unordered_map>
/** @file
* @author Макеев В.Д.
* @version 1.0
* @date 20.12.2023
* @copyright ИБСТ ПГУ
* @warning Курсовая работа студента
* @brief Заголовочный файл для модуля UsersParser
*/


/** @brief Класс UsersParser
* @details Класс UsersParser анализирует файл, содержащий информацию о пользователе.
* @warning Реализация только для типа данных int16_t
*/
class UsersParser{

public:
	/**
     * @brief Разберите файл и сохраните информацию о пользователе в хеш-карте.
     *
     * @param pathToFile Путь к файлу, который нужно проанализировать.
    */
    void parse(std::string pathToFile);
    /**
     * @brief Проверить, существует ли пользователь с указанным именем пользователя
     *
     * @param username Имя пользователя, которого нужно проверить.
     * @return true, если пользователь существует, в противном случае — false
    */
    bool isHasUser(std::string username);
    /**
     * @brief Получить пароль пользователя с указанным именем пользователя
     *
     * @param username Имя пользователя.
     * @return Пароль пользователя
    */
    std::string getPassword(std::string username);

private:

    std::unordered_map<std::string,std::string> m_users;
};

#endif // __USERSPARSER_H__
