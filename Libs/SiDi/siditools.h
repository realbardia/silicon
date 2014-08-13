#ifndef SIDITOOLS_H
#define SIDITOOLS_H

#include <QObject>
#include <QHash>
#include "SiDi_global.h"

class SAbstractDiscBurner;
class SAbstractDataBurner;
class SAbstractDiscReader;
class SAbstractImageCreator;
class SAbstractDiscEraser;
class SAbstractImageMounter;
class SAbstractProcessEngine;
class SAbstractConverterEngine;
class SAbstractAudioBurner;

class SIDI_EXPORT SiDiTools
{
public:
    static SAbstractDiscBurner   *createDiscBurner(   QObject *parent = 0 );
    static SAbstractDataBurner   *createDataBurner(   QObject *parent = 0 );
    static SAbstractDiscReader   *createDiscReader(   QObject *parent = 0 );
    static SAbstractImageCreator *createImageCreator( QObject *parent = 0 );
    static SAbstractDiscEraser   *createDiscEraser(   QObject *parent = 0 );
    static SAbstractImageMounter *createImageMounter( QObject *parent = 0 );
    static SAbstractAudioBurner  *createAudioBurner(  QObject *parent = 0 );

public slots:
    static void setDiscBurnerEngine(   SAbstractProcessEngine *engine );
    static void setDataBurnerEngine(   SAbstractProcessEngine *engine );
    static void setDiscReaderEngine(   SAbstractProcessEngine *engine );
    static void setImageCreatorEngine( SAbstractProcessEngine *engine );
    static void setDiscEraserEngine(   SAbstractProcessEngine *engine );
    static void setImageMounterEngine( SAbstractProcessEngine *engine );
    static void setAudioBurnerEngine(  SAbstractProcessEngine *engine );

    static void addConverterEngine( SAbstractConverterEngine *engine , const QHash<QString,QString> & types );
    static void removeConverterEngine( SAbstractConverterEngine *engine , const QHash<QString,QString> & types );
};

#endif // SIDITOOLS_H
