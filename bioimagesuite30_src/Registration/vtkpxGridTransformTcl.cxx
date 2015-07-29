// tcl wrapper for vtkpxGridTransform object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxGridTransform.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxGridTransformNewCommand()
{
  vtkpxGridTransform *temp = vtkpxGridTransform::New();
  return static_cast<ClientData>(temp);
}

int vtkpxBaseGridTransformCppCommand(vtkpxBaseGridTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxGridTransformCppCommand(vtkpxGridTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxGridTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxGridTransformCppCommand(static_cast<vtkpxGridTransform *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxGridTransformCppCommand(vtkpxGridTransform *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxGridTransform",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxBaseGridTransformCppCommand(static_cast<vtkpxBaseGridTransform *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxBaseGridTransform"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxGridTransform  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxGridTransform");
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
    vtkpxGridTransform  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxGridTransform");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxGridTransform  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxGridTransform");
    return TCL_OK;
    }
    }
  if ((!strcmp("InitializeDisplacementGrid",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    double   temp1;
    double   temp2;
    vtkAbstractTransform  *temp3;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    temp3 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    op->InitializeDisplacementGrid(temp0,temp1,temp2,temp3);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetDisplacementGrid",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetDisplacementGrid(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("InitializeDisplacementGrid",argv[1]))&&(argc == 3))
    {
    vtkpxGridTransform  *temp0;
    error = 0;

    temp0 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxGridTransform"),interp,error));
    if (!error)
    {
    op->InitializeDisplacementGrid(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("InverseApproximateTransformation",argv[1]))&&(argc == 4))
    {
    vtkpxGridTransform  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxGridTransform"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->InverseApproximateTransformation(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetGridDimensions",argv[1]))&&(argc == 2))
    {
    int     *temp20;
    temp20 = (op)->GetGridDimensions();
    if(temp20)
      {
      char tempResult[1024];
      *tempResult = '\0';
      sprintf(tempResult,"%i %i %i ",temp20[0],temp20[1],temp20[2]);
      Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
      }
    else
      {
      Tcl_SetResult(interp, const_cast<char *>(""), TCL_VOLATILE);
      }
    return TCL_OK;
    }
  if ((!strcmp("GetGridSpacing",argv[1]))&&(argc == 2))
    {
    double  *temp20;
    temp20 = (op)->GetGridSpacing();
    if(temp20)
      {
      char tempResult[1024];
      *tempResult = '\0';
      char converted[1024];
      *converted = '\0';
      Tcl_PrintDouble(interp,temp20[0], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_PrintDouble(interp,temp20[1], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_PrintDouble(interp,temp20[2], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
      }
    else
      {
      Tcl_SetResult(interp, const_cast<char *>(""), TCL_VOLATILE);
      }
    return TCL_OK;
    }
  if ((!strcmp("GetGridOrigin",argv[1]))&&(argc == 2))
    {
    double  *temp20;
    temp20 = (op)->GetGridOrigin();
    if(temp20)
      {
      char tempResult[1024];
      *tempResult = '\0';
      char converted[1024];
      *converted = '\0';
      Tcl_PrintDouble(interp,temp20[0], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_PrintDouble(interp,temp20[1], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_PrintDouble(interp,temp20[2], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
      }
    else
      {
      Tcl_SetResult(interp, const_cast<char *>(""), TCL_VOLATILE);
      }
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfControlPoints",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfControlPoints();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetApproximationMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetApproximationMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetApproximationMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetApproximationMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetControlPointIndex",argv[1]))&&(argc == 5))
    {
    int      temp0;
    int      temp1;
    int      temp2;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->GetControlPointIndex(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetControlPointLocation",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->GetControlPointLocation(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetControlPointDisplacement",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->GetControlPointDisplacement(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetControlPointDisplacement",argv[1]))&&(argc == 5))
    {
    int      temp0;
    int      temp1;
    double   temp2;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->SetControlPointDisplacement(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ShiftControlPoint",argv[1]))&&(argc == 5))
    {
    int      temp0;
    int      temp1;
    double   temp2;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->ShiftControlPoint(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeDerivativeMatrix",argv[1]))&&(argc == 6))
    {
    double   temp0;
    double   temp1;
    double   temp2;
    vtkpxMatrix  *temp3;
    double   temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    temp3 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeDerivativeMatrix(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeDerivativeMatrix",argv[1]))&&(argc == 4))
    {
    int      temp0;
    vtkpxMatrix  *temp1;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeDerivativeMatrix(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeDerivative",argv[1]))&&(argc == 5))
    {
    int      temp0;
    int      temp1;
    int      temp2;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeDerivative(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeDerivative",argv[1]))&&(argc == 7))
    {
    double   temp0;
    double   temp1;
    double   temp2;
    int      temp3;
    int      temp4;
    double   temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeDerivative(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ApproximateTransformation",argv[1]))&&(argc == 3))
    {
    vtkAbstractTransform  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->ApproximateTransformation(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ApproximateTransformation",argv[1]))&&(argc == 8))
    {
    vtkAbstractTransform  *temp0;
    double   temp1;
    int      temp2;
    int      temp3;
    double   temp4;
    double   temp5;
    int      temp20;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (!error)
    {
    temp20 = (op)->ApproximateTransformation(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("InitialApproximateTransformation",argv[1]))&&(argc == 3))
    {
    vtkAbstractTransform  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->InitialApproximateTransformation(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateTestPoints",argv[1]))&&(argc == 4))
    {
    vtkPoints  *temp0;
    double   temp1;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    op->GenerateTestPoints(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("FillMatrices",argv[1]))&&(argc == 5))
    {
    vtkAbstractTransform  *temp0;
    vtkpxMatrix  *temp1;
    vtkpxMatrix  *temp2;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    temp2 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    op->FillMatrices(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("TestApproximation",argv[1]))&&(argc == 4))
    {
    vtkAbstractTransform  *temp0;
    vtkPoints  *temp1;
    double   temp20;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp1 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPoints"),interp,error));
    if (!error)
    {
    temp20 = (op)->TestApproximation(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("TestInverseApproximation",argv[1]))&&(argc == 4))
    {
    vtkAbstractTransform  *temp0;
    vtkPoints  *temp1;
    double   temp20;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp1 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPoints"),interp,error));
    if (!error)
    {
    temp20 = (op)->TestInverseApproximation(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("TestApproximation",argv[1]))&&(argc == 4))
    {
    vtkPoints  *temp0;
    vtkPoints  *temp1;
    double   temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    temp1 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPoints"),interp,error));
    if (!error)
    {
    temp20 = (op)->TestApproximation(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("TestTransformPoint",argv[1]))&&(argc == 5))
    {
    double   temp0;
    double   temp1;
    double   temp2;
    double   temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->TestTransformPoint(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeTotalBendingEnergy",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->ComputeTotalBendingEnergy();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ComputeBendingEnergyAroundControlPoint",argv[1]))&&(argc == 3))
    {
    int      temp0;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeBendingEnergyAroundControlPoint(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeBendingEnergyAtControlPoint",argv[1]))&&(argc == 3))
    {
    int      temp0;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeBendingEnergyAtControlPoint(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeBendingEnergyAtControlPoint",argv[1]))&&(argc == 5))
    {
    int      temp0;
    int      temp1;
    int      temp2;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeBendingEnergyAtControlPoint(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeTotalStrainEnergy",argv[1]))&&(argc == 3))
    {
    double   temp0;
    double   temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeTotalStrainEnergy(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeStrainEnergyAtControlPoint",argv[1]))&&(argc == 4))
    {
    int      temp0;
    double   temp1;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeStrainEnergyAtControlPoint(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CombineWithLinearTransform",argv[1]))&&(argc == 4))
    {
    vtkLinearTransform  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkLinearTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkLinearTransform"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->CombineWithLinearTransform(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Inverse",argv[1]))&&(argc == 2))
    {
    op->Inverse();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("FormWeightMatrix",argv[1]))&&(argc == 4))
    {
    vtkPoints  *temp0;
    vtkpxMatrix  *temp1;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    op->FormWeightMatrix(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("FormCollectionStructure",argv[1]))&&(argc == 5))
    {
    vtkPoints  *temp0;
    vtkCollection  *temp1;
    double   temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    temp1 = (vtkCollection *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkCollection"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->FormCollectionStructure(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FormCollectionStructure",argv[1]))&&(argc == 4))
    {
    vtkPoints  *temp0;
    vtkCollection  *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    temp1 = (vtkCollection *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkCollection"),interp,error));
    if (!error)
    {
    temp20 = (op)->FormCollectionStructure(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ApproximatePoints",argv[1]))&&(argc == 14))
    {
    vtkPoints  *temp0;
    vtkPoints  *temp1;
    vtkCollection  *temp2;
    double   temp3;
    int      temp4;
    int      temp5;
    double   temp6;
    double   temp7;
    int      temp8;
    vtkpxMatrix  *temp9;
    vtkpxGridTransform  *temp10;
    vtkDoubleArray  *temp11;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    temp1 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPoints"),interp,error));
    temp2 = (vtkCollection *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkCollection"),interp,error));
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (Tcl_GetDouble(interp,argv[8],&tempd) != TCL_OK) error = 1;
    temp6 = tempd;
    if (Tcl_GetDouble(interp,argv[9],&tempd) != TCL_OK) error = 1;
    temp7 = tempd;
    if (Tcl_GetInt(interp,argv[10],&tempi) != TCL_OK) error = 1;
    temp8 = tempi;
    temp9 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[11],const_cast<char *>("vtkpxMatrix"),interp,error));
    temp10 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[12],const_cast<char *>("vtkpxGridTransform"),interp,error));
    temp11 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[13],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->ApproximatePoints(temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,temp10,temp11);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ErrorAtControlPoint",argv[1]))&&(argc == 10))
    {
    int      temp0;
    vtkPoints  *temp1;
    vtkPoints  *temp2;
    vtkDoubleArray  *temp3;
    double   temp4;
    vtkpxMatrix  *temp5;
    vtkpxGridTransform  *temp6;
    vtkDoubleArray  *temp7;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    temp1 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPoints"),interp,error));
    temp2 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkPoints"),interp,error));
    temp3 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    temp5 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[7],const_cast<char *>("vtkpxMatrix"),interp,error));
    temp6 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[8],const_cast<char *>("vtkpxGridTransform"),interp,error));
    temp7 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[9],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->ErrorAtControlPoint(temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeWeightedDistance",argv[1]))&&(argc == 5))
    {
    vtkpxGridTransform  *temp0;
    int      temp1;
    vtkDoubleArray  *temp2;
    double   temp20;
    error = 0;

    temp0 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxGridTransform"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    temp2 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeWeightedDistance(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FormOutputWeights",argv[1]))&&(argc == 5))
    {
    vtkCollection  *temp0;
    vtkpxMatrix  *temp1;
    vtkDoubleArray  *temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkCollection *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkCollection"),interp,error));
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    temp2 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->FormOutputWeights(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ShiftControlPoints",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    double   temp1;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    op->ShiftControlPoints(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ScaleGradients",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    double   temp1;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    op->ScaleGradients(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("OptimizerFeedback",argv[1]))&&(argc == 5))
    {
    vtkObject  *temp0;
    char    *temp1;
    double   temp2;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    temp1 = argv[3];
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->OptimizerFeedback(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("CalculateGradient",argv[1]))&&(argc == 5))
    {
    vtkObject  *temp0;
    vtkDoubleArray  *temp1;
    vtkDoubleArray  *temp2;
    double   temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    temp1 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkDoubleArray"),interp,error));
    temp2 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->CalculateGradient(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CalculateFunction",argv[1]))&&(argc == 4))
    {
    vtkObject  *temp0;
    vtkDoubleArray  *temp1;
    double   temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    temp1 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->CalculateFunction(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetCurrentParameters",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->SetCurrentParameters(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCurrentParameters",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->GetCurrentParameters(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("OptimizerGradient",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    vtkDoubleArray  *temp1;
    double   temp20;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    temp1 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->OptimizerGradient(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("OptimizerValue",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    double   temp20;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->OptimizerValue(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SmoothCollection",argv[1]))&&(argc == 5))
    {
    vtkCollection  *temp0;
    int      temp1;
    double   temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkCollection *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkCollection"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->SmoothCollection(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeJacobianConstraint",argv[1]))&&(argc == 3))
    {
    float    temp0;
    double   temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeJacobianConstraint(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxGridTransformCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxBaseGridTransformCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxGridTransform:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  InitializeDisplacementGrid\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  SetDisplacementGrid\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  InitializeDisplacementGrid\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  InverseApproximateTransformation\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetGridDimensions\n",NULL);
    Tcl_AppendResult(interp,"  GetGridSpacing\n",NULL);
    Tcl_AppendResult(interp,"  GetGridOrigin\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfControlPoints\n",NULL);
    Tcl_AppendResult(interp,"  GetApproximationMode\n",NULL);
    Tcl_AppendResult(interp,"  SetApproximationMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetControlPointIndex\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetControlPointLocation\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetControlPointDisplacement\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetControlPointDisplacement\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ShiftControlPoint\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeDerivativeMatrix\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeDerivativeMatrix\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeDerivative\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeDerivative\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  ApproximateTransformation\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ApproximateTransformation\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  InitialApproximateTransformation\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GenerateTestPoints\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  FillMatrices\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  TestApproximation\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  TestInverseApproximation\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  TestApproximation\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  TestTransformPoint\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeTotalBendingEnergy\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBendingEnergyAroundControlPoint\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBendingEnergyAtControlPoint\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBendingEnergyAtControlPoint\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeTotalStrainEnergy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ComputeStrainEnergyAtControlPoint\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  CombineWithLinearTransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Inverse\n",NULL);
    Tcl_AppendResult(interp,"  FormWeightMatrix\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  FormCollectionStructure\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  FormCollectionStructure\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ApproximatePoints\t with 12 args\n",NULL);
    Tcl_AppendResult(interp,"  ErrorAtControlPoint\t with 8 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeWeightedDistance\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  FormOutputWeights\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ShiftControlPoints\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ScaleGradients\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerFeedback\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateGradient\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateFunction\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetCurrentParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCurrentParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerGradient\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SmoothCollection\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeJacobianConstraint\t with 1 arg\n",NULL);
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
    vtkpxBaseGridTransformCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "InitializeDisplacementGrid" );
    Tcl_DStringAppendElement ( &dString, "SetDisplacementGrid" );
    Tcl_DStringAppendElement ( &dString, "InitializeDisplacementGrid" );
    Tcl_DStringAppendElement ( &dString, "InverseApproximateTransformation" );
    Tcl_DStringAppendElement ( &dString, "GetGridDimensions" );
    Tcl_DStringAppendElement ( &dString, "GetGridSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetGridOrigin" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfControlPoints" );
    Tcl_DStringAppendElement ( &dString, "GetApproximationMode" );
    Tcl_DStringAppendElement ( &dString, "SetApproximationMode" );
    Tcl_DStringAppendElement ( &dString, "GetControlPointIndex" );
    Tcl_DStringAppendElement ( &dString, "GetControlPointLocation" );
    Tcl_DStringAppendElement ( &dString, "GetControlPointDisplacement" );
    Tcl_DStringAppendElement ( &dString, "SetControlPointDisplacement" );
    Tcl_DStringAppendElement ( &dString, "ShiftControlPoint" );
    Tcl_DStringAppendElement ( &dString, "ComputeDerivativeMatrix" );
    Tcl_DStringAppendElement ( &dString, "ComputeDerivativeMatrix" );
    Tcl_DStringAppendElement ( &dString, "ComputeDerivative" );
    Tcl_DStringAppendElement ( &dString, "ComputeDerivative" );
    Tcl_DStringAppendElement ( &dString, "ApproximateTransformation" );
    Tcl_DStringAppendElement ( &dString, "ApproximateTransformation" );
    Tcl_DStringAppendElement ( &dString, "InitialApproximateTransformation" );
    Tcl_DStringAppendElement ( &dString, "GenerateTestPoints" );
    Tcl_DStringAppendElement ( &dString, "FillMatrices" );
    Tcl_DStringAppendElement ( &dString, "TestApproximation" );
    Tcl_DStringAppendElement ( &dString, "TestInverseApproximation" );
    Tcl_DStringAppendElement ( &dString, "TestApproximation" );
    Tcl_DStringAppendElement ( &dString, "TestTransformPoint" );
    Tcl_DStringAppendElement ( &dString, "ComputeTotalBendingEnergy" );
    Tcl_DStringAppendElement ( &dString, "ComputeBendingEnergyAroundControlPoint" );
    Tcl_DStringAppendElement ( &dString, "ComputeBendingEnergyAtControlPoint" );
    Tcl_DStringAppendElement ( &dString, "ComputeBendingEnergyAtControlPoint" );
    Tcl_DStringAppendElement ( &dString, "ComputeTotalStrainEnergy" );
    Tcl_DStringAppendElement ( &dString, "ComputeStrainEnergyAtControlPoint" );
    Tcl_DStringAppendElement ( &dString, "CombineWithLinearTransform" );
    Tcl_DStringAppendElement ( &dString, "Inverse" );
    Tcl_DStringAppendElement ( &dString, "FormWeightMatrix" );
    Tcl_DStringAppendElement ( &dString, "FormCollectionStructure" );
    Tcl_DStringAppendElement ( &dString, "FormCollectionStructure" );
    Tcl_DStringAppendElement ( &dString, "ApproximatePoints" );
    Tcl_DStringAppendElement ( &dString, "ErrorAtControlPoint" );
    Tcl_DStringAppendElement ( &dString, "ComputeWeightedDistance" );
    Tcl_DStringAppendElement ( &dString, "FormOutputWeights" );
    Tcl_DStringAppendElement ( &dString, "ShiftControlPoints" );
    Tcl_DStringAppendElement ( &dString, "ScaleGradients" );
    Tcl_DStringAppendElement ( &dString, "OptimizerFeedback" );
    Tcl_DStringAppendElement ( &dString, "CalculateGradient" );
    Tcl_DStringAppendElement ( &dString, "CalculateFunction" );
    Tcl_DStringAppendElement ( &dString, "SetCurrentParameters" );
    Tcl_DStringAppendElement ( &dString, "GetCurrentParameters" );
    Tcl_DStringAppendElement ( &dString, "OptimizerGradient" );
    Tcl_DStringAppendElement ( &dString, "OptimizerValue" );
    Tcl_DStringAppendElement ( &dString, "SmoothCollection" );
    Tcl_DStringAppendElement ( &dString, "ComputeJacobianConstraint" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxBaseGridTransformCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxGridTransform *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InitializeDisplacementGrid */
    if ( strcmp ( argv[2], "InitializeDisplacementGrid" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InitializeDisplacementGrid" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InitializeDisplacementGrid */
    Tcl_DStringAppendElement ( &dString, " Initialize Displacement Grid Given Image\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void InitializeDisplacementGrid(vtkImageData *image, double spacing, double offset, vtkAbstractTransform *tr=NULL);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for InitializeDisplacementGrid */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDisplacementGrid */
    if ( strcmp ( argv[2], "SetDisplacementGrid" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDisplacementGrid" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDisplacementGrid */
    Tcl_DStringAppendElement ( &dString, " Set Displacement Grid -- used to create cached structures\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetDisplacementGrid(vtkImageData *grid);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for SetDisplacementGrid */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InitializeDisplacementGrid */
    if ( strcmp ( argv[2], "InitializeDisplacementGrid" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InitializeDisplacementGrid" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InitializeDisplacementGrid */
    Tcl_DStringAppendElement ( &dString, " Initialize the Displacement Grid to be the same as the other\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void InitializeDisplacementGrid(vtkpxGridTransform *other);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for InitializeDisplacementGrid */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InverseApproximateTransformation */
    if ( strcmp ( argv[2], "InverseApproximateTransformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InverseApproximateTransformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InverseApproximateTransformation */
    Tcl_DStringAppendElement ( &dString, " Initialize the Displacement Grid to be the same as the other\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void InverseApproximateTransformation(vtkpxGridTransform *output, int initialize=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for InverseApproximateTransformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGridDimensions */
    if ( strcmp ( argv[2], "GetGridDimensions" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGridDimensions" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGridDimensions */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int *GetGridDimensions();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for GetGridDimensions */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGridSpacing */
    if ( strcmp ( argv[2], "GetGridSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGridSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGridSpacing */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double *GetGridSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for GetGridSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGridOrigin */
    if ( strcmp ( argv[2], "GetGridOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGridOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGridOrigin */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double *GetGridOrigin();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for GetGridOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfControlPoints */
    if ( strcmp ( argv[2], "GetNumberOfControlPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfControlPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfControlPoints */
    Tcl_DStringAppendElement ( &dString, " NumberOfControlPoints\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfControlPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for GetNumberOfControlPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetApproximationMode */
    if ( strcmp ( argv[2], "GetApproximationMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetApproximationMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetApproximationMode */
    Tcl_DStringAppendElement ( &dString, " NumberOfControlPoints\n" );
    Tcl_DStringAppendElement ( &dString, "int GetApproximationMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for GetApproximationMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetApproximationMode */
    if ( strcmp ( argv[2], "SetApproximationMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetApproximationMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetApproximationMode */
    Tcl_DStringAppendElement ( &dString, " NumberOfControlPoints\n" );
    Tcl_DStringAppendElement ( &dString, "void SetApproximationMode(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for SetApproximationMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetControlPointIndex */
    if ( strcmp ( argv[2], "GetControlPointIndex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetControlPointIndex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetControlPointIndex */
    Tcl_DStringAppendElement ( &dString, " Get Index From Indices\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetControlPointIndex(int i, int j, int k);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for GetControlPointIndex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetControlPointLocation */
    if ( strcmp ( argv[2], "GetControlPointLocation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetControlPointLocation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetControlPointLocation */
    Tcl_DStringAppendElement ( &dString, " Shift Control Point and get control point location stuff\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double GetControlPointLocation(int index, int comp);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for GetControlPointLocation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetControlPointDisplacement */
    if ( strcmp ( argv[2], "GetControlPointDisplacement" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetControlPointDisplacement" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetControlPointDisplacement */
    Tcl_DStringAppendElement ( &dString, " Modify individual displacements\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double GetControlPointDisplacement(int index, int comp);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for GetControlPointDisplacement */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetControlPointDisplacement */
    if ( strcmp ( argv[2], "SetControlPointDisplacement" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetControlPointDisplacement" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetControlPointDisplacement */
    Tcl_DStringAppendElement ( &dString, " Modify individual displacements\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetControlPointDisplacement(int index, int comp, double val);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for SetControlPointDisplacement */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShiftControlPoint */
    if ( strcmp ( argv[2], "ShiftControlPoint" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShiftControlPoint" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShiftControlPoint */
    Tcl_DStringAppendElement ( &dString, " Shift Control Point\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void ShiftControlPoint(int index, int comp, double val);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ShiftControlPoint */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeDerivativeMatrix */
    if ( strcmp ( argv[2], "ComputeDerivativeMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeDerivativeMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeDerivativeMatrix */
    Tcl_DStringAppendElement ( &dString, " Compute Derivatives at a point, returns determinat (Jacobian)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeDerivativeMatrix(double x1, double x2, double x3, vtkpxMatrix *matr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeDerivativeMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeDerivativeMatrix */
    if ( strcmp ( argv[2], "ComputeDerivativeMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeDerivativeMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeDerivativeMatrix */
    Tcl_DStringAppendElement ( &dString, " Compute Derivatives at a point, returns determinat (Jacobian)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeDerivativeMatrix(int index, vtkpxMatrix *matr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeDerivativeMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeDerivative */
    if ( strcmp ( argv[2], "ComputeDerivative" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeDerivative" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeDerivative */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeDerivative(int index, int comp1, int comp2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeDerivative */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeDerivative */
    if ( strcmp ( argv[2], "ComputeDerivative" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeDerivative" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeDerivative */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeDerivative(double x1, double x2, double x3, int comp1, int comp2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeDerivative */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ApproximateTransformation */
    if ( strcmp ( argv[2], "ApproximateTransformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ApproximateTransformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ApproximateTransformation */
    Tcl_DStringAppendElement ( &dString, " Initialize Transformation using an arbitrary input transform\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ApproximateTransformation(vtkAbstractTransform *tr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ApproximateTransformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ApproximateTransformation */
    if ( strcmp ( argv[2], "ApproximateTransformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ApproximateTransformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ApproximateTransformation */
    Tcl_DStringAppendElement ( &dString, " Initialize Transformation using an arbitrary input transform\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ApproximateTransformation(vtkAbstractTransform *tr, double step, int numiter, int numlevels, double threshold, double lambda);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ApproximateTransformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InitialApproximateTransformation */
    if ( strcmp ( argv[2], "InitialApproximateTransformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InitialApproximateTransformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InitialApproximateTransformation */
    Tcl_DStringAppendElement ( &dString, " Initialize Transformation using an arbitrary input transform\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int InitialApproximateTransformation(vtkAbstractTransform *tr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for InitialApproximateTransformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateTestPoints */
    if ( strcmp ( argv[2], "GenerateTestPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateTestPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateTestPoints */
    Tcl_DStringAppendElement ( &dString, " Approximation Helper Routines\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void GenerateTestPoints(vtkPoints *points, double inc=1.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for GenerateTestPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FillMatrices */
    if ( strcmp ( argv[2], "FillMatrices" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FillMatrices" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FillMatrices */
    Tcl_DStringAppendElement ( &dString, " Approximation Helper Routines\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void FillMatrices(vtkAbstractTransform *tr, vtkpxMatrix *A, vtkpxMatrix *B);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for FillMatrices */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TestApproximation */
    if ( strcmp ( argv[2], "TestApproximation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TestApproximation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TestApproximation */
    Tcl_DStringAppendElement ( &dString, " Approximation Helper Routines\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double TestApproximation(vtkAbstractTransform *tr, vtkPoints *points);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for TestApproximation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TestInverseApproximation */
    if ( strcmp ( argv[2], "TestInverseApproximation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TestInverseApproximation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TestInverseApproximation */
    Tcl_DStringAppendElement ( &dString, " Approximation Helper Routines\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double TestInverseApproximation(vtkAbstractTransform *tr, vtkPoints *points);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for TestInverseApproximation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TestApproximation */
    if ( strcmp ( argv[2], "TestApproximation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TestApproximation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TestApproximation */
    Tcl_DStringAppendElement ( &dString, " Approximation Helper Routines\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double TestApproximation(vtkPoints *points, vtkPoints *points2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for TestApproximation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TestTransformPoint */
    if ( strcmp ( argv[2], "TestTransformPoint" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TestTransformPoint" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TestTransformPoint */
    Tcl_DStringAppendElement ( &dString, " Test of B-Spline\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double TestTransformPoint(double x, double y, double z);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for TestTransformPoint */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeTotalBendingEnergy */
    if ( strcmp ( argv[2], "ComputeTotalBendingEnergy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeTotalBendingEnergy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeTotalBendingEnergy */
    Tcl_DStringAppendElement ( &dString, " Compute Bending Energy (this is the one used internally)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeTotalBendingEnergy();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeTotalBendingEnergy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBendingEnergyAroundControlPoint */
    if ( strcmp ( argv[2], "ComputeBendingEnergyAroundControlPoint" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBendingEnergyAroundControlPoint" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBendingEnergyAroundControlPoint */
    Tcl_DStringAppendElement ( &dString, " Compute Bending Energy (this is the one used internally)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeBendingEnergyAroundControlPoint(int cpoint);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeBendingEnergyAroundControlPoint */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBendingEnergyAtControlPoint */
    if ( strcmp ( argv[2], "ComputeBendingEnergyAtControlPoint" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBendingEnergyAtControlPoint" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBendingEnergyAtControlPoint */
    Tcl_DStringAppendElement ( &dString, " Compute Bending Energy (this is the one used internally)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeBendingEnergyAtControlPoint(int cpoint);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeBendingEnergyAtControlPoint */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBendingEnergyAtControlPoint */
    if ( strcmp ( argv[2], "ComputeBendingEnergyAtControlPoint" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBendingEnergyAtControlPoint" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBendingEnergyAtControlPoint */
    Tcl_DStringAppendElement ( &dString, " Compute Bending Energy (this is the one used internally)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeBendingEnergyAtControlPoint(int i, int j, int k);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeBendingEnergyAtControlPoint */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeTotalStrainEnergy */
    if ( strcmp ( argv[2], "ComputeTotalStrainEnergy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeTotalStrainEnergy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeTotalStrainEnergy */
    Tcl_DStringAppendElement ( &dString, " Compute Strain Energy (Linear Elasticity)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeTotalStrainEnergy(double v=0.45);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeTotalStrainEnergy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeStrainEnergyAtControlPoint */
    if ( strcmp ( argv[2], "ComputeStrainEnergyAtControlPoint" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeStrainEnergyAtControlPoint" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeStrainEnergyAtControlPoint */
    Tcl_DStringAppendElement ( &dString, " Compute Strain Energy (Linear Elasticity)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeStrainEnergyAtControlPoint(int cpoint, double v=0.45);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeStrainEnergyAtControlPoint */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CombineWithLinearTransform */
    if ( strcmp ( argv[2], "CombineWithLinearTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CombineWithLinearTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkLinearTransform" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CombineWithLinearTransform */
    Tcl_DStringAppendElement ( &dString, " CombineWithLinear Transform\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void CombineWithLinearTransform(vtkLinearTransform *tr, int premultiply=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for CombineWithLinearTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Inverse */
    if ( strcmp ( argv[2], "Inverse" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Inverse" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Inverse */
    Tcl_DStringAppendElement ( &dString, " Invert the transformation.  This is done by point fitting\n" );
    Tcl_DStringAppendElement ( &dString, "void Inverse();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for Inverse */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FormWeightMatrix */
    if ( strcmp ( argv[2], "FormWeightMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FormWeightMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FormWeightMatrix */
    Tcl_DStringAppendElement ( &dString, " Form Weight Matrix \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void FormWeightMatrix(vtkPoints *points, vtkpxMatrix *W);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for FormWeightMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FormCollectionStructure */
    if ( strcmp ( argv[2], "FormCollectionStructure" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FormCollectionStructure" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkCollection" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FormCollectionStructure */
    Tcl_DStringAppendElement ( &dString, " Form Weight Structures\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int FormCollectionStructure(vtkPoints *points, vtkCollection *ControlsCollection, double thr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for FormCollectionStructure */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FormCollectionStructure */
    if ( strcmp ( argv[2], "FormCollectionStructure" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FormCollectionStructure" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkCollection" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FormCollectionStructure */
    Tcl_DStringAppendElement ( &dString, " Form Weight Structures\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int FormCollectionStructure(vtkPoints *points, vtkCollection *ControlsCollection);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for FormCollectionStructure */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ApproximatePoints */
    if ( strcmp ( argv[2], "ApproximatePoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ApproximatePoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkCollection" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ApproximatePoints */
    Tcl_DStringAppendElement ( &dString, " Form Weight Structures\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void ApproximatePoints(vtkPoints *points, vtkPoints *points2, vtkCollection *ControlsCollection, double step, int numiter, int numlevels, double threshold, double lambda, int initialize, vtkpxMatrix *controlweights=NULL, vtkpxGridTransform *priorTransform=NULL, vtkDoubleArray *priorWeights=NULL);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ApproximatePoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ErrorAtControlPoint */
    if ( strcmp ( argv[2], "ErrorAtControlPoint" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ErrorAtControlPoint" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ErrorAtControlPoint */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual double ErrorAtControlPoint(int cp, vtkPoints *points, vtkPoints *points2, vtkDoubleArray *indices, double lambda, vtkpxMatrix *lsqweight=NULL, vtkpxGridTransform *priorTransform=NULL, vtkDoubleArray *priorWeights=NULL);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ErrorAtControlPoint */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeWeightedDistance */
    if ( strcmp ( argv[2], "ComputeWeightedDistance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeWeightedDistance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeWeightedDistance */
    Tcl_DStringAppendElement ( &dString, " Compute Weighted Distance\n -------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeWeightedDistance(vtkpxGridTransform *other, int cp, vtkDoubleArray *weights=NULL);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeWeightedDistance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FormOutputWeights */
    if ( strcmp ( argv[2], "FormOutputWeights" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FormOutputWeights" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkCollection" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FormOutputWeights */
    Tcl_DStringAppendElement ( &dString, " Compute Weighted Distance\n -------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int FormOutputWeights(vtkCollection *collection, vtkpxMatrix *PointWeights, vtkDoubleArray *OutputWeights);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for FormOutputWeights */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShiftControlPoints */
    if ( strcmp ( argv[2], "ShiftControlPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShiftControlPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShiftControlPoints */
    Tcl_DStringAppendElement ( &dString, " Shift Control Points by step*values in grad\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void ShiftControlPoints(vtkDoubleArray *grad, double step);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ShiftControlPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScaleGradients */
    if ( strcmp ( argv[2], "ScaleGradients" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScaleGradients" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScaleGradients */
    Tcl_DStringAppendElement ( &dString, " Shift Control Points by step*values in grad\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void ScaleGradients(vtkDoubleArray *grad, double scale);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ScaleGradients */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OptimizerFeedback */
    if ( strcmp ( argv[2], "OptimizerFeedback" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OptimizerFeedback" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OptimizerFeedback */
    Tcl_DStringAppendElement ( &dString, " Stuff Needed for Using vtkpxOptimizer to improve approximation of new points\n -----------------------------------------------------------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "static void OptimizerFeedback(vtkObject *self, const char *line, double val);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for OptimizerFeedback */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CalculateGradient */
    if ( strcmp ( argv[2], "CalculateGradient" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CalculateGradient" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CalculateGradient */
    Tcl_DStringAppendElement ( &dString, " Stuff Needed for Using vtkpxOptimizer to improve approximation of new points\n -----------------------------------------------------------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "static double CalculateGradient(vtkObject *self, vtkDoubleArray *position, vtkDoubleArray *output);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for CalculateGradient */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CalculateFunction */
    if ( strcmp ( argv[2], "CalculateFunction" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CalculateFunction" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CalculateFunction */
    Tcl_DStringAppendElement ( &dString, " Stuff Needed for Using vtkpxOptimizer to improve approximation of new points\n -----------------------------------------------------------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "static double CalculateFunction(vtkObject *self, vtkDoubleArray *position);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for CalculateFunction */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCurrentParameters */
    if ( strcmp ( argv[2], "SetCurrentParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCurrentParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCurrentParameters */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetCurrentParameters(vtkDoubleArray *par);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for SetCurrentParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCurrentParameters */
    if ( strcmp ( argv[2], "GetCurrentParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCurrentParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCurrentParameters */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void GetCurrentParameters(vtkDoubleArray *par);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for GetCurrentParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OptimizerGradient */
    if ( strcmp ( argv[2], "OptimizerGradient" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OptimizerGradient" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OptimizerGradient */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual double OptimizerGradient(vtkDoubleArray *params, vtkDoubleArray *grad);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for OptimizerGradient */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OptimizerValue */
    if ( strcmp ( argv[2], "OptimizerValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OptimizerValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OptimizerValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual double OptimizerValue(vtkDoubleArray *params);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for OptimizerValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SmoothCollection */
    if ( strcmp ( argv[2], "SmoothCollection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SmoothCollection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkCollection" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SmoothCollection */
    Tcl_DStringAppendElement ( &dString, " Smoothing of Collections to yield new transformations\n -----------------------------------------------------------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "static int SmoothCollection(vtkCollection *TransformationCollection, int periodic=0, double sigma=1.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for SmoothCollection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeJacobianConstraint */
    if ( strcmp ( argv[2], "ComputeJacobianConstraint" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeJacobianConstraint" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeJacobianConstraint */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeJacobianConstraint(float gamma);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    /* Closing for ComputeJacobianConstraint */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxBaseGridTransformCppCommand(static_cast<vtkpxBaseGridTransform *>(op),interp,argc,argv) == TCL_OK)
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
