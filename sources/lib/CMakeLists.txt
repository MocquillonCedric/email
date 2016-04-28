cmake_minimum_required(VERSION 2.8)

project(MailLib)
set(${PROJECT_NAME}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../include/)

set(Boost_USE_STATIC_LIBS ON) 
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

if(MSVC)
    add_definitions("-D_WIN32_WINNT=0x0601")
endif()

find_package(Boost 1.60 COMPONENTS system date_time regex)
find_package(OpenSSL)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "*.cpp")
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS "${${PROJECT_NAME}_INCLUDE_DIR}/*.h")

file(GLOB_RECURSE PROJECT_FILES ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})
group_sources(${PROJECT_FILES})
set(PROJECT_FILES ${Result})

add_library(MailLib SHARED ${PROJECT_FILES})

include_directories(${${PROJECT_NAME}_INCLUDE_DIR})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${OPENSSL_INCLUDE_DIR})
#if(MSVC)
	target_link_libraries(MailLib ${Boost_LIBRARIES} ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
#else()
#	target_link_libraries(MailLib ${Boost_LIBRARIES} "ws2_32" "mswsock")
#endif()
