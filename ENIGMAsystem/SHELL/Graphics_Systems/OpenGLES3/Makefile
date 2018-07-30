include $(ESHELL)Graphics_Systems/OpenGLES/Makefile
include $(ESHELL)Graphics_Systems/OpenGL3-OpenGLES3/Makefile
override CXXFLAGS += -I$(ESHELL)Graphics_Systems/OpenGLES3/glad/ -DENIGMA_GRAPHICS_OPENGLES3
SOURCES += $(wildcard $(ESHELL)Graphics_Systems/OpenGLES3/*.cpp) $(ESHELL)Graphics_Systems/OpenGLES3/glad/glad.c

SHADER_VERSION := $(ESHELL)Graphics_Systems/OpenGLES3/version.glsl
