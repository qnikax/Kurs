#ifndef __CMDLINE_H__
#define __CMDLINE_H__

#include <iostream>
#include <unordered_map>

/** 
* @file
* @author Макеев В.Д.
* @version 1.0
* @date 20.12.2023
* @copyright ИБСТ ПГУ
* @warning Курсовая работа студента
* @brief Заголовочный файл для модуля CmdLine
*/


/** 
* @brief Класс CmdLine
* @details Класс CmdLine позволяет добавлять параметры, анализировать аргументы командной строки и получать значения параметров
* @warning Реализация только для типа данных int16_t
*/
/**
 * @brief Перечисление типов параметров
 * 
 * Это перечисление определяет типы параметров, которые может обрабатывать класс CmdLine.
*/
enum param_type{
    string,
    integer,
};

class CmdLine{
	/**
     * @brief Структура, представляющая параметр.
     * 
     * Эта структура хранит информацию о параметре, такую ​​как его имя, краткое имя, значение, описание и тип.
	*/
    struct param{
        std::string name;
        std::string shortName;
        std::string valStr;
        std::string description;
        
        param_type type;
        int valInt;
    };

public:
	/**
     * @brief Добавляет параметр к объекту CmdLine.
     * @param name Имя параметра.
     * @param shortName Краткое имя (один символ) параметра.
     * @param описание Описание параметра.
     * @param type Тип параметра.
	*/
    void addParam(std::string name, std::string shortName,
                  std::string description = std::string(),param_type type = param_type::string);
	/**
     * @brief Анализирует аргументы командной строки.
     * @param argc Количество аргументов командной строки.
     * @param argv Массив строк аргументов командной строки.
	*/
    void parse(int argc, char** argv);
    /**
     * @brief Выводит справочное сообщение.
    */
    void printHelp();
    /**
     * @brief Возвращает целочисленное значение указанного параметра.
     * @param name Имя параметра.
     * @return Целочисленное значение параметра.
    */
    int getInt(std::string name);
    /**
     * @brief Возвращает строковое значение указанного параметра.
     * @param name Имя параметра.
     * @return Строковое значение параметра.
     */
    std::string getString(std::string name);
	/**
     * @brief Проверяет, присутствует ли опция «--help» или «-h».
     * @return True, если присутствует опция справки, в противном случае — false.
    */
    bool isHasHelp(){return m_isHasHelp;}
private:
	/**
     * @brief Получает следующий аргумент командной строки.
     * @param argc Количество аргументов командной строки.
     * @param argv Массив строк аргументов командной строки.
     * @return Следующий аргумент командной строки.
     */
    std::string getNextArg(int argc,char** argv);
    /**
     * @brief Проверяет, присутствует ли указанный параметр.
     * @param name Имя параметра.
     * @return True, если параметр присутствует, в противном случае — false.
    */
    bool isHasParam(std::string name);
    /**
     * @brief Проверяет, присутствует ли указанный короткий параметр.
     * @param name Краткое имя параметра.
     * @return True, если присутствует параметр short, в противном случае — false.
    */
    bool isHasParamShort(std::string name);
	/**
     * @brief Проверяет, представляет ли строка число.
     * @param str Строка для проверки.
     * @return True, если строка представляет число, в противном случае — false.
     */
    bool isnumber(std::string str);
    /**
     * @brief Изменяет параметр на основе текущего аргумента.
     * @param параметр Параметр, который нужно изменить.
     * @param curArg Текущий аргумент.
     */
    void modifyParam(param parameter, std::string curArg);

    std::unordered_map<std::string,param> m_params;
    std::unordered_map<std::string,param> m_paramsShort;


    int m_curArgIndx = 1;
    bool m_isHasHelp = false;
};

#endif // __CMDLINE_H__
