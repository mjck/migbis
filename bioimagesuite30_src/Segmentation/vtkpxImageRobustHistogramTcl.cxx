// tcl wrapper for vtkpxImageRobustHistogram object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxImageRobustHistogram.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxImageRobustHistogramNewCommand()
{
  vtkpxImageRobustHistogram *temp = vtkpxImageRobustHistogram::New();
  return static_cast<ClientData>(temp);
}

int vtkpxSimpleImageToImageFilterCppCommand(vtkpxSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxImageRobustHistogramCppCommand(vtkpxImageRobustHistogram *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxImageRobustHistogramCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxImageRobustHistogramCppCommand(static_cast<vtkpxImageRobustHistogram *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxImageRobustHistogramCppCommand(vtkpxImageRobustHistogram *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxImageRobustHistogram",argv[1]))
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
    vtkpxImageRobustHistogram  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxImageRobustHistogram");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxImageRobustHistogram  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxImageRobustHistogram");
    return TCL_OK;
    }
    }
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxImageRobustHistogram  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxImageRobustHistogram");
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
  if ((!strcmp("GetNumberOfBins",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfBins();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetWeightImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetWeightImage(temp0);
    Tcl_ResetResult(interp);
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
  if ((!strcmp("SetIgnoreMaskedVoxels",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetIgnoreMaskedVoxels(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetIgnoreMaskedVoxelsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIgnoreMaskedVoxelsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetIgnoreMaskedVoxelsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIgnoreMaskedVoxelsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetIgnoreMaskedVoxels",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIgnoreMaskedVoxels();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("IgnoreMaskedVoxelsOn",argv[1]))&&(argc == 2))
    {
    op->IgnoreMaskedVoxelsOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("IgnoreMaskedVoxelsOff",argv[1]))&&(argc == 2))
    {
    op->IgnoreMaskedVoxelsOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetInterpolation",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetInterpolation(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInterpolationMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInterpolationMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInterpolationMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInterpolationMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInterpolation",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInterpolation();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("InterpolationOn",argv[1]))&&(argc == 2))
    {
    op->InterpolationOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("InterpolationOff",argv[1]))&&(argc == 2))
    {
    op->InterpolationOff();
    Tcl_ResetResult(interp);
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
  if ((!strcmp("SetRobustMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetRobustMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRobustModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRobustModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRobustModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRobustModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRobustMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRobustMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("RobustModeOn",argv[1]))&&(argc == 2))
    {
    op->RobustModeOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("RobustModeOff",argv[1]))&&(argc == 2))
    {
    op->RobustModeOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSaturationMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetSaturationMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSaturationModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSaturationModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSaturationModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSaturationModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSaturationMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSaturationMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SaturationModeOn",argv[1]))&&(argc == 2))
    {
    op->SaturationModeOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SaturationModeOff",argv[1]))&&(argc == 2))
    {
    op->SaturationModeOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetPlainHistogram",argv[1]))&&(argc == 2))
    {
    op->SetPlainHistogram();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetLowerBound",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLowerBound(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLowerBoundMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLowerBoundMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLowerBoundMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLowerBoundMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLowerBound",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLowerBound();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetUpperBound",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetUpperBound(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUpperBoundMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetUpperBoundMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUpperBoundMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetUpperBoundMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUpperBound",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetUpperBound();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLowerThreshold",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLowerThreshold();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUpperThreshold",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetUpperThreshold();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetLowerThreshold",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLowerThreshold(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetUpperThreshold",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetUpperThreshold(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetEntropy",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetEntropy();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("BackupHistogram",argv[1]))&&(argc == 2))
    {
    op->BackupHistogram();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("AddImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->AddImage(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("RemoveImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->RemoveImage(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("UndoLastChange",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->UndoLastChange();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ComputeEntropy",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->ComputeEntropy();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SaveGnuplot",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->SaveGnuplot(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetObjectmapStatistics",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkDoubleArray  *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->GetObjectmapStatistics(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxImageRobustHistogramCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxImageRobustHistogram:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfBins\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBinsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBinsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBins\n",NULL);
    Tcl_AppendResult(interp,"  SetWeightImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetWeightImage\n",NULL);
    Tcl_AppendResult(interp,"  SetIgnoreMaskedVoxels\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetIgnoreMaskedVoxelsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIgnoreMaskedVoxelsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetIgnoreMaskedVoxels\n",NULL);
    Tcl_AppendResult(interp,"  IgnoreMaskedVoxelsOn\n",NULL);
    Tcl_AppendResult(interp,"  IgnoreMaskedVoxelsOff\n",NULL);
    Tcl_AppendResult(interp,"  SetInterpolation\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolationMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolationMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolation\n",NULL);
    Tcl_AppendResult(interp,"  InterpolationOn\n",NULL);
    Tcl_AppendResult(interp,"  InterpolationOff\n",NULL);
    Tcl_AppendResult(interp,"  SetFrame\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFrame\n",NULL);
    Tcl_AppendResult(interp,"  SetRobustMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRobustModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRobustModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRobustMode\n",NULL);
    Tcl_AppendResult(interp,"  RobustModeOn\n",NULL);
    Tcl_AppendResult(interp,"  RobustModeOff\n",NULL);
    Tcl_AppendResult(interp,"  SetSaturationMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSaturationModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSaturationModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSaturationMode\n",NULL);
    Tcl_AppendResult(interp,"  SaturationModeOn\n",NULL);
    Tcl_AppendResult(interp,"  SaturationModeOff\n",NULL);
    Tcl_AppendResult(interp,"  SetPlainHistogram\n",NULL);
    Tcl_AppendResult(interp,"  SetLowerBound\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLowerBoundMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLowerBoundMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLowerBound\n",NULL);
    Tcl_AppendResult(interp,"  SetUpperBound\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUpperBoundMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUpperBoundMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUpperBound\n",NULL);
    Tcl_AppendResult(interp,"  GetLowerThreshold\n",NULL);
    Tcl_AppendResult(interp,"  GetUpperThreshold\n",NULL);
    Tcl_AppendResult(interp,"  SetLowerThreshold\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetUpperThreshold\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetEntropy\n",NULL);
    Tcl_AppendResult(interp,"  BackupHistogram\n",NULL);
    Tcl_AppendResult(interp,"  AddImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  RemoveImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  UndoLastChange\n",NULL);
    Tcl_AppendResult(interp,"  ComputeEntropy\n",NULL);
    Tcl_AppendResult(interp,"  SaveGnuplot\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetObjectmapStatistics\t with 2 args\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfBins" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBinsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBinsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBins" );
    Tcl_DStringAppendElement ( &dString, "SetWeightImage" );
    Tcl_DStringAppendElement ( &dString, "GetWeightImage" );
    Tcl_DStringAppendElement ( &dString, "SetIgnoreMaskedVoxels" );
    Tcl_DStringAppendElement ( &dString, "GetIgnoreMaskedVoxelsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetIgnoreMaskedVoxelsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetIgnoreMaskedVoxels" );
    Tcl_DStringAppendElement ( &dString, "IgnoreMaskedVoxelsOn" );
    Tcl_DStringAppendElement ( &dString, "IgnoreMaskedVoxelsOff" );
    Tcl_DStringAppendElement ( &dString, "SetInterpolation" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolationMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolationMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolation" );
    Tcl_DStringAppendElement ( &dString, "InterpolationOn" );
    Tcl_DStringAppendElement ( &dString, "InterpolationOff" );
    Tcl_DStringAppendElement ( &dString, "SetFrame" );
    Tcl_DStringAppendElement ( &dString, "GetFrame" );
    Tcl_DStringAppendElement ( &dString, "SetRobustMode" );
    Tcl_DStringAppendElement ( &dString, "GetRobustModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetRobustModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetRobustMode" );
    Tcl_DStringAppendElement ( &dString, "RobustModeOn" );
    Tcl_DStringAppendElement ( &dString, "RobustModeOff" );
    Tcl_DStringAppendElement ( &dString, "SetSaturationMode" );
    Tcl_DStringAppendElement ( &dString, "GetSaturationModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSaturationModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetSaturationMode" );
    Tcl_DStringAppendElement ( &dString, "SaturationModeOn" );
    Tcl_DStringAppendElement ( &dString, "SaturationModeOff" );
    Tcl_DStringAppendElement ( &dString, "SetPlainHistogram" );
    Tcl_DStringAppendElement ( &dString, "SetLowerBound" );
    Tcl_DStringAppendElement ( &dString, "GetLowerBoundMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetLowerBoundMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetLowerBound" );
    Tcl_DStringAppendElement ( &dString, "SetUpperBound" );
    Tcl_DStringAppendElement ( &dString, "GetUpperBoundMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetUpperBoundMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUpperBound" );
    Tcl_DStringAppendElement ( &dString, "GetLowerThreshold" );
    Tcl_DStringAppendElement ( &dString, "GetUpperThreshold" );
    Tcl_DStringAppendElement ( &dString, "SetLowerThreshold" );
    Tcl_DStringAppendElement ( &dString, "SetUpperThreshold" );
    Tcl_DStringAppendElement ( &dString, "GetEntropy" );
    Tcl_DStringAppendElement ( &dString, "BackupHistogram" );
    Tcl_DStringAppendElement ( &dString, "AddImage" );
    Tcl_DStringAppendElement ( &dString, "RemoveImage" );
    Tcl_DStringAppendElement ( &dString, "UndoLastChange" );
    Tcl_DStringAppendElement ( &dString, "ComputeEntropy" );
    Tcl_DStringAppendElement ( &dString, "SaveGnuplot" );
    Tcl_DStringAppendElement ( &dString, "GetObjectmapStatistics" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
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
    Tcl_DStringAppendElement ( &dString, " Construct object to extract all of the input data.\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkpxImageRobustHistogram *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for New */

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
    Tcl_DStringAppendElement ( &dString, " Polar Mode used for ultrasound images\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfBins(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
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
    Tcl_DStringAppendElement ( &dString, " Polar Mode used for ultrasound images\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfBinsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
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
    Tcl_DStringAppendElement ( &dString, " Polar Mode used for ultrasound images\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfBinsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetNumberOfBinsMaxValue */

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
    Tcl_DStringAppendElement ( &dString, " Polar Mode used for ultrasound images\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfBins();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetNumberOfBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetWeightImage */
    if ( strcmp ( argv[2], "SetWeightImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetWeightImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetWeightImage */
    Tcl_DStringAppendElement ( &dString, " Get/Set the Mask Image\n" );
    Tcl_DStringAppendElement ( &dString, "void SetWeightImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SetWeightImage */

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
    Tcl_DStringAppendElement ( &dString, " Get/Set the Mask Image\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetWeightImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetWeightImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetIgnoreMaskedVoxels */
    if ( strcmp ( argv[2], "SetIgnoreMaskedVoxels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetIgnoreMaskedVoxels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetIgnoreMaskedVoxels */
    Tcl_DStringAppendElement ( &dString, " Flag Igored Mask Voxels\n" );
    Tcl_DStringAppendElement ( &dString, "void SetIgnoreMaskedVoxels(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SetIgnoreMaskedVoxels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIgnoreMaskedVoxelsMinValue */
    if ( strcmp ( argv[2], "GetIgnoreMaskedVoxelsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIgnoreMaskedVoxelsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIgnoreMaskedVoxelsMinValue */
    Tcl_DStringAppendElement ( &dString, " Flag Igored Mask Voxels\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIgnoreMaskedVoxelsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetIgnoreMaskedVoxelsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIgnoreMaskedVoxelsMaxValue */
    if ( strcmp ( argv[2], "GetIgnoreMaskedVoxelsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIgnoreMaskedVoxelsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIgnoreMaskedVoxelsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Flag Igored Mask Voxels\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIgnoreMaskedVoxelsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetIgnoreMaskedVoxelsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIgnoreMaskedVoxels */
    if ( strcmp ( argv[2], "GetIgnoreMaskedVoxels" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIgnoreMaskedVoxels" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIgnoreMaskedVoxels */
    Tcl_DStringAppendElement ( &dString, " Flag Igored Mask Voxels\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIgnoreMaskedVoxels();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetIgnoreMaskedVoxels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IgnoreMaskedVoxelsOn */
    if ( strcmp ( argv[2], "IgnoreMaskedVoxelsOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IgnoreMaskedVoxelsOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IgnoreMaskedVoxelsOn */
    Tcl_DStringAppendElement ( &dString, " Flag Igored Mask Voxels\n" );
    Tcl_DStringAppendElement ( &dString, "void IgnoreMaskedVoxelsOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for IgnoreMaskedVoxelsOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IgnoreMaskedVoxelsOff */
    if ( strcmp ( argv[2], "IgnoreMaskedVoxelsOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IgnoreMaskedVoxelsOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IgnoreMaskedVoxelsOff */
    Tcl_DStringAppendElement ( &dString, " Flag Igored Mask Voxels\n" );
    Tcl_DStringAppendElement ( &dString, "void IgnoreMaskedVoxelsOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for IgnoreMaskedVoxelsOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInterpolation */
    if ( strcmp ( argv[2], "SetInterpolation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInterpolation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInterpolation */
    Tcl_DStringAppendElement ( &dString, " Use this to set sliceno,numslices and currentplane\n for extracting frames\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInterpolation(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SetInterpolation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolationMinValue */
    if ( strcmp ( argv[2], "GetInterpolationMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolationMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolationMinValue */
    Tcl_DStringAppendElement ( &dString, " Use this to set sliceno,numslices and currentplane\n for extracting frames\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInterpolationMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetInterpolationMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolationMaxValue */
    if ( strcmp ( argv[2], "GetInterpolationMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolationMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolationMaxValue */
    Tcl_DStringAppendElement ( &dString, " Use this to set sliceno,numslices and currentplane\n for extracting frames\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInterpolationMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetInterpolationMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolation */
    if ( strcmp ( argv[2], "GetInterpolation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolation */
    Tcl_DStringAppendElement ( &dString, " Use this to set sliceno,numslices and currentplane\n for extracting frames\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInterpolation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetInterpolation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InterpolationOn */
    if ( strcmp ( argv[2], "InterpolationOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InterpolationOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InterpolationOn */
    Tcl_DStringAppendElement ( &dString, " Use this to set sliceno,numslices and currentplane\n for extracting frames\n" );
    Tcl_DStringAppendElement ( &dString, "void InterpolationOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for InterpolationOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InterpolationOff */
    if ( strcmp ( argv[2], "InterpolationOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InterpolationOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InterpolationOff */
    Tcl_DStringAppendElement ( &dString, " Use this to set sliceno,numslices and currentplane\n for extracting frames\n" );
    Tcl_DStringAppendElement ( &dString, "void InterpolationOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for InterpolationOff */

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
    Tcl_DStringAppendElement ( &dString, " Use this to set sliceno,numslices and currentplane\n for extracting frames\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFrame(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
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
    Tcl_DStringAppendElement ( &dString, " Use this to set sliceno,numslices and currentplane\n for extracting frames\n" );
    Tcl_DStringAppendElement ( &dString, "int GetFrame();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetFrame */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRobustMode */
    if ( strcmp ( argv[2], "SetRobustMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRobustMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRobustMode */
    Tcl_DStringAppendElement ( &dString, " Robust Mode Automatically Determines Bounds from Lower and UpperBound\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRobustMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SetRobustMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRobustModeMinValue */
    if ( strcmp ( argv[2], "GetRobustModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRobustModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRobustModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Robust Mode Automatically Determines Bounds from Lower and UpperBound\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRobustModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetRobustModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRobustModeMaxValue */
    if ( strcmp ( argv[2], "GetRobustModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRobustModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRobustModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Robust Mode Automatically Determines Bounds from Lower and UpperBound\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRobustModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetRobustModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRobustMode */
    if ( strcmp ( argv[2], "GetRobustMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRobustMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRobustMode */
    Tcl_DStringAppendElement ( &dString, " Robust Mode Automatically Determines Bounds from Lower and UpperBound\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRobustMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetRobustMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RobustModeOn */
    if ( strcmp ( argv[2], "RobustModeOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RobustModeOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RobustModeOn */
    Tcl_DStringAppendElement ( &dString, " Robust Mode Automatically Determines Bounds from Lower and UpperBound\n" );
    Tcl_DStringAppendElement ( &dString, "void RobustModeOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for RobustModeOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RobustModeOff */
    if ( strcmp ( argv[2], "RobustModeOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RobustModeOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RobustModeOff */
    Tcl_DStringAppendElement ( &dString, " Robust Mode Automatically Determines Bounds from Lower and UpperBound\n" );
    Tcl_DStringAppendElement ( &dString, "void RobustModeOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for RobustModeOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSaturationMode */
    if ( strcmp ( argv[2], "SetSaturationMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSaturationMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSaturationMode */
    Tcl_DStringAppendElement ( &dString, " Saturation Mode (if v<Saturate Ignore if saturation is off)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSaturationMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SetSaturationMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSaturationModeMinValue */
    if ( strcmp ( argv[2], "GetSaturationModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSaturationModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSaturationModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Saturation Mode (if v<Saturate Ignore if saturation is off)\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSaturationModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetSaturationModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSaturationModeMaxValue */
    if ( strcmp ( argv[2], "GetSaturationModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSaturationModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSaturationModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Saturation Mode (if v<Saturate Ignore if saturation is off)\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSaturationModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetSaturationModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSaturationMode */
    if ( strcmp ( argv[2], "GetSaturationMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSaturationMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSaturationMode */
    Tcl_DStringAppendElement ( &dString, " Saturation Mode (if v<Saturate Ignore if saturation is off)\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSaturationMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetSaturationMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaturationModeOn */
    if ( strcmp ( argv[2], "SaturationModeOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaturationModeOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaturationModeOn */
    Tcl_DStringAppendElement ( &dString, " Saturation Mode (if v<Saturate Ignore if saturation is off)\n" );
    Tcl_DStringAppendElement ( &dString, "void SaturationModeOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SaturationModeOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaturationModeOff */
    if ( strcmp ( argv[2], "SaturationModeOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaturationModeOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaturationModeOff */
    Tcl_DStringAppendElement ( &dString, " Saturation Mode (if v<Saturate Ignore if saturation is off)\n" );
    Tcl_DStringAppendElement ( &dString, "void SaturationModeOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SaturationModeOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPlainHistogram */
    if ( strcmp ( argv[2], "SetPlainHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPlainHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPlainHistogram */
    Tcl_DStringAppendElement ( &dString, " Plain Histogram i.e. no tricks\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetPlainHistogram();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SetPlainHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLowerBound */
    if ( strcmp ( argv[2], "SetLowerBound" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLowerBound" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLowerBound */
    Tcl_DStringAppendElement ( &dString, " Upper and Lower Bounds of Cumulative Histogram \n" );
    Tcl_DStringAppendElement ( &dString, "void SetLowerBound(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SetLowerBound */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLowerBoundMinValue */
    if ( strcmp ( argv[2], "GetLowerBoundMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLowerBoundMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLowerBoundMinValue */
    Tcl_DStringAppendElement ( &dString, " Upper and Lower Bounds of Cumulative Histogram \n" );
    Tcl_DStringAppendElement ( &dString, "double GetLowerBoundMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetLowerBoundMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLowerBoundMaxValue */
    if ( strcmp ( argv[2], "GetLowerBoundMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLowerBoundMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLowerBoundMaxValue */
    Tcl_DStringAppendElement ( &dString, " Upper and Lower Bounds of Cumulative Histogram \n" );
    Tcl_DStringAppendElement ( &dString, "double GetLowerBoundMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetLowerBoundMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLowerBound */
    if ( strcmp ( argv[2], "GetLowerBound" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLowerBound" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLowerBound */
    Tcl_DStringAppendElement ( &dString, " Upper and Lower Bounds of Cumulative Histogram \n" );
    Tcl_DStringAppendElement ( &dString, "double GetLowerBound();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetLowerBound */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUpperBound */
    if ( strcmp ( argv[2], "SetUpperBound" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUpperBound" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUpperBound */
    Tcl_DStringAppendElement ( &dString, " Upper and Lower Bounds of Cumulative Histogram \n" );
    Tcl_DStringAppendElement ( &dString, "void SetUpperBound(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SetUpperBound */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUpperBoundMinValue */
    if ( strcmp ( argv[2], "GetUpperBoundMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUpperBoundMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUpperBoundMinValue */
    Tcl_DStringAppendElement ( &dString, " Upper and Lower Bounds of Cumulative Histogram \n" );
    Tcl_DStringAppendElement ( &dString, "double GetUpperBoundMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetUpperBoundMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUpperBoundMaxValue */
    if ( strcmp ( argv[2], "GetUpperBoundMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUpperBoundMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUpperBoundMaxValue */
    Tcl_DStringAppendElement ( &dString, " Upper and Lower Bounds of Cumulative Histogram \n" );
    Tcl_DStringAppendElement ( &dString, "double GetUpperBoundMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetUpperBoundMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUpperBound */
    if ( strcmp ( argv[2], "GetUpperBound" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUpperBound" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUpperBound */
    Tcl_DStringAppendElement ( &dString, " Upper and Lower Bounds of Cumulative Histogram \n" );
    Tcl_DStringAppendElement ( &dString, "double GetUpperBound();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetUpperBound */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLowerThreshold */
    if ( strcmp ( argv[2], "GetLowerThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLowerThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLowerThreshold */
    Tcl_DStringAppendElement ( &dString, " Key Outputs\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLowerThreshold();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetLowerThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUpperThreshold */
    if ( strcmp ( argv[2], "GetUpperThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUpperThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUpperThreshold */
    Tcl_DStringAppendElement ( &dString, " Key Outputs\n" );
    Tcl_DStringAppendElement ( &dString, "double GetUpperThreshold();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetUpperThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLowerThreshold */
    if ( strcmp ( argv[2], "SetLowerThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLowerThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLowerThreshold */
    Tcl_DStringAppendElement ( &dString, " Key Outputs\n" );
    Tcl_DStringAppendElement ( &dString, "void SetLowerThreshold(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SetLowerThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUpperThreshold */
    if ( strcmp ( argv[2], "SetUpperThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUpperThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUpperThreshold */
    Tcl_DStringAppendElement ( &dString, " Key Outputs\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUpperThreshold(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SetUpperThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetEntropy */
    if ( strcmp ( argv[2], "GetEntropy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetEntropy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetEntropy */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetEntropy();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetEntropy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: BackupHistogram */
    if ( strcmp ( argv[2], "BackupHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "BackupHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for BackupHistogram */
    Tcl_DStringAppendElement ( &dString, " Modify Current Histogram\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void BackupHistogram();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for BackupHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AddImage */
    if ( strcmp ( argv[2], "AddImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AddImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddImage */
    Tcl_DStringAppendElement ( &dString, " Modify Current Histogram\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int AddImage(vtkImageData *newimage, vtkImageData *weight=NULL);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for AddImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RemoveImage */
    if ( strcmp ( argv[2], "RemoveImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RemoveImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RemoveImage */
    Tcl_DStringAppendElement ( &dString, " Modify Current Histogram\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int RemoveImage(vtkImageData *newimage, vtkImageData *weight=NULL);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for RemoveImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UndoLastChange */
    if ( strcmp ( argv[2], "UndoLastChange" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UndoLastChange" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UndoLastChange */
    Tcl_DStringAppendElement ( &dString, " Modify Current Histogram\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int UndoLastChange();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for UndoLastChange */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeEntropy */
    if ( strcmp ( argv[2], "ComputeEntropy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeEntropy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeEntropy */
    Tcl_DStringAppendElement ( &dString, " Modify Current Histogram\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ComputeEntropy();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for ComputeEntropy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaveGnuplot */
    if ( strcmp ( argv[2], "SaveGnuplot" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaveGnuplot" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaveGnuplot */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int SaveGnuplot(char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for SaveGnuplot */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetObjectmapStatistics */
    if ( strcmp ( argv[2], "GetObjectmapStatistics" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetObjectmapStatistics" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetObjectmapStatistics */
    Tcl_DStringAppendElement ( &dString, " Get Centroids\n" );
    Tcl_DStringAppendElement ( &dString, "static int GetObjectmapStatistics(vtkImageData *img, vtkDoubleArray *array);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxImageRobustHistogram" );
    /* Closing for GetObjectmapStatistics */

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
