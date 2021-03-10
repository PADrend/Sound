#
# This file is part of the Util library.
# Copyright (C) 2018-2019 Sascha Brandt <sascha@brandt.graphics>
#
# This library is subject to the terms of the Mozilla Public License, v. 2.0.
# You should have received a copy of the MPL along with this library; see the 
# file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
#

add_library(SoundExtern INTERFACE)

if(${CMAKE_VERSION} VERSION_LESS "3.11.0") 
	message("Please consider to switch to CMake 3.11.0")
	include(FetchContentFallback)
else()
	include(FetchContent)
endif()

include(CMakeDependentOption)

# ------------------------------------------------------------------------------
# OpenAL

# try to find system OpenAL first
find_package(OpenAL QUIET)
if(OPENAL_FOUND)
	if(WIN32)
		get_filename_component(AL_DIR ${OPENAL_INCLUDE_DIR} NAME)
		if(${AL_DIR} STREQUAL "AL" AND EXISTS "${OPENAL_INCLUDE_DIR}/al.h")
			# remove trailing AL from include path
			set(OPENAL_INCLUDE_DIR "${OPENAL_INCLUDE_DIR}/..")
		endif()
	endif()
	target_compile_definitions(SoundExtern INTERFACE UTIL_HAVE_LIB_OPENAL)
	target_include_directories(SoundExtern INTERFACE ${OPENAL_INCLUDE_DIR})
	target_link_libraries(SoundExtern INTERFACE ${OPENAL_LIBRARY})
else()
	message(STATUS "No OpenAL implementation found.")

	FetchContent_Declare(
		openal
		GIT_REPOSITORY https://github.com/kcat/openal-soft.git
		GIT_TAG        1.21.1
	)
	
	FetchContent_GetProperties(openal)
	if(NOT openal_POPULATED)
		message(STATUS "Fetching OpenAL soft...")
		FetchContent_Populate(openal)
		set(ALSOFT_UTILS OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_NO_CONFIG_UTIL OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_EXAMPLES OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_TESTS OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_CONFIG OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_HRTF_DEFS OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_AMBDEC_PRESETS OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_INSTALL OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_BACKEND_WAVE OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_BUILD_ROUTER OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_NO_UID_DEFS OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_WERROR OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_INSTALL_UTILS OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_INSTALL_EXAMPLES OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_INSTALL_HRTF_DATA OFF CACHE INTERNAL "" FORCE)
		set(ALSOFT_INSTALL_CONFIG OFF CACHE INTERNAL "" FORCE)
		set(LIBTYPE "SHARED")
		add_subdirectory(${openal_SOURCE_DIR} ${openal_BINARY_DIR} EXCLUDE_FROM_ALL)
	endif()
	
	target_compile_definitions(SoundExtern INTERFACE UTIL_HAVE_LIB_OPENAL)
	target_link_libraries(SoundExtern INTERFACE OpenAL)
	target_include_directories(SoundExtern INTERFACE ${openal_SOURCE_DIR}/include)
  set_target_properties(OpenAL PROPERTIES 
    BUILD_SHARED_LIBS OFF
    POSITION_INDEPENDENT_CODE ON
  )
endif()

# ------------------------------------------------------------------------------
# dr_libs (single-file public domain libraries; used for audio loading)

FetchContent_Declare(
	dr_libs
	GIT_REPOSITORY https://github.com/mackron/dr_libs.git
	GIT_TAG        9ed6a56330751541636f412a027774f61723d052
)

FetchContent_GetProperties(dr_libs)
if(NOT dr_libs_POPULATED)
	message(STATUS "Fetching dr_libs...")
	FetchContent_Populate(dr_libs)
endif()
target_compile_definitions(SoundExtern INTERFACE UTIL_HAVE_DR_LIBS)
target_include_directories(SoundExtern INTERFACE ${dr_libs_SOURCE_DIR})
