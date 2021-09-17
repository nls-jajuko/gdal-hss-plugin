
#ifndef OGR_HSS_H_INCLUDED
#define OGR_HSS_H_INCLUDED

#include "ogrsf_frmts.h"

#include <vector>

class OGRHSSDataSource;

/************************************************************************/
/*                             OGRHSSLayer                              */
/************************************************************************/

typedef struct
{
  vsi_l_offset   offset;
  int   line;
} OffsetAndLine;

class OGRHSSLayer final: public OGRLayer
{
    OGRFeatureDefn*    poFeatureDefn;

    OGRHSSDataSource*  poDS;
    int                bWriter;

    bool               eof;
    bool               failed;
    int                curLine;
    int                nNextFID;
    VSILFILE*          fpHSS;
    int                nFeatures;
    std::vector<OffsetAndLine> offsetAndLineFeaturesTable;

    void               WriteFeatureAttributes(VSILFILE* fp, OGRFeature *poFeature);

  public:
                        OGRHSSLayer(const char *pszFilename,
                                    const char* layerName,
                                    OGRwkbGeometryType eLayerGeomType,
                                    int bWriter,
                                    OGRHSSDataSource* poDS);
                        ~OGRHSSLayer();

    void                SetFeatureIndexTable(std::vector<OffsetAndLine>&& offsetAndLineFeaturesTable);

    void                ResetReading() override;
    OGRFeature *        GetNextFeature() override;

    OGRErr              ICreateFeature( OGRFeature *poFeature ) override;
    OGRErr              CreateField( OGRFieldDefn *poField, int bApproxOK ) override;

    OGRFeatureDefn *    GetLayerDefn() override { return poFeatureDefn; }

    OGRFeature *        GetFeature( GIntBig nFID ) override;

    int                 TestCapability( const char * ) override;
};

/************************************************************************/
/*                           OGRHSSDataSource                           */
/************************************************************************/

class OGRHSSDataSource final: public OGRDataSource
{
    char*               pszName;

    OGRHSSLayer**       papoLayers;
    int                 nLayers;

    /*  Export related */
    VSILFILE                *fpOutput; /* Virtual file API */
    int                 coordinatePrecision;
    char*               pszCoordinateSeparator;

  public:
                        OGRHSSDataSource();
                        ~OGRHSSDataSource();

    VSILFILE                *GetOutputFP() { return fpOutput; }
    int                 GetCoordinatePrecision() { return coordinatePrecision; }

    int                 Open( const char * pszFilename );
    int                 Create( const char *pszFilename,
                              char **papszOptions );

    const char*         GetName() override { return pszName; }

    int                 GetLayerCount() override { return nLayers; }
    OGRLayer*           GetLayer( int ) override;

    OGRLayer *          ICreateLayer( const char * pszLayerName,
                                    OGRSpatialReference *poSRS,
                                    OGRwkbGeometryType eType,
                                    char ** papszOptions ) override;

    int                 TestCapability( const char * ) override;

    OGRErr                exportFeature(OGRFeature *poFeature);

    void                PrintLine(const char *fmt, ...) CPL_PRINT_FUNC_FORMAT (2, 3);    
};


#endif