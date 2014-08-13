#include <QtCore/qglobal.h>

#if defined(SDATABASE_LIBRARY)
#  define SDATABASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SDATABASESHARED_EXPORT Q_DECL_IMPORT
#endif
