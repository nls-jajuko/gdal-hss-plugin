
#include "ogr_hss.h"
#include "cpl_conv.h"
#include "cpl_string.h"
#include "cpl_csv.h"

CPL_CVSID("$Id$")

/************************************************************************/
/*                          OGRHSSDataSource()                          */
/************************************************************************/

OGRHSSDataSource::OGRHSSDataSource() :
    pszName(nullptr),
    papoLayers(nullptr),
    nLayers(0),
    fpOutput(nullptr),
    coordinatePrecision(0)
{}

/************************************************************************/
/*                         ~OGRHSSDataSource()                          */
/************************************************************************/

OGRHSSDataSource::~OGRHSSDataSource()

{
    if ( fpOutput != nullptr )
    {
        VSIFCloseL( fpOutput);
    }

    for( int i = 0; i < nLayers; i++ )
        delete papoLayers[i];
    CPLFree( papoLayers );


    CPLFree( pszName );
}

/************************************************************************/
/*                           TestCapability()                           */
/************************************************************************/

int OGRHSSDataSource::TestCapability( const char * pszCap )

{
    if( EQUAL(pszCap,ODsCCreateLayer) )
        return TRUE;

    return FALSE;
}

/************************************************************************/
/*                              GetLayer()                              */
/************************************************************************/

OGRLayer *OGRHSSDataSource::GetLayer( int iLayer )

{
    if( iLayer < 0 || iLayer >= nLayers )
        return nullptr;

    return papoLayers[iLayer];
}

/************************************************************************/
/*                           ICreateLayer()                             */
/************************************************************************/

OGRLayer * OGRHSSDataSource::ICreateLayer( const char * pszLayerName,
                                           OGRSpatialReference * /*poSRS */,
                                           OGRwkbGeometryType eType,
                                           char ** /* papszOptions */ )
{
    HSSFeatureType HSSFeatureType;

    nLayers++;
    papoLayers = static_cast<OGRHSSLayer **>(
        CPLRealloc( papoLayers, nLayers * sizeof(OGRHSSLayer*) ) );
    papoLayers[nLayers-1] = new OGRHSSLayer(
        pszName, pszLayerName, eType, TRUE, this );

    return papoLayers[nLayers-1];
}

/************************************************************************/
/*                                Open()                                */
/************************************************************************/

int OGRHSSDataSource::Open( const char * pszFilename )

{
    int ok = FALSE;


    return ok;
}

/************************************************************************/
/*                               Create()                               */
/************************************************************************/

int OGRHSSDataSource::Create( const char *pszFilename,
                              char **papszOptions )
{
    if( fpOutput != nullptr)
    {
        CPLAssert( false );
        return FALSE;
    }

    if( strcmp(pszFilename,"/dev/stdout") == 0 )
        pszFilename = "/vsistdout/";

/* -------------------------------------------------------------------- */
/*     Do not override exiting file.                                    */
/* -------------------------------------------------------------------- */
    VSIStatBufL sStatBuf;

    if( VSIStatL( pszFilename, &sStatBuf ) == 0 )
        return FALSE;

/* -------------------------------------------------------------------- */
/*      Create the output file.                                         */
/* -------------------------------------------------------------------- */
    pszName = CPLStrdup( pszFilename );

    fpOutput = VSIFOpenL( pszFilename, "wb" );
    if( fpOutput == nullptr )
    {
        CPLError( CE_Failure, CPLE_OpenFailed,
                  "Failed to create HSS file %s.",
                  pszFilename );
        return FALSE;
    }

    /* Coordinate precision */
    const char* pszCoordinatePrecision
        = CSLFetchNameValue( papszOptions, "COORDINATE_PRECISION" );
    if (pszCoordinatePrecision)
    {
        coordinatePrecision = atoi(pszCoordinatePrecision);
        if (coordinatePrecision <= 0)
            coordinatePrecision = 0;
        else if (coordinatePrecision >= 20)
            coordinatePrecision = 20;
    }
    else
    {
        coordinatePrecision = 10;
    }

    return TRUE;
}
