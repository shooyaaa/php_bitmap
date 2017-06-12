#include "php.h"


#if ZEND_MODULE_API_NO > 20131226
#include "bitmap_7.c"
#else
#include "bitmap_5.c"
#endif
