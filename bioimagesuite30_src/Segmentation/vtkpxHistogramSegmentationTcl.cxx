// tcl wrapper for vtkpxHistogramSegmentation object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxHistogramSegmentation.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxHistogramSegmentationNewCommand()
{
  vtkpxHistogramSegmentation *temp = vtkpxHistogramSegmentation::New();
  return static_cast<ClientData>(temp);
}

int vtkpxSimpleImageToImageFilterCppCommand(vtkpxSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxHistogramSegmentationCppCommand(vtkpxHistogramSegmentation *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxHistogramSegmentationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxHistogramSegmentationCppCommand(static_cast<vtkpxHistogramSegmentation *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxHistogramSegmentationCppCommand(vtkpxHistogramSegmentation *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxHistogramSegmentation",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxSimpleImageToImageFilterCppCommand(static_cast<vtkpxSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxSimpleImageToImageFilter"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxHistogramSegmentation  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxHistogramSegmentation");
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
    vtkpxHistogramSegmentation  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxHistogramSegmentation");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxHistogramSegmentation  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxHistogramSegmentation");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDistributionModel",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDistributionModel();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetDistributionModel",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetDistributionModel(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDistributionModelMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDistributionModelMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDistributionModelMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDistributionModelMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRobustScaleFactor",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetRobustScaleFactor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRobustScaleFactorMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetRobustScaleFactorMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRobustScaleFactorMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetRobustScaleFactorMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRobustScaleFactor",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetRobustScaleFactor();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxSigmaRatio",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMaxSigmaRatio();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMaxSigmaRatio",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMaxSigmaRatio(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaxSigmaRatioMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMaxSigmaRatioMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxSigmaRatioMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMaxSigmaRatioMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfClasses",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfClasses();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNumberOfClasses",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNumberOfClasses(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfClassesMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfClassesMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfClassesMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfClassesMaxValue();
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
  if ((!strcmp("GetKeyChannel",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetKeyChannel();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetKeyChannel",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetKeyChannel(temp0);
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
  if ((!strcmp("GetNumberOfBinsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfBinsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfBinsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfBinsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetConvergence",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetConvergence();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetConvergence",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetConvergence(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetConvergenceMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetConvergenceMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetConvergenceMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetConvergenceMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSigma",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSigma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSigma",argv[1]))&&(argc == 3))
    {
    float    temp0;
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
  if ((!strcmp("GetSigmaMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSigmaMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSigmaMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSigmaMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetGenerateLabels",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetGenerateLabels();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetGenerateLabels",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetGenerateLabels(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetGenerateLabelsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetGenerateLabelsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetGenerateLabelsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetGenerateLabelsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GenerateLabelsOn",argv[1]))&&(argc == 2))
    {
    op->GenerateLabelsOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GenerateLabelsOff",argv[1]))&&(argc == 2))
    {
    op->GenerateLabelsOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetInitialParameters",argv[1]))&&(argc == 2))
    {
    vtkFloatArray  *temp20;
    temp20 = (op)->GetInitialParameters();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
  if ((!strcmp("SetInitialParameters",argv[1]))&&(argc == 3))
    {
    vtkFloatArray  *temp0;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    if (!error)
    {
    op->SetInitialParameters(temp0);
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
  if ((!strcmp("GetParameters",argv[1]))&&(argc == 2))
    {
    vtkFloatArray  *temp20;
    temp20 = (op)->GetParameters();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
  if ((!strcmp("GetHistogram",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetHistogram();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetOutlierDetection",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetOutlierDetection(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutlierDetectionMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetOutlierDetectionMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetOutlierDetectionMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetOutlierDetectionMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetOutlierDetection",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetOutlierDetection();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("OutlierDetectionOn",argv[1]))&&(argc == 2))
    {
    op->OutlierDetectionOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("OutlierDetectionOff",argv[1]))&&(argc == 2))
    {
    op->OutlierDetectionOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOutlierProbability",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetOutlierProbability(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutlierProbability",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetOutlierProbability();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPreprocessingMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetPreprocessingMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPreprocessingModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPreprocessingModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPreprocessingModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPreprocessingModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPreprocessingMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPreprocessingMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("HistogramNormalizeImage",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    float    temp1;
    float    temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->HistogramNormalizeImage(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeFatValues",argv[1]))&&(argc == 9))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkpxSplineStackSource  *temp2;
    int      temp3;
    int      temp4;
    int      temp5;
    int      temp6;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (Tcl_GetInt(interp,argv[8],&tempi) != TCL_OK) error = 1;
    temp6 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeFatValues(temp0,temp1,temp2,temp3,temp4,temp5,temp6);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeCaprioFatValues",argv[1]))&&(argc == 9))
    {
    vtkImageData  *temp0;
    vtkpxSplineStackSource  *temp1;
    vtkpxSplineStackSource  *temp2;
    vtkpxSplineStackSource  *temp3;
    vtkpxSplineStackSource  *temp4;
    float    temp5;
    float    temp6;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    temp2 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    temp3 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    temp4 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[6],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (Tcl_GetDouble(interp,argv[8],&tempd) != TCL_OK) error = 1;
    temp6 = tempd;
    if (!error)
    {
    temp20 = (op)->ComputeCaprioFatValues(temp0,temp1,temp2,temp3,temp4,temp5,temp6);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeSegmentedMapStatistics",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkFloatArray  *temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkFloatArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeSegmentedMapStatistics(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateRayleighSynthetic",argv[1]))&&(argc == 7))
    {
    float    temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    float    temp4;
    vtkImageData  *temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (!error)
    {
    temp20 = (op)->CreateRayleighSynthetic(temp0,temp1,temp2,temp3,temp4);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetThreshold",argv[1]))&&(argc == 6))
    {
    float    temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    float    temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (!error)
    {
    temp20 = (op)->GetThreshold(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetGaussianDifference",argv[1]))&&(argc == 7))
    {
    float    temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    float    temp4;
    float    temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (!error)
    {
    temp20 = (op)->GetGaussianDifference(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxHistogramSegmentationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxHistogramSegmentation:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDistributionModel\n",NULL);
    Tcl_AppendResult(interp,"  SetDistributionModel\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDistributionModelMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDistributionModelMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetRobustScaleFactor\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRobustScaleFactorMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRobustScaleFactorMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRobustScaleFactor\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxSigmaRatio\n",NULL);
    Tcl_AppendResult(interp,"  SetMaxSigmaRatio\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxSigmaRatioMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxSigmaRatioMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfClasses\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfClasses\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfClassesMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfClassesMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIterations\n",NULL);
    Tcl_AppendResult(interp,"  SetIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetKeyChannel\n",NULL);
    Tcl_AppendResult(interp,"  SetKeyChannel\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBins\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfBins\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBinsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBinsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetConvergence\n",NULL);
    Tcl_AppendResult(interp,"  SetConvergence\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetConvergenceMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetConvergenceMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSigma\n",NULL);
    Tcl_AppendResult(interp,"  SetSigma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSigmaMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSigmaMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetGenerateLabels\n",NULL);
    Tcl_AppendResult(interp,"  SetGenerateLabels\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetGenerateLabelsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetGenerateLabelsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GenerateLabelsOn\n",NULL);
    Tcl_AppendResult(interp,"  GenerateLabelsOff\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialParameters\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaskImage\n",NULL);
    Tcl_AppendResult(interp,"  SetMaskImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetParameters\n",NULL);
    Tcl_AppendResult(interp,"  GetHistogram\n",NULL);
    Tcl_AppendResult(interp,"  SetOutlierDetection\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutlierDetectionMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetOutlierDetectionMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetOutlierDetection\n",NULL);
    Tcl_AppendResult(interp,"  OutlierDetectionOn\n",NULL);
    Tcl_AppendResult(interp,"  OutlierDetectionOff\n",NULL);
    Tcl_AppendResult(interp,"  SetOutlierProbability\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutlierProbability\n",NULL);
    Tcl_AppendResult(interp,"  SetPreprocessingMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPreprocessingModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPreprocessingModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPreprocessingMode\n",NULL);
    Tcl_AppendResult(interp,"  HistogramNormalizeImage\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeFatValues\t with 7 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeCaprioFatValues\t with 7 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeSegmentedMapStatistics\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateRayleighSynthetic\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  GetThreshold\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  GetGaussianDifference\t with 5 args\n",NULL);
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
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "GetDistributionModel" );
    Tcl_DStringAppendElement ( &dString, "SetDistributionModel" );
    Tcl_DStringAppendElement ( &dString, "GetDistributionModelMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetDistributionModelMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetRobustScaleFactor" );
    Tcl_DStringAppendElement ( &dString, "GetRobustScaleFactorMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetRobustScaleFactorMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetRobustScaleFactor" );
    Tcl_DStringAppendElement ( &dString, "GetMaxSigmaRatio" );
    Tcl_DStringAppendElement ( &dString, "SetMaxSigmaRatio" );
    Tcl_DStringAppendElement ( &dString, "GetMaxSigmaRatioMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMaxSigmaRatioMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfClasses" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfClasses" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfClassesMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfClassesMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterations" );
    Tcl_DStringAppendElement ( &dString, "SetIterations" );
    Tcl_DStringAppendElement ( &dString, "GetIterationsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterationsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetKeyChannel" );
    Tcl_DStringAppendElement ( &dString, "SetKeyChannel" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBins" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfBins" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBinsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBinsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetConvergence" );
    Tcl_DStringAppendElement ( &dString, "SetConvergence" );
    Tcl_DStringAppendElement ( &dString, "GetConvergenceMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetConvergenceMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetSigma" );
    Tcl_DStringAppendElement ( &dString, "SetSigma" );
    Tcl_DStringAppendElement ( &dString, "GetSigmaMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSigmaMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetGenerateLabels" );
    Tcl_DStringAppendElement ( &dString, "SetGenerateLabels" );
    Tcl_DStringAppendElement ( &dString, "GetGenerateLabelsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetGenerateLabelsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GenerateLabelsOn" );
    Tcl_DStringAppendElement ( &dString, "GenerateLabelsOff" );
    Tcl_DStringAppendElement ( &dString, "GetInitialParameters" );
    Tcl_DStringAppendElement ( &dString, "SetInitialParameters" );
    Tcl_DStringAppendElement ( &dString, "GetMaskImage" );
    Tcl_DStringAppendElement ( &dString, "SetMaskImage" );
    Tcl_DStringAppendElement ( &dString, "GetParameters" );
    Tcl_DStringAppendElement ( &dString, "GetHistogram" );
    Tcl_DStringAppendElement ( &dString, "SetOutlierDetection" );
    Tcl_DStringAppendElement ( &dString, "GetOutlierDetectionMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetOutlierDetectionMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetOutlierDetection" );
    Tcl_DStringAppendElement ( &dString, "OutlierDetectionOn" );
    Tcl_DStringAppendElement ( &dString, "OutlierDetectionOff" );
    Tcl_DStringAppendElement ( &dString, "SetOutlierProbability" );
    Tcl_DStringAppendElement ( &dString, "GetOutlierProbability" );
    Tcl_DStringAppendElement ( &dString, "SetPreprocessingMode" );
    Tcl_DStringAppendElement ( &dString, "GetPreprocessingModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetPreprocessingModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetPreprocessingMode" );
    Tcl_DStringAppendElement ( &dString, "HistogramNormalizeImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeFatValues" );
    Tcl_DStringAppendElement ( &dString, "ComputeCaprioFatValues" );
    Tcl_DStringAppendElement ( &dString, "ComputeSegmentedMapStatistics" );
    Tcl_DStringAppendElement ( &dString, "CreateRayleighSynthetic" );
    Tcl_DStringAppendElement ( &dString, "GetThreshold" );
    Tcl_DStringAppendElement ( &dString, "GetGaussianDifference" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxHistogramSegmentation *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDistributionModel */
    if ( strcmp ( argv[2], "GetDistributionModel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDistributionModel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDistributionModel */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDistributionModel();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetDistributionModel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDistributionModel */
    if ( strcmp ( argv[2], "SetDistributionModel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDistributionModel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDistributionModel */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "void SetDistributionModel(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetDistributionModel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDistributionModelMinValue */
    if ( strcmp ( argv[2], "GetDistributionModelMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDistributionModelMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDistributionModelMinValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDistributionModelMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetDistributionModelMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDistributionModelMaxValue */
    if ( strcmp ( argv[2], "GetDistributionModelMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDistributionModelMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDistributionModelMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "int GetDistributionModelMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetDistributionModelMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRobustScaleFactor */
    if ( strcmp ( argv[2], "SetRobustScaleFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRobustScaleFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRobustScaleFactor */
    Tcl_DStringAppendElement ( &dString, " Sigma Scale for Robust Distribution Model (i.e. Sum of Gaussians)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRobustScaleFactor(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetRobustScaleFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRobustScaleFactorMinValue */
    if ( strcmp ( argv[2], "GetRobustScaleFactorMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRobustScaleFactorMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRobustScaleFactorMinValue */
    Tcl_DStringAppendElement ( &dString, " Sigma Scale for Robust Distribution Model (i.e. Sum of Gaussians)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetRobustScaleFactorMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetRobustScaleFactorMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRobustScaleFactorMaxValue */
    if ( strcmp ( argv[2], "GetRobustScaleFactorMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRobustScaleFactorMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRobustScaleFactorMaxValue */
    Tcl_DStringAppendElement ( &dString, " Sigma Scale for Robust Distribution Model (i.e. Sum of Gaussians)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetRobustScaleFactorMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetRobustScaleFactorMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRobustScaleFactor */
    if ( strcmp ( argv[2], "GetRobustScaleFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRobustScaleFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRobustScaleFactor */
    Tcl_DStringAppendElement ( &dString, " Sigma Scale for Robust Distribution Model (i.e. Sum of Gaussians)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetRobustScaleFactor();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetRobustScaleFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxSigmaRatio */
    if ( strcmp ( argv[2], "GetMaxSigmaRatio" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxSigmaRatio" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxSigmaRatio */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMaxSigmaRatio();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetMaxSigmaRatio */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaxSigmaRatio */
    if ( strcmp ( argv[2], "SetMaxSigmaRatio" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaxSigmaRatio" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaxSigmaRatio */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaxSigmaRatio(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetMaxSigmaRatio */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxSigmaRatioMinValue */
    if ( strcmp ( argv[2], "GetMaxSigmaRatioMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxSigmaRatioMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxSigmaRatioMinValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMaxSigmaRatioMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetMaxSigmaRatioMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxSigmaRatioMaxValue */
    if ( strcmp ( argv[2], "GetMaxSigmaRatioMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxSigmaRatioMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxSigmaRatioMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMaxSigmaRatioMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetMaxSigmaRatioMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfClasses */
    if ( strcmp ( argv[2], "GetNumberOfClasses" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfClasses" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfClasses */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfClasses();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetNumberOfClasses */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNumberOfClasses */
    if ( strcmp ( argv[2], "SetNumberOfClasses" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfClasses" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNumberOfClasses */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfClasses(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetNumberOfClasses */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfClassesMinValue */
    if ( strcmp ( argv[2], "GetNumberOfClassesMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfClassesMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfClassesMinValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfClassesMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetNumberOfClassesMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfClassesMaxValue */
    if ( strcmp ( argv[2], "GetNumberOfClassesMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfClassesMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfClassesMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfClassesMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetNumberOfClassesMaxValue */

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
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetIterations */

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
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetIterations(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetIterationsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetKeyChannel */
    if ( strcmp ( argv[2], "GetKeyChannel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetKeyChannel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetKeyChannel */
    Tcl_DStringAppendElement ( &dString, " Key Channel i.e. channel to perform initial clustering on\n" );
    Tcl_DStringAppendElement ( &dString, "int GetKeyChannel();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetKeyChannel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetKeyChannel */
    if ( strcmp ( argv[2], "SetKeyChannel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetKeyChannel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetKeyChannel */
    Tcl_DStringAppendElement ( &dString, " Key Channel i.e. channel to perform initial clustering on\n" );
    Tcl_DStringAppendElement ( &dString, "void SetKeyChannel(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetKeyChannel */

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
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfBins();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetNumberOfBins */

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
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfBins(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetNumberOfBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfBinsMinValue */
    if ( strcmp ( argv[2], "GetNumberOfBinsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBinsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfBinsMinValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfBinsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetNumberOfBinsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfBinsMaxValue */
    if ( strcmp ( argv[2], "GetNumberOfBinsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBinsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfBinsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfBinsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetNumberOfBinsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetConvergence */
    if ( strcmp ( argv[2], "GetConvergence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetConvergence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetConvergence */
    Tcl_DStringAppendElement ( &dString, " Convergence for CMeans Values (<1.0)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetConvergence();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetConvergence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetConvergence */
    if ( strcmp ( argv[2], "SetConvergence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetConvergence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetConvergence */
    Tcl_DStringAppendElement ( &dString, " Convergence for CMeans Values (<1.0)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetConvergence(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetConvergence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetConvergenceMinValue */
    if ( strcmp ( argv[2], "GetConvergenceMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetConvergenceMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetConvergenceMinValue */
    Tcl_DStringAppendElement ( &dString, " Convergence for CMeans Values (<1.0)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetConvergenceMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetConvergenceMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetConvergenceMaxValue */
    if ( strcmp ( argv[2], "GetConvergenceMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetConvergenceMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetConvergenceMaxValue */
    Tcl_DStringAppendElement ( &dString, " Convergence for CMeans Values (<1.0)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetConvergenceMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetConvergenceMaxValue */

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
    Tcl_DStringAppendElement ( &dString, " Histogram Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSigma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, " Histogram Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSigma(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSigmaMinValue */
    if ( strcmp ( argv[2], "GetSigmaMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSigmaMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSigmaMinValue */
    Tcl_DStringAppendElement ( &dString, " Histogram Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSigmaMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetSigmaMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSigmaMaxValue */
    if ( strcmp ( argv[2], "GetSigmaMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSigmaMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSigmaMaxValue */
    Tcl_DStringAppendElement ( &dString, " Histogram Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSigmaMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetSigmaMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGenerateLabels */
    if ( strcmp ( argv[2], "GetGenerateLabels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGenerateLabels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGenerateLabels */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetGenerateLabels();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetGenerateLabels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetGenerateLabels */
    if ( strcmp ( argv[2], "SetGenerateLabels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetGenerateLabels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetGenerateLabels */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetGenerateLabels(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetGenerateLabels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGenerateLabelsMinValue */
    if ( strcmp ( argv[2], "GetGenerateLabelsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGenerateLabelsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGenerateLabelsMinValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetGenerateLabelsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetGenerateLabelsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGenerateLabelsMaxValue */
    if ( strcmp ( argv[2], "GetGenerateLabelsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGenerateLabelsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGenerateLabelsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetGenerateLabelsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetGenerateLabelsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateLabelsOn */
    if ( strcmp ( argv[2], "GenerateLabelsOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateLabelsOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateLabelsOn */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void GenerateLabelsOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GenerateLabelsOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateLabelsOff */
    if ( strcmp ( argv[2], "GenerateLabelsOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateLabelsOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateLabelsOff */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void GenerateLabelsOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GenerateLabelsOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialParameters */
    if ( strcmp ( argv[2], "GetInitialParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialParameters */
    Tcl_DStringAppendElement ( &dString, " Get Outputs\n" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray *GetInitialParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetInitialParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialParameters */
    if ( strcmp ( argv[2], "SetInitialParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialParameters */
    Tcl_DStringAppendElement ( &dString, " Get Outputs\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialParameters(vtkFloatArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetInitialParameters */

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
    Tcl_DStringAppendElement ( &dString, " Get Outputs\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetMaskImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
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
    Tcl_DStringAppendElement ( &dString, " Get Outputs\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaskImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetMaskImage */

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
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetParameters */
    Tcl_DStringAppendElement ( &dString, " Get Outputs\n" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray *GetParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetParameters */

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
    Tcl_DStringAppendElement ( &dString, " Get Outputs\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetHistogram();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutlierDetection */
    if ( strcmp ( argv[2], "SetOutlierDetection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutlierDetection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutlierDetection */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "void SetOutlierDetection(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetOutlierDetection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutlierDetectionMinValue */
    if ( strcmp ( argv[2], "GetOutlierDetectionMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutlierDetectionMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutlierDetectionMinValue */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "int GetOutlierDetectionMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetOutlierDetectionMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutlierDetectionMaxValue */
    if ( strcmp ( argv[2], "GetOutlierDetectionMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutlierDetectionMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutlierDetectionMaxValue */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "int GetOutlierDetectionMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetOutlierDetectionMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutlierDetection */
    if ( strcmp ( argv[2], "GetOutlierDetection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutlierDetection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutlierDetection */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "int GetOutlierDetection();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetOutlierDetection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OutlierDetectionOn */
    if ( strcmp ( argv[2], "OutlierDetectionOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OutlierDetectionOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OutlierDetectionOn */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "void OutlierDetectionOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for OutlierDetectionOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OutlierDetectionOff */
    if ( strcmp ( argv[2], "OutlierDetectionOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OutlierDetectionOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OutlierDetectionOff */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "void OutlierDetectionOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for OutlierDetectionOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutlierProbability */
    if ( strcmp ( argv[2], "SetOutlierProbability" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutlierProbability" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutlierProbability */
    Tcl_DStringAppendElement ( &dString, " Outlier Probability\n" );
    Tcl_DStringAppendElement ( &dString, "void SetOutlierProbability(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetOutlierProbability */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutlierProbability */
    if ( strcmp ( argv[2], "GetOutlierProbability" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutlierProbability" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutlierProbability */
    Tcl_DStringAppendElement ( &dString, " Outlier Probability\n" );
    Tcl_DStringAppendElement ( &dString, "float GetOutlierProbability();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetOutlierProbability */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPreprocessingMode */
    if ( strcmp ( argv[2], "SetPreprocessingMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPreprocessingMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPreprocessingMode */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPreprocessingMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for SetPreprocessingMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPreprocessingModeMinValue */
    if ( strcmp ( argv[2], "GetPreprocessingModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPreprocessingModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPreprocessingModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPreprocessingModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetPreprocessingModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPreprocessingModeMaxValue */
    if ( strcmp ( argv[2], "GetPreprocessingModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPreprocessingModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPreprocessingModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPreprocessingModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetPreprocessingModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPreprocessingMode */
    if ( strcmp ( argv[2], "GetPreprocessingMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPreprocessingMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPreprocessingMode */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPreprocessingMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetPreprocessingMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: HistogramNormalizeImage */
    if ( strcmp ( argv[2], "HistogramNormalizeImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "HistogramNormalizeImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for HistogramNormalizeImage */
    Tcl_DStringAppendElement ( &dString, " Normalize Image Using Histogram\n" );
    Tcl_DStringAppendElement ( &dString, "static int HistogramNormalizeImage(vtkImageData *image, float pmin, float pmax);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for HistogramNormalizeImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeFatValues */
    if ( strcmp ( argv[2], "ComputeFatValues" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeFatValues" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeFatValues */
    Tcl_DStringAppendElement ( &dString, " Fat Computation Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeFatValues(vtkImageData *orig_image, vtkImageData *orig_map, vtkpxSplineStackSource *surface, int abd_fat_value, int artifact_value, int subq_fat_value, int numclasses);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for ComputeFatValues */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeCaprioFatValues */
    if ( strcmp ( argv[2], "ComputeCaprioFatValues" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeCaprioFatValues" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeCaprioFatValues */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeCaprioFatValues(vtkImageData *orig_map, vtkpxSplineStackSource *abdomen_surface, vtkpxSplineStackSource *visceral_surface, vtkpxSplineStackSource *fascia_surface, vtkpxSplineStackSource *skin_surface, float minv, float maxv);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for ComputeCaprioFatValues */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeSegmentedMapStatistics */
    if ( strcmp ( argv[2], "ComputeSegmentedMapStatistics" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeSegmentedMapStatistics" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeSegmentedMapStatistics */
    Tcl_DStringAppendElement ( &dString, " ROI Analysis Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "static int ComputeSegmentedMapStatistics(vtkImageData *image, vtkImageData *map, vtkFloatArray *output_values);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for ComputeSegmentedMapStatistics */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateRayleighSynthetic */
    if ( strcmp ( argv[2], "CreateRayleighSynthetic" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateRayleighSynthetic" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateRayleighSynthetic */
    Tcl_DStringAppendElement ( &dString, " Create Synthetic Two Channel Image (Three concentric circles) with Rayleigh Noise\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *CreateRayleighSynthetic(float beta, float m1, float sigma1, float m2, float sigma2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for CreateRayleighSynthetic */

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
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetThreshold */
    Tcl_DStringAppendElement ( &dString, " Get Threshold\n" );
    Tcl_DStringAppendElement ( &dString, "static float GetThreshold(float mean1, float mean2, float sigma1, float sigma2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGaussianDifference */
    if ( strcmp ( argv[2], "GetGaussianDifference" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGaussianDifference" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGaussianDifference */
    Tcl_DStringAppendElement ( &dString, " Get Threshold\n" );
    Tcl_DStringAppendElement ( &dString, "static float GetGaussianDifference(float x, float m1, float m2, float var1, float var2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxHistogramSegmentation" );
    /* Closing for GetGaussianDifference */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxSimpleImageToImageFilterCppCommand(static_cast<vtkpxSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
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
