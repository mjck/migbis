// tcl wrapper for vtkpxRPMCorrespondenceFinder object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxRPMCorrespondenceFinder.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxRPMCorrespondenceFinderNewCommand()
{
  vtkpxRPMCorrespondenceFinder *temp = vtkpxRPMCorrespondenceFinder::New();
  return static_cast<ClientData>(temp);
}

int vtkProcessObjectCppCommand(vtkProcessObject *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxRPMCorrespondenceFinderCppCommand(vtkpxRPMCorrespondenceFinder *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxRPMCorrespondenceFinderCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxRPMCorrespondenceFinderCppCommand(static_cast<vtkpxRPMCorrespondenceFinder *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxRPMCorrespondenceFinderCppCommand(vtkpxRPMCorrespondenceFinder *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxRPMCorrespondenceFinder",argv[1]))
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
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxRPMCorrespondenceFinder  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxRPMCorrespondenceFinder");
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
    vtkpxRPMCorrespondenceFinder  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxRPMCorrespondenceFinder");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxRPMCorrespondenceFinder  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxRPMCorrespondenceFinder");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetTemperature",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetTemperature(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTemperatureMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetTemperatureMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTemperatureMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetTemperatureMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTemperature",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetTemperature();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Initialize",argv[1]))&&(argc == 3))
    {
    vtkAbstractTransform  *temp0;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    op->Initialize(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Initialize",argv[1]))&&(argc == 2))
    {
    op->Initialize();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("WarpAndUpdateCorrespondence",argv[1]))&&(argc == 4))
    {
    vtkAbstractTransform  *temp0;
    float    temp1;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    op->WarpAndUpdateCorrespondence(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeDistance",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    float    temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->ComputeDistance(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeDistance",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->ComputeDistance();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPointsX",argv[1]))&&(argc == 2))
    {
    vtkPoints  *temp20;
    temp20 = (op)->GetPointsX();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPoints");
    return TCL_OK;
    }
  if ((!strcmp("GetPointsVY",argv[1]))&&(argc == 2))
    {
    vtkPoints  *temp20;
    temp20 = (op)->GetPointsVY();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPoints");
    return TCL_OK;
    }
  if ((!strcmp("GetM_Outliers_Column",argv[1]))&&(argc == 2))
    {
    vtkpxMatrix  *temp20;
    temp20 = (op)->GetM_Outliers_Column();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxMatrix");
    return TCL_OK;
    }
  if ((!strcmp("GetCollection",argv[1]))&&(argc == 2))
    {
    vtkCollection  *temp20;
    temp20 = (op)->GetCollection();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkCollection");
    return TCL_OK;
    }
  if ((!strcmp("SetSource",argv[1]))&&(argc == 3))
    {
    vtkPolyData  *temp0;
    error = 0;

    temp0 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPolyData"),interp,error));
    if (!error)
    {
    op->SetSource(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetTarget",argv[1]))&&(argc == 3))
    {
    vtkPolyData  *temp0;
    error = 0;

    temp0 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPolyData"),interp,error));
    if (!error)
    {
    op->SetTarget(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSource",argv[1]))&&(argc == 2))
    {
    vtkPolyData  *temp20;
    temp20 = (op)->GetSource();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
  if ((!strcmp("GetTarget",argv[1]))&&(argc == 2))
    {
    vtkPolyData  *temp20;
    temp20 = (op)->GetTarget();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
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
  if ((!strcmp("SetMaximumNumberOfLandmarks",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMaximumNumberOfLandmarks(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaximumNumberOfLandmarks",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMaximumNumberOfLandmarks();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetExtraDebug",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetExtraDebug(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetExtraDebugMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetExtraDebugMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetExtraDebugMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetExtraDebugMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetExtraDebug",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetExtraDebug();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMatchMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMatchModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMatchModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMatchModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMatchModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMatchMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMatchMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchModeToICP",argv[1]))&&(argc == 2))
    {
    op->SetMatchModeToICP();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchModeToMixture",argv[1]))&&(argc == 2))
    {
    op->SetMatchModeToMixture();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchModeToRPM",argv[1]))&&(argc == 2))
    {
    op->SetMatchModeToRPM();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchModeToMixtureFast",argv[1]))&&(argc == 2))
    {
    op->SetMatchModeToMixtureFast();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchModeToRPMFast",argv[1]))&&(argc == 2))
    {
    op->SetMatchModeToRPMFast();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchModeToICPPlus",argv[1]))&&(argc == 2))
    {
    op->SetMatchModeToICPPlus();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetUseLabels",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetUseLabels(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUseLabelsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseLabelsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseLabelsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseLabelsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseLabels",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseLabels();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("UseLabelsOn",argv[1]))&&(argc == 2))
    {
    op->UseLabelsOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("UseLabelsOff",argv[1]))&&(argc == 2))
    {
    op->UseLabelsOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetUseAttributes",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetUseAttributes(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUseAttributesMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseAttributesMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseAttributesMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseAttributesMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseAttributes",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseAttributes();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("UseAttributesOn",argv[1]))&&(argc == 2))
    {
    op->UseAttributesOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("UseAttributesOff",argv[1]))&&(argc == 2))
    {
    op->UseAttributesOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetAttributesWeight",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetAttributesWeight(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAttributesWeight",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetAttributesWeight();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPreferentialSampling",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetPreferentialSampling(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPreferentialSamplingMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPreferentialSamplingMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPreferentialSamplingMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPreferentialSamplingMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPreferentialSampling",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPreferentialSampling();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("PreferentialSamplingOn",argv[1]))&&(argc == 2))
    {
    op->PreferentialSamplingOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("PreferentialSamplingOff",argv[1]))&&(argc == 2))
    {
    op->PreferentialSamplingOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetUseWeightedLeastSquares",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetUseWeightedLeastSquares(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUseWeightedLeastSquaresMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseWeightedLeastSquaresMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseWeightedLeastSquaresMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseWeightedLeastSquaresMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseWeightedLeastSquares",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseWeightedLeastSquares();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("UseWeightedLeastSquaresOn",argv[1]))&&(argc == 2))
    {
    op->UseWeightedLeastSquaresOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("UseWeightedLeastSquaresOff",argv[1]))&&(argc == 2))
    {
    op->UseWeightedLeastSquaresOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetUseInitialTransform",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetUseInitialTransform(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUseInitialTransformMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseInitialTransformMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseInitialTransformMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseInitialTransformMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseInitialTransform",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseInitialTransform();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("UseInitialTransformOn",argv[1]))&&(argc == 2))
    {
    op->UseInitialTransformOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("UseInitialTransformOff",argv[1]))&&(argc == 2))
    {
    op->UseInitialTransformOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchMatrixType",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMatchMatrixType(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMatchMatrixTypeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMatchMatrixTypeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMatchMatrixTypeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMatchMatrixTypeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMatchMatrixType",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMatchMatrixType();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("MatchMatrixTypeOn",argv[1]))&&(argc == 2))
    {
    op->MatchMatrixTypeOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("MatchMatrixTypeOff",argv[1]))&&(argc == 2))
    {
    op->MatchMatrixTypeOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchMatrixTypeToAuto",argv[1]))&&(argc == 2))
    {
    op->SetMatchMatrixTypeToAuto();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchMatrixTypeToSparse",argv[1]))&&(argc == 2))
    {
    op->SetMatchMatrixTypeToSparse();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMatchMatrixTypeToDense",argv[1]))&&(argc == 2))
    {
    op->SetMatchMatrixTypeToDense();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetFastThreshold",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetFastThreshold(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFastThreshold",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetFastThreshold();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetEnableFeedback",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetEnableFeedback(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetEnableFeedback",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetEnableFeedback();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("EnableFeedbackOn",argv[1]))&&(argc == 2))
    {
    op->EnableFeedbackOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("EnableFeedbackOff",argv[1]))&&(argc == 2))
    {
    op->EnableFeedbackOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetAverageDistance",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetAverageDistance();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxRPMCorrespondenceFinderCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkProcessObjectCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxRPMCorrespondenceFinder:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetTemperature\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTemperatureMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTemperatureMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTemperature\n",NULL);
    Tcl_AppendResult(interp,"  Initialize\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Initialize\n",NULL);
    Tcl_AppendResult(interp,"  WarpAndUpdateCorrespondence\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeDistance\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ComputeDistance\n",NULL);
    Tcl_AppendResult(interp,"  GetPointsX\n",NULL);
    Tcl_AppendResult(interp,"  GetPointsVY\n",NULL);
    Tcl_AppendResult(interp,"  GetM_Outliers_Column\n",NULL);
    Tcl_AppendResult(interp,"  GetCollection\n",NULL);
    Tcl_AppendResult(interp,"  SetSource\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetTarget\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSource\n",NULL);
    Tcl_AppendResult(interp,"  GetTarget\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialTransform\n",NULL);
    Tcl_AppendResult(interp,"  SetMaximumNumberOfLandmarks\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaximumNumberOfLandmarks\n",NULL);
    Tcl_AppendResult(interp,"  SetExtraDebug\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetExtraDebugMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetExtraDebugMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetExtraDebug\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMatchModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMatchModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMatchMode\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchModeToICP\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchModeToMixture\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchModeToRPM\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchModeToMixtureFast\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchModeToRPMFast\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchModeToICPPlus\n",NULL);
    Tcl_AppendResult(interp,"  SetUseLabels\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUseLabelsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseLabelsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseLabels\n",NULL);
    Tcl_AppendResult(interp,"  UseLabelsOn\n",NULL);
    Tcl_AppendResult(interp,"  UseLabelsOff\n",NULL);
    Tcl_AppendResult(interp,"  SetUseAttributes\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUseAttributesMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseAttributesMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseAttributes\n",NULL);
    Tcl_AppendResult(interp,"  UseAttributesOn\n",NULL);
    Tcl_AppendResult(interp,"  UseAttributesOff\n",NULL);
    Tcl_AppendResult(interp,"  SetAttributesWeight\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAttributesWeight\n",NULL);
    Tcl_AppendResult(interp,"  SetPreferentialSampling\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPreferentialSamplingMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPreferentialSamplingMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPreferentialSampling\n",NULL);
    Tcl_AppendResult(interp,"  PreferentialSamplingOn\n",NULL);
    Tcl_AppendResult(interp,"  PreferentialSamplingOff\n",NULL);
    Tcl_AppendResult(interp,"  SetUseWeightedLeastSquares\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUseWeightedLeastSquaresMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseWeightedLeastSquaresMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseWeightedLeastSquares\n",NULL);
    Tcl_AppendResult(interp,"  UseWeightedLeastSquaresOn\n",NULL);
    Tcl_AppendResult(interp,"  UseWeightedLeastSquaresOff\n",NULL);
    Tcl_AppendResult(interp,"  SetUseInitialTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUseInitialTransformMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseInitialTransformMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseInitialTransform\n",NULL);
    Tcl_AppendResult(interp,"  UseInitialTransformOn\n",NULL);
    Tcl_AppendResult(interp,"  UseInitialTransformOff\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchMatrixType\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMatchMatrixTypeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMatchMatrixTypeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMatchMatrixType\n",NULL);
    Tcl_AppendResult(interp,"  MatchMatrixTypeOn\n",NULL);
    Tcl_AppendResult(interp,"  MatchMatrixTypeOff\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchMatrixTypeToAuto\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchMatrixTypeToSparse\n",NULL);
    Tcl_AppendResult(interp,"  SetMatchMatrixTypeToDense\n",NULL);
    Tcl_AppendResult(interp,"  SetFastThreshold\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFastThreshold\n",NULL);
    Tcl_AppendResult(interp,"  SetEnableFeedback\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetEnableFeedback\n",NULL);
    Tcl_AppendResult(interp,"  EnableFeedbackOn\n",NULL);
    Tcl_AppendResult(interp,"  EnableFeedbackOff\n",NULL);
    Tcl_AppendResult(interp,"  GetAverageDistance\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetTemperature" );
    Tcl_DStringAppendElement ( &dString, "GetTemperatureMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetTemperatureMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetTemperature" );
    Tcl_DStringAppendElement ( &dString, "Initialize" );
    Tcl_DStringAppendElement ( &dString, "Initialize" );
    Tcl_DStringAppendElement ( &dString, "WarpAndUpdateCorrespondence" );
    Tcl_DStringAppendElement ( &dString, "ComputeDistance" );
    Tcl_DStringAppendElement ( &dString, "ComputeDistance" );
    Tcl_DStringAppendElement ( &dString, "GetPointsX" );
    Tcl_DStringAppendElement ( &dString, "GetPointsVY" );
    Tcl_DStringAppendElement ( &dString, "GetM_Outliers_Column" );
    Tcl_DStringAppendElement ( &dString, "GetCollection" );
    Tcl_DStringAppendElement ( &dString, "SetSource" );
    Tcl_DStringAppendElement ( &dString, "SetTarget" );
    Tcl_DStringAppendElement ( &dString, "GetSource" );
    Tcl_DStringAppendElement ( &dString, "GetTarget" );
    Tcl_DStringAppendElement ( &dString, "SetInitialTransform" );
    Tcl_DStringAppendElement ( &dString, "GetInitialTransform" );
    Tcl_DStringAppendElement ( &dString, "SetMaximumNumberOfLandmarks" );
    Tcl_DStringAppendElement ( &dString, "GetMaximumNumberOfLandmarks" );
    Tcl_DStringAppendElement ( &dString, "SetExtraDebug" );
    Tcl_DStringAppendElement ( &dString, "GetExtraDebugMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetExtraDebugMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetExtraDebug" );
    Tcl_DStringAppendElement ( &dString, "SetMatchMode" );
    Tcl_DStringAppendElement ( &dString, "GetMatchModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMatchModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMatchMode" );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToICP" );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToMixture" );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToRPM" );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToMixtureFast" );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToRPMFast" );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToICPPlus" );
    Tcl_DStringAppendElement ( &dString, "SetUseLabels" );
    Tcl_DStringAppendElement ( &dString, "GetUseLabelsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseLabelsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseLabels" );
    Tcl_DStringAppendElement ( &dString, "UseLabelsOn" );
    Tcl_DStringAppendElement ( &dString, "UseLabelsOff" );
    Tcl_DStringAppendElement ( &dString, "SetUseAttributes" );
    Tcl_DStringAppendElement ( &dString, "GetUseAttributesMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseAttributesMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseAttributes" );
    Tcl_DStringAppendElement ( &dString, "UseAttributesOn" );
    Tcl_DStringAppendElement ( &dString, "UseAttributesOff" );
    Tcl_DStringAppendElement ( &dString, "SetAttributesWeight" );
    Tcl_DStringAppendElement ( &dString, "GetAttributesWeight" );
    Tcl_DStringAppendElement ( &dString, "SetPreferentialSampling" );
    Tcl_DStringAppendElement ( &dString, "GetPreferentialSamplingMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetPreferentialSamplingMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetPreferentialSampling" );
    Tcl_DStringAppendElement ( &dString, "PreferentialSamplingOn" );
    Tcl_DStringAppendElement ( &dString, "PreferentialSamplingOff" );
    Tcl_DStringAppendElement ( &dString, "SetUseWeightedLeastSquares" );
    Tcl_DStringAppendElement ( &dString, "GetUseWeightedLeastSquaresMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseWeightedLeastSquaresMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseWeightedLeastSquares" );
    Tcl_DStringAppendElement ( &dString, "UseWeightedLeastSquaresOn" );
    Tcl_DStringAppendElement ( &dString, "UseWeightedLeastSquaresOff" );
    Tcl_DStringAppendElement ( &dString, "SetUseInitialTransform" );
    Tcl_DStringAppendElement ( &dString, "GetUseInitialTransformMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseInitialTransformMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseInitialTransform" );
    Tcl_DStringAppendElement ( &dString, "UseInitialTransformOn" );
    Tcl_DStringAppendElement ( &dString, "UseInitialTransformOff" );
    Tcl_DStringAppendElement ( &dString, "SetMatchMatrixType" );
    Tcl_DStringAppendElement ( &dString, "GetMatchMatrixTypeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMatchMatrixTypeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMatchMatrixType" );
    Tcl_DStringAppendElement ( &dString, "MatchMatrixTypeOn" );
    Tcl_DStringAppendElement ( &dString, "MatchMatrixTypeOff" );
    Tcl_DStringAppendElement ( &dString, "SetMatchMatrixTypeToAuto" );
    Tcl_DStringAppendElement ( &dString, "SetMatchMatrixTypeToSparse" );
    Tcl_DStringAppendElement ( &dString, "SetMatchMatrixTypeToDense" );
    Tcl_DStringAppendElement ( &dString, "SetFastThreshold" );
    Tcl_DStringAppendElement ( &dString, "GetFastThreshold" );
    Tcl_DStringAppendElement ( &dString, "SetEnableFeedback" );
    Tcl_DStringAppendElement ( &dString, "GetEnableFeedback" );
    Tcl_DStringAppendElement ( &dString, "EnableFeedbackOn" );
    Tcl_DStringAppendElement ( &dString, "EnableFeedbackOff" );
    Tcl_DStringAppendElement ( &dString, "GetAverageDistance" );
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
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkpxRPMCorrespondenceFinder *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTemperature */
    if ( strcmp ( argv[2], "SetTemperature" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTemperature" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTemperature */
    Tcl_DStringAppendElement ( &dString, " Set the current Temperature\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTemperature(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetTemperature */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTemperatureMinValue */
    if ( strcmp ( argv[2], "GetTemperatureMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTemperatureMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTemperatureMinValue */
    Tcl_DStringAppendElement ( &dString, " Set the current Temperature\n" );
    Tcl_DStringAppendElement ( &dString, "float GetTemperatureMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetTemperatureMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTemperatureMaxValue */
    if ( strcmp ( argv[2], "GetTemperatureMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTemperatureMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTemperatureMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set the current Temperature\n" );
    Tcl_DStringAppendElement ( &dString, "float GetTemperatureMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetTemperatureMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTemperature */
    if ( strcmp ( argv[2], "GetTemperature" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTemperature" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTemperature */
    Tcl_DStringAppendElement ( &dString, " Set the current Temperature\n" );
    Tcl_DStringAppendElement ( &dString, "float GetTemperature();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetTemperature */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Initialize */
    if ( strcmp ( argv[2], "Initialize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Initialize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Initialize */
    Tcl_DStringAppendElement ( &dString, " Initialize by allocating Memory etc\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Initialize(vtkAbstractTransform *OffsetTransform);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for Initialize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Initialize */
    if ( strcmp ( argv[2], "Initialize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Initialize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Initialize */
    Tcl_DStringAppendElement ( &dString, " Initialize by allocating Memory etc\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Initialize();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for Initialize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: WarpAndUpdateCorrespondence */
    if ( strcmp ( argv[2], "WarpAndUpdateCorrespondence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "WarpAndUpdateCorrespondence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for WarpAndUpdateCorrespondence */
    Tcl_DStringAppendElement ( &dString, " Warp and Update Correspondence\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void WarpAndUpdateCorrespondence(vtkAbstractTransform *warpTransform, float temperature);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for WarpAndUpdateCorrespondence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeDistance */
    if ( strcmp ( argv[2], "ComputeDistance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeDistance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeDistance */
    Tcl_DStringAppendElement ( &dString, " Warp and Update Correspondence\n" );
    Tcl_DStringAppendElement ( &dString, "virtual float ComputeDistance(const char *label);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for ComputeDistance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeDistance */
    if ( strcmp ( argv[2], "ComputeDistance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeDistance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeDistance */
    Tcl_DStringAppendElement ( &dString, " Warp and Update Correspondence\n" );
    Tcl_DStringAppendElement ( &dString, "virtual float ComputeDistance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for ComputeDistance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPointsX */
    if ( strcmp ( argv[2], "GetPointsX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPointsX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPointsX */
    Tcl_DStringAppendElement ( &dString, " Access Certain Internal Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints *GetPointsX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetPointsX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPointsVY */
    if ( strcmp ( argv[2], "GetPointsVY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPointsVY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPointsVY */
    Tcl_DStringAppendElement ( &dString, " Access Certain Internal Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints *GetPointsVY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetPointsVY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetM_Outliers_Column */
    if ( strcmp ( argv[2], "GetM_Outliers_Column" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetM_Outliers_Column" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetM_Outliers_Column */
    Tcl_DStringAppendElement ( &dString, " Access Certain Internal Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix *GetM_Outliers_Column();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetM_Outliers_Column */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCollection */
    if ( strcmp ( argv[2], "GetCollection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCollection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCollection */
    Tcl_DStringAppendElement ( &dString, " Access Certain Internal Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "vtkCollection *GetCollection();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetCollection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSource */
    if ( strcmp ( argv[2], "SetSource" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSource" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSource */
    Tcl_DStringAppendElement ( &dString, " Specify the source and target data sets.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSource(vtkPolyData *source);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetSource */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTarget */
    if ( strcmp ( argv[2], "SetTarget" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTarget" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTarget */
    Tcl_DStringAppendElement ( &dString, " Specify the source and target data sets.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTarget(vtkPolyData *target);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetTarget */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSource */
    if ( strcmp ( argv[2], "GetSource" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSource" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSource */
    Tcl_DStringAppendElement ( &dString, " Specify the source and target data sets.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData *GetSource();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetSource */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTarget */
    if ( strcmp ( argv[2], "GetTarget" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTarget" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTarget */
    Tcl_DStringAppendElement ( &dString, " Specify the source and target data sets.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData *GetTarget();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetTarget */

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
    Tcl_DStringAppendElement ( &dString, " Specify the initial transformation\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialTransform(vtkAbstractTransform *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
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
    Tcl_DStringAppendElement ( &dString, " Specify the initial transformation\n" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform *GetInitialTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetInitialTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaximumNumberOfLandmarks */
    if ( strcmp ( argv[2], "SetMaximumNumberOfLandmarks" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaximumNumberOfLandmarks" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaximumNumberOfLandmarks */
    Tcl_DStringAppendElement ( &dString, " Set/Get vertex step\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaximumNumberOfLandmarks(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMaximumNumberOfLandmarks */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaximumNumberOfLandmarks */
    if ( strcmp ( argv[2], "GetMaximumNumberOfLandmarks" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaximumNumberOfLandmarks" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaximumNumberOfLandmarks */
    Tcl_DStringAppendElement ( &dString, " Set/Get vertex step\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMaximumNumberOfLandmarks();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetMaximumNumberOfLandmarks */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetExtraDebug */
    if ( strcmp ( argv[2], "SetExtraDebug" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetExtraDebug" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetExtraDebug */
    Tcl_DStringAppendElement ( &dString, " Extra Debug\n" );
    Tcl_DStringAppendElement ( &dString, "void SetExtraDebug(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetExtraDebug */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetExtraDebugMinValue */
    if ( strcmp ( argv[2], "GetExtraDebugMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetExtraDebugMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetExtraDebugMinValue */
    Tcl_DStringAppendElement ( &dString, " Extra Debug\n" );
    Tcl_DStringAppendElement ( &dString, "int GetExtraDebugMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetExtraDebugMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetExtraDebugMaxValue */
    if ( strcmp ( argv[2], "GetExtraDebugMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetExtraDebugMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetExtraDebugMaxValue */
    Tcl_DStringAppendElement ( &dString, " Extra Debug\n" );
    Tcl_DStringAppendElement ( &dString, "int GetExtraDebugMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetExtraDebugMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetExtraDebug */
    if ( strcmp ( argv[2], "GetExtraDebug" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetExtraDebug" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetExtraDebug */
    Tcl_DStringAppendElement ( &dString, " Extra Debug\n" );
    Tcl_DStringAppendElement ( &dString, "int GetExtraDebug();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetExtraDebug */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchMode */
    if ( strcmp ( argv[2], "SetMatchMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchMode */
    Tcl_DStringAppendElement ( &dString, " Set the current mode 0=ICP, 1=RPM-Mixture 2=RPM 3=RPM-Mixture-Fast 4=RPM Fast 5=ICP-Plus\n 6=RPM-Mixture-Hybrid 7-RPM Hybrid\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMatchMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMatchModeMinValue */
    if ( strcmp ( argv[2], "GetMatchModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMatchModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMatchModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Set the current mode 0=ICP, 1=RPM-Mixture 2=RPM 3=RPM-Mixture-Fast 4=RPM Fast 5=ICP-Plus\n 6=RPM-Mixture-Hybrid 7-RPM Hybrid\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMatchModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetMatchModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMatchModeMaxValue */
    if ( strcmp ( argv[2], "GetMatchModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMatchModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMatchModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set the current mode 0=ICP, 1=RPM-Mixture 2=RPM 3=RPM-Mixture-Fast 4=RPM Fast 5=ICP-Plus\n 6=RPM-Mixture-Hybrid 7-RPM Hybrid\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMatchModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetMatchModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMatchMode */
    if ( strcmp ( argv[2], "GetMatchMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMatchMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMatchMode */
    Tcl_DStringAppendElement ( &dString, " Set the current mode 0=ICP, 1=RPM-Mixture 2=RPM 3=RPM-Mixture-Fast 4=RPM Fast 5=ICP-Plus\n 6=RPM-Mixture-Hybrid 7-RPM Hybrid\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMatchMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetMatchMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchModeToICP */
    if ( strcmp ( argv[2], "SetMatchModeToICP" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToICP" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchModeToICP */
    Tcl_DStringAppendElement ( &dString, " Dedicated Set/Get MatchMode stuff \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMatchModeToICP();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchModeToICP */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchModeToMixture */
    if ( strcmp ( argv[2], "SetMatchModeToMixture" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToMixture" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchModeToMixture */
    Tcl_DStringAppendElement ( &dString, " Dedicated Set/Get MatchMode stuff \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMatchModeToMixture();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchModeToMixture */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchModeToRPM */
    if ( strcmp ( argv[2], "SetMatchModeToRPM" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToRPM" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchModeToRPM */
    Tcl_DStringAppendElement ( &dString, " Dedicated Set/Get MatchMode stuff \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMatchModeToRPM();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchModeToRPM */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchModeToMixtureFast */
    if ( strcmp ( argv[2], "SetMatchModeToMixtureFast" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToMixtureFast" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchModeToMixtureFast */
    Tcl_DStringAppendElement ( &dString, " Dedicated Set/Get MatchMode stuff \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMatchModeToMixtureFast();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchModeToMixtureFast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchModeToRPMFast */
    if ( strcmp ( argv[2], "SetMatchModeToRPMFast" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToRPMFast" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchModeToRPMFast */
    Tcl_DStringAppendElement ( &dString, " Dedicated Set/Get MatchMode stuff \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMatchModeToRPMFast();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchModeToRPMFast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchModeToICPPlus */
    if ( strcmp ( argv[2], "SetMatchModeToICPPlus" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchModeToICPPlus" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchModeToICPPlus */
    Tcl_DStringAppendElement ( &dString, " Dedicated Set/Get MatchMode stuff \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMatchModeToICPPlus();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchModeToICPPlus */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUseLabels */
    if ( strcmp ( argv[2], "SetUseLabels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUseLabels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUseLabels */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUseLabels(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetUseLabels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseLabelsMinValue */
    if ( strcmp ( argv[2], "GetUseLabelsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseLabelsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseLabelsMinValue */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseLabelsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseLabelsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseLabelsMaxValue */
    if ( strcmp ( argv[2], "GetUseLabelsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseLabelsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseLabelsMaxValue */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseLabelsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseLabelsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseLabels */
    if ( strcmp ( argv[2], "GetUseLabels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseLabels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseLabels */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseLabels();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseLabels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseLabelsOn */
    if ( strcmp ( argv[2], "UseLabelsOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseLabelsOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseLabelsOn */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "void UseLabelsOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for UseLabelsOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseLabelsOff */
    if ( strcmp ( argv[2], "UseLabelsOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseLabelsOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseLabelsOff */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "void UseLabelsOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for UseLabelsOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUseAttributes */
    if ( strcmp ( argv[2], "SetUseAttributes" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUseAttributes" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUseAttributes */
    Tcl_DStringAppendElement ( &dString, " Use Attributes \n" );
    Tcl_DStringAppendElement ( &dString, "void SetUseAttributes(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetUseAttributes */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseAttributesMinValue */
    if ( strcmp ( argv[2], "GetUseAttributesMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseAttributesMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseAttributesMinValue */
    Tcl_DStringAppendElement ( &dString, " Use Attributes \n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseAttributesMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseAttributesMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseAttributesMaxValue */
    if ( strcmp ( argv[2], "GetUseAttributesMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseAttributesMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseAttributesMaxValue */
    Tcl_DStringAppendElement ( &dString, " Use Attributes \n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseAttributesMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseAttributesMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseAttributes */
    if ( strcmp ( argv[2], "GetUseAttributes" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseAttributes" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseAttributes */
    Tcl_DStringAppendElement ( &dString, " Use Attributes \n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseAttributes();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseAttributes */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseAttributesOn */
    if ( strcmp ( argv[2], "UseAttributesOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseAttributesOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseAttributesOn */
    Tcl_DStringAppendElement ( &dString, " Use Attributes \n" );
    Tcl_DStringAppendElement ( &dString, "void UseAttributesOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for UseAttributesOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseAttributesOff */
    if ( strcmp ( argv[2], "UseAttributesOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseAttributesOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseAttributesOff */
    Tcl_DStringAppendElement ( &dString, " Use Attributes \n" );
    Tcl_DStringAppendElement ( &dString, "void UseAttributesOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for UseAttributesOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAttributesWeight */
    if ( strcmp ( argv[2], "SetAttributesWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAttributesWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAttributesWeight */
    Tcl_DStringAppendElement ( &dString, " Threshold for fast search\n" );
    Tcl_DStringAppendElement ( &dString, "void SetAttributesWeight(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetAttributesWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAttributesWeight */
    if ( strcmp ( argv[2], "GetAttributesWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAttributesWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAttributesWeight */
    Tcl_DStringAppendElement ( &dString, " Threshold for fast search\n" );
    Tcl_DStringAppendElement ( &dString, "float GetAttributesWeight();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetAttributesWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPreferentialSampling */
    if ( strcmp ( argv[2], "SetPreferentialSampling" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPreferentialSampling" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPreferentialSampling */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPreferentialSampling(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetPreferentialSampling */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPreferentialSamplingMinValue */
    if ( strcmp ( argv[2], "GetPreferentialSamplingMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPreferentialSamplingMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPreferentialSamplingMinValue */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPreferentialSamplingMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetPreferentialSamplingMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPreferentialSamplingMaxValue */
    if ( strcmp ( argv[2], "GetPreferentialSamplingMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPreferentialSamplingMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPreferentialSamplingMaxValue */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPreferentialSamplingMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetPreferentialSamplingMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPreferentialSampling */
    if ( strcmp ( argv[2], "GetPreferentialSampling" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPreferentialSampling" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPreferentialSampling */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPreferentialSampling();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetPreferentialSampling */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: PreferentialSamplingOn */
    if ( strcmp ( argv[2], "PreferentialSamplingOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "PreferentialSamplingOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for PreferentialSamplingOn */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "void PreferentialSamplingOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for PreferentialSamplingOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: PreferentialSamplingOff */
    if ( strcmp ( argv[2], "PreferentialSamplingOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "PreferentialSamplingOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for PreferentialSamplingOff */
    Tcl_DStringAppendElement ( &dString, " UseLabels\n" );
    Tcl_DStringAppendElement ( &dString, "void PreferentialSamplingOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for PreferentialSamplingOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUseWeightedLeastSquares */
    if ( strcmp ( argv[2], "SetUseWeightedLeastSquares" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUseWeightedLeastSquares" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUseWeightedLeastSquares */
    Tcl_DStringAppendElement ( &dString, " UseWeightedLeastSquares\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUseWeightedLeastSquares(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetUseWeightedLeastSquares */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseWeightedLeastSquaresMinValue */
    if ( strcmp ( argv[2], "GetUseWeightedLeastSquaresMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseWeightedLeastSquaresMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseWeightedLeastSquaresMinValue */
    Tcl_DStringAppendElement ( &dString, " UseWeightedLeastSquares\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseWeightedLeastSquaresMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseWeightedLeastSquaresMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseWeightedLeastSquaresMaxValue */
    if ( strcmp ( argv[2], "GetUseWeightedLeastSquaresMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseWeightedLeastSquaresMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseWeightedLeastSquaresMaxValue */
    Tcl_DStringAppendElement ( &dString, " UseWeightedLeastSquares\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseWeightedLeastSquaresMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseWeightedLeastSquaresMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseWeightedLeastSquares */
    if ( strcmp ( argv[2], "GetUseWeightedLeastSquares" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseWeightedLeastSquares" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseWeightedLeastSquares */
    Tcl_DStringAppendElement ( &dString, " UseWeightedLeastSquares\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseWeightedLeastSquares();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseWeightedLeastSquares */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseWeightedLeastSquaresOn */
    if ( strcmp ( argv[2], "UseWeightedLeastSquaresOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseWeightedLeastSquaresOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseWeightedLeastSquaresOn */
    Tcl_DStringAppendElement ( &dString, " UseWeightedLeastSquares\n" );
    Tcl_DStringAppendElement ( &dString, "void UseWeightedLeastSquaresOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for UseWeightedLeastSquaresOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseWeightedLeastSquaresOff */
    if ( strcmp ( argv[2], "UseWeightedLeastSquaresOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseWeightedLeastSquaresOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseWeightedLeastSquaresOff */
    Tcl_DStringAppendElement ( &dString, " UseWeightedLeastSquares\n" );
    Tcl_DStringAppendElement ( &dString, "void UseWeightedLeastSquaresOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for UseWeightedLeastSquaresOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUseInitialTransform */
    if ( strcmp ( argv[2], "SetUseInitialTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUseInitialTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUseInitialTransform */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUseInitialTransform(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetUseInitialTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseInitialTransformMinValue */
    if ( strcmp ( argv[2], "GetUseInitialTransformMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseInitialTransformMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseInitialTransformMinValue */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseInitialTransformMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseInitialTransformMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseInitialTransformMaxValue */
    if ( strcmp ( argv[2], "GetUseInitialTransformMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseInitialTransformMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseInitialTransformMaxValue */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseInitialTransformMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseInitialTransformMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseInitialTransform */
    if ( strcmp ( argv[2], "GetUseInitialTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseInitialTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseInitialTransform */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseInitialTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetUseInitialTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseInitialTransformOn */
    if ( strcmp ( argv[2], "UseInitialTransformOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseInitialTransformOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseInitialTransformOn */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "void UseInitialTransformOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for UseInitialTransformOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseInitialTransformOff */
    if ( strcmp ( argv[2], "UseInitialTransformOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseInitialTransformOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseInitialTransformOff */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "void UseInitialTransformOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for UseInitialTransformOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchMatrixType */
    if ( strcmp ( argv[2], "SetMatchMatrixType" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchMatrixType" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchMatrixType */
    Tcl_DStringAppendElement ( &dString, " MatchMatrixType\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMatchMatrixType(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchMatrixType */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMatchMatrixTypeMinValue */
    if ( strcmp ( argv[2], "GetMatchMatrixTypeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMatchMatrixTypeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMatchMatrixTypeMinValue */
    Tcl_DStringAppendElement ( &dString, " MatchMatrixType\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMatchMatrixTypeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetMatchMatrixTypeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMatchMatrixTypeMaxValue */
    if ( strcmp ( argv[2], "GetMatchMatrixTypeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMatchMatrixTypeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMatchMatrixTypeMaxValue */
    Tcl_DStringAppendElement ( &dString, " MatchMatrixType\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMatchMatrixTypeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetMatchMatrixTypeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMatchMatrixType */
    if ( strcmp ( argv[2], "GetMatchMatrixType" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMatchMatrixType" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMatchMatrixType */
    Tcl_DStringAppendElement ( &dString, " MatchMatrixType\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMatchMatrixType();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetMatchMatrixType */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MatchMatrixTypeOn */
    if ( strcmp ( argv[2], "MatchMatrixTypeOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MatchMatrixTypeOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MatchMatrixTypeOn */
    Tcl_DStringAppendElement ( &dString, " MatchMatrixType\n" );
    Tcl_DStringAppendElement ( &dString, "void MatchMatrixTypeOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for MatchMatrixTypeOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MatchMatrixTypeOff */
    if ( strcmp ( argv[2], "MatchMatrixTypeOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MatchMatrixTypeOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MatchMatrixTypeOff */
    Tcl_DStringAppendElement ( &dString, " MatchMatrixType\n" );
    Tcl_DStringAppendElement ( &dString, "void MatchMatrixTypeOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for MatchMatrixTypeOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchMatrixTypeToAuto */
    if ( strcmp ( argv[2], "SetMatchMatrixTypeToAuto" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchMatrixTypeToAuto" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchMatrixTypeToAuto */
    Tcl_DStringAppendElement ( &dString, " MatchMatrixType\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMatchMatrixTypeToAuto();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchMatrixTypeToAuto */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchMatrixTypeToSparse */
    if ( strcmp ( argv[2], "SetMatchMatrixTypeToSparse" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchMatrixTypeToSparse" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchMatrixTypeToSparse */
    Tcl_DStringAppendElement ( &dString, " MatchMatrixType\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMatchMatrixTypeToSparse();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchMatrixTypeToSparse */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMatchMatrixTypeToDense */
    if ( strcmp ( argv[2], "SetMatchMatrixTypeToDense" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMatchMatrixTypeToDense" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMatchMatrixTypeToDense */
    Tcl_DStringAppendElement ( &dString, " MatchMatrixType\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMatchMatrixTypeToDense();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetMatchMatrixTypeToDense */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFastThreshold */
    if ( strcmp ( argv[2], "SetFastThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFastThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFastThreshold */
    Tcl_DStringAppendElement ( &dString, " Threshold for fast search\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFastThreshold(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetFastThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFastThreshold */
    if ( strcmp ( argv[2], "GetFastThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFastThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFastThreshold */
    Tcl_DStringAppendElement ( &dString, " Threshold for fast search\n" );
    Tcl_DStringAppendElement ( &dString, "float GetFastThreshold();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetFastThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetEnableFeedback */
    if ( strcmp ( argv[2], "SetEnableFeedback" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetEnableFeedback" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetEnableFeedback */
    Tcl_DStringAppendElement ( &dString, " Enable/Disable Update of Transformation in conjuction with UpdateProgress\n" );
    Tcl_DStringAppendElement ( &dString, "void SetEnableFeedback(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for SetEnableFeedback */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetEnableFeedback */
    if ( strcmp ( argv[2], "GetEnableFeedback" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetEnableFeedback" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetEnableFeedback */
    Tcl_DStringAppendElement ( &dString, " Enable/Disable Update of Transformation in conjuction with UpdateProgress\n" );
    Tcl_DStringAppendElement ( &dString, "int GetEnableFeedback();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetEnableFeedback */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: EnableFeedbackOn */
    if ( strcmp ( argv[2], "EnableFeedbackOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "EnableFeedbackOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for EnableFeedbackOn */
    Tcl_DStringAppendElement ( &dString, " Enable/Disable Update of Transformation in conjuction with UpdateProgress\n" );
    Tcl_DStringAppendElement ( &dString, "void EnableFeedbackOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for EnableFeedbackOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: EnableFeedbackOff */
    if ( strcmp ( argv[2], "EnableFeedbackOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "EnableFeedbackOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for EnableFeedbackOff */
    Tcl_DStringAppendElement ( &dString, " Enable/Disable Update of Transformation in conjuction with UpdateProgress\n" );
    Tcl_DStringAppendElement ( &dString, "void EnableFeedbackOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for EnableFeedbackOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAverageDistance */
    if ( strcmp ( argv[2], "GetAverageDistance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAverageDistance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAverageDistance */
    Tcl_DStringAppendElement ( &dString, " Threshold for fast search\n" );
    Tcl_DStringAppendElement ( &dString, "float GetAverageDistance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder" );
    /* Closing for GetAverageDistance */

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
