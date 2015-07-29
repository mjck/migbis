// tcl wrapper for vtkpxSplineSourceUndoStack object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxSplineSourceUndoStack.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxSplineSourceUndoStackNewCommand()
{
  vtkpxSplineSourceUndoStack *temp = vtkpxSplineSourceUndoStack::New();
  return static_cast<ClientData>(temp);
}

int vtkpxGenericUndoStackCppCommand(vtkpxGenericUndoStack *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxSplineSourceUndoStackCppCommand(vtkpxSplineSourceUndoStack *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxSplineSourceUndoStackCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxSplineSourceUndoStackCppCommand(static_cast<vtkpxSplineSourceUndoStack *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxSplineSourceUndoStackCppCommand(vtkpxSplineSourceUndoStack *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxSplineSourceUndoStack",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxGenericUndoStackCppCommand(static_cast<vtkpxGenericUndoStack *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxGenericUndoStack"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxSplineSourceUndoStack  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineSourceUndoStack");
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
    vtkpxSplineSourceUndoStack  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineSourceUndoStack");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxSplineSourceUndoStack  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineSourceUndoStack");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNextSpline",argv[1]))&&(argc == 2))
    {
    vtkpxSplineSource  *temp20;
    temp20 = (op)->GetNextSpline();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineSource");
    return TCL_OK;
    }
  if ((!strcmp("GetCurrentSpline",argv[1]))&&(argc == 2))
    {
    vtkpxSplineSource  *temp20;
    temp20 = (op)->GetCurrentSpline();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineSource");
    return TCL_OK;
    }
  if ((!strcmp("GetNextUndoSpline",argv[1]))&&(argc == 2))
    {
    vtkpxSplineSource  *temp20;
    temp20 = (op)->GetNextUndoSpline();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineSource");
    return TCL_OK;
    }
  if ((!strcmp("GetNextRedoSpline",argv[1]))&&(argc == 2))
    {
    vtkpxSplineSource  *temp20;
    temp20 = (op)->GetNextRedoSpline();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineSource");
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxSplineSourceUndoStackCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxGenericUndoStackCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxSplineSourceUndoStack:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNextSpline\n",NULL);
    Tcl_AppendResult(interp,"  GetCurrentSpline\n",NULL);
    Tcl_AppendResult(interp,"  GetNextUndoSpline\n",NULL);
    Tcl_AppendResult(interp,"  GetNextRedoSpline\n",NULL);
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
    vtkpxGenericUndoStackCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "GetNextSpline" );
    Tcl_DStringAppendElement ( &dString, "GetCurrentSpline" );
    Tcl_DStringAppendElement ( &dString, "GetNextUndoSpline" );
    Tcl_DStringAppendElement ( &dString, "GetNextRedoSpline" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxGenericUndoStackCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxSplineSourceUndoStack *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNextSpline */
    if ( strcmp ( argv[2], "GetNextSpline" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNextSpline" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNextSpline */
    Tcl_DStringAppendElement ( &dString, " Insert Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkpxSplineSource *GetNextSpline();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack" );
    /* Closing for GetNextSpline */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCurrentSpline */
    if ( strcmp ( argv[2], "GetCurrentSpline" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCurrentSpline" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCurrentSpline */
    Tcl_DStringAppendElement ( &dString, " Insert Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkpxSplineSource *GetCurrentSpline();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack" );
    /* Closing for GetCurrentSpline */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNextUndoSpline */
    if ( strcmp ( argv[2], "GetNextUndoSpline" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNextUndoSpline" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNextUndoSpline */
    Tcl_DStringAppendElement ( &dString, " Insert Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkpxSplineSource *GetNextUndoSpline();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack" );
    /* Closing for GetNextUndoSpline */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNextRedoSpline */
    if ( strcmp ( argv[2], "GetNextRedoSpline" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNextRedoSpline" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNextRedoSpline */
    Tcl_DStringAppendElement ( &dString, " Insert Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkpxSplineSource *GetNextRedoSpline();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSourceUndoStack" );
    /* Closing for GetNextRedoSpline */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxGenericUndoStackCppCommand(static_cast<vtkpxGenericUndoStack *>(op),interp,argc,argv) == TCL_OK)
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
