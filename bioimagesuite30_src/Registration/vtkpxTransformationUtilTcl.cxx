// tcl wrapper for vtkpxTransformationUtil object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxTransformationUtil.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxTransformationUtilNewCommand()
{
  vtkpxTransformationUtil *temp = vtkpxTransformationUtil::New();
  return static_cast<ClientData>(temp);
}

int vtkObjectCppCommand(vtkObject *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxTransformationUtilCppCommand(vtkpxTransformationUtil *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxTransformationUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxTransformationUtilCppCommand(static_cast<vtkpxTransformationUtil *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxTransformationUtilCppCommand(vtkpxTransformationUtil *op, Tcl_Interp *interp,
             int argc, char *argv[])
{
  int    tempi = 0;      (void)tempi;
  double tempd = 0.0;    (void)tempd;
  static char temps[80]; (void)temps;
  int    error = 0;      (void)error;
  temps[0] = 0;

  if (argc < 2)
    {
    Tcl_SetResult(interp,const_cast<char *>("Could not find requested method."), TCL_VOLATILE);
    return TCL_ERROR;
    }
  if (!interp)
    {
    if (!strcmp("DoTypecasting",argv[0]))
      {
      if (!strcmp("vtkpxTransformationUtil",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkObjectCppCommand(static_cast<vtkObject *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkObject"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxTransformationUtil  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxTransformationUtil");
    return TCL_OK;
    }
  if ((!strcmp("GetClassName",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetClassName();
    if (temp20)
      {
      Tcl_SetResult(interp, const_cast<char *>(temp20), TCL_VOLATILE);
      }
    else
      {
      Tcl_ResetResult(interp);
      }
    return TCL_OK;
    }
  if ((!strcmp("IsA",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->IsA(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("NewInstance",argv[1]))&&(argc == 2))
    {
    vtkpxTransformationUtil  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxTransformationUtil");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxTransformationUtil  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxTransformationUtil");
    return TCL_OK;
    }
    }
  if ((!strcmp("SaveAbstractTransform",argv[1]))&&(argc == 4))
    {
    vtkAbstractTransform  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->SaveAbstractTransform(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("LoadAbstractTransform",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    vtkAbstractTransform  *temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->LoadAbstractTransform(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
    return TCL_OK;
    }
    }
  if ((!strcmp("LoadMatrix",argv[1]))&&(argc == 4))
    {
    vtkMatrix4x4  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkMatrix4x4 *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkMatrix4x4"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->LoadMatrix(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SaveMatrix",argv[1]))&&(argc == 4))
    {
    vtkMatrix4x4  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkMatrix4x4 *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkMatrix4x4"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->SaveMatrix(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("LoadMatrixTransform",argv[1]))&&(argc == 4))
    {
    vtkTransform  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkTransform"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->LoadMatrixTransform(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SaveMatrixTransform",argv[1]))&&(argc == 4))
    {
    vtkTransform  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkTransform"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->SaveMatrixTransform(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("LoadThinPlateSplineTransform",argv[1]))&&(argc == 4))
    {
    vtkThinPlateSplineTransform  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkThinPlateSplineTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkThinPlateSplineTransform"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->LoadThinPlateSplineTransform(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SaveThinPlateSplineTransform",argv[1]))&&(argc == 4))
    {
    vtkThinPlateSplineTransform  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkThinPlateSplineTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkThinPlateSplineTransform"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->SaveThinPlateSplineTransform(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("LoadGridTransform",argv[1]))&&(argc == 4))
    {
    vtkpxGridTransform  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxGridTransform"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->LoadGridTransform(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SaveGridTransform",argv[1]))&&(argc == 4))
    {
    vtkpxGridTransform  *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxGridTransform"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->SaveGridTransform(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateTransform",argv[1]))&&(argc == 5))
    {
    vtkTransform  *temp0;
    vtkPoints  *temp1;
    int      temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkTransform"),interp,error));
    temp1 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPoints"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->GenerateTransform(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FixMmvregOutput",argv[1]))&&(argc == 4))
    {
    vtkMatrix4x4  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkMatrix4x4 *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkMatrix4x4"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->FixMmvregOutput(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetJacobian",argv[1]))&&(argc == 7))
    {
    vtkAbstractTransform  *temp0;
    vtkpxMatrix  *temp1;
    float    temp2;
    float    temp3;
    float    temp4;
    float    temp20;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (!error)
    {
    temp20 = (op)->GetJacobian(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreatePolygonalModel",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkPolyData  *temp1;
    float    temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPolyData"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->CreatePolygonalModel(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateRegionLabelImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    float    temp1;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->CreateRegionLabelImage(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateRegionLabelImage",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    float    temp1;
    vtkPolyData  *temp2;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    temp2 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkPolyData"),interp,error));
    if (!error)
    {
    temp20 = (op)->CreateRegionLabelImage(temp0,temp1,temp2);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ChopLeg",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkFloatArray  *temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkFloatArray"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->ChopLeg(temp0,temp1,temp2,temp3,temp4);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaskImage",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkFloatArray  *temp1;
    int      temp2;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkFloatArray"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->GetMaskImage(temp0,temp1,temp2);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaskImage",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkFloatArray  *temp1;
    int      temp2;
    int      temp3;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkFloatArray"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->GetMaskImage(temp0,temp1,temp2,temp3);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateWholeBodyRegionLabelImage",argv[1]))&&(argc == 10))
    {
    vtkImageData  *temp0;
    vtkFloatArray  *temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    int      temp5;
    int      temp6;
    int      temp7;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkFloatArray"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (Tcl_GetInt(interp,argv[8],&tempi) != TCL_OK) error = 1;
    temp6 = tempi;
    if (Tcl_GetInt(interp,argv[9],&tempi) != TCL_OK) error = 1;
    temp7 = tempi;
    if (!error)
    {
    temp20 = (op)->CreateWholeBodyRegionLabelImage(temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCentroids",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    double   temp1;
    vtkPoints  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->GetCentroids(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPoints");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPlaneNumber",argv[1]))&&(argc == 9))
    {
    int      temp0;
    int      temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    int      temp5;
    int      temp6;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (Tcl_GetInt(interp,argv[8],&tempi) != TCL_OK) error = 1;
    temp6 = tempi;
    if (!error)
    {
    temp20 = (op)->GetPlaneNumber(temp0,temp1,temp2,temp3,temp4,temp5,temp6);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxTransformationUtilCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkObjectCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxTransformationUtil:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SaveAbstractTransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  LoadAbstractTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  LoadMatrix\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SaveMatrix\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  LoadMatrixTransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SaveMatrixTransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  LoadThinPlateSplineTransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SaveThinPlateSplineTransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  LoadGridTransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SaveGridTransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GenerateTransform\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  FixMmvregOutput\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetJacobian\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  CreatePolygonalModel\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateRegionLabelImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateRegionLabelImage\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ChopLeg\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  GetMaskImage\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetMaskImage\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateWholeBodyRegionLabelImage\t with 8 args\n",NULL);
    Tcl_AppendResult(interp,"  GetCentroids\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetPlaneNumber\t with 7 args\n",NULL);
    return TCL_OK;
    }

  if (!strcmp("DescribeMethods",argv[1]))
    {
    if(argc>3) {
      Tcl_SetResult ( interp, const_cast<char*>("Wrong number of arguments: object DescribeMethods <MethodName>"), TCL_VOLATILE ); 
      return TCL_ERROR;
 }
    if(argc==2) {

  Tcl_DString dString, dStringParent;

  Tcl_DStringInit ( &dString );

  Tcl_DStringInit ( &dStringParent );
    vtkObjectCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SaveAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "LoadAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "LoadMatrix" );
    Tcl_DStringAppendElement ( &dString, "SaveMatrix" );
    Tcl_DStringAppendElement ( &dString, "LoadMatrixTransform" );
    Tcl_DStringAppendElement ( &dString, "SaveMatrixTransform" );
    Tcl_DStringAppendElement ( &dString, "LoadThinPlateSplineTransform" );
    Tcl_DStringAppendElement ( &dString, "SaveThinPlateSplineTransform" );
    Tcl_DStringAppendElement ( &dString, "LoadGridTransform" );
    Tcl_DStringAppendElement ( &dString, "SaveGridTransform" );
    Tcl_DStringAppendElement ( &dString, "GenerateTransform" );
    Tcl_DStringAppendElement ( &dString, "FixMmvregOutput" );
    Tcl_DStringAppendElement ( &dString, "GetJacobian" );
    Tcl_DStringAppendElement ( &dString, "CreatePolygonalModel" );
    Tcl_DStringAppendElement ( &dString, "CreateRegionLabelImage" );
    Tcl_DStringAppendElement ( &dString, "CreateRegionLabelImage" );
    Tcl_DStringAppendElement ( &dString, "ChopLeg" );
    Tcl_DStringAppendElement ( &dString, "GetMaskImage" );
    Tcl_DStringAppendElement ( &dString, "GetMaskImage" );
    Tcl_DStringAppendElement ( &dString, "CreateWholeBodyRegionLabelImage" );
    Tcl_DStringAppendElement ( &dString, "GetCentroids" );
    Tcl_DStringAppendElement ( &dString, "GetPlaneNumber" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkObjectCppCommand(op,interp,argc,argv);
    if ( SuperClassStatus == TCL_OK ) { return TCL_OK; }
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkpxTransformationUtil *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for New */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetClassName */
    if ( strcmp ( argv[2], "GetClassName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetClassName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "const char *GetClassName();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for GetClassName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IsA */
    if ( strcmp ( argv[2], "IsA" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IsA */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int IsA(const char *name);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for IsA */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NewInstance */
    if ( strcmp ( argv[2], "NewInstance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NewInstance */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for NewInstance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SafeDownCast */
    if ( strcmp ( argv[2], "SafeDownCast" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SafeDownCast */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaveAbstractTransform */
    if ( strcmp ( argv[2], "SaveAbstractTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaveAbstractTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaveAbstractTransform */
    Tcl_DStringAppendElement ( &dString, " Save And Load Abstract Transformation as matrix/grid/thin-plate-spline\n" );
    Tcl_DStringAppendElement ( &dString, "static int SaveAbstractTransform(vtkAbstractTransform *tr, const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for SaveAbstractTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LoadAbstractTransform */
    if ( strcmp ( argv[2], "LoadAbstractTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LoadAbstractTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LoadAbstractTransform */
    Tcl_DStringAppendElement ( &dString, " Save And Load Abstract Transformation as matrix/grid/thin-plate-spline\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkAbstractTransform *LoadAbstractTransform(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for LoadAbstractTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LoadMatrix */
    if ( strcmp ( argv[2], "LoadMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LoadMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkMatrix4x4" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LoadMatrix */
    Tcl_DStringAppendElement ( &dString, " Matrix I/O\n" );
    Tcl_DStringAppendElement ( &dString, "static int LoadMatrix(vtkMatrix4x4 *mat, const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for LoadMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaveMatrix */
    if ( strcmp ( argv[2], "SaveMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaveMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkMatrix4x4" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaveMatrix */
    Tcl_DStringAppendElement ( &dString, " Matrix I/O\n" );
    Tcl_DStringAppendElement ( &dString, "static int SaveMatrix(vtkMatrix4x4 *mat, const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for SaveMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LoadMatrixTransform */
    if ( strcmp ( argv[2], "LoadMatrixTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LoadMatrixTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkTransform" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LoadMatrixTransform */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int LoadMatrixTransform(vtkTransform *tr, const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for LoadMatrixTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaveMatrixTransform */
    if ( strcmp ( argv[2], "SaveMatrixTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaveMatrixTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkTransform" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaveMatrixTransform */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int SaveMatrixTransform(vtkTransform *tr, const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for SaveMatrixTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LoadThinPlateSplineTransform */
    if ( strcmp ( argv[2], "LoadThinPlateSplineTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LoadThinPlateSplineTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkThinPlateSplineTransform" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LoadThinPlateSplineTransform */
    Tcl_DStringAppendElement ( &dString, " Thin-Plate-Spline I/O\n" );
    Tcl_DStringAppendElement ( &dString, "static int LoadThinPlateSplineTransform(vtkThinPlateSplineTransform *mat, const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for LoadThinPlateSplineTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaveThinPlateSplineTransform */
    if ( strcmp ( argv[2], "SaveThinPlateSplineTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaveThinPlateSplineTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkThinPlateSplineTransform" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaveThinPlateSplineTransform */
    Tcl_DStringAppendElement ( &dString, " Thin-Plate-Spline I/O\n" );
    Tcl_DStringAppendElement ( &dString, "static int SaveThinPlateSplineTransform(vtkThinPlateSplineTransform *mat, const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for SaveThinPlateSplineTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LoadGridTransform */
    if ( strcmp ( argv[2], "LoadGridTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LoadGridTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LoadGridTransform */
    Tcl_DStringAppendElement ( &dString, " Grid Transform  I/O\n" );
    Tcl_DStringAppendElement ( &dString, "static int LoadGridTransform(vtkpxGridTransform *mat, const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for LoadGridTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaveGridTransform */
    if ( strcmp ( argv[2], "SaveGridTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaveGridTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaveGridTransform */
    Tcl_DStringAppendElement ( &dString, " Grid Transform  I/O\n" );
    Tcl_DStringAppendElement ( &dString, "static int SaveGridTransform(vtkpxGridTransform *mat, const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for SaveGridTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateTransform */
    if ( strcmp ( argv[2], "GenerateTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateTransform */
    Tcl_DStringAppendElement ( &dString, " Creates reslice transform given 2 or 3 points (specified by npoints ) stored in vtkPoints \n" );
    Tcl_DStringAppendElement ( &dString, "static int GenerateTransform(vtkTransform *tr, vtkPoints *pt, int npoints=-1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for GenerateTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FixMmvregOutput */
    if ( strcmp ( argv[2], "FixMmvregOutput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FixMmvregOutput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkMatrix4x4" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FixMmvregOutput */
    Tcl_DStringAppendElement ( &dString, " Fixes mmvreg output to standard coordinate space\n" );
    Tcl_DStringAppendElement ( &dString, "static void FixMmvregOutput(vtkMatrix4x4 *matrix, int permute);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for FixMmvregOutput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetJacobian */
    if ( strcmp ( argv[2], "GetJacobian" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetJacobian" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetJacobian */
    Tcl_DStringAppendElement ( &dString, " GetJacobian\n" );
    Tcl_DStringAppendElement ( &dString, "static float GetJacobian(vtkAbstractTransform *tr, vtkpxMatrix *jac, float x, float y, float z);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for GetJacobian */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreatePolygonalModel */
    if ( strcmp ( argv[2], "CreatePolygonalModel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreatePolygonalModel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreatePolygonalModel */
    Tcl_DStringAppendElement ( &dString, " Create Basis Image\n" );
    Tcl_DStringAppendElement ( &dString, "static int CreatePolygonalModel(vtkImageData *multiframeinput, vtkPolyData *surface, float threshold=50.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for CreatePolygonalModel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateRegionLabelImage */
    if ( strcmp ( argv[2], "CreateRegionLabelImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateRegionLabelImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateRegionLabelImage */
    Tcl_DStringAppendElement ( &dString, " Create Basis Image\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *CreateRegionLabelImage(vtkImageData *input, float sigma);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for CreateRegionLabelImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateRegionLabelImage */
    if ( strcmp ( argv[2], "CreateRegionLabelImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateRegionLabelImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateRegionLabelImage */
    Tcl_DStringAppendElement ( &dString, " Create Basis Image\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *CreateRegionLabelImage(vtkImageData *input, float sigma, vtkPolyData *surface);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for CreateRegionLabelImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ChopLeg */
    if ( strcmp ( argv[2], "ChopLeg" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ChopLeg" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ChopLeg */
    Tcl_DStringAppendElement ( &dString, " Create Basis Image\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ChopLeg(vtkImageData *Leg, vtkFloatArray *planes, int offset, int numcuts, int legmode=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for ChopLeg */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaskImage */
    if ( strcmp ( argv[2], "GetMaskImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaskImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaskImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *GetMaskImage(vtkImageData *input, vtkFloatArray *planes, int offset);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for GetMaskImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaskImage */
    if ( strcmp ( argv[2], "GetMaskImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaskImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaskImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *GetMaskImage(vtkImageData *input, vtkFloatArray *planes, int offset, int flip);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for GetMaskImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateWholeBodyRegionLabelImage */
    if ( strcmp ( argv[2], "CreateWholeBodyRegionLabelImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateWholeBodyRegionLabelImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateWholeBodyRegionLabelImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *CreateWholeBodyRegionLabelImage(vtkImageData *input, vtkFloatArray *planes, int hasfeet, int hashindlimbs, int hasforelimbs, int hashead, int taildivisions, int spinedivisions);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for CreateWholeBodyRegionLabelImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCentroids */
    if ( strcmp ( argv[2], "GetCentroids" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCentroids" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCentroids */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkPoints *GetCentroids(vtkImageData *img, double axislength=10.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for GetCentroids */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPlaneNumber */
    if ( strcmp ( argv[2], "GetPlaneNumber" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPlaneNumber" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPlaneNumber */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int GetPlaneNumber(int jointno, int hasfeet, int hashindlimbs, int hasforelimbs, int hashead, int taildivisions, int spinedivisions);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTransformationUtil" );
    /* Closing for GetPlaneNumber */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkObjectCppCommand(static_cast<vtkObject *>(op),interp,argc,argv) == TCL_OK)
    {
    return TCL_OK;
    }
    }
  catch (std::exception &e)
    {
    Tcl_AppendResult(interp, "Uncaught exception: ",  e.what(), "\n", NULL);
    return TCL_ERROR;
    }
  return TCL_ERROR;
}
