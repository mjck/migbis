// tcl wrapper for vtkpxRegistration object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxRegistration.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxRegistrationNewCommand()
{
  vtkpxRegistration *temp = vtkpxRegistration::New();
  return static_cast<ClientData>(temp);
}

int vtkProcessObjectCppCommand(vtkProcessObject *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxRegistrationCppCommand(vtkpxRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxRegistrationCppCommand(static_cast<vtkpxRegistration *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxRegistrationCppCommand(vtkpxRegistration *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxRegistration",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkProcessObjectCppCommand(static_cast<vtkProcessObject *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkProcessObject"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
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
    vtkpxRegistration  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxRegistration");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxRegistration  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxRegistration");
    return TCL_OK;
    }
    }
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxRegistration  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxRegistration");
    return TCL_OK;
    }
  if ((!strcmp("ComputeJacobianImage",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkAbstractTransform  *temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeJacobianImage(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeSimpleJacobianImage",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkAbstractTransform  *temp2;
    float    temp3;
    float    temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeSimpleJacobianImage(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeSimpleJacobianImage",argv[1]))&&(argc == 8))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkAbstractTransform  *temp2;
    float    temp3;
    float    temp4;
    float    temp5;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeSimpleJacobianImage(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ConvertAreaExpansionImageToSurface",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkPolyData  *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPolyData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ConvertAreaExpansionImageToSurface(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeAreaExpansionSurface",argv[1]))&&(argc == 6))
    {
    vtkPolyData  *temp0;
    vtkAbstractTransform  *temp1;
    float    temp2;
    float    temp3;
    float    temp20;
    error = 0;

    temp0 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPolyData"),interp,error));
    temp1 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeAreaExpansionSurface(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SmoothAreaExpansionSurface",argv[1]))&&(argc == 5))
    {
    vtkPolyData  *temp0;
    int      temp1;
    float    temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPolyData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->SmoothAreaExpansionSurface(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeAreaExpansionImage",argv[1]))&&(argc == 3))
    {
    vtkPolyData  *temp0;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPolyData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeAreaExpansionImage(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputePrincipalStrains",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkAbstractTransform  *temp2;
    float    temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputePrincipalStrains(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeJacobianImage",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkAbstractTransform  *temp2;
    float    temp3;
    float    temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeJacobianImage(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeJacobianImage",argv[1]))&&(argc == 8))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkAbstractTransform  *temp2;
    float    temp3;
    float    temp4;
    float    temp5;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeJacobianImage(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeCompleteJacobianImage",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkpxComboTransform  *temp2;
    int      temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkpxComboTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkpxComboTransform"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeCompleteJacobianImage(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeAverageJacobian",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkAbstractTransform  *temp1;
    float    temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeAverageJacobian(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeAverageJacobian",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkAbstractTransform  *temp1;
    float    temp2;
    float    temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeAverageJacobian(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CenterImageOrigin",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->CenterImageOrigin(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeTotalGroupEntropy",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    double   temp1;
    short    temp2;
    double   temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeTotalGroupEntropy(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeWeightedTotalGroupEntropy",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    double   temp2;
    short    temp3;
    double   temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeWeightedTotalGroupEntropy(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeVoxelwiseEntropyImage",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    double   temp1;
    short    temp2;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeVoxelwiseEntropyImage(temp0,temp1,temp2);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeSimpleSimilarityBetweenImages",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkAbstractTransform  *temp2;
    float    temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeSimpleSimilarityBetweenImages(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeAllSimilarityMetricsBetweenImages",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkAbstractTransform  *temp2;
    vtkFloatArray  *temp3;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp3 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkFloatArray"),interp,error));
    if (!error)
    {
    op->ComputeAllSimilarityMetricsBetweenImages(temp0,temp1,temp2,temp3);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeAllSimilarityMetricsBetweenImages",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkAbstractTransform  *temp2;
    vtkFloatArray  *temp3;
    int      temp4;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp3 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkFloatArray"),interp,error));
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    op->ComputeAllSimilarityMetricsBetweenImages(temp0,temp1,temp2,temp3,temp4);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("AdjustImage",argv[1]))&&(argc == 9))
    {
    vtkImageData  *temp0;
    int      temp1;
    short    temp2;
    short    temp3;
    int      temp4;
    int      temp5;
    int      temp6;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
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
    temp20 = (op)->AdjustImage(temp0,temp1,temp2,temp3,temp4,temp5,temp6);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ScaleWeightImage",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    double   temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->ScaleWeightImage(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ExtractComponentAndCastImageToShort",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp2;
    int      temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->ExtractComponentAndCastImageToShort(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ResliceImage",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    vtkAbstractTransform  *temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->ResliceImage(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("MultiplyAndOverWriteImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->MultiplyAndOverWriteImage(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ReOrientImage",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp2;
    int      temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->ReOrientImage(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("HistogramEvaluate",argv[1]))&&(argc == 9))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkAbstractTransform  *temp2;
    vtkpxJointHistogram  *temp3;
    int      temp4;
    int      temp5;
    int      temp6;
    float    temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp3 = (vtkpxJointHistogram *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkpxJointHistogram"),interp,error));
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (Tcl_GetInt(interp,argv[8],&tempi) != TCL_OK) error = 1;
    temp6 = tempi;
    if (!error)
    {
    temp20 = (op)->HistogramEvaluate(temp0,temp1,temp2,temp3,temp4,temp5,temp6);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("WeightedHistogramEvaluate",argv[1]))&&(argc == 11))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    vtkImageData  *temp3;
    vtkAbstractTransform  *temp4;
    vtkpxJointHistogram  *temp5;
    int      temp6;
    int      temp7;
    int      temp8;
    float    temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkImageData"),interp,error));
    temp4 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[6],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp5 = (vtkpxJointHistogram *)(vtkTclGetPointerFromObject(argv[7],const_cast<char *>("vtkpxJointHistogram"),interp,error));
    if (Tcl_GetInt(interp,argv[8],&tempi) != TCL_OK) error = 1;
    temp6 = tempi;
    if (Tcl_GetInt(interp,argv[9],&tempi) != TCL_OK) error = 1;
    temp7 = tempi;
    if (Tcl_GetInt(interp,argv[10],&tempi) != TCL_OK) error = 1;
    temp8 = tempi;
    if (!error)
    {
    temp20 = (op)->WeightedHistogramEvaluate(temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SeamImages",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkLinearTransform  *temp2;
    vtkImageData  *temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkLinearTransform *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkLinearTransform"),interp,error));
    temp3 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->SeamImages(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("AutoReorientImages",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    int      temp1;
    vtkImageData  *temp2;
    int      temp3;
    vtkImageData  *temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    temp4 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[6],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->AutoReorientImages(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("PrintImageInfo",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    char    *temp1;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = argv[3];
    if (!error)
    {
    op->PrintImageInfo(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("FixPipelineSpacingAndOrigin",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->FixPipelineSpacingAndOrigin(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxRegistrationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkProcessObjectCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxRegistration:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  ComputeJacobianImage\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeSimpleJacobianImage\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeSimpleJacobianImage\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  ConvertAreaExpansionImageToSurface\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeAreaExpansionSurface\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  SmoothAreaExpansionSurface\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeAreaExpansionImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ComputePrincipalStrains\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeJacobianImage\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeJacobianImage\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeCompleteJacobianImage\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeAverageJacobian\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeAverageJacobian\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CenterImageOrigin\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ComputeTotalGroupEntropy\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeWeightedTotalGroupEntropy\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeVoxelwiseEntropyImage\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeSimpleSimilarityBetweenImages\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeAllSimilarityMetricsBetweenImages\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeAllSimilarityMetricsBetweenImages\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  AdjustImage\t with 7 args\n",NULL);
    Tcl_AppendResult(interp,"  ScaleWeightImage\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ExtractComponentAndCastImageToShort\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ResliceImage\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  MultiplyAndOverWriteImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ReOrientImage\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  HistogramEvaluate\t with 7 args\n",NULL);
    Tcl_AppendResult(interp,"  WeightedHistogramEvaluate\t with 9 args\n",NULL);
    Tcl_AppendResult(interp,"  SeamImages\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  AutoReorientImages\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  PrintImageInfo\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  FixPipelineSpacingAndOrigin\t with 1 arg\n",NULL);
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
    vtkProcessObjectCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "ComputeJacobianImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeSimpleJacobianImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeSimpleJacobianImage" );
    Tcl_DStringAppendElement ( &dString, "ConvertAreaExpansionImageToSurface" );
    Tcl_DStringAppendElement ( &dString, "ComputeAreaExpansionSurface" );
    Tcl_DStringAppendElement ( &dString, "SmoothAreaExpansionSurface" );
    Tcl_DStringAppendElement ( &dString, "ComputeAreaExpansionImage" );
    Tcl_DStringAppendElement ( &dString, "ComputePrincipalStrains" );
    Tcl_DStringAppendElement ( &dString, "ComputeJacobianImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeJacobianImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeCompleteJacobianImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeAverageJacobian" );
    Tcl_DStringAppendElement ( &dString, "ComputeAverageJacobian" );
    Tcl_DStringAppendElement ( &dString, "CenterImageOrigin" );
    Tcl_DStringAppendElement ( &dString, "ComputeTotalGroupEntropy" );
    Tcl_DStringAppendElement ( &dString, "ComputeWeightedTotalGroupEntropy" );
    Tcl_DStringAppendElement ( &dString, "ComputeVoxelwiseEntropyImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeSimpleSimilarityBetweenImages" );
    Tcl_DStringAppendElement ( &dString, "ComputeAllSimilarityMetricsBetweenImages" );
    Tcl_DStringAppendElement ( &dString, "ComputeAllSimilarityMetricsBetweenImages" );
    Tcl_DStringAppendElement ( &dString, "AdjustImage" );
    Tcl_DStringAppendElement ( &dString, "ScaleWeightImage" );
    Tcl_DStringAppendElement ( &dString, "ExtractComponentAndCastImageToShort" );
    Tcl_DStringAppendElement ( &dString, "ResliceImage" );
    Tcl_DStringAppendElement ( &dString, "MultiplyAndOverWriteImage" );
    Tcl_DStringAppendElement ( &dString, "ReOrientImage" );
    Tcl_DStringAppendElement ( &dString, "HistogramEvaluate" );
    Tcl_DStringAppendElement ( &dString, "WeightedHistogramEvaluate" );
    Tcl_DStringAppendElement ( &dString, "SeamImages" );
    Tcl_DStringAppendElement ( &dString, "AutoReorientImages" );
    Tcl_DStringAppendElement ( &dString, "PrintImageInfo" );
    Tcl_DStringAppendElement ( &dString, "FixPipelineSpacingAndOrigin" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkProcessObjectCppCommand(op,interp,argc,argv);
    if ( SuperClassStatus == TCL_OK ) { return TCL_OK; }
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
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, " This one also \n" );
    Tcl_DStringAppendElement ( &dString, "static vtkpxRegistration *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for New */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeJacobianImage */
    if ( strcmp ( argv[2], "ComputeJacobianImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeJacobianImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeJacobianImage */
    Tcl_DStringAppendElement ( &dString, " Generate Jacobian Image using transformation tr and reference ref and transform tr\n" );
    Tcl_DStringAppendElement ( &dString, "static int ComputeJacobianImage(vtkImageData *ref, vtkImageData *jacobian, vtkAbstractTransform *tr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeJacobianImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeSimpleJacobianImage */
    if ( strcmp ( argv[2], "ComputeSimpleJacobianImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeSimpleJacobianImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeSimpleJacobianImage */
    Tcl_DStringAppendElement ( &dString, " Generate Jacobian Image using transformation tr and reference ref and transform tr\n" );
    Tcl_DStringAppendElement ( &dString, "static int ComputeSimpleJacobianImage(vtkImageData *ref, vtkImageData *jacobian, vtkAbstractTransform *tr, float scale=100, float offset=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeSimpleJacobianImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeSimpleJacobianImage */
    if ( strcmp ( argv[2], "ComputeSimpleJacobianImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeSimpleJacobianImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeSimpleJacobianImage */
    Tcl_DStringAppendElement ( &dString, " Generate Jacobian Image using transformation tr and reference ref and transform tr\n" );
    Tcl_DStringAppendElement ( &dString, "static int ComputeSimpleJacobianImage(vtkImageData *ref, vtkImageData *jacobian, vtkAbstractTransform *tr, float scale, float offset, float threshold);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeSimpleJacobianImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ConvertAreaExpansionImageToSurface */
    if ( strcmp ( argv[2], "ConvertAreaExpansionImageToSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ConvertAreaExpansionImageToSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ConvertAreaExpansionImageToSurface */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int ConvertAreaExpansionImageToSurface(vtkImageData *img, vtkPolyData *surface);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ConvertAreaExpansionImageToSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeAreaExpansionSurface */
    if ( strcmp ( argv[2], "ComputeAreaExpansionSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeAreaExpansionSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeAreaExpansionSurface */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static float ComputeAreaExpansionSurface(vtkPolyData *surface, vtkAbstractTransform *tr, float scale=100.0, float offset=1.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeAreaExpansionSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SmoothAreaExpansionSurface */
    if ( strcmp ( argv[2], "SmoothAreaExpansionSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SmoothAreaExpansionSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SmoothAreaExpansionSurface */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int SmoothAreaExpansionSurface(vtkPolyData *surface, int iterations=10, float lambda=0.5);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for SmoothAreaExpansionSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeAreaExpansionImage */
    if ( strcmp ( argv[2], "ComputeAreaExpansionImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeAreaExpansionImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeAreaExpansionImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeAreaExpansionImage(vtkPolyData *surface);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeAreaExpansionImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputePrincipalStrains */
    if ( strcmp ( argv[2], "ComputePrincipalStrains" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputePrincipalStrains" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputePrincipalStrains */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int ComputePrincipalStrains(vtkImageData *ref, vtkImageData *jacobian, vtkAbstractTransform *tr, float threshold=0.05);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputePrincipalStrains */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeJacobianImage */
    if ( strcmp ( argv[2], "ComputeJacobianImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeJacobianImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeJacobianImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int ComputeJacobianImage(vtkImageData *ref, vtkImageData *jacobian, vtkAbstractTransform *tr, float scale, float shift=0.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeJacobianImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeJacobianImage */
    if ( strcmp ( argv[2], "ComputeJacobianImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeJacobianImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeJacobianImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int ComputeJacobianImage(vtkImageData *ref, vtkImageData *jacobian, vtkAbstractTransform *tr, float scale, float shift, float threshold);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeJacobianImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeCompleteJacobianImage */
    if ( strcmp ( argv[2], "ComputeCompleteJacobianImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeCompleteJacobianImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeCompleteJacobianImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int ComputeCompleteJacobianImage(vtkImageData *ref, vtkImageData *jacobian, vtkpxComboTransform *tr, int nonlinearonly);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeCompleteJacobianImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeAverageJacobian */
    if ( strcmp ( argv[2], "ComputeAverageJacobian" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeAverageJacobian" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeAverageJacobian */
    Tcl_DStringAppendElement ( &dString, " Compute Average Jacobian for image\n" );
    Tcl_DStringAppendElement ( &dString, "static float ComputeAverageJacobian(vtkImageData *ref, vtkAbstractTransform *tr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeAverageJacobian */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeAverageJacobian */
    if ( strcmp ( argv[2], "ComputeAverageJacobian" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeAverageJacobian" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeAverageJacobian */
    Tcl_DStringAppendElement ( &dString, " Compute Average Jacobian for image\n" );
    Tcl_DStringAppendElement ( &dString, "static float ComputeAverageJacobian(vtkImageData *ref, vtkAbstractTransform *tr, float srate);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeAverageJacobian */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CenterImageOrigin */
    if ( strcmp ( argv[2], "CenterImageOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CenterImageOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CenterImageOrigin */
    Tcl_DStringAppendElement ( &dString, " Center Image Origin\n" );
    Tcl_DStringAppendElement ( &dString, "static void CenterImageOrigin(vtkImageData *img);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for CenterImageOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeTotalGroupEntropy */
    if ( strcmp ( argv[2], "ComputeTotalGroupEntropy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeTotalGroupEntropy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeTotalGroupEntropy */
    Tcl_DStringAppendElement ( &dString, " Voxel Wise Estimation of Group Entropy from Miller et al \n Intensity Correction   y(t)=x*(a*t+b) -- defaults a=0 b=1;\n" );
    Tcl_DStringAppendElement ( &dString, "static double ComputeTotalGroupEntropy(vtkImageData *img, double drift=1.0, short ignore=-20000);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeTotalGroupEntropy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeWeightedTotalGroupEntropy */
    if ( strcmp ( argv[2], "ComputeWeightedTotalGroupEntropy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeWeightedTotalGroupEntropy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeWeightedTotalGroupEntropy */
    Tcl_DStringAppendElement ( &dString, " Voxel Wise Estimation of Group Entropy from Miller et al \n Intensity Correction   y(t)=x*(a*t+b) -- defaults a=0 b=1;\n" );
    Tcl_DStringAppendElement ( &dString, "static double ComputeWeightedTotalGroupEntropy(vtkImageData *img, vtkImageData *wgt, double drift=1.0, short ignore=-20000);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeWeightedTotalGroupEntropy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeVoxelwiseEntropyImage */
    if ( strcmp ( argv[2], "ComputeVoxelwiseEntropyImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeVoxelwiseEntropyImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeVoxelwiseEntropyImage */
    Tcl_DStringAppendElement ( &dString, " Voxel Wise Estimation of Group Entropy from Miller et al \n Intensity Correction   y(t)=x*(a*t+b) -- defaults a=0 b=1;\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeVoxelwiseEntropyImage(vtkImageData *img, double drift=1.0, short ignore=-20000);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeVoxelwiseEntropyImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeSimpleSimilarityBetweenImages */
    if ( strcmp ( argv[2], "ComputeSimpleSimilarityBetweenImages" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeSimpleSimilarityBetweenImages" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeSimpleSimilarityBetweenImages */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static float ComputeSimpleSimilarityBetweenImages(vtkImageData *img1, vtkImageData *img2, vtkAbstractTransform *tr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeSimpleSimilarityBetweenImages */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeAllSimilarityMetricsBetweenImages */
    if ( strcmp ( argv[2], "ComputeAllSimilarityMetricsBetweenImages" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeAllSimilarityMetricsBetweenImages" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeAllSimilarityMetricsBetweenImages */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static void ComputeAllSimilarityMetricsBetweenImages(vtkImageData *img1, vtkImageData *img2, vtkAbstractTransform *tr, vtkFloatArray *results);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeAllSimilarityMetricsBetweenImages */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeAllSimilarityMetricsBetweenImages */
    if ( strcmp ( argv[2], "ComputeAllSimilarityMetricsBetweenImages" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeAllSimilarityMetricsBetweenImages" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeAllSimilarityMetricsBetweenImages */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static void ComputeAllSimilarityMetricsBetweenImages(vtkImageData *img1, vtkImageData *img2, vtkAbstractTransform *tr, vtkFloatArray *results, int numbins);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ComputeAllSimilarityMetricsBetweenImages */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AdjustImage */
    if ( strcmp ( argv[2], "AdjustImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AdjustImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AdjustImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int AdjustImage(vtkImageData *image, int maxbins, short minv, short maxv, int logmode, int posmode=0, int autonorm=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for AdjustImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScaleWeightImage */
    if ( strcmp ( argv[2], "ScaleWeightImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScaleWeightImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScaleWeightImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int ScaleWeightImage(vtkImageData *weightimage, vtkImageData *sampledimage, double maxvalue=100.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ScaleWeightImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ExtractComponentAndCastImageToShort */
    if ( strcmp ( argv[2], "ExtractComponentAndCastImageToShort" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ExtractComponentAndCastImageToShort" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ExtractComponentAndCastImageToShort */
    Tcl_DStringAppendElement ( &dString, " Create A Level in the Multi-scale pyramid\n" );
    Tcl_DStringAppendElement ( &dString, "static int ExtractComponentAndCastImageToShort(vtkImageData *destination, vtkImageData *source, int frame, int doscale=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ExtractComponentAndCastImageToShort */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ResliceImage */
    if ( strcmp ( argv[2], "ResliceImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ResliceImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ResliceImage */
    Tcl_DStringAppendElement ( &dString, " Reslice Image using transformation and fill in histogram\n" );
    Tcl_DStringAppendElement ( &dString, "static int ResliceImage(vtkImageData *destination, vtkImageData *source, vtkImageData *ref, vtkAbstractTransform *transform, int interp);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ResliceImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MultiplyAndOverWriteImage */
    if ( strcmp ( argv[2], "MultiplyAndOverWriteImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MultiplyAndOverWriteImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MultiplyAndOverWriteImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int MultiplyAndOverWriteImage(vtkImageData *inout, vtkImageData *scale);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for MultiplyAndOverWriteImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ReOrientImage */
    if ( strcmp ( argv[2], "ReOrientImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ReOrientImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ReOrientImage */
    Tcl_DStringAppendElement ( &dString, " Re-orient Image\n" );
    Tcl_DStringAppendElement ( &dString, "static int ReOrientImage(vtkImageData *destination, vtkImageData *source, int inputorient, int outputorient);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for ReOrientImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: HistogramEvaluate */
    if ( strcmp ( argv[2], "HistogramEvaluate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "HistogramEvaluate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for HistogramEvaluate */
    Tcl_DStringAppendElement ( &dString, " Evaluate Histogram Based Similarity\n" );
    Tcl_DStringAppendElement ( &dString, "static float HistogramEvaluate(vtkImageData *ref, vtkImageData *targ, vtkAbstractTransform *trans, vtkpxJointHistogram *histo, int similaritymeasure, int interp=1, int reset=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for HistogramEvaluate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: WeightedHistogramEvaluate */
    if ( strcmp ( argv[2], "WeightedHistogramEvaluate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "WeightedHistogramEvaluate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for WeightedHistogramEvaluate */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static float WeightedHistogramEvaluate(vtkImageData *ref, vtkImageData *wgt, vtkImageData *targ, vtkImageData *wgt2, vtkAbstractTransform *trans, vtkpxJointHistogram *histo, int similaritymeasure, int interp=1, int reset=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for WeightedHistogramEvaluate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SeamImages */
    if ( strcmp ( argv[2], "SeamImages" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SeamImages" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkLinearTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SeamImages */
    Tcl_DStringAppendElement ( &dString, " Seam Images \n" );
    Tcl_DStringAppendElement ( &dString, "static int SeamImages(vtkImageData *ref, vtkImageData *targ, vtkLinearTransform *tr, vtkImageData *output, int intensitymap);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for SeamImages */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AutoReorientImages */
    if ( strcmp ( argv[2], "AutoReorientImages" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AutoReorientImages" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AutoReorientImages */
    Tcl_DStringAppendElement ( &dString, " Auto Reorient Image\n" );
    Tcl_DStringAppendElement ( &dString, "static int AutoReorientImages(vtkImageData *orig_ref, int ref_orient, vtkImageData *targ, int trg_orient, vtkImageData *output);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for AutoReorientImages */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: PrintImageInfo */
    if ( strcmp ( argv[2], "PrintImageInfo" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "PrintImageInfo" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for PrintImageInfo */
    Tcl_DStringAppendElement ( &dString, " Debugging Aid \n" );
    Tcl_DStringAppendElement ( &dString, "static void PrintImageInfo(vtkImageData *tmp, const char *comment);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for PrintImageInfo */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FixPipelineSpacingAndOrigin */
    if ( strcmp ( argv[2], "FixPipelineSpacingAndOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FixPipelineSpacingAndOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FixPipelineSpacingAndOrigin */
    Tcl_DStringAppendElement ( &dString, " Fix Pipeline Spacing and Origin\n" );
    Tcl_DStringAppendElement ( &dString, "static void FixPipelineSpacingAndOrigin(vtkImageData *img);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRegistration" );
    /* Closing for FixPipelineSpacingAndOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkProcessObjectCppCommand(static_cast<vtkProcessObject *>(op),interp,argc,argv) == TCL_OK)
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
