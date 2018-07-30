include $(ESHELL)Graphics_Systems/OpenGLES/Makefile
include $(ESHELL)Graphics_Systems/OpenGL2-OpenGLES2/Makefile
override CXXFLAGS += -I$(ESHELL)Graphics_Systems/OpenGLES2/glad/ -DENIGMA_GRAPHICS_OPENGLES2
SOURCES += $(wildcard $(ESHELL)Graphics_Systems/OpenGLES2/*.cpp) $(ESHELL)Graphics_Systems/OpenGLES2/glad/glad.c

SHADER_VERSION := $(ESHELL)Graphics_Systems/OpenGLES2/version.glsl
