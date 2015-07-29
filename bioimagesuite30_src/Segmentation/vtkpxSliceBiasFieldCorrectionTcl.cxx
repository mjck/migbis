// tcl wrapper for vtkpxSliceBiasFieldCorrection object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxSliceBiasFieldCorrection.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxSliceBiasFieldCorrectionNewCommand()
{
  vtkpxSliceBiasFieldCorrection *temp = vtkpxSliceBiasFieldCorrection::New();
  return static_cast<ClientData>(temp);
}

int vtkpxAbstractBiasFieldCorrectionCppCommand(vtkpxAbstractBiasFieldCorrection *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxSliceBiasFieldCorrectionCppCommand(vtkpxSliceBiasFieldCorrection *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxSliceBiasFieldCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxSliceBiasFieldCorrectionCppCommand(static_cast<vtkpxSliceBiasFieldCorrection *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxSliceBiasFieldCorrectionCppCommand(vtkpxSliceBiasFieldCorrection *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxSliceBiasFieldCorrection",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxAbstractBiasFieldCorrectionCppCommand(static_cast<vtkpxAbstractBiasFieldCorrection *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxAbstractBiasFieldCorrection"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxSliceBiasFieldCorrection  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSliceBiasFieldCorrection");
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
    vtkpxSliceBiasFieldCorrection  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSliceBiasFieldCorrection");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxSliceBiasFieldCorrection  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSliceBiasFieldCorrection");
    return TCL_OK;
    }
    }
  if ((!strcmp("PrintInformation",argv[1]))&&(argc == 2))
    {
    op->PrintInformation();
    Tcl_ResetResult(interp);
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
  if ((!strcmp("GetSliceParameters",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetSliceParameters();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
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
  if ((!strcmp("SetAxis",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetAxis(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAxisMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAxisMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAxisMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAxisMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAxis",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetAxis();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPureScaling",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetPureScaling(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPureScalingMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPureScalingMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPureScalingMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPureScalingMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPureScaling",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPureScaling();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("PolynomialFitToParameters",argv[1]))&&(argc == 4))
    {
    int      temp0;
    vtkFloatArray  *temp1;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    temp1 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkFloatArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->PolynomialFitToParameters(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxSliceBiasFieldCorrectionCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxAbstractBiasFieldCorrectionCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxSliceBiasFieldCorrection:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  PrintInformation\n",NULL);
    Tcl_AppendResult(interp,"  GenerateSynthetic\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetSliceParameters\n",NULL);
    Tcl_AppendResult(interp,"  SetRobustMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRobustModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRobustModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetRobustMode\n",NULL);
    Tcl_AppendResult(interp,"  SetThreshold\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetThresholdMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetThresholdMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetThreshold\n",NULL);
    Tcl_AppendResult(interp,"  SetAxis\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAxisMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAxisMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAxis\n",NULL);
    Tcl_AppendResult(interp,"  SetPureScaling\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPureScalingMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPureScalingMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPureScaling\n",NULL);
    Tcl_AppendResult(interp,"  PolynomialFitToParameters\t with 2 args\n",NULL);
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
    vtkpxAbstractBiasFieldCorrectionCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "PrintInformation" );
    Tcl_DStringAppendElement ( &dString, "GenerateSynthetic" );
    Tcl_DStringAppendElement ( &dString, "GetSliceParameters" );
    Tcl_DStringAppendElement ( &dString, "SetRobustMode" );
    Tcl_DStringAppendElement ( &dString, "GetRobustModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetRobustModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetRobustMode" );
    Tcl_DStringAppendElement ( &dString, "SetThreshold" );
    Tcl_DStringAppendElement ( &dString, "GetThresholdMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetThresholdMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetThreshold" );
    Tcl_DStringAppendElement ( &dString, "SetAxis" );
    Tcl_DStringAppendElement ( &dString, "GetAxisMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetAxisMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetAxis" );
    Tcl_DStringAppendElement ( &dString, "SetPureScaling" );
    Tcl_DStringAppendElement ( &dString, "GetPureScalingMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetPureScalingMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetPureScaling" );
    Tcl_DStringAppendElement ( &dString, "PolynomialFitToParameters" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxAbstractBiasFieldCorrectionCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxSliceBiasFieldCorrection *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for PrintInformation */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for GenerateSynthetic */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSliceParameters */
    if ( strcmp ( argv[2], "GetSliceParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSliceParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSliceParameters */
    Tcl_DStringAppendElement ( &dString, " Get Internal Transformation Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetSliceParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for GetSliceParameters */

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
    Tcl_DStringAppendElement ( &dString, " RobustMode\n 0 == correlation\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRobustMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " RobustMode\n 0 == correlation\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRobustModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " RobustMode\n 0 == correlation\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRobustModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " RobustMode\n 0 == correlation\n" );
    Tcl_DStringAppendElement ( &dString, "int GetRobustMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for GetRobustMode */

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
    Tcl_DStringAppendElement ( &dString, " Threshold\n Values below this are ignored in image ratio computation (Image Range 0..1)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetThreshold(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " Threshold\n Values below this are ignored in image ratio computation (Image Range 0..1)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetThresholdMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " Threshold\n Values below this are ignored in image ratio computation (Image Range 0..1)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetThresholdMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
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
    Tcl_DStringAppendElement ( &dString, " Threshold\n Values below this are ignored in image ratio computation (Image Range 0..1)\n" );
    Tcl_DStringAppendElement ( &dString, "float GetThreshold();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for GetThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAxis */
    if ( strcmp ( argv[2], "SetAxis" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAxis" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAxis */
    Tcl_DStringAppendElement ( &dString, " Axis 0=X, 1=Y, 2=Z\n" );
    Tcl_DStringAppendElement ( &dString, "void SetAxis(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for SetAxis */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAxisMinValue */
    if ( strcmp ( argv[2], "GetAxisMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAxisMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAxisMinValue */
    Tcl_DStringAppendElement ( &dString, " Axis 0=X, 1=Y, 2=Z\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAxisMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for GetAxisMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAxisMaxValue */
    if ( strcmp ( argv[2], "GetAxisMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAxisMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAxisMaxValue */
    Tcl_DStringAppendElement ( &dString, " Axis 0=X, 1=Y, 2=Z\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAxisMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for GetAxisMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAxis */
    if ( strcmp ( argv[2], "GetAxis" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAxis" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAxis */
    Tcl_DStringAppendElement ( &dString, " Axis 0=X, 1=Y, 2=Z\n" );
    Tcl_DStringAppendElement ( &dString, "int GetAxis();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for GetAxis */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPureScaling */
    if ( strcmp ( argv[2], "SetPureScaling" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPureScaling" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPureScaling */
    Tcl_DStringAppendElement ( &dString, " Pure Scaling i.e. no Offset Computed\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPureScaling(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for SetPureScaling */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPureScalingMinValue */
    if ( strcmp ( argv[2], "GetPureScalingMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPureScalingMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPureScalingMinValue */
    Tcl_DStringAppendElement ( &dString, " Pure Scaling i.e. no Offset Computed\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPureScalingMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for GetPureScalingMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPureScalingMaxValue */
    if ( strcmp ( argv[2], "GetPureScalingMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPureScalingMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPureScalingMaxValue */
    Tcl_DStringAppendElement ( &dString, " Pure Scaling i.e. no Offset Computed\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPureScalingMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for GetPureScalingMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPureScaling */
    if ( strcmp ( argv[2], "GetPureScaling" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPureScaling" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPureScaling */
    Tcl_DStringAppendElement ( &dString, " Pure Scaling i.e. no Offset Computed\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPureScaling();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for GetPureScaling */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: PolynomialFitToParameters */
    if ( strcmp ( argv[2], "PolynomialFitToParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "PolynomialFitToParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for PolynomialFitToParameters */
    Tcl_DStringAppendElement ( &dString, " ComputePolynomial Fit \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int PolynomialFitToParameters(int order, vtkFloatArray *params);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSliceBiasFieldCorrection" );
    /* Closing for PolynomialFitToParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxAbstractBiasFieldCorrectionCppCommand(static_cast<vtkpxAbstractBiasFieldCorrection *>(op),interp,argc,argv) == TCL_OK)
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
