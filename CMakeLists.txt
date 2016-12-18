cmake_minimum_required( VERSION 3.5 )
project( ncurses_tutorial )

file( GLOB SOURCE_FILES
	"src/*.h"
	"src/*.c"
)

find_package( Curses REQUIRED )
include_directories( ${CURSES_INCLUDE_DIRS} )
add_executable( ncurses_tut ${SOURCE_FILES} )
target_link_libraries( ncurses_tut ${CURSES_LIBRARIES} )
