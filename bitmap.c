#include "php.h"

#include "bitmap.h"

typedef struct {
    zend_object object;
    char *bits;
} php_bitmap_base_t;

static zend_class_entry *bitmap_ce;
zend_object_handlers bitmap_handlers;

static ZEND_METHOD(bitmap, __construct);
static ZEND_METHOD(bitmap, setOption);
static ZEND_METHOD(bitmap, setBit);
static ZEND_METHOD(bitmap, getBit);
static ZEND_METHOD(bitmap, dumpBit);

ZEND_BEGIN_ARG_INFO_EX(arginfo_bitmap_base___construct, 0, 0, 0)
    ZEND_ARG_INFO(0, opt)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_bitmap_base_setOption, 0, 0, 2)
    ZEND_ARG_INFO(0, option)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_bitmap_base_getBit, 0, 0, 1)
    ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_bitmap_base_dumpBit, 0, 0, 1)
    ZEND_ARG_INFO(0, offset)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_bitmap_base_setBit, 0, 0, 2)
    ZEND_ARG_INFO(0, offset)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

static zend_function_entry bitmap_base_methods[] = {
    ZEND_ME(bitmap, __construct, arginfo_bitmap_base___construct, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, setOption, arginfo_bitmap_base_setOption, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, setBit, arginfo_bitmap_base_setBit, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, getBit, arginfo_bitmap_base_getBit, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, dumpBit, arginfo_bitmap_base_dumpBit, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

static inline php_bitmap_base_t *bitmap_base_fetch_object(zend_object *obj) {
    return (php_bitmap_base_t *)((char*)(obj) - XtOffsetOf(php_bitmap_base_t, object));
}

#define Z_BITMAP_BASE_P(zv) php_bitmap_fetch_object(Z_OBJ_P((zv)))

zend_object *php_bitmap_base_new(zend_class_entry *ce) /* {{{ */ {
    php_bitmap_base_t *intern = ecalloc(1, sizeof(php_bitmap_base_t) + zend_object_properties_size(ce));
    intern->bits = ecalloc(1, 8);
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
    efree(intern->bits);
    zend_object_std_dtor(&intern->object);
}


static ZEND_METHOD(bitmap, getBit) {
    php_bitmap_base_t *intern = bitmap_base_fetch_object(Z_BITMAP_BASE_P(getThis()));
}

static ZEND_METHOD(bitmap, setBit) {
    php_bitmap_base_t *intern = bitmap_base_fetch_object(Z_BITMAP_BASE_P(getThis()));
}

static ZEND_METHOD(bitmap, dumpBit) {
    zend_string *result = {0};
    ZVAL_STR(return_value, 'abc');
}

static ZEND_METHOD(bitmap, __construct) /* {{{ */ {

}
/* }}} */

static ZEND_METHOD(bitmap, setOption) /* {{{ */ {

    RETURN_TRUE;
}
/* }}} */

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
