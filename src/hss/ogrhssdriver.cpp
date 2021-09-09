
#include "gdal_hss_plugin.h"

#include "ogr_hss.h"
#include "cpl_conv.h"

CPL_CVSID("$Id$")

/************************************************************************/
/*                                Open()                                */
/************************************************************************/

static GDALDataset *OGRHSSDriverOpen( GDALOpenInfo* poOpenInfo )

{
// --------------------------------------------------------------------
//      Does this appear to be a .HSS file?
// --------------------------------------------------------------------
    const char* pszFilename = poOpenInfo->pszFilename;
    if( STARTS_WITH_CI(pszFilename, "HSS:") )
    {
        pszFilename += 4;
    }
    else if( poOpenInfo->fpL == nullptr ||
        !(EQUAL( CPLGetExtension(pszFilename), "hss" )
           || ((STARTS_WITH_CI(pszFilename, "/vsigzip/") ||
                STARTS_WITH_CI(pszFilename, "/vsizip/")) &&
               (strstr( pszFilename, ".hss") ||
                strstr( pszFilename, ".hss")))) )
    {
        return nullptr;
    }
    if( poOpenInfo->eAccess == GA_Update )
    {
        return nullptr;
    }

    OGRHSSDataSource   *poDS = new OGRHSSDataSource();

    if( !poDS->Open( pszFilename ) )
    {
        delete poDS;
        poDS = nullptr;
    }

    return poDS;
}

/************************************************************************/
/*                               Create()                               */
/************************************************************************/

static GDALDataset *OGRHSSDriverCreate( const char * pszName,
                                        int /* nBands */,
                                        int /* nXSize */,
                                        int /* nYSize */,
                                        GDALDataType /* eDT */,
                                        char **papszOptions )
{
    OGRHSSDataSource *poDS = new OGRHSSDataSource();

    if( !poDS->Create( pszName, papszOptions ) )
    {
        delete poDS;
        poDS = nullptr;
    }

    return poDS;
}

/************************************************************************/
/*                               Delete()                               */
/************************************************************************/

static CPLErr OGRHSSDriverDelete( const char *pszFilename )

{
    if( VSIUnlink( pszFilename ) == 0 )
        return CE_None;

    return CE_Failure;
}

/************************************************************************/
/*                           RegisterOGRHSS()                           */
/************************************************************************/

void RegisterOGRHSS()

{
    if( GDALGetDriverByName( "HSS" ) != nullptr )
        return;

    GDALDriver  *poDriver = new GDALDriver();

    poDriver->SetDescription( "HSS" );
    poDriver->SetMetadataItem( GDAL_DCAP_VECTOR, "YES" );
    poDriver->SetMetadataItem( GDAL_DMD_LONGNAME, "Hakuna Snapshot" );
    poDriver->SetMetadataItem( GDAL_DMD_EXTENSION, "HSS" );
    poDriver->SetMetadataItem( GDAL_DMD_HELPTOPIC, "drivers/vector/HSS.html" );
    poDriver->SetMetadataItem( GDAL_DCAP_VIRTUALIO, "YES" );
    poDriver->SetMetadataItem( GDAL_DMD_CREATIONOPTIONLIST,
                                "<CreationOptionList />");
    poDriver->SetMetadataItem( GDAL_DS_LAYER_CREATIONOPTIONLIST,
                               "<LayerCreationOptionList/>");

    poDriver->pfnOpen = OGRHSSDriverOpen;
    poDriver->pfnCreate = OGRHSSDriverCreate;
    poDriver->pfnDelete = OGRHSSDriverDelete;

    GetGDALDriverManager()->RegisterDriver( poDriver );
}