########################################################################
# Copyright 2014 Benoît Vey                                            #
#                                                                      #
# This file is part of Green.                                          #
#                                                                      #
# Green is free software: you can redistribute it and/or modify        #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Green is distributed in the hope that it will be useful,             #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Green.  If not, see <http://www.gnu.org/licenses/>.       #
########################################################################

cmake_minimum_required(VERSION 2.8)

project(green CXX)

set(CMAKE_BUID_TYPE "Release")
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Boost COMPONENTS system REQUIRED)
find_package(SFML 2 COMPONENTS system window graphics REQUIRED)

file(
		GLOB_RECURSE
		source_files
		src/*
)

add_executable(
		green
		${source_files}
)

target_link_libraries(
		green
		${CMAKE_EXE_LINKER_FLAGS}
		${Boost_LIBRARIES}
		${SFML_LIBRARIES}
)

install(
		TARGETS green
		RUNTIME DESTINATION bin
)
