// tcl wrapper for vtkpxJointHistogram object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxJointHistogram.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxJointHistogramNewCommand()
{
  vtkpxJointHistogram *temp = vtkpxJointHistogram::New();
  return static_cast<ClientData>(temp);
}

int vtkObjectCppCommand(vtkObject *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxJointHistogramCppCommand(vtkpxJointHistogram *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxJointHistogramCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxJointHistogramCppCommand(static_cast<vtkpxJointHistogram *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxJointHistogramCppCommand(vtkpxJointHistogram *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxJointHistogram",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkObjectCppCommand(static_cast<vtkObject *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkObject"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxJointHistogram  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxJointHistogram");
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
    vtkpxJointHistogram  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxJointHistogram");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxJointHistogram  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxJointHistogram");
    return TCL_OK;
    }
    }
  if ((!strcmp("ResetHistogram",argv[1]))&&(argc == 2))
    {
    op->ResetHistogram();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("BackupHistogram",argv[1]))&&(argc == 2))
    {
    op->BackupHistogram();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfBins",argv[1]))&&(argc == 2))
    {
    int     *temp20;
    temp20 = (op)->GetNumberOfBins();
    if(temp20)
      {
      char tempResult[1024];
      *tempResult = '\0';
      sprintf(tempResult,"%i %i ",temp20[0],temp20[1]);
      Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
      }
    else
      {
      Tcl_SetResult(interp, const_cast<char *>(""), TCL_VOLATILE);
      }
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfBinsX",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfBinsX();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfBinsY",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfBinsY();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Reset",argv[1]))&&(argc == 2))
    {
    op->Reset();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("Allocate",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->Allocate(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfSamples",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfSamples();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("AddToBin",argv[1]))&&(argc == 5))
    {
    short    temp0;
    short    temp1;
    short    temp2;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->AddToBin(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("AddToBin",argv[1]))&&(argc == 5))
    {
    unsigned char     temp0;
    unsigned char     temp1;
    unsigned char     temp2;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = static_cast<unsigned char>(tempi);
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = static_cast<unsigned char>(tempi);
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = static_cast<unsigned char>(tempi);
    if (!error)
    {
    temp20 = (op)->AddToBin(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("DelToBin",argv[1]))&&(argc == 5))
    {
    short    temp0;
    short    temp1;
    short    temp2;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->DelToBin(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("DelToBin",argv[1]))&&(argc == 5))
    {
    unsigned char     temp0;
    unsigned char     temp1;
    unsigned char     temp2;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = static_cast<unsigned char>(tempi);
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = static_cast<unsigned char>(tempi);
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = static_cast<unsigned char>(tempi);
    if (!error)
    {
    temp20 = (op)->DelToBin(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FillHistogram",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->FillHistogram(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("WeightedFillHistogram",argv[1]))&&(argc == 6))
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
    temp20 = (op)->WeightedFillHistogram(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("DualWeightedFillHistogram",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    vtkImageData  *temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->DualWeightedFillHistogram(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FillHistogramComponent",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->FillHistogramComponent(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FillGradientHistogram",argv[1]))&&(argc == 8))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    double   temp3;
    int      temp4;
    int      temp5;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (!error)
    {
    temp20 = (op)->FillGradientHistogram(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("RemoveFromHistogram",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->RemoveFromHistogram(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("RemoveFromHistogramComponent",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp2;
    int      temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->RemoveFromHistogramComponent(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("WeightedRemoveFromHistogram",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->WeightedRemoveFromHistogram(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("DualWeightedRemoveFromHistogram",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    vtkImageData  *temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->DualWeightedRemoveFromHistogram(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBinCount",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->GetBinCount(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetBinCount",argv[1]))&&(argc == 5))
    {
    int      temp0;
    int      temp1;
    int      temp2;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    op->SetBinCount(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("JointProbability",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->JointProbability(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("MarginalProbabilityX",argv[1]))&&(argc == 3))
    {
    int      temp0;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->MarginalProbabilityX(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("MarginalProbabilityY",argv[1]))&&(argc == 3))
    {
    int      temp0;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->MarginalProbabilityY(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ConditionalProbabilityXY",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->ConditionalProbabilityXY(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ConditionalProbabilityYX",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->ConditionalProbabilityYX(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("MeanX",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->MeanX();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("MeanY",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->MeanY();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ConditionalMeanXY",argv[1]))&&(argc == 3))
    {
    int      temp0;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->ConditionalMeanXY(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ConditionalMeanYX",argv[1]))&&(argc == 3))
    {
    int      temp0;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->ConditionalMeanYX(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("VarianceX",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->VarianceX();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("VarianceY",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->VarianceY();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Covariance",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->Covariance();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("StandardDeviationX",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->StandardDeviationX();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("StandardDeviationY",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->StandardDeviationY();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("EntropyX",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->EntropyX();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("EntropyY",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->EntropyY();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("JointEntropy",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->JointEntropy();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("MutualInformation",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->MutualInformation();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("NormalizedMutualInformation",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->NormalizedMutualInformation();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("CrossCorrelation",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->CrossCorrelation();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("CorrelationRatioXY",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->CorrelationRatioXY();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("CorrelationRatioYX",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->CorrelationRatioYX();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SumsOfSquaredDifferences",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->SumsOfSquaredDifferences();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ComputeProduct",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->ComputeProduct();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ComputeEntropyOfDifference",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->ComputeEntropyOfDifference();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("NameOfMeasure",argv[1]))&&(argc == 3))
    {
    int      temp0;
    const char    *temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->NameOfMeasure(temp0);
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
    }
  if ((!strcmp("Similarity",argv[1]))&&(argc == 3))
    {
    int      temp0;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->Similarity(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetHistogramAsImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetHistogramAsImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetHistogramFromImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->SetHistogramFromImage(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateHistogram",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    int      temp1;
    vtkImageData  *temp2;
    int      temp3;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    op->CreateHistogram(temp0,temp1,temp2,temp3);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateDualWeightedHistogram",argv[1]))&&(argc == 8))
    {
    vtkImageData  *temp0;
    int      temp1;
    vtkImageData  *temp2;
    int      temp3;
    vtkImageData  *temp4;
    vtkImageData  *temp5;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    temp4 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[6],const_cast<char *>("vtkImageData"),interp,error));
    temp5 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[7],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->CreateDualWeightedHistogram(temp0,temp1,temp2,temp3,temp4,temp5);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxJointHistogramCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkObjectCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxJointHistogram:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ResetHistogram\n",NULL);
    Tcl_AppendResult(interp,"  BackupHistogram\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBins\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBinsX\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfBinsY\n",NULL);
    Tcl_AppendResult(interp,"  Reset\n",NULL);
    Tcl_AppendResult(interp,"  Allocate\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfSamples\n",NULL);
    Tcl_AppendResult(interp,"  AddToBin\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  AddToBin\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  DelToBin\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  DelToBin\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  FillHistogram\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  WeightedFillHistogram\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  DualWeightedFillHistogram\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  FillHistogramComponent\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  FillGradientHistogram\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  RemoveFromHistogram\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  RemoveFromHistogramComponent\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  WeightedRemoveFromHistogram\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  DualWeightedRemoveFromHistogram\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  GetBinCount\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetBinCount\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  JointProbability\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  MarginalProbabilityX\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  MarginalProbabilityY\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ConditionalProbabilityXY\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ConditionalProbabilityYX\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  MeanX\n",NULL);
    Tcl_AppendResult(interp,"  MeanY\n",NULL);
    Tcl_AppendResult(interp,"  ConditionalMeanXY\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ConditionalMeanYX\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  VarianceX\n",NULL);
    Tcl_AppendResult(interp,"  VarianceY\n",NULL);
    Tcl_AppendResult(interp,"  Covariance\n",NULL);
    Tcl_AppendResult(interp,"  StandardDeviationX\n",NULL);
    Tcl_AppendResult(interp,"  StandardDeviationY\n",NULL);
    Tcl_AppendResult(interp,"  EntropyX\n",NULL);
    Tcl_AppendResult(interp,"  EntropyY\n",NULL);
    Tcl_AppendResult(interp,"  JointEntropy\n",NULL);
    Tcl_AppendResult(interp,"  MutualInformation\n",NULL);
    Tcl_AppendResult(interp,"  NormalizedMutualInformation\n",NULL);
    Tcl_AppendResult(interp,"  CrossCorrelation\n",NULL);
    Tcl_AppendResult(interp,"  CorrelationRatioXY\n",NULL);
    Tcl_AppendResult(interp,"  CorrelationRatioYX\n",NULL);
    Tcl_AppendResult(interp,"  SumsOfSquaredDifferences\n",NULL);
    Tcl_AppendResult(interp,"  ComputeProduct\n",NULL);
    Tcl_AppendResult(interp,"  ComputeEntropyOfDifference\n",NULL);
    Tcl_AppendResult(interp,"  NameOfMeasure\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Similarity\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetHistogramAsImage\n",NULL);
    Tcl_AppendResult(interp,"  SetHistogramFromImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  CreateHistogram\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateDualWeightedHistogram\t with 6 args\n",NULL);
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
    vtkObjectCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "ResetHistogram" );
    Tcl_DStringAppendElement ( &dString, "BackupHistogram" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBins" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBinsX" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBinsY" );
    Tcl_DStringAppendElement ( &dString, "Reset" );
    Tcl_DStringAppendElement ( &dString, "Allocate" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfSamples" );
    Tcl_DStringAppendElement ( &dString, "AddToBin" );
    Tcl_DStringAppendElement ( &dString, "AddToBin" );
    Tcl_DStringAppendElement ( &dString, "DelToBin" );
    Tcl_DStringAppendElement ( &dString, "DelToBin" );
    Tcl_DStringAppendElement ( &dString, "FillHistogram" );
    Tcl_DStringAppendElement ( &dString, "WeightedFillHistogram" );
    Tcl_DStringAppendElement ( &dString, "DualWeightedFillHistogram" );
    Tcl_DStringAppendElement ( &dString, "FillHistogramComponent" );
    Tcl_DStringAppendElement ( &dString, "FillGradientHistogram" );
    Tcl_DStringAppendElement ( &dString, "RemoveFromHistogram" );
    Tcl_DStringAppendElement ( &dString, "RemoveFromHistogramComponent" );
    Tcl_DStringAppendElement ( &dString, "WeightedRemoveFromHistogram" );
    Tcl_DStringAppendElement ( &dString, "DualWeightedRemoveFromHistogram" );
    Tcl_DStringAppendElement ( &dString, "GetBinCount" );
    Tcl_DStringAppendElement ( &dString, "SetBinCount" );
    Tcl_DStringAppendElement ( &dString, "JointProbability" );
    Tcl_DStringAppendElement ( &dString, "MarginalProbabilityX" );
    Tcl_DStringAppendElement ( &dString, "MarginalProbabilityY" );
    Tcl_DStringAppendElement ( &dString, "ConditionalProbabilityXY" );
    Tcl_DStringAppendElement ( &dString, "ConditionalProbabilityYX" );
    Tcl_DStringAppendElement ( &dString, "MeanX" );
    Tcl_DStringAppendElement ( &dString, "MeanY" );
    Tcl_DStringAppendElement ( &dString, "ConditionalMeanXY" );
    Tcl_DStringAppendElement ( &dString, "ConditionalMeanYX" );
    Tcl_DStringAppendElement ( &dString, "VarianceX" );
    Tcl_DStringAppendElement ( &dString, "VarianceY" );
    Tcl_DStringAppendElement ( &dString, "Covariance" );
    Tcl_DStringAppendElement ( &dString, "StandardDeviationX" );
    Tcl_DStringAppendElement ( &dString, "StandardDeviationY" );
    Tcl_DStringAppendElement ( &dString, "EntropyX" );
    Tcl_DStringAppendElement ( &dString, "EntropyY" );
    Tcl_DStringAppendElement ( &dString, "JointEntropy" );
    Tcl_DStringAppendElement ( &dString, "MutualInformation" );
    Tcl_DStringAppendElement ( &dString, "NormalizedMutualInformation" );
    Tcl_DStringAppendElement ( &dString, "CrossCorrelation" );
    Tcl_DStringAppendElement ( &dString, "CorrelationRatioXY" );
    Tcl_DStringAppendElement ( &dString, "CorrelationRatioYX" );
    Tcl_DStringAppendElement ( &dString, "SumsOfSquaredDifferences" );
    Tcl_DStringAppendElement ( &dString, "ComputeProduct" );
    Tcl_DStringAppendElement ( &dString, "ComputeEntropyOfDifference" );
    Tcl_DStringAppendElement ( &dString, "NameOfMeasure" );
    Tcl_DStringAppendElement ( &dString, "Similarity" );
    Tcl_DStringAppendElement ( &dString, "GetHistogramAsImage" );
    Tcl_DStringAppendElement ( &dString, "SetHistogramFromImage" );
    Tcl_DStringAppendElement ( &dString, "CreateHistogram" );
    Tcl_DStringAppendElement ( &dString, "CreateDualWeightedHistogram" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkObjectCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxJointHistogram *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ResetHistogram */
    if ( strcmp ( argv[2], "ResetHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ResetHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ResetHistogram */
    Tcl_DStringAppendElement ( &dString, " Backup and Reset Histogram\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void ResetHistogram();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for ResetHistogram */

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
    Tcl_DStringAppendElement ( &dString, " Backup and Reset Histogram\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void BackupHistogram();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for BackupHistogram */

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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int *GetNumberOfBins();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for GetNumberOfBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfBinsX */
    if ( strcmp ( argv[2], "GetNumberOfBinsX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBinsX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfBinsX */
    Tcl_DStringAppendElement ( &dString, " Get/Set number of bins\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumberOfBinsX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for GetNumberOfBinsX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfBinsY */
    if ( strcmp ( argv[2], "GetNumberOfBinsY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfBinsY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfBinsY */
    Tcl_DStringAppendElement ( &dString, " Get/Set number of bins\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumberOfBinsY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for GetNumberOfBinsY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Reset */
    if ( strcmp ( argv[2], "Reset" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Reset" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Reset */
    Tcl_DStringAppendElement ( &dString, " Clear histogram\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Reset();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for Reset */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Allocate */
    if ( strcmp ( argv[2], "Allocate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Allocate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Allocate */
    Tcl_DStringAppendElement ( &dString, " Allocate Histogram\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Allocate(int numx, int numy);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for Allocate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfSamples */
    if ( strcmp ( argv[2], "GetNumberOfSamples" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfSamples" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfSamples */
    Tcl_DStringAppendElement ( &dString, " Get number of samples in histogram\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfSamples();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for GetNumberOfSamples */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AddToBin */
    if ( strcmp ( argv[2], "AddToBin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AddToBin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddToBin */
    Tcl_DStringAppendElement ( &dString, " Add counts to bins\n" );
    Tcl_DStringAppendElement ( &dString, "int AddToBin(short x, short y, short count=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for AddToBin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AddToBin */
    if ( strcmp ( argv[2], "AddToBin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AddToBin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddToBin */
    Tcl_DStringAppendElement ( &dString, " Add counts to bins\n" );
    Tcl_DStringAppendElement ( &dString, "int AddToBin(unsigned char x, unsigned char y, unsigned char count=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for AddToBin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DelToBin */
    if ( strcmp ( argv[2], "DelToBin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DelToBin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DelToBin */
    Tcl_DStringAppendElement ( &dString, " Delete counts from bins\n" );
    Tcl_DStringAppendElement ( &dString, "int DelToBin(short x, short y, short count=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for DelToBin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DelToBin */
    if ( strcmp ( argv[2], "DelToBin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DelToBin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DelToBin */
    Tcl_DStringAppendElement ( &dString, " Delete counts from bins\n" );
    Tcl_DStringAppendElement ( &dString, "int DelToBin(unsigned char x, unsigned char y, unsigned char count=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for DelToBin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FillHistogram */
    if ( strcmp ( argv[2], "FillHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FillHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FillHistogram */
    Tcl_DStringAppendElement ( &dString, " Fill Histogram From Images \n" );
    Tcl_DStringAppendElement ( &dString, "int FillHistogram(vtkImageData *ref, vtkImageData *targ, int reset);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for FillHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: WeightedFillHistogram */
    if ( strcmp ( argv[2], "WeightedFillHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "WeightedFillHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for WeightedFillHistogram */
    Tcl_DStringAppendElement ( &dString, " Fill Histogram From Images \n" );
    Tcl_DStringAppendElement ( &dString, "int WeightedFillHistogram(vtkImageData *ref, vtkImageData *targ, vtkImageData *weights, int reset);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for WeightedFillHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DualWeightedFillHistogram */
    if ( strcmp ( argv[2], "DualWeightedFillHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DualWeightedFillHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DualWeightedFillHistogram */
    Tcl_DStringAppendElement ( &dString, " Fill Histogram From Images \n" );
    Tcl_DStringAppendElement ( &dString, "int DualWeightedFillHistogram(vtkImageData *ref, vtkImageData *targ, vtkImageData *weight1, vtkImageData *weights2, int reset);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for DualWeightedFillHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FillHistogramComponent */
    if ( strcmp ( argv[2], "FillHistogramComponent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FillHistogramComponent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FillHistogramComponent */
    Tcl_DStringAppendElement ( &dString, " Fill Histogram From Images \n" );
    Tcl_DStringAppendElement ( &dString, "int FillHistogramComponent(vtkImageData *ref, vtkImageData *targ, int component1, int component2, int reset);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for FillHistogramComponent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FillGradientHistogram */
    if ( strcmp ( argv[2], "FillGradientHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FillGradientHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FillGradientHistogram */
    Tcl_DStringAppendElement ( &dString, " Fill Histogram from Combination of Image and Intensity Gradient for Optimization\n" );
    Tcl_DStringAppendElement ( &dString, "int FillGradientHistogram(vtkImageData *ref, vtkImageData *targ, vtkImageData *grad, double step, int component, int reset);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for FillGradientHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RemoveFromHistogram */
    if ( strcmp ( argv[2], "RemoveFromHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RemoveFromHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RemoveFromHistogram */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int RemoveFromHistogram(vtkImageData *ref, vtkImageData *targ);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for RemoveFromHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: RemoveFromHistogramComponent */
    if ( strcmp ( argv[2], "RemoveFromHistogramComponent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "RemoveFromHistogramComponent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for RemoveFromHistogramComponent */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int RemoveFromHistogramComponent(vtkImageData *ref, vtkImageData *targ, int component1, int component2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for RemoveFromHistogramComponent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: WeightedRemoveFromHistogram */
    if ( strcmp ( argv[2], "WeightedRemoveFromHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "WeightedRemoveFromHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for WeightedRemoveFromHistogram */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int WeightedRemoveFromHistogram(vtkImageData *ref, vtkImageData *targ, vtkImageData *weights);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for WeightedRemoveFromHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DualWeightedRemoveFromHistogram */
    if ( strcmp ( argv[2], "DualWeightedRemoveFromHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DualWeightedRemoveFromHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DualWeightedRemoveFromHistogram */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int DualWeightedRemoveFromHistogram(vtkImageData *ref, vtkImageData *targ, vtkImageData *weight1, vtkImageData *weights2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for DualWeightedRemoveFromHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBinCount */
    if ( strcmp ( argv[2], "GetBinCount" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBinCount" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBinCount */
    Tcl_DStringAppendElement ( &dString, " Get Bin Count\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBinCount(int, int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for GetBinCount */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBinCount */
    if ( strcmp ( argv[2], "SetBinCount" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBinCount" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBinCount */
    Tcl_DStringAppendElement ( &dString, " Get Bin Count\n" );
    Tcl_DStringAppendElement ( &dString, "void SetBinCount(int a, int b, int val);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for SetBinCount */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: JointProbability */
    if ( strcmp ( argv[2], "JointProbability" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "JointProbability" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for JointProbability */
    Tcl_DStringAppendElement ( &dString, " Calculate joint probability p(x, y)\n" );
    Tcl_DStringAppendElement ( &dString, "float JointProbability(int, int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for JointProbability */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MarginalProbabilityX */
    if ( strcmp ( argv[2], "MarginalProbabilityX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MarginalProbabilityX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MarginalProbabilityX */
    Tcl_DStringAppendElement ( &dString, " Calculate marginal probability p(x)\n" );
    Tcl_DStringAppendElement ( &dString, "float MarginalProbabilityX(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for MarginalProbabilityX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MarginalProbabilityY */
    if ( strcmp ( argv[2], "MarginalProbabilityY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MarginalProbabilityY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MarginalProbabilityY */
    Tcl_DStringAppendElement ( &dString, " Calculate marginal probability p(y)\n" );
    Tcl_DStringAppendElement ( &dString, "float MarginalProbabilityY(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for MarginalProbabilityY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ConditionalProbabilityXY */
    if ( strcmp ( argv[2], "ConditionalProbabilityXY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ConditionalProbabilityXY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ConditionalProbabilityXY */
    Tcl_DStringAppendElement ( &dString, " Calculate conditional probability p(x|y)\n" );
    Tcl_DStringAppendElement ( &dString, "float ConditionalProbabilityXY(int, int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for ConditionalProbabilityXY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ConditionalProbabilityYX */
    if ( strcmp ( argv[2], "ConditionalProbabilityYX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ConditionalProbabilityYX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ConditionalProbabilityYX */
    Tcl_DStringAppendElement ( &dString, " Calculate conditional probability p(y|x)\n" );
    Tcl_DStringAppendElement ( &dString, "float ConditionalProbabilityYX(int, int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for ConditionalProbabilityYX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MeanX */
    if ( strcmp ( argv[2], "MeanX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MeanX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MeanX */
    Tcl_DStringAppendElement ( &dString, " Calculate mean\n" );
    Tcl_DStringAppendElement ( &dString, "float MeanX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for MeanX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MeanY */
    if ( strcmp ( argv[2], "MeanY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MeanY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MeanY */
    Tcl_DStringAppendElement ( &dString, " Calculate mean\n" );
    Tcl_DStringAppendElement ( &dString, "float MeanY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for MeanY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ConditionalMeanXY */
    if ( strcmp ( argv[2], "ConditionalMeanXY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ConditionalMeanXY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ConditionalMeanXY */
    Tcl_DStringAppendElement ( &dString, " Calculate conditional mean\n" );
    Tcl_DStringAppendElement ( &dString, "float ConditionalMeanXY(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for ConditionalMeanXY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ConditionalMeanYX */
    if ( strcmp ( argv[2], "ConditionalMeanYX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ConditionalMeanYX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ConditionalMeanYX */
    Tcl_DStringAppendElement ( &dString, " Calculate conditional mean\n" );
    Tcl_DStringAppendElement ( &dString, "float ConditionalMeanYX(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for ConditionalMeanYX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: VarianceX */
    if ( strcmp ( argv[2], "VarianceX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "VarianceX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for VarianceX */
    Tcl_DStringAppendElement ( &dString, " Calculate variance\n" );
    Tcl_DStringAppendElement ( &dString, "float VarianceX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for VarianceX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: VarianceY */
    if ( strcmp ( argv[2], "VarianceY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "VarianceY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for VarianceY */
    Tcl_DStringAppendElement ( &dString, " Calculate variance\n" );
    Tcl_DStringAppendElement ( &dString, "float VarianceY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for VarianceY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Covariance */
    if ( strcmp ( argv[2], "Covariance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Covariance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Covariance */
    Tcl_DStringAppendElement ( &dString, " Calculate covariance\n" );
    Tcl_DStringAppendElement ( &dString, "float Covariance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for Covariance */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: StandardDeviationX */
    if ( strcmp ( argv[2], "StandardDeviationX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "StandardDeviationX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for StandardDeviationX */
    Tcl_DStringAppendElement ( &dString, " Calculate standard deviation\n" );
    Tcl_DStringAppendElement ( &dString, "float StandardDeviationX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for StandardDeviationX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: StandardDeviationY */
    if ( strcmp ( argv[2], "StandardDeviationY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "StandardDeviationY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for StandardDeviationY */
    Tcl_DStringAppendElement ( &dString, " Calculate standard deviation\n" );
    Tcl_DStringAppendElement ( &dString, "float StandardDeviationY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for StandardDeviationY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: EntropyX */
    if ( strcmp ( argv[2], "EntropyX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "EntropyX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for EntropyX */
    Tcl_DStringAppendElement ( &dString, " Calculate marginal entropy\n" );
    Tcl_DStringAppendElement ( &dString, "float EntropyX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for EntropyX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: EntropyY */
    if ( strcmp ( argv[2], "EntropyY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "EntropyY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for EntropyY */
    Tcl_DStringAppendElement ( &dString, " Calculate marginal entropy\n" );
    Tcl_DStringAppendElement ( &dString, "float EntropyY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for EntropyY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: JointEntropy */
    if ( strcmp ( argv[2], "JointEntropy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "JointEntropy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for JointEntropy */
    Tcl_DStringAppendElement ( &dString, " Calculate joint entropy\n" );
    Tcl_DStringAppendElement ( &dString, "float JointEntropy();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for JointEntropy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MutualInformation */
    if ( strcmp ( argv[2], "MutualInformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MutualInformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MutualInformation */
    Tcl_DStringAppendElement ( &dString, " Calculate mutual information\n" );
    Tcl_DStringAppendElement ( &dString, "float MutualInformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for MutualInformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NormalizedMutualInformation */
    if ( strcmp ( argv[2], "NormalizedMutualInformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NormalizedMutualInformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NormalizedMutualInformation */
    Tcl_DStringAppendElement ( &dString, " Calculate normalized mutual information\n" );
    Tcl_DStringAppendElement ( &dString, "float NormalizedMutualInformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for NormalizedMutualInformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CrossCorrelation */
    if ( strcmp ( argv[2], "CrossCorrelation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CrossCorrelation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CrossCorrelation */
    Tcl_DStringAppendElement ( &dString, " Calculate cross correlation\n" );
    Tcl_DStringAppendElement ( &dString, "float CrossCorrelation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for CrossCorrelation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CorrelationRatioXY */
    if ( strcmp ( argv[2], "CorrelationRatioXY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CorrelationRatioXY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CorrelationRatioXY */
    Tcl_DStringAppendElement ( &dString, " Calculate correlation ratio\n" );
    Tcl_DStringAppendElement ( &dString, "float CorrelationRatioXY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for CorrelationRatioXY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CorrelationRatioYX */
    if ( strcmp ( argv[2], "CorrelationRatioYX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CorrelationRatioYX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CorrelationRatioYX */
    Tcl_DStringAppendElement ( &dString, " Calculate correlation ratio\n" );
    Tcl_DStringAppendElement ( &dString, "float CorrelationRatioYX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for CorrelationRatioYX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SumsOfSquaredDifferences */
    if ( strcmp ( argv[2], "SumsOfSquaredDifferences" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SumsOfSquaredDifferences" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SumsOfSquaredDifferences */
    Tcl_DStringAppendElement ( &dString, " Calculate sums of squared differences\n" );
    Tcl_DStringAppendElement ( &dString, "float SumsOfSquaredDifferences();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for SumsOfSquaredDifferences */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeProduct */
    if ( strcmp ( argv[2], "ComputeProduct" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeProduct" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeProduct */
    Tcl_DStringAppendElement ( &dString, " Compute Product (ala Convolution)\n" );
    Tcl_DStringAppendElement ( &dString, "float ComputeProduct();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for ComputeProduct */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeEntropyOfDifference */
    if ( strcmp ( argv[2], "ComputeEntropyOfDifference" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeEntropyOfDifference" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeEntropyOfDifference */
    Tcl_DStringAppendElement ( &dString, " Compute Entropy of (y-x)\n" );
    Tcl_DStringAppendElement ( &dString, "float ComputeEntropyOfDifference();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for ComputeEntropyOfDifference */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NameOfMeasure */
    if ( strcmp ( argv[2], "NameOfMeasure" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NameOfMeasure" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NameOfMeasure */
    Tcl_DStringAppendElement ( &dString, " Compute Similarity\n" );
    Tcl_DStringAppendElement ( &dString, "static const char *NameOfMeasure(int measureno);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for NameOfMeasure */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Similarity */
    if ( strcmp ( argv[2], "Similarity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Similarity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Similarity */
    Tcl_DStringAppendElement ( &dString, " Compute Similarity\n" );
    Tcl_DStringAppendElement ( &dString, "virtual float Similarity(int measureno);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for Similarity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetHistogramAsImage */
    if ( strcmp ( argv[2], "GetHistogramAsImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetHistogramAsImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetHistogramAsImage */
    Tcl_DStringAppendElement ( &dString, " Image Input/Output\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkImageData *GetHistogramAsImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for GetHistogramAsImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetHistogramFromImage */
    if ( strcmp ( argv[2], "SetHistogramFromImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetHistogramFromImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetHistogramFromImage */
    Tcl_DStringAppendElement ( &dString, " Image Input/Output\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int SetHistogramFromImage(vtkImageData *img);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for SetHistogramFromImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateHistogram */
    if ( strcmp ( argv[2], "CreateHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateHistogram */
    Tcl_DStringAppendElement ( &dString, " One Stop Shop\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void CreateHistogram(vtkImageData *ref, int numbinsref, vtkImageData *trg, int numbinstrg);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for CreateHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateDualWeightedHistogram */
    if ( strcmp ( argv[2], "CreateDualWeightedHistogram" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateDualWeightedHistogram" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateDualWeightedHistogram */
    Tcl_DStringAppendElement ( &dString, " One Stop Shop\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void CreateDualWeightedHistogram(vtkImageData *ref, int numbinsref, vtkImageData *trg, int numbinstrg, vtkImageData *refw, vtkImageData *trgw);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxJointHistogram" );
    /* Closing for CreateDualWeightedHistogram */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkObjectCppCommand(static_cast<vtkObject *>(op),interp,argc,argv) == TCL_OK)
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
