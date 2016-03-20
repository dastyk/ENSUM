#pragma once
#include <stdint.h>

struct MetaData
{
	uint32_t used = 0;
	uint32_t allocated = 0;
	void *buffer = nullptr;
};