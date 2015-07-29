// tcl wrapper for vtkpxImageTensorSpline object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxImageTensorSpline.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxImageTensorSplineNewCommand()
{
  vtkpxImageTensorSpline *temp = vtkpxImageTensorSpline::New();
  return static_cast<ClientData>(temp);
}

int vtkpxTensorBSplineTransformCppCommand(vtkpxTensorBSplineTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxImageTensorSplineCppCommand(vtkpxImageTensorSpline *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxImageTensorSplineCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxImageTensorSplineCppCommand(static_cast<vtkpxImageTensorSpline *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxImageTensorSplineCppCommand(vtkpxImageTensorSpline *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxImageTensorSpline",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxTensorBSplineTransformCppCommand(static_cast<vtkpxTensorBSplineTransform *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxTensorBSplineTransform"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxImageTensorSpline  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxImageTensorSpline");
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
    vtkpxImageTensorSpline  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxImageTensorSpline");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxImageTensorSpline  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxImageTensorSpline");
    return TCL_OK;
    }
    }
  if ((!strcmp("Run",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->Run();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ApproximateImage",argv[1]))&&(argc == 8))
    {
    vtkImageData  *temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    float    temp4;
    float    temp5;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (!error)
    {
    temp20 = (op)->ApproximateImage(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ApproximateFieldMap",argv[1]))&&(argc == 4))
    {
    double   temp0;
    int      temp1;
    int      temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->ApproximateFieldMap(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutput",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetOutput();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetInput",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetInput();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetInput",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetInput(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetWeightImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetWeightImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetWeightImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetWeightImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaskImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetMaskImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetMaskImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetMaskImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLambda",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLambda();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetLambda",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLambda(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSigma",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetSigma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSigma",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetSigma(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetStepSize",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetStepSize(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetStepSize",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetStepSize();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetControlPointSpacing",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetControlPointSpacing(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetControlPointSpacing",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetControlPointSpacing();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetResolution",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetResolution(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetResolution",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetResolution();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPreSmoothIterations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetPreSmoothIterations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPreSmoothIterations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPreSmoothIterations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetAutoInitialize",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetAutoInitialize(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAutoInitializeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAutoInitializeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAutoInitializeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAutoInitializeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAutoInitialize",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAutoInitialize();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetIterations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetIterations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetIterationsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetIterationsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetIterations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetEpsilon",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetEpsilon(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetEpsilonMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetEpsilonMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetEpsilonMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetEpsilonMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetEpsilon",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetEpsilon();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
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

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxImageTensorSplineCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxTensorBSplineTransformCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxImageTensorSpline:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Run\n",NULL);
    Tcl_AppendResult(interp,"  ApproximateImage\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  ApproximateFieldMap\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetOutput\n",NULL);
    Tcl_AppendResult(interp,"  GetInput\n",NULL);
    Tcl_AppendResult(interp,"  SetInput\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetWeightImage\n",NULL);
    Tcl_AppendResult(interp,"  SetWeightImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaskImage\n",NULL);
    Tcl_AppendResult(interp,"  SetMaskImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLambda\n",NULL);
    Tcl_AppendResult(interp,"  SetLambda\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSigma\n",NULL);
    Tcl_AppendResult(interp,"  SetSigma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetStepSize\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetStepSize\n",NULL);
    Tcl_AppendResult(interp,"  SetControlPointSpacing\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetControlPointSpacing\n",NULL);
    Tcl_AppendResult(interp,"  SetResolution\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetResolution\n",NULL);
    Tcl_AppendResult(interp,"  SetPreSmoothIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPreSmoothIterations\n",NULL);
    Tcl_AppendResult(interp,"  SetMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMode\n",NULL);
    Tcl_AppendResult(interp,"  SetAutoInitialize\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAutoInitializeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAutoInitializeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAutoInitialize\n",NULL);
    Tcl_AppendResult(interp,"  SetIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIterations\n",NULL);
    Tcl_AppendResult(interp,"  SetEpsilon\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetEpsilonMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetEpsilonMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetEpsilon\n",NULL);
    Tcl_AppendResult(interp,"  SetCurrentParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCurrentParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerGradient\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerFeedback\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateGradient\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateFunction\t with 2 args\n",NULL);
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
    vtkpxTensorBSplineTransformCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "Run" );
    Tcl_DStringAppendElement ( &dString, "ApproximateImage" );
    Tcl_DStringAppendElement ( &dString, "ApproximateFieldMap" );
    Tcl_DStringAppendElement ( &dString, "GetOutput" );
    Tcl_DStringAppendElement ( &dString, "GetInput" );
    Tcl_DStringAppendElement ( &dString, "SetInput" );
    Tcl_DStringAppendElement ( &dString, "GetWeightImage" );
    Tcl_DStringAppendElement ( &dString, "SetWeightImage" );
    Tcl_DStringAppendElement ( &dString, "GetMaskImage" );
    Tcl_DStringAppendElement ( &dString, "SetMaskImage" );
    Tcl_DStringAppendElement ( &dString, "GetLambda" );
    Tcl_DStringAppendElement ( &dString, "SetLambda" );
    Tcl_DStringAppendElement ( &dString, "GetSigma" );
    Tcl_DStringAppendElement ( &dString, "SetSigma" );
    Tcl_DStringAppendElement ( &dString, "SetStepSize" );
    Tcl_DStringAppendElement ( &dString, "GetStepSize" );
    Tcl_DStringAppendElement ( &dString, "SetControlPointSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetControlPointSpacing" );
    Tcl_DStringAppendElement ( &dString, "SetResolution" );
    Tcl_DStringAppendElement ( &dString, "GetResolution" );
    Tcl_DStringAppendElement ( &dString, "SetPreSmoothIterations" );
    Tcl_DStringAppendElement ( &dString, "GetPreSmoothIterations" );
    Tcl_DStringAppendElement ( &dString, "SetMode" );
    Tcl_DStringAppendElement ( &dString, "GetModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMode" );
    Tcl_DStringAppendElement ( &dString, "SetAutoInitialize" );
    Tcl_DStringAppendElement ( &dString, "GetAutoInitializeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetAutoInitializeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetAutoInitialize" );
    Tcl_DStringAppendElement ( &dString, "SetIterations" );
    Tcl_DStringAppendElement ( &dString, "GetIterationsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterationsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterations" );
    Tcl_DStringAppendElement ( &dString, "SetEpsilon" );
    Tcl_DStringAppendElement ( &dString, "GetEpsilonMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetEpsilonMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetEpsilon" );
    Tcl_DStringAppendElement ( &dString, "SetCurrentParameters" );
    Tcl_DStringAppendElement ( &dString, "GetCurrentParameters" );
    Tcl_DStringAppendElement ( &dString, "OptimizerGradient" );
    Tcl_DStringAppendElement ( &dString, "OptimizerValue" );
    Tcl_DStringAppendElement ( &dString, "OptimizerFeedback" );
    Tcl_DStringAppendElement ( &dString, "CalculateGradient" );
    Tcl_DStringAppendElement ( &dString, "CalculateFunction" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxTensorBSplineTransformCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxImageTensorSpline *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Run */
    if ( strcmp ( argv[2], "Run" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Run" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Run */
    Tcl_DStringAppendElement ( &dString, " Approximate Image Value\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Run();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for Run */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ApproximateImage */
    if ( strcmp ( argv[2], "ApproximateImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ApproximateImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ApproximateImage */
    Tcl_DStringAppendElement ( &dString, " Approximate Image Value\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ApproximateImage(vtkImageData *img, float resolution, float spacing, float offest, float smoothness, float initial);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for ApproximateImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ApproximateFieldMap */
    if ( strcmp ( argv[2], "ApproximateFieldMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ApproximateFieldMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ApproximateFieldMap */
    Tcl_DStringAppendElement ( &dString, " Approximate Image Value\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ApproximateFieldMap(double scalefactor, int phasedirection=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for ApproximateFieldMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutput */
    if ( strcmp ( argv[2], "GetOutput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutput */
    Tcl_DStringAppendElement ( &dString, " Output Image\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetOutput();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetOutput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInput */
    if ( strcmp ( argv[2], "GetInput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInput */
    Tcl_DStringAppendElement ( &dString, " Image To be Approximated\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetInput();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetInput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInput */
    if ( strcmp ( argv[2], "SetInput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInput */
    Tcl_DStringAppendElement ( &dString, " Image To be Approximated\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInput(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetInput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWeightImage */
    if ( strcmp ( argv[2], "GetWeightImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWeightImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWeightImage */
    Tcl_DStringAppendElement ( &dString, " Weight Image -- auto generate if Mode = 1\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetWeightImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetWeightImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetWeightImage */
    if ( strcmp ( argv[2], "SetWeightImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetWeightImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetWeightImage */
    Tcl_DStringAppendElement ( &dString, " Weight Image -- auto generate if Mode = 1\n" );
    Tcl_DStringAppendElement ( &dString, "void SetWeightImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetWeightImage */

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
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaskImage */
    Tcl_DStringAppendElement ( &dString, " Mask Image\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetMaskImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetMaskImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaskImage */
    if ( strcmp ( argv[2], "SetMaskImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaskImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaskImage */
    Tcl_DStringAppendElement ( &dString, " Mask Image\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaskImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetMaskImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLambda */
    if ( strcmp ( argv[2], "GetLambda" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLambda" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLambda */
    Tcl_DStringAppendElement ( &dString, " Lambda is smoothness constant (default=0.0)\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLambda();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetLambda */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLambda */
    if ( strcmp ( argv[2], "SetLambda" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLambda" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLambda */
    Tcl_DStringAppendElement ( &dString, " Lambda is smoothness constant (default=0.0)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetLambda(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetLambda */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSigma */
    if ( strcmp ( argv[2], "GetSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSigma */
    Tcl_DStringAppendElement ( &dString, " Sigma is image space smoothness (default =0.0)\n" );
    Tcl_DStringAppendElement ( &dString, "double GetSigma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSigma */
    if ( strcmp ( argv[2], "SetSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSigma */
    Tcl_DStringAppendElement ( &dString, " Sigma is image space smoothness (default =0.0)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSigma(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetStepSize */
    if ( strcmp ( argv[2], "SetStepSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetStepSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetStepSize */
    Tcl_DStringAppendElement ( &dString, " Step Size for Gradient Computation\n" );
    Tcl_DStringAppendElement ( &dString, "void SetStepSize(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetStepSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetStepSize */
    if ( strcmp ( argv[2], "GetStepSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetStepSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetStepSize */
    Tcl_DStringAppendElement ( &dString, " Step Size for Gradient Computation\n" );
    Tcl_DStringAppendElement ( &dString, "double GetStepSize();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetStepSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetControlPointSpacing */
    if ( strcmp ( argv[2], "SetControlPointSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetControlPointSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetControlPointSpacing */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "void SetControlPointSpacing(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetControlPointSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetControlPointSpacing */
    if ( strcmp ( argv[2], "GetControlPointSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetControlPointSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetControlPointSpacing */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "double GetControlPointSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetControlPointSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetResolution */
    if ( strcmp ( argv[2], "SetResolution" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetResolution" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetResolution */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "void SetResolution(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetResolution */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetResolution */
    if ( strcmp ( argv[2], "GetResolution" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetResolution" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetResolution */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "double GetResolution();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetResolution */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPreSmoothIterations */
    if ( strcmp ( argv[2], "SetPreSmoothIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPreSmoothIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPreSmoothIterations */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPreSmoothIterations(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetPreSmoothIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPreSmoothIterations */
    if ( strcmp ( argv[2], "GetPreSmoothIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPreSmoothIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPreSmoothIterations */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPreSmoothIterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetPreSmoothIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMode */
    if ( strcmp ( argv[2], "SetMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMode */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetModeMinValue */
    if ( strcmp ( argv[2], "GetModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "int GetModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetModeMaxValue */
    if ( strcmp ( argv[2], "GetModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "int GetModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMode */
    if ( strcmp ( argv[2], "GetMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMode */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAutoInitialize */
    if ( strcmp ( argv[2], "SetAutoInitialize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAutoInitialize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAutoInitialize */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "void SetAutoInitialize(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetAutoInitialize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAutoInitializeMinValue */
    if ( strcmp ( argv[2], "GetAutoInitializeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAutoInitializeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAutoInitializeMinValue */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAutoInitializeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetAutoInitializeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAutoInitializeMaxValue */
    if ( strcmp ( argv[2], "GetAutoInitializeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAutoInitializeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAutoInitializeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAutoInitializeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetAutoInitializeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAutoInitialize */
    if ( strcmp ( argv[2], "GetAutoInitialize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAutoInitialize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAutoInitialize */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAutoInitialize();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetAutoInitialize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetIterations */
    if ( strcmp ( argv[2], "SetIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetIterations */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "void SetIterations(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationsMinValue */
    if ( strcmp ( argv[2], "GetIterationsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationsMinValue */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetIterationsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationsMaxValue */
    if ( strcmp ( argv[2], "GetIterationsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetIterationsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterations */
    if ( strcmp ( argv[2], "GetIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterations */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetEpsilon */
    if ( strcmp ( argv[2], "SetEpsilon" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetEpsilon" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetEpsilon */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "void SetEpsilon(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for SetEpsilon */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetEpsilonMinValue */
    if ( strcmp ( argv[2], "GetEpsilonMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetEpsilonMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetEpsilonMinValue */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "double GetEpsilonMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetEpsilonMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetEpsilonMaxValue */
    if ( strcmp ( argv[2], "GetEpsilonMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetEpsilonMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetEpsilonMaxValue */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "double GetEpsilonMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetEpsilonMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetEpsilon */
    if ( strcmp ( argv[2], "GetEpsilon" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetEpsilon" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetEpsilon */
    Tcl_DStringAppendElement ( &dString, " Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "double GetEpsilon();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for GetEpsilon */

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
    Tcl_DStringAppendElement ( &dString, " Needed for Optimization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetCurrentParameters(vtkDoubleArray *par);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
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
    Tcl_DStringAppendElement ( &dString, " Needed for Optimization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void GetCurrentParameters(vtkDoubleArray *par);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
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
    Tcl_DStringAppendElement ( &dString, " Needed for Optimization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double OptimizerGradient(vtkDoubleArray *params, vtkDoubleArray *grad);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
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
    Tcl_DStringAppendElement ( &dString, " Needed for Optimization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double OptimizerValue(vtkDoubleArray *params);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for OptimizerValue */

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
    Tcl_DStringAppendElement ( &dString, " Needed for Optimization\n" );
    Tcl_DStringAppendElement ( &dString, "static void OptimizerFeedback(vtkObject *self, const char *line, double val);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
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
    Tcl_DStringAppendElement ( &dString, " Needed for Optimization\n" );
    Tcl_DStringAppendElement ( &dString, "static double CalculateGradient(vtkObject *self, vtkDoubleArray *position, vtkDoubleArray *output);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
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
    Tcl_DStringAppendElement ( &dString, " Needed for Optimization\n" );
    Tcl_DStringAppendElement ( &dString, "static double CalculateFunction(vtkObject *self, vtkDoubleArray *position);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageTensorSpline" );
    /* Closing for CalculateFunction */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxTensorBSplineTransformCppCommand(static_cast<vtkpxTensorBSplineTransform *>(op),interp,argc,argv) == TCL_OK)
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
