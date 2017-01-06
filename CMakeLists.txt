cmake_minimum_required(VERSION 3.5)

  project (ecs)

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)

  set(SOURCES_ECS
    ${CMAKE_CURRENT_SOURCE_DIR}/srcs/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/srcs/GameEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/srcs/EntityManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/srcs/SystemManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/srcs/MessageBus.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/srcs/ASystem.cpp
  )

  add_executable(ecs ${SOURCES_ECS})
  IF (WIN32)
  ELSEIF (UNIX AND NOT MINGW)
	target_link_libraries(ecs dl)
  ENDIF()
