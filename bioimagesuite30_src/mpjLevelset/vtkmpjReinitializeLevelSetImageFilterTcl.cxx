// tcl wrapper for vtkmpjReinitializeLevelSetImageFilter object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkmpjReinitializeLevelSetImageFilter.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkmpjReinitializeLevelSetImageFilterNewCommand()
{
  vtkmpjReinitializeLevelSetImageFilter *temp = vtkmpjReinitializeLevelSetImageFilter::New();
  return static_cast<ClientData>(temp);
}

int vtkpxSimpleImageToImageFilterCppCommand(vtkpxSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkmpjReinitializeLevelSetImageFilterCppCommand(vtkmpjReinitializeLevelSetImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkmpjReinitializeLevelSetImageFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkmpjReinitializeLevelSetImageFilterCppCommand(static_cast<vtkmpjReinitializeLevelSetImageFilter *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkmpjReinitializeLevelSetImageFilterCppCommand(vtkmpjReinitializeLevelSetImageFilter *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkmpjReinitializeLevelSetImageFilter",argv[1]))
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
    vtkmpjReinitializeLevelSetImageFilter  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjReinitializeLevelSetImageFilter");
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
    vtkmpjReinitializeLevelSetImageFilter  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjReinitializeLevelSetImageFilter");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkmpjReinitializeLevelSetImageFilter  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjReinitializeLevelSetImageFilter");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetLevelSetValue",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLevelSetValue(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLevelSetValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLevelSetValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNarrowBanding",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNarrowBanding();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNarrowBanding",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNarrowBanding(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("NarrowBandingOn",argv[1]))&&(argc == 2))
    {
    op->NarrowBandingOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("NarrowBandingOff",argv[1]))&&(argc == 2))
    {
    op->NarrowBandingOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetInputNarrowBandwidth",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetInputNarrowBandwidth(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInputNarrowBandwidth",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetInputNarrowBandwidth();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOutputNarrowBandwidth",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetOutputNarrowBandwidth(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutputNarrowBandwidth",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetOutputNarrowBandwidth();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNarrowBandwidth",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetNarrowBandwidth(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetInputNarrowBand",argv[1]))&&(argc == 3))
    {
    vtkDataArray  *temp0;
    error = 0;

    temp0 = (vtkDataArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDataArray"),interp,error));
    if (!error)
    {
    op->SetInputNarrowBand(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutputNarrowBand",argv[1]))&&(argc == 2))
    {
    vtkDataArray  *temp20;
    temp20 = (op)->GetOutputNarrowBand();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDataArray");
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkmpjReinitializeLevelSetImageFilterCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkmpjReinitializeLevelSetImageFilter:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetLevelSetValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLevelSetValue\n",NULL);
    Tcl_AppendResult(interp,"  GetNarrowBanding\n",NULL);
    Tcl_AppendResult(interp,"  SetNarrowBanding\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NarrowBandingOn\n",NULL);
    Tcl_AppendResult(interp,"  NarrowBandingOff\n",NULL);
    Tcl_AppendResult(interp,"  SetInputNarrowBandwidth\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInputNarrowBandwidth\n",NULL);
    Tcl_AppendResult(interp,"  SetOutputNarrowBandwidth\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutputNarrowBandwidth\n",NULL);
    Tcl_AppendResult(interp,"  SetNarrowBandwidth\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetInputNarrowBand\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutputNarrowBand\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetLevelSetValue" );
    Tcl_DStringAppendElement ( &dString, "GetLevelSetValue" );
    Tcl_DStringAppendElement ( &dString, "GetNarrowBanding" );
    Tcl_DStringAppendElement ( &dString, "SetNarrowBanding" );
    Tcl_DStringAppendElement ( &dString, "NarrowBandingOn" );
    Tcl_DStringAppendElement ( &dString, "NarrowBandingOff" );
    Tcl_DStringAppendElement ( &dString, "SetInputNarrowBandwidth" );
    Tcl_DStringAppendElement ( &dString, "GetInputNarrowBandwidth" );
    Tcl_DStringAppendElement ( &dString, "SetOutputNarrowBandwidth" );
    Tcl_DStringAppendElement ( &dString, "GetOutputNarrowBandwidth" );
    Tcl_DStringAppendElement ( &dString, "SetNarrowBandwidth" );
    Tcl_DStringAppendElement ( &dString, "SetInputNarrowBand" );
    Tcl_DStringAppendElement ( &dString, "GetOutputNarrowBand" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkmpjReinitializeLevelSetImageFilter *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLevelSetValue */
    if ( strcmp ( argv[2], "SetLevelSetValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLevelSetValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLevelSetValue */
    Tcl_DStringAppendElement ( &dString, " Set/Get Set the value of the level set to be located. Default is 0.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetLevelSetValue(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for SetLevelSetValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLevelSetValue */
    if ( strcmp ( argv[2], "GetLevelSetValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLevelSetValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLevelSetValue */
    Tcl_DStringAppendElement ( &dString, " Set/Get Set the value of the level set to be located. Default is 0.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLevelSetValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for GetLevelSetValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNarrowBanding */
    if ( strcmp ( argv[2], "GetNarrowBanding" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNarrowBanding" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNarrowBanding */
    Tcl_DStringAppendElement ( &dString, " Set/Get the narrowbanding flag. By default, narrowbanding is switched off\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNarrowBanding();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for GetNarrowBanding */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNarrowBanding */
    if ( strcmp ( argv[2], "SetNarrowBanding" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNarrowBanding" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNarrowBanding */
    Tcl_DStringAppendElement ( &dString, " Set/Get the narrowbanding flag. By default, narrowbanding is switched off\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNarrowBanding(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for SetNarrowBanding */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NarrowBandingOn */
    if ( strcmp ( argv[2], "NarrowBandingOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NarrowBandingOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NarrowBandingOn */
    Tcl_DStringAppendElement ( &dString, " Set/Get the narrowbanding flag. By default, narrowbanding is switched off\n" );
    Tcl_DStringAppendElement ( &dString, "void NarrowBandingOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for NarrowBandingOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NarrowBandingOff */
    if ( strcmp ( argv[2], "NarrowBandingOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NarrowBandingOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NarrowBandingOff */
    Tcl_DStringAppendElement ( &dString, " Set/Get the narrowbanding flag. By default, narrowbanding is switched off\n" );
    Tcl_DStringAppendElement ( &dString, "void NarrowBandingOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for NarrowBandingOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInputNarrowBandwidth */
    if ( strcmp ( argv[2], "SetInputNarrowBandwidth" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInputNarrowBandwidth" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInputNarrowBandwidth */
    Tcl_DStringAppendElement ( &dString, " Set/Get the input narrow bandwidth. Default is 12.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInputNarrowBandwidth(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for SetInputNarrowBandwidth */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInputNarrowBandwidth */
    if ( strcmp ( argv[2], "GetInputNarrowBandwidth" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInputNarrowBandwidth" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInputNarrowBandwidth */
    Tcl_DStringAppendElement ( &dString, " Set/Get the input narrow bandwidth. Default is 12.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetInputNarrowBandwidth();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for GetInputNarrowBandwidth */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutputNarrowBandwidth */
    if ( strcmp ( argv[2], "SetOutputNarrowBandwidth" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutputNarrowBandwidth" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutputNarrowBandwidth */
    Tcl_DStringAppendElement ( &dString, " Set/Get the output narrow bandwidth. Default is 12.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetOutputNarrowBandwidth(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for SetOutputNarrowBandwidth */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutputNarrowBandwidth */
    if ( strcmp ( argv[2], "GetOutputNarrowBandwidth" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutputNarrowBandwidth" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutputNarrowBandwidth */
    Tcl_DStringAppendElement ( &dString, " Set/Get the output narrow bandwidth. Default is 12.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetOutputNarrowBandwidth();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for GetOutputNarrowBandwidth */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNarrowBandwidth */
    if ( strcmp ( argv[2], "SetNarrowBandwidth" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNarrowBandwidth" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNarrowBandwidth */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetNarrowBandwidth(double value);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for SetNarrowBandwidth */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInputNarrowBand */
    if ( strcmp ( argv[2], "SetInputNarrowBand" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInputNarrowBand" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDataArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInputNarrowBand */
    Tcl_DStringAppendElement ( &dString, " Set the input narrowband.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInputNarrowBand(vtkDataArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for SetInputNarrowBand */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutputNarrowBand */
    if ( strcmp ( argv[2], "GetOutputNarrowBand" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutputNarrowBand" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutputNarrowBand */
    Tcl_DStringAppendElement ( &dString, " Get the input narrowband.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDataArray *GetOutputNarrowBand();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjReinitializeLevelSetImageFilter" );
    /* Closing for GetOutputNarrowBand */

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
