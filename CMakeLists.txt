
FIND_PACKAGE(wxWidgets)

INCLUDE(${wxWidgets_USE_FILE})
ADD_EXECUTABLE(desktopstopwatch desktopstopwatch.cpp)
TARGET_LINK_LIBRARIES(desktopstopwatch ${wxWidgets_LIBRARIES})

