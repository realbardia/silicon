#ifndef SIDI_GLOBAL_H
#define SIDI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SIDI_LIBRARY)
#  define SIDI_EXPORT Q_DECL_EXPORT
#else
#  define SIDI_EXPORT Q_DECL_IMPORT
#endif

#endif // SIDI_GLOBAL_H
