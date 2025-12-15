#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <iostream>
#include <fstream>
#include "ServerError.h"
/** @file
* @author Макеев В.Д.
* @version 1.0
* @date 20.12.2023
* @copyright ИБСТ ПГУ
* @warning Курсовая работа студента
* @brief Заголовочный файл для модуля Logger
*/


/** @brief Класс Logger
* @details Класс Logger отвечает за регистрацию информации и ошибок сервера.
* @warning Реализация только для типа данных int16_t
*/

class Logger{
    public:
    	/**
     	* @brief Инициализирует регистратор с указанным путем к журналу.
     	* @param logPath Путь к файлу журнала.
    	* @return True, если регистратор успешно инициализирован, в противном случае — false.
     	*/
        bool Initialize(std::string logPath);
		/**
     	* @brief Регистрирует информационное сообщение.
     	* @param str Информационное сообщение, которое необходимо записать в журнал.
     	*/
        void info(std::string str);
        /**
     	* @brief Регистрирует ошибку сервера.
     	* @param serverError Ошибка сервера, которая должна быть зарегистрирована.
     	*/
        void serverError(ServerError serverError);
		/**
     	* @brief Возвращает ссылку на одноэлементный экземпляр средства ведения журнала.
     	* @return Ссылка на экземпляр регистратора
     	*/
        static Logger &get();
        /**
     	* @brief Возвращает указатель на одноэлементный экземпляр средства ведения журнала.
     	* @return Указатель на экземпляр регистратора
     	*/
        static Logger *getPtr();

    private:
        static Logger* m_state;

        std::string m_pathToFile;
        std::ofstream m_logFile;
};
#endif // __LOGGER_H__
