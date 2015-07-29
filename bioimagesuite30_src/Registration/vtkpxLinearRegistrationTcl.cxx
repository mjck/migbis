// tcl wrapper for vtkpxLinearRegistration object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxLinearRegistration.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxLinearRegistrationNewCommand()
{
  vtkpxLinearRegistration *temp = vtkpxLinearRegistration::New();
  return static_cast<ClientData>(temp);
}

int vtkpxAbstractIntensityRegistrationCppCommand(vtkpxAbstractIntensityRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxLinearRegistrationCppCommand(vtkpxLinearRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxLinearRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxLinearRegistrationCppCommand(static_cast<vtkpxLinearRegistration *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxLinearRegistrationCppCommand(vtkpxLinearRegistration *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxLinearRegistration",argv[1]))
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
    vtkpxLinearRegistration  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearRegistration");
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
    vtkpxLinearRegistration  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearRegistration");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxLinearRegistration  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearRegistration");
    return TCL_OK;
    }
    }
  if ((!strcmp("PrintInformation",argv[1]))&&(argc == 2))
    {
    op->PrintInformation();
    Tcl_ResetResult(interp);
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
  if ((!strcmp("Optimize",argv[1]))&&(argc == 7))
    {
    int      temp0;
    double   temp1;
    double   temp2;
    int      temp3;
    int      temp4;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
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
    temp20 = (op)->Optimize(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("OldOptimize",argv[1]))&&(argc == 7))
    {
    int      temp0;
    double   temp1;
    double   temp2;
    int      temp3;
    int      temp4;
    double   temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
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
    temp20 = (op)->OldOptimize(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetAutoCenterOrigin",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetAutoCenterOrigin(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAutoCenterOriginMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAutoCenterOriginMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAutoCenterOriginMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAutoCenterOriginMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAutoCenterOrigin",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAutoCenterOrigin();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("AutoCenterOriginOn",argv[1]))&&(argc == 2))
    {
    op->AutoCenterOriginOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("AutoCenterOriginOff",argv[1]))&&(argc == 2))
    {
    op->AutoCenterOriginOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationAutoScale",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetOptimizationAutoScale(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOptimizationAutoScaleMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetOptimizationAutoScaleMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetOptimizationAutoScaleMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetOptimizationAutoScaleMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetOptimizationAutoScale",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetOptimizationAutoScale();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("OptimizationAutoScaleOn",argv[1]))&&(argc == 2))
    {
    op->OptimizationAutoScaleOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("OptimizationAutoScaleOff",argv[1]))&&(argc == 2))
    {
    op->OptimizationAutoScaleOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOptimizationScaleFactor",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetOptimizationScaleFactor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOptimizationScaleFactor",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetOptimizationScaleFactor();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformModeToRigid",argv[1]))&&(argc == 2))
    {
    op->SetTransformModeToRigid();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformModeToSimilarity",argv[1]))&&(argc == 2))
    {
    op->SetTransformModeToSimilarity();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformModeToAffine",argv[1]))&&(argc == 2))
    {
    op->SetTransformModeToAffine();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformModeToRigid2D",argv[1]))&&(argc == 2))
    {
    op->SetTransformModeToRigid2D();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformModeToSimilarity2D",argv[1]))&&(argc == 2))
    {
    op->SetTransformModeToSimilarity2D();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformModeToAffine2D",argv[1]))&&(argc == 2))
    {
    op->SetTransformModeToAffine2D();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformModeToDistortion",argv[1]))&&(argc == 2))
    {
    op->SetTransformModeToDistortion();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetDistortionMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetDistortionMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDistortionModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDistortionModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDistortionModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDistortionModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDistortionMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDistortionMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("DistortionModeOn",argv[1]))&&(argc == 2))
    {
    op->DistortionModeOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("DistortionModeOff",argv[1]))&&(argc == 2))
    {
    op->DistortionModeOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetTranslationX",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetTranslationX(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTranslationXMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTranslationXMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTranslationXMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTranslationXMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetTranslationY",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetTranslationY(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTranslationYMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTranslationYMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTranslationYMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTranslationYMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetTranslationZ",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetTranslationZ(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTranslationZMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTranslationZMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTranslationZMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTranslationZMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRotationX",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetRotationX(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRotationXMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRotationXMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRotationXMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRotationXMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRotationY",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetRotationY(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRotationYMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRotationYMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRotationYMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRotationYMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRotationZ",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetRotationZ(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRotationZMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRotationZMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRotationZMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRotationZMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetScalingX",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetScalingX(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetScalingXMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetScalingXMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetScalingXMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetScalingXMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetScalingY",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetScalingY(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetScalingYMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetScalingYMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetScalingYMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetScalingYMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetScalingZ",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetScalingZ(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetScalingZMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetScalingZMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetScalingZMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetScalingZMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetShearingXY",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetShearingXY(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetShearingXYMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingXYMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingXYMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingXYMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetShearingYX",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetShearingYX(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetShearingYXMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingYXMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingYXMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingYXMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetShearingYZ",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetShearingYZ(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetShearingYZMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingYZMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingYZMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingYZMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetShearingZY",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetShearingZY(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetShearingZYMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingZYMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingZYMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingZYMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetShearingZX",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetShearingZX(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetShearingZXMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingZXMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingZXMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingZXMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetShearingXZ",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetShearingXZ(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetShearingXZMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingXZMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingXZMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingXZMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTranslationX",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTranslationX();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTranslationY",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTranslationY();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTranslationZ",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTranslationZ();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRotationX",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRotationX();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRotationY",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRotationY();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRotationZ",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRotationZ();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetScalingX",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetScalingX();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetScalingY",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetScalingY();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetScalingZ",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetScalingZ();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingXY",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingXY();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingYX",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingYX();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingYZ",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingYZ();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingZY",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingZY();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingZX",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingZX();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetShearingXZ",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetShearingXZ();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("TranslationXOn",argv[1]))&&(argc == 2))
    {
    op->TranslationXOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("TranslationXOff",argv[1]))&&(argc == 2))
    {
    op->TranslationXOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("TranslationYOn",argv[1]))&&(argc == 2))
    {
    op->TranslationYOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("TranslationYOff",argv[1]))&&(argc == 2))
    {
    op->TranslationYOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("TranslationZOn",argv[1]))&&(argc == 2))
    {
    op->TranslationZOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("TranslationZOff",argv[1]))&&(argc == 2))
    {
    op->TranslationZOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("RotationXOn",argv[1]))&&(argc == 2))
    {
    op->RotationXOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("RotationXOff",argv[1]))&&(argc == 2))
    {
    op->RotationXOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("RotationYOn",argv[1]))&&(argc == 2))
    {
    op->RotationYOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("RotationYOff",argv[1]))&&(argc == 2))
    {
    op->RotationYOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("RotationZOn",argv[1]))&&(argc == 2))
    {
    op->RotationZOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("RotationZOff",argv[1]))&&(argc == 2))
    {
    op->RotationZOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ScalingXOn",argv[1]))&&(argc == 2))
    {
    op->ScalingXOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ScalingXOff",argv[1]))&&(argc == 2))
    {
    op->ScalingXOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ScalingYOn",argv[1]))&&(argc == 2))
    {
    op->ScalingYOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ScalingYOff",argv[1]))&&(argc == 2))
    {
    op->ScalingYOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ScalingZOn",argv[1]))&&(argc == 2))
    {
    op->ScalingZOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ScalingZOff",argv[1]))&&(argc == 2))
    {
    op->ScalingZOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingXYOn",argv[1]))&&(argc == 2))
    {
    op->ShearingXYOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingXYOff",argv[1]))&&(argc == 2))
    {
    op->ShearingXYOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingYXOn",argv[1]))&&(argc == 2))
    {
    op->ShearingYXOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingYXOff",argv[1]))&&(argc == 2))
    {
    op->ShearingYXOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingYZOn",argv[1]))&&(argc == 2))
    {
    op->ShearingYZOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingYZOff",argv[1]))&&(argc == 2))
    {
    op->ShearingYZOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingZYOn",argv[1]))&&(argc == 2))
    {
    op->ShearingZYOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingZYOff",argv[1]))&&(argc == 2))
    {
    op->ShearingZYOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingZXOn",argv[1]))&&(argc == 2))
    {
    op->ShearingZXOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingZXOff",argv[1]))&&(argc == 2))
    {
    op->ShearingZXOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingXZOn",argv[1]))&&(argc == 2))
    {
    op->ShearingXZOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ShearingXZOff",argv[1]))&&(argc == 2))
    {
    op->ShearingXZOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetLinearTransform",argv[1]))&&(argc == 2))
    {
    vtkpxLinearTransform  *temp20;
    temp20 = (op)->GetLinearTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearTransform");
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
  if ((!strcmp("GetInitialTransform",argv[1]))&&(argc == 2))
    {
    vtkpxLinearTransform  *temp20;
    temp20 = (op)->GetInitialTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearTransform");
    return TCL_OK;
    }
  if ((!strcmp("SetInitialTransform",argv[1]))&&(argc == 3))
    {
    vtkpxLinearTransform  *temp0;
    error = 0;

    temp0 = (vtkpxLinearTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxLinearTransform"),interp,error));
    if (!error)
    {
    op->SetInitialTransform(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDofStatus",argv[1]))&&(argc == 3))
    {
    int      temp0;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->GetDofStatus(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("UpdateDofLabels",argv[1]))&&(argc == 2))
    {
    op->UpdateDofLabels();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxLinearRegistrationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxAbstractIntensityRegistrationCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxLinearRegistration:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  PrintInformation\n",NULL);
    Tcl_AppendResult(interp,"  ComputeRegistration\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Optimize\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  OldOptimize\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  SetAutoCenterOrigin\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAutoCenterOriginMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAutoCenterOriginMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAutoCenterOrigin\n",NULL);
    Tcl_AppendResult(interp,"  AutoCenterOriginOn\n",NULL);
    Tcl_AppendResult(interp,"  AutoCenterOriginOff\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationAutoScale\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOptimizationAutoScaleMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetOptimizationAutoScaleMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetOptimizationAutoScale\n",NULL);
    Tcl_AppendResult(interp,"  OptimizationAutoScaleOn\n",NULL);
    Tcl_AppendResult(interp,"  OptimizationAutoScaleOff\n",NULL);
    Tcl_AppendResult(interp,"  SetOptimizationScaleFactor\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOptimizationScaleFactor\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformModeToRigid\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformModeToSimilarity\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformModeToAffine\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformModeToRigid2D\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformModeToSimilarity2D\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformModeToAffine2D\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformModeToDistortion\n",NULL);
    Tcl_AppendResult(interp,"  SetDistortionMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDistortionModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDistortionModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDistortionMode\n",NULL);
    Tcl_AppendResult(interp,"  DistortionModeOn\n",NULL);
    Tcl_AppendResult(interp,"  DistortionModeOff\n",NULL);
    Tcl_AppendResult(interp,"  SetTranslationX\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationXMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationXMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetTranslationY\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationYMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationYMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetTranslationZ\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationZMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationZMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetRotationX\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationXMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationXMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetRotationY\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationYMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationYMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetRotationZ\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationZMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationZMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetScalingX\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetScalingXMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetScalingXMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetScalingY\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetScalingYMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetScalingYMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetScalingZ\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetScalingZMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetScalingZMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetShearingXY\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingXYMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingXYMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetShearingYX\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingYXMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingYXMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetShearingYZ\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingYZMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingYZMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetShearingZY\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingZYMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingZYMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetShearingZX\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingZXMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingZXMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetShearingXZ\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingXZMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingXZMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationX\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationY\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationZ\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationX\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationY\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationZ\n",NULL);
    Tcl_AppendResult(interp,"  GetScalingX\n",NULL);
    Tcl_AppendResult(interp,"  GetScalingY\n",NULL);
    Tcl_AppendResult(interp,"  GetScalingZ\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingXY\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingYX\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingYZ\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingZY\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingZX\n",NULL);
    Tcl_AppendResult(interp,"  GetShearingXZ\n",NULL);
    Tcl_AppendResult(interp,"  TranslationXOn\n",NULL);
    Tcl_AppendResult(interp,"  TranslationXOff\n",NULL);
    Tcl_AppendResult(interp,"  TranslationYOn\n",NULL);
    Tcl_AppendResult(interp,"  TranslationYOff\n",NULL);
    Tcl_AppendResult(interp,"  TranslationZOn\n",NULL);
    Tcl_AppendResult(interp,"  TranslationZOff\n",NULL);
    Tcl_AppendResult(interp,"  RotationXOn\n",NULL);
    Tcl_AppendResult(interp,"  RotationXOff\n",NULL);
    Tcl_AppendResult(interp,"  RotationYOn\n",NULL);
    Tcl_AppendResult(interp,"  RotationYOff\n",NULL);
    Tcl_AppendResult(interp,"  RotationZOn\n",NULL);
    Tcl_AppendResult(interp,"  RotationZOff\n",NULL);
    Tcl_AppendResult(interp,"  ScalingXOn\n",NULL);
    Tcl_AppendResult(interp,"  ScalingXOff\n",NULL);
    Tcl_AppendResult(interp,"  ScalingYOn\n",NULL);
    Tcl_AppendResult(interp,"  ScalingYOff\n",NULL);
    Tcl_AppendResult(interp,"  ScalingZOn\n",NULL);
    Tcl_AppendResult(interp,"  ScalingZOff\n",NULL);
    Tcl_AppendResult(interp,"  ShearingXYOn\n",NULL);
    Tcl_AppendResult(interp,"  ShearingXYOff\n",NULL);
    Tcl_AppendResult(interp,"  ShearingYXOn\n",NULL);
    Tcl_AppendResult(interp,"  ShearingYXOff\n",NULL);
    Tcl_AppendResult(interp,"  ShearingYZOn\n",NULL);
    Tcl_AppendResult(interp,"  ShearingYZOff\n",NULL);
    Tcl_AppendResult(interp,"  ShearingZYOn\n",NULL);
    Tcl_AppendResult(interp,"  ShearingZYOff\n",NULL);
    Tcl_AppendResult(interp,"  ShearingZXOn\n",NULL);
    Tcl_AppendResult(interp,"  ShearingZXOff\n",NULL);
    Tcl_AppendResult(interp,"  ShearingXZOn\n",NULL);
    Tcl_AppendResult(interp,"  ShearingXZOff\n",NULL);
    Tcl_AppendResult(interp,"  GetLinearTransform\n",NULL);
    Tcl_AppendResult(interp,"  GetUpdateTransform\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformation\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialTransform\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDofStatus\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  UpdateDofLabels\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "ComputeRegistration" );
    Tcl_DStringAppendElement ( &dString, "Optimize" );
    Tcl_DStringAppendElement ( &dString, "OldOptimize" );
    Tcl_DStringAppendElement ( &dString, "SetAutoCenterOrigin" );
    Tcl_DStringAppendElement ( &dString, "GetAutoCenterOriginMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetAutoCenterOriginMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetAutoCenterOrigin" );
    Tcl_DStringAppendElement ( &dString, "AutoCenterOriginOn" );
    Tcl_DStringAppendElement ( &dString, "AutoCenterOriginOff" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationAutoScale" );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationAutoScaleMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationAutoScaleMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationAutoScale" );
    Tcl_DStringAppendElement ( &dString, "OptimizationAutoScaleOn" );
    Tcl_DStringAppendElement ( &dString, "OptimizationAutoScaleOff" );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationScaleFactor" );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationScaleFactor" );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToRigid" );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToSimilarity" );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToAffine" );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToRigid2D" );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToSimilarity2D" );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToAffine2D" );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToDistortion" );
    Tcl_DStringAppendElement ( &dString, "SetDistortionMode" );
    Tcl_DStringAppendElement ( &dString, "GetDistortionModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetDistortionModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetDistortionMode" );
    Tcl_DStringAppendElement ( &dString, "DistortionModeOn" );
    Tcl_DStringAppendElement ( &dString, "DistortionModeOff" );
    Tcl_DStringAppendElement ( &dString, "SetTranslationX" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationXMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationXMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetTranslationY" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationYMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationYMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetTranslationZ" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationZMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationZMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetRotationX" );
    Tcl_DStringAppendElement ( &dString, "GetRotationXMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetRotationXMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetRotationY" );
    Tcl_DStringAppendElement ( &dString, "GetRotationYMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetRotationYMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetRotationZ" );
    Tcl_DStringAppendElement ( &dString, "GetRotationZMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetRotationZMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetScalingX" );
    Tcl_DStringAppendElement ( &dString, "GetScalingXMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetScalingXMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetScalingY" );
    Tcl_DStringAppendElement ( &dString, "GetScalingYMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetScalingYMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetScalingZ" );
    Tcl_DStringAppendElement ( &dString, "GetScalingZMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetScalingZMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetShearingXY" );
    Tcl_DStringAppendElement ( &dString, "GetShearingXYMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetShearingXYMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetShearingYX" );
    Tcl_DStringAppendElement ( &dString, "GetShearingYXMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetShearingYXMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetShearingYZ" );
    Tcl_DStringAppendElement ( &dString, "GetShearingYZMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetShearingYZMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetShearingZY" );
    Tcl_DStringAppendElement ( &dString, "GetShearingZYMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetShearingZYMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetShearingZX" );
    Tcl_DStringAppendElement ( &dString, "GetShearingZXMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetShearingZXMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetShearingXZ" );
    Tcl_DStringAppendElement ( &dString, "GetShearingXZMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetShearingXZMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationX" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationY" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationZ" );
    Tcl_DStringAppendElement ( &dString, "GetRotationX" );
    Tcl_DStringAppendElement ( &dString, "GetRotationY" );
    Tcl_DStringAppendElement ( &dString, "GetRotationZ" );
    Tcl_DStringAppendElement ( &dString, "GetScalingX" );
    Tcl_DStringAppendElement ( &dString, "GetScalingY" );
    Tcl_DStringAppendElement ( &dString, "GetScalingZ" );
    Tcl_DStringAppendElement ( &dString, "GetShearingXY" );
    Tcl_DStringAppendElement ( &dString, "GetShearingYX" );
    Tcl_DStringAppendElement ( &dString, "GetShearingYZ" );
    Tcl_DStringAppendElement ( &dString, "GetShearingZY" );
    Tcl_DStringAppendElement ( &dString, "GetShearingZX" );
    Tcl_DStringAppendElement ( &dString, "GetShearingXZ" );
    Tcl_DStringAppendElement ( &dString, "TranslationXOn" );
    Tcl_DStringAppendElement ( &dString, "TranslationXOff" );
    Tcl_DStringAppendElement ( &dString, "TranslationYOn" );
    Tcl_DStringAppendElement ( &dString, "TranslationYOff" );
    Tcl_DStringAppendElement ( &dString, "TranslationZOn" );
    Tcl_DStringAppendElement ( &dString, "TranslationZOff" );
    Tcl_DStringAppendElement ( &dString, "RotationXOn" );
    Tcl_DStringAppendElement ( &dString, "RotationXOff" );
    Tcl_DStringAppendElement ( &dString, "RotationYOn" );
    Tcl_DStringAppendElement ( &dString, "RotationYOff" );
    Tcl_DStringAppendElement ( &dString, "RotationZOn" );
    Tcl_DStringAppendElement ( &dString, "RotationZOff" );
    Tcl_DStringAppendElement ( &dString, "ScalingXOn" );
    Tcl_DStringAppendElement ( &dString, "ScalingXOff" );
    Tcl_DStringAppendElement ( &dString, "ScalingYOn" );
    Tcl_DStringAppendElement ( &dString, "ScalingYOff" );
    Tcl_DStringAppendElement ( &dString, "ScalingZOn" );
    Tcl_DStringAppendElement ( &dString, "ScalingZOff" );
    Tcl_DStringAppendElement ( &dString, "ShearingXYOn" );
    Tcl_DStringAppendElement ( &dString, "ShearingXYOff" );
    Tcl_DStringAppendElement ( &dString, "ShearingYXOn" );
    Tcl_DStringAppendElement ( &dString, "ShearingYXOff" );
    Tcl_DStringAppendElement ( &dString, "ShearingYZOn" );
    Tcl_DStringAppendElement ( &dString, "ShearingYZOff" );
    Tcl_DStringAppendElement ( &dString, "ShearingZYOn" );
    Tcl_DStringAppendElement ( &dString, "ShearingZYOff" );
    Tcl_DStringAppendElement ( &dString, "ShearingZXOn" );
    Tcl_DStringAppendElement ( &dString, "ShearingZXOff" );
    Tcl_DStringAppendElement ( &dString, "ShearingXZOn" );
    Tcl_DStringAppendElement ( &dString, "ShearingXZOff" );
    Tcl_DStringAppendElement ( &dString, "GetLinearTransform" );
    Tcl_DStringAppendElement ( &dString, "GetUpdateTransform" );
    Tcl_DStringAppendElement ( &dString, "GetTransformation" );
    Tcl_DStringAppendElement ( &dString, "GetInitialTransform" );
    Tcl_DStringAppendElement ( &dString, "SetInitialTransform" );
    Tcl_DStringAppendElement ( &dString, "GetDofStatus" );
    Tcl_DStringAppendElement ( &dString, "UpdateDofLabels" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxLinearRegistration *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for PrintInformation */

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
    Tcl_DStringAppendElement ( &dString, " Run Optimization for Level level\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ComputeRegistration(int level);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ComputeRegistration */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Optimize */
    if ( strcmp ( argv[2], "Optimize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Optimize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Optimize */
    Tcl_DStringAppendElement ( &dString, " Internal Optimization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double Optimize(int maxdofs, double step, double old_similarity, int level, int st);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for Optimize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OldOptimize */
    if ( strcmp ( argv[2], "OldOptimize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OldOptimize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OldOptimize */
    Tcl_DStringAppendElement ( &dString, " Internal Optimization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double OldOptimize(int maxdofs, double step, double old_similarity, int level, int st);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for OldOptimize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAutoCenterOrigin */
    if ( strcmp ( argv[2], "SetAutoCenterOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAutoCenterOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAutoCenterOrigin */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "void SetAutoCenterOrigin(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetAutoCenterOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAutoCenterOriginMinValue */
    if ( strcmp ( argv[2], "GetAutoCenterOriginMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAutoCenterOriginMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAutoCenterOriginMinValue */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAutoCenterOriginMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetAutoCenterOriginMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAutoCenterOriginMaxValue */
    if ( strcmp ( argv[2], "GetAutoCenterOriginMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAutoCenterOriginMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAutoCenterOriginMaxValue */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAutoCenterOriginMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetAutoCenterOriginMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAutoCenterOrigin */
    if ( strcmp ( argv[2], "GetAutoCenterOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAutoCenterOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAutoCenterOrigin */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAutoCenterOrigin();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetAutoCenterOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AutoCenterOriginOn */
    if ( strcmp ( argv[2], "AutoCenterOriginOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AutoCenterOriginOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AutoCenterOriginOn */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "void AutoCenterOriginOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for AutoCenterOriginOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AutoCenterOriginOff */
    if ( strcmp ( argv[2], "AutoCenterOriginOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AutoCenterOriginOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AutoCenterOriginOff */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "void AutoCenterOriginOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for AutoCenterOriginOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationAutoScale */
    if ( strcmp ( argv[2], "SetOptimizationAutoScale" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationAutoScale" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationAutoScale */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "void SetOptimizationAutoScale(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetOptimizationAutoScale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOptimizationAutoScaleMinValue */
    if ( strcmp ( argv[2], "GetOptimizationAutoScaleMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationAutoScaleMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOptimizationAutoScaleMinValue */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "int GetOptimizationAutoScaleMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetOptimizationAutoScaleMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOptimizationAutoScaleMaxValue */
    if ( strcmp ( argv[2], "GetOptimizationAutoScaleMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationAutoScaleMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOptimizationAutoScaleMaxValue */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "int GetOptimizationAutoScaleMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetOptimizationAutoScaleMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOptimizationAutoScale */
    if ( strcmp ( argv[2], "GetOptimizationAutoScale" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationAutoScale" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOptimizationAutoScale */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "int GetOptimizationAutoScale();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetOptimizationAutoScale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OptimizationAutoScaleOn */
    if ( strcmp ( argv[2], "OptimizationAutoScaleOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OptimizationAutoScaleOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OptimizationAutoScaleOn */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "void OptimizationAutoScaleOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for OptimizationAutoScaleOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OptimizationAutoScaleOff */
    if ( strcmp ( argv[2], "OptimizationAutoScaleOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OptimizationAutoScaleOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OptimizationAutoScaleOff */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "void OptimizationAutoScaleOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for OptimizationAutoScaleOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOptimizationScaleFactor */
    if ( strcmp ( argv[2], "SetOptimizationScaleFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOptimizationScaleFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOptimizationScaleFactor */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "void SetOptimizationScaleFactor(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetOptimizationScaleFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOptimizationScaleFactor */
    if ( strcmp ( argv[2], "GetOptimizationScaleFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOptimizationScaleFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOptimizationScaleFactor */
    Tcl_DStringAppendElement ( &dString, " Auto Center Origin\n" );
    Tcl_DStringAppendElement ( &dString, "double GetOptimizationScaleFactor();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetOptimizationScaleFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformModeToRigid */
    if ( strcmp ( argv[2], "SetTransformModeToRigid" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToRigid" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformModeToRigid */
    Tcl_DStringAppendElement ( &dString, " Set Different Modes\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformModeToRigid();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetTransformModeToRigid */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformModeToSimilarity */
    if ( strcmp ( argv[2], "SetTransformModeToSimilarity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToSimilarity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformModeToSimilarity */
    Tcl_DStringAppendElement ( &dString, " Set Different Modes\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformModeToSimilarity();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetTransformModeToSimilarity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformModeToAffine */
    if ( strcmp ( argv[2], "SetTransformModeToAffine" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToAffine" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformModeToAffine */
    Tcl_DStringAppendElement ( &dString, " Set Different Modes\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformModeToAffine();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetTransformModeToAffine */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformModeToRigid2D */
    if ( strcmp ( argv[2], "SetTransformModeToRigid2D" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToRigid2D" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformModeToRigid2D */
    Tcl_DStringAppendElement ( &dString, " Set Different Modes\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformModeToRigid2D();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetTransformModeToRigid2D */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformModeToSimilarity2D */
    if ( strcmp ( argv[2], "SetTransformModeToSimilarity2D" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToSimilarity2D" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformModeToSimilarity2D */
    Tcl_DStringAppendElement ( &dString, " Set Different Modes\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformModeToSimilarity2D();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetTransformModeToSimilarity2D */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformModeToAffine2D */
    if ( strcmp ( argv[2], "SetTransformModeToAffine2D" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToAffine2D" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformModeToAffine2D */
    Tcl_DStringAppendElement ( &dString, " Set Different Modes\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformModeToAffine2D();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetTransformModeToAffine2D */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformModeToDistortion */
    if ( strcmp ( argv[2], "SetTransformModeToDistortion" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToDistortion" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformModeToDistortion */
    Tcl_DStringAppendElement ( &dString, " Set Different Modes\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformModeToDistortion();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetTransformModeToDistortion */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDistortionMode */
    if ( strcmp ( argv[2], "SetDistortionMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDistortionMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDistortionMode */
    Tcl_DStringAppendElement ( &dString, " Distortion Mode;\n" );
    Tcl_DStringAppendElement ( &dString, "void SetDistortionMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetDistortionMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDistortionModeMinValue */
    if ( strcmp ( argv[2], "GetDistortionModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDistortionModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDistortionModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Distortion Mode;\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDistortionModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetDistortionModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDistortionModeMaxValue */
    if ( strcmp ( argv[2], "GetDistortionModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDistortionModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDistortionModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Distortion Mode;\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDistortionModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetDistortionModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDistortionMode */
    if ( strcmp ( argv[2], "GetDistortionMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDistortionMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDistortionMode */
    Tcl_DStringAppendElement ( &dString, " Distortion Mode;\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDistortionMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetDistortionMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DistortionModeOn */
    if ( strcmp ( argv[2], "DistortionModeOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DistortionModeOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DistortionModeOn */
    Tcl_DStringAppendElement ( &dString, " Distortion Mode;\n" );
    Tcl_DStringAppendElement ( &dString, "void DistortionModeOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for DistortionModeOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DistortionModeOff */
    if ( strcmp ( argv[2], "DistortionModeOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DistortionModeOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DistortionModeOff */
    Tcl_DStringAppendElement ( &dString, " Distortion Mode;\n" );
    Tcl_DStringAppendElement ( &dString, "void DistortionModeOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for DistortionModeOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTranslationX */
    if ( strcmp ( argv[2], "SetTranslationX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTranslationX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTranslationX */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTranslationX(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetTranslationX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationXMinValue */
    if ( strcmp ( argv[2], "GetTranslationXMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationXMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationXMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetTranslationXMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetTranslationXMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationXMaxValue */
    if ( strcmp ( argv[2], "GetTranslationXMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationXMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationXMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetTranslationXMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetTranslationXMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTranslationY */
    if ( strcmp ( argv[2], "SetTranslationY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTranslationY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTranslationY */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTranslationY(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetTranslationY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationYMinValue */
    if ( strcmp ( argv[2], "GetTranslationYMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationYMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationYMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetTranslationYMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetTranslationYMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationYMaxValue */
    if ( strcmp ( argv[2], "GetTranslationYMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationYMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationYMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetTranslationYMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetTranslationYMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTranslationZ */
    if ( strcmp ( argv[2], "SetTranslationZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTranslationZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTranslationZ */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTranslationZ(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetTranslationZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationZMinValue */
    if ( strcmp ( argv[2], "GetTranslationZMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationZMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationZMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetTranslationZMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetTranslationZMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationZMaxValue */
    if ( strcmp ( argv[2], "GetTranslationZMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationZMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationZMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetTranslationZMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetTranslationZMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRotationX */
    if ( strcmp ( argv[2], "SetRotationX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRotationX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRotationX */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRotationX(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetRotationX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationXMinValue */
    if ( strcmp ( argv[2], "GetRotationXMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationXMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationXMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRotationXMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetRotationXMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationXMaxValue */
    if ( strcmp ( argv[2], "GetRotationXMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationXMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationXMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRotationXMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetRotationXMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRotationY */
    if ( strcmp ( argv[2], "SetRotationY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRotationY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRotationY */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRotationY(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetRotationY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationYMinValue */
    if ( strcmp ( argv[2], "GetRotationYMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationYMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationYMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRotationYMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetRotationYMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationYMaxValue */
    if ( strcmp ( argv[2], "GetRotationYMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationYMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationYMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRotationYMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetRotationYMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRotationZ */
    if ( strcmp ( argv[2], "SetRotationZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRotationZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRotationZ */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRotationZ(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetRotationZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationZMinValue */
    if ( strcmp ( argv[2], "GetRotationZMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationZMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationZMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRotationZMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetRotationZMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationZMaxValue */
    if ( strcmp ( argv[2], "GetRotationZMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationZMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationZMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRotationZMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetRotationZMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetScalingX */
    if ( strcmp ( argv[2], "SetScalingX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetScalingX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetScalingX */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetScalingX(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetScalingX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetScalingXMinValue */
    if ( strcmp ( argv[2], "GetScalingXMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetScalingXMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetScalingXMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetScalingXMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetScalingXMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetScalingXMaxValue */
    if ( strcmp ( argv[2], "GetScalingXMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetScalingXMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetScalingXMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetScalingXMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetScalingXMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetScalingY */
    if ( strcmp ( argv[2], "SetScalingY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetScalingY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetScalingY */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetScalingY(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetScalingY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetScalingYMinValue */
    if ( strcmp ( argv[2], "GetScalingYMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetScalingYMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetScalingYMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetScalingYMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetScalingYMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetScalingYMaxValue */
    if ( strcmp ( argv[2], "GetScalingYMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetScalingYMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetScalingYMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetScalingYMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetScalingYMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetScalingZ */
    if ( strcmp ( argv[2], "SetScalingZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetScalingZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetScalingZ */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetScalingZ(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetScalingZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetScalingZMinValue */
    if ( strcmp ( argv[2], "GetScalingZMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetScalingZMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetScalingZMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetScalingZMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetScalingZMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetScalingZMaxValue */
    if ( strcmp ( argv[2], "GetScalingZMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetScalingZMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetScalingZMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetScalingZMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetScalingZMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetShearingXY */
    if ( strcmp ( argv[2], "SetShearingXY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetShearingXY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetShearingXY */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetShearingXY(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetShearingXY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingXYMinValue */
    if ( strcmp ( argv[2], "GetShearingXYMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingXYMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingXYMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingXYMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingXYMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingXYMaxValue */
    if ( strcmp ( argv[2], "GetShearingXYMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingXYMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingXYMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingXYMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingXYMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetShearingYX */
    if ( strcmp ( argv[2], "SetShearingYX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetShearingYX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetShearingYX */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetShearingYX(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetShearingYX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingYXMinValue */
    if ( strcmp ( argv[2], "GetShearingYXMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingYXMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingYXMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingYXMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingYXMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingYXMaxValue */
    if ( strcmp ( argv[2], "GetShearingYXMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingYXMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingYXMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingYXMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingYXMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetShearingYZ */
    if ( strcmp ( argv[2], "SetShearingYZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetShearingYZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetShearingYZ */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetShearingYZ(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetShearingYZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingYZMinValue */
    if ( strcmp ( argv[2], "GetShearingYZMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingYZMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingYZMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingYZMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingYZMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingYZMaxValue */
    if ( strcmp ( argv[2], "GetShearingYZMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingYZMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingYZMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingYZMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingYZMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetShearingZY */
    if ( strcmp ( argv[2], "SetShearingZY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetShearingZY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetShearingZY */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetShearingZY(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetShearingZY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingZYMinValue */
    if ( strcmp ( argv[2], "GetShearingZYMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingZYMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingZYMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingZYMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingZYMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingZYMaxValue */
    if ( strcmp ( argv[2], "GetShearingZYMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingZYMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingZYMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingZYMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingZYMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetShearingZX */
    if ( strcmp ( argv[2], "SetShearingZX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetShearingZX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetShearingZX */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetShearingZX(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetShearingZX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingZXMinValue */
    if ( strcmp ( argv[2], "GetShearingZXMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingZXMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingZXMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingZXMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingZXMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingZXMaxValue */
    if ( strcmp ( argv[2], "GetShearingZXMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingZXMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingZXMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingZXMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingZXMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetShearingXZ */
    if ( strcmp ( argv[2], "SetShearingXZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetShearingXZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetShearingXZ */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "void SetShearingXZ(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetShearingXZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingXZMinValue */
    if ( strcmp ( argv[2], "GetShearingXZMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingXZMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingXZMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingXZMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingXZMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingXZMaxValue */
    if ( strcmp ( argv[2], "GetShearingXZMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingXZMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingXZMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Active/Passive Degrees of Freedom\n" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingXZMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingXZMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationX */
    if ( strcmp ( argv[2], "GetTranslationX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationX */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetTranslationX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetTranslationX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationY */
    if ( strcmp ( argv[2], "GetTranslationY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationY */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetTranslationY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetTranslationY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationZ */
    if ( strcmp ( argv[2], "GetTranslationZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationZ */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetTranslationZ();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetTranslationZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationX */
    if ( strcmp ( argv[2], "GetRotationX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationX */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetRotationX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetRotationX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationY */
    if ( strcmp ( argv[2], "GetRotationY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationY */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetRotationY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetRotationY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationZ */
    if ( strcmp ( argv[2], "GetRotationZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationZ */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetRotationZ();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetRotationZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetScalingX */
    if ( strcmp ( argv[2], "GetScalingX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetScalingX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetScalingX */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetScalingX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetScalingX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetScalingY */
    if ( strcmp ( argv[2], "GetScalingY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetScalingY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetScalingY */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetScalingY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetScalingY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetScalingZ */
    if ( strcmp ( argv[2], "GetScalingZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetScalingZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetScalingZ */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetScalingZ();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetScalingZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingXY */
    if ( strcmp ( argv[2], "GetShearingXY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingXY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingXY */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingXY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingXY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingYX */
    if ( strcmp ( argv[2], "GetShearingYX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingYX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingYX */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingYX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingYX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingYZ */
    if ( strcmp ( argv[2], "GetShearingYZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingYZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingYZ */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingYZ();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingYZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingZY */
    if ( strcmp ( argv[2], "GetShearingZY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingZY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingZY */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingZY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingZY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingZX */
    if ( strcmp ( argv[2], "GetShearingZX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingZX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingZX */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingZX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingZX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetShearingXZ */
    if ( strcmp ( argv[2], "GetShearingXZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetShearingXZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetShearingXZ */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetShearingXZ();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetShearingXZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TranslationXOn */
    if ( strcmp ( argv[2], "TranslationXOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TranslationXOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TranslationXOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void TranslationXOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for TranslationXOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TranslationXOff */
    if ( strcmp ( argv[2], "TranslationXOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TranslationXOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TranslationXOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void TranslationXOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for TranslationXOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TranslationYOn */
    if ( strcmp ( argv[2], "TranslationYOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TranslationYOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TranslationYOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void TranslationYOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for TranslationYOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TranslationYOff */
    if ( strcmp ( argv[2], "TranslationYOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TranslationYOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TranslationYOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void TranslationYOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for TranslationYOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TranslationZOn */
    if ( strcmp ( argv[2], "TranslationZOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TranslationZOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TranslationZOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void TranslationZOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for TranslationZOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: TranslationZOff */
    if ( strcmp ( argv[2], "TranslationZOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "TranslationZOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for TranslationZOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void TranslationZOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for TranslationZOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RotationXOn */
    if ( strcmp ( argv[2], "RotationXOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RotationXOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RotationXOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void RotationXOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for RotationXOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RotationXOff */
    if ( strcmp ( argv[2], "RotationXOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RotationXOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RotationXOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void RotationXOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for RotationXOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RotationYOn */
    if ( strcmp ( argv[2], "RotationYOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RotationYOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RotationYOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void RotationYOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for RotationYOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RotationYOff */
    if ( strcmp ( argv[2], "RotationYOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RotationYOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RotationYOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void RotationYOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for RotationYOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RotationZOn */
    if ( strcmp ( argv[2], "RotationZOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RotationZOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RotationZOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void RotationZOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for RotationZOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RotationZOff */
    if ( strcmp ( argv[2], "RotationZOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RotationZOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RotationZOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void RotationZOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for RotationZOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScalingXOn */
    if ( strcmp ( argv[2], "ScalingXOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScalingXOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScalingXOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ScalingXOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ScalingXOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScalingXOff */
    if ( strcmp ( argv[2], "ScalingXOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScalingXOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScalingXOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ScalingXOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ScalingXOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScalingYOn */
    if ( strcmp ( argv[2], "ScalingYOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScalingYOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScalingYOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ScalingYOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ScalingYOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScalingYOff */
    if ( strcmp ( argv[2], "ScalingYOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScalingYOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScalingYOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ScalingYOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ScalingYOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScalingZOn */
    if ( strcmp ( argv[2], "ScalingZOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScalingZOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScalingZOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ScalingZOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ScalingZOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScalingZOff */
    if ( strcmp ( argv[2], "ScalingZOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScalingZOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScalingZOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ScalingZOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ScalingZOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingXYOn */
    if ( strcmp ( argv[2], "ShearingXYOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingXYOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingXYOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingXYOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingXYOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingXYOff */
    if ( strcmp ( argv[2], "ShearingXYOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingXYOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingXYOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingXYOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingXYOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingYXOn */
    if ( strcmp ( argv[2], "ShearingYXOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingYXOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingYXOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingYXOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingYXOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingYXOff */
    if ( strcmp ( argv[2], "ShearingYXOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingYXOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingYXOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingYXOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingYXOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingYZOn */
    if ( strcmp ( argv[2], "ShearingYZOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingYZOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingYZOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingYZOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingYZOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingYZOff */
    if ( strcmp ( argv[2], "ShearingYZOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingYZOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingYZOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingYZOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingYZOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingZYOn */
    if ( strcmp ( argv[2], "ShearingZYOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingZYOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingZYOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingZYOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingZYOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingZYOff */
    if ( strcmp ( argv[2], "ShearingZYOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingZYOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingZYOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingZYOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingZYOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingZXOn */
    if ( strcmp ( argv[2], "ShearingZXOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingZXOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingZXOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingZXOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingZXOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingZXOff */
    if ( strcmp ( argv[2], "ShearingZXOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingZXOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingZXOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingZXOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingZXOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingXZOn */
    if ( strcmp ( argv[2], "ShearingXZOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingXZOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingXZOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingXZOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingXZOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ShearingXZOff */
    if ( strcmp ( argv[2], "ShearingXZOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ShearingXZOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ShearingXZOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ShearingXZOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for ShearingXZOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLinearTransform */
    if ( strcmp ( argv[2], "GetLinearTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLinearTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLinearTransform */
    Tcl_DStringAppendElement ( &dString, " Return Internal Transform Structure\n" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform *GetLinearTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetLinearTransform */

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
    Tcl_DStringAppendElement ( &dString, " Return Update Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkAbstractTransform *GetUpdateTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
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
    Tcl_DStringAppendElement ( &dString, " Get Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkAbstractTransform *GetTransformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetTransformation */

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
    Tcl_DStringAppendElement ( &dString, " Initial Transform\n" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform *GetInitialTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetInitialTransform */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialTransform */
    Tcl_DStringAppendElement ( &dString, " Initial Transform\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialTransform(vtkpxLinearTransform *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for SetInitialTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDofStatus */
    if ( strcmp ( argv[2], "GetDofStatus" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDofStatus" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDofStatus */
    Tcl_DStringAppendElement ( &dString, " Dof Status\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetDofStatus(int dof);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for GetDofStatus */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UpdateDofLabels */
    if ( strcmp ( argv[2], "UpdateDofLabels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UpdateDofLabels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UpdateDofLabels */
    Tcl_DStringAppendElement ( &dString, " Dof Status\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void UpdateDofLabels();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRegistration" );
    /* Closing for UpdateDofLabels */

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
