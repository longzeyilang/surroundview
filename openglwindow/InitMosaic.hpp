
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <vector>

#include "opengl_common.h"

#define SCALE1 100000
#define SCALE2 100
#define SCALE3 10000

#define IMGWIDTH 1280
#define IMGHEIGHT 720

#define LENGTH 65

#define PI 3.141592653589793238462643383279
#define RADIAN (PI / 180.0)

#define MAX_WHEEL_ANGLE (900 / 23.6)

#define AVERAGE_COUNT 10

#define SHADOW_X_OFFSET 0.0  //0.28//0.2//0.1
#define SHADOW_Y_OFFSET 0.05 //0.19//0.15//0.05

typedef struct _vec2
{
    float x;
    float y;
} vec2;

typedef struct _vec3
{
    float x;
    float y;
    float z;
} vec3;

typedef struct _vec4
{
    float r;
    float g;
    float b;
    float a;
} vec4;

typedef struct PARA_FIELD
{
    int car_width;
    int car_length;
    int LRchess2carFront_distance;
    int chessboard_width_corners;
    int chessboard_length_corners;
    int square_size;
    int carWorldX;
    int carWorldX2;
    int carWorldY;
    int carWorldY2;
} PARA_FIELD;

typedef struct _textureCoords {
  std::vector<vec2> glTexCoord_F;
  std::vector<vec2> glTexCoord_B;
  std::vector<vec2> glTexCoord_L;
  std::vector<vec2> glTexCoord_R;
  std::vector<vec2> glTexCoord_FL_F;
  std::vector<vec2> glTexCoord_FL_L;
  std::vector<vec2> glTexCoord_FR_F;
  std::vector<vec2> glTexCoord_FR_R;
  std::vector<vec2> glTexCoord_BL_B;
  std::vector<vec2> glTexCoord_BL_L;
  std::vector<vec2> glTexCoord_BR_B;
  std::vector<vec2> glTexCoord_BR_R;
} TexCoords;

typedef struct _objectPoints {
  std::vector<vec3> glObjPoints_F;
  std::vector<vec3> glObjPoints_B;
  std::vector<vec3> glObjPoints_L;
  std::vector<vec3> glObjPoints_R;
  std::vector<vec3> glObjPoints_FL_F;
  std::vector<vec3> glObjPoints_FL_L;
  std::vector<vec3> glObjPoints_FR_F;
  std::vector<vec3> glObjPoints_FR_R;
  std::vector<vec3> glObjPoints_BL_B;
  std::vector<vec3> glObjPoints_BL_L;
  std::vector<vec3> glObjPoints_BR_B;
  std::vector<vec3> glObjPoints_BR_R;
} ObjPoints;

typedef struct _vertexCoords {
  std::vector<vec3> glVertex_F;
  std::vector<vec3> glVertex_B;
  std::vector<vec3> glVertex_L;
  std::vector<vec3> glVertex_R;
  std::vector<vec3> glVertex_FL;
  std::vector<vec3> glVertex_FR;
  std::vector<vec3> glVertex_BL;
  std::vector<vec3> glVertex_BR;
} VertexCoords;

typedef struct _blendAlpha
{
    std::vector<float> glAlpha_FL;
    std::vector<float> glAlpha_FR;
    std::vector<float> glAlpha_BL;
    std::vector<float> glAlpha_BR;
} BlendAlpha;

typedef struct _lumiaAdjust
{
    std::vector<float> glLumiaAdjust_F;
    std::vector<float> glLumiaAdjust_B;
    std::vector<float> glLumiaAdjust_L;
    std::vector<float> glLumiaAdjust_R;
} LumiaAdjust;

typedef struct _objectPointsStatistics {
  std::vector<vec3> glObjPoints_FL_F;
  std::vector<vec3> glObjPoints_FL_L;
  std::vector<vec3> glObjPoints_FR_F;
  std::vector<vec3> glObjPoints_FR_R;
  std::vector<vec3> glObjPoints_BL_B;
  std::vector<vec3> glObjPoints_BL_L;
  std::vector<vec3> glObjPoints_BR_B;
  std::vector<vec3> glObjPoints_BR_R;
} ObjPointsStatistics;

typedef struct _textureCoordsStatistics
{
    std::vector<int> glTexCoord_FL_F;
    std::vector<int> glTexCoord_FL_L;
    std::vector<int> glTexCoord_FR_F;
    std::vector<int> glTexCoord_FR_R;
    std::vector<int> glTexCoord_BL_B;
    std::vector<int> glTexCoord_BL_L;
    std::vector<int> glTexCoord_BR_B;
    std::vector<int> glTexCoord_BR_R;
} TexCoordsStatistics;

typedef struct _VBO3DMosaicImage
{
    GLuint CamVerticesPoints[4];
    GLuint CamImagePoints[4];
    GLuint MosaicCamVerticesPoints[4];
    GLuint MosaicFLCamImagePoints[2];
    GLuint MosaicFRCamImagePoints[2];
    GLuint MosaicBLCamImagePoints[2];
    GLuint MosaicBRCamImagePoints[2];
    GLuint LumiaBalance[4];
    GLuint Alpha[4];
    GLuint CarVerTexCoord[2];
} VBO3DMosaicImage;

typedef struct _cam_params {
  int mrInt[3];
  int mtInt[3];
  int mimdInt[8];
  float mr[3];
  float mt[3];
  float mi[9];
  float md[4];
} camParams;

typedef struct undistortParams
{
    float x;
    float y;
    float xZoom;
    float yZoom;
} undistortParams;

extern void readParamsXML(int flag);

extern void init2DModel(int flag);
extern void initTextureCoords(int flag);
extern void initVBO();
extern void initCamParaData();
extern void getCamPixelPosition(int flag);
extern void findRearCurve(float wheelAngle, undistortParams params,
                          float *camera, float *distortTable, float *rVec,
                          float *tVec, double *invR, int width, int height);
extern void getInvertMatrix(double *src, double *dst);

extern void rotateVectorToRotateMatrix(float *vector, float *matrix);

extern PARA_FIELD para_field[2];

extern VertexCoords vertexCoords2D[2];
extern TexCoords texCoords2D[2];

extern VBO3DMosaicImage VBO2DMosaicImageParams[2];

extern GLuint curveVerticesPoints[2];
extern GLuint cameraVerTexCoord[2];

extern TexCoordsStatistics texCoordsStatistics2D[2];

extern camParams frontCamParams[2]; 
extern camParams rearCamParams[2];
extern camParams leftCamParams[2];
extern camParams rightCamParams[2];
extern camParams backWardCamParams[2];


extern vec3 verticesRearTrajLinePoint[11][LENGTH * 2];
