cmake_minimum_required( VERSION 3.5 )
set( PROJECT_NAME flappy_ncurses )
project( ${PROJECT_NAME} )

set(CMAKE_BUILD_TYPE RELWITHDEBUGINFO)

file( GLOB SOURCE_FILES
	"src/*.h"
	"src/*.c"
)

find_package( Curses REQUIRED )
include_directories( ${CURSES_INCLUDE_DIRS} )
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )
target_link_libraries( ${PROJECT_NAME} ${CURSES_LIBRARIES} )
