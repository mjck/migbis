// tcl wrapper for vtkpxTissueLevelSetFilter object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxTissueLevelSetFilter.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxTissueLevelSetFilterNewCommand()
{
  vtkpxTissueLevelSetFilter *temp = vtkpxTissueLevelSetFilter::New();
  return static_cast<ClientData>(temp);
}

int vtkpxBaseLevelSetFilterCppCommand(vtkpxBaseLevelSetFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxTissueLevelSetFilterCppCommand(vtkpxTissueLevelSetFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxTissueLevelSetFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxTissueLevelSetFilterCppCommand(static_cast<vtkpxTissueLevelSetFilter *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxTissueLevelSetFilterCppCommand(vtkpxTissueLevelSetFilter *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxTissueLevelSetFilter",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxBaseLevelSetFilterCppCommand(static_cast<vtkpxBaseLevelSetFilter *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxBaseLevelSetFilter"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxTissueLevelSetFilter  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxTissueLevelSetFilter");
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
    vtkpxTissueLevelSetFilter  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxTissueLevelSetFilter");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxTissueLevelSetFilter  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxTissueLevelSetFilter");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetInsideMean",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetInsideMean(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInsideMean",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInsideMean();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetInsideSigma",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetInsideSigma(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInsideSigma",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInsideSigma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOutsideMean",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetOutsideMean(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutsideMean",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetOutsideMean();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOutsideSigma",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetOutsideSigma(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutsideSigma",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetOutsideSigma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOutside2Mean",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetOutside2Mean(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutside2Mean",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetOutside2Mean();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOutside2Sigma",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetOutside2Sigma(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutside2Sigma",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetOutside2Sigma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetBiExponentialOutside",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetBiExponentialOutside(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBiExponentialOutsideMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBiExponentialOutsideMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBiExponentialOutsideMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBiExponentialOutsideMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBiExponentialOutside",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBiExponentialOutside();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("BiExponentialOutsideOn",argv[1]))&&(argc == 2))
    {
    op->BiExponentialOutsideOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("BiExponentialOutsideOff",argv[1]))&&(argc == 2))
    {
    op->BiExponentialOutsideOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetDynamicParameterUpdate",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetDynamicParameterUpdate(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDynamicParameterUpdateMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDynamicParameterUpdateMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDynamicParameterUpdateMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDynamicParameterUpdateMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDynamicParameterUpdate",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDynamicParameterUpdate();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("DynamicParameterUpdateOn",argv[1]))&&(argc == 2))
    {
    op->DynamicParameterUpdateOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("DynamicParameterUpdateOff",argv[1]))&&(argc == 2))
    {
    op->DynamicParameterUpdateOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetBeginLevel",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetBeginLevel(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBeginLevelMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBeginLevelMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBeginLevelMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBeginLevelMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBeginLevel",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBeginLevel();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetEndLevel",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetEndLevel(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetEndLevelMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetEndLevelMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetEndLevelMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetEndLevelMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetEndLevel",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetEndLevel();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
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
  if ((!strcmp("SetVolumeWeight",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetVolumeWeight(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetVolumeWeight",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetVolumeWeight();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetApproximateVolume",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetApproximateVolume(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetApproximateVolume",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetApproximateVolume();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxTissueLevelSetFilterCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxBaseLevelSetFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxTissueLevelSetFilter:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetInsideMean\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInsideMean\n",NULL);
    Tcl_AppendResult(interp,"  SetInsideSigma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInsideSigma\n",NULL);
    Tcl_AppendResult(interp,"  SetOutsideMean\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutsideMean\n",NULL);
    Tcl_AppendResult(interp,"  SetOutsideSigma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutsideSigma\n",NULL);
    Tcl_AppendResult(interp,"  SetOutside2Mean\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutside2Mean\n",NULL);
    Tcl_AppendResult(interp,"  SetOutside2Sigma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutside2Sigma\n",NULL);
    Tcl_AppendResult(interp,"  SetBiExponentialOutside\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBiExponentialOutsideMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBiExponentialOutsideMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBiExponentialOutside\n",NULL);
    Tcl_AppendResult(interp,"  BiExponentialOutsideOn\n",NULL);
    Tcl_AppendResult(interp,"  BiExponentialOutsideOff\n",NULL);
    Tcl_AppendResult(interp,"  SetDynamicParameterUpdate\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDynamicParameterUpdateMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDynamicParameterUpdateMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDynamicParameterUpdate\n",NULL);
    Tcl_AppendResult(interp,"  DynamicParameterUpdateOn\n",NULL);
    Tcl_AppendResult(interp,"  DynamicParameterUpdateOff\n",NULL);
    Tcl_AppendResult(interp,"  SetBeginLevel\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBeginLevelMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBeginLevelMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBeginLevel\n",NULL);
    Tcl_AppendResult(interp,"  SetEndLevel\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetEndLevelMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetEndLevelMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetEndLevel\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfLevels\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfLevels\n",NULL);
    Tcl_AppendResult(interp,"  SetVolumeWeight\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetVolumeWeight\n",NULL);
    Tcl_AppendResult(interp,"  SetApproximateVolume\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetApproximateVolume\n",NULL);
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
    vtkpxBaseLevelSetFilterCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetInsideMean" );
    Tcl_DStringAppendElement ( &dString, "GetInsideMean" );
    Tcl_DStringAppendElement ( &dString, "SetInsideSigma" );
    Tcl_DStringAppendElement ( &dString, "GetInsideSigma" );
    Tcl_DStringAppendElement ( &dString, "SetOutsideMean" );
    Tcl_DStringAppendElement ( &dString, "GetOutsideMean" );
    Tcl_DStringAppendElement ( &dString, "SetOutsideSigma" );
    Tcl_DStringAppendElement ( &dString, "GetOutsideSigma" );
    Tcl_DStringAppendElement ( &dString, "SetOutside2Mean" );
    Tcl_DStringAppendElement ( &dString, "GetOutside2Mean" );
    Tcl_DStringAppendElement ( &dString, "SetOutside2Sigma" );
    Tcl_DStringAppendElement ( &dString, "GetOutside2Sigma" );
    Tcl_DStringAppendElement ( &dString, "SetBiExponentialOutside" );
    Tcl_DStringAppendElement ( &dString, "GetBiExponentialOutsideMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetBiExponentialOutsideMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetBiExponentialOutside" );
    Tcl_DStringAppendElement ( &dString, "BiExponentialOutsideOn" );
    Tcl_DStringAppendElement ( &dString, "BiExponentialOutsideOff" );
    Tcl_DStringAppendElement ( &dString, "SetDynamicParameterUpdate" );
    Tcl_DStringAppendElement ( &dString, "GetDynamicParameterUpdateMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetDynamicParameterUpdateMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetDynamicParameterUpdate" );
    Tcl_DStringAppendElement ( &dString, "DynamicParameterUpdateOn" );
    Tcl_DStringAppendElement ( &dString, "DynamicParameterUpdateOff" );
    Tcl_DStringAppendElement ( &dString, "SetBeginLevel" );
    Tcl_DStringAppendElement ( &dString, "GetBeginLevelMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetBeginLevelMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetBeginLevel" );
    Tcl_DStringAppendElement ( &dString, "SetEndLevel" );
    Tcl_DStringAppendElement ( &dString, "GetEndLevelMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetEndLevelMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetEndLevel" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfLevels" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfLevels" );
    Tcl_DStringAppendElement ( &dString, "SetVolumeWeight" );
    Tcl_DStringAppendElement ( &dString, "GetVolumeWeight" );
    Tcl_DStringAppendElement ( &dString, "SetApproximateVolume" );
    Tcl_DStringAppendElement ( &dString, "GetApproximateVolume" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxBaseLevelSetFilterCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxTissueLevelSetFilter *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInsideMean */
    if ( strcmp ( argv[2], "SetInsideMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInsideMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInsideMean */
    Tcl_DStringAppendElement ( &dString, " Part 1 Tissue Description\n Inside Mean and Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInsideMean(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetInsideMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInsideMean */
    if ( strcmp ( argv[2], "GetInsideMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInsideMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInsideMean */
    Tcl_DStringAppendElement ( &dString, " Part 1 Tissue Description\n Inside Mean and Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInsideMean();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetInsideMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInsideSigma */
    if ( strcmp ( argv[2], "SetInsideSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInsideSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInsideSigma */
    Tcl_DStringAppendElement ( &dString, " Part 1 Tissue Description\n Inside Mean and Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInsideSigma(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetInsideSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInsideSigma */
    if ( strcmp ( argv[2], "GetInsideSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInsideSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInsideSigma */
    Tcl_DStringAppendElement ( &dString, " Part 1 Tissue Description\n Inside Mean and Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInsideSigma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetInsideSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutsideMean */
    if ( strcmp ( argv[2], "SetOutsideMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutsideMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutsideMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOutsideMean(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetOutsideMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutsideMean */
    if ( strcmp ( argv[2], "GetOutsideMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutsideMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutsideMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetOutsideMean();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetOutsideMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutsideSigma */
    if ( strcmp ( argv[2], "SetOutsideSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutsideSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutsideSigma */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOutsideSigma(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetOutsideSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutsideSigma */
    if ( strcmp ( argv[2], "GetOutsideSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutsideSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutsideSigma */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetOutsideSigma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetOutsideSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutside2Mean */
    if ( strcmp ( argv[2], "SetOutside2Mean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutside2Mean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutside2Mean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOutside2Mean(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetOutside2Mean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutside2Mean */
    if ( strcmp ( argv[2], "GetOutside2Mean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutside2Mean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutside2Mean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetOutside2Mean();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetOutside2Mean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutside2Sigma */
    if ( strcmp ( argv[2], "SetOutside2Sigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutside2Sigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutside2Sigma */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOutside2Sigma(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetOutside2Sigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutside2Sigma */
    if ( strcmp ( argv[2], "GetOutside2Sigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutside2Sigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutside2Sigma */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetOutside2Sigma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetOutside2Sigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBiExponentialOutside */
    if ( strcmp ( argv[2], "SetBiExponentialOutside" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBiExponentialOutside" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBiExponentialOutside */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetBiExponentialOutside(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetBiExponentialOutside */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBiExponentialOutsideMinValue */
    if ( strcmp ( argv[2], "GetBiExponentialOutsideMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBiExponentialOutsideMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBiExponentialOutsideMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetBiExponentialOutsideMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetBiExponentialOutsideMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBiExponentialOutsideMaxValue */
    if ( strcmp ( argv[2], "GetBiExponentialOutsideMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBiExponentialOutsideMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBiExponentialOutsideMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetBiExponentialOutsideMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetBiExponentialOutsideMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBiExponentialOutside */
    if ( strcmp ( argv[2], "GetBiExponentialOutside" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBiExponentialOutside" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBiExponentialOutside */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetBiExponentialOutside();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetBiExponentialOutside */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: BiExponentialOutsideOn */
    if ( strcmp ( argv[2], "BiExponentialOutsideOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "BiExponentialOutsideOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for BiExponentialOutsideOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void BiExponentialOutsideOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for BiExponentialOutsideOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: BiExponentialOutsideOff */
    if ( strcmp ( argv[2], "BiExponentialOutsideOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "BiExponentialOutsideOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for BiExponentialOutsideOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void BiExponentialOutsideOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for BiExponentialOutsideOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDynamicParameterUpdate */
    if ( strcmp ( argv[2], "SetDynamicParameterUpdate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDynamicParameterUpdate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDynamicParameterUpdate */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetDynamicParameterUpdate(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetDynamicParameterUpdate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDynamicParameterUpdateMinValue */
    if ( strcmp ( argv[2], "GetDynamicParameterUpdateMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDynamicParameterUpdateMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDynamicParameterUpdateMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetDynamicParameterUpdateMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetDynamicParameterUpdateMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDynamicParameterUpdateMaxValue */
    if ( strcmp ( argv[2], "GetDynamicParameterUpdateMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDynamicParameterUpdateMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDynamicParameterUpdateMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetDynamicParameterUpdateMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetDynamicParameterUpdateMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDynamicParameterUpdate */
    if ( strcmp ( argv[2], "GetDynamicParameterUpdate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDynamicParameterUpdate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDynamicParameterUpdate */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetDynamicParameterUpdate();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetDynamicParameterUpdate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DynamicParameterUpdateOn */
    if ( strcmp ( argv[2], "DynamicParameterUpdateOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DynamicParameterUpdateOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DynamicParameterUpdateOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void DynamicParameterUpdateOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for DynamicParameterUpdateOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DynamicParameterUpdateOff */
    if ( strcmp ( argv[2], "DynamicParameterUpdateOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DynamicParameterUpdateOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DynamicParameterUpdateOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void DynamicParameterUpdateOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for DynamicParameterUpdateOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBeginLevel */
    if ( strcmp ( argv[2], "SetBeginLevel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBeginLevel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBeginLevel */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetBeginLevel(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetBeginLevel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBeginLevelMinValue */
    if ( strcmp ( argv[2], "GetBeginLevelMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBeginLevelMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBeginLevelMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetBeginLevelMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetBeginLevelMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBeginLevelMaxValue */
    if ( strcmp ( argv[2], "GetBeginLevelMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBeginLevelMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBeginLevelMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetBeginLevelMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetBeginLevelMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBeginLevel */
    if ( strcmp ( argv[2], "GetBeginLevel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBeginLevel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBeginLevel */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetBeginLevel();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetBeginLevel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetEndLevel */
    if ( strcmp ( argv[2], "SetEndLevel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetEndLevel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetEndLevel */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetEndLevel(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetEndLevel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetEndLevelMinValue */
    if ( strcmp ( argv[2], "GetEndLevelMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetEndLevelMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetEndLevelMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetEndLevelMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetEndLevelMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetEndLevelMaxValue */
    if ( strcmp ( argv[2], "GetEndLevelMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetEndLevelMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetEndLevelMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetEndLevelMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetEndLevelMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetEndLevel */
    if ( strcmp ( argv[2], "GetEndLevel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetEndLevel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetEndLevel */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetEndLevel();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetEndLevel */

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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetNumberOfLevels(int n);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfLevels();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetNumberOfLevels */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetVolumeWeight */
    if ( strcmp ( argv[2], "SetVolumeWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetVolumeWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetVolumeWeight */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetVolumeWeight(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetVolumeWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetVolumeWeight */
    if ( strcmp ( argv[2], "GetVolumeWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetVolumeWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetVolumeWeight */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetVolumeWeight();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetVolumeWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetApproximateVolume */
    if ( strcmp ( argv[2], "SetApproximateVolume" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetApproximateVolume" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetApproximateVolume */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetApproximateVolume(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for SetApproximateVolume */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetApproximateVolume */
    if ( strcmp ( argv[2], "GetApproximateVolume" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetApproximateVolume" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetApproximateVolume */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetApproximateVolume();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTissueLevelSetFilter" );
    /* Closing for GetApproximateVolume */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxBaseLevelSetFilterCppCommand(static_cast<vtkpxBaseLevelSetFilter *>(op),interp,argc,argv) == TCL_OK)
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
