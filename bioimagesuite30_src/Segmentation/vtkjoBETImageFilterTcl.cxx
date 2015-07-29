// tcl wrapper for vtkjoBETImageFilter object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkjoBETImageFilter.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkjoBETImageFilterNewCommand()
{
  vtkjoBETImageFilter *temp = vtkjoBETImageFilter::New();
  return static_cast<ClientData>(temp);
}

int vtkSimpleImageToImageFilterCppCommand(vtkSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkjoBETImageFilterCppCommand(vtkjoBETImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkjoBETImageFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkjoBETImageFilterCppCommand(static_cast<vtkjoBETImageFilter *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkjoBETImageFilterCppCommand(vtkjoBETImageFilter *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkjoBETImageFilter",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkSimpleImageToImageFilterCppCommand(static_cast<vtkSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkSimpleImageToImageFilter"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkjoBETImageFilter  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkjoBETImageFilter");
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
    vtkjoBETImageFilter  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkjoBETImageFilter");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkjoBETImageFilter  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkjoBETImageFilter");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetSurfaceInput",argv[1]))&&(argc == 3))
    {
    vtkPolyData  *temp0;
    error = 0;

    temp0 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPolyData"),interp,error));
    if (!error)
    {
    op->SetSurfaceInput(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSurface",argv[1]))&&(argc == 2))
    {
    vtkPolyData  *temp20;
    temp20 = (op)->GetSurface();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
  if ((!strcmp("GetOutputMask",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetOutputMask();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetMinIntensityPercent",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMinIntensityPercent(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMinIntensityPercentMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinIntensityPercentMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinIntensityPercentMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinIntensityPercentMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinIntensityPercent",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinIntensityPercent();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMaxIntensityPercent",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMaxIntensityPercent(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaxIntensityPercentMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxIntensityPercentMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxIntensityPercentMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxIntensityPercentMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxIntensityPercent",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxIntensityPercent();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinIntensityThresh",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinIntensityThresh();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxIntensityThresh",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxIntensityThresh();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetThreshold",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetThreshold();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetBins",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetBins(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBinsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBinsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBinsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBinsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBins",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBins();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCenterOfGravity",argv[1]))&&(argc == 2))
    {
    double  *temp20;
    temp20 = (op)->GetCenterOfGravity();
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
  if ((!strcmp("SetCenterOfGravity",argv[1]))&&(argc == 5))
    {
    double   temp0;
    double   temp1;
    double   temp2;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->SetCenterOfGravity(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetCenterOfGravity",argv[1]))&&(argc == 5))
    {
    double temp0[3];
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0[0] = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp0[1] = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp0[2] = tempd;
    if (!error)
    {
    op->SetCenterOfGravity(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetRadius",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetRadius(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRadius",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetRadius();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMedianIntensity",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMedianIntensity();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetManualRadiusAndCOG",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetManualRadiusAndCOG(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetManualRadiusAndCOGMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetManualRadiusAndCOGMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetManualRadiusAndCOGMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetManualRadiusAndCOGMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetManualRadiusAndCOG",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetManualRadiusAndCOG();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ManualRadiusAndCOGOn",argv[1]))&&(argc == 2))
    {
    op->ManualRadiusAndCOGOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ManualRadiusAndCOGOff",argv[1]))&&(argc == 2))
    {
    op->ManualRadiusAndCOGOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetRadiusScaleFactor",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetRadiusScaleFactor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetSurfaceResolution",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetSurfaceResolution(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSurfaceResolutionMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSurfaceResolutionMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSurfaceResolutionMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSurfaceResolutionMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMaxIterations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMaxIterations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetMaxPasses",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMaxPasses(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetMinChangeTolerance",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMinChangeTolerance(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetMinIntensitySearchDist",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMinIntensitySearchDist(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMinIntensitySearchDist",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinIntensitySearchDist();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMaxIntensitySearchDist",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMaxIntensitySearchDist(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaxIntensitySearchDist",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxIntensitySearchDist();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetLocalThresholdConst",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLocalThresholdConst(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLocalThresholdConstMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLocalThresholdConstMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLocalThresholdConstMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLocalThresholdConstMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLocalThresholdConst",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLocalThresholdConst();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRMin",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetRMin(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetRMax",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetRMax(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetIncreasedSmoothing",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetIncreasedSmoothing(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetIncreasedSmoothing",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIncreasedSmoothing();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("IncreasedSmoothingOn",argv[1]))&&(argc == 2))
    {
    op->IncreasedSmoothingOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("IncreasedSmoothingOff",argv[1]))&&(argc == 2))
    {
    op->IncreasedSmoothingOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSelfIntersectionThreshold",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetSelfIntersectionThreshold(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkjoBETImageFilterCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkjoBETImageFilter:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetSurfaceInput\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSurface\n",NULL);
    Tcl_AppendResult(interp,"  GetOutputMask\n",NULL);
    Tcl_AppendResult(interp,"  SetMinIntensityPercent\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMinIntensityPercentMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMinIntensityPercentMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMinIntensityPercent\n",NULL);
    Tcl_AppendResult(interp,"  SetMaxIntensityPercent\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxIntensityPercentMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxIntensityPercentMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxIntensityPercent\n",NULL);
    Tcl_AppendResult(interp,"  GetMinIntensityThresh\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxIntensityThresh\n",NULL);
    Tcl_AppendResult(interp,"  GetThreshold\n",NULL);
    Tcl_AppendResult(interp,"  SetBins\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBinsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBinsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBins\n",NULL);
    Tcl_AppendResult(interp,"  GetCenterOfGravity\n",NULL);
    Tcl_AppendResult(interp,"  SetCenterOfGravity\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  SetCenterOfGravity\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  SetRadius\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRadius\n",NULL);
    Tcl_AppendResult(interp,"  GetMedianIntensity\n",NULL);
    Tcl_AppendResult(interp,"  SetManualRadiusAndCOG\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetManualRadiusAndCOGMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetManualRadiusAndCOGMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetManualRadiusAndCOG\n",NULL);
    Tcl_AppendResult(interp,"  ManualRadiusAndCOGOn\n",NULL);
    Tcl_AppendResult(interp,"  ManualRadiusAndCOGOff\n",NULL);
    Tcl_AppendResult(interp,"  SetRadiusScaleFactor\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetSurfaceResolution\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSurfaceResolutionMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSurfaceResolutionMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetMaxIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetMaxPasses\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetMinChangeTolerance\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetMinIntensitySearchDist\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMinIntensitySearchDist\n",NULL);
    Tcl_AppendResult(interp,"  SetMaxIntensitySearchDist\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxIntensitySearchDist\n",NULL);
    Tcl_AppendResult(interp,"  SetLocalThresholdConst\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLocalThresholdConstMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLocalThresholdConstMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLocalThresholdConst\n",NULL);
    Tcl_AppendResult(interp,"  SetRMin\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetRMax\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetIncreasedSmoothing\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetIncreasedSmoothing\n",NULL);
    Tcl_AppendResult(interp,"  IncreasedSmoothingOn\n",NULL);
    Tcl_AppendResult(interp,"  IncreasedSmoothingOff\n",NULL);
    Tcl_AppendResult(interp,"  SetSelfIntersectionThreshold\t with 1 arg\n",NULL);
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
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetSurfaceInput" );
    Tcl_DStringAppendElement ( &dString, "GetSurface" );
    Tcl_DStringAppendElement ( &dString, "GetOutputMask" );
    Tcl_DStringAppendElement ( &dString, "SetMinIntensityPercent" );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercentMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercentMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercent" );
    Tcl_DStringAppendElement ( &dString, "SetMaxIntensityPercent" );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercentMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercentMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercent" );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityThresh" );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityThresh" );
    Tcl_DStringAppendElement ( &dString, "GetThreshold" );
    Tcl_DStringAppendElement ( &dString, "SetBins" );
    Tcl_DStringAppendElement ( &dString, "GetBinsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetBinsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetBins" );
    Tcl_DStringAppendElement ( &dString, "GetCenterOfGravity" );
    Tcl_DStringAppendElement ( &dString, "SetCenterOfGravity" );
    Tcl_DStringAppendElement ( &dString, "SetCenterOfGravity" );
    Tcl_DStringAppendElement ( &dString, "SetRadius" );
    Tcl_DStringAppendElement ( &dString, "GetRadius" );
    Tcl_DStringAppendElement ( &dString, "GetMedianIntensity" );
    Tcl_DStringAppendElement ( &dString, "SetManualRadiusAndCOG" );
    Tcl_DStringAppendElement ( &dString, "GetManualRadiusAndCOGMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetManualRadiusAndCOGMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetManualRadiusAndCOG" );
    Tcl_DStringAppendElement ( &dString, "ManualRadiusAndCOGOn" );
    Tcl_DStringAppendElement ( &dString, "ManualRadiusAndCOGOff" );
    Tcl_DStringAppendElement ( &dString, "SetRadiusScaleFactor" );
    Tcl_DStringAppendElement ( &dString, "SetSurfaceResolution" );
    Tcl_DStringAppendElement ( &dString, "GetSurfaceResolutionMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSurfaceResolutionMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetMaxIterations" );
    Tcl_DStringAppendElement ( &dString, "SetMaxPasses" );
    Tcl_DStringAppendElement ( &dString, "SetMinChangeTolerance" );
    Tcl_DStringAppendElement ( &dString, "SetMinIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "SetMaxIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "SetLocalThresholdConst" );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConstMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConstMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConst" );
    Tcl_DStringAppendElement ( &dString, "SetRMin" );
    Tcl_DStringAppendElement ( &dString, "SetRMax" );
    Tcl_DStringAppendElement ( &dString, "SetIncreasedSmoothing" );
    Tcl_DStringAppendElement ( &dString, "GetIncreasedSmoothing" );
    Tcl_DStringAppendElement ( &dString, "IncreasedSmoothingOn" );
    Tcl_DStringAppendElement ( &dString, "IncreasedSmoothingOff" );
    Tcl_DStringAppendElement ( &dString, "SetSelfIntersectionThreshold" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkjoBETImageFilter *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSurfaceInput */
    if ( strcmp ( argv[2], "SetSurfaceInput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSurfaceInput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSurfaceInput */
    Tcl_DStringAppendElement ( &dString, " Set the initial surface geometry from which the BET\n algorithm will start running.  If no surface is provided here\n BET proceeds as normally by creating an initial sphere surface\n based on parameters found creating the histogram.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSurfaceInput(vtkPolyData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetSurfaceInput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSurface */
    if ( strcmp ( argv[2], "GetSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSurface */
    Tcl_DStringAppendElement ( &dString, " Returns the BET brain surface geometry mesh.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData *GetSurface();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutputMask */
    if ( strcmp ( argv[2], "GetOutputMask" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutputMask" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutputMask */
    Tcl_DStringAppendElement ( &dString, " Returns the binary image mask of the extracted brain.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetOutputMask();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetOutputMask */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMinIntensityPercent */
    if ( strcmp ( argv[2], "SetMinIntensityPercent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMinIntensityPercent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMinIntensityPercent */
    Tcl_DStringAppendElement ( &dString, " MinIntensityPercent - the lower percentage of the cumulative histogram\n used to determine MinIntensity.  We limit the minimum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.02.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMinIntensityPercent(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetMinIntensityPercent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinIntensityPercentMinValue */
    if ( strcmp ( argv[2], "GetMinIntensityPercentMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercentMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinIntensityPercentMinValue */
    Tcl_DStringAppendElement ( &dString, " MinIntensityPercent - the lower percentage of the cumulative histogram\n used to determine MinIntensity.  We limit the minimum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.02.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinIntensityPercentMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMinIntensityPercentMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinIntensityPercentMaxValue */
    if ( strcmp ( argv[2], "GetMinIntensityPercentMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercentMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinIntensityPercentMaxValue */
    Tcl_DStringAppendElement ( &dString, " MinIntensityPercent - the lower percentage of the cumulative histogram\n used to determine MinIntensity.  We limit the minimum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.02.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinIntensityPercentMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMinIntensityPercentMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinIntensityPercent */
    if ( strcmp ( argv[2], "GetMinIntensityPercent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinIntensityPercent */
    Tcl_DStringAppendElement ( &dString, " MinIntensityPercent - the lower percentage of the cumulative histogram\n used to determine MinIntensity.  We limit the minimum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.02.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinIntensityPercent();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMinIntensityPercent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaxIntensityPercent */
    if ( strcmp ( argv[2], "SetMaxIntensityPercent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaxIntensityPercent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaxIntensityPercent */
    Tcl_DStringAppendElement ( &dString, " MaxIntensityPercent - the upper percentage of the cumulative histogram\n used to determine MaxIntensity.  We limit the maximum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.98.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaxIntensityPercent(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetMaxIntensityPercent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxIntensityPercentMinValue */
    if ( strcmp ( argv[2], "GetMaxIntensityPercentMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercentMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxIntensityPercentMinValue */
    Tcl_DStringAppendElement ( &dString, " MaxIntensityPercent - the upper percentage of the cumulative histogram\n used to determine MaxIntensity.  We limit the maximum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.98.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxIntensityPercentMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMaxIntensityPercentMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxIntensityPercentMaxValue */
    if ( strcmp ( argv[2], "GetMaxIntensityPercentMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercentMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxIntensityPercentMaxValue */
    Tcl_DStringAppendElement ( &dString, " MaxIntensityPercent - the upper percentage of the cumulative histogram\n used to determine MaxIntensity.  We limit the maximum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.98.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxIntensityPercentMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMaxIntensityPercentMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxIntensityPercent */
    if ( strcmp ( argv[2], "GetMaxIntensityPercent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxIntensityPercent */
    Tcl_DStringAppendElement ( &dString, " MaxIntensityPercent - the upper percentage of the cumulative histogram\n used to determine MaxIntensity.  We limit the maximum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.98.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxIntensityPercent();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMaxIntensityPercent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinIntensityThresh */
    if ( strcmp ( argv[2], "GetMinIntensityThresh" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityThresh" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinIntensityThresh */
    Tcl_DStringAppendElement ( &dString, " MinIntensityThresh - the robust minimum intensity threshold below which\n lies MinIntensityPercent percent of the cumulative histogram.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinIntensityThresh();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMinIntensityThresh */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxIntensityThresh */
    if ( strcmp ( argv[2], "GetMaxIntensityThresh" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityThresh" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxIntensityThresh */
    Tcl_DStringAppendElement ( &dString, " MaxIntensityThresh - the robust maximum intensity threshold above which\n lies MaxIntensityPercent percent of the cumulative histogram.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxIntensityThresh();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMaxIntensityThresh */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetThreshold */
    if ( strcmp ( argv[2], "GetThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetThreshold */
    Tcl_DStringAppendElement ( &dString, " Threshold - set to be 10% of the way between MinIntensityThresh\n and MaxIntensityThresh.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetThreshold();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBins */
    if ( strcmp ( argv[2], "SetBins" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBins" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBins */
    Tcl_DStringAppendElement ( &dString, " Bins - the number of histogram bins.  We limit the number of bins to be\n between 1 and 1024.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetBins(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBinsMinValue */
    if ( strcmp ( argv[2], "GetBinsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBinsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBinsMinValue */
    Tcl_DStringAppendElement ( &dString, " Bins - the number of histogram bins.  We limit the number of bins to be\n between 1 and 1024.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBinsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetBinsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBinsMaxValue */
    if ( strcmp ( argv[2], "GetBinsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBinsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBinsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Bins - the number of histogram bins.  We limit the number of bins to be\n between 1 and 1024.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBinsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetBinsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBins */
    if ( strcmp ( argv[2], "GetBins" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBins" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBins */
    Tcl_DStringAppendElement ( &dString, " Bins - the number of histogram bins.  We limit the number of bins to be\n between 1 and 1024.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBins();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCenterOfGravity */
    if ( strcmp ( argv[2], "GetCenterOfGravity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCenterOfGravity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCenterOfGravity */
    Tcl_DStringAppendElement ( &dString, " Center Of Gravity - the center of gravity (COG) of the brain/head in the\n image.  Only voxels with intensity greater than Threshold and less than\n MaxIntensityThresh are used to compute this.\n" );
    Tcl_DStringAppendElement ( &dString, "double *GetCenterOfGravity();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetCenterOfGravity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCenterOfGravity */
    if ( strcmp ( argv[2], "SetCenterOfGravity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCenterOfGravity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCenterOfGravity */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetCenterOfGravity(double, double, double);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetCenterOfGravity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCenterOfGravity */
    if ( strcmp ( argv[2], "SetCenterOfGravity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCenterOfGravity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCenterOfGravity */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetCenterOfGravity(double a\\[3\\]);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetCenterOfGravity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRadius */
    if ( strcmp ( argv[2], "SetRadius" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRadius" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRadius */
    Tcl_DStringAppendElement ( &dString, " Radius - the estimated \"mean' radius of the brain/head volume.  All voxels\n with intensity greater than Threshold are counted.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRadius(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetRadius */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRadius */
    if ( strcmp ( argv[2], "GetRadius" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRadius" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRadius */
    Tcl_DStringAppendElement ( &dString, " Radius - the estimated \"mean' radius of the brain/head volume.  All voxels\n with intensity greater than Threshold are counted.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetRadius();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetRadius */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMedianIntensity */
    if ( strcmp ( argv[2], "GetMedianIntensity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMedianIntensity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMedianIntensity */
    Tcl_DStringAppendElement ( &dString, " MedianIntensity - the median intensity within a sphere of the estimated\n radius and centered on the COG.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMedianIntensity();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMedianIntensity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetManualRadiusAndCOG */
    if ( strcmp ( argv[2], "SetManualRadiusAndCOG" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetManualRadiusAndCOG" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetManualRadiusAndCOG */
    Tcl_DStringAppendElement ( &dString, " ManualRadiusAndCOG - the filter uses the radius and center of gravity\n manually specified by the user instead of those automatically found\n during image preprocessing.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetManualRadiusAndCOG(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetManualRadiusAndCOG */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetManualRadiusAndCOGMinValue */
    if ( strcmp ( argv[2], "GetManualRadiusAndCOGMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetManualRadiusAndCOGMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetManualRadiusAndCOGMinValue */
    Tcl_DStringAppendElement ( &dString, " ManualRadiusAndCOG - the filter uses the radius and center of gravity\n manually specified by the user instead of those automatically found\n during image preprocessing.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetManualRadiusAndCOGMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetManualRadiusAndCOGMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetManualRadiusAndCOGMaxValue */
    if ( strcmp ( argv[2], "GetManualRadiusAndCOGMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetManualRadiusAndCOGMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetManualRadiusAndCOGMaxValue */
    Tcl_DStringAppendElement ( &dString, " ManualRadiusAndCOG - the filter uses the radius and center of gravity\n manually specified by the user instead of those automatically found\n during image preprocessing.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetManualRadiusAndCOGMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetManualRadiusAndCOGMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetManualRadiusAndCOG */
    if ( strcmp ( argv[2], "GetManualRadiusAndCOG" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetManualRadiusAndCOG" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetManualRadiusAndCOG */
    Tcl_DStringAppendElement ( &dString, " ManualRadiusAndCOG - the filter uses the radius and center of gravity\n manually specified by the user instead of those automatically found\n during image preprocessing.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetManualRadiusAndCOG();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetManualRadiusAndCOG */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ManualRadiusAndCOGOn */
    if ( strcmp ( argv[2], "ManualRadiusAndCOGOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ManualRadiusAndCOGOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ManualRadiusAndCOGOn */
    Tcl_DStringAppendElement ( &dString, " ManualRadiusAndCOG - the filter uses the radius and center of gravity\n manually specified by the user instead of those automatically found\n during image preprocessing.\n" );
    Tcl_DStringAppendElement ( &dString, "void ManualRadiusAndCOGOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for ManualRadiusAndCOGOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ManualRadiusAndCOGOff */
    if ( strcmp ( argv[2], "ManualRadiusAndCOGOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ManualRadiusAndCOGOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ManualRadiusAndCOGOff */
    Tcl_DStringAppendElement ( &dString, " ManualRadiusAndCOG - the filter uses the radius and center of gravity\n manually specified by the user instead of those automatically found\n during image preprocessing.\n" );
    Tcl_DStringAppendElement ( &dString, "void ManualRadiusAndCOGOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for ManualRadiusAndCOGOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRadiusScaleFactor */
    if ( strcmp ( argv[2], "SetRadiusScaleFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRadiusScaleFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRadiusScaleFactor */
    Tcl_DStringAppendElement ( &dString, " RadiusScaleFactor - constant factor to scale the InitialRadius by.  Default\n value set to 0.5;\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRadiusScaleFactor(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetRadiusScaleFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSurfaceResolution */
    if ( strcmp ( argv[2], "SetSurfaceResolution" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSurfaceResolution" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSurfaceResolution */
    Tcl_DStringAppendElement ( &dString, " SurfaceResolution - sets the number of surface points to use.  This value\n represents the number of times an icosahedron's faces are subdivided into\n 4 triangles.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSurfaceResolution(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetSurfaceResolution */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSurfaceResolutionMinValue */
    if ( strcmp ( argv[2], "GetSurfaceResolutionMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSurfaceResolutionMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSurfaceResolutionMinValue */
    Tcl_DStringAppendElement ( &dString, " SurfaceResolution - sets the number of surface points to use.  This value\n represents the number of times an icosahedron's faces are subdivided into\n 4 triangles.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSurfaceResolutionMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetSurfaceResolutionMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSurfaceResolutionMaxValue */
    if ( strcmp ( argv[2], "GetSurfaceResolutionMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSurfaceResolutionMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSurfaceResolutionMaxValue */
    Tcl_DStringAppendElement ( &dString, " SurfaceResolution - sets the number of surface points to use.  This value\n represents the number of times an icosahedron's faces are subdivided into\n 4 triangles.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSurfaceResolutionMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetSurfaceResolutionMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaxIterations */
    if ( strcmp ( argv[2], "SetMaxIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaxIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaxIterations */
    Tcl_DStringAppendElement ( &dString, " MaxIterations - the maximum number of iterations for the algorithm\n to run.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaxIterations(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetMaxIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaxPasses */
    if ( strcmp ( argv[2], "SetMaxPasses" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaxPasses" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaxPasses */
    Tcl_DStringAppendElement ( &dString, " MaxPasses - the maximum number of passes for the algorithm\n to run.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaxPasses(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetMaxPasses */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMinChangeTolerance */
    if ( strcmp ( argv[2], "SetMinChangeTolerance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMinChangeTolerance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMinChangeTolerance */
    Tcl_DStringAppendElement ( &dString, " MinChangeTolerance - the minimum allowable maximum change in point distance\n shift before the algorithm stops.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMinChangeTolerance(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetMinChangeTolerance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMinIntensitySearchDist */
    if ( strcmp ( argv[2], "SetMinIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMinIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMinIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MinSearchDist - the distance searched for the minimum intensity\n from a surface point (d1 in the BET paper).  Default d1 = 7mm.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMinIntensitySearchDist(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetMinIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinIntensitySearchDist */
    if ( strcmp ( argv[2], "GetMinIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MinSearchDist - the distance searched for the minimum intensity\n from a surface point (d1 in the BET paper).  Default d1 = 7mm.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinIntensitySearchDist();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMinIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaxIntensitySearchDist */
    if ( strcmp ( argv[2], "SetMaxIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaxIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaxIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MaxSearchDist - the distance searched for the maximum intensity\n from a surface point (d2 in the BET paper).  Default d2 = 0.5*d1.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaxIntensitySearchDist(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetMaxIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxIntensitySearchDist */
    if ( strcmp ( argv[2], "GetMaxIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MaxSearchDist - the distance searched for the maximum intensity\n from a surface point (d2 in the BET paper).  Default d2 = 0.5*d1.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxIntensitySearchDist();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetMaxIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLocalThresholdConst */
    if ( strcmp ( argv[2], "SetLocalThresholdConst" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLocalThresholdConst" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLocalThresholdConst */
    Tcl_DStringAppendElement ( &dString, " LocalThresholdConst - fractional constant (b_t in the BET paper)\n used when computing the locally appropriate intensity threshold t_l.\n Default b_t = 0.5;\n" );
    Tcl_DStringAppendElement ( &dString, "void SetLocalThresholdConst(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetLocalThresholdConst */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLocalThresholdConstMinValue */
    if ( strcmp ( argv[2], "GetLocalThresholdConstMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConstMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLocalThresholdConstMinValue */
    Tcl_DStringAppendElement ( &dString, " LocalThresholdConst - fractional constant (b_t in the BET paper)\n used when computing the locally appropriate intensity threshold t_l.\n Default b_t = 0.5;\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLocalThresholdConstMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetLocalThresholdConstMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLocalThresholdConstMaxValue */
    if ( strcmp ( argv[2], "GetLocalThresholdConstMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConstMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLocalThresholdConstMaxValue */
    Tcl_DStringAppendElement ( &dString, " LocalThresholdConst - fractional constant (b_t in the BET paper)\n used when computing the locally appropriate intensity threshold t_l.\n Default b_t = 0.5;\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLocalThresholdConstMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetLocalThresholdConstMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLocalThresholdConst */
    if ( strcmp ( argv[2], "GetLocalThresholdConst" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConst" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLocalThresholdConst */
    Tcl_DStringAppendElement ( &dString, " LocalThresholdConst - fractional constant (b_t in the BET paper)\n used when computing the locally appropriate intensity threshold t_l.\n Default b_t = 0.5;\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLocalThresholdConst();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetLocalThresholdConst */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRMin */
    if ( strcmp ( argv[2], "SetRMin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRMin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRMin */
    Tcl_DStringAppendElement ( &dString, " RMin - the minimum local radius used for smoothing (default 3.33mm).\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRMin(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetRMin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRMax */
    if ( strcmp ( argv[2], "SetRMax" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRMax" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRMax */
    Tcl_DStringAppendElement ( &dString, " RMax - the maximum local radius used for smoothing (default 10mm).\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRMax(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetRMax */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetIncreasedSmoothing */
    if ( strcmp ( argv[2], "SetIncreasedSmoothing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetIncreasedSmoothing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetIncreasedSmoothing */
    Tcl_DStringAppendElement ( &dString, " IncreasedSmoothing - tells the algorithm to used increased smoothing.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetIncreasedSmoothing(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetIncreasedSmoothing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIncreasedSmoothing */
    if ( strcmp ( argv[2], "GetIncreasedSmoothing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIncreasedSmoothing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIncreasedSmoothing */
    Tcl_DStringAppendElement ( &dString, " IncreasedSmoothing - tells the algorithm to used increased smoothing.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIncreasedSmoothing();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for GetIncreasedSmoothing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IncreasedSmoothingOn */
    if ( strcmp ( argv[2], "IncreasedSmoothingOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IncreasedSmoothingOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IncreasedSmoothingOn */
    Tcl_DStringAppendElement ( &dString, " IncreasedSmoothing - tells the algorithm to used increased smoothing.\n" );
    Tcl_DStringAppendElement ( &dString, "void IncreasedSmoothingOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for IncreasedSmoothingOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IncreasedSmoothingOff */
    if ( strcmp ( argv[2], "IncreasedSmoothingOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IncreasedSmoothingOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IncreasedSmoothingOff */
    Tcl_DStringAppendElement ( &dString, " IncreasedSmoothing - tells the algorithm to used increased smoothing.\n" );
    Tcl_DStringAppendElement ( &dString, "void IncreasedSmoothingOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for IncreasedSmoothingOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSelfIntersectionThreshold */
    if ( strcmp ( argv[2], "SetSelfIntersectionThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSelfIntersectionThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSelfIntersectionThreshold */
    Tcl_DStringAppendElement ( &dString, " SelfIntersectionThreshold - threshold for the surface self intersections.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSelfIntersectionThreshold(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETImageFilter" );
    /* Closing for SetSelfIntersectionThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkSimpleImageToImageFilterCppCommand(static_cast<vtkSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
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
