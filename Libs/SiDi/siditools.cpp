#include "siditools.h"
#include "sabstractprocessengine.h"
#include "sabstractdiscreader.h"
#include "sabstractdiscburner.h"
#include "sabstractdataburner.h"
#include "sabstractimagecreator.h"
#include "sabstractdisceraser.h"
#include "sabstractimagemounter.h"
#include "sabstractaudioburner.h"
#include "sconverter.h"

SAbstractProcessEngine *disc_burner   = 0;
SAbstractProcessEngine *data_burner   = 0;
SAbstractProcessEngine *disc_reader   = 0;
SAbstractProcessEngine *image_creator = 0;
SAbstractProcessEngine *disc_eraser   = 0;
SAbstractProcessEngine *image_mounter = 0;
SAbstractProcessEngine *audio_burner  = 0;

SAbstractDiscBurner *SiDiTools::createDiscBurner( QObject *parent )
{
    if( disc_burner == 0 )
        return new SAbstractDiscBurner( parent );

    return static_cast<SAbstractDiscBurner *>( disc_burner->create( parent ) );
}

SAbstractDataBurner *SiDiTools::createDataBurner( QObject *parent )
{
    if( data_burner == 0 )
        return new SAbstractDataBurner( parent );

    return static_cast<SAbstractDataBurner *>( data_burner->create( parent ) );
}

SAbstractDiscReader *SiDiTools::createDiscReader( QObject *parent )
{
    if( disc_reader == 0 )
        return new SAbstractDiscReader( parent );

    return static_cast<SAbstractDiscReader *>( disc_reader->create( parent ) );
}

SAbstractImageCreator *SiDiTools::createImageCreator( QObject *parent )
{
    if( image_creator == 0 )
        return new SAbstractImageCreator( parent );

    return static_cast<SAbstractImageCreator *>( image_creator->create( parent ) );
}

SAbstractDiscEraser *SiDiTools::createDiscEraser( QObject *parent )
{
    if( disc_eraser == 0 )
        return new SAbstractDiscEraser( parent );

    return static_cast<SAbstractDiscEraser *>( disc_eraser->create( parent ) );
}

SAbstractImageMounter *SiDiTools::createImageMounter( QObject *parent )
{
    if( image_mounter == 0 )
        return new SAbstractImageMounter( parent );

    return static_cast<SAbstractImageMounter *>( image_mounter->create( parent ) );
}

SAbstractAudioBurner  *SiDiTools::createAudioBurner(  QObject *parent )
{
    if( audio_burner == 0 )
        return new SAbstractAudioBurner( parent );

    return static_cast<SAbstractAudioBurner *>( audio_burner->create( parent ) );
}

void SiDiTools::setDiscBurnerEngine( SAbstractProcessEngine *engine )
{
    if( disc_burner != 0 )
        delete disc_burner;

    disc_burner = engine;
}

void SiDiTools::setDataBurnerEngine( SAbstractProcessEngine *engine )
{
    if( data_burner != 0 )
        delete data_burner;

    data_burner = engine;
}

void SiDiTools::setDiscReaderEngine( SAbstractProcessEngine *engine )
{
    if( disc_reader != 0 )
        delete disc_reader;

    disc_reader = engine;
}

void SiDiTools::setImageCreatorEngine( SAbstractProcessEngine *engine )
{
    if( image_creator != 0 )
        delete image_creator;

    image_creator = engine;
}

void SiDiTools::setDiscEraserEngine( SAbstractProcessEngine *engine )
{
    if( disc_eraser != 0 )
        delete disc_eraser;

    disc_eraser = engine;
}

void SiDiTools::setImageMounterEngine( SAbstractProcessEngine *engine )
{
    if( image_mounter != 0 )
        delete image_mounter;

    image_mounter = engine;
}

void SiDiTools::setAudioBurnerEngine(  SAbstractProcessEngine *engine )
{
    if( audio_burner != 0 )
        delete audio_burner;

    audio_burner = engine;
}

void SiDiTools::addConverterEngine( SAbstractConverterEngine *engine , const QHash<QString,QString> & types )
{
    SConverter::addConverterEngine( engine , types );
}

void SiDiTools::removeConverterEngine( SAbstractConverterEngine *engine , const QHash<QString,QString> & types )
{
    SConverter::removeConverterEngine( engine , types );
}
