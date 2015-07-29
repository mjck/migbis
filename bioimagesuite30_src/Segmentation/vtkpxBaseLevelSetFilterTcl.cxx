// tcl wrapper for vtkpxBaseLevelSetFilter object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxBaseLevelSetFilter.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>
int vtkSimpleImageToImageFilterCppCommand(vtkSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxBaseLevelSetFilterCppCommand(vtkpxBaseLevelSetFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxBaseLevelSetFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxBaseLevelSetFilterCppCommand(static_cast<vtkpxBaseLevelSetFilter *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxBaseLevelSetFilterCppCommand(vtkpxBaseLevelSetFilter *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxBaseLevelSetFilter",argv[1]))
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
    vtkpxBaseLevelSetFilter  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxBaseLevelSetFilter");
    return TCL_OK;
    }
  if ((!strcmp("SetInitialLevelSet",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetInitialLevelSet(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInitialLevelSet",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetInitialLevelSet();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetInitialPoints",argv[1]))&&(argc == 3))
    {
    vtkPoints  *temp0;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    if (!error)
    {
    op->SetInitialPoints(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInitialPoints",argv[1]))&&(argc == 2))
    {
    vtkPoints  *temp20;
    temp20 = (op)->GetInitialPoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPoints");
    return TCL_OK;
    }
  if ((!strcmp("SetInitialDistance",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetInitialDistance(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInitialDistanceMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialDistanceMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInitialDistanceMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialDistanceMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInitialDistance",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialDistance();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRMSError",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetRMSError(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRMSErrorMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetRMSErrorMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRMSErrorMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetRMSErrorMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRMSError",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetRMSError();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetLevelsetIterations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetLevelsetIterations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLevelsetIterationsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetLevelsetIterationsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLevelsetIterationsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetLevelsetIterationsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLevelsetIterations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetLevelsetIterations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInputLevelSet",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetInputLevelSet();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetIterationName",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetIterationName(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetIterationName",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetIterationName();
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
  if ((!strcmp("SetOutputStatus",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetOutputStatus(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutputStatus",argv[1]))&&(argc == 2))
    {
    char    *temp20;
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
  if ((!strcmp("SetIterationSave",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetIterationSave(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetIterationSaveMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationSaveMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetIterationSaveMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationSaveMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetIterationSave",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationSave();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("IterationSaveOn",argv[1]))&&(argc == 2))
    {
    op->IterationSaveOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("IterationSaveOff",argv[1]))&&(argc == 2))
    {
    op->IterationSaveOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetIterationSaveFrequency",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetIterationSaveFrequency(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetIterationSaveFrequencyMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationSaveFrequencyMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetIterationSaveFrequencyMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationSaveFrequencyMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetIterationSaveFrequency",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationSaveFrequency();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("IterationSaveFrequencyOn",argv[1]))&&(argc == 2))
    {
    op->IterationSaveFrequencyOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("IterationSaveFrequencyOff",argv[1]))&&(argc == 2))
    {
    op->IterationSaveFrequencyOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetIterationSaveOrientation",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetIterationSaveOrientation(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetIterationSaveOrientationMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationSaveOrientationMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetIterationSaveOrientationMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationSaveOrientationMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetIterationSaveOrientation",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationSaveOrientation();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("IterationSaveOrientationOn",argv[1]))&&(argc == 2))
    {
    op->IterationSaveOrientationOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("IterationSaveOrientationOff",argv[1]))&&(argc == 2))
    {
    op->IterationSaveOrientationOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetPropagationWeight",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetPropagationWeight(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPropagationWeightMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPropagationWeightMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPropagationWeightMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPropagationWeightMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPropagationWeight",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPropagationWeight();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCurvatureWeight",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetCurvatureWeight(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCurvatureWeightMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetCurvatureWeightMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCurvatureWeightMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetCurvatureWeightMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCurvatureWeight",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetCurvatureWeight();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetAdvectionWeight",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetAdvectionWeight(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAdvectionWeightMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetAdvectionWeightMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAdvectionWeightMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetAdvectionWeightMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAdvectionWeight",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetAdvectionWeight();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetUpdatePercentage",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetUpdatePercentage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUpdatePercentageMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetUpdatePercentageMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUpdatePercentageMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetUpdatePercentageMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUpdatePercentage",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetUpdatePercentage();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRealIteration",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetRealIteration(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRealIteration",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRealIteration();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxBaseLevelSetFilterCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxBaseLevelSetFilter:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialLevelSet\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialLevelSet\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialPoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialPoints\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialDistance\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialDistanceMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialDistanceMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialDistance\n",NULL);
    Tcl_AppendResult(interp,"  SetRMSError\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRMSErrorMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRMSErrorMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRMSError\n",NULL);
    Tcl_AppendResult(interp,"  SetLevelsetIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLevelsetIterationsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLevelsetIterationsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLevelsetIterations\n",NULL);
    Tcl_AppendResult(interp,"  GetInputLevelSet\n",NULL);
    Tcl_AppendResult(interp,"  SetIterationName\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationName\n",NULL);
    Tcl_AppendResult(interp,"  SetOutputStatus\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutputStatus\n",NULL);
    Tcl_AppendResult(interp,"  SetIterationSave\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationSaveMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationSaveMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationSave\n",NULL);
    Tcl_AppendResult(interp,"  IterationSaveOn\n",NULL);
    Tcl_AppendResult(interp,"  IterationSaveOff\n",NULL);
    Tcl_AppendResult(interp,"  SetIterationSaveFrequency\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationSaveFrequencyMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationSaveFrequencyMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationSaveFrequency\n",NULL);
    Tcl_AppendResult(interp,"  IterationSaveFrequencyOn\n",NULL);
    Tcl_AppendResult(interp,"  IterationSaveFrequencyOff\n",NULL);
    Tcl_AppendResult(interp,"  SetIterationSaveOrientation\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationSaveOrientationMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationSaveOrientationMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationSaveOrientation\n",NULL);
    Tcl_AppendResult(interp,"  IterationSaveOrientationOn\n",NULL);
    Tcl_AppendResult(interp,"  IterationSaveOrientationOff\n",NULL);
    Tcl_AppendResult(interp,"  SetPropagationWeight\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPropagationWeightMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPropagationWeightMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPropagationWeight\n",NULL);
    Tcl_AppendResult(interp,"  SetCurvatureWeight\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCurvatureWeightMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCurvatureWeightMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCurvatureWeight\n",NULL);
    Tcl_AppendResult(interp,"  SetAdvectionWeight\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAdvectionWeightMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAdvectionWeightMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAdvectionWeight\n",NULL);
    Tcl_AppendResult(interp,"  SetUpdatePercentage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUpdatePercentageMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUpdatePercentageMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUpdatePercentage\n",NULL);
    Tcl_AppendResult(interp,"  SetRealIteration\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRealIteration\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SetInitialLevelSet" );
    Tcl_DStringAppendElement ( &dString, "GetInitialLevelSet" );
    Tcl_DStringAppendElement ( &dString, "SetInitialPoints" );
    Tcl_DStringAppendElement ( &dString, "GetInitialPoints" );
    Tcl_DStringAppendElement ( &dString, "SetInitialDistance" );
    Tcl_DStringAppendElement ( &dString, "GetInitialDistanceMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetInitialDistanceMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetInitialDistance" );
    Tcl_DStringAppendElement ( &dString, "SetRMSError" );
    Tcl_DStringAppendElement ( &dString, "GetRMSErrorMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetRMSErrorMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetRMSError" );
    Tcl_DStringAppendElement ( &dString, "SetLevelsetIterations" );
    Tcl_DStringAppendElement ( &dString, "GetLevelsetIterationsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetLevelsetIterationsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetLevelsetIterations" );
    Tcl_DStringAppendElement ( &dString, "GetInputLevelSet" );
    Tcl_DStringAppendElement ( &dString, "SetIterationName" );
    Tcl_DStringAppendElement ( &dString, "GetIterationName" );
    Tcl_DStringAppendElement ( &dString, "SetOutputStatus" );
    Tcl_DStringAppendElement ( &dString, "GetOutputStatus" );
    Tcl_DStringAppendElement ( &dString, "SetIterationSave" );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterationSave" );
    Tcl_DStringAppendElement ( &dString, "IterationSaveOn" );
    Tcl_DStringAppendElement ( &dString, "IterationSaveOff" );
    Tcl_DStringAppendElement ( &dString, "SetIterationSaveFrequency" );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveFrequencyMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveFrequencyMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveFrequency" );
    Tcl_DStringAppendElement ( &dString, "IterationSaveFrequencyOn" );
    Tcl_DStringAppendElement ( &dString, "IterationSaveFrequencyOff" );
    Tcl_DStringAppendElement ( &dString, "SetIterationSaveOrientation" );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveOrientationMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveOrientationMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveOrientation" );
    Tcl_DStringAppendElement ( &dString, "IterationSaveOrientationOn" );
    Tcl_DStringAppendElement ( &dString, "IterationSaveOrientationOff" );
    Tcl_DStringAppendElement ( &dString, "SetPropagationWeight" );
    Tcl_DStringAppendElement ( &dString, "GetPropagationWeightMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetPropagationWeightMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetPropagationWeight" );
    Tcl_DStringAppendElement ( &dString, "SetCurvatureWeight" );
    Tcl_DStringAppendElement ( &dString, "GetCurvatureWeightMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetCurvatureWeightMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetCurvatureWeight" );
    Tcl_DStringAppendElement ( &dString, "SetAdvectionWeight" );
    Tcl_DStringAppendElement ( &dString, "GetAdvectionWeightMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetAdvectionWeightMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetAdvectionWeight" );
    Tcl_DStringAppendElement ( &dString, "SetUpdatePercentage" );
    Tcl_DStringAppendElement ( &dString, "GetUpdatePercentageMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetUpdatePercentageMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUpdatePercentage" );
    Tcl_DStringAppendElement ( &dString, "SetRealIteration" );
    Tcl_DStringAppendElement ( &dString, "GetRealIteration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for NewInstance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialLevelSet */
    if ( strcmp ( argv[2], "SetInitialLevelSet" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialLevelSet" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialLevelSet */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialLevelSet(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetInitialLevelSet */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialLevelSet */
    if ( strcmp ( argv[2], "GetInitialLevelSet" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialLevelSet" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialLevelSet */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetInitialLevelSet();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetInitialLevelSet */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialPoints */
    if ( strcmp ( argv[2], "SetInitialPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialPoints */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialPoints(vtkPoints *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetInitialPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialPoints */
    if ( strcmp ( argv[2], "GetInitialPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialPoints */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints *GetInitialPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetInitialPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialDistance */
    if ( strcmp ( argv[2], "SetInitialDistance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialDistance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialDistance */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialDistance(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetInitialDistance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialDistanceMinValue */
    if ( strcmp ( argv[2], "GetInitialDistanceMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialDistanceMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialDistanceMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialDistanceMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetInitialDistanceMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialDistanceMaxValue */
    if ( strcmp ( argv[2], "GetInitialDistanceMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialDistanceMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialDistanceMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialDistanceMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetInitialDistanceMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialDistance */
    if ( strcmp ( argv[2], "GetInitialDistance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialDistance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialDistance */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialDistance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetInitialDistance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRMSError */
    if ( strcmp ( argv[2], "SetRMSError" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRMSError" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRMSError */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetRMSError(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetRMSError */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRMSErrorMinValue */
    if ( strcmp ( argv[2], "GetRMSErrorMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRMSErrorMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRMSErrorMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetRMSErrorMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetRMSErrorMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRMSErrorMaxValue */
    if ( strcmp ( argv[2], "GetRMSErrorMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRMSErrorMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRMSErrorMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetRMSErrorMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetRMSErrorMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRMSError */
    if ( strcmp ( argv[2], "GetRMSError" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRMSError" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRMSError */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetRMSError();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetRMSError */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLevelsetIterations */
    if ( strcmp ( argv[2], "SetLevelsetIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLevelsetIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLevelsetIterations */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetLevelsetIterations(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetLevelsetIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLevelsetIterationsMinValue */
    if ( strcmp ( argv[2], "GetLevelsetIterationsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLevelsetIterationsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLevelsetIterationsMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetLevelsetIterationsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetLevelsetIterationsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLevelsetIterationsMaxValue */
    if ( strcmp ( argv[2], "GetLevelsetIterationsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLevelsetIterationsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLevelsetIterationsMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetLevelsetIterationsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetLevelsetIterationsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLevelsetIterations */
    if ( strcmp ( argv[2], "GetLevelsetIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLevelsetIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLevelsetIterations */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetLevelsetIterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetLevelsetIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInputLevelSet */
    if ( strcmp ( argv[2], "GetInputLevelSet" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInputLevelSet" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInputLevelSet */
    Tcl_DStringAppendElement ( &dString, " Internal Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetInputLevelSet();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetInputLevelSet */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetIterationName */
    if ( strcmp ( argv[2], "SetIterationName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetIterationName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetIterationName */
    Tcl_DStringAppendElement ( &dString, " Debug Output \n" );
    Tcl_DStringAppendElement ( &dString, "void SetIterationName(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetIterationName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationName */
    if ( strcmp ( argv[2], "GetIterationName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationName */
    Tcl_DStringAppendElement ( &dString, " Debug Output \n" );
    Tcl_DStringAppendElement ( &dString, "char *GetIterationName();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetIterationName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutputStatus */
    if ( strcmp ( argv[2], "SetOutputStatus" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutputStatus" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutputStatus */
    Tcl_DStringAppendElement ( &dString, " Last Output\n" );
    Tcl_DStringAppendElement ( &dString, "void SetOutputStatus(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetOutputStatus */

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
    Tcl_DStringAppendElement ( &dString, " Last Output\n" );
    Tcl_DStringAppendElement ( &dString, "char *GetOutputStatus();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetOutputStatus */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetIterationSave */
    if ( strcmp ( argv[2], "SetIterationSave" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetIterationSave" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetIterationSave */
    Tcl_DStringAppendElement ( &dString, " Debug Save On\n" );
    Tcl_DStringAppendElement ( &dString, "void SetIterationSave(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetIterationSave */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationSaveMinValue */
    if ( strcmp ( argv[2], "GetIterationSaveMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationSaveMinValue */
    Tcl_DStringAppendElement ( &dString, " Debug Save On\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationSaveMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetIterationSaveMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationSaveMaxValue */
    if ( strcmp ( argv[2], "GetIterationSaveMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationSaveMaxValue */
    Tcl_DStringAppendElement ( &dString, " Debug Save On\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationSaveMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetIterationSaveMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationSave */
    if ( strcmp ( argv[2], "GetIterationSave" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationSave" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationSave */
    Tcl_DStringAppendElement ( &dString, " Debug Save On\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationSave();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetIterationSave */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IterationSaveOn */
    if ( strcmp ( argv[2], "IterationSaveOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IterationSaveOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IterationSaveOn */
    Tcl_DStringAppendElement ( &dString, " Debug Save On\n" );
    Tcl_DStringAppendElement ( &dString, "void IterationSaveOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for IterationSaveOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IterationSaveOff */
    if ( strcmp ( argv[2], "IterationSaveOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IterationSaveOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IterationSaveOff */
    Tcl_DStringAppendElement ( &dString, " Debug Save On\n" );
    Tcl_DStringAppendElement ( &dString, "void IterationSaveOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for IterationSaveOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetIterationSaveFrequency */
    if ( strcmp ( argv[2], "SetIterationSaveFrequency" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetIterationSaveFrequency" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetIterationSaveFrequency */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetIterationSaveFrequency(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetIterationSaveFrequency */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationSaveFrequencyMinValue */
    if ( strcmp ( argv[2], "GetIterationSaveFrequencyMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveFrequencyMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationSaveFrequencyMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationSaveFrequencyMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetIterationSaveFrequencyMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationSaveFrequencyMaxValue */
    if ( strcmp ( argv[2], "GetIterationSaveFrequencyMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveFrequencyMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationSaveFrequencyMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationSaveFrequencyMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetIterationSaveFrequencyMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationSaveFrequency */
    if ( strcmp ( argv[2], "GetIterationSaveFrequency" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveFrequency" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationSaveFrequency */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationSaveFrequency();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetIterationSaveFrequency */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IterationSaveFrequencyOn */
    if ( strcmp ( argv[2], "IterationSaveFrequencyOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IterationSaveFrequencyOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IterationSaveFrequencyOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void IterationSaveFrequencyOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for IterationSaveFrequencyOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IterationSaveFrequencyOff */
    if ( strcmp ( argv[2], "IterationSaveFrequencyOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IterationSaveFrequencyOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IterationSaveFrequencyOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void IterationSaveFrequencyOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for IterationSaveFrequencyOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetIterationSaveOrientation */
    if ( strcmp ( argv[2], "SetIterationSaveOrientation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetIterationSaveOrientation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetIterationSaveOrientation */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetIterationSaveOrientation(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetIterationSaveOrientation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationSaveOrientationMinValue */
    if ( strcmp ( argv[2], "GetIterationSaveOrientationMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveOrientationMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationSaveOrientationMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationSaveOrientationMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetIterationSaveOrientationMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationSaveOrientationMaxValue */
    if ( strcmp ( argv[2], "GetIterationSaveOrientationMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveOrientationMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationSaveOrientationMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationSaveOrientationMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetIterationSaveOrientationMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationSaveOrientation */
    if ( strcmp ( argv[2], "GetIterationSaveOrientation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationSaveOrientation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationSaveOrientation */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationSaveOrientation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetIterationSaveOrientation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IterationSaveOrientationOn */
    if ( strcmp ( argv[2], "IterationSaveOrientationOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IterationSaveOrientationOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IterationSaveOrientationOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void IterationSaveOrientationOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for IterationSaveOrientationOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IterationSaveOrientationOff */
    if ( strcmp ( argv[2], "IterationSaveOrientationOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IterationSaveOrientationOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IterationSaveOrientationOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void IterationSaveOrientationOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for IterationSaveOrientationOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPropagationWeight */
    if ( strcmp ( argv[2], "SetPropagationWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPropagationWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPropagationWeight */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetPropagationWeight(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetPropagationWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPropagationWeightMinValue */
    if ( strcmp ( argv[2], "GetPropagationWeightMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPropagationWeightMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPropagationWeightMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetPropagationWeightMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetPropagationWeightMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPropagationWeightMaxValue */
    if ( strcmp ( argv[2], "GetPropagationWeightMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPropagationWeightMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPropagationWeightMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetPropagationWeightMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetPropagationWeightMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPropagationWeight */
    if ( strcmp ( argv[2], "GetPropagationWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPropagationWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPropagationWeight */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetPropagationWeight();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetPropagationWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCurvatureWeight */
    if ( strcmp ( argv[2], "SetCurvatureWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCurvatureWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCurvatureWeight */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetCurvatureWeight(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetCurvatureWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCurvatureWeightMinValue */
    if ( strcmp ( argv[2], "GetCurvatureWeightMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCurvatureWeightMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCurvatureWeightMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetCurvatureWeightMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetCurvatureWeightMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCurvatureWeightMaxValue */
    if ( strcmp ( argv[2], "GetCurvatureWeightMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCurvatureWeightMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCurvatureWeightMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetCurvatureWeightMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetCurvatureWeightMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCurvatureWeight */
    if ( strcmp ( argv[2], "GetCurvatureWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCurvatureWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCurvatureWeight */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetCurvatureWeight();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetCurvatureWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAdvectionWeight */
    if ( strcmp ( argv[2], "SetAdvectionWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAdvectionWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAdvectionWeight */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetAdvectionWeight(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetAdvectionWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAdvectionWeightMinValue */
    if ( strcmp ( argv[2], "GetAdvectionWeightMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAdvectionWeightMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAdvectionWeightMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetAdvectionWeightMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetAdvectionWeightMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAdvectionWeightMaxValue */
    if ( strcmp ( argv[2], "GetAdvectionWeightMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAdvectionWeightMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAdvectionWeightMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetAdvectionWeightMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetAdvectionWeightMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAdvectionWeight */
    if ( strcmp ( argv[2], "GetAdvectionWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAdvectionWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAdvectionWeight */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetAdvectionWeight();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetAdvectionWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUpdatePercentage */
    if ( strcmp ( argv[2], "SetUpdatePercentage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUpdatePercentage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUpdatePercentage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetUpdatePercentage(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetUpdatePercentage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUpdatePercentageMinValue */
    if ( strcmp ( argv[2], "GetUpdatePercentageMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUpdatePercentageMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUpdatePercentageMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetUpdatePercentageMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetUpdatePercentageMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUpdatePercentageMaxValue */
    if ( strcmp ( argv[2], "GetUpdatePercentageMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUpdatePercentageMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUpdatePercentageMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetUpdatePercentageMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetUpdatePercentageMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUpdatePercentage */
    if ( strcmp ( argv[2], "GetUpdatePercentage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUpdatePercentage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUpdatePercentage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetUpdatePercentage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetUpdatePercentage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRealIteration */
    if ( strcmp ( argv[2], "SetRealIteration" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRealIteration" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRealIteration */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetRealIteration(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for SetRealIteration */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRealIteration */
    if ( strcmp ( argv[2], "GetRealIteration" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRealIteration" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRealIteration */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetRealIteration();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseLevelSetFilter" );
    /* Closing for GetRealIteration */

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
