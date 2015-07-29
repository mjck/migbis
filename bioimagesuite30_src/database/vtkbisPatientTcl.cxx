// tcl wrapper for vtkbisPatient object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkbisPatient.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkbisPatientNewCommand()
{
  vtkbisPatient *temp = vtkbisPatient::New();
  return static_cast<ClientData>(temp);
}

int vtkbisBaseProcedureCppCommand(vtkbisBaseProcedure *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkbisPatientCppCommand(vtkbisPatient *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkbisPatientCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkbisPatientCppCommand(static_cast<vtkbisPatient *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkbisPatientCppCommand(vtkbisPatient *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkbisPatient",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkbisBaseProcedureCppCommand(static_cast<vtkbisBaseProcedure *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkbisBaseProcedure"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkbisPatient  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisPatient");
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
    vtkbisPatient  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisPatient");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkbisPatient  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisPatient");
    return TCL_OK;
    }
    }
  if ((!strcmp("Initialize",argv[1]))&&(argc == 5))
    {
    char    *temp0;
    char    *temp1;
    char    *temp2;
    error = 0;

    temp0 = argv[2];
    temp1 = argv[3];
    temp2 = argv[4];
    if (!error)
    {
    op->Initialize(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("AddPatient",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->AddPatient();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SearchPatientID",argv[1]))&&(argc == 5))
    {
    char    *temp0;
    char    *temp1;
    char    *temp2;
    const char    *temp20;
    error = 0;

    temp0 = argv[2];
    temp1 = argv[3];
    temp2 = argv[4];
    if (!error)
    {
    temp20 = (op)->SearchPatientID(temp0,temp1,temp2);
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
  if ((!strcmp("DebugPrint",argv[1]))&&(argc == 2))
    {
    op->DebugPrint();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetPatientID",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetPatientID(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPatientID",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetPatientID();
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
  if ((!strcmp("SetFirstName",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetFirstName(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFirstName",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetFirstName();
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
  if ((!strcmp("SetLastName",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetLastName(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLastName",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetLastName();
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
  if ((!strcmp("SetBirthDate",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetBirthDate(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBirthDate",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetBirthDate();
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

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkbisPatientCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkbisBaseProcedureCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkbisPatient:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Initialize\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  AddPatient\n",NULL);
    Tcl_AppendResult(interp,"  SearchPatientID\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  DebugPrint\n",NULL);
    Tcl_AppendResult(interp,"  SetPatientID\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPatientID\n",NULL);
    Tcl_AppendResult(interp,"  SetFirstName\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFirstName\n",NULL);
    Tcl_AppendResult(interp,"  SetLastName\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLastName\n",NULL);
    Tcl_AppendResult(interp,"  SetBirthDate\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBirthDate\n",NULL);
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
    vtkbisBaseProcedureCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "Initialize" );
    Tcl_DStringAppendElement ( &dString, "AddPatient" );
    Tcl_DStringAppendElement ( &dString, "SearchPatientID" );
    Tcl_DStringAppendElement ( &dString, "DebugPrint" );
    Tcl_DStringAppendElement ( &dString, "SetPatientID" );
    Tcl_DStringAppendElement ( &dString, "GetPatientID" );
    Tcl_DStringAppendElement ( &dString, "SetFirstName" );
    Tcl_DStringAppendElement ( &dString, "GetFirstName" );
    Tcl_DStringAppendElement ( &dString, "SetLastName" );
    Tcl_DStringAppendElement ( &dString, "GetLastName" );
    Tcl_DStringAppendElement ( &dString, "SetBirthDate" );
    Tcl_DStringAppendElement ( &dString, "GetBirthDate" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkbisBaseProcedureCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkbisPatient *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Initialize */
    if ( strcmp ( argv[2], "Initialize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Initialize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Initialize */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void Initialize(const char *FirstName, const char *LastName, const char *BirthDate);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for Initialize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AddPatient */
    if ( strcmp ( argv[2], "AddPatient" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AddPatient" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddPatient */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int AddPatient();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for AddPatient */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SearchPatientID */
    if ( strcmp ( argv[2], "SearchPatientID" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SearchPatientID" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SearchPatientID */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual const char *SearchPatientID(const char *FirstName, const char *LastName, const char *BirthDate);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for SearchPatientID */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DebugPrint */
    if ( strcmp ( argv[2], "DebugPrint" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DebugPrint" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DebugPrint */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void DebugPrint();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for DebugPrint */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPatientID */
    if ( strcmp ( argv[2], "SetPatientID" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPatientID" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPatientID */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetPatientID(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for SetPatientID */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPatientID */
    if ( strcmp ( argv[2], "GetPatientID" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPatientID" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPatientID */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "char *GetPatientID();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for GetPatientID */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFirstName */
    if ( strcmp ( argv[2], "SetFirstName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFirstName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFirstName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetFirstName(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for SetFirstName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFirstName */
    if ( strcmp ( argv[2], "GetFirstName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFirstName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFirstName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "char *GetFirstName();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for GetFirstName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLastName */
    if ( strcmp ( argv[2], "SetLastName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLastName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLastName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetLastName(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for SetLastName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLastName */
    if ( strcmp ( argv[2], "GetLastName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLastName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLastName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "char *GetLastName();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for GetLastName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBirthDate */
    if ( strcmp ( argv[2], "SetBirthDate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBirthDate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBirthDate */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetBirthDate(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for SetBirthDate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBirthDate */
    if ( strcmp ( argv[2], "GetBirthDate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBirthDate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBirthDate */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "char *GetBirthDate();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisPatient" );
    /* Closing for GetBirthDate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkbisBaseProcedureCppCommand(static_cast<vtkbisBaseProcedure *>(op),interp,argc,argv) == TCL_OK)
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
