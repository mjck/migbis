// tcl wrapper for vtkpxAbstractBiasFieldCorrection object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxAbstractBiasFieldCorrection.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxAbstractBiasFieldCorrectionNewCommand()
{
  vtkpxAbstractBiasFieldCorrection *temp = vtkpxAbstractBiasFieldCorrection::New();
  return static_cast<ClientData>(temp);
}

int vtkpxSimpleImageToImageFilterCppCommand(vtkpxSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxAbstractBiasFieldCorrectionCppCommand(vtkpxAbstractBiasFieldCorrection *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxAbstractBiasFieldCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxAbstractBiasFieldCorrectionCppCommand(static_cast<vtkpxAbstractBiasFieldCorrection *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxAbstractBiasFieldCorrectionCppCommand(vtkpxAbstractBiasFieldCorrection *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxAbstractBiasFieldCorrection",argv[1]))
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
    vtkpxAbstractBiasFieldCorrection  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxAbstractBiasFieldCorrection");
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
    vtkpxAbstractBiasFieldCorrection  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxAbstractBiasFieldCorrection");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxAbstractBiasFieldCorrection  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxAbstractBiasFieldCorrection");
    return TCL_OK;
    }
    }
  if ((!strcmp("PrintInformation",argv[1]))&&(argc == 2))
    {
    op->PrintInformation();
    Tcl_ResetResult(interp);
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
  if ((!strcmp("SetFixedNumberOfSlices",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetFixedNumberOfSlices(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFixedNumberOfSlices",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFixedNumberOfSlices();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("FixedNumberOfSlicesOn",argv[1]))&&(argc == 2))
    {
    op->FixedNumberOfSlicesOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("FixedNumberOfSlicesOff",argv[1]))&&(argc == 2))
    {
    op->FixedNumberOfSlicesOff();
    Tcl_ResetResult(interp);
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
  if ((!strcmp("SetFrame",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetFrame(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFrame",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFrame();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
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
  if ((!strcmp("ComputeBiasFit",argv[1]))&&(argc == 3))
    {
    int      temp0;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeBiasFit(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
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
  if ((!strcmp("GetBiasField",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetBiasField();
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
  if ((!strcmp("GetMaskImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetMaskImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetClassParameters",argv[1]))&&(argc == 2))
    {
    vtkFloatArray  *temp20;
    temp20 = (op)->GetClassParameters();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
  if ((!strcmp("SetClassParameters",argv[1]))&&(argc == 3))
    {
    vtkFloatArray  *temp0;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    if (!error)
    {
    op->SetClassParameters(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
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
  if ((!strcmp("GetLambdaMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLambdaMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLambdaMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLambdaMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMetric",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMetric();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMetric",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMetric(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMetricMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMetricMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMetricMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMetricMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMetricToPABIC",argv[1]))&&(argc == 2))
    {
    op->SetMetricToPABIC();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMetricToEntropy",argv[1]))&&(argc == 2))
    {
    op->SetMetricToEntropy();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMetricToMinimumVariance",argv[1]))&&(argc == 2))
    {
    op->SetMetricToMinimumVariance();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("Valley",argv[1]))&&(argc == 4))
    {
    double   temp0;
    double   temp1;
    double   temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->Valley(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Valley2",argv[1]))&&(argc == 4))
    {
    double   temp0;
    double   temp1;
    double   temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->Valley2(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetOutputStatus",argv[1]))&&(argc == 4))
    {
    char    *temp0;
    double   temp1;
    error = 0;

    temp0 = argv[2];
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    op->SetOutputStatus(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
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
  if ((!strcmp("GenerateSynthetic",argv[1]))&&(argc == 4))
    {
    int      temp0;
    double   temp1;
    vtkImageData  *temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->GenerateSynthetic(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetHisto_OutlierDetection",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetHisto_OutlierDetection(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHisto_OutlierDetectionMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_OutlierDetectionMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_OutlierDetectionMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_OutlierDetectionMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_OutlierDetection",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_OutlierDetection();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Histo_OutlierDetectionOn",argv[1]))&&(argc == 2))
    {
    op->Histo_OutlierDetectionOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("Histo_OutlierDetectionOff",argv[1]))&&(argc == 2))
    {
    op->Histo_OutlierDetectionOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetHisto_OutlierProbability",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetHisto_OutlierProbability(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHisto_OutlierProbability",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetHisto_OutlierProbability();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetHisto_MaxSigmaRatio",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetHisto_MaxSigmaRatio(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHisto_MaxSigmaRatioMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetHisto_MaxSigmaRatioMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_MaxSigmaRatioMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetHisto_MaxSigmaRatioMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_MaxSigmaRatio",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetHisto_MaxSigmaRatio();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_NumberOfClasses",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_NumberOfClasses();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetHisto_NumberOfClasses",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetHisto_NumberOfClasses(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHisto_NumberOfClassesMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_NumberOfClassesMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_NumberOfClassesMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_NumberOfClassesMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_NumberOfBins",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_NumberOfBins();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetHisto_NumberOfBins",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetHisto_NumberOfBins(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHisto_NumberOfBinsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_NumberOfBinsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_NumberOfBinsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_NumberOfBinsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_Convergence",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetHisto_Convergence();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetHisto_Convergence",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetHisto_Convergence(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHisto_ConvergenceMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetHisto_ConvergenceMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_ConvergenceMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetHisto_ConvergenceMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_Sigma",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetHisto_Sigma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetHisto_Sigma",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetHisto_Sigma(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHisto_SigmaMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetHisto_SigmaMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_SigmaMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetHisto_SigmaMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_Iterations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_Iterations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetHisto_Iterations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetHisto_Iterations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHisto_IterationsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_IterationsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_IterationsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_IterationsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_PreprocessingMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_PreprocessingMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetHisto_PreprocessingMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetHisto_PreprocessingMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHisto_PreprocessingModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_PreprocessingModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetHisto_PreprocessingModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetHisto_PreprocessingModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ComputeScaleFactor",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    double   temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeScaleFactor(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeCorrectedImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeCorrectedImage(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeMeanPreservingCorrectedImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeMeanPreservingCorrectedImage(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeCorrectedImage",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp2;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeCorrectedImage(temp0,temp1,temp2);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeCorrectedImage",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    int      temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeCorrectedImage(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeMaskCorrectedImage",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    int      temp3;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeMaskCorrectedImage(temp0,temp1,temp2,temp3);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeCorrectedLogarithmImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeCorrectedLogarithmImage(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeMeanPreservingCorrectedLogarithmImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeMeanPreservingCorrectedLogarithmImage(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeCorrectedLogarithmImage",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp2;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeCorrectedLogarithmImage(temp0,temp1,temp2);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ExponentiateBiasField",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ExponentiateBiasField(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateLogReferenceImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->CreateLogReferenceImage(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeRatioBiasFieldImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeRatioBiasFieldImage(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxAbstractBiasFieldCorrectionCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxAbstractBiasFieldCorrection:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  PrintInformation\n",NULL);
    Tcl_AppendResult(interp,"  SetProvideFeedback\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetProvideFeedback\n",NULL);
    Tcl_AppendResult(interp,"  ProvideFeedbackOn\n",NULL);
    Tcl_AppendResult(interp,"  ProvideFeedbackOff\n",NULL);
    Tcl_AppendResult(interp,"  SetFixedNumberOfSlices\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFixedNumberOfSlices\n",NULL);
    Tcl_AppendResult(interp,"  FixedNumberOfSlicesOn\n",NULL);
    Tcl_AppendResult(interp,"  FixedNumberOfSlicesOff\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfLevels\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfLevels\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfIterations\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfSteps\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfSteps\n",NULL);
    Tcl_AppendResult(interp,"  SetResolution\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetResolution\n",NULL);
    Tcl_AppendResult(interp,"  SetStepSize\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetStepSize\n",NULL);
    Tcl_AppendResult(interp,"  SetFrame\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFrame\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerFeedback\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateGradient\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateFunction\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  OnBeginIteration\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeBiasFit\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetWeightImage\n",NULL);
    Tcl_AppendResult(interp,"  GetBiasField\n",NULL);
    Tcl_AppendResult(interp,"  SetMaskImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaskImage\n",NULL);
    Tcl_AppendResult(interp,"  GetClassParameters\n",NULL);
    Tcl_AppendResult(interp,"  SetClassParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetEpsilon\n",NULL);
    Tcl_AppendResult(interp,"  SetEpsilon\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetEpsilonMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetEpsilonMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLambda\n",NULL);
    Tcl_AppendResult(interp,"  SetLambda\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLambdaMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLambdaMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMetric\n",NULL);
    Tcl_AppendResult(interp,"  SetMetric\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMetricMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMetricMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  SetMetricToPABIC\n",NULL);
    Tcl_AppendResult(interp,"  SetMetricToEntropy\n",NULL);
    Tcl_AppendResult(interp,"  SetMetricToMinimumVariance\n",NULL);
    Tcl_AppendResult(interp,"  Valley\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Valley2\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetOutputStatus\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetOutputStatus\n",NULL);
    Tcl_AppendResult(interp,"  GenerateSynthetic\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetHisto_OutlierDetection\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_OutlierDetectionMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_OutlierDetectionMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_OutlierDetection\n",NULL);
    Tcl_AppendResult(interp,"  Histo_OutlierDetectionOn\n",NULL);
    Tcl_AppendResult(interp,"  Histo_OutlierDetectionOff\n",NULL);
    Tcl_AppendResult(interp,"  SetHisto_OutlierProbability\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_OutlierProbability\n",NULL);
    Tcl_AppendResult(interp,"  SetHisto_MaxSigmaRatio\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_MaxSigmaRatioMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_MaxSigmaRatioMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_MaxSigmaRatio\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_NumberOfClasses\n",NULL);
    Tcl_AppendResult(interp,"  SetHisto_NumberOfClasses\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_NumberOfClassesMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_NumberOfClassesMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_NumberOfBins\n",NULL);
    Tcl_AppendResult(interp,"  SetHisto_NumberOfBins\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_NumberOfBinsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_NumberOfBinsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_Convergence\n",NULL);
    Tcl_AppendResult(interp,"  SetHisto_Convergence\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_ConvergenceMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_ConvergenceMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_Sigma\n",NULL);
    Tcl_AppendResult(interp,"  SetHisto_Sigma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_SigmaMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_SigmaMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_Iterations\n",NULL);
    Tcl_AppendResult(interp,"  SetHisto_Iterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_IterationsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_IterationsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_PreprocessingMode\n",NULL);
    Tcl_AppendResult(interp,"  SetHisto_PreprocessingMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_PreprocessingModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetHisto_PreprocessingModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  ComputeScaleFactor\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeCorrectedImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeMeanPreservingCorrectedImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeCorrectedImage\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeCorrectedImage\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeMaskCorrectedImage\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeCorrectedLogarithmImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeMeanPreservingCorrectedLogarithmImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeCorrectedLogarithmImage\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ExponentiateBiasField\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  CreateLogReferenceImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ComputeRatioBiasFieldImage\t with 2 args\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "PrintInformation" );
    Tcl_DStringAppendElement ( &dString, "SetProvideFeedback" );
    Tcl_DStringAppendElement ( &dString, "GetProvideFeedback" );
    Tcl_DStringAppendElement ( &dString, "ProvideFeedbackOn" );
    Tcl_DStringAppendElement ( &dString, "ProvideFeedbackOff" );
    Tcl_DStringAppendElement ( &dString, "SetFixedNumberOfSlices" );
    Tcl_DStringAppendElement ( &dString, "GetFixedNumberOfSlices" );
    Tcl_DStringAppendElement ( &dString, "FixedNumberOfSlicesOn" );
    Tcl_DStringAppendElement ( &dString, "FixedNumberOfSlicesOff" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfLevels" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfLevels" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfIterations" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfIterations" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfSteps" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfSteps" );
    Tcl_DStringAppendElement ( &dString, "SetResolution" );
    Tcl_DStringAppendElement ( &dString, "GetResolution" );
    Tcl_DStringAppendElement ( &dString, "SetStepSize" );
    Tcl_DStringAppendElement ( &dString, "GetStepSize" );
    Tcl_DStringAppendElement ( &dString, "SetFrame" );
    Tcl_DStringAppendElement ( &dString, "GetFrame" );
    Tcl_DStringAppendElement ( &dString, "OptimizerFeedback" );
    Tcl_DStringAppendElement ( &dString, "CalculateGradient" );
    Tcl_DStringAppendElement ( &dString, "CalculateFunction" );
    Tcl_DStringAppendElement ( &dString, "OnBeginIteration" );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFit" );
    Tcl_DStringAppendElement ( &dString, "GetWeightImage" );
    Tcl_DStringAppendElement ( &dString, "GetBiasField" );
    Tcl_DStringAppendElement ( &dString, "SetMaskImage" );
    Tcl_DStringAppendElement ( &dString, "GetMaskImage" );
    Tcl_DStringAppendElement ( &dString, "GetClassParameters" );
    Tcl_DStringAppendElement ( &dString, "SetClassParameters" );
    Tcl_DStringAppendElement ( &dString, "GetEpsilon" );
    Tcl_DStringAppendElement ( &dString, "SetEpsilon" );
    Tcl_DStringAppendElement ( &dString, "GetEpsilonMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetEpsilonMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetLambda" );
    Tcl_DStringAppendElement ( &dString, "SetLambda" );
    Tcl_DStringAppendElement ( &dString, "GetLambdaMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetLambdaMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMetric" );
    Tcl_DStringAppendElement ( &dString, "SetMetric" );
    Tcl_DStringAppendElement ( &dString, "GetMetricMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMetricMaxValue" );
    Tcl_DStringAppendElement ( &dString, "SetMetricToPABIC" );
    Tcl_DStringAppendElement ( &dString, "SetMetricToEntropy" );
    Tcl_DStringAppendElement ( &dString, "SetMetricToMinimumVariance" );
    Tcl_DStringAppendElement ( &dString, "Valley" );
    Tcl_DStringAppendElement ( &dString, "Valley2" );
    Tcl_DStringAppendElement ( &dString, "SetOutputStatus" );
    Tcl_DStringAppendElement ( &dString, "GetOutputStatus" );
    Tcl_DStringAppendElement ( &dString, "GenerateSynthetic" );
    Tcl_DStringAppendElement ( &dString, "SetHisto_OutlierDetection" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_OutlierDetectionMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_OutlierDetectionMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_OutlierDetection" );
    Tcl_DStringAppendElement ( &dString, "Histo_OutlierDetectionOn" );
    Tcl_DStringAppendElement ( &dString, "Histo_OutlierDetectionOff" );
    Tcl_DStringAppendElement ( &dString, "SetHisto_OutlierProbability" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_OutlierProbability" );
    Tcl_DStringAppendElement ( &dString, "SetHisto_MaxSigmaRatio" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_MaxSigmaRatioMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_MaxSigmaRatioMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_MaxSigmaRatio" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfClasses" );
    Tcl_DStringAppendElement ( &dString, "SetHisto_NumberOfClasses" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfClassesMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfClassesMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfBins" );
    Tcl_DStringAppendElement ( &dString, "SetHisto_NumberOfBins" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfBinsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfBinsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_Convergence" );
    Tcl_DStringAppendElement ( &dString, "SetHisto_Convergence" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_ConvergenceMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_ConvergenceMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_Sigma" );
    Tcl_DStringAppendElement ( &dString, "SetHisto_Sigma" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_SigmaMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_SigmaMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_Iterations" );
    Tcl_DStringAppendElement ( &dString, "SetHisto_Iterations" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_IterationsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_IterationsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_PreprocessingMode" );
    Tcl_DStringAppendElement ( &dString, "SetHisto_PreprocessingMode" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_PreprocessingModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetHisto_PreprocessingModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "ComputeScaleFactor" );
    Tcl_DStringAppendElement ( &dString, "ComputeCorrectedImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeMeanPreservingCorrectedImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeCorrectedImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeCorrectedImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeMaskCorrectedImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeCorrectedLogarithmImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeMeanPreservingCorrectedLogarithmImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeCorrectedLogarithmImage" );
    Tcl_DStringAppendElement ( &dString, "ExponentiateBiasField" );
    Tcl_DStringAppendElement ( &dString, "CreateLogReferenceImage" );
    Tcl_DStringAppendElement ( &dString, "ComputeRatioBiasFieldImage" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxAbstractBiasFieldCorrection *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for PrintInformation */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ProvideFeedbackOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFixedNumberOfSlices */
    if ( strcmp ( argv[2], "SetFixedNumberOfSlices" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFixedNumberOfSlices" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFixedNumberOfSlices */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFixedNumberOfSlices(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetFixedNumberOfSlices */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFixedNumberOfSlices */
    if ( strcmp ( argv[2], "GetFixedNumberOfSlices" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFixedNumberOfSlices" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFixedNumberOfSlices */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "int GetFixedNumberOfSlices();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetFixedNumberOfSlices */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FixedNumberOfSlicesOn */
    if ( strcmp ( argv[2], "FixedNumberOfSlicesOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FixedNumberOfSlicesOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FixedNumberOfSlicesOn */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void FixedNumberOfSlicesOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for FixedNumberOfSlicesOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FixedNumberOfSlicesOff */
    if ( strcmp ( argv[2], "FixedNumberOfSlicesOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FixedNumberOfSlicesOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FixedNumberOfSlicesOff */
    Tcl_DStringAppendElement ( &dString, " Feedback Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void FixedNumberOfSlicesOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for FixedNumberOfSlicesOff */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetNumberOfLevels */

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
    Tcl_DStringAppendElement ( &dString, " NumberOfLevels, NumberOfSteps, Resolution\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfIterations(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " NumberOfLevels, NumberOfSteps, Resolution\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfIterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " NumberOfLevels, NumberOfSteps, Resolution\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfSteps(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " NumberOfLevels, NumberOfSteps, Resolution\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfSteps();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " NumberOfLevels, NumberOfSteps, Resolution\n" );
    Tcl_DStringAppendElement ( &dString, "void SetResolution(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " NumberOfLevels, NumberOfSteps, Resolution\n" );
    Tcl_DStringAppendElement ( &dString, "double GetResolution();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetResolution */

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
    Tcl_DStringAppendElement ( &dString, " NumberOfLevels, NumberOfSteps, Resolution\n" );
    Tcl_DStringAppendElement ( &dString, "void SetStepSize(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " NumberOfLevels, NumberOfSteps, Resolution\n" );
    Tcl_DStringAppendElement ( &dString, "double GetStepSize();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetStepSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFrame */
    if ( strcmp ( argv[2], "SetFrame" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFrame" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFrame */
    Tcl_DStringAppendElement ( &dString, " Frame\n Selects the frame to use for correction in 4D images (3D + t)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFrame(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetFrame */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFrame */
    if ( strcmp ( argv[2], "GetFrame" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFrame" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFrame */
    Tcl_DStringAppendElement ( &dString, " Frame\n Selects the frame to use for correction in 4D images (3D + t)\n" );
    Tcl_DStringAppendElement ( &dString, "int GetFrame();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetFrame */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for OnBeginIteration */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeBiasFit */
    if ( strcmp ( argv[2], "ComputeBiasFit" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeBiasFit" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeBiasFit */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int ComputeBiasFit(int level);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeBiasFit */

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
    Tcl_DStringAppendElement ( &dString, " Get The Weight Image\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetWeightImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetWeightImage */

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
    Tcl_DStringAppendElement ( &dString, " Get The Weight Image\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetBiasField();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetBiasField */

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
    Tcl_DStringAppendElement ( &dString, " Get/Set the Mask Image\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaskImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetMaskImage */

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
    Tcl_DStringAppendElement ( &dString, " Get/Set the Mask Image\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetMaskImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetMaskImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetClassParameters */
    if ( strcmp ( argv[2], "GetClassParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetClassParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetClassParameters */
    Tcl_DStringAppendElement ( &dString, " Get Internal Transformation Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray *GetClassParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetClassParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetClassParameters */
    if ( strcmp ( argv[2], "SetClassParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetClassParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetClassParameters */
    Tcl_DStringAppendElement ( &dString, " Get Internal Transformation Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "void SetClassParameters(vtkFloatArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetClassParameters */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetEpsilonMaxValue */

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
    Tcl_DStringAppendElement ( &dString, " Lambda is the smoothness weight\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLambda();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " Lambda is the smoothness weight\n" );
    Tcl_DStringAppendElement ( &dString, "void SetLambda(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetLambda */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLambdaMinValue */
    if ( strcmp ( argv[2], "GetLambdaMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLambdaMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLambdaMinValue */
    Tcl_DStringAppendElement ( &dString, " Lambda is the smoothness weight\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLambdaMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetLambdaMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLambdaMaxValue */
    if ( strcmp ( argv[2], "GetLambdaMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLambdaMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLambdaMaxValue */
    Tcl_DStringAppendElement ( &dString, " Lambda is the smoothness weight\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLambdaMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetLambdaMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMetric */
    if ( strcmp ( argv[2], "GetMetric" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMetric" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMetric */
    Tcl_DStringAppendElement ( &dString, " Metric\n 0 -- pabic multi-valley stuff\n 1 -- entropy\n 2 -- input itself is bias field\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMetric();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetMetric */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMetric */
    if ( strcmp ( argv[2], "SetMetric" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMetric" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMetric */
    Tcl_DStringAppendElement ( &dString, " Metric\n 0 -- pabic multi-valley stuff\n 1 -- entropy\n 2 -- input itself is bias field\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMetric(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetMetric */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMetricMinValue */
    if ( strcmp ( argv[2], "GetMetricMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMetricMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMetricMinValue */
    Tcl_DStringAppendElement ( &dString, " Metric\n 0 -- pabic multi-valley stuff\n 1 -- entropy\n 2 -- input itself is bias field\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMetricMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetMetricMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMetricMaxValue */
    if ( strcmp ( argv[2], "GetMetricMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMetricMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMetricMaxValue */
    Tcl_DStringAppendElement ( &dString, " Metric\n 0 -- pabic multi-valley stuff\n 1 -- entropy\n 2 -- input itself is bias field\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMetricMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetMetricMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMetricToPABIC */
    if ( strcmp ( argv[2], "SetMetricToPABIC" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMetricToPABIC" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMetricToPABIC */
    Tcl_DStringAppendElement ( &dString, " Metric\n 0 -- pabic multi-valley stuff\n 1 -- entropy\n 2 -- input itself is bias field\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMetricToPABIC();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetMetricToPABIC */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMetricToEntropy */
    if ( strcmp ( argv[2], "SetMetricToEntropy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMetricToEntropy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMetricToEntropy */
    Tcl_DStringAppendElement ( &dString, " Metric\n 0 -- pabic multi-valley stuff\n 1 -- entropy\n 2 -- input itself is bias field\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMetricToEntropy();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetMetricToEntropy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMetricToMinimumVariance */
    if ( strcmp ( argv[2], "SetMetricToMinimumVariance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMetricToMinimumVariance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMetricToMinimumVariance */
    Tcl_DStringAppendElement ( &dString, " Metric\n 0 -- pabic multi-valley stuff\n 1 -- entropy\n 2 -- input itself is bias field\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMetricToMinimumVariance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetMetricToMinimumVariance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Valley */
    if ( strcmp ( argv[2], "Valley" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Valley" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Valley */
    Tcl_DStringAppendElement ( &dString, " Valley Function\n" );
    Tcl_DStringAppendElement ( &dString, "static double Valley(double x, double sigma);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for Valley */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Valley2 */
    if ( strcmp ( argv[2], "Valley2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Valley2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Valley2 */
    Tcl_DStringAppendElement ( &dString, " Valley Function\n" );
    Tcl_DStringAppendElement ( &dString, "static double Valley2(double x, double sigma2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for Valley2 */

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
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutputStatus */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOutputStatus(const char *line, double v);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "const char *GetOutputStatus();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetOutputStatus */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateSynthetic */
    if ( strcmp ( argv[2], "GenerateSynthetic" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateSynthetic" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateSynthetic */
    Tcl_DStringAppendElement ( &dString, " Synthetic Data\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GenerateSynthetic(int width, double factor);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GenerateSynthetic */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHisto_OutlierDetection */
    if ( strcmp ( argv[2], "SetHisto_OutlierDetection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHisto_OutlierDetection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHisto_OutlierDetection */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHisto_OutlierDetection(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetHisto_OutlierDetection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_OutlierDetectionMinValue */
    if ( strcmp ( argv[2], "GetHisto_OutlierDetectionMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_OutlierDetectionMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_OutlierDetectionMinValue */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_OutlierDetectionMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_OutlierDetectionMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_OutlierDetectionMaxValue */
    if ( strcmp ( argv[2], "GetHisto_OutlierDetectionMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_OutlierDetectionMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_OutlierDetectionMaxValue */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_OutlierDetectionMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_OutlierDetectionMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_OutlierDetection */
    if ( strcmp ( argv[2], "GetHisto_OutlierDetection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_OutlierDetection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_OutlierDetection */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_OutlierDetection();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_OutlierDetection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Histo_OutlierDetectionOn */
    if ( strcmp ( argv[2], "Histo_OutlierDetectionOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Histo_OutlierDetectionOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Histo_OutlierDetectionOn */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "void Histo_OutlierDetectionOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for Histo_OutlierDetectionOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Histo_OutlierDetectionOff */
    if ( strcmp ( argv[2], "Histo_OutlierDetectionOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Histo_OutlierDetectionOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Histo_OutlierDetectionOff */
    Tcl_DStringAppendElement ( &dString, " Outlier Class\n" );
    Tcl_DStringAppendElement ( &dString, "void Histo_OutlierDetectionOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for Histo_OutlierDetectionOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHisto_OutlierProbability */
    if ( strcmp ( argv[2], "SetHisto_OutlierProbability" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHisto_OutlierProbability" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHisto_OutlierProbability */
    Tcl_DStringAppendElement ( &dString, " Outlier Probability\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHisto_OutlierProbability(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetHisto_OutlierProbability */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_OutlierProbability */
    if ( strcmp ( argv[2], "GetHisto_OutlierProbability" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_OutlierProbability" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_OutlierProbability */
    Tcl_DStringAppendElement ( &dString, " Outlier Probability\n" );
    Tcl_DStringAppendElement ( &dString, "float GetHisto_OutlierProbability();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_OutlierProbability */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHisto_MaxSigmaRatio */
    if ( strcmp ( argv[2], "SetHisto_MaxSigmaRatio" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHisto_MaxSigmaRatio" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHisto_MaxSigmaRatio */
    Tcl_DStringAppendElement ( &dString, " Outlier Probability\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHisto_MaxSigmaRatio(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetHisto_MaxSigmaRatio */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_MaxSigmaRatioMinValue */
    if ( strcmp ( argv[2], "GetHisto_MaxSigmaRatioMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_MaxSigmaRatioMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_MaxSigmaRatioMinValue */
    Tcl_DStringAppendElement ( &dString, " Outlier Probability\n" );
    Tcl_DStringAppendElement ( &dString, "float GetHisto_MaxSigmaRatioMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_MaxSigmaRatioMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_MaxSigmaRatioMaxValue */
    if ( strcmp ( argv[2], "GetHisto_MaxSigmaRatioMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_MaxSigmaRatioMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_MaxSigmaRatioMaxValue */
    Tcl_DStringAppendElement ( &dString, " Outlier Probability\n" );
    Tcl_DStringAppendElement ( &dString, "float GetHisto_MaxSigmaRatioMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_MaxSigmaRatioMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_MaxSigmaRatio */
    if ( strcmp ( argv[2], "GetHisto_MaxSigmaRatio" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_MaxSigmaRatio" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_MaxSigmaRatio */
    Tcl_DStringAppendElement ( &dString, " Outlier Probability\n" );
    Tcl_DStringAppendElement ( &dString, "float GetHisto_MaxSigmaRatio();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_MaxSigmaRatio */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_NumberOfClasses */
    if ( strcmp ( argv[2], "GetHisto_NumberOfClasses" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfClasses" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_NumberOfClasses */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_NumberOfClasses();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_NumberOfClasses */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHisto_NumberOfClasses */
    if ( strcmp ( argv[2], "SetHisto_NumberOfClasses" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHisto_NumberOfClasses" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHisto_NumberOfClasses */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHisto_NumberOfClasses(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetHisto_NumberOfClasses */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_NumberOfClassesMinValue */
    if ( strcmp ( argv[2], "GetHisto_NumberOfClassesMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfClassesMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_NumberOfClassesMinValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_NumberOfClassesMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_NumberOfClassesMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_NumberOfClassesMaxValue */
    if ( strcmp ( argv[2], "GetHisto_NumberOfClassesMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfClassesMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_NumberOfClassesMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Classes\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_NumberOfClassesMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_NumberOfClassesMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_NumberOfBins */
    if ( strcmp ( argv[2], "GetHisto_NumberOfBins" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfBins" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_NumberOfBins */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_NumberOfBins();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_NumberOfBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHisto_NumberOfBins */
    if ( strcmp ( argv[2], "SetHisto_NumberOfBins" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHisto_NumberOfBins" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHisto_NumberOfBins */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHisto_NumberOfBins(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetHisto_NumberOfBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_NumberOfBinsMinValue */
    if ( strcmp ( argv[2], "GetHisto_NumberOfBinsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfBinsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_NumberOfBinsMinValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_NumberOfBinsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_NumberOfBinsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_NumberOfBinsMaxValue */
    if ( strcmp ( argv[2], "GetHisto_NumberOfBinsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_NumberOfBinsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_NumberOfBinsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_NumberOfBinsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_NumberOfBinsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_Convergence */
    if ( strcmp ( argv[2], "GetHisto_Convergence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_Convergence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_Convergence */
    Tcl_DStringAppendElement ( &dString, " Convergence for CMeans Values (<1.0)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetHisto_Convergence();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_Convergence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHisto_Convergence */
    if ( strcmp ( argv[2], "SetHisto_Convergence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHisto_Convergence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHisto_Convergence */
    Tcl_DStringAppendElement ( &dString, " Convergence for CMeans Values (<1.0)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHisto_Convergence(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetHisto_Convergence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_ConvergenceMinValue */
    if ( strcmp ( argv[2], "GetHisto_ConvergenceMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_ConvergenceMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_ConvergenceMinValue */
    Tcl_DStringAppendElement ( &dString, " Convergence for CMeans Values (<1.0)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetHisto_ConvergenceMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_ConvergenceMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_ConvergenceMaxValue */
    if ( strcmp ( argv[2], "GetHisto_ConvergenceMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_ConvergenceMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_ConvergenceMaxValue */
    Tcl_DStringAppendElement ( &dString, " Convergence for CMeans Values (<1.0)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetHisto_ConvergenceMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_ConvergenceMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_Sigma */
    if ( strcmp ( argv[2], "GetHisto_Sigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_Sigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_Sigma */
    Tcl_DStringAppendElement ( &dString, " Histogram Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "float GetHisto_Sigma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_Sigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHisto_Sigma */
    if ( strcmp ( argv[2], "SetHisto_Sigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHisto_Sigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHisto_Sigma */
    Tcl_DStringAppendElement ( &dString, " Histogram Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHisto_Sigma(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetHisto_Sigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_SigmaMinValue */
    if ( strcmp ( argv[2], "GetHisto_SigmaMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_SigmaMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_SigmaMinValue */
    Tcl_DStringAppendElement ( &dString, " Histogram Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "float GetHisto_SigmaMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_SigmaMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_SigmaMaxValue */
    if ( strcmp ( argv[2], "GetHisto_SigmaMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_SigmaMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_SigmaMaxValue */
    Tcl_DStringAppendElement ( &dString, " Histogram Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "float GetHisto_SigmaMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_SigmaMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_Iterations */
    if ( strcmp ( argv[2], "GetHisto_Iterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_Iterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_Iterations */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_Iterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_Iterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHisto_Iterations */
    if ( strcmp ( argv[2], "SetHisto_Iterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHisto_Iterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHisto_Iterations */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHisto_Iterations(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetHisto_Iterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_IterationsMinValue */
    if ( strcmp ( argv[2], "GetHisto_IterationsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_IterationsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_IterationsMinValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_IterationsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_IterationsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_IterationsMaxValue */
    if ( strcmp ( argv[2], "GetHisto_IterationsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_IterationsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_IterationsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_IterationsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_IterationsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_PreprocessingMode */
    if ( strcmp ( argv[2], "GetHisto_PreprocessingMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_PreprocessingMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_PreprocessingMode */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_PreprocessingMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_PreprocessingMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHisto_PreprocessingMode */
    if ( strcmp ( argv[2], "SetHisto_PreprocessingMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHisto_PreprocessingMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHisto_PreprocessingMode */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetHisto_PreprocessingMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for SetHisto_PreprocessingMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_PreprocessingModeMinValue */
    if ( strcmp ( argv[2], "GetHisto_PreprocessingModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_PreprocessingModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_PreprocessingModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_PreprocessingModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_PreprocessingModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHisto_PreprocessingModeMaxValue */
    if ( strcmp ( argv[2], "GetHisto_PreprocessingModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHisto_PreprocessingModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHisto_PreprocessingModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number Of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetHisto_PreprocessingModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for GetHisto_PreprocessingModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeScaleFactor */
    if ( strcmp ( argv[2], "ComputeScaleFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeScaleFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeScaleFactor */
    Tcl_DStringAppendElement ( &dString, " Compute Corrected Image \n" );
    Tcl_DStringAppendElement ( &dString, "static double ComputeScaleFactor(vtkImageData *ref, vtkImageData *biasfield);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeScaleFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeCorrectedImage */
    if ( strcmp ( argv[2], "ComputeCorrectedImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeCorrectedImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeCorrectedImage */
    Tcl_DStringAppendElement ( &dString, " Compute Corrected Image \n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeCorrectedImage(vtkImageData *ref, vtkImageData *biasfield);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeCorrectedImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeMeanPreservingCorrectedImage */
    if ( strcmp ( argv[2], "ComputeMeanPreservingCorrectedImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeMeanPreservingCorrectedImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeMeanPreservingCorrectedImage */
    Tcl_DStringAppendElement ( &dString, " Compute Corrected Image \n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeMeanPreservingCorrectedImage(vtkImageData *ref, vtkImageData *biasfield);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeMeanPreservingCorrectedImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeCorrectedImage */
    if ( strcmp ( argv[2], "ComputeCorrectedImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeCorrectedImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeCorrectedImage */
    Tcl_DStringAppendElement ( &dString, " Compute Corrected Image \n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeCorrectedImage(vtkImageData *ref, vtkImageData *biasfield, int meanpreserving);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeCorrectedImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeCorrectedImage */
    if ( strcmp ( argv[2], "ComputeCorrectedImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeCorrectedImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeCorrectedImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int ComputeCorrectedImage(vtkImageData *ref, vtkImageData *biasfield, vtkImageData *fixedimage, int meanpreserving);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeCorrectedImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeMaskCorrectedImage */
    if ( strcmp ( argv[2], "ComputeMaskCorrectedImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeMaskCorrectedImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeMaskCorrectedImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeMaskCorrectedImage(vtkImageData *ref, vtkImageData *biasfield, vtkImageData *Mask, int meanpreserving);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeMaskCorrectedImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeCorrectedLogarithmImage */
    if ( strcmp ( argv[2], "ComputeCorrectedLogarithmImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeCorrectedLogarithmImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeCorrectedLogarithmImage */
    Tcl_DStringAppendElement ( &dString, " Compute Corrected Log Image \n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeCorrectedLogarithmImage(vtkImageData *logref, vtkImageData *logbiasfield);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeCorrectedLogarithmImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeMeanPreservingCorrectedLogarithmImage */
    if ( strcmp ( argv[2], "ComputeMeanPreservingCorrectedLogarithmImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeMeanPreservingCorrectedLogarithmImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeMeanPreservingCorrectedLogarithmImage */
    Tcl_DStringAppendElement ( &dString, " Compute Corrected Log Image \n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeMeanPreservingCorrectedLogarithmImage(vtkImageData *logref, vtkImageData *logbiasfield);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeMeanPreservingCorrectedLogarithmImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeCorrectedLogarithmImage */
    if ( strcmp ( argv[2], "ComputeCorrectedLogarithmImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeCorrectedLogarithmImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeCorrectedLogarithmImage */
    Tcl_DStringAppendElement ( &dString, " Compute Corrected Log Image \n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeCorrectedLogarithmImage(vtkImageData *logref, vtkImageData *logbiasfield, int meanpreserving);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeCorrectedLogarithmImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ExponentiateBiasField */
    if ( strcmp ( argv[2], "ExponentiateBiasField" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ExponentiateBiasField" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ExponentiateBiasField */
    Tcl_DStringAppendElement ( &dString, " Exponentiate Bias Field\n" );
    Tcl_DStringAppendElement ( &dString, "static int ExponentiateBiasField(vtkImageData *biasfield);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ExponentiateBiasField */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateLogReferenceImage */
    if ( strcmp ( argv[2], "CreateLogReferenceImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateLogReferenceImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateLogReferenceImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *CreateLogReferenceImage(vtkImageData *input);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for CreateLogReferenceImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeRatioBiasFieldImage */
    if ( strcmp ( argv[2], "ComputeRatioBiasFieldImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeRatioBiasFieldImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeRatioBiasFieldImage */
    Tcl_DStringAppendElement ( &dString, " Compute Bias Field From Two Images\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *ComputeRatioBiasFieldImage(vtkImageData *original, vtkImageData *corrected);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractBiasFieldCorrection" );
    /* Closing for ComputeRatioBiasFieldImage */

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
