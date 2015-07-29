// tcl wrapper for vtkpxBSplineRPMRegistration object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxBSplineRPMRegistration.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxBSplineRPMRegistrationNewCommand()
{
  vtkpxBSplineRPMRegistration *temp = vtkpxBSplineRPMRegistration::New();
  return static_cast<ClientData>(temp);
}

int vtkpxAbstractRPMRegistrationCppCommand(vtkpxAbstractRPMRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxBSplineRPMRegistrationCppCommand(vtkpxBSplineRPMRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxBSplineRPMRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxBSplineRPMRegistrationCppCommand(static_cast<vtkpxBSplineRPMRegistration *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxBSplineRPMRegistrationCppCommand(vtkpxBSplineRPMRegistration *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxBSplineRPMRegistration",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxAbstractRPMRegistrationCppCommand(static_cast<vtkpxAbstractRPMRegistration *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxAbstractRPMRegistration"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxBSplineRPMRegistration  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxBSplineRPMRegistration");
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
    vtkpxBSplineRPMRegistration  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxBSplineRPMRegistration");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxBSplineRPMRegistration  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxBSplineRPMRegistration");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCurrentTransformation",argv[1]))&&(argc == 2))
    {
    vtkAbstractTransform  *temp20;
    temp20 = (op)->GetCurrentTransformation();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
    return TCL_OK;
    }
  if ((!strcmp("SetInitialControlPointSpacing",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetInitialControlPointSpacing(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInitialControlPointSpacingMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialControlPointSpacingMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInitialControlPointSpacingMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialControlPointSpacingMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInitialControlPointSpacing",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialControlPointSpacing();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetFinalControlPointSpacing",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetFinalControlPointSpacing(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFinalControlPointSpacingMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetFinalControlPointSpacingMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFinalControlPointSpacingMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetFinalControlPointSpacingMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFinalControlPointSpacing",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetFinalControlPointSpacing();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCurrentControlPointSpacing",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetCurrentControlPointSpacing();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFinalWeights",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetFinalWeights();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxBSplineRPMRegistrationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxAbstractRPMRegistrationCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxBSplineRPMRegistration:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCurrentTransformation\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialControlPointSpacing\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialControlPointSpacingMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialControlPointSpacingMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialControlPointSpacing\n",NULL);
    Tcl_AppendResult(interp,"  SetFinalControlPointSpacing\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFinalControlPointSpacingMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFinalControlPointSpacingMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFinalControlPointSpacing\n",NULL);
    Tcl_AppendResult(interp,"  GetCurrentControlPointSpacing\n",NULL);
    Tcl_AppendResult(interp,"  GetFinalWeights\n",NULL);
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
    vtkpxAbstractRPMRegistrationCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "GetCurrentTransformation" );
    Tcl_DStringAppendElement ( &dString, "SetInitialControlPointSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetInitialControlPointSpacingMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetInitialControlPointSpacingMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetInitialControlPointSpacing" );
    Tcl_DStringAppendElement ( &dString, "SetFinalControlPointSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetFinalControlPointSpacingMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetFinalControlPointSpacingMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetFinalControlPointSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetCurrentControlPointSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetFinalWeights" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxAbstractRPMRegistrationCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxBSplineRPMRegistration *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCurrentTransformation */
    if ( strcmp ( argv[2], "GetCurrentTransformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCurrentTransformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCurrentTransformation */
    Tcl_DStringAppendElement ( &dString, " Get Current Transformation -- needed for displaying updates \n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkAbstractTransform *GetCurrentTransformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for GetCurrentTransformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialControlPointSpacing */
    if ( strcmp ( argv[2], "SetInitialControlPointSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialControlPointSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialControlPointSpacing */
    Tcl_DStringAppendElement ( &dString, " Initial ControlPointSpacing for deterministic annealing and bspline grid\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialControlPointSpacing(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for SetInitialControlPointSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialControlPointSpacingMinValue */
    if ( strcmp ( argv[2], "GetInitialControlPointSpacingMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialControlPointSpacingMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialControlPointSpacingMinValue */
    Tcl_DStringAppendElement ( &dString, " Initial ControlPointSpacing for deterministic annealing and bspline grid\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialControlPointSpacingMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for GetInitialControlPointSpacingMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialControlPointSpacingMaxValue */
    if ( strcmp ( argv[2], "GetInitialControlPointSpacingMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialControlPointSpacingMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialControlPointSpacingMaxValue */
    Tcl_DStringAppendElement ( &dString, " Initial ControlPointSpacing for deterministic annealing and bspline grid\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialControlPointSpacingMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for GetInitialControlPointSpacingMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialControlPointSpacing */
    if ( strcmp ( argv[2], "GetInitialControlPointSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialControlPointSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialControlPointSpacing */
    Tcl_DStringAppendElement ( &dString, " Initial ControlPointSpacing for deterministic annealing and bspline grid\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialControlPointSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for GetInitialControlPointSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFinalControlPointSpacing */
    if ( strcmp ( argv[2], "SetFinalControlPointSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFinalControlPointSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFinalControlPointSpacing */
    Tcl_DStringAppendElement ( &dString, " Final ControlPointSpacing for deterministic annealing and bspline grid\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFinalControlPointSpacing(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for SetFinalControlPointSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFinalControlPointSpacingMinValue */
    if ( strcmp ( argv[2], "GetFinalControlPointSpacingMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFinalControlPointSpacingMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFinalControlPointSpacingMinValue */
    Tcl_DStringAppendElement ( &dString, " Final ControlPointSpacing for deterministic annealing and bspline grid\n" );
    Tcl_DStringAppendElement ( &dString, "float GetFinalControlPointSpacingMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for GetFinalControlPointSpacingMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFinalControlPointSpacingMaxValue */
    if ( strcmp ( argv[2], "GetFinalControlPointSpacingMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFinalControlPointSpacingMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFinalControlPointSpacingMaxValue */
    Tcl_DStringAppendElement ( &dString, " Final ControlPointSpacing for deterministic annealing and bspline grid\n" );
    Tcl_DStringAppendElement ( &dString, "float GetFinalControlPointSpacingMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for GetFinalControlPointSpacingMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFinalControlPointSpacing */
    if ( strcmp ( argv[2], "GetFinalControlPointSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFinalControlPointSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFinalControlPointSpacing */
    Tcl_DStringAppendElement ( &dString, " Final ControlPointSpacing for deterministic annealing and bspline grid\n" );
    Tcl_DStringAppendElement ( &dString, "float GetFinalControlPointSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for GetFinalControlPointSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCurrentControlPointSpacing */
    if ( strcmp ( argv[2], "GetCurrentControlPointSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCurrentControlPointSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCurrentControlPointSpacing */
    Tcl_DStringAppendElement ( &dString, " Current Control Point Spacing\n" );
    Tcl_DStringAppendElement ( &dString, "float GetCurrentControlPointSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for GetCurrentControlPointSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFinalWeights */
    if ( strcmp ( argv[2], "GetFinalWeights" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFinalWeights" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFinalWeights */
    Tcl_DStringAppendElement ( &dString, " Get/Set Prior Weights\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetFinalWeights();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplineRPMRegistration" );
    /* Closing for GetFinalWeights */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxAbstractRPMRegistrationCppCommand(static_cast<vtkpxAbstractRPMRegistration *>(op),interp,argc,argv) == TCL_OK)
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
