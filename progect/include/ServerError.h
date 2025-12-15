#ifndef __SERVERERROR_H__
#define __SERVERERROR_H__

#include <stdexcept>
/** @file
* @author Макеев В.Д.
* @version 1.0
* @date 20.12.2023
* @copyright ИБСТ ПГУ
* @warning Курсовая работа студента
* @brief Заголовочный файл для модуля ServerError
*/


/** @brief Класс ServerError
* @details Класс ServerError отвечает за обработку ошибок сервера
* @warning Реализация только для типа данных int16_t
*/
enum ErrorLevel
{
    Warning,
    Critical
};

class ServerError : public std::runtime_error
{

public:
	/**@brief Конструктор с сообщением об ошибке.
     * @param msg Сообщение об ошибке.
     */
    ServerError(const char *msg) : std::runtime_error(msg){};
    /** @brief Конструктор с сообщением об ошибке и уровнем ошибки.
     * @param msg Сообщение об ошибке.
     * @param level Уровень ошибки.
     */
    ServerError(const std::string &msg, ErrorLevel level = ErrorLevel::Warning) : std::runtime_error(msg),
                                                            m_errorLevel(level){

                                                            };
	/** @brief Получаем уровень ошибки.
     * @return Уровень ошибки.
     */
    ErrorLevel getErrorLevel() {return m_errorLevel;}

private:
    ErrorLevel m_errorLevel;
};

#endif // __SERVERERROR_H__
