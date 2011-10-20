project(KReminder)
cmake_minimum_required(VERSION 2.6)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES} ${QT_INCLUDES})

set(KReminder_SRCS ErrorHandling/errorhandling.cpp main.cpp Introduction/introwindow.cpp Reminder/newreminderwindow.cpp)
kde4_add_executable(KReminder ${KReminder_SRCS})
target_link_libraries(KReminder ${KDE4_KDEUI_LIBS})
install(TARGETS KReminder DESTINATION ${BIN_INSTALL_DIR})
install(FILES KReminderui.rc DESTINATION ${DATA_INSTALL_DIR}/KReminder)

kde4_add_executable(kreminder_helper  KAuthHelper/kreminderhelper.cpp)
target_link_libraries(kreminder_helper ${KDE4_KDECORE_LIBS})
install(TARGETS kreminder_helper DESTINATION ${LIBEXEC_INSTALL_DIR})
kde4_install_auth_helper_files(KReminder org.kde.auth.kreminder root)

kde4_install_auth_actions(org.kde.auth.kreminder org.kde.auth.kreminder.actions)