#ifndef BITMAP_CLASS_H
#define BITMAP_CLASS_H
typedef struct {
    zend_object object;
    char *bytes;
    size_t size;

} php_bitmap_base_t;


void php_bitmap_init_class();

#if ZEND_MODULE_API_NO > 20131226
static void php_bitmap_base_free(zend_object *object);
#else
static void php_bitmap_base_free(php_bitmap_base_t *intern TSRMLS_CC);
#endif


#endif
