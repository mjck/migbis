// tcl wrapper for vtkbisIpsilateralConnMap object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkbisIpsilateralConnMap.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkbisIpsilateralConnMapNewCommand()
{
  vtkbisIpsilateralConnMap *temp = vtkbisIpsilateralConnMap::New();
  return static_cast<ClientData>(temp);
}

int vtkSimpleImageToImageFilterCppCommand(vtkSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkbisIpsilateralConnMapCppCommand(vtkbisIpsilateralConnMap *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkbisIpsilateralConnMapCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkbisIpsilateralConnMapCppCommand(static_cast<vtkbisIpsilateralConnMap *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkbisIpsilateralConnMapCppCommand(vtkbisIpsilateralConnMap *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkbisIpsilateralConnMap",argv[1]))
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
    vtkbisIpsilateralConnMap  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisIpsilateralConnMap");
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
    vtkbisIpsilateralConnMap  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisIpsilateralConnMap");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkbisIpsilateralConnMap  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisIpsilateralConnMap");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetThreshold",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetThreshold(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetThresholdMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetThresholdMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetThresholdMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetThresholdMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetThreshold",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetThreshold();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetImageMask",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetImageMask(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetImageMask",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetImageMask();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetRange",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetRange(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRangeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRangeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRangeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRangeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetRange",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRange();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetAbs",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetAbs(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAbsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAbsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAbsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAbsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAbs",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAbs();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOutData2",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetOutData2(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutData2",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetOutData2();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetType",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetType(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTypeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTypeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTypeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTypeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetType",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetType();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetDoContralateral",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetDoContralateral(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDoContralateralMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDoContralateralMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDoContralateralMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDoContralateralMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDoContralateral",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDoContralateral();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetFileName",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetFileName(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFileName",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetFileName();
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
  if ((!strcmp("SetLMean",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLMean(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLMean",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLMean();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetLStd",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLStd(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLStd",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLStd();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRMean",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetRMean(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRMean",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetRMean();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRStd",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetRStd(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRStd",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetRStd();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetLDegreeMean",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLDegreeMean(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLDegreeMean",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLDegreeMean();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetLDegreeStd",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLDegreeStd(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLDegreeStd",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLDegreeStd();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRDegreeMean",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetRDegreeMean(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRDegreeMean",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetRDegreeMean();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRDegreeStd",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetRDegreeStd(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRDegreeStd",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetRDegreeStd();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetLSize",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetLSize(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLSize",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetLSize();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetRSize",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetRSize(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRSize",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRSize();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkbisIpsilateralConnMapCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkbisIpsilateralConnMap:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetThreshold\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetThresholdMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetThresholdMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetThreshold\n",NULL);
    Tcl_AppendResult(interp,"  SetImageMask\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetImageMask\n",NULL);
    Tcl_AppendResult(interp,"  SetRange\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRangeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRangeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRange\n",NULL);
    Tcl_AppendResult(interp,"  SetAbs\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAbsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAbsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAbs\n",NULL);
    Tcl_AppendResult(interp,"  SetOutData2\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutData2\n",NULL);
    Tcl_AppendResult(interp,"  SetType\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTypeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTypeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetType\n",NULL);
    Tcl_AppendResult(interp,"  SetDoContralateral\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDoContralateralMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDoContralateralMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDoContralateral\n",NULL);
    Tcl_AppendResult(interp,"  SetFileName\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFileName\n",NULL);
    Tcl_AppendResult(interp,"  SetLMean\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLMean\n",NULL);
    Tcl_AppendResult(interp,"  SetLStd\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLStd\n",NULL);
    Tcl_AppendResult(interp,"  SetRMean\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRMean\n",NULL);
    Tcl_AppendResult(interp,"  SetRStd\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRStd\n",NULL);
    Tcl_AppendResult(interp,"  SetLDegreeMean\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLDegreeMean\n",NULL);
    Tcl_AppendResult(interp,"  SetLDegreeStd\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLDegreeStd\n",NULL);
    Tcl_AppendResult(interp,"  SetRDegreeMean\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRDegreeMean\n",NULL);
    Tcl_AppendResult(interp,"  SetRDegreeStd\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRDegreeStd\n",NULL);
    Tcl_AppendResult(interp,"  SetLSize\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLSize\n",NULL);
    Tcl_AppendResult(interp,"  SetRSize\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRSize\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetThreshold" );
    Tcl_DStringAppendElement ( &dString, "GetThresholdMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetThresholdMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetThreshold" );
    Tcl_DStringAppendElement ( &dString, "SetImageMask" );
    Tcl_DStringAppendElement ( &dString, "GetImageMask" );
    Tcl_DStringAppendElement ( &dString, "SetRange" );
    Tcl_DStringAppendElement ( &dString, "GetRangeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetRangeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetRange" );
    Tcl_DStringAppendElement ( &dString, "SetAbs" );
    Tcl_DStringAppendElement ( &dString, "GetAbsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetAbsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetAbs" );
    Tcl_DStringAppendElement ( &dString, "SetOutData2" );
    Tcl_DStringAppendElement ( &dString, "GetOutData2" );
    Tcl_DStringAppendElement ( &dString, "SetType" );
    Tcl_DStringAppendElement ( &dString, "GetTypeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetTypeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetType" );
    Tcl_DStringAppendElement ( &dString, "SetDoContralateral" );
    Tcl_DStringAppendElement ( &dString, "GetDoContralateralMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetDoContralateralMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetDoContralateral" );
    Tcl_DStringAppendElement ( &dString, "SetFileName" );
    Tcl_DStringAppendElement ( &dString, "GetFileName" );
    Tcl_DStringAppendElement ( &dString, "SetLMean" );
    Tcl_DStringAppendElement ( &dString, "GetLMean" );
    Tcl_DStringAppendElement ( &dString, "SetLStd" );
    Tcl_DStringAppendElement ( &dString, "GetLStd" );
    Tcl_DStringAppendElement ( &dString, "SetRMean" );
    Tcl_DStringAppendElement ( &dString, "GetRMean" );
    Tcl_DStringAppendElement ( &dString, "SetRStd" );
    Tcl_DStringAppendElement ( &dString, "GetRStd" );
    Tcl_DStringAppendElement ( &dString, "SetLDegreeMean" );
    Tcl_DStringAppendElement ( &dString, "GetLDegreeMean" );
    Tcl_DStringAppendElement ( &dString, "SetLDegreeStd" );
    Tcl_DStringAppendElement ( &dString, "GetLDegreeStd" );
    Tcl_DStringAppendElement ( &dString, "SetRDegreeMean" );
    Tcl_DStringAppendElement ( &dString, "GetRDegreeMean" );
    Tcl_DStringAppendElement ( &dString, "SetRDegreeStd" );
    Tcl_DStringAppendElement ( &dString, "GetRDegreeStd" );
    Tcl_DStringAppendElement ( &dString, "SetLSize" );
    Tcl_DStringAppendElement ( &dString, "GetLSize" );
    Tcl_DStringAppendElement ( &dString, "SetRSize" );
    Tcl_DStringAppendElement ( &dString, "GetRSize" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkbisIpsilateralConnMap *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetThreshold */
    if ( strcmp ( argv[2], "SetThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetThreshold */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetThreshold(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetThresholdMinValue */
    if ( strcmp ( argv[2], "GetThresholdMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetThresholdMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetThresholdMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetThresholdMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetThresholdMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetThresholdMaxValue */
    if ( strcmp ( argv[2], "GetThresholdMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetThresholdMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetThresholdMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetThresholdMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetThresholdMaxValue */

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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetThreshold();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetImageMask */
    if ( strcmp ( argv[2], "SetImageMask" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetImageMask" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetImageMask */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetImageMask(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetImageMask */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageMask */
    if ( strcmp ( argv[2], "GetImageMask" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageMask" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageMask */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetImageMask();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetImageMask */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRange */
    if ( strcmp ( argv[2], "SetRange" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRange" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRange */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetRange(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetRange */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRangeMinValue */
    if ( strcmp ( argv[2], "GetRangeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRangeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRangeMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetRangeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetRangeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRangeMaxValue */
    if ( strcmp ( argv[2], "GetRangeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRangeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRangeMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetRangeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetRangeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRange */
    if ( strcmp ( argv[2], "GetRange" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRange" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRange */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetRange();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetRange */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAbs */
    if ( strcmp ( argv[2], "SetAbs" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAbs" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAbs */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetAbs(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetAbs */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAbsMinValue */
    if ( strcmp ( argv[2], "GetAbsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAbsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAbsMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetAbsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetAbsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAbsMaxValue */
    if ( strcmp ( argv[2], "GetAbsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAbsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAbsMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetAbsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetAbsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAbs */
    if ( strcmp ( argv[2], "GetAbs" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAbs" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAbs */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetAbs();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetAbs */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutData2 */
    if ( strcmp ( argv[2], "SetOutData2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutData2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutData2 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOutData2(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetOutData2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutData2 */
    if ( strcmp ( argv[2], "GetOutData2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutData2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutData2 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetOutData2();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetOutData2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetType */
    if ( strcmp ( argv[2], "SetType" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetType" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetType */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetType(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetType */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTypeMinValue */
    if ( strcmp ( argv[2], "GetTypeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTypeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTypeMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetTypeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetTypeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTypeMaxValue */
    if ( strcmp ( argv[2], "GetTypeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTypeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTypeMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetTypeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetTypeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetType */
    if ( strcmp ( argv[2], "GetType" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetType" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetType */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetType();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetType */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDoContralateral */
    if ( strcmp ( argv[2], "SetDoContralateral" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDoContralateral" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDoContralateral */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetDoContralateral(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetDoContralateral */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDoContralateralMinValue */
    if ( strcmp ( argv[2], "GetDoContralateralMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDoContralateralMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDoContralateralMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetDoContralateralMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetDoContralateralMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDoContralateralMaxValue */
    if ( strcmp ( argv[2], "GetDoContralateralMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDoContralateralMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDoContralateralMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetDoContralateralMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetDoContralateralMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDoContralateral */
    if ( strcmp ( argv[2], "GetDoContralateral" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDoContralateral" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDoContralateral */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetDoContralateral();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetDoContralateral */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFileName */
    if ( strcmp ( argv[2], "SetFileName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFileName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFileName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetFileName(const char *filename);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetFileName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFileName */
    if ( strcmp ( argv[2], "GetFileName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFileName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFileName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual const char *GetFileName();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetFileName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLMean */
    if ( strcmp ( argv[2], "SetLMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetLMean(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetLMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLMean */
    if ( strcmp ( argv[2], "GetLMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetLMean();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetLMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLStd */
    if ( strcmp ( argv[2], "SetLStd" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLStd" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLStd */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetLStd(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetLStd */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLStd */
    if ( strcmp ( argv[2], "GetLStd" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLStd" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLStd */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetLStd();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetLStd */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRMean */
    if ( strcmp ( argv[2], "SetRMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetRMean(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetRMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRMean */
    if ( strcmp ( argv[2], "GetRMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetRMean();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetRMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRStd */
    if ( strcmp ( argv[2], "SetRStd" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRStd" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRStd */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetRStd(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetRStd */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRStd */
    if ( strcmp ( argv[2], "GetRStd" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRStd" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRStd */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetRStd();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetRStd */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLDegreeMean */
    if ( strcmp ( argv[2], "SetLDegreeMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLDegreeMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLDegreeMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetLDegreeMean(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetLDegreeMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLDegreeMean */
    if ( strcmp ( argv[2], "GetLDegreeMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLDegreeMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLDegreeMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetLDegreeMean();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetLDegreeMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLDegreeStd */
    if ( strcmp ( argv[2], "SetLDegreeStd" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLDegreeStd" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLDegreeStd */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetLDegreeStd(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetLDegreeStd */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLDegreeStd */
    if ( strcmp ( argv[2], "GetLDegreeStd" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLDegreeStd" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLDegreeStd */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetLDegreeStd();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetLDegreeStd */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRDegreeMean */
    if ( strcmp ( argv[2], "SetRDegreeMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRDegreeMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRDegreeMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetRDegreeMean(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetRDegreeMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRDegreeMean */
    if ( strcmp ( argv[2], "GetRDegreeMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRDegreeMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRDegreeMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetRDegreeMean();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetRDegreeMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRDegreeStd */
    if ( strcmp ( argv[2], "SetRDegreeStd" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRDegreeStd" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRDegreeStd */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetRDegreeStd(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetRDegreeStd */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRDegreeStd */
    if ( strcmp ( argv[2], "GetRDegreeStd" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRDegreeStd" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRDegreeStd */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetRDegreeStd();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetRDegreeStd */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLSize */
    if ( strcmp ( argv[2], "SetLSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLSize */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetLSize(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetLSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLSize */
    if ( strcmp ( argv[2], "GetLSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLSize */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetLSize();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetLSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRSize */
    if ( strcmp ( argv[2], "SetRSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRSize */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetRSize(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for SetRSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRSize */
    if ( strcmp ( argv[2], "GetRSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRSize */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetRSize();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisIpsilateralConnMap" );
    /* Closing for GetRSize */

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
