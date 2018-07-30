include $(ESHELL)Graphics_Systems/OpenGL-OpenGLES/Makefile
override CXXFLAGS += -I$(ESHELL)Graphics_Systems/OpenGLES/ -I$(ESHELL)Bridges/SDL-OpenGLES
SOURCES += $(wildcard $(ESHELL)Graphics_Systems/OpenGLES/*.cpp) $(wildcard $(ESHELL)Graphics_Systems/General/*.cpp)

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lopengl32 -lglu32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL -lGLU -ldl
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL
endif
