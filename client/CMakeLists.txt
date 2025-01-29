cmake_minimum_required(VERSION 3.0)
project(clientServer)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp connection/client.cpp)

SET(EXECUTABLE_OUTPUT_PATH ${clientServer_SOURCE_DIR}/bin)
add_executable(runClient ${SOURCE_FILES})

install(FILES
  ${clientServer_SOURCE_DIR}/scripts/restartClientServer.sh
  DESTINATION ${clientServer_SOURCE_DIR}/bin/)

