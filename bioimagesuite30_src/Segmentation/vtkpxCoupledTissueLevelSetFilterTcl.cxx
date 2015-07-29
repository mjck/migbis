// tcl wrapper for vtkpxCoupledTissueLevelSetFilter object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxCoupledTissueLevelSetFilter.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxCoupledTissueLevelSetFilterNewCommand()
{
  vtkpxCoupledTissueLevelSetFilter *temp = vtkpxCoupledTissueLevelSetFilter::New();
  return static_cast<ClientData>(temp);
}

int vtkpxBaseLevelSetFilterCppCommand(vtkpxBaseLevelSetFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxCoupledTissueLevelSetFilterCppCommand(vtkpxCoupledTissueLevelSetFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxCoupledTissueLevelSetFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxCoupledTissueLevelSetFilterCppCommand(static_cast<vtkpxCoupledTissueLevelSetFilter *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxCoupledTissueLevelSetFilterCppCommand(vtkpxCoupledTissueLevelSetFilter *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxCoupledTissueLevelSetFilter",argv[1]))
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
    vtkpxCoupledTissueLevelSetFilter  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxCoupledTissueLevelSetFilter");
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
    vtkpxCoupledTissueLevelSetFilter  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxCoupledTissueLevelSetFilter");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxCoupledTissueLevelSetFilter  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxCoupledTissueLevelSetFilter");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetGrayMatterMean",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetGrayMatterMean(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetGrayMatterMean",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetGrayMatterMean();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetGrayMatterSigma",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetGrayMatterSigma(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetGrayMatterSigma",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetGrayMatterSigma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetWhiteMatterMean",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetWhiteMatterMean(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetWhiteMatterMean",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetWhiteMatterMean();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetWhiteMatterSigma",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetWhiteMatterSigma(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetWhiteMatterSigma",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetWhiteMatterSigma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCSFMean",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetCSFMean(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCSFMean",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetCSFMean();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCSFSigma",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetCSFSigma(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCSFSigma",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetCSFSigma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPresetMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetPresetMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutput2",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetOutput2();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetMinThickness",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMinThickness(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMinThicknessMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMinThicknessMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinThicknessMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMinThicknessMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinThickness",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMinThickness();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMaxThickness",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMaxThickness(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaxThicknessMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMaxThicknessMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxThicknessMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMaxThicknessMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxThickness",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMaxThickness();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPadding",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetPadding(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPaddingMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPaddingMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPaddingMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPaddingMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPadding",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPadding();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCouplingOffset",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetCouplingOffset(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCouplingOffsetMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetCouplingOffsetMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCouplingOffsetMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetCouplingOffsetMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCouplingOffset",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetCouplingOffset();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("CreatePaddedImage",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    int      temp1;
    double   temp2;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->CreatePaddedImage(temp0,temp1,temp2);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateInnerOutputImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    int      temp1;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->CreateInnerOutputImage(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateOuterOutputImage",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    int      temp1;
    int      temp2;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->CreateOuterOutputImage(temp0,temp1,temp2);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxCoupledTissueLevelSetFilterCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxBaseLevelSetFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxCoupledTissueLevelSetFilter:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetGrayMatterMean\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetGrayMatterMean\n",NULL);
    Tcl_AppendResult(interp,"  SetGrayMatterSigma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetGrayMatterSigma\n",NULL);
    Tcl_AppendResult(interp,"  SetWhiteMatterMean\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetWhiteMatterMean\n",NULL);
    Tcl_AppendResult(interp,"  SetWhiteMatterSigma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetWhiteMatterSigma\n",NULL);
    Tcl_AppendResult(interp,"  SetCSFMean\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCSFMean\n",NULL);
    Tcl_AppendResult(interp,"  SetCSFSigma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCSFSigma\n",NULL);
    Tcl_AppendResult(interp,"  SetPresetMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutput2\n",NULL);
    Tcl_AppendResult(interp,"  SetMinThickness\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMinThicknessMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMinThicknessMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMinThickness\n",NULL);
    Tcl_AppendResult(interp,"  SetMaxThickness\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxThicknessMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxThicknessMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxThickness\n",NULL);
    Tcl_AppendResult(interp,"  SetPadding\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPaddingMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPaddingMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPadding\n",NULL);
    Tcl_AppendResult(interp,"  SetCouplingOffset\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCouplingOffsetMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCouplingOffsetMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCouplingOffset\n",NULL);
    Tcl_AppendResult(interp,"  CreatePaddedImage\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateInnerOutputImage\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateOuterOutputImage\t with 3 args\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetGrayMatterMean" );
    Tcl_DStringAppendElement ( &dString, "GetGrayMatterMean" );
    Tcl_DStringAppendElement ( &dString, "SetGrayMatterSigma" );
    Tcl_DStringAppendElement ( &dString, "GetGrayMatterSigma" );
    Tcl_DStringAppendElement ( &dString, "SetWhiteMatterMean" );
    Tcl_DStringAppendElement ( &dString, "GetWhiteMatterMean" );
    Tcl_DStringAppendElement ( &dString, "SetWhiteMatterSigma" );
    Tcl_DStringAppendElement ( &dString, "GetWhiteMatterSigma" );
    Tcl_DStringAppendElement ( &dString, "SetCSFMean" );
    Tcl_DStringAppendElement ( &dString, "GetCSFMean" );
    Tcl_DStringAppendElement ( &dString, "SetCSFSigma" );
    Tcl_DStringAppendElement ( &dString, "GetCSFSigma" );
    Tcl_DStringAppendElement ( &dString, "SetPresetMode" );
    Tcl_DStringAppendElement ( &dString, "GetOutput2" );
    Tcl_DStringAppendElement ( &dString, "SetMinThickness" );
    Tcl_DStringAppendElement ( &dString, "GetMinThicknessMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMinThicknessMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMinThickness" );
    Tcl_DStringAppendElement ( &dString, "SetMaxThickness" );
    Tcl_DStringAppendElement ( &dString, "GetMaxThicknessMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMaxThicknessMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMaxThickness" );
    Tcl_DStringAppendElement ( &dString, "SetPadding" );
    Tcl_DStringAppendElement ( &dString, "GetPaddingMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetPaddingMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetPadding" );
    Tcl_DStringAppendElement ( &dString, "SetCouplingOffset" );
    Tcl_DStringAppendElement ( &dString, "GetCouplingOffsetMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetCouplingOffsetMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetCouplingOffset" );
    Tcl_DStringAppendElement ( &dString, "CreatePaddedImage" );
    Tcl_DStringAppendElement ( &dString, "CreateInnerOutputImage" );
    Tcl_DStringAppendElement ( &dString, "CreateOuterOutputImage" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxCoupledTissueLevelSetFilter *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetGrayMatterMean */
    if ( strcmp ( argv[2], "SetGrayMatterMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetGrayMatterMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetGrayMatterMean */
    Tcl_DStringAppendElement ( &dString, " Part 1 Coupled Description\n Inside Mean and Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "void SetGrayMatterMean(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetGrayMatterMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGrayMatterMean */
    if ( strcmp ( argv[2], "GetGrayMatterMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGrayMatterMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGrayMatterMean */
    Tcl_DStringAppendElement ( &dString, " Part 1 Coupled Description\n Inside Mean and Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "float GetGrayMatterMean();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetGrayMatterMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetGrayMatterSigma */
    if ( strcmp ( argv[2], "SetGrayMatterSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetGrayMatterSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetGrayMatterSigma */
    Tcl_DStringAppendElement ( &dString, " Part 1 Coupled Description\n Inside Mean and Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "void SetGrayMatterSigma(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetGrayMatterSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGrayMatterSigma */
    if ( strcmp ( argv[2], "GetGrayMatterSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGrayMatterSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGrayMatterSigma */
    Tcl_DStringAppendElement ( &dString, " Part 1 Coupled Description\n Inside Mean and Sigma\n" );
    Tcl_DStringAppendElement ( &dString, "float GetGrayMatterSigma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetGrayMatterSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetWhiteMatterMean */
    if ( strcmp ( argv[2], "SetWhiteMatterMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetWhiteMatterMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetWhiteMatterMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetWhiteMatterMean(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetWhiteMatterMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWhiteMatterMean */
    if ( strcmp ( argv[2], "GetWhiteMatterMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWhiteMatterMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWhiteMatterMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetWhiteMatterMean();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetWhiteMatterMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetWhiteMatterSigma */
    if ( strcmp ( argv[2], "SetWhiteMatterSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetWhiteMatterSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetWhiteMatterSigma */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetWhiteMatterSigma(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetWhiteMatterSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWhiteMatterSigma */
    if ( strcmp ( argv[2], "GetWhiteMatterSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWhiteMatterSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWhiteMatterSigma */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetWhiteMatterSigma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetWhiteMatterSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCSFMean */
    if ( strcmp ( argv[2], "SetCSFMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCSFMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCSFMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetCSFMean(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetCSFMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCSFMean */
    if ( strcmp ( argv[2], "GetCSFMean" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCSFMean" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCSFMean */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetCSFMean();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetCSFMean */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCSFSigma */
    if ( strcmp ( argv[2], "SetCSFSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCSFSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCSFSigma */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetCSFSigma(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetCSFSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCSFSigma */
    if ( strcmp ( argv[2], "GetCSFSigma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCSFSigma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCSFSigma */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetCSFSigma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetCSFSigma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPresetMode */
    if ( strcmp ( argv[2], "SetPresetMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPresetMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPresetMode */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetPresetMode(int mode);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetPresetMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutput2 */
    if ( strcmp ( argv[2], "GetOutput2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutput2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutput2 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetOutput2();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetOutput2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMinThickness */
    if ( strcmp ( argv[2], "SetMinThickness" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMinThickness" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMinThickness */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetMinThickness(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetMinThickness */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinThicknessMinValue */
    if ( strcmp ( argv[2], "GetMinThicknessMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinThicknessMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinThicknessMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetMinThicknessMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetMinThicknessMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinThicknessMaxValue */
    if ( strcmp ( argv[2], "GetMinThicknessMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinThicknessMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinThicknessMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetMinThicknessMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetMinThicknessMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinThickness */
    if ( strcmp ( argv[2], "GetMinThickness" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinThickness" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinThickness */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetMinThickness();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetMinThickness */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaxThickness */
    if ( strcmp ( argv[2], "SetMaxThickness" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaxThickness" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaxThickness */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetMaxThickness(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetMaxThickness */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxThicknessMinValue */
    if ( strcmp ( argv[2], "GetMaxThicknessMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxThicknessMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxThicknessMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetMaxThicknessMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetMaxThicknessMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxThicknessMaxValue */
    if ( strcmp ( argv[2], "GetMaxThicknessMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxThicknessMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxThicknessMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetMaxThicknessMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetMaxThicknessMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxThickness */
    if ( strcmp ( argv[2], "GetMaxThickness" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxThickness" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxThickness */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetMaxThickness();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetMaxThickness */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPadding */
    if ( strcmp ( argv[2], "SetPadding" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPadding" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPadding */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetPadding(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetPadding */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPaddingMinValue */
    if ( strcmp ( argv[2], "GetPaddingMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPaddingMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPaddingMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetPaddingMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetPaddingMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPaddingMaxValue */
    if ( strcmp ( argv[2], "GetPaddingMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPaddingMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPaddingMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetPaddingMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetPaddingMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPadding */
    if ( strcmp ( argv[2], "GetPadding" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPadding" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPadding */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetPadding();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetPadding */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCouplingOffset */
    if ( strcmp ( argv[2], "SetCouplingOffset" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCouplingOffset" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCouplingOffset */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetCouplingOffset(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for SetCouplingOffset */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCouplingOffsetMinValue */
    if ( strcmp ( argv[2], "GetCouplingOffsetMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCouplingOffsetMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCouplingOffsetMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetCouplingOffsetMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetCouplingOffsetMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCouplingOffsetMaxValue */
    if ( strcmp ( argv[2], "GetCouplingOffsetMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCouplingOffsetMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCouplingOffsetMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetCouplingOffsetMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetCouplingOffsetMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCouplingOffset */
    if ( strcmp ( argv[2], "GetCouplingOffset" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCouplingOffset" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCouplingOffset */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float GetCouplingOffset();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for GetCouplingOffset */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreatePaddedImage */
    if ( strcmp ( argv[2], "CreatePaddedImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreatePaddedImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreatePaddedImage */
    Tcl_DStringAppendElement ( &dString, " Operations for creating and splitting padded images\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *CreatePaddedImage(vtkImageData *input, int padding, double offset);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for CreatePaddedImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateInnerOutputImage */
    if ( strcmp ( argv[2], "CreateInnerOutputImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateInnerOutputImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateInnerOutputImage */
    Tcl_DStringAppendElement ( &dString, " Operations for creating and splitting padded images\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *CreateInnerOutputImage(vtkImageData *result, int dimx);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for CreateInnerOutputImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateOuterOutputImage */
    if ( strcmp ( argv[2], "CreateOuterOutputImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateOuterOutputImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateOuterOutputImage */
    Tcl_DStringAppendElement ( &dString, " Operations for creating and splitting padded images\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *CreateOuterOutputImage(vtkImageData *result, int padding, int dimx);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxCoupledTissueLevelSetFilter" );
    /* Closing for CreateOuterOutputImage */

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
