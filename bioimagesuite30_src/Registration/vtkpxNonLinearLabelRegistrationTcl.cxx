// tcl wrapper for vtkpxNonLinearLabelRegistration object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxNonLinearLabelRegistration.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxNonLinearLabelRegistrationNewCommand()
{
  vtkpxNonLinearLabelRegistration *temp = vtkpxNonLinearLabelRegistration::New();
  return static_cast<ClientData>(temp);
}

int vtkpxAbstractIntensityRegistrationCppCommand(vtkpxAbstractIntensityRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxNonLinearLabelRegistrationCppCommand(vtkpxNonLinearLabelRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxNonLinearLabelRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxNonLinearLabelRegistrationCppCommand(static_cast<vtkpxNonLinearLabelRegistration *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxNonLinearLabelRegistrationCppCommand(vtkpxNonLinearLabelRegistration *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxNonLinearLabelRegistration",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxAbstractIntensityRegistrationCppCommand(static_cast<vtkpxAbstractIntensityRegistration *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxAbstractIntensityRegistration"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxNonLinearLabelRegistration  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxNonLinearLabelRegistration");
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
    vtkpxNonLinearLabelRegistration  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxNonLinearLabelRegistration");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxNonLinearLabelRegistration  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxNonLinearLabelRegistration");
    return TCL_OK;
    }
    }
  if ((!strcmp("PrintInformation",argv[1]))&&(argc == 2))
    {
    op->PrintInformation();
    Tcl_ResetResult(interp);
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
  if ((!strcmp("SetControlPointSpacingRate",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetControlPointSpacingRate(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetControlPointSpacingRateMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetControlPointSpacingRateMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetControlPointSpacingRateMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetControlPointSpacingRateMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetControlPointSpacingRate",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetControlPointSpacingRate();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ComputeRegistration",argv[1]))&&(argc == 3))
    {
    int      temp0;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeRegistration(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetReferenceImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->SetReferenceImage(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetTransformImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->SetTransformImage(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetComboTransform",argv[1]))&&(argc == 2))
    {
    vtkpxComboTransform  *temp20;
    temp20 = (op)->GetComboTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxComboTransform");
    return TCL_OK;
    }
  if ((!strcmp("SetInitialTransform",argv[1]))&&(argc == 3))
    {
    vtkAbstractTransform  *temp0;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    op->SetInitialTransform(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInitialTransform",argv[1]))&&(argc == 2))
    {
    vtkAbstractTransform  *temp20;
    temp20 = (op)->GetInitialTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
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
  if ((!strcmp("GetWindowSize",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetWindowSize();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetWindowSize",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetWindowSize(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetWindowSizeMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetWindowSizeMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetWindowSizeMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetWindowSizeMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetGradientScale",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetGradientScale();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetGradientScale",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetGradientScale(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetGradientScaleMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetGradientScaleMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetGradientScaleMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetGradientScaleMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCombinationMetric",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCombinationMetric();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCombinationMetric",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetCombinationMetric(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCombinationMetricMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCombinationMetricMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCombinationMetricMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCombinationMetricMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUpdateTransform",argv[1]))&&(argc == 2))
    {
    vtkAbstractTransform  *temp20;
    temp20 = (op)->GetUpdateTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
    return TCL_OK;
    }
  if ((!strcmp("GetTransformation",argv[1]))&&(argc == 2))
    {
    vtkAbstractTransform  *temp20;
    temp20 = (op)->GetTransformation();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
    return TCL_OK;
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
  if ((!strcmp("SetInternalEnergyMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetInternalEnergyMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInternalEnergyModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInternalEnergyModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInternalEnergyModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInternalEnergyModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInternalEnergyMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInternalEnergyMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetInternalEnergyModeToBendingEnergy",argv[1]))&&(argc == 2))
    {
    op->SetInternalEnergyModeToBendingEnergy();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetInternalEnergyModeToLinearElastic",argv[1]))&&(argc == 2))
    {
    op->SetInternalEnergyModeToLinearElastic();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetAppendTransformations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetAppendTransformations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAppendTransformationsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAppendTransformationsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAppendTransformationsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAppendTransformationsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAppendTransformations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAppendTransformations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxNonLinearLabelRegistrationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxAbstractIntensityRegistrationCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxNonLinearLabelRegistration:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  PrintInformation\n",NULL);
    Tcl_AppendResult(interp,"  SetControlPointSpacing\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetControlPointSpacing\n",NULL);
    Tcl_AppendResult(interp,"  SetControlPointSpacingRate\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetControlPointSpacingRateMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetControlPointSpacingRateMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetControlPointSpacingRate\n",NULL);
    Tcl_AppendResult(interp,"  ComputeRegistration\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetReferenceImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetComboTransform\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialTransform\n",NULL);
    Tcl_AppendResult(interp,"  GetEpsilon\n",NULL);
    Tcl_AppendResult(interp,"  SetEpsilon\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetEpsilonMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetEpsilonMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetWindowSize\n",NULL);
    Tcl_AppendResult(interp,"  SetWindowSize\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetWindowSizeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetWindowSizeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetGradientScale\n",NULL);
    Tcl_AppendResult(interp,"  SetGradientScale\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetGradientScaleMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetGradientScaleMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCombinationMetric\n",NULL);
    Tcl_AppendResult(interp,"  SetCombinationMetric\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCombinationMetricMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCombinationMetricMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUpdateTransform\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformation\n",NULL);
    Tcl_AppendResult(interp,"  GetLambda\n",NULL);
    Tcl_AppendResult(interp,"  SetLambda\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetInternalEnergyMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInternalEnergyModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInternalEnergyModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInternalEnergyMode\n",NULL);
    Tcl_AppendResult(interp,"  SetInternalEnergyModeToBendingEnergy\n",NULL);
    Tcl_AppendResult(interp,"  SetInternalEnergyModeToLinearElastic\n",NULL);
    Tcl_AppendResult(interp,"  SetAppendTransformations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAppendTransformationsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAppendTransformationsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAppendTransformations\n",NULL);
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
    vtkpxAbstractIntensityRegistrationCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "PrintInformation" );
    Tcl_DStringAppendElement ( &dString, "SetControlPointSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetControlPointSpacing" );
    Tcl_DStringAppendElement ( &dString, "SetControlPointSpacingRate" );
    Tcl_DStringAppendElement ( &dString, "GetControlPointSpacingRateMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetControlPointSpacingRateMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetControlPointSpacingRate" );
    Tcl_DStringAppendElement ( &dString, "ComputeRegistration" );
    Tcl_DStringAppendElement ( &dString, "SetReferenceImage" );
    Tcl_DStringAppendElement ( &dString, "SetTransformImage" );
    Tcl_DStringAppendElement ( &dString, "GetComboTransform" );
    Tcl_DStringAppendElement ( &dString, "SetInitialTransform" );
    Tcl_DStringAppendElement ( &dString, "GetInitialTransform" );
    Tcl_DStringAppendElement ( &dString, "GetEpsilon" );
    Tcl_DStringAppendElement ( &dString, "SetEpsilon" );
    Tcl_DStringAppendElement ( &dString, "GetEpsilonMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetEpsilonMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetWindowSize" );
    Tcl_DStringAppendElement ( &dString, "SetWindowSize" );
    Tcl_DStringAppendElement ( &dString, "GetWindowSizeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetWindowSizeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetGradientScale" );
    Tcl_DStringAppendElement ( &dString, "SetGradientScale" );
    Tcl_DStringAppendElement ( &dString, "GetGradientScaleMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetGradientScaleMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetCombinationMetric" );
    Tcl_DStringAppendElement ( &dString, "SetCombinationMetric" );
    Tcl_DStringAppendElement ( &dString, "GetCombinationMetricMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetCombinationMetricMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUpdateTransform" );
    Tcl_DStringAppendElement ( &dString, "GetTransformation" );
    Tcl_DStringAppendElement ( &dString, "GetLambda" );
    Tcl_DStringAppendElement ( &dString, "SetLambda" );
    Tcl_DStringAppendElement ( &dString, "SetInternalEnergyMode" );
    Tcl_DStringAppendElement ( &dString, "GetInternalEnergyModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetInternalEnergyModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetInternalEnergyMode" );
    Tcl_DStringAppendElement ( &dString, "SetInternalEnergyModeToBendingEnergy" );
    Tcl_DStringAppendElement ( &dString, "SetInternalEnergyModeToLinearElastic" );
    Tcl_DStringAppendElement ( &dString, "SetAppendTransformations" );
    Tcl_DStringAppendElement ( &dString, "GetAppendTransformationsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetAppendTransformationsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetAppendTransformations" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxAbstractIntensityRegistrationCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxNonLinearLabelRegistration *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: PrintInformation */
    if ( strcmp ( argv[2], "PrintInformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "PrintInformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for PrintInformation */
    Tcl_DStringAppendElement ( &dString, " Print information about the progress of the registration\n" );
    Tcl_DStringAppendElement ( &dString, "void PrintInformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for PrintInformation */

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
    Tcl_DStringAppendElement ( &dString, " Set Grid Spacing at highest resolution\n" );
    Tcl_DStringAppendElement ( &dString, "void SetControlPointSpacing(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
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
    Tcl_DStringAppendElement ( &dString, " Set Grid Spacing at highest resolution\n" );
    Tcl_DStringAppendElement ( &dString, "double GetControlPointSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetControlPointSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetControlPointSpacingRate */
    if ( strcmp ( argv[2], "SetControlPointSpacingRate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetControlPointSpacingRate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetControlPointSpacingRate */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetControlPointSpacingRate(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetControlPointSpacingRate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetControlPointSpacingRateMinValue */
    if ( strcmp ( argv[2], "GetControlPointSpacingRateMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetControlPointSpacingRateMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetControlPointSpacingRateMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetControlPointSpacingRateMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetControlPointSpacingRateMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetControlPointSpacingRateMaxValue */
    if ( strcmp ( argv[2], "GetControlPointSpacingRateMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetControlPointSpacingRateMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetControlPointSpacingRateMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetControlPointSpacingRateMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetControlPointSpacingRateMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetControlPointSpacingRate */
    if ( strcmp ( argv[2], "GetControlPointSpacingRate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetControlPointSpacingRate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetControlPointSpacingRate */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetControlPointSpacingRate();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetControlPointSpacingRate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeRegistration */
    if ( strcmp ( argv[2], "ComputeRegistration" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeRegistration" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeRegistration */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int ComputeRegistration(int level);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for ComputeRegistration */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetReferenceImage */
    if ( strcmp ( argv[2], "SetReferenceImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetReferenceImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetReferenceImage */
    Tcl_DStringAppendElement ( &dString, " Sets input for the registration filter -- frame is ignored in ref image, all frames are used\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetReferenceImage(vtkImageData *ref, int frame);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetReferenceImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformImage */
    if ( strcmp ( argv[2], "SetTransformImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformImage */
    Tcl_DStringAppendElement ( &dString, " Sets input for the registration filter -- frame is ignored in ref image, all frames are used\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformImage(vtkImageData *ref, int frame);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetTransformImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetComboTransform */
    if ( strcmp ( argv[2], "GetComboTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetComboTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetComboTransform */
    Tcl_DStringAppendElement ( &dString, " Get Internal Transformation Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform *GetComboTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetComboTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialTransform */
    if ( strcmp ( argv[2], "SetInitialTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialTransform */
    Tcl_DStringAppendElement ( &dString, " Get/Set Initial Transform -- typically an affine estimate\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialTransform(vtkAbstractTransform *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetInitialTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialTransform */
    if ( strcmp ( argv[2], "GetInitialTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialTransform */
    Tcl_DStringAppendElement ( &dString, " Get/Set Initial Transform -- typically an affine estimate\n" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform *GetInitialTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetInitialTransform */

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
    Tcl_DStringAppendElement ( &dString, " Epsilon is the convergence limit (default=0.0001);\n" );
    Tcl_DStringAppendElement ( &dString, "double GetEpsilon();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetEpsilon */

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
    Tcl_DStringAppendElement ( &dString, " Epsilon is the convergence limit (default=0.0001);\n" );
    Tcl_DStringAppendElement ( &dString, "void SetEpsilon(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
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
    Tcl_DStringAppendElement ( &dString, " Epsilon is the convergence limit (default=0.0001);\n" );
    Tcl_DStringAppendElement ( &dString, "double GetEpsilonMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
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
    Tcl_DStringAppendElement ( &dString, " Epsilon is the convergence limit (default=0.0001);\n" );
    Tcl_DStringAppendElement ( &dString, "double GetEpsilonMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetEpsilonMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWindowSize */
    if ( strcmp ( argv[2], "GetWindowSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWindowSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWindowSize */
    Tcl_DStringAppendElement ( &dString, " WindowSize is the window size for updating the transformation (full=2.0)\n" );
    Tcl_DStringAppendElement ( &dString, "double GetWindowSize();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetWindowSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetWindowSize */
    if ( strcmp ( argv[2], "SetWindowSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetWindowSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetWindowSize */
    Tcl_DStringAppendElement ( &dString, " WindowSize is the window size for updating the transformation (full=2.0)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetWindowSize(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetWindowSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWindowSizeMinValue */
    if ( strcmp ( argv[2], "GetWindowSizeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWindowSizeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWindowSizeMinValue */
    Tcl_DStringAppendElement ( &dString, " WindowSize is the window size for updating the transformation (full=2.0)\n" );
    Tcl_DStringAppendElement ( &dString, "double GetWindowSizeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetWindowSizeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWindowSizeMaxValue */
    if ( strcmp ( argv[2], "GetWindowSizeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWindowSizeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWindowSizeMaxValue */
    Tcl_DStringAppendElement ( &dString, " WindowSize is the window size for updating the transformation (full=2.0)\n" );
    Tcl_DStringAppendElement ( &dString, "double GetWindowSizeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetWindowSizeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGradientScale */
    if ( strcmp ( argv[2], "GetGradientScale" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGradientScale" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGradientScale */
    Tcl_DStringAppendElement ( &dString, " WindowSize is the window size for updating the transformation (full=2.0)\n" );
    Tcl_DStringAppendElement ( &dString, "double GetGradientScale();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetGradientScale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetGradientScale */
    if ( strcmp ( argv[2], "SetGradientScale" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetGradientScale" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetGradientScale */
    Tcl_DStringAppendElement ( &dString, " WindowSize is the window size for updating the transformation (full=2.0)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetGradientScale(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetGradientScale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGradientScaleMinValue */
    if ( strcmp ( argv[2], "GetGradientScaleMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGradientScaleMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGradientScaleMinValue */
    Tcl_DStringAppendElement ( &dString, " WindowSize is the window size for updating the transformation (full=2.0)\n" );
    Tcl_DStringAppendElement ( &dString, "double GetGradientScaleMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetGradientScaleMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGradientScaleMaxValue */
    if ( strcmp ( argv[2], "GetGradientScaleMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGradientScaleMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGradientScaleMaxValue */
    Tcl_DStringAppendElement ( &dString, " WindowSize is the window size for updating the transformation (full=2.0)\n" );
    Tcl_DStringAppendElement ( &dString, "double GetGradientScaleMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetGradientScaleMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCombinationMetric */
    if ( strcmp ( argv[2], "GetCombinationMetric" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCombinationMetric" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCombinationMetric */
    Tcl_DStringAppendElement ( &dString, " 0 = Mean, 1=Median, 2 = Minimum\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCombinationMetric();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetCombinationMetric */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCombinationMetric */
    if ( strcmp ( argv[2], "SetCombinationMetric" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCombinationMetric" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCombinationMetric */
    Tcl_DStringAppendElement ( &dString, " 0 = Mean, 1=Median, 2 = Minimum\n" );
    Tcl_DStringAppendElement ( &dString, "void SetCombinationMetric(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetCombinationMetric */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCombinationMetricMinValue */
    if ( strcmp ( argv[2], "GetCombinationMetricMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCombinationMetricMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCombinationMetricMinValue */
    Tcl_DStringAppendElement ( &dString, " 0 = Mean, 1=Median, 2 = Minimum\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCombinationMetricMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetCombinationMetricMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCombinationMetricMaxValue */
    if ( strcmp ( argv[2], "GetCombinationMetricMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCombinationMetricMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCombinationMetricMaxValue */
    Tcl_DStringAppendElement ( &dString, " 0 = Mean, 1=Median, 2 = Minimum\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCombinationMetricMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetCombinationMetricMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUpdateTransform */
    if ( strcmp ( argv[2], "GetUpdateTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUpdateTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUpdateTransform */
    Tcl_DStringAppendElement ( &dString, " Get Update Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkAbstractTransform *GetUpdateTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetUpdateTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransformation */
    if ( strcmp ( argv[2], "GetTransformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransformation */
    Tcl_DStringAppendElement ( &dString, " GetTransformation\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkAbstractTransform *GetTransformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetTransformation */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetLambda */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInternalEnergyMode */
    if ( strcmp ( argv[2], "SetInternalEnergyMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInternalEnergyMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInternalEnergyMode */
    Tcl_DStringAppendElement ( &dString, " Prior Mode\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInternalEnergyMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetInternalEnergyMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInternalEnergyModeMinValue */
    if ( strcmp ( argv[2], "GetInternalEnergyModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInternalEnergyModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInternalEnergyModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Prior Mode\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInternalEnergyModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetInternalEnergyModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInternalEnergyModeMaxValue */
    if ( strcmp ( argv[2], "GetInternalEnergyModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInternalEnergyModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInternalEnergyModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Prior Mode\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInternalEnergyModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetInternalEnergyModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInternalEnergyMode */
    if ( strcmp ( argv[2], "GetInternalEnergyMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInternalEnergyMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInternalEnergyMode */
    Tcl_DStringAppendElement ( &dString, " Prior Mode\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInternalEnergyMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetInternalEnergyMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInternalEnergyModeToBendingEnergy */
    if ( strcmp ( argv[2], "SetInternalEnergyModeToBendingEnergy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInternalEnergyModeToBendingEnergy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInternalEnergyModeToBendingEnergy */
    Tcl_DStringAppendElement ( &dString, " Utility Functions for Setting the Smoothness Mode\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetInternalEnergyModeToBendingEnergy();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetInternalEnergyModeToBendingEnergy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInternalEnergyModeToLinearElastic */
    if ( strcmp ( argv[2], "SetInternalEnergyModeToLinearElastic" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInternalEnergyModeToLinearElastic" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInternalEnergyModeToLinearElastic */
    Tcl_DStringAppendElement ( &dString, " Utility Functions for Setting the Smoothness Mode\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetInternalEnergyModeToLinearElastic();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetInternalEnergyModeToLinearElastic */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAppendTransformations */
    if ( strcmp ( argv[2], "SetAppendTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAppendTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAppendTransformations */
    Tcl_DStringAppendElement ( &dString, " Append Transformations or Fit to Single\n" );
    Tcl_DStringAppendElement ( &dString, "void SetAppendTransformations(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for SetAppendTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAppendTransformationsMinValue */
    if ( strcmp ( argv[2], "GetAppendTransformationsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAppendTransformationsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAppendTransformationsMinValue */
    Tcl_DStringAppendElement ( &dString, " Append Transformations or Fit to Single\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAppendTransformationsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetAppendTransformationsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAppendTransformationsMaxValue */
    if ( strcmp ( argv[2], "GetAppendTransformationsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAppendTransformationsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAppendTransformationsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Append Transformations or Fit to Single\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAppendTransformationsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetAppendTransformationsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAppendTransformations */
    if ( strcmp ( argv[2], "GetAppendTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAppendTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAppendTransformations */
    Tcl_DStringAppendElement ( &dString, " Append Transformations or Fit to Single\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAppendTransformations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearLabelRegistration" );
    /* Closing for GetAppendTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxAbstractIntensityRegistrationCppCommand(static_cast<vtkpxAbstractIntensityRegistration *>(op),interp,argc,argv) == TCL_OK)
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
