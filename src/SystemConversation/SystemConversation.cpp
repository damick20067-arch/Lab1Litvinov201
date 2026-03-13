#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <memory>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/utsname.h>
#endif

std::string getOSName() {
#ifdef _WIN32
    return "Windows";
#elif _linux_
    return "Linux";
#elif _APPLE_
    return "macOS";
#else
    return "Unknown";
#endif
}

std::string getOSVersion() {
#ifdef _WIN32
    char buffer[256];
    DWORD bufferSize = sizeof(buffer);
    if (GetComputerNameA(buffer, &bufferSize)) {
        return "Windows";
    }
    return "Windows";
#elif _linux_
    struct utsname sysInfo;
    if (uname(&sysInfo) == 0) {
        return std::string(sysInfo.sysname) + " " + sysInfo.release;
    }
    return "Linux";
#else
    return "Unknown";
#endif
}

std::string getCurrentUser() {
#ifdef _WIN32
    char username[256];
    DWORD usernameLen = sizeof(username);
    if (GetUserNameA(username, &usernameLen)) {
        return std::string(username);
    }
    return "Unknown";
#else
    char* user = getenv("USER");
    if (user) return std::string(user);
    return "Unknown";
#endif
}

std::string getEnvVar(const std::string& name) {
#ifdef _WIN32
    // Безопасный способ для Windows
    char* buffer = nullptr;
    size_t size = 0;
    if (_dupenv_s(&buffer, &size, name.c_str()) == 0 && buffer != nullptr) {
        std::string result(buffer);
        free(buffer);
        return result;
    }
    return "";
#else
    // Для Linux оставляем getenv (там нет такого предупреждения)
    const char* val = std::getenv(name.c_str());
    return val ? val : "";
#endif
}

std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

int main() {
    // Используем безопасное получение переменной окружения
    std::string config = getEnvVar("MY_LAB_CONFIG");

    std::cout << "=== Лабораторная работа ===\n";
    std::cout << "MY_LAB_CONFIG = " << (config.empty() ? "<не задана>" : config) << "\n\n";

    // Системная информация
    std::cout << "ОС: " << getOSName() << "\n";
    std::cout << "Версия: " << getOSVersion() << "\n";
    std::cout << "Пользователь: " << getCurrentUser() << "\n\n";

    // Проверка конфигурации
    std::string configUpper = toUpper(config);

    if (configUpper == "WIN") {
        std::cout << "Добро пожаловать в Windows-версию программы!\n";

#ifdef _WIN32
        // Дополнительная информация для Windows
        char systemRoot[256];
        DWORD systemRootSize = sizeof(systemRoot);
        if (GetEnvironmentVariableA("SystemRoot", systemRoot, systemRootSize)) {
            std::cout << "SystemRoot: " << systemRoot << "\n";
        }
#endif
    }
    else if (configUpper == "LINUX") {
        std::cout << "Добро пожаловать в Linux-версию программы!\n";

#ifdef _linux_
        // Дополнительная информация для Linux
        char* home = getenv("HOME");
        if (home) {
            std::cout << "Home: " << home << "\n";
        }
#endif
    }
    else {
        std::cout << "ПРЕДУПРЕЖДЕНИЕ: Неизвестная конфигурация!\n";
        std::cout << "Ожидаемые значения: WIN или LINUX\n";
    }

    return 0;
}