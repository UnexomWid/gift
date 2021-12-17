#ifndef GIFT_H_GUARD
#define GIFT_H_GUARD

#ifdef __EMSCRIPTEN__
#    include <emscripten.h>
#else
#    define EMSCRIPTEN_KEEPALIVE
#endif

EMSCRIPTEN_KEEPALIVE char* gift(const char* src);
EMSCRIPTEN_KEEPALIVE void  gift_free_result(char* result);

#endif