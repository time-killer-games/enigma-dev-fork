include $(ESHELL)Graphics_Systems/OpenGL-OpenGLES/Makefile
override CXXFLAGS += -I$(ESHELL)Graphics_Systems/OpenGL/ -I$(ESHELL)Bridges/Glew-OpenGL
SOURCES += $(wildcard $(ESHELL)Graphics_Systems/OpenGL/*.cpp) $(wildcard $(ESHELL)Graphics_Systems/General/*.cpp)

ifeq ($(TARGET-PLATFORM), Windows)
	override CXXFLAGS += -DGLEW_STATIC
	override LDLIBS += -lglew32 -lopengl32 -lglu32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL -lGLU -lGLEW
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL -lGLEW
endif
