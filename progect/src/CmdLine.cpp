#include "CmdLine.h"

#include "ServerError.h"

void CmdLine::printHelp()
{
    std::cout<<"USAGE server [parameters]"<<std::endl;

    for(auto curParam:m_params){
        std::cout<<"\t"<<curParam.first<<"  or  "<<curParam.second.shortName<<"  ";
        std::cout<<curParam.second.description<<std::endl;
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
    for (const auto &ch : str){
        if (!std::isdigit(ch))
            return false;
    }
    return true;
}

void CmdLine::modifyParam(param parameter, std::string curArg)
{
    if(curArg.length() == 0)
        throw ServerError("Invalid argument input",ErrorLevel::Critical);

    if(parameter.type == param_type::integer){
        if(!isnumber(curArg))
            throw ServerError("Invalid argument type",ErrorLevel::Critical);
                
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
    if(argc < m_params.size())
        throw ServerError("Invalid usage");
    std::string curArg = getNextArg(argc,argv);

    while (curArg.length()){

        if(isHasParam(curArg)){
            param _param = m_params[curArg];
            curArg = getNextArg(argc,argv);   
            
            modifyParam(_param,curArg);
        }
        else if(isHasParamShort(curArg)){
            param _param = m_paramsShort[curArg];
            curArg = getNextArg(argc,argv);
            
            modifyParam(_param,curArg);
        }
        else if(curArg == "--help" || curArg == "-h"){
            printHelp();
        }
        else 
            throw ServerError("Invalid argument",ErrorLevel::Critical);

        curArg = getNextArg(argc,argv);
    }
    
}

void CmdLine::addParam(std::string name, std::string shortName,std::string description,param_type type)
{
    param p;

    p.name = name;
    p.shortName = shortName;
    p.type = type;
    p.description = description;

    m_params.insert(std::make_pair(name,p));
    m_paramsShort.insert(std::make_pair(shortName,p));

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
