cmake_minimum_required(VERSION 3.15)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Отключаем автоматические цели ALL_BUILD и ZERO_CHECK
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set(CMAKE_SUPPRESS_REGENERATION true)
endif()

# Установка политики для совместимости с новыми версиями CMake
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# Настройка Boost. Boost лежит в "C:/local/boost_1_89_0"
set(BOOST_ROOT "C:/local/boost_1_89_0")
message(STATUS "Looking for Boost in: ${BOOST_ROOT}")

if(EXISTS "${BOOST_ROOT}")
    message(STATUS "Boost directory exists: ${BOOST_ROOT}")
    
    find_path(BOOST_INCLUDE_DIR
        NAMES boost/asio.hpp
        PATHS "${BOOST_ROOT}"
        PATH_SUFFIXES include
        NO_DEFAULT_PATH
    )
    
    if(BOOST_INCLUDE_DIR)
        message(STATUS "Found Boost include: ${BOOST_INCLUDE_DIR}")
        set(Boost_FOUND TRUE)
        set(Boost_INCLUDE_DIRS ${BOOST_INCLUDE_DIR})
        
        # Проверяем наличие необходимых заголовков
        set(REQUIRED_BOOST_HEADERS asio.hpp beast.hpp)
        set(ALL_HEADERS_FOUND TRUE)
        
        foreach(header ${REQUIRED_BOOST_HEADERS})
            if(NOT EXISTS "${BOOST_INCLUDE_DIR}/boost/${header}")
                message(WARNING "Missing Boost header: boost/${header}")
                set(ALL_HEADERS_FOUND FALSE)
            endif()
        endforeach()
        
        if(ALL_HEADERS_FOUND)
            message(STATUS "All required Boost headers found")
            add_definitions(-DUSE_BOOST_BEAST)
        else()
            message(WARNING "Some Boost headers missing. Using cpp-httplib.")
            set(Boost_FOUND FALSE)
        endif()
    else()
        message(WARNING "Boost headers not found in ${BOOST_ROOT}")
        set(Boost_FOUND FALSE)
    endif()
else()
    message(WARNING "Boost directory not found: ${BOOST_ROOT}")
    set(Boost_FOUND FALSE)
endif()

# Настройка OpenSSL. OpenSSL лежит в "C:/OpenSSL-Win64" 
set(OPENSSL_ROOT_DIR "C:/OpenSSL-Win64")
message(STATUS "Looking for OpenSSL in: ${OPENSSL_ROOT_DIR}")

if(EXISTS "${OPENSSL_ROOT_DIR}")
    message(STATUS "OpenSSL directory exists: ${OPENSSL_ROOT_DIR}")
    
    find_path(OPENSSL_INCLUDE_DIR
        NAMES openssl/ssl.h
        PATHS "${OPENSSL_ROOT_DIR}"
        PATH_SUFFIXES include
        NO_DEFAULT_PATH
    )
    
    if(OPENSSL_INCLUDE_DIR)
        message(STATUS "Found OpenSSL include: ${OPENSSL_INCLUDE_DIR}")
    else()
        message(WARNING "OpenSSL include not found in ${OPENSSL_ROOT_DIR}")
    endif()
    
    # Ищем библиотеки в разных подпапках
    set(OPENSSL_LIB_PATHS
        "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD"  # Ваш путь
        "${OPENSSL_ROOT_DIR}/lib/VC/x64/MT"
        "${OPENSSL_ROOT_DIR}/lib/VC/x64"
        "${OPENSSL_ROOT_DIR}/lib/VC"
        "${OPENSSL_ROOT_DIR}/lib"
    )
    
    find_library(OPENSSL_SSL_LIBRARY
        NAMES ssl libssl ssleay32
        PATHS ${OPENSSL_LIB_PATHS}
        NO_DEFAULT_PATH
    )
    
    if(OPENSSL_SSL_LIBRARY)
        message(STATUS "Found OpenSSL SSL library: ${OPENSSL_SSL_LIBRARY}")
    else()
        message(WARNING "OpenSSL SSL library not found in searched paths")
    endif()
    
    find_library(OPENSSL_CRYPTO_LIBRARY
        NAMES crypto libcrypto libeay32
        PATHS ${OPENSSL_LIB_PATHS}
        NO_DEFAULT_PATH
    )
    
    if(OPENSSL_CRYPTO_LIBRARY)
        message(STATUS "Found OpenSSL Crypto library: ${OPENSSL_CRYPTO_LIBRARY}")
    else()
        message(WARNING "OpenSSL Crypto library not found in searched paths")
    endif()
    
    if(OPENSSL_INCLUDE_DIR AND OPENSSL_SSL_LIBRARY AND OPENSSL_CRYPTO_LIBRARY)
        set(OPENSSL_FOUND TRUE)
        set(OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
        message(STATUS "OpenSSL: FOUND")
    else()
        message(WARNING "OpenSSL not completely found")
        set(OPENSSL_FOUND FALSE)
    endif()
else()
    message(WARNING "OpenSSL directory not found: ${OPENSSL_ROOT_DIR}")
    set(OPENSSL_FOUND FALSE)
endif()

# Поиск PostgreSQL. PostgreSQL лежит в "C:/Program Files/PostgreSQL"
find_path(POSTGRESQL_INCLUDE_DIR
    NAMES libpq-fe.h
    PATHS
        "C:/Program Files/PostgreSQL/*/include"
        "C:/Program Files/PostgreSQL/*/include/server"
)

find_library(POSTGRESQL_LIBRARY
    NAMES libpq libpq.lib
    PATHS
        "C:/Program Files/PostgreSQL/*/lib"
)

if(NOT POSTGRESQL_INCLUDE_DIR OR NOT POSTGRESQL_LIBRARY)
    message(FATAL_ERROR "PostgreSQL not found! Please install PostgreSQL")
endif()

message(STATUS "Found PostgreSQL include: ${POSTGRESQL_INCLUDE_DIR}")
message(STATUS "Found PostgreSQL library: ${POSTGRESQL_LIBRARY}")

# Ручное указание libpqxx. 
set(LIBPQXX_ROOT "C:/libpqxx-installed")
find_path(LIBPQXX_INCLUDE_DIR
    NAMES pqxx/pqxx
    PATHS "${LIBPQXX_ROOT}/include"
)

find_library(LIBPQXX_LIBRARY
    NAMES pqxx
    PATHS "${LIBPQXX_ROOT}/lib"
)

if(NOT LIBPQXX_INCLUDE_DIR OR NOT LIBPQXX_LIBRARY)
    message(FATAL_ERROR "libpqxx not found!")
endif()

message(STATUS "Found libpqxx include: ${LIBPQXX_INCLUDE_DIR}")
message(STATUS "Found libpqxx library: ${LIBPQXX_LIBRARY}")

# Определение для поддержки HTTPS в cpp-httplib
if(OPENSSL_FOUND)
    add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)
    message(STATUS "HTTPS support: ENABLED")
else()
    message(STATUS "HTTPS support: DISABLED")
endif()

# Исходные файлы
add_executable(search_engine
    src/main.cpp
    src/config/config.cpp
    src/database/db.cpp
    src/parser/html_parser.cpp
    src/spider/spider.cpp
    src/search/server.cpp
    lib/httplib.h
)

# Связывание с библиотеками
target_link_libraries(search_engine PRIVATE 
    ${LIBPQXX_LIBRARY}
    ${POSTGRESQL_LIBRARY}
)

# Добавляем Boost
if(Boost_FOUND)
    target_include_directories(search_engine PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(search_engine PRIVATE ${Boost_LIBRARIES})
    message(STATUS "HTTP server: Boost.Beast")
else()
    message(STATUS "HTTP server: cpp-httplib")
endif()

# Добавляем OpenSSL
if(OPENSSL_FOUND)
    target_link_libraries(search_engine PRIVATE 
        ${OPENSSL_SSL_LIBRARY}
        ${OPENSSL_CRYPTO_LIBRARY}
    )
    target_include_directories(search_engine PRIVATE ${OPENSSL_INCLUDE_DIR})
endif()

# Для Windows
if(WIN32)
    target_link_libraries(search_engine PRIVATE 
        ws2_32 
        wsock32 
        crypt32
        advapi32
    )
endif()

# Включаем директории
target_include_directories(search_engine PRIVATE 
    ${POSTGRESQL_INCLUDE_DIR}
    ${LIBPQXX_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
)

# Настройки компилятора
if(MSVC)
    target_compile_options(search_engine PRIVATE /W3 /O2 /std:c++17)
endif()

message(STATUS "=== КОНФИГУРАЦИЯ ЗАВЕРШЕНА ===")
message(STATUS "PostgreSQL: ${POSTGRESQL_INCLUDE_DIR}")
message(STATUS "libpqxx: ${LIBPQXX_INCLUDE_DIR}")
if(Boost_FOUND)
    message(STATUS "Boost: ${BOOST_INCLUDE_DIR}")
else()
    message(STATUS "Boost: не найден")
endif()
if(OPENSSL_FOUND)
    message(STATUS "OpenSSL: найден")
else()
    message(STATUS "OpenSSL: не найден")
endif()
message(STATUS "Проект готов к сборке!")