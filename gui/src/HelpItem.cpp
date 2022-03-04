#include "pch.h"

#include "HelpItem.h"

// constructor
HelpItem::HelpItem(std::string imgPath, std::string title, std::string description) {
	this->imgPath = imgPath;
	this->title = title;
	this->description = description;
}

bool HelpItem::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// initializing image width and height
	int image_width = 0;
	int image_height = 0;
	// stb loads in the image of the data via the path of the file
	// it updates the width and height of the image, and is set to 4 desired channels for rgba
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	
	// failure to load image data returns false
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier for a 2D image
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	// This is all essentially standard for 2D images
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Upload pixels into texture
	// The execution of this is strange, but the functionality of this is standard
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}