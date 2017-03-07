CMAKE_MINIMUM_REQUIRED( VERSION 3.1 )

SET( APP_NAME testapp )
PROJECT( ${APP_NAME} )

IF( NOT MSVC )   
    SET( CMAKE_C_FLAGS_DEBUG "-g -DDEBUG=1" )
    SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" )
    SET( CMAKE_C_FLAGS_RELEASE "-DNDEBUG=1" )
    SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" )
    SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99" )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Weffc++ -Wall -Werror -Wextra -Wpedantic -ansi -pedantic" )
    IF( CLANG )
        SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++" )
    ENDIF()
ENDIF( NOT MSVC )

SET( GOGSDK_ROOT ${CMAKE_SOURCE_DIR}/../gogsdk )

INCLUDE_DIRECTORIES( SYSTEM ${GOGSDK_ROOT}/Include )

SET(HEADERS_FILES
	BackgroundTask.h
	Controls.h
	Decision.h
	Error.h
	Game.h
	Input.h
	Lobbies.h
	Lobby.h
	LobbyList.h
	LobbyLoad.h
	LobbyMain.h
	LoginSelector.h
	Members.h
	Selector.h
	SignIn.h
	Stack.h
	State.h
	StateInterface.h
)

SET(SOURCES_FILES
	BackgroundTask.cpp
	Controls.cpp
	Decision.cpp
	Error.cpp
	Game.cpp
	Input.cpp
	Lobbies.cpp
	Lobby.cpp
	LobbyList.cpp
	LobbyLoad.cpp
	LobbyMain.cpp
	LoginSelector.cpp
	main.cpp
	Members.cpp
	SignIn.cpp
	Stack.cpp
	State.cpp
)

LINK_DIRECTORIES(${GOGSDK_ROOT}/Libraries)

ADD_EXECUTABLE( ${APP_NAME} ${SOURCES_FILES} ${HEADERS_FILES} )

TARGET_LINK_LIBRARIES(${APP_NAME} Galaxy.lib )

ADD_CUSTOM_COMMAND(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${GOGSDK_ROOT}/Libraries/Galaxy.dll $<TARGET_FILE_DIR:${APP_NAME}>
)

ADD_CUSTOM_COMMAND(TARGET ${APP_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${GOGSDK_ROOT}/Libraries/GalaxyPeer.dll $<TARGET_FILE_DIR:${APP_NAME}>
)