#include "php.h"

#include "bitmap.h"


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


typedef struct {
    zend_object object;
    char *bytes;
    size_t size;
} php_bitmap_base_t;

static inline php_bitmap_base_t *bitmap_base_fetch_object(zend_object *obj) {
    return (php_bitmap_base_t *)((char*)(obj) - XtOffsetOf(php_bitmap_base_t, object));
}

#define Z_BITMAP_INTERN(intern, obj) {\
    intern = (php_bitmap_base_t *)((char*)(obj) - XtOffsetOf(php_bitmap_base_t, object));\
}
#define Z_BITMAP_BASE_P(zv) php_bitmap_fetch_object(Z_OBJ_P((zv)))

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

static int bitmap_get_byte_addr(php_bitmap_base_t *intern, size_t index, char **pos, char *rm) {
    size_t byteIndex;
    short offset, i;
    unsigned char mask = 0x80;
    if (index <= 0) {
        return 0;
    }
    byteIndex = (size_t)floor((index - 1) / 8);
    offset = (index - 1) % 8;
    for (i = 0; i < offset; i++) {
        mask = mask >> 1;
    }
    *rm = mask;
    *pos = intern->bytes + byteIndex;
    return 1;
}


static int bitmap_resize_byte_map(php_bitmap_base_t *intern, size_t size) {
    size_t oldSize = intern->size;
    if (oldSize > size) {
        return 1;
    }
    char *newBytes = ecalloc(1, size);
    memcpy(newBytes, intern->bytes, intern->size);
    efree(intern->bytes);
    intern->bytes = newBytes;
    intern->size = size;
    return 0;
}

static ZEND_METHOD(bitmap, getBit) {
    php_bitmap_base_t *intern;
    int offset, index, byteIndex, i;
    char* pos, rm;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &index) == FAILURE) {
        return;
    }

    Z_BITMAP_INTERN(intern, Z_OBJ_P(getThis()));
    if (index > intern->size) {
        RETURN_FALSE;
    }

    bitmap_get_byte_addr(intern, index, &pos, &rm);
    char result = *pos & rm ? '1' : '0';
    RETURN_STRINGL(&result, 1);
}


static ZEND_METHOD(bitmap, setBit) {
    php_bitmap_base_t *intern;
    int offset, index, byteIndex, i;
    char *pos, rm;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &index) == FAILURE) {
        return;
    }
    Z_BITMAP_INTERN(intern, Z_OBJ_P(getThis()));
    size_t byteOffset = (size_t)floor((index - 1) / 8);
    if (byteOffset >= intern->size) {
        bitmap_resize_byte_map(intern, byteOffset * 2);
    }

    bitmap_get_byte_addr(intern, index, &pos, &rm);
    *pos = *pos | rm;
}

static ZEND_METHOD(bitmap, dumpBit) {
    php_bitmap_base_t *intern;
    Z_BITMAP_INTERN(intern, Z_OBJ_P(getThis()));
    short j, count;
    size_t total, i;
    total = intern->size * sizeof(char) * 8;
    char *result = ecalloc(1, total);
    for (i = 0; i < intern->size; i ++) {
        count = 0;
        for (j = 0x80; j > 0; j /= 2) {
            result[i * 8 + count] = intern->bytes[i] & j ? '1' : '0';
            count ++;
        }
    }
    RETVAL_STRINGL(result, total);
    efree(result);
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
