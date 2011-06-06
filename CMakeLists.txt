project(KReminder)
cmake_minimum_required(VERSION 2.6)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES} ${QT_INCLUDES})

set(KReminder_SRCS main.cpp ButtonBox/kreminderbuttonbox.cpp Introduction/introwindow.cpp Reminder/newreminderwindow.cpp)
kde4_add_executable(KReminder ${KReminder_SRCS})
target_link_libraries(KReminder ${KDE4_KDEUI_LIBS})

install(TARGETS KReminder DESTINATION ${BIN_INSTALL_DIR})
install(FILES KReminderui.rc DESTINATION ${DATA_INSTALL_DIR}/KReminder)