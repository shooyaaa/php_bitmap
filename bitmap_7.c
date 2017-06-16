#include "php.h"

#include "bitmap.h"

#define BITMAP_RETURN_STRING(s, l) RETVAL_STRINGL(s, l);

zend_object_handlers bitmap_handlers;
#include "bitmap_declare.c"

static inline php_bitmap_base_t *bitmap_base_fetch_object(zend_object *obj) {
    return (php_bitmap_base_t *)((char*)(obj) - XtOffsetOf(php_bitmap_base_t, object));
}

#define Z_BITMAP_INTERN(intern, obj) {\
    intern = (php_bitmap_base_t *)((char*)(obj) - XtOffsetOf(php_bitmap_base_t, object));\
}
#define Z_BITMAP_BASE_P(zv) php_bitmap_fetch_object(Z_OBJ_P((zv)))

#define GET_BITMAP_THIS(obj) Z_OBJ_P(obj)

zend_object *php_bitmap_base_new(zend_class_entry *ce) /* {{{ */ {
    php_bitmap_base_t *intern = ecalloc(1, sizeof(php_bitmap_base_t) + zend_object_properties_size(ce));
    intern->size = 8;
    intern->bytes = ecalloc(1, intern->size);
    zend_object_std_init(&intern->object, ce);
    object_properties_init(&intern->object, ce);
    intern->object.handlers = &bitmap_handlers;

    return &intern->object;
}

static void php_bitmap_base_free(zend_object *object) /* {{{ */ {
    php_bitmap_base_t *intern = bitmap_base_fetch_object(object);

    if (!intern) {
        return;
    }
    efree(intern->bytes);
    zend_object_std_dtor(&intern->object);
}

#include "bitmap_method.c"

void php_bitmap_init_class() /* {{{ */ {
    zend_class_entry ce;

    /* base */
    INIT_CLASS_ENTRY(ce, "PhpBitmap", bitmap_base_methods);
    bitmap_ce = zend_register_internal_class(&ce);
    bitmap_ce->create_object = php_bitmap_base_new;
    memcpy(&bitmap_handlers, zend_get_std_object_handlers(),sizeof bitmap_handlers);
    bitmap_handlers.offset = XtOffsetOf(php_bitmap_base_t, object);
    bitmap_handlers.free_obj = php_bitmap_base_free;
}
