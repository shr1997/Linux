cmake_minimum_required(VERSION 3.10)
project(FTP)
set(CMAKE_CXX_STANDARD 11)
add_executable(FTP main.c server_control.h server_control.c link_list.c link_list.h v_check_username.c v_check_username.h)