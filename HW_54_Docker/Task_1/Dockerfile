# Используем официальный образ C++ с компилятором
FROM gcc:latest

# Устанавливаем рабочую директорию
WORKDIR /app

# Копируем исходный код
COPY Task_1.cpp .

# Компилируем приложение
RUN g++ -o HelloWorld Task_1.cpp -static

# Запускаем приложение
CMD ["./HelloWorld"]