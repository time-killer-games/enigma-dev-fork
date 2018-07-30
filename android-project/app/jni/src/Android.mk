LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL


# Default Shits
EROOT := C:/Users/Owner/Desktop/enigma-dev/
GMODE ?= Debug
GRAPHICS ?= OpenGLES2
AUDIO ?= None
COLLISION ?= None
WIDGETS ?= None
NETWORKING ?= None
PLATFORM ?= SDL
CODEGEN ?= C:/Users/Owner/Desktop/enigma-dev/

ESHELL := $(EROOT)ENIGMAsystem/SHELL/

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(ESHELL) $(EROOT)shared/lodepng

SOURCES := $(wildcard $(ESHELL)*.cpp) $(wildcard $(ESHELL)Platforms/General/*.cpp) $(EROOT)/shared/lodepng/lodepng.cpp $(CODEGEN)/$(GRAPHICS)_shader.cpp
SYSTEMS := $(ESHELL)Platforms/$(PLATFORM) $(ESHELL)Graphics_Systems/$(GRAPHICS) $(ESHELL)Audio_Systems/$(AUDIO) $(ESHELL)Collision_Systems/$(COLLISION) $(ESHELL)Widget_Systems/$(WIDGETS) $(ESHELL)Networking_Systems/$(NETWORKING) $(ESHELL)Universal_System

include $(addsuffix /Makefile,$(SYSTEMS) $(EXTENSIONS))
include $(ESHELL)Bridges/$(PLATFORM)-$(GRAPHICS)/Makefile

LOCAL_SRC_FILES := $(SOURCES:$(LOCAL_PATH)/%=%)

LOCAL_CPPFLAGS := $(CXXFLAGS) -I$(CODEGEN) -I$(ESHELL) -I../SDL/include

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -lGLESv2 -lz -llog

include $(BUILD_SHARED_LIBRARY)
