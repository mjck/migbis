// tcl wrapper for vtkpxAbstractIntensityRegistration object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxAbstractIntensityRegistration.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxAbstractIntensityRegistrationNewCommand()
{
  vtkpxAbstractIntensityRegistration *temp = vtkpxAbstractIntensityRegistration::New();
  return static_cast<ClientData>(temp);
}

int vtkProcessObjectCppCommand(vtkProcessObject *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxAbstractIntensityRegistrationCppCommand(vtkpxAbstractIntensityRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxAbstractIntensityRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxAbstractIntensityRegistrationCppCommand(static_cast<vtkpxAbstractIntensityRegistration *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxAbstractIntensityRegistrationCppCommand(vtkpxAbstractIntensityRegistration *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxAbstractIntensityRegistration",argv[1]))
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
    vtkpxAbstractIntensityRegistration  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxAbstractIntensityRegistration");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxAbstractIntensityRegistration  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxAbstractIntensityRegistration");
    return TCL_OK;
    }
    }
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxAbstractIntensityRegistration  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxAbstractIntensityRegistration");
    return TCL_OK;
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
  if ((!strcmp("SetReferenceImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetReferenceImage(temp0);
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
  if ((!strcmp("SetTransformImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetTransformImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetReferenceWeightImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetReferenceWeightImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetReferenceWeightImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetReferenceWeightImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetTransformWeightImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetTransformWeightImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTransformWeightImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetTransformWeightImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetReferenceOrientation",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetReferenceOrientation();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetReferenceOrientation",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetReferenceOrientation(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetReferenceOrientationMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetReferenceOrientationMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetReferenceOrientationMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetReferenceOrientationMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTransformOrientation",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTransformOrientation();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformOrientation",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetTransformOrientation(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTransformOrientationMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTransformOrientationMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTransformOrientationMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTransformOrientationMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetProvideFeedback",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetProvideFeedback(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetProvideFeedback",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetProvideFeedback();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ProvideFeedbackOn",argv[1]))&&(argc == 2))
    {
    op->ProvideFeedbackOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ProvideFeedbackOff",argv[1]))&&(argc == 2))
    {
    op->ProvideFeedbackOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetLogarithmMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetLogarithmMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLogarithmModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetLogarithmModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLogarithmModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetLogarithmModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLogarithmMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetLogarithmMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("LogarithmModeOn",argv[1]))&&(argc == 2))
    {
    op->LogarithmModeOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("LogarithmModeOff",argv[1]))&&(argc == 2))
    {
    op->LogarithmModeOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetInterpolationMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetInterpolationMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInterpolationModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInterpolationModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInterpolationModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInterpolationModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInterpolationMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInterpolationMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPositiveIntensitiesOnly",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetPositiveIntensitiesOnly(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPositiveIntensitiesOnlyMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPositiveIntensitiesOnlyMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPositiveIntensitiesOnlyMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPositiveIntensitiesOnlyMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPositiveIntensitiesOnly",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPositiveIntensitiesOnly();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("PositiveIntensitiesOnlyOn",argv[1]))&&(argc == 2))
    {
    op->PositiveIntensitiesOnlyOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("PositiveIntensitiesOnlyOff",argv[1]))&&(argc == 2))
    {
    op->PositiveIntensitiesOnlyOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetAutoNormalizeIntensities",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetAutoNormalizeIntensities(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAutoNormalizeIntensitiesMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAutoNormalizeIntensitiesMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAutoNormalizeIntensitiesMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAutoNormalizeIntensitiesMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAutoNormalizeIntensities",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAutoNormalizeIntensities();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("AutoNormalizeIntensitiesOn",argv[1]))&&(argc == 2))
    {
    op->AutoNormalizeIntensitiesOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("AutoNormalizeIntensitiesOff",argv[1]))&&(argc == 2))
    {
    op->AutoNormalizeIntensitiesOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetJointlyNormalizeIntensities",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetJointlyNormalizeIntensities(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetJointlyNormalizeIntensitiesMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetJointlyNormalizeIntensitiesMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetJointlyNormalizeIntensitiesMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetJointlyNormalizeIntensitiesMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetJointlyNormalizeIntensities",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetJointlyNormalizeIntensities();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("JointlyNormalizeIntensitiesOn",argv[1]))&&(argc == 2))
    {
    op->JointlyNormalizeIntensitiesOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("JointlyNormalizeIntensitiesOff",argv[1]))&&(argc == 2))
    {
    op->JointlyNormalizeIntensitiesOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetHistogram",argv[1]))&&(argc == 3))
    {
    vtkpxJointHistogram  *temp0;
    error = 0;

    temp0 = (vtkpxJointHistogram *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxJointHistogram"),interp,error));
    if (!error)
    {
    op->SetHistogram(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHistogram",argv[1]))&&(argc == 2))
    {
    vtkpxJointHistogram  *temp20;
    temp20 = (op)->GetHistogram();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxJointHistogram");
    return TCL_OK;
    }
  if ((!strcmp("GetTransformation",argv[1]))&&(argc == 2))
    {
    vtkAbstractTransform  *temp20;
    temp20 = (op)->GetTransformation();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
    return TCL_OK;
    }
  if ((!strcmp("SetNumberOfLevels",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNumberOfLevels(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfLevels",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfLevels();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNumberOfExtraLevels",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNumberOfExtraLevels(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfExtraLevels",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfExtraLevels();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNumberOfBins",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNumberOfBins(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfBins",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfBins();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNumberOfIterations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNumberOfIterations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfIterations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfIterations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNumberOfSteps",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNumberOfSteps(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfSteps",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfSteps();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
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
  if ((!strcmp("SetBlurFactor",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetBlurFactor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBlurFactorMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetBlurFactorMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBlurFactorMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetBlurFactorMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBlurFactor",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetBlurFactor();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
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
  if ((!strcmp("GetSimilarityMeasure",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSimilarityMeasure();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSimilarityMeasure",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetSimilarityMeasure(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetSimilarityMeasureToJointEntropy",argv[1]))&&(argc == 2))
    {
    op->SetSimilarityMeasureToJointEntropy();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSimilarityMeasureToCorrelation",argv[1]))&&(argc == 2))
    {
    op->SetSimilarityMeasureToCorrelation();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSimilarityMeasureToGradientCorrelation",argv[1]))&&(argc == 2))
    {
    op->SetSimilarityMeasureToGradientCorrelation();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSimilarityMeasureToMutualInformation",argv[1]))&&(argc == 2))
    {
    op->SetSimilarityMeasureToMutualInformation();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSimilarityMeasureToNormalizedMutualInformation",argv[1]))&&(argc == 2))
    {
    op->SetSimilarityMeasureToNormalizedMutualInformation();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSimilarityMeasureToSumofSquaredDifferences",argv[1]))&&(argc == 2))
    {
    op->SetSimilarityMeasureToSumofSquaredDifferences();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSimilarityMeasureToCorrelationRatioYX",argv[1]))&&(argc == 2))
    {
    op->SetSimilarityMeasureToCorrelationRatioYX();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSimilarityMeasureToCorrelationRatioXY",argv[1]))&&(argc == 2))
    {
    op->SetSimilarityMeasureToCorrelationRatioXY();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSimilarityMeasureToProduct",argv[1]))&&(argc == 2))
    {
    op->SetSimilarityMeasureToProduct();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSimilarityMeasureToDifferenceEntropy",argv[1]))&&(argc == 2))
    {
    op->SetSimilarityMeasureToDifferenceEntropy();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("Run",argv[1]))&&(argc == 2))
    {
    op->Run();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("NameOfMeasure",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->NameOfMeasure();
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
  if ((!strcmp("PrintInformation",argv[1]))&&(argc == 2))
    {
    op->PrintInformation();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetOutputStatus",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetOutputStatus();
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
  if ((!strcmp("GetUpdateTransformation",argv[1]))&&(argc == 2))
    {
    vtkAbstractTransform  *temp20;
    temp20 = (op)->GetUpdateTransformation();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
    return TCL_OK;
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
  if ((!strcmp("OnBeginIteration",argv[1]))&&(argc == 4))
    {
    vtkObject  *temp0;
    vtkDoubleArray  *temp1;
    double   temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    temp1 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->OnBeginIteration(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetOptimizationMethod",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetOptimizationMethod(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOptimizationMethod",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetOptimizationMethod();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationMethodToOldStyle",argv[1]))&&(argc == 2))
    {
    op->SetOptimizationMethodToOldStyle();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationMethodToDefault",argv[1]))&&(argc == 2))
    {
    op->SetOptimizationMethodToDefault();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationMethodToSlowClimb",argv[1]))&&(argc == 2))
    {
    op->SetOptimizationMethodToSlowClimb();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationMethodToPowell",argv[1]))&&(argc == 2))
    {
    op->SetOptimizationMethodToPowell();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationMethodToGradientDescent",argv[1]))&&(argc == 2))
    {
    op->SetOptimizationMethodToGradientDescent();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationMethodToConjugateGradient",argv[1]))&&(argc == 2))
    {
    op->SetOptimizationMethodToConjugateGradient();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationMethodToHybrid",argv[1]))&&(argc == 2))
    {
    op->SetOptimizationMethodToHybrid();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetLastSimilarity",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLastSimilarity();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLastSmoothness",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLastSmoothness();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetResolutionRate",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetResolutionRate(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetResolutionRateMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetResolutionRateMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetResolutionRateMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetResolutionRateMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetResolutionRate",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetResolutionRate();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxAbstractIntensityRegistrationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkProcessObjectCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxAbstractIntensityRegistration:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  SetReferenceImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetReferenceImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetReferenceWeightImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetReferenceWeightImage\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformWeightImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformWeightImage\n",NULL);
    Tcl_AppendResult(interp,"  GetReferenceOrientation\n",NULL);
    Tcl_AppendResult(interp,"  SetReferenceOrientation\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetReferenceOrientationMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetReferenceOrientationMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformOrientation\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformOrientation\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformOrientationMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformOrientationMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetProvideFeedback\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetProvideFeedback\n",NULL);
    Tcl_AppendResult(interp,"  ProvideFeedbackOn\n",NULL);
    Tcl_AppendResult(interp,"  ProvideFeedbackOff\n",NULL);
    Tcl_AppendResult(interp,"  SetLogarithmMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLogarithmModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLogarithmModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLogarithmMode\n",NULL);
    Tcl_AppendResult(interp,"  LogarithmModeOn\n",NULL);
    Tcl_AppendResult(interp,"  LogarithmModeOff\n",NULL);
    Tcl_AppendResult(interp,"  SetInterpolationMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolationModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolationModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolationMode\n",NULL);
    Tcl_AppendResult(interp,"  SetPositiveIntensitiesOnly\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPositiveIntensitiesOnlyMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPositiveIntensitiesOnlyMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPositiveIntensitiesOnly\n",NULL);
    Tcl_AppendResult(interp,"  PositiveIntensitiesOnlyOn\n",NULL);
    Tcl_AppendResult(interp,"  PositiveIntensitiesOnlyOff\n",NULL);
    Tcl_AppendResult(interp,"  SetAutoNormalizeIntensities\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAutoNormalizeIntensitiesMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAutoNormalizeIntensitiesMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAutoNormalizeIntensities\n",NULL);
    Tcl_AppendResult(interp,"  AutoNormalizeIntensitiesOn\n",NULL);
    Tcl_AppendResult(interp,"  AutoNormalizeIntensitiesOff\n",NULL);
    Tcl_AppendResult(interp,"  SetJointlyNormalizeIntensities\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetJointlyNormalizeIntensitiesMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetJointlyNormalizeIntensitiesMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetJointlyNormalizeIntensities\n",NULL);
    Tcl_AppendResult(interp,"  JointlyNormalizeIntensitiesOn\n",NULL);
    Tcl_AppendResult(interp,"  JointlyNormalizeIntensitiesOff\n",NULL);
    Tcl_AppendResult(interp,"  SetHistogram\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHistogram\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformation\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfLevels\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfLevels\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfExtraLevels\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfExtraLevels\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfBins\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBins\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfIterations\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfSteps\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfSteps\n",NULL);
    Tcl_AppendResult(interp,"  SetResolution\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetResolution\n",NULL);
    Tcl_AppendResult(interp,"  SetBlurFactor\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBlurFactorMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBlurFactorMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBlurFactor\n",NULL);
    Tcl_AppendResult(interp,"  SetStepSize\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetStepSize\n",NULL);
    Tcl_AppendResult(interp,"  GetSimilarityMeasure\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasure\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasureToJointEntropy\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasureToCorrelation\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasureToGradientCorrelation\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasureToMutualInformation\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasureToNormalizedMutualInformation\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasureToSumofSquaredDifferences\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasureToCorrelationRatioYX\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasureToCorrelationRatioXY\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasureToProduct\n",NULL);
    Tcl_AppendResult(interp,"  SetSimilarityMeasureToDifferenceEntropy\n",NULL);
    Tcl_AppendResult(interp,"  Run\n",NULL);
    Tcl_AppendResult(interp,"  NameOfMeasure\n",NULL);
    Tcl_AppendResult(interp,"  PrintInformation\n",NULL);
    Tcl_AppendResult(interp,"  GetOutputStatus\n",NULL);
    Tcl_AppendResult(interp,"  GetUpdateTransformation\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerFeedback\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateGradient\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateFunction\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  OnBeginIteration\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethod\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOptimizationMethod\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethodToOldStyle\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethodToDefault\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethodToSlowClimb\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethodToPowell\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethodToGradientDescent\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethodToConjugateGradient\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationMethodToHybrid\n",NULL);
    Tcl_AppendResult(interp,"  GetLastSimilarity\n",NULL);
    Tcl_AppendResult(interp,"  GetLastSmoothness\n",NULL);
    Tcl_AppendResult(interp,"  SetResolutionRate\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetResolutionRateMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetResolutionRateMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetResolutionRate\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetReferenceImage" );
    Tcl_DStringAppendElement ( &dString, "SetReferenceImage" );
    Tcl_DStringAppendElement ( &dString, "SetTransformImage" );
    Tcl_DStringAppendElement ( &dString, "SetTransformImage" );
    Tcl_DStringAppendElement ( &dString, "SetReferenceWeightImage" );
    Tcl_DStringAppendElement ( &dString, "GetReferenceWeightImage" );
    Tcl_DStringAppendElement ( &dString, "SetTransformWeightImage" );
    Tcl_DStringAppendElement ( &dString, "GetTransformWeightImage" );
    Tcl_DStringAppendElement ( &dString, "GetReferenceOrientation" );
    Tcl_DStringAppendElement ( &dString, "SetReferenceOrientation" );
    Tcl_DStringAppendElement ( &dString, "GetReferenceOrientationMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetReferenceOrientationMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetTransformOrientation" );
    Tcl_DStringAppendElement ( &dString, "SetTransformOrientation" );
    Tcl_DStringAppendElement ( &dString, "GetTransformOrientationMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetTransformOrientationMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetProvideFeedback" );
    Tcl_DStringAppendElement ( &dString, "GetProvideFeedback" );
    Tcl_DStringAppendElement ( &dString, "ProvideFeedbackOn" );
    Tcl_DStringAppendElement ( &dString, "ProvideFeedbackOff" );
    Tcl_DStringAppendElement ( &dString, "SetLogarithmMode" );
    Tcl_DStringAppendElement ( &dString, "GetLogarithmModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetLogarithmModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetLogarithmMode" );
    Tcl_DStringAppendElement ( &dString, "LogarithmModeOn" );
    Tcl_DStringAppendElement ( &dString, "LogarithmModeOff" );
    Tcl_DStringAppendElement ( &dString, "SetInterpolationMode" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolationModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolationModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolationMode" );
    Tcl_DStringAppendElement ( &dString, "SetPositiveIntensitiesOnly" );
    Tcl_DStringAppendElement ( &dString, "GetPositiveIntensitiesOnlyMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetPositiveIntensitiesOnlyMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetPositiveIntensitiesOnly" );
    Tcl_DStringAppendElement ( &dString, "PositiveIntensitiesOnlyOn" );
    Tcl_DStringAppendElement ( &dString, "PositiveIntensitiesOnlyOff" );
    Tcl_DStringAppendElement ( &dString, "SetAutoNormalizeIntensities" );
    Tcl_DStringAppendElement ( &dString, "GetAutoNormalizeIntensitiesMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetAutoNormalizeIntensitiesMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetAutoNormalizeIntensities" );
    Tcl_DStringAppendElement ( &dString, "AutoNormalizeIntensitiesOn" );
    Tcl_DStringAppendElement ( &dString, "AutoNormalizeIntensitiesOff" );
    Tcl_DStringAppendElement ( &dString, "SetJointlyNormalizeIntensities" );
    Tcl_DStringAppendElement ( &dString, "GetJointlyNormalizeIntensitiesMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetJointlyNormalizeIntensitiesMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetJointlyNormalizeIntensities" );
    Tcl_DStringAppendElement ( &dString, "JointlyNormalizeIntensitiesOn" );
    Tcl_DStringAppendElement ( &dString, "JointlyNormalizeIntensitiesOff" );
    Tcl_DStringAppendElement ( &dString, "SetHistogram" );
    Tcl_DStringAppendElement ( &dString, "GetHistogram" );
    Tcl_DStringAppendElement ( &dString, "GetTransformation" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfLevels" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfLevels" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfExtraLevels" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfExtraLevels" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfBins" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBins" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfIterations" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfIterations" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfSteps" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfSteps" );
    Tcl_DStringAppendElement ( &dString, "SetResolution" );
    Tcl_DStringAppendElement ( &dString, "GetResolution" );
    Tcl_DStringAppendElement ( &dString, "SetBlurFactor" );
    Tcl_DStringAppendElement ( &dString, "GetBlurFactorMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetBlurFactorMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetBlurFactor" );
    Tcl_DStringAppendElement ( &dString, "SetStepSize" );
    Tcl_DStringAppendElement ( &dString, "GetStepSize" );
    Tcl_DStringAppendElement ( &dString, "GetSimilarityMeasure" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasure" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToJointEntropy" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToCorrelation" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToGradientCorrelation" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToMutualInformation" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToNormalizedMutualInformation" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToSumofSquaredDifferences" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToCorrelationRatioYX" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToCorrelationRatioXY" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToProduct" );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToDifferenceEntropy" );
    Tcl_DStringAppendElement ( &dString, "Run" );
    Tcl_DStringAppendElement ( &dString, "NameOfMeasure" );
    Tcl_DStringAppendElement ( &dString, "PrintInformation" );
    Tcl_DStringAppendElement ( &dString, "GetOutputStatus" );
    Tcl_DStringAppendElement ( &dString, "GetUpdateTransformation" );
    Tcl_DStringAppendElement ( &dString, "OptimizerFeedback" );
    Tcl_DStringAppendElement ( &dString, "CalculateGradient" );
    Tcl_DStringAppendElement ( &dString, "CalculateFunction" );
    Tcl_DStringAppendElement ( &dString, "OnBeginIteration" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethod" );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationMethod" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToOldStyle" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToDefault" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToSlowClimb" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToPowell" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToGradientDescent" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToConjugateGradient" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToHybrid" );
    Tcl_DStringAppendElement ( &dString, "GetLastSimilarity" );
    Tcl_DStringAppendElement ( &dString, "GetLastSmoothness" );
    Tcl_DStringAppendElement ( &dString, "SetResolutionRate" );
    Tcl_DStringAppendElement ( &dString, "GetResolutionRateMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetResolutionRateMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetResolutionRate" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxAbstractIntensityRegistration *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for New */

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
    Tcl_DStringAppendElement ( &dString, " Sets input for the registration filter\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetReferenceImage(vtkImageData *targ, int frame);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetReferenceImage */

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
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetReferenceImage */
    Tcl_DStringAppendElement ( &dString, " Sets input for the registration filter\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetReferenceImage(vtkImageData *targ);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformImage(vtkImageData *src, int frame);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetTransformImage */

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
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformImage(vtkImageData *src);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetTransformImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetReferenceWeightImage */
    if ( strcmp ( argv[2], "SetReferenceWeightImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetReferenceWeightImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetReferenceWeightImage */
    Tcl_DStringAppendElement ( &dString, " Set Reference and Target Weights (Target Weights unused mostly)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetReferenceWeightImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetReferenceWeightImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetReferenceWeightImage */
    if ( strcmp ( argv[2], "GetReferenceWeightImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetReferenceWeightImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetReferenceWeightImage */
    Tcl_DStringAppendElement ( &dString, " Set Reference and Target Weights (Target Weights unused mostly)\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetReferenceWeightImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetReferenceWeightImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformWeightImage */
    if ( strcmp ( argv[2], "SetTransformWeightImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformWeightImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformWeightImage */
    Tcl_DStringAppendElement ( &dString, " Set Reference and Target Weights (Target Weights unused mostly)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTransformWeightImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetTransformWeightImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransformWeightImage */
    if ( strcmp ( argv[2], "GetTransformWeightImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransformWeightImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransformWeightImage */
    Tcl_DStringAppendElement ( &dString, " Set Reference and Target Weights (Target Weights unused mostly)\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetTransformWeightImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetTransformWeightImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetReferenceOrientation */
    if ( strcmp ( argv[2], "GetReferenceOrientation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetReferenceOrientation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetReferenceOrientation */
    Tcl_DStringAppendElement ( &dString, " Set Orientations if needed\n" );
    Tcl_DStringAppendElement ( &dString, "int GetReferenceOrientation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetReferenceOrientation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetReferenceOrientation */
    if ( strcmp ( argv[2], "SetReferenceOrientation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetReferenceOrientation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetReferenceOrientation */
    Tcl_DStringAppendElement ( &dString, " Set Orientations if needed\n" );
    Tcl_DStringAppendElement ( &dString, "void SetReferenceOrientation(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetReferenceOrientation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetReferenceOrientationMinValue */
    if ( strcmp ( argv[2], "GetReferenceOrientationMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetReferenceOrientationMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetReferenceOrientationMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Orientations if needed\n" );
    Tcl_DStringAppendElement ( &dString, "int GetReferenceOrientationMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetReferenceOrientationMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetReferenceOrientationMaxValue */
    if ( strcmp ( argv[2], "GetReferenceOrientationMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetReferenceOrientationMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetReferenceOrientationMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Orientations if needed\n" );
    Tcl_DStringAppendElement ( &dString, "int GetReferenceOrientationMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetReferenceOrientationMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransformOrientation */
    if ( strcmp ( argv[2], "GetTransformOrientation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransformOrientation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransformOrientation */
    Tcl_DStringAppendElement ( &dString, " Set Orientations if needed\n" );
    Tcl_DStringAppendElement ( &dString, "int GetTransformOrientation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetTransformOrientation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformOrientation */
    if ( strcmp ( argv[2], "SetTransformOrientation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformOrientation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformOrientation */
    Tcl_DStringAppendElement ( &dString, " Set Orientations if needed\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTransformOrientation(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetTransformOrientation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransformOrientationMinValue */
    if ( strcmp ( argv[2], "GetTransformOrientationMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransformOrientationMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransformOrientationMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Orientations if needed\n" );
    Tcl_DStringAppendElement ( &dString, "int GetTransformOrientationMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetTransformOrientationMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransformOrientationMaxValue */
    if ( strcmp ( argv[2], "GetTransformOrientationMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransformOrientationMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransformOrientationMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Orientations if needed\n" );
    Tcl_DStringAppendElement ( &dString, "int GetTransformOrientationMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetTransformOrientationMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetProvideFeedback */
    if ( strcmp ( argv[2], "SetProvideFeedback" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetProvideFeedback" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetProvideFeedback */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void SetProvideFeedback(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetProvideFeedback */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetProvideFeedback */
    if ( strcmp ( argv[2], "GetProvideFeedback" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetProvideFeedback" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetProvideFeedback */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "int GetProvideFeedback();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetProvideFeedback */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ProvideFeedbackOn */
    if ( strcmp ( argv[2], "ProvideFeedbackOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ProvideFeedbackOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ProvideFeedbackOn */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void ProvideFeedbackOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for ProvideFeedbackOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ProvideFeedbackOff */
    if ( strcmp ( argv[2], "ProvideFeedbackOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ProvideFeedbackOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ProvideFeedbackOff */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void ProvideFeedbackOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for ProvideFeedbackOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLogarithmMode */
    if ( strcmp ( argv[2], "SetLogarithmMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLogarithmMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLogarithmMode */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void SetLogarithmMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetLogarithmMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLogarithmModeMinValue */
    if ( strcmp ( argv[2], "GetLogarithmModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLogarithmModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLogarithmModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "int GetLogarithmModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetLogarithmModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLogarithmModeMaxValue */
    if ( strcmp ( argv[2], "GetLogarithmModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLogarithmModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLogarithmModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "int GetLogarithmModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetLogarithmModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLogarithmMode */
    if ( strcmp ( argv[2], "GetLogarithmMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLogarithmMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLogarithmMode */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "int GetLogarithmMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetLogarithmMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LogarithmModeOn */
    if ( strcmp ( argv[2], "LogarithmModeOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LogarithmModeOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LogarithmModeOn */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void LogarithmModeOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for LogarithmModeOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LogarithmModeOff */
    if ( strcmp ( argv[2], "LogarithmModeOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LogarithmModeOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LogarithmModeOff */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void LogarithmModeOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for LogarithmModeOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInterpolationMode */
    if ( strcmp ( argv[2], "SetInterpolationMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInterpolationMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInterpolationMode */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInterpolationMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetInterpolationMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolationModeMinValue */
    if ( strcmp ( argv[2], "GetInterpolationModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolationModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolationModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInterpolationModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetInterpolationModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolationModeMaxValue */
    if ( strcmp ( argv[2], "GetInterpolationModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolationModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolationModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInterpolationModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetInterpolationModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolationMode */
    if ( strcmp ( argv[2], "GetInterpolationMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolationMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolationMode */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInterpolationMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetInterpolationMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPositiveIntensitiesOnly */
    if ( strcmp ( argv[2], "SetPositiveIntensitiesOnly" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPositiveIntensitiesOnly" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPositiveIntensitiesOnly */
    Tcl_DStringAppendElement ( &dString, " Positive Intensities Only\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPositiveIntensitiesOnly(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetPositiveIntensitiesOnly */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPositiveIntensitiesOnlyMinValue */
    if ( strcmp ( argv[2], "GetPositiveIntensitiesOnlyMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPositiveIntensitiesOnlyMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPositiveIntensitiesOnlyMinValue */
    Tcl_DStringAppendElement ( &dString, " Positive Intensities Only\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPositiveIntensitiesOnlyMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetPositiveIntensitiesOnlyMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPositiveIntensitiesOnlyMaxValue */
    if ( strcmp ( argv[2], "GetPositiveIntensitiesOnlyMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPositiveIntensitiesOnlyMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPositiveIntensitiesOnlyMaxValue */
    Tcl_DStringAppendElement ( &dString, " Positive Intensities Only\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPositiveIntensitiesOnlyMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetPositiveIntensitiesOnlyMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPositiveIntensitiesOnly */
    if ( strcmp ( argv[2], "GetPositiveIntensitiesOnly" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPositiveIntensitiesOnly" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPositiveIntensitiesOnly */
    Tcl_DStringAppendElement ( &dString, " Positive Intensities Only\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPositiveIntensitiesOnly();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetPositiveIntensitiesOnly */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: PositiveIntensitiesOnlyOn */
    if ( strcmp ( argv[2], "PositiveIntensitiesOnlyOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "PositiveIntensitiesOnlyOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for PositiveIntensitiesOnlyOn */
    Tcl_DStringAppendElement ( &dString, " Positive Intensities Only\n" );
    Tcl_DStringAppendElement ( &dString, "void PositiveIntensitiesOnlyOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for PositiveIntensitiesOnlyOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: PositiveIntensitiesOnlyOff */
    if ( strcmp ( argv[2], "PositiveIntensitiesOnlyOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "PositiveIntensitiesOnlyOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for PositiveIntensitiesOnlyOff */
    Tcl_DStringAppendElement ( &dString, " Positive Intensities Only\n" );
    Tcl_DStringAppendElement ( &dString, "void PositiveIntensitiesOnlyOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for PositiveIntensitiesOnlyOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAutoNormalizeIntensities */
    if ( strcmp ( argv[2], "SetAutoNormalizeIntensities" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAutoNormalizeIntensities" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAutoNormalizeIntensities */
    Tcl_DStringAppendElement ( &dString, " Normalize Intensities\n" );
    Tcl_DStringAppendElement ( &dString, "void SetAutoNormalizeIntensities(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetAutoNormalizeIntensities */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAutoNormalizeIntensitiesMinValue */
    if ( strcmp ( argv[2], "GetAutoNormalizeIntensitiesMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAutoNormalizeIntensitiesMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAutoNormalizeIntensitiesMinValue */
    Tcl_DStringAppendElement ( &dString, " Normalize Intensities\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAutoNormalizeIntensitiesMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetAutoNormalizeIntensitiesMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAutoNormalizeIntensitiesMaxValue */
    if ( strcmp ( argv[2], "GetAutoNormalizeIntensitiesMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAutoNormalizeIntensitiesMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAutoNormalizeIntensitiesMaxValue */
    Tcl_DStringAppendElement ( &dString, " Normalize Intensities\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAutoNormalizeIntensitiesMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetAutoNormalizeIntensitiesMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAutoNormalizeIntensities */
    if ( strcmp ( argv[2], "GetAutoNormalizeIntensities" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAutoNormalizeIntensities" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAutoNormalizeIntensities */
    Tcl_DStringAppendElement ( &dString, " Normalize Intensities\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAutoNormalizeIntensities();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetAutoNormalizeIntensities */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AutoNormalizeIntensitiesOn */
    if ( strcmp ( argv[2], "AutoNormalizeIntensitiesOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AutoNormalizeIntensitiesOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AutoNormalizeIntensitiesOn */
    Tcl_DStringAppendElement ( &dString, " Normalize Intensities\n" );
    Tcl_DStringAppendElement ( &dString, "void AutoNormalizeIntensitiesOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for AutoNormalizeIntensitiesOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AutoNormalizeIntensitiesOff */
    if ( strcmp ( argv[2], "AutoNormalizeIntensitiesOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AutoNormalizeIntensitiesOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AutoNormalizeIntensitiesOff */
    Tcl_DStringAppendElement ( &dString, " Normalize Intensities\n" );
    Tcl_DStringAppendElement ( &dString, "void AutoNormalizeIntensitiesOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for AutoNormalizeIntensitiesOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetJointlyNormalizeIntensities */
    if ( strcmp ( argv[2], "SetJointlyNormalizeIntensities" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetJointlyNormalizeIntensities" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetJointlyNormalizeIntensities */
    Tcl_DStringAppendElement ( &dString, " Intra Modality Use same range\n" );
    Tcl_DStringAppendElement ( &dString, "void SetJointlyNormalizeIntensities(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetJointlyNormalizeIntensities */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetJointlyNormalizeIntensitiesMinValue */
    if ( strcmp ( argv[2], "GetJointlyNormalizeIntensitiesMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetJointlyNormalizeIntensitiesMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetJointlyNormalizeIntensitiesMinValue */
    Tcl_DStringAppendElement ( &dString, " Intra Modality Use same range\n" );
    Tcl_DStringAppendElement ( &dString, "int GetJointlyNormalizeIntensitiesMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetJointlyNormalizeIntensitiesMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetJointlyNormalizeIntensitiesMaxValue */
    if ( strcmp ( argv[2], "GetJointlyNormalizeIntensitiesMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetJointlyNormalizeIntensitiesMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetJointlyNormalizeIntensitiesMaxValue */
    Tcl_DStringAppendElement ( &dString, " Intra Modality Use same range\n" );
    Tcl_DStringAppendElement ( &dString, "int GetJointlyNormalizeIntensitiesMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetJointlyNormalizeIntensitiesMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetJointlyNormalizeIntensities */
    if ( strcmp ( argv[2], "GetJointlyNormalizeIntensities" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetJointlyNormalizeIntensities" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetJointlyNormalizeIntensities */
    Tcl_DStringAppendElement ( &dString, " Intra Modality Use same range\n" );
    Tcl_DStringAppendElement ( &dString, "int GetJointlyNormalizeIntensities();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetJointlyNormalizeIntensities */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: JointlyNormalizeIntensitiesOn */
    if ( strcmp ( argv[2], "JointlyNormalizeIntensitiesOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "JointlyNormalizeIntensitiesOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for JointlyNormalizeIntensitiesOn */
    Tcl_DStringAppendElement ( &dString, " Intra Modality Use same range\n" );
    Tcl_DStringAppendElement ( &dString, "void JointlyNormalizeIntensitiesOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for JointlyNormalizeIntensitiesOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: JointlyNormalizeIntensitiesOff */
    if ( strcmp ( argv[2], "JointlyNormalizeIntensitiesOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "JointlyNormalizeIntensitiesOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for JointlyNormalizeIntensitiesOff */
    Tcl_DStringAppendElement ( &dString, " Intra Modality Use same range\n" );
    Tcl_DStringAppendElement ( &dString, "void JointlyNormalizeIntensitiesOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for JointlyNormalizeIntensitiesOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHistogram */
    if ( strcmp ( argv[2], "SetHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHistogram */
    Tcl_DStringAppendElement ( &dString, " Histogram Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHistogram(vtkpxJointHistogram *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHistogram */
    if ( strcmp ( argv[2], "GetHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHistogram */
    Tcl_DStringAppendElement ( &dString, " Histogram Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram *GetHistogram();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetHistogram */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetTransformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNumberOfLevels */
    if ( strcmp ( argv[2], "SetNumberOfLevels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfLevels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNumberOfLevels */
    Tcl_DStringAppendElement ( &dString, " NumberOfLevels, NumberOfSteps, Resolution\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfLevels(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetNumberOfLevels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfLevels */
    if ( strcmp ( argv[2], "GetNumberOfLevels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfLevels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfLevels */
    Tcl_DStringAppendElement ( &dString, " NumberOfLevels, NumberOfSteps, Resolution\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfLevels();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetNumberOfLevels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNumberOfExtraLevels */
    if ( strcmp ( argv[2], "SetNumberOfExtraLevels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfExtraLevels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNumberOfExtraLevels */
    Tcl_DStringAppendElement ( &dString, " Number Of Extra Levels\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfExtraLevels(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetNumberOfExtraLevels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfExtraLevels */
    if ( strcmp ( argv[2], "GetNumberOfExtraLevels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfExtraLevels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfExtraLevels */
    Tcl_DStringAppendElement ( &dString, " Number Of Extra Levels\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfExtraLevels();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetNumberOfExtraLevels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNumberOfBins */
    if ( strcmp ( argv[2], "SetNumberOfBins" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfBins" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNumberOfBins */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfBins(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetNumberOfBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfBins */
    if ( strcmp ( argv[2], "GetNumberOfBins" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBins" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfBins */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfBins();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetNumberOfBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNumberOfIterations */
    if ( strcmp ( argv[2], "SetNumberOfIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNumberOfIterations */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfIterations(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetNumberOfIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfIterations */
    if ( strcmp ( argv[2], "GetNumberOfIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfIterations */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfIterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetNumberOfIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNumberOfSteps */
    if ( strcmp ( argv[2], "SetNumberOfSteps" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfSteps" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNumberOfSteps */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfSteps(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetNumberOfSteps */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfSteps */
    if ( strcmp ( argv[2], "GetNumberOfSteps" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfSteps" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfSteps */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfSteps();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetNumberOfSteps */

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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetResolution(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetResolution();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetResolution */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBlurFactor */
    if ( strcmp ( argv[2], "SetBlurFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBlurFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBlurFactor */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetBlurFactor(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetBlurFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBlurFactorMinValue */
    if ( strcmp ( argv[2], "GetBlurFactorMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBlurFactorMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBlurFactorMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetBlurFactorMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetBlurFactorMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBlurFactorMaxValue */
    if ( strcmp ( argv[2], "GetBlurFactorMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBlurFactorMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBlurFactorMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetBlurFactorMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetBlurFactorMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBlurFactor */
    if ( strcmp ( argv[2], "GetBlurFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBlurFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBlurFactor */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetBlurFactor();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetBlurFactor */

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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetStepSize(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetStepSize();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetStepSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSimilarityMeasure */
    if ( strcmp ( argv[2], "GetSimilarityMeasure" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSimilarityMeasure" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSimilarityMeasure */
    Tcl_DStringAppendElement ( &dString, " Similarity Measure\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSimilarityMeasure();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetSimilarityMeasure */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasure */
    if ( strcmp ( argv[2], "SetSimilarityMeasure" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasure" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasure */
    Tcl_DStringAppendElement ( &dString, " Similarity Measure\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSimilarityMeasure(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasure */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasureToJointEntropy */
    if ( strcmp ( argv[2], "SetSimilarityMeasureToJointEntropy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToJointEntropy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasureToJointEntropy */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSimilarityMeasureToJointEntropy();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasureToJointEntropy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasureToCorrelation */
    if ( strcmp ( argv[2], "SetSimilarityMeasureToCorrelation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToCorrelation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasureToCorrelation */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSimilarityMeasureToCorrelation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasureToCorrelation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasureToGradientCorrelation */
    if ( strcmp ( argv[2], "SetSimilarityMeasureToGradientCorrelation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToGradientCorrelation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasureToGradientCorrelation */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSimilarityMeasureToGradientCorrelation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasureToGradientCorrelation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasureToMutualInformation */
    if ( strcmp ( argv[2], "SetSimilarityMeasureToMutualInformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToMutualInformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasureToMutualInformation */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSimilarityMeasureToMutualInformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasureToMutualInformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasureToNormalizedMutualInformation */
    if ( strcmp ( argv[2], "SetSimilarityMeasureToNormalizedMutualInformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToNormalizedMutualInformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasureToNormalizedMutualInformation */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSimilarityMeasureToNormalizedMutualInformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasureToNormalizedMutualInformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasureToSumofSquaredDifferences */
    if ( strcmp ( argv[2], "SetSimilarityMeasureToSumofSquaredDifferences" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToSumofSquaredDifferences" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasureToSumofSquaredDifferences */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSimilarityMeasureToSumofSquaredDifferences();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasureToSumofSquaredDifferences */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasureToCorrelationRatioYX */
    if ( strcmp ( argv[2], "SetSimilarityMeasureToCorrelationRatioYX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToCorrelationRatioYX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasureToCorrelationRatioYX */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSimilarityMeasureToCorrelationRatioYX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasureToCorrelationRatioYX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasureToCorrelationRatioXY */
    if ( strcmp ( argv[2], "SetSimilarityMeasureToCorrelationRatioXY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToCorrelationRatioXY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasureToCorrelationRatioXY */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSimilarityMeasureToCorrelationRatioXY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasureToCorrelationRatioXY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasureToProduct */
    if ( strcmp ( argv[2], "SetSimilarityMeasureToProduct" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToProduct" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasureToProduct */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSimilarityMeasureToProduct();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasureToProduct */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSimilarityMeasureToDifferenceEntropy */
    if ( strcmp ( argv[2], "SetSimilarityMeasureToDifferenceEntropy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSimilarityMeasureToDifferenceEntropy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSimilarityMeasureToDifferenceEntropy */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSimilarityMeasureToDifferenceEntropy();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetSimilarityMeasureToDifferenceEntropy */

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
    Tcl_DStringAppendElement ( &dString, " Runs the registration filter\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Run();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for Run */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NameOfMeasure */
    if ( strcmp ( argv[2], "NameOfMeasure" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NameOfMeasure" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NameOfMeasure */
    Tcl_DStringAppendElement ( &dString, " Returns the name of the similarity measure\n" );
    Tcl_DStringAppendElement ( &dString, "const char *NameOfMeasure();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for NameOfMeasure */

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
    Tcl_DStringAppendElement ( &dString, " Prints information about the progress of the registration\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void PrintInformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for PrintInformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutputStatus */
    if ( strcmp ( argv[2], "GetOutputStatus" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutputStatus" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutputStatus */
    Tcl_DStringAppendElement ( &dString, " GetOutputStatus \n" );
    Tcl_DStringAppendElement ( &dString, "const char *GetOutputStatus();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetOutputStatus */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUpdateTransformation */
    if ( strcmp ( argv[2], "GetUpdateTransformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUpdateTransformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUpdateTransformation */
    Tcl_DStringAppendElement ( &dString, " Get Update Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform *GetUpdateTransformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetUpdateTransformation */

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
    Tcl_DStringAppendElement ( &dString, " These are needed for the Optimizer \n" );
    Tcl_DStringAppendElement ( &dString, "static void OptimizerFeedback(vtkObject *self, const char *line, double val);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
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
    Tcl_DStringAppendElement ( &dString, " These are needed for the Optimizer \n" );
    Tcl_DStringAppendElement ( &dString, "static double CalculateGradient(vtkObject *self, vtkDoubleArray *position, vtkDoubleArray *output);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
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
    Tcl_DStringAppendElement ( &dString, " These are needed for the Optimizer \n" );
    Tcl_DStringAppendElement ( &dString, "static double CalculateFunction(vtkObject *self, vtkDoubleArray *position);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for CalculateFunction */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OnBeginIteration */
    if ( strcmp ( argv[2], "OnBeginIteration" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OnBeginIteration" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OnBeginIteration */
    Tcl_DStringAppendElement ( &dString, " These are needed for the Optimizer \n" );
    Tcl_DStringAppendElement ( &dString, "static double OnBeginIteration(vtkObject *self, vtkDoubleArray *position);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for OnBeginIteration */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethod */
    if ( strcmp ( argv[2], "SetOptimizationMethod" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethod" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethod */
    Tcl_DStringAppendElement ( &dString, " Optimization Method\n" );
    Tcl_DStringAppendElement ( &dString, "void SetOptimizationMethod(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetOptimizationMethod */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOptimizationMethod */
    if ( strcmp ( argv[2], "GetOptimizationMethod" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationMethod" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOptimizationMethod */
    Tcl_DStringAppendElement ( &dString, " Optimization Method\n" );
    Tcl_DStringAppendElement ( &dString, "int GetOptimizationMethod();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetOptimizationMethod */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethodToOldStyle */
    if ( strcmp ( argv[2], "SetOptimizationMethodToOldStyle" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToOldStyle" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethodToOldStyle */
    Tcl_DStringAppendElement ( &dString, " Shorthand for different optimization methods \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOptimizationMethodToOldStyle();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetOptimizationMethodToOldStyle */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethodToDefault */
    if ( strcmp ( argv[2], "SetOptimizationMethodToDefault" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToDefault" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethodToDefault */
    Tcl_DStringAppendElement ( &dString, " Shorthand for different optimization methods \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOptimizationMethodToDefault();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetOptimizationMethodToDefault */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethodToSlowClimb */
    if ( strcmp ( argv[2], "SetOptimizationMethodToSlowClimb" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToSlowClimb" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethodToSlowClimb */
    Tcl_DStringAppendElement ( &dString, " Shorthand for different optimization methods \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOptimizationMethodToSlowClimb();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetOptimizationMethodToSlowClimb */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethodToPowell */
    if ( strcmp ( argv[2], "SetOptimizationMethodToPowell" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToPowell" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethodToPowell */
    Tcl_DStringAppendElement ( &dString, " Shorthand for different optimization methods \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOptimizationMethodToPowell();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetOptimizationMethodToPowell */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethodToGradientDescent */
    if ( strcmp ( argv[2], "SetOptimizationMethodToGradientDescent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToGradientDescent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethodToGradientDescent */
    Tcl_DStringAppendElement ( &dString, " Shorthand for different optimization methods \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOptimizationMethodToGradientDescent();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetOptimizationMethodToGradientDescent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethodToConjugateGradient */
    if ( strcmp ( argv[2], "SetOptimizationMethodToConjugateGradient" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToConjugateGradient" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethodToConjugateGradient */
    Tcl_DStringAppendElement ( &dString, " Shorthand for different optimization methods \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOptimizationMethodToConjugateGradient();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetOptimizationMethodToConjugateGradient */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationMethodToHybrid */
    if ( strcmp ( argv[2], "SetOptimizationMethodToHybrid" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationMethodToHybrid" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationMethodToHybrid */
    Tcl_DStringAppendElement ( &dString, " Shorthand for different optimization methods \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOptimizationMethodToHybrid();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetOptimizationMethodToHybrid */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLastSimilarity */
    if ( strcmp ( argv[2], "GetLastSimilarity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLastSimilarity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLastSimilarity */
    Tcl_DStringAppendElement ( &dString, " Get Last Output Measures\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLastSimilarity();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetLastSimilarity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLastSmoothness */
    if ( strcmp ( argv[2], "GetLastSmoothness" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLastSmoothness" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLastSmoothness */
    Tcl_DStringAppendElement ( &dString, " Get Last Output Measures\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLastSmoothness();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetLastSmoothness */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetResolutionRate */
    if ( strcmp ( argv[2], "SetResolutionRate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetResolutionRate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetResolutionRate */
    Tcl_DStringAppendElement ( &dString, " Set Grid Spacing at highest resolution\n" );
    Tcl_DStringAppendElement ( &dString, "void SetResolutionRate(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for SetResolutionRate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetResolutionRateMinValue */
    if ( strcmp ( argv[2], "GetResolutionRateMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetResolutionRateMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetResolutionRateMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Grid Spacing at highest resolution\n" );
    Tcl_DStringAppendElement ( &dString, "double GetResolutionRateMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetResolutionRateMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetResolutionRateMaxValue */
    if ( strcmp ( argv[2], "GetResolutionRateMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetResolutionRateMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetResolutionRateMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Grid Spacing at highest resolution\n" );
    Tcl_DStringAppendElement ( &dString, "double GetResolutionRateMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetResolutionRateMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetResolutionRate */
    if ( strcmp ( argv[2], "GetResolutionRate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetResolutionRate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetResolutionRate */
    Tcl_DStringAppendElement ( &dString, " Set Grid Spacing at highest resolution\n" );
    Tcl_DStringAppendElement ( &dString, "double GetResolutionRate();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractIntensityRegistration" );
    /* Closing for GetResolutionRate */

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
