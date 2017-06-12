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

    Z_BITMAP_INTERN(intern, GET_BITMAP_THIS(getThis()));
    if (index > intern->size) {
        RETURN_FALSE;
    }

    bitmap_get_byte_addr(intern, index, &pos, &rm);
    char result = *pos & rm ? '1' : '0';
    BITMAP_RETURN_STRING(&result, 1);
}


static ZEND_METHOD(bitmap, setBit) {
    php_bitmap_base_t *intern;
    int offset, index, byteIndex, i;
    char *pos, rm;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &index) == FAILURE) {
        return;
    }
    Z_BITMAP_INTERN(intern, GET_BITMAP_THIS(getThis()));
    size_t byteOffset = (size_t)floor((index - 1) / 8);
    if (byteOffset >= intern->size) {
        bitmap_resize_byte_map(intern, byteOffset * 2);
    }

    bitmap_get_byte_addr(intern, index, &pos, &rm);
    *pos = *pos | rm;
}

static ZEND_METHOD(bitmap, dumpBit) {
    php_bitmap_base_t *intern;
    Z_BITMAP_INTERN(intern, GET_BITMAP_THIS(getThis()));
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
    BITMAP_RETURN_STRING(result, total);
    efree(result);
}

static ZEND_METHOD(bitmap, getBytes) {
    php_bitmap_base_t *intern;
    Z_BITMAP_INTERN(intern, GET_BITMAP_THIS(getThis()));
    BITMAP_RETURN_STRING(intern->bytes, intern->size);
}

static ZEND_METHOD(bitmap, __construct) /* {{{ */ {

}
/* }}} */

static ZEND_METHOD(bitmap, setOption) /* {{{ */ {

    RETURN_TRUE;
}
/* }}} */
