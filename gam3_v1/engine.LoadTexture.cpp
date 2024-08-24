#include "engine.LoadTexture.hpp"

#include <SDL_image.h>

std::string engine_LoadTexture::getName() {
	return std::string("LoadTexture");
}

/*
This function loads up a texture into the GPU, ready to be used.
Thanks to: http://www.sdltutorials.com/sdl-tip-sdl-surface-to-opengl-texture
*/
int engine_LoadTexture::handle(lua_State* L) {
	if (lua_gettop(L) != 2) {
		this->m_engine->Error("LoadTexture requires 1 argument.");
		return 1;
	}

	GLuint TextureID = 0;

	SDL_Surface* Surface = IMG_Load(lua_tostring(L, 2));

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	int Mode = GL_RGB;

	if (Surface->format->BytesPerPixel == 4) {
		Mode = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	lua_pushinteger(L, TextureID);

	return 1;
}