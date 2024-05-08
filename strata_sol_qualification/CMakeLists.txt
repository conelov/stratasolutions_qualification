set(name ${PROJECT_NAME})


add_executable(${name}
  main.cpp
  utils.hpp
  Application.hpp
  MainWindow.hpp src/MainWindow.cpp
  ControlBlock.hpp src/ControlBlock.cpp
  Runner.hpp src/Runner.cpp
  ControlBlockMessage.hpp
  RunnerMessage.hpp
  Plot.hpp src/Plot.cpp
)

target_link_libraries(${name} PRIVATE
  Boost::headers
  Qt${QT_VERSION}::Widgets
  range-v3::range-v3
  qcustomplot
)

target_include_directories(${name} PRIVATE
  "${PROJECT_SOURCE_DIR}"
)

set_target_properties(${name} PROPERTIES
  AUTOMOC ON
  AUTOUIC ON
  #  AUTORCC ON
  INCLUDE_CURRENT_DIR ON
  AUTOUIC_SEARCH_PATHS ui
)

target_compile_definitions(${name} PRIVATE
  QT_USE_QSTRINGBUILDER
)

target_compile_definitions(${name} PRIVATE
  PROJECT_NAME="${PROJECT_NAME}"
  PROJECT_VERSION="${PROJECT_VERSION}"
)
