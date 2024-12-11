cmake_minimum_required(VERSION 3.14)

project(Server_Gammatek LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QT
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network)

# Protobuf
option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "gRPC_FOUND: ${gRPC_FOUND}")
message(STATUS "Protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
find_program(_PROTOBUF_PROTOC protoc REQUIRED)

# gRPC
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Protobuf_FOUND: ${Protobuf_FOUND}")
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin REQUIRED)

# Generation proto
set(proto_file "${CMAKE_CURRENT_SOURCE_DIR}/proto/api.proto")
get_filename_component(proto_path "${proto_file}" PATH)

set(proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/api.pb.cc")
set(proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/api.pb.h")
set(grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/api.grpc.pb.cc")
set(grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/api.grpc.pb.h")

add_custom_command(
  OUTPUT ${proto_srcs} ${proto_hdrs} ${grpc_srcs} ${grpc_hdrs}
  COMMAND ${_PROTOBUF_PROTOC}
  ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
       --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
       -I "${proto_path}"
       --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
       "${proto_file}"
  DEPENDS "${proto_file}"
  COMMENT "Generating gRPC and protobuf files"
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# Build
add_library(grpc_proto
  ${grpc_srcs}
  ${grpc_hdrs}
  ${proto_srcs}
  ${proto_hdrs})

target_link_libraries(grpc_proto
  absl::check
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

foreach(_target Client Server)
    add_executable(${_target} "${_target}.cpp")

  if(_target STREQUAL "Server")
    target_sources(${_target} PRIVATE
    ServerWidget.h ServerWidget.cpp
    ServerLogic.h ServerLogic.cpp
    GrpcServer.h GrpcServer.cpp
    UdpServer.h UdpServer.cpp)

  elseif(_target STREQUAL "Client")
    target_sources(${_target} PRIVATE
        ClientWidget.h ClientWidget.cpp
        UdpClient.h UdpClient.cpp
        ClientLogic.h ClientLogic.cpp
        GrpcClient.h GrpcClient.cpp
        TableModel.h TableModel.cpp
        ButtonDelegate.h ButtonDelegate.cpp)
  endif()

  target_link_libraries(${_target}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    grpc_proto
    absl::check
    absl::flags
    absl::flags_parse
    absl::log
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()

# include(GNUInstallDirs)
# install(TARGETS Server_Gammatek
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
# )
