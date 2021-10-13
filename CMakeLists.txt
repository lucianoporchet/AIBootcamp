cmake_minimum_required( VERSION 3.0 )
project( MyFirstAIBot C CXX )

#add_definitions(-DDEBUG_FRAMEWORK)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Filters
include( Filters.cmake )


SET( AIBOT_BUILDFILES 
	
	CleanAll.bat
	CMakeLists.txt
	Filters.cmake
	GenSubmit.bat
	GenSubmit.py
	PlayLocalMatch.bat
	PlayLocalMatch.py
	rebuild_vis_sln_x64.bat
)

SET( AIBOT_HEADERS 

	#Framework
	AIBot.h
	BotLogicIF.h
	ConfigData.h
	Globals.h
	InitData.h
	Logger.h
	MyBotLogic.h	
	TurnData.h
	Grid.h
	A_star.h
	A_star_to_goal.h
	HasPath.h
	Move_to_goal.h
	Move_to_next.h
	NewGoalVisible.h
	Selector.h
	Sequence.h
	Task.h
	UpdateGrid.h
	
	#DLL Stuff
	Interop_API.h	
	stdafx.h
	targetver.h
)

SET( AIBOT_SRC 

	#Framework
	AIBot.cpp
	Logger.cpp
	MyBotLogic.cpp
	Grid.cpp
	A_star_to_goal.cpp
	HasPath.cpp
	Move_to_goal.cpp
	Move_to_next.cpp
	NewGoalVisible.cpp
	Selector.cpp
	Sequence.cpp
	UpdateGrid.cpp

	#DLL Stuff
	dllmain.cpp
	Interop_API.cpp
	stdafx.cpp
)

SET(jobfiles "${AIBOT_HEADERS};${AIBOT_SRC};${AIBOT_BUILDFILES}")
file(WRITE jobfiles.txt "${jobfiles}")

add_library(myfirstaibot SHARED ${AIBOT_SRC} ${AIBOT_HEADERS})

CREATE_VISUAL_STUDIO_FILTERS(AIBOT)

