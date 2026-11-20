cmake_minimum_required(VERSION 3.16)

project(project_1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Gui Qml ShaderTools)
find_package(Qt6 COMPONENTS GuiPrivate QUIET)

# Vulkan SDK (в ./tools или через переменную окружения VULKAN_SDK)
if(NOT DEFINED ENV{VULKAN_SDK})
    file(GLOB _vulkan_sdk_candidates "${CMAKE_CURRENT_SOURCE_DIR}/../tools/vulkan-*/*")
    foreach(_vk IN LISTS _vulkan_sdk_candidates)
        if(EXISTS "${_vk}/include/vulkan/vulkan.h")
            set(ENV{VULKAN_SDK} "${_vk}")
            list(APPEND CMAKE_PREFIX_PATH "${_vk}")
            list(APPEND CMAKE_LIBRARY_PATH "${_vk}/lib" "${_vk}/lib/VulkanLoader/lib")
            break()
        endif()
    endforeach()
endif()
find_package(Vulkan REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appproject_1
    main.cpp
    src/graphics/vulkan/VulkanItem.cpp
    src/graphics/camera/Camera.cpp
    src/controls/CameraController.cpp
    src/controls/keyboard/KeyboardInput.cpp
    src/controls/mouse/MouseInput.cpp
    src/graphics/vulkan/VulkanWindow.cpp
)

qt_add_resources(appproject_1 "app"
    PREFIX "/"
    FILES
        Main.qml

        Constants/Colors.qml
        Constants/ButtonToolbarWindowActions.qml
        Constants/SizeConsts.qml
        Constants/StringConsts.qml
        Constants/Spaces.qml
        Constants/qmldir

        qml/CommonElements/ButtonNewPage.qml
        qml/WindowComponents/TopWindowToolbar.qml
        qml/WindowComponents/WindowResizebleItem.qml
        qml/CommonElements/ButtonToolbarWindow.qml

        qml/Pages/generalPage/GeneralPage.qml
        qml/Pages/generalPage/PagesButtons.qml

        qml/Pages/savesPage/SavesPage.qml
        qml/Pages/savesPage/LeftMenuButton.qml

        qml/Pages/optionsPage/OptionsPage.qml

        qml/Interphase/gameMenu/gameMenu.qml



)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appproject_1 PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appproject_1
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appproject_1
    PRIVATE Qt6::Quick Qt6::Gui Qt6::Qml Qt6::ShaderTools Vulkan::Vulkan
)

if(TARGET Qt6::GuiPrivate)
    target_link_libraries(appproject_1 PRIVATE Qt6::GuiPrivate)
endif()

qt_add_shaders(appproject_1 "vulkan_shaders"
    PREFIX "/shaders"
    BASE "src/graphics/vulkan/shaders"
    FILES
        src/graphics/vulkan/shaders/texture.vert
        src/graphics/vulkan/shaders/texture.frag
)

target_include_directories(appproject_1 PRIVATE src)

include(GNUInstallDirs)
install(TARGETS appproject_1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
