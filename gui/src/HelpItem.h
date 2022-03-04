#pragma once

#include "pch.h"

#ifndef HELP_ITEM_H
#define HELP_ITEM_H

#include <string>
#include <stb_image.h>

struct HelpItem {
public:
	/// the relative path of the image based on where it's constructed in the project
	std::string imgPath;
	/// the title that should be displayed on this help item
	std::string title;
	/// the description that should be displayed on this help item
	std::string description;

	HelpItem(std::string imgPath, std::string title, std::string description);

	/**
	 * Helper function to assist loading in the texture of the image since ImGui
	 *     requires textures to load images in its windows
	 * Some adjustments would've been necessary to the entire renderer to implement
	 *     texture loading, so this workaround is used instead
	 * 
	 * Returns true if the texture was successfully loaded
	*/
	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
};

#endif