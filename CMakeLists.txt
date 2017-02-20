cmake_minimum_required( VERSION 3.0 )

project( hex_hav )
set( CMAKE_CXX_STANDARD 14 )
include_directories( src )


# dependencies

find_package( PkgConfig REQUIRED )
pkg_check_modules( GTKMM REQUIRED gtkmm-2.4 )
include_directories( ${GTKMM_INCLUDE_DIRS} )
find_package(Threads REQUIRED)


# local libraries

set( SOURCE_HEX_HAV
    src/hex_hav/Board.cpp
    src/hex_hav/BoardHavannah.cpp
    src/hex_hav/BoardHex.cpp
    src/hex_hav/Chrono.cpp
    src/hex_hav/Game.cpp
    src/hex_hav/Player.cpp
    src/hex_hav/PlayerMcts.cpp
    src/hex_hav/PlayerRave.cpp
    src/hex_hav/PlayerRaveLgrf1.cpp
    src/hex_hav/PlayerRaveMast.cpp
    src/hex_hav/PlayerRaveNast2.cpp
    src/hex_hav/PlayerRavePpr.cpp
    src/hex_hav/PlayerRavePr.cpp
    src/hex_hav/Prng.cpp
    )

set ( SOURCE_GUI1
    src/hex_hav_gui1/Controller.cpp
    src/hex_hav_gui1/View.cpp
    src/hex_hav_gui1/ViewDrawing.cpp
    src/hex_hav_gui1/ViewMenu.cpp
    )

set ( SOURCE_GUI2
    src/hex_hav_gui2/Controller.cpp
    src/hex_hav_gui2/View.cpp
    src/hex_hav_gui2/ViewDrawing.cpp
    src/hex_hav_gui2/ViewMenu.cpp
    )

add_library ( hex STATIC ${SOURCE_HEX_HAV} )
target_compile_definitions( hex PUBLIC -DBOARD_TYPE=BoardHex )

add_library ( hav STATIC ${SOURCE_HEX_HAV} )
target_compile_definitions( hav PUBLIC -DBOARD_TYPE=BoardHavannah )

add_library ( hex_gui1 STATIC ${SOURCE_GUI1} )
target_link_libraries( hex_gui1 hex )

add_library ( hav_gui1 STATIC ${SOURCE_GUI1} )
target_link_libraries( hav_gui1 hav )

add_library ( hex_gui2 STATIC ${SOURCE_GUI2} )
target_link_libraries( hex_gui2 hex )

add_library ( hav_gui2 STATIC ${SOURCE_GUI2} )
target_link_libraries( hav_gui2 hav )


# executables

add_executable( hex_cli.out src/mains/hex_hav_cli.cpp )
target_link_libraries( hex_cli.out hex )

add_executable( hav_cli.out src/mains/hex_hav_cli.cpp )
target_link_libraries( hav_cli.out hav )

add_executable( hex_gui1.out src/mains/hex_hav_gui1.cpp )
target_link_libraries( hex_gui1.out 
    hex_gui1 
    ${GTKMM_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT} 
    )

add_executable( hav_gui1.out src/mains/hex_hav_gui1.cpp )
target_link_libraries( hav_gui1.out 
    hav_gui1 
    ${GTKMM_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT} 
    )

add_executable( hex_gui2.out src/mains/hex_hav_gui2.cpp )
target_link_libraries( hex_gui2.out 
    hex_gui2 
    ${GTKMM_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT} 
    )

add_executable( hav_gui2.out src/mains/hex_hav_gui2.cpp )
target_link_libraries( hav_gui2.out 
    hav_gui2 
    ${GTKMM_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT} 
    )


# unit tests

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()

    set( TEST_SOURCE
	${CMAKE_CURRENT_SOURCE_DIR}/src/unittests/BoardHex_test.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/unittests/Board_test.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/unittests/PlayerMcts_test.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/unittests/PlayerRavePr_test.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/unittests/PlayerRave_test.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/unittests/Player_test.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/unittests/Prng_test.hpp
	)

    CXXTEST_ADD_TEST(hex_unittests.out hex_unittests.cpp ${TEST_SOURCE} )
    target_link_libraries(hex_unittests.out hex )

    CXXTEST_ADD_TEST(hav_unittests.out hav_unittests.cpp ${TEST_SOURCE} )
    target_link_libraries(hav_unittests.out hav )
endif()


# install targets

install ( TARGETS 
    hex_cli.out 
    hav_cli.out 
    hex_gui1.out 
    hav_gui1.out 
    hex_gui2.out 
    hav_gui2.out 
    DESTINATION bin)

