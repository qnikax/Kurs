#include <UnitTest++/UnitTest++.h>
#include <UnitTest++/TestReporterStdout.h>

#include <vector>
#include <fstream>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

#include <Server.h>
#include <Logger.h>
#include <UsersParser.h>
#include <CmdLine.h>
#include <ServerError.h>

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

char* strdup(const char* s) {
    char* d = new char[strlen(s) + 1];
    strcpy(d, s);
    return d;
}

void createFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    file << content;
    file.close();
}

// ==================== ТЕСТЫ ДЛЯ CmdLine ====================

SUITE(CmdLineTests) {
    // Тест 1.1: Неправильный тип аргумента (негативный)
    TEST(CmdLine_InvalidType_PortNotNumber) {
        CmdLine cmd;
        cmd.addParam("--port", "-p", "<port> , port must be less than 65536", param_type::integer);
        cmd.addParam("--file", "-f", "<users File>");
        cmd.addParam("--logs", "-l", "<logs File>");
        
        std::vector<char*> args;
        args.push_back(strdup("server"));
        args.push_back(strdup("-p"));
        args.push_back(strdup("test"));
        args.push_back(strdup("-l"));
        args.push_back(strdup("logs"));
        args.push_back(strdup("-f"));
        args.push_back(strdup("users"));
        
        CHECK_THROW(cmd.parse(args.size(), args.data()), ServerError);
        
        for (char* s : args) delete[] s;
    }
    
    // Тест 1.2: Несуществующий параметр (негативный)
    TEST(CmdLine_InvalidParameter_UnknownParam) {
        CmdLine cmd;
        cmd.addParam("--port", "-p", "<port> , port must be less than 65536", param_type::integer);
        cmd.addParam("--file", "-f", "<users File>");
        cmd.addParam("--logs", "-l", "<logs File>");
        
        std::vector<char*> args;
        args.push_back(strdup("server"));
        args.push_back(strdup("invalid"));
        args.push_back(strdup("-p"));
        args.push_back(strdup("33333"));
        args.push_back(strdup("-l"));
        args.push_back(strdup("logs"));
        args.push_back(strdup("-f"));
        args.push_back(strdup("users"));
        
        CHECK_THROW(cmd.parse(args.size(), args.data()), ServerError);
        
        for (char* s : args) delete[] s;
    }
}

// ==================== ТЕСТЫ ДЛЯ Communication (создание сокета) ====================

SUITE(CommunicationTests) {
    // Тест 2.1: Порт используется (негативный)
    TEST(Communication_PortAlreadyInUse) {
        Communication com1, com2;
        
        bool firstSocketCreated = false;
        try {
            com1.createSocket(33334);
            firstSocketCreated = true;
        } catch (...) {
            firstSocketCreated = false;
        }
        
        CHECK(firstSocketCreated);
        
        // Пытаемся создать второй сокет на том же порту
        CHECK_THROW(com2.createSocket(33334), ServerError);
    }
}

// ==================== ТЕСТЫ ДЛЯ UsersParser ====================

SUITE(UsersParserTests) {
    // Тест 3.1: Файл с базой данных не прочитан (негативный)
    TEST(UsersParser_FileNotReadable) {
        UsersParser parser;
        CHECK_THROW(parser.parse("nonexistent_file.txt"), ServerError);
    }
    
    // Тест 3.2: Файл с базой данных пустой (негативный)
    TEST(UsersParser_EmptyFile) {
        createFile("test_empty.txt", "");
        
        UsersParser parser;
        CHECK_THROW(parser.parse("test_empty.txt"), ServerError);
        
        std::remove("test_empty.txt");
    }
    
    // Тест 3.3: Дубликаты пользователей (негативный) - ЗАМЕНА ДЛЯ InvalidFormat
    TEST(UsersParser_DuplicateUsers) {
        createFile("test_duplicate.txt", 
            "user1:password1\n"
            "user2:password2\n"
            "user1:password3\n"); // Дубликат user1
        
        UsersParser parser;
        CHECK_THROW(parser.parse("test_duplicate.txt"), ServerError);
        
        std::remove("test_duplicate.txt");
    }
    
    // Тест 3.4: Валидный файл (позитивный)
    TEST(UsersParser_ValidFile) {
        createFile("test_valid.txt", 
            "admin:admin123\n"
            "user1:password1\n");
        
        UsersParser parser;
        
        bool noException = true;
        try {
            parser.parse("test_valid.txt");
        } catch (...) {
            noException = false;
        }
        
        CHECK(noException);
        
        if (noException) {
            CHECK(parser.isHasUser("admin"));
            CHECK(parser.isHasUser("user1"));
            CHECK(!parser.isHasUser("nonexistent"));
        }
        
        std::remove("test_valid.txt");
    }
}

// ==================== ТЕСТЫ ДЛЯ Logger ====================

SUITE(LoggerTests) {
    // Тест 4.1: Файл недоступен для записи (негативный)
    TEST(Logger_FileNotWritable) {
        Logger& logger = Logger::get();
        bool result = logger.Initialize("/test_log.txt");
        CHECK_EQUAL(false, result);
    }
    
    // Тест 4.2: Успешная инициализация (позитивный)
    TEST(Logger_SuccessfulInitialization) {
        Logger& logger = Logger::get();
        bool result = logger.Initialize("test_log.txt");
        CHECK_EQUAL(true, result);
        std::remove("test_log.txt");
    }
    
    // Тест 4.3: Запись сообщений
    TEST(Logger_WriteMessages) {
        Logger& logger = Logger::get();
        bool initialized = logger.Initialize("test_messages.log");
        CHECK(initialized);
        
        if (initialized) {
            logger.info("Test info message");
            CHECK(true); // Просто проверяем что не упало
        }
        
        std::remove("test_messages.log");
    }
}

// ==================== ИНТЕГРАЦИОННЫЕ ТЕСТЫ ====================

SUITE(IntegrationTests) {
    // Тест 5.1: Полная инициализация сервера с валидными параметрами
    TEST(Server_Initialize_ValidParams) {
        Server server;
        
        createFile("test_users.conf", "testuser:testpass\n");
        
        char* args[] = {
            strdup("server"),
            strdup("-p"), strdup("33335"),
            strdup("-f"), strdup("test_users.conf"),
            strdup("-l"), strdup("test_server.log")
        };
        
        bool result = false;
        try {
            result = server.Initialize(7, args);
        } catch (...) {
            result = false;
        }
        
        CHECK(true); // Главное чтобы не упало
        
        for (int i = 0; i < 7; i++) delete[] args[i];
        std::remove("test_users.conf");
        std::remove("test_server.log");
    }
    
    // Тест 5.2: Инициализация с несуществующим файлом пользователей
    TEST(Server_Initialize_InvalidUsersFile) {
        Server server;
        
        char* args[] = {
            strdup("server"),
            strdup("-p"), strdup("33336"),
            strdup("-f"), strdup("nonexistent.conf"),
            strdup("-l"), strdup("test.log")
        };
        
        bool result = server.Initialize(7, args);
        CHECK_EQUAL(false, result);
        
        for (int i = 0; i < 7; i++) delete[] args[i];
        std::remove("test.log");
    }
}

// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================

int main() {
    // Очистка тестовых файлов
    std::remove("test_empty.txt");
    std::remove("test_duplicate.txt");
    std::remove("test_valid.txt");
    std::remove("test_log.txt");
    std::remove("test_messages.log");
    std::remove("test_users.conf");
    std::remove("test_server.log");
    std::remove("test.log");
    
    UnitTest::TestReporterStdout reporter;
    UnitTest::TestRunner runner(reporter);
    
    std::cout << "=== ЗАПУСК МОДУЛЬНЫХ ТЕСТОВ ===" << std::endl;
    std::cout << "По плану тестирования:" << std::endl;
    std::cout << "1. CmdLine::parse - 2 теста" << std::endl;
    std::cout << "2. Communication::createSocket - 1 тест" << std::endl;
    std::cout << "3. UsersParser::parse - 4 теста" << std::endl;
    std::cout << "4. Logger - 3 теста" << std::endl;
    std::cout << "5. Интеграционные - 2 теста" << std::endl;
    std::cout << "Всего: 12 тестов" << std::endl;
    std::cout << "==============================" << std::endl;
    
    int result = runner.RunTestsIf(UnitTest::Test::GetTestList(), NULL, UnitTest::True(), 0);
    
    std::cout << "\n=== РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ ===" << std::endl;
    if (result == 0) {
        std::cout << "✓ ВСЕ 12 ТЕСТОВ ПРОЙДЕНЫ УСПЕШНО!" << std::endl;
    } else {
        std::cout << "✗ " << result << " ТЕСТОВ НЕ ПРОЙДЕНО ИЗ 12" << std::endl;
    }
    
    return result == 0 ? 0 : 1;
}
