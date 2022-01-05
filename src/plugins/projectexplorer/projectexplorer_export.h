#pragma once

#include <qglobal.h>

#if defined(PROJECTEXPLORER_LIBRARY)
#  define PROJECTEXPLORER_EXPORT Q_DECL_EXPORT
#elif defined(PROJECTEXPLORER_STATIC_LIBRARY)
#  define PROJECTEXPLORER_EXPORT
#else
#  define PROJECTEXPLORER_EXPORT Q_DECL_IMPORT
#endif
