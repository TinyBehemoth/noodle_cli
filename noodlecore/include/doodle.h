#pragma once
#include "includes.h"

namespace Noodle
{

struct Doodle
{
	size_t nameHash;
	bool abstract;
	bool layer;
	bool directory;
	string path;	// relative to working directory and includes filename
	string name;	// same as filename

	static const unsigned int previewTextSize = 128;
	void PreviewText (string workingdirectory) const;
};

// Must contain all doodles.
using Doodles = unordered_map<Name,Doodle>;

}   // namespace Noodle
