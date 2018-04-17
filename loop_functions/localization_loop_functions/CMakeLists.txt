#link_directories(${CMAKE_BINARY_DIR}/controllers/footbot_foraging)
set(localization_loop_functions_SOURCES localization_loop_functions.cpp)

#if(ARGOS_COMPILE_QTOPENGL)
#  set(localization_loop_functions_SOURCES
#    ${localization_loop_functions_SOURCES}
#    localization_qt_user_functions.cpp)
#endif(ARGOS_COMPILE_QTOPENGL)

add_library(localization_loop_functions MODULE ${localization_loop_functions_SOURCES})
target_link_libraries(localization_loop_functions
  footbot_foraging
  argos3core_simulator
  argos3plugin_simulator_dynamics2d
  argos3plugin_simulator_entities
  argos3plugin_simulator_kheperaiv
  argos3plugin_simulator_genericrobot
  argos3plugin_simulator_media)

if(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(localization_loop_functions argos3plugin_simulator_qtopengl)
endif(ARGOS_COMPILE_QTOPENGL)
