#pragma once
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if(x){delete x; x = nullptr;}
#endif