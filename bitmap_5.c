#include "php.h"

#include "bitmap.h"


#if ZEND_MODULE_API_NO > 20060613
#   define BITMAP_METHOD_BASE(className, name) zim_##className##_##name
#else
#   define BITMAP_METHOD_BASE(className, name) zif_##className##_##name
#endif

#define Z_BITMAP_INTERN(intern, obj) {\
    intern = (php_bitmap_base_t *)zend_object_store_get_object(obj TSRMLS_CC);\
}

#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 0)
#   define FREE_OBJECT_EXTRA(base) {\
        zend_object_std_dtor(&base->object TSRMLS_CC);\
        efree(base);\
    }
#else
#   define FREE_OBJECT_EXTRA(base) {\
    if (base->object.properties) {\
        zend_hash_destory(base->object.properties);\
        free_hashTable(base->object.properties);\
    }\
    efree(base);\
    }
#endif

#define GET_BITMAP_THIS(obj) obj

#define MALLOC_BITMAP_BASE_T emalloc(sizeof(php_bitmap_base_t));

#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 0)
#   define INIT_BITMAP_OBJECT(base, ce) zend_object_std_init(&base->object, ce TSRMLS_CC);
#else
#   define INIT_BITMAP_OBJECT(base, ce) {\
        ALLOC_HASHTABLE(base->object.properties);\
        zend_hash_init(base->object.properties, 0, NULL, ZVAL_PTR_DTOR, 0);\
        base->object.ce = ce;\
    }
#endif

#if PHP_API_VERSION < 20100412
#    define INIT_BITMAP_PROPERTIES(base, ce){\
        zval *tmp;\
        zend_hash_copy(\
            base->object.properties, &ce->default_properties,\
            (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));\
    }
#else
#    define INIT_BITMAP_PROPERTIES(base, ce) {\
        object_properties_init(&base->object, ce);\
    }
#endif

#define BITMAP_DECLARE_RETVAL zend_object_value BITMAP_NEW_RETVAL;

#define BITMAP_NEW_RETVAL retval
#define BITMAP_NEW_RETVAL_TYPE zend_object_value
#define BITMAP_INIT_HANDLER(base) {\
    BITMAP_NEW_RETVAL.handle = zend_objects_store_put(\
        base, (zend_objects_store_dtor_t)zend_objects_destroy_object,\
        (zend_objects_free_object_storage_t)php_bitmap_base_free,\
        NULL TSRMLS_CC);\
    retval.handlers = zend_get_std_object_handlers();\
}

#define BITMAP_RETURN_STRING(s, l) RETVAL_STRINGL(s, l ,1);


//common
static zend_class_entry *bitmap_ce;

static ZEND_METHOD(bitmap, __construct);
static ZEND_METHOD(bitmap, setOption);
static ZEND_METHOD(bitmap, setBit);
static ZEND_METHOD(bitmap, getBit);
static ZEND_METHOD(bitmap, dumpBit);
static ZEND_METHOD(bitmap, getBytes);

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

ZEND_BEGIN_ARG_INFO_EX(arginfo_bitmap_base_getBytes, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry bitmap_base_methods[] = {
    ZEND_ME(bitmap, __construct, arginfo_bitmap_base___construct, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, setOption, arginfo_bitmap_base_setOption, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, setBit, arginfo_bitmap_base_setBit, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, getBit, arginfo_bitmap_base_getBit, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, dumpBit, arginfo_bitmap_base_dumpBit, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, getBytes, arginfo_bitmap_base_getBytes, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};



BITMAP_NEW_RETVAL_TYPE php_bitmap_base_new(zend_class_entry *ce) /* {{{ */ {
    BITMAP_DECLARE_RETVAL;
    php_bitmap_base_t *intern = MALLOC_BITMAP_BASE_T;
    intern->size = 8;
    intern->bytes = ecalloc(1, intern->size);
    INIT_BITMAP_OBJECT(intern, ce);
    INIT_BITMAP_PROPERTIES(intern, ce);
    BITMAP_INIT_HANDLER(intern);
    return BITMAP_NEW_RETVAL;
}

static void php_bitmap_base_free(php_bitmap_base_t *intern TSRMLS_CC) /* {{{ */ {
    efree(intern->bytes);
    FREE_OBJECT_EXTRA(intern);
}

#include "bitmap_method.c"

void php_bitmap_init_class() /* {{{ */ {
    zend_class_entry ce;
    TSRMLS_FETCH();
    /* base */
    INIT_CLASS_ENTRY(ce, "PhpBitmap", bitmap_base_methods);
    bitmap_ce = zend_register_internal_class(&ce);
    bitmap_ce->create_object = php_bitmap_base_new;
}
