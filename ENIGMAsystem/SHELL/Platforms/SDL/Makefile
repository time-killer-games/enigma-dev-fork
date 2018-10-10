ifndef PKG-CONFIG
	PKG-CONFIG := pkg-config
endif

SOURCES += $(wildcard $(ESHELL)Platforms/SDL/*.cpp) $(ESHELL)Platforms/General/POSIX/Utility.cpp
override CXXFLAGS += $(shell $(PKG-CONFIG) --cflags sdl2) -DENIGMA_PLATFORM_SDL

ifeq ($(TARGET-PLATFORM), Windows)
	SOURCES += $(wildcard $(ESHELL)Bridges/SDL-Win32/*.cpp) #$(ESHELL)Platforms/Win32/WINDOWSfilemanip.cpp
override LDLIBS += $(shell $(PKG-CONFIG) --libs --static sdl2)
else
	SOURCES += $(ESHELL)Platforms/General/POSIX/POSIXfilemanip.cpp
	override LDLIBS += -lpthread $(shell $(PKG-CONFIG) --libs sdl2)
endif
