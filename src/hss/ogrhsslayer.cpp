
#include "ogr_hss.h"
#include "cpl_conv.h"
#include "cpl_string.h"
#include "cpl_csv.h"
#include "ogr_p.h"

CPL_CVSID("$Id$")

/************************************************************************/
/*                            OGRHSSLayer()                             */
/*                                                                      */
/*      Note that the OGRHSSLayer assumes ownership of the passed       */
/*      file pointer.                                                   */
/************************************************************************/

OGRHSSLayer::OGRHSSLayer( const char *pszFilename,
                          const char* layerName,
                          OGRwkbGeometryType eLayerGeomType,
                          int bWriterIn,
                          OGRHSSDataSource* poDSIn) :
    poDS(poDSIn),
    bWriter(bWriterIn),
    failed(false),
    nFeatures(0)
{
    poFeatureDefn = new OGRFeatureDefn(
        CPLSPrintf( "%s_%s",
                    CPLGetBasename( pszFilename ),
                    layerName ));
    poFeatureDefn->Reference();
    poFeatureDefn->SetGeomType( eLayerGeomType );
    SetDescription( poFeatureDefn->GetName() );

    fpHSS = nullptr;
    
}

/************************************************************************/
/*                            ~OGRHSSLayer()                            */
/************************************************************************/

OGRHSSLayer::~OGRHSSLayer()

{
    poFeatureDefn->Release();

    if (fpHSS)
        VSIFCloseL( fpHSS );
}

/************************************************************************/
/*                         SetFeatureIndexTable()                       */
/************************************************************************/
void OGRHSSLayer::SetFeatureIndexTable(std::vector<OffsetAndLine>&& offsetAndLineFeaturesTableIn)
{
    nFeatures = static_cast<int>(offsetAndLineFeaturesTableIn.size());
    offsetAndLineFeaturesTable = std::move(offsetAndLineFeaturesTableIn);
}

/************************************************************************/
/*                            ResetReading()                            */
/************************************************************************/

void OGRHSSLayer::ResetReading()

{
    if( fpHSS == nullptr )
        return;
    eof = false;
    failed = false;
    curLine = 0;
    nNextFID = 0;
    CPL_IGNORE_RET_VAL(VSIFSeekL( fpHSS, 0, SEEK_SET ));
}

/************************************************************************/
/*                           GetNextFeature()                           */
/************************************************************************/

OGRFeature *OGRHSSLayer::GetNextFeature()
{
    if( failed || eof || fpHSS == nullptr  )
        return nullptr;

    return nullptr;
}

/************************************************************************/
/*                      WriteFeatureAttributes()                        */
/************************************************************************/

void OGRHSSLayer::WriteFeatureAttributes( VSILFILE* fp, OGRFeature *poFeature )
{
   
}

/************************************************************************/
/*                           ICreateFeature()                            */
/************************************************************************/

OGRErr OGRHSSLayer::ICreateFeature( OGRFeature *poFeature )

{
    OGRGeometry     *poGeom = poFeature->GetGeometryRef();

    return OGRERR_FAILURE;
}

/************************************************************************/
/*                            CreateField()                             */
/************************************************************************/

OGRErr OGRHSSLayer::CreateField( OGRFieldDefn *poField, int /* bApproxOK */ )
{
    return OGRERR_FAILURE;

}

/************************************************************************/
/*                           GetFeature()                               */
/************************************************************************/

OGRFeature *  OGRHSSLayer::GetFeature( GIntBig nFID )
{
    return nullptr;

}

/************************************************************************/
/*                           TestCapability()                           */
/************************************************************************/

int OGRHSSLayer::TestCapability( const char * pszCap )

{
    if( EQUAL(pszCap,OLCSequentialWrite) )
        return bWriter;
    else if( EQUAL(pszCap,OLCCreateField) )
        return bWriter && nFeatures == 0;
    else
        return FALSE;
}
