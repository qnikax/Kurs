#include "Server.h"
#include <unistd.h>
#include <stdint.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <vector>
#include <cstdlib>
#include <sys/stat.h>

bool Server::Initialize(int argc,char** argv)
{
    m_cmd.addParam("--port", "-p", "<port> , port must be less than 65536", param_type::integer);
    m_cmd.addParam("--file", "-f", "<users File>");
    m_cmd.addParam("--logs", "-l", "<logs File>");
    
    try
    {
        m_cmd.parse(argc, argv);
    }
    catch (ServerError err)
    {
        // Только сообщение об ошибке, справку parse() уже показал
        std::cout << "Error: " << err.what() << std::endl;
        return false;
    }

    // Если показали справку - завершаем
    if (m_cmd.isHasHelp())
    {
        return false;  // <-- ИЗМЕНЕНО: false при выводе справки
    }

    // Устанавливаем значения по умолчанию из ТЗ
    std::string logPath = m_cmd.getString("--logs");
    if (logPath.empty()) {
        logPath = "./log/scale.log";
        // Создаем директорию если нужно
        system("mkdir -p ./log 2>/dev/null");
    }
    
    std::string usersPath = m_cmd.getString("--file");
    if (usersPath.empty()) {
        usersPath = "./scale.conf";
    }
    
    int port = m_cmd.getInt("--port");
    if (port == 0) {
        port = 33333;  // Значение по умолчанию из ТЗ
    }
    
    // Проверяем порт
    if (port < 1 || port > 65535) {
        std::cout << "Error: Port must be between 1 and 65535" << std::endl;
        return false;
    }

    try
    {
        if (!Logger::get().Initialize(logPath)) {
            std::cout << "Failed to initialize logger with path: " << logPath << std::endl;
            return false;
        }
        
        m_usersParser.parse(usersPath);
        m_com.createSocket(port);
    }
    catch (ServerError err)
    {
        Logger::get().serverError(err);
        return false;
    }

    m_mustDie = false;
    Logger::get().info("Initialization successful");
    
    return true;
}

void Server::start()
{
    int client = 0;

    while (!m_mustDie)
    {
        try
        {
            client = m_com.waitClient();
            m_com.autorize(client, m_usersParser);
            executeCalculations(client);
            close(client);
        }
        catch (ServerError err)
        {
            Logger::get().serverError(err);
            if (err.getErrorLevel() == ErrorLevel::Critical)
                m_mustDie = true;

            if (client > 0) {
                close(client);
            }
        }
    }
}

void Server::executeCalculations(int client)
{
    uint32_t numOfVectors;
    uint32_t vectorLength;
    std::vector<int16_t> vector;

    // Получаем количество векторов
    ssize_t bytesRead = recv(client, &numOfVectors, sizeof(numOfVectors), 0);
    if (bytesRead != sizeof(numOfVectors)) {
        throw ServerError("Failed to receive number of vectors", ErrorLevel::Warning);
    }

    for (uint32_t i = 0; i < numOfVectors; i++)
    {
        // Получаем размер вектора
        bytesRead = recv(client, &vectorLength, sizeof(vectorLength), 0);
        if (bytesRead != sizeof(vectorLength)) {
            throw ServerError("Failed to receive vector size", ErrorLevel::Warning);
        }

        // Проверяем разумный размер
        if (vectorLength > 1000000) { // Максимальный размер 1 млн элементов
            throw ServerError("Vector size too large", ErrorLevel::Warning);
        }

        vector.resize(vectorLength);
        
        // Получаем данные вектора
        size_t totalBytes = vectorLength * sizeof(int16_t);
        size_t receivedBytes = 0;
        
        while (receivedBytes < totalBytes) {
            bytesRead = recv(client, 
                           reinterpret_cast<char*>(vector.data()) + receivedBytes,
                           totalBytes - receivedBytes, 0);
            if (bytesRead <= 0) {
                throw ServerError("Failed to receive vector data", ErrorLevel::Warning);
            }
            receivedBytes += bytesRead;
        }

        // Вычисляем сумму с проверкой переполнения
        int32_t sum32 = 0;  // Используем 32-бит для промежуточных вычислений
        for (const auto &vecEl : vector) {
            sum32 += vecEl;
            
            // Проверка переполнения int16_t
            if (sum32 > 32767) {
                sum32 = 32767;
            } else if (sum32 < -32768) {
                sum32 = -32768;
            }
        }
        
        int16_t sum = static_cast<int16_t>(sum32);

        // Отправляем результат
        send(client, &sum, sizeof(sum), 0);
    }
}
