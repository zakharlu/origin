#include "sql_query_builder.h"
#include <iostream>
#include <cassert>

int main() {
    setlocale(LC_ALL, "Russian");
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");

    std::string result = query_builder.BuildQuery();
    std::string expected = "SELECT name, phone FROM students WHERE id=42 AND name=John;";

    // Проверяем соответствие
    assert(result == expected);

    // Выводим результат
    std::cout << "Сгенерированный запрос: " << result << std::endl;
    std::cout << "Запрос корректный!" << std::endl;

    return 0;
}