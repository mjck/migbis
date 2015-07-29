// tcl wrapper for vtkpxBSplinePriorRPMRegistration object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxBSplinePriorRPMRegistration.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxBSplinePriorRPMRegistrationNewCommand()
{
  vtkpxBSplinePriorRPMRegistration *temp = vtkpxBSplinePriorRPMRegistration::New();
  return static_cast<ClientData>(temp);
}

int vtkpxBSplineRPMRegistrationCppCommand(vtkpxBSplineRPMRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxBSplinePriorRPMRegistrationCppCommand(vtkpxBSplinePriorRPMRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxBSplinePriorRPMRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxBSplinePriorRPMRegistrationCppCommand(static_cast<vtkpxBSplinePriorRPMRegistration *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxBSplinePriorRPMRegistrationCppCommand(vtkpxBSplinePriorRPMRegistration *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxBSplinePriorRPMRegistration",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxBSplineRPMRegistrationCppCommand(static_cast<vtkpxBSplineRPMRegistration *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxBSplineRPMRegistration"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxBSplinePriorRPMRegistration  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxBSplinePriorRPMRegistration");
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
    vtkpxBSplinePriorRPMRegistration  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxBSplinePriorRPMRegistration");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxBSplinePriorRPMRegistration  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxBSplinePriorRPMRegistration");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetPriorWeights",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->SetPriorWeights(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPriorWeights",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetPriorWeights();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
    return TCL_OK;
    }
  if ((!strcmp("SetPriorTransform",argv[1]))&&(argc == 3))
    {
    vtkpxComboTransform  *temp0;
    error = 0;

    temp0 = (vtkpxComboTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxComboTransform"),interp,error));
    if (!error)
    {
    op->SetPriorTransform(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPriorTransform",argv[1]))&&(argc == 2))
    {
    vtkpxComboTransform  *temp20;
    temp20 = (op)->GetPriorTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxComboTransform");
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxBSplinePriorRPMRegistrationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxBSplineRPMRegistrationCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxBSplinePriorRPMRegistration:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetPriorWeights\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPriorWeights\n",NULL);
    Tcl_AppendResult(interp,"  SetPriorTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPriorTransform\n",NULL);
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
    vtkpxBSplineRPMRegistrationCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetPriorWeights" );
    Tcl_DStringAppendElement ( &dString, "GetPriorWeights" );
    Tcl_DStringAppendElement ( &dString, "SetPriorTransform" );
    Tcl_DStringAppendElement ( &dString, "GetPriorTransform" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxBSplineRPMRegistrationCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxBSplinePriorRPMRegistration *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPriorWeights */
    if ( strcmp ( argv[2], "SetPriorWeights" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPriorWeights" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPriorWeights */
    Tcl_DStringAppendElement ( &dString, " Get/Set Prior Weights\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPriorWeights(vtkDoubleArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration" );
    /* Closing for SetPriorWeights */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPriorWeights */
    if ( strcmp ( argv[2], "GetPriorWeights" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPriorWeights" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPriorWeights */
    Tcl_DStringAppendElement ( &dString, " Get/Set Prior Weights\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetPriorWeights();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration" );
    /* Closing for GetPriorWeights */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPriorTransform */
    if ( strcmp ( argv[2], "SetPriorTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPriorTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPriorTransform */
    Tcl_DStringAppendElement ( &dString, " Get/Set Prior Transform -- needs to be a combo transform\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPriorTransform(vtkpxComboTransform *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration" );
    /* Closing for SetPriorTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPriorTransform */
    if ( strcmp ( argv[2], "GetPriorTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPriorTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPriorTransform */
    Tcl_DStringAppendElement ( &dString, " Get/Set Prior Transform -- needs to be a combo transform\n" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform *GetPriorTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxBSplinePriorRPMRegistration" );
    /* Closing for GetPriorTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxBSplineRPMRegistrationCppCommand(static_cast<vtkpxBSplineRPMRegistration *>(op),interp,argc,argv) == TCL_OK)
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
