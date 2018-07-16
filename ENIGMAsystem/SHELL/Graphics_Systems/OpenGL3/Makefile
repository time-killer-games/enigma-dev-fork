override CXXFLAGS += -I$(ESHELL)Graphics_Systems/OpenGL3/glad/
SOURCES += $(wildcard $(ESHELL)Graphics_Systems/OpenGL3/*.cpp) $(wildcard $(ESHELL)Graphics_Systems/General/*.cpp) $(ESHELL)Graphics_Systems/OpenGL3/glad/glad.c

ifeq ($(TARGET-PLATFORM), Windows)
	override LDLIBS += -lopengl32 -lglu32
else ifeq ($(TARGET-PLATFORM), Linux)
	override LDLIBS += -lGL -lGLU -ldl
else ifeq ($(TARGET-PLATFORM), MacOSX)
	override LDLIBS += -framework OpenGL
endif
