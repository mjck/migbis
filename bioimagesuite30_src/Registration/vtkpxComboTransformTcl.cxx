// tcl wrapper for vtkpxComboTransform object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxComboTransform.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxComboTransformNewCommand()
{
  vtkpxComboTransform *temp = vtkpxComboTransform::New();
  return static_cast<ClientData>(temp);
}

int vtkGeneralTransformCppCommand(vtkGeneralTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxComboTransformCppCommand(vtkpxComboTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxComboTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxComboTransformCppCommand(static_cast<vtkpxComboTransform *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxComboTransformCppCommand(vtkpxComboTransform *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxComboTransform",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkGeneralTransformCppCommand(static_cast<vtkGeneralTransform *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkGeneralTransform"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxComboTransform  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxComboTransform");
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
    vtkpxComboTransform  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxComboTransform");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxComboTransform  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxComboTransform");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLinearTransform",argv[1]))&&(argc == 2))
    {
    vtkTransform  *temp20;
    temp20 = (op)->GetLinearTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkTransform");
    return TCL_OK;
    }
  if ((!strcmp("ChangeLinearTransform",argv[1]))&&(argc == 3))
    {
    vtkTransform  *temp0;
    error = 0;

    temp0 = (vtkTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkTransform"),interp,error));
    if (!error)
    {
    op->ChangeLinearTransform(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfGridTransformations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfGridTransformations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetGridTransform",argv[1]))&&(argc == 2))
    {
    vtkpxGridTransform  *temp20;
    temp20 = (op)->GetGridTransform();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxGridTransform");
    return TCL_OK;
    }
  if ((!strcmp("GetGridTransformByIndex",argv[1]))&&(argc == 3))
    {
    int      temp0;
    vtkpxGridTransform  *temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->GetGridTransformByIndex(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxGridTransform");
    return TCL_OK;
    }
    }
  if ((!strcmp("ChangeGridTransform",argv[1]))&&(argc == 3))
    {
    vtkpxGridTransform  *temp0;
    error = 0;

    temp0 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxGridTransform"),interp,error));
    if (!error)
    {
    op->ChangeGridTransform(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ChangeGridTransformByIndex",argv[1]))&&(argc == 4))
    {
    vtkpxGridTransform  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxGridTransform"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->ChangeGridTransformByIndex(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetGridTransform",argv[1]))&&(argc == 3))
    {
    vtkpxGridTransform  *temp0;
    error = 0;

    temp0 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxGridTransform"),interp,error));
    if (!error)
    {
    op->SetGridTransform(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("AddGridTransform",argv[1]))&&(argc == 3))
    {
    vtkpxGridTransform  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxGridTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->AddGridTransform(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetTransformations",argv[1]))&&(argc == 4))
    {
    vtkTransform  *temp0;
    vtkpxGridTransform  *temp1;
    error = 0;

    temp0 = (vtkTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkTransform"),interp,error));
    temp1 = (vtkpxGridTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxGridTransform"),interp,error));
    if (!error)
    {
    op->SetTransformations(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ClearGridTransformationsCollection",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->ClearGridTransformationsCollection();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("CollapseTransormationsCollection",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->CollapseTransormationsCollection();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Load",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->Load(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Save",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->Save(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Save",argv[1]))&&(argc == 4))
    {
    char    *temp0;
    int      temp1;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->Save(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Copy",argv[1]))&&(argc == 3))
    {
    vtkpxComboTransform  *temp0;
    error = 0;

    temp0 = (vtkpxComboTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxComboTransform"),interp,error));
    if (!error)
    {
    op->Copy(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNonLinearFirst",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNonLinearFirst();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNonLinearFirst",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNonLinearFirst(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Inverse",argv[1]))&&(argc == 2))
    {
    op->Inverse();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxComboTransformCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkGeneralTransformCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxComboTransform:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLinearTransform\n",NULL);
    Tcl_AppendResult(interp,"  ChangeLinearTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfGridTransformations\n",NULL);
    Tcl_AppendResult(interp,"  GetGridTransform\n",NULL);
    Tcl_AppendResult(interp,"  GetGridTransformByIndex\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ChangeGridTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ChangeGridTransformByIndex\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetGridTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  AddGridTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformations\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ClearGridTransformationsCollection\n",NULL);
    Tcl_AppendResult(interp,"  CollapseTransormationsCollection\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Copy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNonLinearFirst\n",NULL);
    Tcl_AppendResult(interp,"  SetNonLinearFirst\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Inverse\n",NULL);
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
    vtkGeneralTransformCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "GetLinearTransform" );
    Tcl_DStringAppendElement ( &dString, "ChangeLinearTransform" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfGridTransformations" );
    Tcl_DStringAppendElement ( &dString, "GetGridTransform" );
    Tcl_DStringAppendElement ( &dString, "GetGridTransformByIndex" );
    Tcl_DStringAppendElement ( &dString, "ChangeGridTransform" );
    Tcl_DStringAppendElement ( &dString, "ChangeGridTransformByIndex" );
    Tcl_DStringAppendElement ( &dString, "SetGridTransform" );
    Tcl_DStringAppendElement ( &dString, "AddGridTransform" );
    Tcl_DStringAppendElement ( &dString, "SetTransformations" );
    Tcl_DStringAppendElement ( &dString, "ClearGridTransformationsCollection" );
    Tcl_DStringAppendElement ( &dString, "CollapseTransormationsCollection" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    Tcl_DStringAppendElement ( &dString, "GetNonLinearFirst" );
    Tcl_DStringAppendElement ( &dString, "SetNonLinearFirst" );
    Tcl_DStringAppendElement ( &dString, "Inverse" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkGeneralTransformCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxComboTransform *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLinearTransform */
    if ( strcmp ( argv[2], "GetLinearTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLinearTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLinearTransform */
    Tcl_DStringAppendElement ( &dString, " Manipulate the Linear Transformation \n" );
    Tcl_DStringAppendElement ( &dString, "vtkTransform *GetLinearTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for GetLinearTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ChangeLinearTransform */
    if ( strcmp ( argv[2], "ChangeLinearTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ChangeLinearTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ChangeLinearTransform */
    Tcl_DStringAppendElement ( &dString, " Manipulate the Linear Transformation \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void ChangeLinearTransform(vtkTransform *LinearTransform);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for ChangeLinearTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfGridTransformations */
    if ( strcmp ( argv[2], "GetNumberOfGridTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfGridTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfGridTransformations */
    Tcl_DStringAppendElement ( &dString, " Change The Grid Transformations \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumberOfGridTransformations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for GetNumberOfGridTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGridTransform */
    if ( strcmp ( argv[2], "GetGridTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGridTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGridTransform */
    Tcl_DStringAppendElement ( &dString, " Change The Grid Transformations \n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkpxGridTransform *GetGridTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for GetGridTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGridTransformByIndex */
    if ( strcmp ( argv[2], "GetGridTransformByIndex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGridTransformByIndex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGridTransformByIndex */
    Tcl_DStringAppendElement ( &dString, " Change The Grid Transformations \n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkpxGridTransform *GetGridTransformByIndex(int index);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for GetGridTransformByIndex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ChangeGridTransform */
    if ( strcmp ( argv[2], "ChangeGridTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ChangeGridTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ChangeGridTransform */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void ChangeGridTransform(vtkpxGridTransform *GridTransform);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for ChangeGridTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ChangeGridTransformByIndex */
    if ( strcmp ( argv[2], "ChangeGridTransformByIndex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ChangeGridTransformByIndex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ChangeGridTransformByIndex */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void ChangeGridTransformByIndex(vtkpxGridTransform *GridTransform, int index);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for ChangeGridTransformByIndex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetGridTransform */
    if ( strcmp ( argv[2], "SetGridTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetGridTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetGridTransform */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetGridTransform(vtkpxGridTransform *GridTransform);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for SetGridTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AddGridTransform */
    if ( strcmp ( argv[2], "AddGridTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AddGridTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddGridTransform */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int AddGridTransform(vtkpxGridTransform *GridTransform);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for AddGridTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformations */
    if ( strcmp ( argv[2], "SetTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkpxGridTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformations */
    Tcl_DStringAppendElement ( &dString, " Change The Internal Transformations -- if more than one grid manipulate the last Grid Transform\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransformations(vtkTransform *LinearTransform, vtkpxGridTransform *GridTransform);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for SetTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ClearGridTransformationsCollection */
    if ( strcmp ( argv[2], "ClearGridTransformationsCollection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ClearGridTransformationsCollection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ClearGridTransformationsCollection */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int ClearGridTransformationsCollection();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for ClearGridTransformationsCollection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CollapseTransormationsCollection */
    if ( strcmp ( argv[2], "CollapseTransormationsCollection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CollapseTransormationsCollection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CollapseTransormationsCollection */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int CollapseTransormationsCollection();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for CollapseTransormationsCollection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Load */
    if ( strcmp ( argv[2], "Load" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Load" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Load */
    Tcl_DStringAppendElement ( &dString, " Load and Save Functionality \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Load(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for Load */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Save */
    if ( strcmp ( argv[2], "Save" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Save" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Save */
    Tcl_DStringAppendElement ( &dString, " Load and Save Functionality \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Save(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for Save */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Save */
    if ( strcmp ( argv[2], "Save" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Save" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Save */
    Tcl_DStringAppendElement ( &dString, " Load and Save Functionality \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Save(const char *fname, int compression);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for Save */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Copy */
    if ( strcmp ( argv[2], "Copy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Copy */
    Tcl_DStringAppendElement ( &dString, " Virtual Do Copy (Shallow)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Copy(vtkpxComboTransform *transform);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for Copy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNonLinearFirst */
    if ( strcmp ( argv[2], "GetNonLinearFirst" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNonLinearFirst" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNonLinearFirst */
    Tcl_DStringAppendElement ( &dString, " NonLinear First\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNonLinearFirst();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for GetNonLinearFirst */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNonLinearFirst */
    if ( strcmp ( argv[2], "SetNonLinearFirst" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNonLinearFirst" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNonLinearFirst */
    Tcl_DStringAppendElement ( &dString, " NonLinear First\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetNonLinearFirst(int s);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for SetNonLinearFirst */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Inverse */
    if ( strcmp ( argv[2], "Inverse" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Inverse" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Inverse */
    Tcl_DStringAppendElement ( &dString, " Invert Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "void Inverse();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    /* Closing for Inverse */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkGeneralTransformCppCommand(static_cast<vtkGeneralTransform *>(op),interp,argc,argv) == TCL_OK)
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
