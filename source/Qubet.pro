###############################################################################
#                                                                             #
#                                 Qubet Project                               #
#                                      #34                                    #
#                                                                             #
#    Qubet  -  Copyright (C) 2011                                             #
#                                                                             #
#    Enrico Bacis                                                             #
#    Daniele Ciriello                                                         #
#                                                                             #
#    Musics by Binarpilot <binaerpilot.no>                                    #
#    Sound FX by SoundBible and Super Mario                                   #
#                                                                             #
#    Qubet is free software: you can redistribute it and/or modify            #
#    it under the terms of the GNU General Public License as published by     #
#    the Free Software Foundation, either version 3 of the License, or        #
#    (at your option) any later version.                                      #
#                                                                             #
#    This program is distributed in the hope that it will be useful,          #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of           #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
#    GNU General Public License for more details.                             #
#                                                                             #
#    You should have received a copy of the GNU General Public License        #
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.    #
#                                                                             #
###############################################################################


QT += core gui
QT += opengl
QT += xml
QT += phonon

TARGET = Qubet
TEMPLATE = app

DEFINES += USE_DISPLAY_LISTS

CONFIG(debug, debug|release) { DEFINES += _DEBUG }
CONFIG-=app_bundle

SOURCES += \
    main.cpp\
    game.cpp \
    cube.cpp \
    positioncontroller.cpp \
    obstacle.cpp \
    audiomanager.cpp \
    menu.cpp \
    leveleditor.cpp \
    qubet.cpp \
    window.cpp \
    utilities.cpp \
    skin.cpp \
    alphabet.cpp \
    letter.cpp \
    cubestring.cpp \
    actionlist.cpp \
    cubestringlist.cpp \
    skybox.cpp \
    level.cpp \
    ailevelfiller.cpp \
    pathfindinggraph.cpp \
    aicubemover.cpp

HEADERS  += \
    vector3f.h \
    game.h \
    cube.h \
    positioncontroller.h \
    obstacle.h \
    level.h \
    audiomanager.h \
    menu.h \
    leveleditor.h \
    defines.h \
    qubet.h \
    window.h \
    utilities.h \
    game_defines.h \
    leveleditor_defines.h \
    menu_defines.h \
    skin.h \
    alphabet.h \
    letter.h \
    cubestring.h \
    actionlist.h \
    cubestringlist.h \
    effects_defines.h \
    skybox.h \
    cube_defines.h \
    ailevelfiller.h \
    pathfindinggraph.h \
    aicubemover.h

OTHER_FILES += \
   resources/skins/skins.xml \
   resources/sound/music/menu.wav \
   resources/levels/levels.xml \
   resources/icons/icons.xml












