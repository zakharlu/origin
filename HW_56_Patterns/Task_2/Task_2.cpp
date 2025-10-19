#include "sql_query_builder.h"
#include <iostream>
#include <cassert>

int main() {
    setlocale(LC_ALL, "Russian");
    SqlSelectQueryBuilder query_builder;

    // Создаем сложные условия фильтрации
    std::map<std::string, std::string> complex_conditions = {
        {"age_min", "18"},
        {"age_max", "65"},
        {"country", "Russia"},
        {"subscription", "premium"},
        {"last_login", "2024"}
    };

    query_builder.AddFrom("users")
        .AddColumns({ "username", "email", "created_at" })
        .AddWhere(complex_conditions);

    std::string result = query_builder.BuildQuery();
    // Проверяем, что все условия присутствуют в запросе
    assert(result.find("age_min=18") != std::string::npos);
    assert(result.find("age_max=65") != std::string::npos);
    assert(result.find("country=Russia") != std::string::npos);
    assert(result.find("subscription=premium") != std::string::npos);
    assert(result.find("last_login=2024") != std::string::npos);

    // Выводим результат
    std::cout << "Сгенерированный запрос: " << result << std::endl;
    std::cout << "Запрос корректный!" << std::endl;

    return 0;
}