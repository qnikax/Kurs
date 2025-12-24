#include "CmdLine.h"
#include "ServerError.h"

void CmdLine::printHelp()
{
    std::cout << "USAGE server [parameters]" << std::endl;
    std::cout << "Default values:" << std::endl;
    std::cout << "  --port/-p : 33333" << std::endl;
    std::cout << "  --file/-f : ./scale.conf" << std::endl;
    std::cout << "  --logs/-l : ./log/scale.log" << std::endl;
    std::cout << std::endl;

    for(auto curParam : m_params){
        std::cout << "\t" << curParam.first << "  or  " << curParam.second.shortName << "  ";
        std::cout << curParam.second.description << std::endl;
    }
}

std::string CmdLine::getNextArg(int argc,char** argv)
{
    if(argc > m_curArgIndx)
        return std::string(argv[m_curArgIndx++]);
    return std::string();
}

bool CmdLine::isHasParam(std::string name)
{
    return m_params.find(name) != m_params.end();   
}

bool CmdLine::isHasParamShort(std::string name)
{
    return m_paramsShort.find(name) != m_paramsShort.end();
}

bool CmdLine::isnumber(std::string str)
{
    if(str.empty()) return false;
    for (const auto &ch : str){
        if (!std::isdigit(ch))
            return false;
    }
    return true;
}

// ВАЖНО: здесь должен быть & после param!
void CmdLine::modifyParam(param& parameter, std::string curArg)
{
    if(curArg.empty())
        throw ServerError("Invalid argument input", ErrorLevel::Critical);

    if(parameter.type == param_type::integer){
        if(!isnumber(curArg))
            throw ServerError("Invalid argument type", ErrorLevel::Critical);
                
        parameter.valInt = std::stoi(curArg);
        parameter.valStr = curArg;
    }
    else{
        parameter.valStr = curArg;
    }

    m_params[parameter.name] = parameter;
    m_paramsShort[parameter.shortName] = parameter;
}

void CmdLine::parse(int argc, char** argv)
{
    m_curArgIndx = 1;
    m_isHasHelp = false;
    
    // Разрешаем запуск без параметров или только с help
    if(argc == 1) {
        // Запуск без параметров - показываем справку
        printHelp();
        m_isHasHelp = true;
        return;
    }
    
    std::string curArg = getNextArg(argc, argv);

    while (!curArg.empty()){
        if(curArg == "--help" || curArg == "-h"){
            printHelp();
            m_isHasHelp = true;
            return;
        }
        else if(isHasParam(curArg)){
            param& _param = m_params[curArg];
            curArg = getNextArg(argc, argv);   
            
            if(curArg.empty()) {
                throw ServerError("Missing value for parameter: " + _param.name, ErrorLevel::Critical);
            }
            
            modifyParam(_param, curArg);
        }
        else if(isHasParamShort(curArg)){
            param& _param = m_paramsShort[curArg];
            curArg = getNextArg(argc, argv);
            
            if(curArg.empty()) {
                throw ServerError("Missing value for parameter: " + _param.name, ErrorLevel::Critical);
            }
            
            modifyParam(_param, curArg);
        }
        else {
            throw ServerError("Invalid argument: " + curArg, ErrorLevel::Critical);
        }

        curArg = getNextArg(argc, argv);
    }
}

void CmdLine::addParam(std::string name, std::string shortName, std::string description, param_type type)
{
    param p;
    p.name = name;
    p.shortName = shortName;
    p.type = type;
    p.description = description;
    p.valInt = 0;
    p.valStr = "";

    m_params.insert(std::make_pair(name, p));
    m_paramsShort.insert(std::make_pair(shortName, p));
}

int CmdLine::getInt(std::string name){
    if(!isHasParam(name))
        return 0;
    return m_params[name].valInt;
}

std::string CmdLine::getString(std::string name)
{
    if(!isHasParam(name))
        return std::string();
    return m_params[name].valStr;
}
