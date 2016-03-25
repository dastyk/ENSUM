#pragma once
#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if(x){delete x; x = nullptr;}
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) { if (x) { delete[] (x); (x) = nullptr; } }
#endif


#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) if(x){(x)->Release(); (x) = nullptr;}
#endif