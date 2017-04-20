dnl $Id$
dnl config.m4 for extension php_bitmap

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(php_bitmap, for php_bitmap support,
dnl Make sure that the comment is aligned:
dnl [  --with-php_bitmap             Include php_bitmap support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(php_bitmap, whether to enable php_bitmap support,
Make sure that the comment is aligned:
[  --enable-php_bitmap           Enable php_bitmap support])

echo $PHP_PHP_BITMAP
if test "$PHP_PHP_BITMAP" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-php_bitmap -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/php_bitmap.h"  # you most likely want to change this
  dnl if test -r $PHP_PHP_BITMAP/$SEARCH_FOR; then # path given as parameter
  dnl   PHP_BITMAP_DIR=$PHP_PHP_BITMAP
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for php_bitmap files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PHP_BITMAP_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PHP_BITMAP_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the php_bitmap distribution])
  dnl fi

  dnl # --with-php_bitmap -> add include path
  dnl PHP_ADD_INCLUDE($PHP_BITMAP_DIR/include)

  dnl # --with-php_bitmap -> check for lib and symbol presence
  dnl LIBNAME=php_bitmap # you may want to change this
  dnl LIBSYMBOL=php_bitmap # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PHP_BITMAP_DIR/$PHP_LIBDIR, PHP_BITMAP_SHARED_LIBADD)
  AC_DEFINE(HAVE_PHP_BITMAPLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong php_bitmap lib version or lib not found])
  dnl ],[
  dnl   -L$PHP_BITMAP_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(PHP_BITMAP_SHARED_LIBADD)

  PHP_NEW_EXTENSION(php_bitmap, bitmap.c php_bitmap.c, $ext_shared)
fi
