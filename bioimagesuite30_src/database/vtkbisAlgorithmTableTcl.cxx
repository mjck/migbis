// tcl wrapper for vtkbisAlgorithmTable object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkbisAlgorithmTable.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkbisAlgorithmTableNewCommand()
{
  vtkbisAlgorithmTable *temp = vtkbisAlgorithmTable::New();
  return static_cast<ClientData>(temp);
}

int vtkbisBaseProcedureCppCommand(vtkbisBaseProcedure *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkbisAlgorithmTableCppCommand(vtkbisAlgorithmTable *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkbisAlgorithmTableCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkbisAlgorithmTableCppCommand(static_cast<vtkbisAlgorithmTable *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkbisAlgorithmTableCppCommand(vtkbisAlgorithmTable *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkbisAlgorithmTable",argv[1]))
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
    vtkbisAlgorithmTable  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisAlgorithmTable");
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
    vtkbisAlgorithmTable  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisAlgorithmTable");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkbisAlgorithmTable  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisAlgorithmTable");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetAlgorithmName",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetAlgorithmName(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAlgorithmName",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetAlgorithmName();
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
  if ((!strcmp("SetPresetName",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetPresetName(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPresetName",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetPresetName();
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
  if ((!strcmp("SetDescription",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetDescription(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDescription",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetDescription();
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
  if ((!strcmp("SetDate",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetDate(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDate",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetDate();
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
  if ((!strcmp("SetParameter",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetParameter(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetParameter",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetParameter();
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
  if ((!strcmp("GetConnector",argv[1]))&&(argc == 2))
    {
    vtkbisDatabase  *temp20;
    temp20 = (op)->GetConnector();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisDatabase");
    return TCL_OK;
    }
  if ((!strcmp("SetConnector",argv[1]))&&(argc == 3))
    {
    vtkbisDatabase  *temp0;
    error = 0;

    temp0 = (vtkbisDatabase *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkbisDatabase"),interp,error));
    if (!error)
    {
    op->SetConnector(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateTable",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->CreateTable();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("AddItem",argv[1]))&&(argc == 7))
    {
    char    *temp0;
    char    *temp1;
    char    *temp2;
    char    *temp3;
    char    *temp4;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    temp1 = argv[3];
    temp2 = argv[4];
    temp3 = argv[5];
    temp4 = argv[6];
    if (!error)
    {
    temp20 = (op)->AddItem(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SearchItem",argv[1]))&&(argc == 8))
    {
    char    *temp0;
    char    *temp1;
    char    *temp2;
    char    *temp3;
    char    *temp4;
    int      temp5;
    const char    *temp20;
    error = 0;

    temp0 = argv[2];
    temp1 = argv[3];
    temp2 = argv[4];
    temp3 = argv[5];
    temp4 = argv[6];
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (!error)
    {
    temp20 = (op)->SearchItem(temp0,temp1,temp2,temp3,temp4,temp5);
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
  if ((!strcmp("SearchItem",argv[1]))&&(argc == 3))
    {
    int      temp0;
    const char    *temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->SearchItem(temp0);
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
  if ((!strcmp("DeleteItem",argv[1]))&&(argc == 4))
    {
    char    *temp0;
    char    *temp1;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    temp1 = argv[3];
    if (!error)
    {
    temp20 = (op)->DeleteItem(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("DeleteAll",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->DeleteAll(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("QueryAll",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    const char    *temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->QueryAll(temp0);
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
  if ((!strcmp("PrintTable",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->PrintTable();
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
  if ((!strcmp("Cleanup",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->Cleanup();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkbisAlgorithmTableCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkbisBaseProcedureCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkbisAlgorithmTable:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetAlgorithmName\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAlgorithmName\n",NULL);
    Tcl_AppendResult(interp,"  SetPresetName\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPresetName\n",NULL);
    Tcl_AppendResult(interp,"  SetDescription\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDescription\n",NULL);
    Tcl_AppendResult(interp,"  SetDate\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDate\n",NULL);
    Tcl_AppendResult(interp,"  SetParameter\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetParameter\n",NULL);
    Tcl_AppendResult(interp,"  GetConnector\n",NULL);
    Tcl_AppendResult(interp,"  SetConnector\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  CreateTable\n",NULL);
    Tcl_AppendResult(interp,"  AddItem\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  SearchItem\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  SearchItem\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  DeleteItem\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  DeleteAll\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  QueryAll\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  PrintTable\n",NULL);
    Tcl_AppendResult(interp,"  Cleanup\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetAlgorithmName" );
    Tcl_DStringAppendElement ( &dString, "GetAlgorithmName" );
    Tcl_DStringAppendElement ( &dString, "SetPresetName" );
    Tcl_DStringAppendElement ( &dString, "GetPresetName" );
    Tcl_DStringAppendElement ( &dString, "SetDescription" );
    Tcl_DStringAppendElement ( &dString, "GetDescription" );
    Tcl_DStringAppendElement ( &dString, "SetDate" );
    Tcl_DStringAppendElement ( &dString, "GetDate" );
    Tcl_DStringAppendElement ( &dString, "SetParameter" );
    Tcl_DStringAppendElement ( &dString, "GetParameter" );
    Tcl_DStringAppendElement ( &dString, "GetConnector" );
    Tcl_DStringAppendElement ( &dString, "SetConnector" );
    Tcl_DStringAppendElement ( &dString, "CreateTable" );
    Tcl_DStringAppendElement ( &dString, "AddItem" );
    Tcl_DStringAppendElement ( &dString, "SearchItem" );
    Tcl_DStringAppendElement ( &dString, "SearchItem" );
    Tcl_DStringAppendElement ( &dString, "DeleteItem" );
    Tcl_DStringAppendElement ( &dString, "DeleteAll" );
    Tcl_DStringAppendElement ( &dString, "QueryAll" );
    Tcl_DStringAppendElement ( &dString, "PrintTable" );
    Tcl_DStringAppendElement ( &dString, "Cleanup" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkbisAlgorithmTable *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAlgorithmName */
    if ( strcmp ( argv[2], "SetAlgorithmName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAlgorithmName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAlgorithmName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetAlgorithmName(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for SetAlgorithmName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAlgorithmName */
    if ( strcmp ( argv[2], "GetAlgorithmName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAlgorithmName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAlgorithmName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "char *GetAlgorithmName();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for GetAlgorithmName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPresetName */
    if ( strcmp ( argv[2], "SetPresetName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPresetName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPresetName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetPresetName(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for SetPresetName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPresetName */
    if ( strcmp ( argv[2], "GetPresetName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPresetName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPresetName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "char *GetPresetName();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for GetPresetName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDescription */
    if ( strcmp ( argv[2], "SetDescription" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDescription" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDescription */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetDescription(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for SetDescription */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDescription */
    if ( strcmp ( argv[2], "GetDescription" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDescription" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDescription */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "char *GetDescription();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for GetDescription */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDate */
    if ( strcmp ( argv[2], "SetDate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDate */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetDate(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for SetDate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDate */
    if ( strcmp ( argv[2], "GetDate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDate */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "char *GetDate();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for GetDate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetParameter */
    if ( strcmp ( argv[2], "SetParameter" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetParameter" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetParameter */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetParameter(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for SetParameter */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetParameter */
    if ( strcmp ( argv[2], "GetParameter" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetParameter" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetParameter */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "char *GetParameter();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for GetParameter */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetConnector */
    if ( strcmp ( argv[2], "GetConnector" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetConnector" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetConnector */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDatabase *GetConnector();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for GetConnector */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetConnector */
    if ( strcmp ( argv[2], "SetConnector" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetConnector" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkbisDatabase" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetConnector */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetConnector(vtkbisDatabase *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for SetConnector */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateTable */
    if ( strcmp ( argv[2], "CreateTable" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateTable" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateTable */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int CreateTable();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for CreateTable */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AddItem */
    if ( strcmp ( argv[2], "AddItem" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AddItem" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddItem */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int AddItem(const char *algorithmName, const char *presetName, const char *description, const char *date, const char *parameter);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for AddItem */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SearchItem */
    if ( strcmp ( argv[2], "SearchItem" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SearchItem" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SearchItem */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual const char *SearchItem(const char *algorithmName, const char *presetName, const char *description, const char *date, const char *parameter, int exact=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for SearchItem */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SearchItem */
    if ( strcmp ( argv[2], "SearchItem" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SearchItem" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SearchItem */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual const char *SearchItem(int exact=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for SearchItem */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DeleteItem */
    if ( strcmp ( argv[2], "DeleteItem" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DeleteItem" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DeleteItem */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int DeleteItem(const char *algorithmName, const char *presetName);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for DeleteItem */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DeleteAll */
    if ( strcmp ( argv[2], "DeleteAll" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DeleteAll" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DeleteAll */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int DeleteAll(const char *AlgorithmName);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for DeleteAll */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: QueryAll */
    if ( strcmp ( argv[2], "QueryAll" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "QueryAll" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for QueryAll */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual const char *QueryAll(const char *AlgorithmName);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for QueryAll */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: PrintTable */
    if ( strcmp ( argv[2], "PrintTable" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "PrintTable" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for PrintTable */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual const char *PrintTable();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for PrintTable */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Cleanup */
    if ( strcmp ( argv[2], "Cleanup" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Cleanup" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Cleanup */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int Cleanup();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisAlgorithmTable" );
    /* Closing for Cleanup */

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
