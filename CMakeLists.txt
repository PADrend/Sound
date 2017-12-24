#
# This file is part of the Sound library.
# Copyright (C) 2010-2017 Benjamin Eikel <benjamin@eikel.org>
#
# This library is subject to the terms of the Mozilla Public License, v. 2.0.
# You should have received a copy of the MPL along with this library; see the 
# file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
#
cmake_minimum_required(VERSION 3.1.0)
project(Sound LANGUAGES CXX VERSION 0.2.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up install directories
include(GNUInstallDirs)
set(CMAKE_INSTALL_CMAKECONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/Sound)

add_library(Sound
	Buffer.cpp
	Listener.cpp
	Sound.cpp
	Source.cpp
)
# Install the header files
install(FILES
	Buffer.h
	Listener.h
	Sound.h
	SoundInternals.h
	Source.h
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Sound
	COMPONENT headers
)

# Dependency to OpenAL
find_package(OpenAL REQUIRED NO_MODULE)
target_link_libraries(Sound PRIVATE OpenAL::OpenAL)

# Dependency to SDL2
find_package(SDL2 NO_MODULE)
if(SDL2_FOUND)
	target_include_directories(Sound PRIVATE ${SDL2_INCLUDE_DIRS})
	target_link_libraries(Sound PRIVATE ${SDL2_LIBRARIES})
endif()

# Dependency to Util
if(NOT TARGET Util)
	find_package(Util 0.2.0 REQUIRED NO_MODULE)
endif()
target_link_libraries(Sound PUBLIC Util)

# Set version of library
set_target_properties(Sound PROPERTIES VERSION ${Sound_VERSION}
                                       SOVERSION ${Sound_VERSION_MAJOR})

# Install the library
install(TARGETS Sound EXPORT LibraryExport
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtimelibraries
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtimelibraries
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT developmentlibraries
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Set include directories for users of this library
target_include_directories(Sound
                           INTERFACE
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"
                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# Library export
#   to use from the build tree
export(EXPORT LibraryExport FILE SoundTargets.cmake)
#   to use from installation tree
install(EXPORT LibraryExport DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR} FILE SoundTargets.cmake COMPONENT developmentlibraries)

# Create the configuration files
include(CMakePackageConfigHelpers)
configure_package_config_file(SoundConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/SoundConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR}
                              PATH_VARS CMAKE_INSTALL_CMAKECONFIGDIR
                              NO_SET_AND_CHECK_MACRO
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/SoundConfigVersion.cmake
                                 VERSION ${Sound_VERSION}
                                 COMPATIBILITY SameMajorVersion)

# Install the configuration files
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/SoundConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/SoundConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR} COMPONENT developmentlibraries)

# Packaging
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "libsound${Sound_VERSION_MAJOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Sound library")
set(CPACK_PACKAGE_VENDOR "Benjamin Eikel; Claudius Jaehn; Ralf Petring")
set(CPACK_PACKAGE_CONTACT "Benjamin Eikel <benjamin@eikel.org>")
set(CPACK_PACKAGE_VERSION_MAJOR ${Sound_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Sound_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Sound_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "libSound")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_STRIP_FILES ON)

include(CPack)
