#ifndef CFPARSEGLOBAL_H
#define CFPARSEGLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#  if defined(QT_BUILD_CFMVC_LIB)
#    define Q_CFPARSE_EXPORT Q_DECL_EXPORT
#  else
#    define Q_CFPARSE_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_CFPARSE_EXPORT
#endif

QT_END_NAMESPACE
#endif // CFPARSEGLOBAL_H
