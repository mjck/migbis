// tcl wrapper for vtkpxLinearRPMRegistration object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxLinearRPMRegistration.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxLinearRPMRegistrationNewCommand()
{
  vtkpxLinearRPMRegistration *temp = vtkpxLinearRPMRegistration::New();
  return static_cast<ClientData>(temp);
}

int vtkpxAbstractRPMRegistrationCppCommand(vtkpxAbstractRPMRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxLinearRPMRegistrationCppCommand(vtkpxLinearRPMRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxLinearRPMRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxLinearRPMRegistrationCppCommand(static_cast<vtkpxLinearRPMRegistration *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxLinearRPMRegistrationCppCommand(vtkpxLinearRPMRegistration *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxLinearRPMRegistration",argv[1]))
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
    vtkpxLinearRPMRegistration  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearRPMRegistration");
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
    vtkpxLinearRPMRegistration  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearRPMRegistration");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxLinearRPMRegistration  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearRPMRegistration");
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
  if ((!strcmp("SetTransformMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetTransformMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTransformModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTransformModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTransformModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTransformModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTransformMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTransformMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformModeToRigid",argv[1]))&&(argc == 2))
    {
    op->SetTransformModeToRigid();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformModeToSimilarity",argv[1]))&&(argc == 2))
    {
    op->SetTransformModeToSimilarity();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetTransformModeToAffine",argv[1]))&&(argc == 2))
    {
    op->SetTransformModeToAffine();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxLinearRPMRegistrationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxAbstractRPMRegistrationCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxLinearRPMRegistration:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCurrentTransformation\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformMode\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformModeToRigid\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformModeToSimilarity\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformModeToAffine\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetTransformMode" );
    Tcl_DStringAppendElement ( &dString, "GetTransformModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetTransformModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetTransformMode" );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToRigid" );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToSimilarity" );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToAffine" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxLinearRPMRegistration *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
    /* Closing for GetCurrentTransformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformMode */
    if ( strcmp ( argv[2], "SetTransformMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformMode */
    Tcl_DStringAppendElement ( &dString, " Set the current mode 0=Rigid, 1=Similarity 2=Affine \n" );
    Tcl_DStringAppendElement ( &dString, "void SetTransformMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
    /* Closing for SetTransformMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransformModeMinValue */
    if ( strcmp ( argv[2], "GetTransformModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransformModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransformModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Set the current mode 0=Rigid, 1=Similarity 2=Affine \n" );
    Tcl_DStringAppendElement ( &dString, "int GetTransformModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
    /* Closing for GetTransformModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransformModeMaxValue */
    if ( strcmp ( argv[2], "GetTransformModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransformModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransformModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set the current mode 0=Rigid, 1=Similarity 2=Affine \n" );
    Tcl_DStringAppendElement ( &dString, "int GetTransformModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
    /* Closing for GetTransformModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransformMode */
    if ( strcmp ( argv[2], "GetTransformMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransformMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransformMode */
    Tcl_DStringAppendElement ( &dString, " Set the current mode 0=Rigid, 1=Similarity 2=Affine \n" );
    Tcl_DStringAppendElement ( &dString, "int GetTransformMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
    /* Closing for GetTransformMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformModeToRigid */
    if ( strcmp ( argv[2], "SetTransformModeToRigid" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToRigid" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformModeToRigid */
    Tcl_DStringAppendElement ( &dString, " Dedicated Set/Get TransformMode stuff \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformModeToRigid();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
    /* Closing for SetTransformModeToRigid */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformModeToSimilarity */
    if ( strcmp ( argv[2], "SetTransformModeToSimilarity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToSimilarity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformModeToSimilarity */
    Tcl_DStringAppendElement ( &dString, " Dedicated Set/Get TransformMode stuff \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformModeToSimilarity();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
    /* Closing for SetTransformModeToSimilarity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformModeToAffine */
    if ( strcmp ( argv[2], "SetTransformModeToAffine" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformModeToAffine" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformModeToAffine */
    Tcl_DStringAppendElement ( &dString, " Dedicated Set/Get TransformMode stuff \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformModeToAffine();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearRPMRegistration" );
    /* Closing for SetTransformModeToAffine */

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
