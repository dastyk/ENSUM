#ifndef _MODEL_LOADER_H_
#define _MODEL_LOADER_H_
#pragma once
#include "Mesh.h"
#include "dll_export.h"

namespace Ensum
{
	namespace FileHandler
	{
		ENSUM_FILEHANDLER_EXPORT const void LoadModel(char* path);
	}
}
#endif