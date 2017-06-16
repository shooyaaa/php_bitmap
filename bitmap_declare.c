#ifndef __bitmap_declare_c
#define __bitmap_declare_c
//common
static zend_class_entry *bitmap_ce;

static ZEND_METHOD(bitmap, __construct);
static ZEND_METHOD(bitmap, setBit);
static ZEND_METHOD(bitmap, getBit);
static ZEND_METHOD(bitmap, dumpBit);
static ZEND_METHOD(bitmap, getBytes);

ZEND_BEGIN_ARG_INFO_EX(arginfo_bitmap_base___construct, 0, 0, 2)
    ZEND_ARG_INFO(0, str)
    ZEND_ARG_INFO(0, len)
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
    ZEND_ME(bitmap, setBit, arginfo_bitmap_base_setBit, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, getBit, arginfo_bitmap_base_getBit, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, dumpBit, arginfo_bitmap_base_dumpBit, ZEND_ACC_PUBLIC)
    ZEND_ME(bitmap, getBytes, arginfo_bitmap_base_getBytes, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};
#endif
