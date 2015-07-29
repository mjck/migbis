// tcl wrapper for vtkpxMRFSegmentation object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxMRFSegmentation.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxMRFSegmentationNewCommand()
{
  vtkpxMRFSegmentation *temp = vtkpxMRFSegmentation::New();
  return static_cast<ClientData>(temp);
}

int vtkpxHistogramSegmentationCppCommand(vtkpxHistogramSegmentation *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxMRFSegmentationCppCommand(vtkpxMRFSegmentation *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxMRFSegmentationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxMRFSegmentationCppCommand(static_cast<vtkpxMRFSegmentation *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxMRFSegmentationCppCommand(vtkpxMRFSegmentation *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxMRFSegmentation",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxHistogramSegmentationCppCommand(static_cast<vtkpxHistogramSegmentation *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxHistogramSegmentation"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxMRFSegmentation  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxMRFSegmentation");
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
    vtkpxMRFSegmentation  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxMRFSegmentation");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxMRFSegmentation  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxMRFSegmentation");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSingleChannel",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSingleChannel();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSingleChannel",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetSingleChannel(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSingleChannelMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSingleChannelMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSingleChannelMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSingleChannelMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SingleChannelOn",argv[1]))&&(argc == 2))
    {
    op->SingleChannelOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SingleChannelOff",argv[1]))&&(argc == 2))
    {
    op->SingleChannelOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetComputeBiasField",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetComputeBiasField();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetComputeBiasField",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetComputeBiasField(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetComputeBiasFieldMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetComputeBiasFieldMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetComputeBiasFieldMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetComputeBiasFieldMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ComputeBiasFieldOn",argv[1]))&&(argc == 2))
    {
    op->ComputeBiasFieldOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ComputeBiasFieldOff",argv[1]))&&(argc == 2))
    {
    op->ComputeBiasFieldOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetMRFIterations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMRFIterations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMRFIterations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMRFIterations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMRFIterationsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMRFIterationsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMRFIterationsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMRFIterationsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSmoothness",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSmoothness();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSmoothness",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetSmoothness(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSmoothnessMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSmoothnessMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSmoothnessMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSmoothnessMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBiasFieldFactor",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetBiasFieldFactor();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetBiasFieldFactor",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetBiasFieldFactor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBiasFieldFactorMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetBiasFieldFactorMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBiasFieldFactorMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetBiasFieldFactorMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMRFThreshold",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMRFThreshold();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMRFThreshold",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMRFThreshold(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMRFThresholdMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMRFThresholdMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMRFThresholdMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMRFThresholdMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetImageNoiseSigma",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetImageNoiseSigma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetImageNoiseSigma",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetImageNoiseSigma(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetImageNoiseSigmaMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetImageNoiseSigmaMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetImageNoiseSigmaMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetImageNoiseSigmaMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPartialStep",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPartialStep();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPartialStep",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetPartialStep(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPartialStepMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPartialStepMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPartialStepMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPartialStepMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetHistogramNormalization",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetHistogramNormalization(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHistogramNormalizationMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHistogramNormalizationMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHistogramNormalizationMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHistogramNormalizationMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHistogramNormalization",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHistogramNormalization();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMRFConvergencePercentage",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMRFConvergencePercentage();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMRFConvergencePercentage",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMRFConvergencePercentage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMRFConvergencePercentageMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMRFConvergencePercentageMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMRFConvergencePercentageMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMRFConvergencePercentageMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDebugVoxel",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDebugVoxel();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetDebugVoxel",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetDebugVoxel(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFuzzyClassification",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetFuzzyClassification(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFuzzyClassificationMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFuzzyClassificationMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFuzzyClassificationMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFuzzyClassificationMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFuzzyClassification",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFuzzyClassification();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMultiFrameOutput",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetMultiFrameOutput();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetBiasField",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetBiasField();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetBiasFieldWeights",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetBiasFieldWeights();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetFitQualityMap",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetFitQualityMap(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFitQualityMap",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetFitQualityMap();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxMRFSegmentationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxHistogramSegmentationCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxMRFSegmentation:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSingleChannel\n",NULL);
    Tcl_AppendResult(interp,"  SetSingleChannel\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSingleChannelMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSingleChannelMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SingleChannelOn\n",NULL);
    Tcl_AppendResult(interp,"  SingleChannelOff\n",NULL);
    Tcl_AppendResult(interp,"  GetComputeBiasField\n",NULL);
    Tcl_AppendResult(interp,"  SetComputeBiasField\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetComputeBiasFieldMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetComputeBiasFieldMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBiasFieldOn\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBiasFieldOff\n",NULL);
    Tcl_AppendResult(interp,"  GetMRFIterations\n",NULL);
    Tcl_AppendResult(interp,"  SetMRFIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMRFIterationsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMRFIterationsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSmoothness\n",NULL);
    Tcl_AppendResult(interp,"  SetSmoothness\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSmoothnessMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSmoothnessMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBiasFieldFactor\n",NULL);
    Tcl_AppendResult(interp,"  SetBiasFieldFactor\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBiasFieldFactorMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBiasFieldFactorMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMRFThreshold\n",NULL);
    Tcl_AppendResult(interp,"  SetMRFThreshold\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMRFThresholdMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMRFThresholdMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetImageNoiseSigma\n",NULL);
    Tcl_AppendResult(interp,"  SetImageNoiseSigma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetImageNoiseSigmaMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetImageNoiseSigmaMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPartialStep\n",NULL);
    Tcl_AppendResult(interp,"  SetPartialStep\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPartialStepMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPartialStepMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetHistogramNormalization\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHistogramNormalizationMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHistogramNormalizationMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHistogramNormalization\n",NULL);
    Tcl_AppendResult(interp,"  GetMRFConvergencePercentage\n",NULL);
    Tcl_AppendResult(interp,"  SetMRFConvergencePercentage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMRFConvergencePercentageMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMRFConvergencePercentageMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDebugVoxel\n",NULL);
    Tcl_AppendResult(interp,"  SetDebugVoxel\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetFuzzyClassification\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFuzzyClassificationMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFuzzyClassificationMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFuzzyClassification\n",NULL);
    Tcl_AppendResult(interp,"  GetMultiFrameOutput\n",NULL);
    Tcl_AppendResult(interp,"  GetBiasField\n",NULL);
    Tcl_AppendResult(interp,"  GetBiasFieldWeights\n",NULL);
    Tcl_AppendResult(interp,"  SetFitQualityMap\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFitQualityMap\n",NULL);
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
    vtkpxHistogramSegmentationCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "GetSingleChannel" );
    Tcl_DStringAppendElement ( &dString, "SetSingleChannel" );
    Tcl_DStringAppendElement ( &dString, "GetSingleChannelMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSingleChannelMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SingleChannelOn" );
    Tcl_DStringAppendElement ( &dString, "SingleChannelOff" );
    Tcl_DStringAppendElement ( &dString, "GetComputeBiasField" );
    Tcl_DStringAppendElement ( &dString, "SetComputeBiasField" );
    Tcl_DStringAppendElement ( &dString, "GetComputeBiasFieldMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetComputeBiasFieldMaxValue" );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFieldOn" );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFieldOff" );
    Tcl_DStringAppendElement ( &dString, "GetMRFIterations" );
    Tcl_DStringAppendElement ( &dString, "SetMRFIterations" );
    Tcl_DStringAppendElement ( &dString, "GetMRFIterationsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMRFIterationsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetSmoothness" );
    Tcl_DStringAppendElement ( &dString, "SetSmoothness" );
    Tcl_DStringAppendElement ( &dString, "GetSmoothnessMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSmoothnessMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetBiasFieldFactor" );
    Tcl_DStringAppendElement ( &dString, "SetBiasFieldFactor" );
    Tcl_DStringAppendElement ( &dString, "GetBiasFieldFactorMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetBiasFieldFactorMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMRFThreshold" );
    Tcl_DStringAppendElement ( &dString, "SetMRFThreshold" );
    Tcl_DStringAppendElement ( &dString, "GetMRFThresholdMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMRFThresholdMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetImageNoiseSigma" );
    Tcl_DStringAppendElement ( &dString, "SetImageNoiseSigma" );
    Tcl_DStringAppendElement ( &dString, "GetImageNoiseSigmaMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetImageNoiseSigmaMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetPartialStep" );
    Tcl_DStringAppendElement ( &dString, "SetPartialStep" );
    Tcl_DStringAppendElement ( &dString, "GetPartialStepMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetPartialStepMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetHistogramNormalization" );
    Tcl_DStringAppendElement ( &dString, "GetHistogramNormalizationMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetHistogramNormalizationMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetHistogramNormalization" );
    Tcl_DStringAppendElement ( &dString, "GetMRFConvergencePercentage" );
    Tcl_DStringAppendElement ( &dString, "SetMRFConvergencePercentage" );
    Tcl_DStringAppendElement ( &dString, "GetMRFConvergencePercentageMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMRFConvergencePercentageMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetDebugVoxel" );
    Tcl_DStringAppendElement ( &dString, "SetDebugVoxel" );
    Tcl_DStringAppendElement ( &dString, "SetFuzzyClassification" );
    Tcl_DStringAppendElement ( &dString, "GetFuzzyClassificationMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetFuzzyClassificationMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetFuzzyClassification" );
    Tcl_DStringAppendElement ( &dString, "GetMultiFrameOutput" );
    Tcl_DStringAppendElement ( &dString, "GetBiasField" );
    Tcl_DStringAppendElement ( &dString, "GetBiasFieldWeights" );
    Tcl_DStringAppendElement ( &dString, "SetFitQualityMap" );
    Tcl_DStringAppendElement ( &dString, "GetFitQualityMap" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxHistogramSegmentationCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxMRFSegmentation *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSingleChannel */
    if ( strcmp ( argv[2], "GetSingleChannel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSingleChannel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSingleChannel */
    Tcl_DStringAppendElement ( &dString, " Single Channel i.e. force Single Channel Mode\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSingleChannel();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetSingleChannel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSingleChannel */
    if ( strcmp ( argv[2], "SetSingleChannel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSingleChannel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSingleChannel */
    Tcl_DStringAppendElement ( &dString, " Single Channel i.e. force Single Channel Mode\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSingleChannel(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetSingleChannel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSingleChannelMinValue */
    if ( strcmp ( argv[2], "GetSingleChannelMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSingleChannelMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSingleChannelMinValue */
    Tcl_DStringAppendElement ( &dString, " Single Channel i.e. force Single Channel Mode\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSingleChannelMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetSingleChannelMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSingleChannelMaxValue */
    if ( strcmp ( argv[2], "GetSingleChannelMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSingleChannelMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSingleChannelMaxValue */
    Tcl_DStringAppendElement ( &dString, " Single Channel i.e. force Single Channel Mode\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSingleChannelMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetSingleChannelMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SingleChannelOn */
    if ( strcmp ( argv[2], "SingleChannelOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SingleChannelOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SingleChannelOn */
    Tcl_DStringAppendElement ( &dString, " Single Channel i.e. force Single Channel Mode\n" );
    Tcl_DStringAppendElement ( &dString, "void SingleChannelOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SingleChannelOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SingleChannelOff */
    if ( strcmp ( argv[2], "SingleChannelOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SingleChannelOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SingleChannelOff */
    Tcl_DStringAppendElement ( &dString, " Single Channel i.e. force Single Channel Mode\n" );
    Tcl_DStringAppendElement ( &dString, "void SingleChannelOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SingleChannelOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetComputeBiasField */
    if ( strcmp ( argv[2], "GetComputeBiasField" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetComputeBiasField" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetComputeBiasField */
    Tcl_DStringAppendElement ( &dString, " Compute Bias Field\n" );
    Tcl_DStringAppendElement ( &dString, "int GetComputeBiasField();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetComputeBiasField */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetComputeBiasField */
    if ( strcmp ( argv[2], "SetComputeBiasField" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetComputeBiasField" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetComputeBiasField */
    Tcl_DStringAppendElement ( &dString, " Compute Bias Field\n" );
    Tcl_DStringAppendElement ( &dString, "void SetComputeBiasField(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetComputeBiasField */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetComputeBiasFieldMinValue */
    if ( strcmp ( argv[2], "GetComputeBiasFieldMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetComputeBiasFieldMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetComputeBiasFieldMinValue */
    Tcl_DStringAppendElement ( &dString, " Compute Bias Field\n" );
    Tcl_DStringAppendElement ( &dString, "int GetComputeBiasFieldMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetComputeBiasFieldMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetComputeBiasFieldMaxValue */
    if ( strcmp ( argv[2], "GetComputeBiasFieldMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetComputeBiasFieldMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetComputeBiasFieldMaxValue */
    Tcl_DStringAppendElement ( &dString, " Compute Bias Field\n" );
    Tcl_DStringAppendElement ( &dString, "int GetComputeBiasFieldMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetComputeBiasFieldMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBiasFieldOn */
    if ( strcmp ( argv[2], "ComputeBiasFieldOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFieldOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBiasFieldOn */
    Tcl_DStringAppendElement ( &dString, " Compute Bias Field\n" );
    Tcl_DStringAppendElement ( &dString, "void ComputeBiasFieldOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for ComputeBiasFieldOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBiasFieldOff */
    if ( strcmp ( argv[2], "ComputeBiasFieldOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFieldOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBiasFieldOff */
    Tcl_DStringAppendElement ( &dString, " Compute Bias Field\n" );
    Tcl_DStringAppendElement ( &dString, "void ComputeBiasFieldOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for ComputeBiasFieldOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMRFIterations */
    if ( strcmp ( argv[2], "GetMRFIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMRFIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMRFIterations */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMRFIterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetMRFIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMRFIterations */
    if ( strcmp ( argv[2], "SetMRFIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMRFIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMRFIterations */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMRFIterations(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetMRFIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMRFIterationsMinValue */
    if ( strcmp ( argv[2], "GetMRFIterationsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMRFIterationsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMRFIterationsMinValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMRFIterationsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetMRFIterationsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMRFIterationsMaxValue */
    if ( strcmp ( argv[2], "GetMRFIterationsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMRFIterationsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMRFIterationsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMRFIterationsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetMRFIterationsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSmoothness */
    if ( strcmp ( argv[2], "GetSmoothness" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSmoothness" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSmoothness */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSmoothness();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetSmoothness */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSmoothness */
    if ( strcmp ( argv[2], "SetSmoothness" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSmoothness" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSmoothness */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSmoothness(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetSmoothness */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSmoothnessMinValue */
    if ( strcmp ( argv[2], "GetSmoothnessMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSmoothnessMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSmoothnessMinValue */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSmoothnessMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetSmoothnessMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSmoothnessMaxValue */
    if ( strcmp ( argv[2], "GetSmoothnessMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSmoothnessMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSmoothnessMaxValue */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSmoothnessMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetSmoothnessMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBiasFieldFactor */
    if ( strcmp ( argv[2], "GetBiasFieldFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBiasFieldFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBiasFieldFactor */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetBiasFieldFactor();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetBiasFieldFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBiasFieldFactor */
    if ( strcmp ( argv[2], "SetBiasFieldFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBiasFieldFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBiasFieldFactor */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "void SetBiasFieldFactor(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetBiasFieldFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBiasFieldFactorMinValue */
    if ( strcmp ( argv[2], "GetBiasFieldFactorMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBiasFieldFactorMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBiasFieldFactorMinValue */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetBiasFieldFactorMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetBiasFieldFactorMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBiasFieldFactorMaxValue */
    if ( strcmp ( argv[2], "GetBiasFieldFactorMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBiasFieldFactorMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBiasFieldFactorMaxValue */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetBiasFieldFactorMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetBiasFieldFactorMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMRFThreshold */
    if ( strcmp ( argv[2], "GetMRFThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMRFThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMRFThreshold */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMRFThreshold();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetMRFThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMRFThreshold */
    if ( strcmp ( argv[2], "SetMRFThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMRFThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMRFThreshold */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMRFThreshold(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetMRFThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMRFThresholdMinValue */
    if ( strcmp ( argv[2], "GetMRFThresholdMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMRFThresholdMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMRFThresholdMinValue */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMRFThresholdMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetMRFThresholdMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMRFThresholdMaxValue */
    if ( strcmp ( argv[2], "GetMRFThresholdMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMRFThresholdMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMRFThresholdMaxValue */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMRFThresholdMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetMRFThresholdMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageNoiseSigma */
    if ( strcmp ( argv[2], "GetImageNoiseSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageNoiseSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageNoiseSigma */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetImageNoiseSigma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetImageNoiseSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetImageNoiseSigma */
    if ( strcmp ( argv[2], "SetImageNoiseSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetImageNoiseSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetImageNoiseSigma */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "void SetImageNoiseSigma(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetImageNoiseSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageNoiseSigmaMinValue */
    if ( strcmp ( argv[2], "GetImageNoiseSigmaMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageNoiseSigmaMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageNoiseSigmaMinValue */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetImageNoiseSigmaMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetImageNoiseSigmaMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageNoiseSigmaMaxValue */
    if ( strcmp ( argv[2], "GetImageNoiseSigmaMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageNoiseSigmaMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageNoiseSigmaMaxValue */
    Tcl_DStringAppendElement ( &dString, " Weight of MRF\n" );
    Tcl_DStringAppendElement ( &dString, "float GetImageNoiseSigmaMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetImageNoiseSigmaMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPartialStep */
    if ( strcmp ( argv[2], "GetPartialStep" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPartialStep" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPartialStep */
    Tcl_DStringAppendElement ( &dString, " Partial Step\n" );
    Tcl_DStringAppendElement ( &dString, "float GetPartialStep();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetPartialStep */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPartialStep */
    if ( strcmp ( argv[2], "SetPartialStep" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPartialStep" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPartialStep */
    Tcl_DStringAppendElement ( &dString, " Partial Step\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPartialStep(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetPartialStep */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPartialStepMinValue */
    if ( strcmp ( argv[2], "GetPartialStepMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPartialStepMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPartialStepMinValue */
    Tcl_DStringAppendElement ( &dString, " Partial Step\n" );
    Tcl_DStringAppendElement ( &dString, "float GetPartialStepMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetPartialStepMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPartialStepMaxValue */
    if ( strcmp ( argv[2], "GetPartialStepMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPartialStepMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPartialStepMaxValue */
    Tcl_DStringAppendElement ( &dString, " Partial Step\n" );
    Tcl_DStringAppendElement ( &dString, "float GetPartialStepMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetPartialStepMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHistogramNormalization */
    if ( strcmp ( argv[2], "SetHistogramNormalization" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHistogramNormalization" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHistogramNormalization */
    Tcl_DStringAppendElement ( &dString, " Do Histogram Normalization 0.01:0.99\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHistogramNormalization(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetHistogramNormalization */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHistogramNormalizationMinValue */
    if ( strcmp ( argv[2], "GetHistogramNormalizationMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHistogramNormalizationMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHistogramNormalizationMinValue */
    Tcl_DStringAppendElement ( &dString, " Do Histogram Normalization 0.01:0.99\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHistogramNormalizationMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetHistogramNormalizationMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHistogramNormalizationMaxValue */
    if ( strcmp ( argv[2], "GetHistogramNormalizationMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHistogramNormalizationMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHistogramNormalizationMaxValue */
    Tcl_DStringAppendElement ( &dString, " Do Histogram Normalization 0.01:0.99\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHistogramNormalizationMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetHistogramNormalizationMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHistogramNormalization */
    if ( strcmp ( argv[2], "GetHistogramNormalization" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHistogramNormalization" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHistogramNormalization */
    Tcl_DStringAppendElement ( &dString, " Do Histogram Normalization 0.01:0.99\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHistogramNormalization();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetHistogramNormalization */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMRFConvergencePercentage */
    if ( strcmp ( argv[2], "GetMRFConvergencePercentage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMRFConvergencePercentage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMRFConvergencePercentage */
    Tcl_DStringAppendElement ( &dString, " Max Percentage of Voxels changing needed for iteration to converge (0 == all voxels)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMRFConvergencePercentage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetMRFConvergencePercentage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMRFConvergencePercentage */
    if ( strcmp ( argv[2], "SetMRFConvergencePercentage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMRFConvergencePercentage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMRFConvergencePercentage */
    Tcl_DStringAppendElement ( &dString, " Max Percentage of Voxels changing needed for iteration to converge (0 == all voxels)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMRFConvergencePercentage(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetMRFConvergencePercentage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMRFConvergencePercentageMinValue */
    if ( strcmp ( argv[2], "GetMRFConvergencePercentageMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMRFConvergencePercentageMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMRFConvergencePercentageMinValue */
    Tcl_DStringAppendElement ( &dString, " Max Percentage of Voxels changing needed for iteration to converge (0 == all voxels)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMRFConvergencePercentageMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetMRFConvergencePercentageMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMRFConvergencePercentageMaxValue */
    if ( strcmp ( argv[2], "GetMRFConvergencePercentageMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMRFConvergencePercentageMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMRFConvergencePercentageMaxValue */
    Tcl_DStringAppendElement ( &dString, " Max Percentage of Voxels changing needed for iteration to converge (0 == all voxels)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMRFConvergencePercentageMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetMRFConvergencePercentageMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDebugVoxel */
    if ( strcmp ( argv[2], "GetDebugVoxel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDebugVoxel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDebugVoxel */
    Tcl_DStringAppendElement ( &dString, " Debug Voxel\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDebugVoxel();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetDebugVoxel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDebugVoxel */
    if ( strcmp ( argv[2], "SetDebugVoxel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDebugVoxel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDebugVoxel */
    Tcl_DStringAppendElement ( &dString, " Debug Voxel\n" );
    Tcl_DStringAppendElement ( &dString, "void SetDebugVoxel(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetDebugVoxel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFuzzyClassification */
    if ( strcmp ( argv[2], "SetFuzzyClassification" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFuzzyClassification" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFuzzyClassification */
    Tcl_DStringAppendElement ( &dString, " Fuzzy Classification\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFuzzyClassification(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetFuzzyClassification */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFuzzyClassificationMinValue */
    if ( strcmp ( argv[2], "GetFuzzyClassificationMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFuzzyClassificationMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFuzzyClassificationMinValue */
    Tcl_DStringAppendElement ( &dString, " Fuzzy Classification\n" );
    Tcl_DStringAppendElement ( &dString, "int GetFuzzyClassificationMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetFuzzyClassificationMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFuzzyClassificationMaxValue */
    if ( strcmp ( argv[2], "GetFuzzyClassificationMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFuzzyClassificationMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFuzzyClassificationMaxValue */
    Tcl_DStringAppendElement ( &dString, " Fuzzy Classification\n" );
    Tcl_DStringAppendElement ( &dString, "int GetFuzzyClassificationMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetFuzzyClassificationMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFuzzyClassification */
    if ( strcmp ( argv[2], "GetFuzzyClassification" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFuzzyClassification" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFuzzyClassification */
    Tcl_DStringAppendElement ( &dString, " Fuzzy Classification\n" );
    Tcl_DStringAppendElement ( &dString, "int GetFuzzyClassification();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetFuzzyClassification */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMultiFrameOutput */
    if ( strcmp ( argv[2], "GetMultiFrameOutput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMultiFrameOutput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMultiFrameOutput */
    Tcl_DStringAppendElement ( &dString, " MultiFrameOutput\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetMultiFrameOutput();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetMultiFrameOutput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBiasField */
    if ( strcmp ( argv[2], "GetBiasField" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBiasField" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBiasField */
    Tcl_DStringAppendElement ( &dString, " BiasField\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetBiasField();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetBiasField */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBiasFieldWeights */
    if ( strcmp ( argv[2], "GetBiasFieldWeights" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBiasFieldWeights" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBiasFieldWeights */
    Tcl_DStringAppendElement ( &dString, " BiasFieldWeights\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetBiasFieldWeights();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetBiasFieldWeights */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFitQualityMap */
    if ( strcmp ( argv[2], "SetFitQualityMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFitQualityMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFitQualityMap */
    Tcl_DStringAppendElement ( &dString, " Fit Quality Map\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFitQualityMap(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for SetFitQualityMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFitQualityMap */
    if ( strcmp ( argv[2], "GetFitQualityMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFitQualityMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFitQualityMap */
    Tcl_DStringAppendElement ( &dString, " Fit Quality Map\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetFitQualityMap();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMRFSegmentation" );
    /* Closing for GetFitQualityMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxHistogramSegmentationCppCommand(static_cast<vtkpxHistogramSegmentation *>(op),interp,argc,argv) == TCL_OK)
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
