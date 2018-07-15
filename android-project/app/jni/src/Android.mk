LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(EROOT)/ENIGMAsystem/SHELL $(EROOT)/shared/lodepng

ESHELL := $(EROOT)/ENIGMAsystem/SHELL/

SOURCES := $(wildcard $(ESHELL)*.cpp) $(wildcard $(ESHELL)Platforms/General/*.cpp) $(EROOT)/shared/lodepng/lodepng.cpp
SYSTEMS := $(ESHELL)Platforms/$(PLATFORM) $(ESHELL)Graphics_Systems/$(GRAPHICS) $(ESHELL)Audio_Systems/$(AUDIO) $(ESHELL)Collision_Systems/$(COLLISION) $(ESHELL)Widget_Systems/$(WIDGETS) $(ESHELL)Networking_Systems/$(NETWORKING) $(ESHELL)Universal_System

include $(addsuffix /Makefile,$(SYSTEMS) $(EXTENSIONS))
include $(ESHELL)Bridges/$(PLATFORM)-$(GRAPHICS)/Makefile

LOCAL_SRC_FILES := $(SOURCES:$(LOCAL_PATH)/%=%)

LOCAL_CPPFLAGS := $(CXXFLAGS) -I$(CODEGEN) -I$(ESHELL) -I../SDL/include

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -lGLESv2 -lz -llog

include $(BUILD_SHARED_LIBRARY)
