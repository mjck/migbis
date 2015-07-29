// tcl wrapper for vtkpxArticulatedJointTransform object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxArticulatedJointTransform.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxArticulatedJointTransformNewCommand()
{
  vtkpxArticulatedJointTransform *temp = vtkpxArticulatedJointTransform::New();
  return static_cast<ClientData>(temp);
}

int vtkAbstractTransformCppCommand(vtkAbstractTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxArticulatedJointTransformCppCommand(vtkpxArticulatedJointTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxArticulatedJointTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxArticulatedJointTransformCppCommand(static_cast<vtkpxArticulatedJointTransform *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxArticulatedJointTransformCppCommand(vtkpxArticulatedJointTransform *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxArticulatedJointTransform",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkAbstractTransformCppCommand(static_cast<vtkAbstractTransform *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkAbstractTransform"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxArticulatedJointTransform  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxArticulatedJointTransform");
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
    vtkpxArticulatedJointTransform  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxArticulatedJointTransform");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxArticulatedJointTransform  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxArticulatedJointTransform");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetParameters",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->SetParameters(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetParameters",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->GetParameters(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDetailMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDetailMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetDetailMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetDetailMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDetailModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDetailModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDetailModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDetailModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSmoothingMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSmoothingMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSmoothingMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetSmoothingMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSmoothingModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSmoothingModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSmoothingModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSmoothingModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSmoothingRadius",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSmoothingRadius();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSmoothingRadius",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetSmoothingRadius(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSmoothingRadiusMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSmoothingRadiusMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSmoothingRadiusMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSmoothingRadiusMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetZBlendingSize",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetZBlendingSize();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetZBlendingSize",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetZBlendingSize(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetZBlendingSizeMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetZBlendingSizeMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetZBlendingSizeMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetZBlendingSizeMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBlendedMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBlendedMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetBlendedMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetBlendedMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBlendedModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBlendedModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBlendedModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBlendedModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSeparateZRotation",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSeparateZRotation();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSeparateZRotation",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetSeparateZRotation(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSeparateZRotationMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSeparateZRotationMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSeparateZRotationMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSeparateZRotationMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBlendingWindow",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetBlendingWindow();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetBlendingWindow",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetBlendingWindow(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBlendingWindowMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetBlendingWindowMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBlendingWindowMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetBlendingWindowMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCubicBlending",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCubicBlending();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCubicBlending",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetCubicBlending(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCubicBlendingMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCubicBlendingMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCubicBlendingMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCubicBlendingMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetActive",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetActive();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetActive",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetActive(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMinimumSectorAngle",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMinimumSectorAngle();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMinimumSectorAngle",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMinimumSectorAngle(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMinimumSectorAngleMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMinimumSectorAngleMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinimumSectorAngleMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMinimumSectorAngleMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRegionLabelImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetRegionLabelImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("ExternalSetRegionLabelImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->ExternalSetRegionLabelImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRegionLabelIndex",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRegionLabelIndex();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRegionLabelIndex",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetRegionLabelIndex(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetChildRegionLabelIndex",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetChildRegionLabelIndex();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetChildRegionLabelIndex",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetChildRegionLabelIndex(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetChildRegionLabelIndex2",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetChildRegionLabelIndex2();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetChildRegionLabelIndex2",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetChildRegionLabelIndex2(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSimilarityTransform",argv[1]))&&(argc == 2))
    {
    vtkpxSimilarityTransform  *temp20;
    temp20 = (op)->GetSimilarityTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSimilarityTransform");
    return TCL_OK;
    }
  if ((!strcmp("GetZSimilarityTransform",argv[1]))&&(argc == 2))
    {
    vtkpxSimilarityTransform  *temp20;
    temp20 = (op)->GetZSimilarityTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSimilarityTransform");
    return TCL_OK;
    }
  if ((!strcmp("Copy",argv[1]))&&(argc == 3))
    {
    vtkpxArticulatedJointTransform  *temp0;
    error = 0;

    temp0 = (vtkpxArticulatedJointTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxArticulatedJointTransform"),interp,error));
    if (!error)
    {
    op->Copy(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetModeToRotationXYOnly",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationXYOnly();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationOnly",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationOnly();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationScale",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationScale();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationTranslation",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationTranslation();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationScaleTranslation",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationScaleTranslation();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationOrigin",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationOrigin();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToFixedParameters",argv[1]))&&(argc == 2))
    {
    op->SetModeToFixedParameters();
    Tcl_ResetResult(interp);
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
  if ((!strcmp("GetMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Put",argv[1]))&&(argc == 4))
    {
    int      temp0;
    float    temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    op->Put(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Set",argv[1]))&&(argc == 4))
    {
    int      temp0;
    float    temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    op->Set(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Get",argv[1]))&&(argc == 3))
    {
    int      temp0;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->Get(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFreezeUpdates",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetFreezeUpdates(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFreezeUpdates",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFreezeUpdates();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Load",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->Load(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Save",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->Save(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfParameters",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfParameters();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAlignmentTransform",argv[1]))&&(argc == 2))
    {
    vtkAbstractTransform  *temp20;
    temp20 = (op)->GetAlignmentTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
    return TCL_OK;
    }
  if ((!strcmp("SetAlignmentTransform",argv[1]))&&(argc == 3))
    {
    vtkAbstractTransform  *temp0;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    op->SetAlignmentTransform(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAlignmentTransformIndex",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAlignmentTransformIndex();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetAlignmentTransformIndex",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetAlignmentTransformIndex(temp0);
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
  if ((!strcmp("MakeTransform",argv[1]))&&(argc == 2))
    {
    vtkAbstractTransform  *temp20;
    temp20 = (op)->MakeTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
    return TCL_OK;
    }
  if ((!strcmp("ComputeAllAngles",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->ComputeAllAngles();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("AdjustOriginToBeOutside",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->AdjustOriginToBeOutside(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOrigin",argv[1]))&&(argc == 5))
    {
    float  temp0[3];
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0[0] = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp0[1] = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp0[2] = tempd;
    if (!error)
    {
    op->GetOrigin(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLocalZAxis",argv[1]))&&(argc == 5))
    {
    float  temp0[3];
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0[0] = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp0[1] = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp0[2] = tempd;
    if (!error)
    {
    op->GetLocalZAxis(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLocalXAxis",argv[1]))&&(argc == 5))
    {
    float  temp0[3];
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0[0] = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp0[1] = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp0[2] = tempd;
    if (!error)
    {
    op->GetLocalXAxis(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Identity",argv[1]))&&(argc == 2))
    {
    op->Identity();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetJointName",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetJointName(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetJointName",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetJointName();
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

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxArticulatedJointTransformCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkAbstractTransformCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxArticulatedJointTransform:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetParameters\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetParameters\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetDetailMode\n",NULL);
    Tcl_AppendResult(interp,"  SetDetailMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDetailModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDetailModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSmoothingMode\n",NULL);
    Tcl_AppendResult(interp,"  SetSmoothingMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSmoothingModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSmoothingModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSmoothingRadius\n",NULL);
    Tcl_AppendResult(interp,"  SetSmoothingRadius\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSmoothingRadiusMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSmoothingRadiusMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetZBlendingSize\n",NULL);
    Tcl_AppendResult(interp,"  SetZBlendingSize\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetZBlendingSizeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetZBlendingSizeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBlendedMode\n",NULL);
    Tcl_AppendResult(interp,"  SetBlendedMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBlendedModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBlendedModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSeparateZRotation\n",NULL);
    Tcl_AppendResult(interp,"  SetSeparateZRotation\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSeparateZRotationMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSeparateZRotationMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBlendingWindow\n",NULL);
    Tcl_AppendResult(interp,"  SetBlendingWindow\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBlendingWindowMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBlendingWindowMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCubicBlending\n",NULL);
    Tcl_AppendResult(interp,"  SetCubicBlending\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCubicBlendingMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCubicBlendingMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetActive\n",NULL);
    Tcl_AppendResult(interp,"  SetActive\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMinimumSectorAngle\n",NULL);
    Tcl_AppendResult(interp,"  SetMinimumSectorAngle\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMinimumSectorAngleMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMinimumSectorAngleMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRegionLabelImage\n",NULL);
    Tcl_AppendResult(interp,"  ExternalSetRegionLabelImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRegionLabelIndex\n",NULL);
    Tcl_AppendResult(interp,"  SetRegionLabelIndex\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetChildRegionLabelIndex\n",NULL);
    Tcl_AppendResult(interp,"  SetChildRegionLabelIndex\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetChildRegionLabelIndex2\n",NULL);
    Tcl_AppendResult(interp,"  SetChildRegionLabelIndex2\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSimilarityTransform\n",NULL);
    Tcl_AppendResult(interp,"  GetZSimilarityTransform\n",NULL);
    Tcl_AppendResult(interp,"  Copy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationXYOnly\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationOnly\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationScale\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationTranslation\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationScaleTranslation\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationOrigin\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToFixedParameters\n",NULL);
    Tcl_AppendResult(interp,"  SetMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMode\n",NULL);
    Tcl_AppendResult(interp,"  Put\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Set\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Get\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetFreezeUpdates\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFreezeUpdates\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfParameters\n",NULL);
    Tcl_AppendResult(interp,"  GetAlignmentTransform\n",NULL);
    Tcl_AppendResult(interp,"  SetAlignmentTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAlignmentTransformIndex\n",NULL);
    Tcl_AppendResult(interp,"  SetAlignmentTransformIndex\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Inverse\n",NULL);
    Tcl_AppendResult(interp,"  MakeTransform\n",NULL);
    Tcl_AppendResult(interp,"  ComputeAllAngles\n",NULL);
    Tcl_AppendResult(interp,"  AdjustOriginToBeOutside\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOrigin\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetLocalZAxis\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetLocalXAxis\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  Identity\n",NULL);
    Tcl_AppendResult(interp,"  SetJointName\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetJointName\n",NULL);
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
    vtkAbstractTransformCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetParameters" );
    Tcl_DStringAppendElement ( &dString, "GetParameters" );
    Tcl_DStringAppendElement ( &dString, "GetDetailMode" );
    Tcl_DStringAppendElement ( &dString, "SetDetailMode" );
    Tcl_DStringAppendElement ( &dString, "GetDetailModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetDetailModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingMode" );
    Tcl_DStringAppendElement ( &dString, "SetSmoothingMode" );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingRadius" );
    Tcl_DStringAppendElement ( &dString, "SetSmoothingRadius" );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingRadiusMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingRadiusMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetZBlendingSize" );
    Tcl_DStringAppendElement ( &dString, "SetZBlendingSize" );
    Tcl_DStringAppendElement ( &dString, "GetZBlendingSizeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetZBlendingSizeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetBlendedMode" );
    Tcl_DStringAppendElement ( &dString, "SetBlendedMode" );
    Tcl_DStringAppendElement ( &dString, "GetBlendedModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetBlendedModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetSeparateZRotation" );
    Tcl_DStringAppendElement ( &dString, "SetSeparateZRotation" );
    Tcl_DStringAppendElement ( &dString, "GetSeparateZRotationMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSeparateZRotationMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetBlendingWindow" );
    Tcl_DStringAppendElement ( &dString, "SetBlendingWindow" );
    Tcl_DStringAppendElement ( &dString, "GetBlendingWindowMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetBlendingWindowMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetCubicBlending" );
    Tcl_DStringAppendElement ( &dString, "SetCubicBlending" );
    Tcl_DStringAppendElement ( &dString, "GetCubicBlendingMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetCubicBlendingMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetActive" );
    Tcl_DStringAppendElement ( &dString, "SetActive" );
    Tcl_DStringAppendElement ( &dString, "GetMinimumSectorAngle" );
    Tcl_DStringAppendElement ( &dString, "SetMinimumSectorAngle" );
    Tcl_DStringAppendElement ( &dString, "GetMinimumSectorAngleMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMinimumSectorAngleMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetRegionLabelImage" );
    Tcl_DStringAppendElement ( &dString, "ExternalSetRegionLabelImage" );
    Tcl_DStringAppendElement ( &dString, "GetRegionLabelIndex" );
    Tcl_DStringAppendElement ( &dString, "SetRegionLabelIndex" );
    Tcl_DStringAppendElement ( &dString, "GetChildRegionLabelIndex" );
    Tcl_DStringAppendElement ( &dString, "SetChildRegionLabelIndex" );
    Tcl_DStringAppendElement ( &dString, "GetChildRegionLabelIndex2" );
    Tcl_DStringAppendElement ( &dString, "SetChildRegionLabelIndex2" );
    Tcl_DStringAppendElement ( &dString, "GetSimilarityTransform" );
    Tcl_DStringAppendElement ( &dString, "GetZSimilarityTransform" );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationXYOnly" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationOnly" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationScale" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationTranslation" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationScaleTranslation" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationOrigin" );
    Tcl_DStringAppendElement ( &dString, "SetModeToFixedParameters" );
    Tcl_DStringAppendElement ( &dString, "SetMode" );
    Tcl_DStringAppendElement ( &dString, "GetMode" );
    Tcl_DStringAppendElement ( &dString, "Put" );
    Tcl_DStringAppendElement ( &dString, "Set" );
    Tcl_DStringAppendElement ( &dString, "Get" );
    Tcl_DStringAppendElement ( &dString, "SetFreezeUpdates" );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdates" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfParameters" );
    Tcl_DStringAppendElement ( &dString, "GetAlignmentTransform" );
    Tcl_DStringAppendElement ( &dString, "SetAlignmentTransform" );
    Tcl_DStringAppendElement ( &dString, "GetAlignmentTransformIndex" );
    Tcl_DStringAppendElement ( &dString, "SetAlignmentTransformIndex" );
    Tcl_DStringAppendElement ( &dString, "Inverse" );
    Tcl_DStringAppendElement ( &dString, "MakeTransform" );
    Tcl_DStringAppendElement ( &dString, "ComputeAllAngles" );
    Tcl_DStringAppendElement ( &dString, "AdjustOriginToBeOutside" );
    Tcl_DStringAppendElement ( &dString, "GetOrigin" );
    Tcl_DStringAppendElement ( &dString, "GetLocalZAxis" );
    Tcl_DStringAppendElement ( &dString, "GetLocalXAxis" );
    Tcl_DStringAppendElement ( &dString, "Identity" );
    Tcl_DStringAppendElement ( &dString, "SetJointName" );
    Tcl_DStringAppendElement ( &dString, "GetJointName" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkAbstractTransformCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxArticulatedJointTransform *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetParameters */
    if ( strcmp ( argv[2], "SetParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetParameters */
    Tcl_DStringAppendElement ( &dString, " Set the parameters from array\n This is key for Optimization Classes \n" );
    Tcl_DStringAppendElement ( &dString, "void SetParameters(vtkDoubleArray *parameters, int InputOffset=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetParameters */
    if ( strcmp ( argv[2], "GetParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetParameters */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void GetParameters(vtkDoubleArray *parameters, int InputOffset=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDetailMode */
    if ( strcmp ( argv[2], "GetDetailMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDetailMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDetailMode */
    Tcl_DStringAppendElement ( &dString, " Detail Mode , lots of fprintf statements\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDetailMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetDetailMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDetailMode */
    if ( strcmp ( argv[2], "SetDetailMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDetailMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDetailMode */
    Tcl_DStringAppendElement ( &dString, " Detail Mode , lots of fprintf statements\n" );
    Tcl_DStringAppendElement ( &dString, "void SetDetailMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetDetailMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDetailModeMinValue */
    if ( strcmp ( argv[2], "GetDetailModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDetailModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDetailModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Detail Mode , lots of fprintf statements\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDetailModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetDetailModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDetailModeMaxValue */
    if ( strcmp ( argv[2], "GetDetailModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDetailModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDetailModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Detail Mode , lots of fprintf statements\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDetailModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetDetailModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSmoothingMode */
    if ( strcmp ( argv[2], "GetSmoothingMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSmoothingMode */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSmoothingMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetSmoothingMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSmoothingMode */
    if ( strcmp ( argv[2], "SetSmoothingMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSmoothingMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSmoothingMode */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSmoothingMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetSmoothingMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSmoothingModeMinValue */
    if ( strcmp ( argv[2], "GetSmoothingModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSmoothingModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSmoothingModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetSmoothingModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSmoothingModeMaxValue */
    if ( strcmp ( argv[2], "GetSmoothingModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSmoothingModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSmoothingModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetSmoothingModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSmoothingRadius */
    if ( strcmp ( argv[2], "GetSmoothingRadius" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingRadius" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSmoothingRadius */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSmoothingRadius();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetSmoothingRadius */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSmoothingRadius */
    if ( strcmp ( argv[2], "SetSmoothingRadius" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSmoothingRadius" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSmoothingRadius */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSmoothingRadius(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetSmoothingRadius */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSmoothingRadiusMinValue */
    if ( strcmp ( argv[2], "GetSmoothingRadiusMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingRadiusMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSmoothingRadiusMinValue */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSmoothingRadiusMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetSmoothingRadiusMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSmoothingRadiusMaxValue */
    if ( strcmp ( argv[2], "GetSmoothingRadiusMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSmoothingRadiusMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSmoothingRadiusMaxValue */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSmoothingRadiusMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetSmoothingRadiusMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetZBlendingSize */
    if ( strcmp ( argv[2], "GetZBlendingSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetZBlendingSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetZBlendingSize */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "float GetZBlendingSize();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetZBlendingSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetZBlendingSize */
    if ( strcmp ( argv[2], "SetZBlendingSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetZBlendingSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetZBlendingSize */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "void SetZBlendingSize(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetZBlendingSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetZBlendingSizeMinValue */
    if ( strcmp ( argv[2], "GetZBlendingSizeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetZBlendingSizeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetZBlendingSizeMinValue */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "float GetZBlendingSizeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetZBlendingSizeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetZBlendingSizeMaxValue */
    if ( strcmp ( argv[2], "GetZBlendingSizeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetZBlendingSizeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetZBlendingSizeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Smooth Rotation Angle across r,z\n" );
    Tcl_DStringAppendElement ( &dString, "float GetZBlendingSizeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetZBlendingSizeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBlendedMode */
    if ( strcmp ( argv[2], "GetBlendedMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBlendedMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBlendedMode */
    Tcl_DStringAppendElement ( &dString, " 0 = No Blending\n 1 = Avoid Folding\n 2 = Smooth Wake + Avoid Folding\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBlendedMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetBlendedMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBlendedMode */
    if ( strcmp ( argv[2], "SetBlendedMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBlendedMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBlendedMode */
    Tcl_DStringAppendElement ( &dString, " 0 = No Blending\n 1 = Avoid Folding\n 2 = Smooth Wake + Avoid Folding\n" );
    Tcl_DStringAppendElement ( &dString, "void SetBlendedMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetBlendedMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBlendedModeMinValue */
    if ( strcmp ( argv[2], "GetBlendedModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBlendedModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBlendedModeMinValue */
    Tcl_DStringAppendElement ( &dString, " 0 = No Blending\n 1 = Avoid Folding\n 2 = Smooth Wake + Avoid Folding\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBlendedModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetBlendedModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBlendedModeMaxValue */
    if ( strcmp ( argv[2], "GetBlendedModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBlendedModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBlendedModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " 0 = No Blending\n 1 = Avoid Folding\n 2 = Smooth Wake + Avoid Folding\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBlendedModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetBlendedModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSeparateZRotation */
    if ( strcmp ( argv[2], "GetSeparateZRotation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSeparateZRotation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSeparateZRotation */
    Tcl_DStringAppendElement ( &dString, " SeparateZRotation\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSeparateZRotation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetSeparateZRotation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSeparateZRotation */
    if ( strcmp ( argv[2], "SetSeparateZRotation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSeparateZRotation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSeparateZRotation */
    Tcl_DStringAppendElement ( &dString, " SeparateZRotation\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSeparateZRotation(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetSeparateZRotation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSeparateZRotationMinValue */
    if ( strcmp ( argv[2], "GetSeparateZRotationMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSeparateZRotationMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSeparateZRotationMinValue */
    Tcl_DStringAppendElement ( &dString, " SeparateZRotation\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSeparateZRotationMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetSeparateZRotationMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSeparateZRotationMaxValue */
    if ( strcmp ( argv[2], "GetSeparateZRotationMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSeparateZRotationMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSeparateZRotationMaxValue */
    Tcl_DStringAppendElement ( &dString, " SeparateZRotation\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSeparateZRotationMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetSeparateZRotationMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBlendingWindow */
    if ( strcmp ( argv[2], "GetBlendingWindow" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBlendingWindow" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBlendingWindow */
    Tcl_DStringAppendElement ( &dString, " 0 = No Blending\n 1 = Avoid Folding\n 2 = Smooth Wake + Avoid Folding\n" );
    Tcl_DStringAppendElement ( &dString, "float GetBlendingWindow();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetBlendingWindow */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBlendingWindow */
    if ( strcmp ( argv[2], "SetBlendingWindow" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBlendingWindow" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBlendingWindow */
    Tcl_DStringAppendElement ( &dString, " 0 = No Blending\n 1 = Avoid Folding\n 2 = Smooth Wake + Avoid Folding\n" );
    Tcl_DStringAppendElement ( &dString, "void SetBlendingWindow(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetBlendingWindow */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBlendingWindowMinValue */
    if ( strcmp ( argv[2], "GetBlendingWindowMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBlendingWindowMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBlendingWindowMinValue */
    Tcl_DStringAppendElement ( &dString, " 0 = No Blending\n 1 = Avoid Folding\n 2 = Smooth Wake + Avoid Folding\n" );
    Tcl_DStringAppendElement ( &dString, "float GetBlendingWindowMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetBlendingWindowMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBlendingWindowMaxValue */
    if ( strcmp ( argv[2], "GetBlendingWindowMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBlendingWindowMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBlendingWindowMaxValue */
    Tcl_DStringAppendElement ( &dString, " 0 = No Blending\n 1 = Avoid Folding\n 2 = Smooth Wake + Avoid Folding\n" );
    Tcl_DStringAppendElement ( &dString, "float GetBlendingWindowMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetBlendingWindowMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCubicBlending */
    if ( strcmp ( argv[2], "GetCubicBlending" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCubicBlending" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCubicBlending */
    Tcl_DStringAppendElement ( &dString, " Cubic Blending fits cubic polynomial for $C^1$ continuity, otherwise linear\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCubicBlending();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetCubicBlending */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCubicBlending */
    if ( strcmp ( argv[2], "SetCubicBlending" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCubicBlending" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCubicBlending */
    Tcl_DStringAppendElement ( &dString, " Cubic Blending fits cubic polynomial for $C^1$ continuity, otherwise linear\n" );
    Tcl_DStringAppendElement ( &dString, "void SetCubicBlending(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetCubicBlending */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCubicBlendingMinValue */
    if ( strcmp ( argv[2], "GetCubicBlendingMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCubicBlendingMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCubicBlendingMinValue */
    Tcl_DStringAppendElement ( &dString, " Cubic Blending fits cubic polynomial for $C^1$ continuity, otherwise linear\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCubicBlendingMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetCubicBlendingMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCubicBlendingMaxValue */
    if ( strcmp ( argv[2], "GetCubicBlendingMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCubicBlendingMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCubicBlendingMaxValue */
    Tcl_DStringAppendElement ( &dString, " Cubic Blending fits cubic polynomial for $C^1$ continuity, otherwise linear\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCubicBlendingMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetCubicBlendingMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetActive */
    if ( strcmp ( argv[2], "GetActive" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetActive" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetActive */
    Tcl_DStringAppendElement ( &dString, " Active = 0 is fixed parameters, 1 = enabled\n" );
    Tcl_DStringAppendElement ( &dString, "int GetActive();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetActive */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetActive */
    if ( strcmp ( argv[2], "SetActive" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetActive" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetActive */
    Tcl_DStringAppendElement ( &dString, " Active = 0 is fixed parameters, 1 = enabled\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetActive(int active);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetActive */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinimumSectorAngle */
    if ( strcmp ( argv[2], "GetMinimumSectorAngle" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinimumSectorAngle" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinimumSectorAngle */
    Tcl_DStringAppendElement ( &dString, " Fast Interface Search\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMinimumSectorAngle();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetMinimumSectorAngle */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMinimumSectorAngle */
    if ( strcmp ( argv[2], "SetMinimumSectorAngle" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMinimumSectorAngle" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMinimumSectorAngle */
    Tcl_DStringAppendElement ( &dString, " Fast Interface Search\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMinimumSectorAngle(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetMinimumSectorAngle */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinimumSectorAngleMinValue */
    if ( strcmp ( argv[2], "GetMinimumSectorAngleMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinimumSectorAngleMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinimumSectorAngleMinValue */
    Tcl_DStringAppendElement ( &dString, " Fast Interface Search\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMinimumSectorAngleMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetMinimumSectorAngleMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinimumSectorAngleMaxValue */
    if ( strcmp ( argv[2], "GetMinimumSectorAngleMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinimumSectorAngleMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinimumSectorAngleMaxValue */
    Tcl_DStringAppendElement ( &dString, " Fast Interface Search\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMinimumSectorAngleMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetMinimumSectorAngleMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRegionLabelImage */
    if ( strcmp ( argv[2], "GetRegionLabelImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRegionLabelImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRegionLabelImage */
    Tcl_DStringAppendElement ( &dString, " RegionLabelImage\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetRegionLabelImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetRegionLabelImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ExternalSetRegionLabelImage */
    if ( strcmp ( argv[2], "ExternalSetRegionLabelImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ExternalSetRegionLabelImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ExternalSetRegionLabelImage */
    Tcl_DStringAppendElement ( &dString, " RegionLabelImage\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void ExternalSetRegionLabelImage(vtkImageData *img);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for ExternalSetRegionLabelImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRegionLabelIndex */
    if ( strcmp ( argv[2], "GetRegionLabelIndex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRegionLabelIndex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRegionLabelIndex */
    Tcl_DStringAppendElement ( &dString, " Index in Region\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRegionLabelIndex();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetRegionLabelIndex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRegionLabelIndex */
    if ( strcmp ( argv[2], "SetRegionLabelIndex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRegionLabelIndex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRegionLabelIndex */
    Tcl_DStringAppendElement ( &dString, " Index in Region\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRegionLabelIndex(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetRegionLabelIndex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetChildRegionLabelIndex */
    if ( strcmp ( argv[2], "GetChildRegionLabelIndex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetChildRegionLabelIndex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetChildRegionLabelIndex */
    Tcl_DStringAppendElement ( &dString, " Child Index in Region\n If ChildRegionLabelIndex == 1000 then all regions > RegionLabelIndex are children by default, in this case ChildRegionLabelIndex2 is ignored!\n" );
    Tcl_DStringAppendElement ( &dString, "int GetChildRegionLabelIndex();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetChildRegionLabelIndex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetChildRegionLabelIndex */
    if ( strcmp ( argv[2], "SetChildRegionLabelIndex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetChildRegionLabelIndex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetChildRegionLabelIndex */
    Tcl_DStringAppendElement ( &dString, " Child Index in Region\n If ChildRegionLabelIndex == 1000 then all regions > RegionLabelIndex are children by default, in this case ChildRegionLabelIndex2 is ignored!\n" );
    Tcl_DStringAppendElement ( &dString, "void SetChildRegionLabelIndex(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetChildRegionLabelIndex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetChildRegionLabelIndex2 */
    if ( strcmp ( argv[2], "GetChildRegionLabelIndex2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetChildRegionLabelIndex2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetChildRegionLabelIndex2 */
    Tcl_DStringAppendElement ( &dString, " Index in Region\n" );
    Tcl_DStringAppendElement ( &dString, "int GetChildRegionLabelIndex2();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetChildRegionLabelIndex2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetChildRegionLabelIndex2 */
    if ( strcmp ( argv[2], "SetChildRegionLabelIndex2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetChildRegionLabelIndex2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetChildRegionLabelIndex2 */
    Tcl_DStringAppendElement ( &dString, " Index in Region\n" );
    Tcl_DStringAppendElement ( &dString, "void SetChildRegionLabelIndex2(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetChildRegionLabelIndex2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSimilarityTransform */
    if ( strcmp ( argv[2], "GetSimilarityTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSimilarityTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSimilarityTransform */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform *GetSimilarityTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetSimilarityTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetZSimilarityTransform */
    if ( strcmp ( argv[2], "GetZSimilarityTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetZSimilarityTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetZSimilarityTransform */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform *GetZSimilarityTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetZSimilarityTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Copy */
    if ( strcmp ( argv[2], "Copy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Copy */
    Tcl_DStringAppendElement ( &dString, " Copy\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Copy(vtkpxArticulatedJointTransform *other);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for Copy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationXYOnly */
    if ( strcmp ( argv[2], "SetModeToRotationXYOnly" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationXYOnly" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationXYOnly */
    Tcl_DStringAppendElement ( &dString, " Limit Modes to only applicable ones (Nothing, \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationXYOnly();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetModeToRotationXYOnly */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationOnly */
    if ( strcmp ( argv[2], "SetModeToRotationOnly" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationOnly" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationOnly */
    Tcl_DStringAppendElement ( &dString, " Limit Modes to only applicable ones (Nothing, \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationOnly();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetModeToRotationOnly */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationScale */
    if ( strcmp ( argv[2], "SetModeToRotationScale" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationScale" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationScale */
    Tcl_DStringAppendElement ( &dString, " Limit Modes to only applicable ones (Nothing, \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationScale();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetModeToRotationScale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationTranslation */
    if ( strcmp ( argv[2], "SetModeToRotationTranslation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationTranslation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationTranslation */
    Tcl_DStringAppendElement ( &dString, " Limit Modes to only applicable ones (Nothing, \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationTranslation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetModeToRotationTranslation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationScaleTranslation */
    if ( strcmp ( argv[2], "SetModeToRotationScaleTranslation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationScaleTranslation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationScaleTranslation */
    Tcl_DStringAppendElement ( &dString, " Limit Modes to only applicable ones (Nothing, \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationScaleTranslation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetModeToRotationScaleTranslation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationOrigin */
    if ( strcmp ( argv[2], "SetModeToRotationOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationOrigin */
    Tcl_DStringAppendElement ( &dString, " Limit Modes to only applicable ones (Nothing, \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationOrigin();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetModeToRotationOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToFixedParameters */
    if ( strcmp ( argv[2], "SetModeToFixedParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToFixedParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToFixedParameters */
    Tcl_DStringAppendElement ( &dString, " Limit Modes to only applicable ones (Nothing, \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToFixedParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetModeToFixedParameters */

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
    Tcl_DStringAppendElement ( &dString, " Limit Modes to only applicable ones (Nothing, \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMode(int m);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetMode */

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
    Tcl_DStringAppendElement ( &dString, " Limit Modes to only applicable ones (Nothing, \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Put */
    if ( strcmp ( argv[2], "Put" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Put" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Put */
    Tcl_DStringAppendElement ( &dString, " Puts a transformation parameter (transformation matrix is updated)\n" );
    Tcl_DStringAppendElement ( &dString, "void Put(int a, float v);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for Put */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Set */
    if ( strcmp ( argv[2], "Set" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Set" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Set */
    Tcl_DStringAppendElement ( &dString, " Puts a transformation parameter (transformation matrix is updated)\n" );
    Tcl_DStringAppendElement ( &dString, "void Set(int a, float v);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for Set */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Get */
    if ( strcmp ( argv[2], "Get" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Get" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Get */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float Get(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for Get */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFreezeUpdates */
    if ( strcmp ( argv[2], "SetFreezeUpdates" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFreezeUpdates" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFreezeUpdates */
    Tcl_DStringAppendElement ( &dString, " Stop Auto Updates\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetFreezeUpdates(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetFreezeUpdates */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFreezeUpdates */
    if ( strcmp ( argv[2], "GetFreezeUpdates" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdates" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFreezeUpdates */
    Tcl_DStringAppendElement ( &dString, " Stop Auto Updates\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetFreezeUpdates();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetFreezeUpdates */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Load */
    if ( strcmp ( argv[2], "Load" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Load" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Load */
    Tcl_DStringAppendElement ( &dString, " Load / Save Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Load(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for Load */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Save */
    if ( strcmp ( argv[2], "Save" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Save" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Save */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int Save(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for Save */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfParameters */
    if ( strcmp ( argv[2], "GetNumberOfParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfParameters */
    Tcl_DStringAppendElement ( &dString, " Number Of Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumberOfParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetNumberOfParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAlignmentTransform */
    if ( strcmp ( argv[2], "GetAlignmentTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAlignmentTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAlignmentTransform */
    Tcl_DStringAppendElement ( &dString, " Set/Get Alignment Transform\n" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform *GetAlignmentTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetAlignmentTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAlignmentTransform */
    if ( strcmp ( argv[2], "SetAlignmentTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAlignmentTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAlignmentTransform */
    Tcl_DStringAppendElement ( &dString, " Set/Get Alignment Transform\n" );
    Tcl_DStringAppendElement ( &dString, "void SetAlignmentTransform(vtkAbstractTransform *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetAlignmentTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAlignmentTransformIndex */
    if ( strcmp ( argv[2], "GetAlignmentTransformIndex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAlignmentTransformIndex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAlignmentTransformIndex */
    Tcl_DStringAppendElement ( &dString, " Index in Region\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAlignmentTransformIndex();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetAlignmentTransformIndex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAlignmentTransformIndex */
    if ( strcmp ( argv[2], "SetAlignmentTransformIndex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAlignmentTransformIndex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAlignmentTransformIndex */
    Tcl_DStringAppendElement ( &dString, " Index in Region\n" );
    Tcl_DStringAppendElement ( &dString, "void SetAlignmentTransformIndex(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetAlignmentTransformIndex */

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
    Tcl_DStringAppendElement ( &dString, " Inverse\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Inverse();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for Inverse */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MakeTransform */
    if ( strcmp ( argv[2], "MakeTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MakeTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MakeTransform */
    Tcl_DStringAppendElement ( &dString, " Make another transform of the same type.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform *MakeTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for MakeTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeAllAngles */
    if ( strcmp ( argv[2], "ComputeAllAngles" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeAllAngles" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeAllAngles */
    Tcl_DStringAppendElement ( &dString, " Compute All Angles \n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *ComputeAllAngles();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for ComputeAllAngles */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AdjustOriginToBeOutside */
    if ( strcmp ( argv[2], "AdjustOriginToBeOutside" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AdjustOriginToBeOutside" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AdjustOriginToBeOutside */
    Tcl_DStringAppendElement ( &dString, " Adjust Origin to be outside \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void AdjustOriginToBeOutside(double gap);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for AdjustOriginToBeOutside */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOrigin */
    if ( strcmp ( argv[2], "GetOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOrigin */
    Tcl_DStringAppendElement ( &dString, " Local Z Axis\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void GetOrigin(float x\\[3\\]);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLocalZAxis */
    if ( strcmp ( argv[2], "GetLocalZAxis" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLocalZAxis" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLocalZAxis */
    Tcl_DStringAppendElement ( &dString, " Local Z Axis\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void GetLocalZAxis(float z\\[3\\]);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetLocalZAxis */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLocalXAxis */
    if ( strcmp ( argv[2], "GetLocalXAxis" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLocalXAxis" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLocalXAxis */
    Tcl_DStringAppendElement ( &dString, " Local Z Axis\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void GetLocalXAxis(float x\\[3\\]);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetLocalXAxis */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Identity */
    if ( strcmp ( argv[2], "Identity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Identity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Identity */
    Tcl_DStringAppendElement ( &dString, " Identity\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Identity();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for Identity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetJointName */
    if ( strcmp ( argv[2], "SetJointName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetJointName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetJointName */
    Tcl_DStringAppendElement ( &dString, " Specify name of the joint\n" );
    Tcl_DStringAppendElement ( &dString, "void SetJointName(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for SetJointName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetJointName */
    if ( strcmp ( argv[2], "GetJointName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetJointName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetJointName */
    Tcl_DStringAppendElement ( &dString, " Specify name of the joint\n" );
    Tcl_DStringAppendElement ( &dString, "char *GetJointName();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    /* Closing for GetJointName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkAbstractTransformCppCommand(static_cast<vtkAbstractTransform *>(op),interp,argc,argv) == TCL_OK)
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
