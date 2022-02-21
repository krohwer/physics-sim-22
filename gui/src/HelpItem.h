#pragma once

#include "pch.h"

#ifndef HELP_ITEM_H
#define HELP_ITEM_H

#include <string>
#include <stb_image.h>

struct HelpItem {
public:
	std::string imgPath;
	std::string title;
	std::string description;

	HelpItem(std::string imgPath, std::string title, std::string description);

	bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
};

#endif