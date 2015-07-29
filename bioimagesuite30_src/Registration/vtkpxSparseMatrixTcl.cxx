// tcl wrapper for vtkpxSparseMatrix object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxSparseMatrix.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxSparseMatrixNewCommand()
{
  vtkpxSparseMatrix *temp = vtkpxSparseMatrix::New();
  return static_cast<ClientData>(temp);
}

int vtkpxAbstractMatrixCppCommand(vtkpxAbstractMatrix *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxSparseMatrixCppCommand(vtkpxSparseMatrix *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxSparseMatrixCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxSparseMatrixCppCommand(static_cast<vtkpxSparseMatrix *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxSparseMatrixCppCommand(vtkpxSparseMatrix *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxSparseMatrix",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxAbstractMatrixCppCommand(static_cast<vtkpxAbstractMatrix *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxAbstractMatrix"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxSparseMatrix  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSparseMatrix");
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
    vtkpxSparseMatrix  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSparseMatrix");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxSparseMatrix  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSparseMatrix");
    return TCL_OK;
    }
    }
  if ((!strcmp("Density",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->Density();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Zeros",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->Zeros(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Allocate",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->Allocate(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Ones",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->Ones(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Eye",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->Eye(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Identity",argv[1]))&&(argc == 2))
    {
    op->Identity();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("Zero",argv[1]))&&(argc == 2))
    {
    op->Zero();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("Fill",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->Fill(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("InternalPrint",argv[1]))&&(argc == 2))
    {
    op->InternalPrint();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("PrintBW",argv[1]))&&(argc == 2))
    {
    op->PrintBW();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("Print",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->Print(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetElement",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->GetElement(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetElement",argv[1]))&&(argc == 5))
    {
    int      temp0;
    int      temp1;
    float    temp2;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->SetElement(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("AddToElement",argv[1]))&&(argc == 5))
    {
    int      temp0;
    int      temp1;
    float    temp2;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->AddToElement(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Scale",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->Scale(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Add",argv[1]))&&(argc == 5))
    {
    vtkpxSparseMatrix  *temp0;
    vtkpxSparseMatrix  *temp1;
    vtkpxSparseMatrix  *temp2;
    error = 0;

    temp0 = (vtkpxSparseMatrix *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSparseMatrix"),interp,error));
    temp1 = (vtkpxSparseMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxSparseMatrix"),interp,error));
    temp2 = (vtkpxSparseMatrix *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkpxSparseMatrix"),interp,error));
    if (!error)
    {
    op->Add(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Add",argv[1]))&&(argc == 7))
    {
    float    temp0;
    vtkpxSparseMatrix  *temp1;
    float    temp2;
    vtkpxSparseMatrix  *temp3;
    vtkpxSparseMatrix  *temp4;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    temp1 = (vtkpxSparseMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxSparseMatrix"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    temp3 = (vtkpxSparseMatrix *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkpxSparseMatrix"),interp,error));
    temp4 = (vtkpxSparseMatrix *)(vtkTclGetPointerFromObject(argv[6],const_cast<char *>("vtkpxSparseMatrix"),interp,error));
    if (!error)
    {
    op->Add(temp0,temp1,temp2,temp3,temp4);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Multiply",argv[1]))&&(argc == 5))
    {
    vtkpxSparseMatrix  *temp0;
    vtkpxMatrix  *temp1;
    vtkpxMatrix  *temp2;
    error = 0;

    temp0 = (vtkpxSparseMatrix *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSparseMatrix"),interp,error));
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    temp2 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    op->Multiply(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("MultiplyThisWith",argv[1]))&&(argc == 4))
    {
    vtkpxMatrix  *temp0;
    vtkpxMatrix  *temp1;
    error = 0;

    temp0 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxMatrix"),interp,error));
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    op->MultiplyThisWith(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("MapPoints",argv[1]))&&(argc == 4))
    {
    vtkpxMatrix  *temp0;
    vtkpxMatrix  *temp1;
    error = 0;

    temp0 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxMatrix"),interp,error));
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    op->MapPoints(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("NormalizeMixtureMatrix",argv[1]))&&(argc == 3))
    {
    vtkpxMatrix  *temp0;
    error = 0;

    temp0 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    op->NormalizeMixtureMatrix(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("NormalizeRPMMatrix",argv[1]))&&(argc == 4))
    {
    vtkpxMatrix  *temp0;
    vtkpxMatrix  *temp1;
    error = 0;

    temp0 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxMatrix"),interp,error));
    temp1 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    op->NormalizeRPMMatrix(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxSparseMatrixCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxAbstractMatrixCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxSparseMatrix:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Density\n",NULL);
    Tcl_AppendResult(interp,"  Zeros\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Allocate\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Ones\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Eye\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Identity\n",NULL);
    Tcl_AppendResult(interp,"  Zero\n",NULL);
    Tcl_AppendResult(interp,"  Fill\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  InternalPrint\n",NULL);
    Tcl_AppendResult(interp,"  PrintBW\n",NULL);
    Tcl_AppendResult(interp,"  Print\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetElement\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetElement\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  AddToElement\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  Scale\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Add\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  Add\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  Multiply\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  MultiplyThisWith\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  MapPoints\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  NormalizeMixtureMatrix\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NormalizeRPMMatrix\t with 2 args\n",NULL);
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
    vtkpxAbstractMatrixCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "Density" );
    Tcl_DStringAppendElement ( &dString, "Zeros" );
    Tcl_DStringAppendElement ( &dString, "Allocate" );
    Tcl_DStringAppendElement ( &dString, "Ones" );
    Tcl_DStringAppendElement ( &dString, "Eye" );
    Tcl_DStringAppendElement ( &dString, "Identity" );
    Tcl_DStringAppendElement ( &dString, "Zero" );
    Tcl_DStringAppendElement ( &dString, "Fill" );
    Tcl_DStringAppendElement ( &dString, "InternalPrint" );
    Tcl_DStringAppendElement ( &dString, "PrintBW" );
    Tcl_DStringAppendElement ( &dString, "Print" );
    Tcl_DStringAppendElement ( &dString, "GetElement" );
    Tcl_DStringAppendElement ( &dString, "SetElement" );
    Tcl_DStringAppendElement ( &dString, "AddToElement" );
    Tcl_DStringAppendElement ( &dString, "Scale" );
    Tcl_DStringAppendElement ( &dString, "Add" );
    Tcl_DStringAppendElement ( &dString, "Add" );
    Tcl_DStringAppendElement ( &dString, "Multiply" );
    Tcl_DStringAppendElement ( &dString, "MultiplyThisWith" );
    Tcl_DStringAppendElement ( &dString, "MapPoints" );
    Tcl_DStringAppendElement ( &dString, "NormalizeMixtureMatrix" );
    Tcl_DStringAppendElement ( &dString, "NormalizeRPMMatrix" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxAbstractMatrixCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxSparseMatrix *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Density */
    if ( strcmp ( argv[2], "Density" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Density" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Density */
    Tcl_DStringAppendElement ( &dString, " Matrix Density\n" );
    Tcl_DStringAppendElement ( &dString, "virtual float Density();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Density */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Zeros */
    if ( strcmp ( argv[2], "Zeros" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Zeros" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Zeros */
    Tcl_DStringAppendElement ( &dString, " Routine to allocate memory and set equal to zero (alias for allocate ala matlab)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Zeros(int rows, int cols);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Zeros */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Allocate */
    if ( strcmp ( argv[2], "Allocate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Allocate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Allocate */
    Tcl_DStringAppendElement ( &dString, " Routine to allocate memory \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Allocate(int rows, int columns);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Allocate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Ones */
    if ( strcmp ( argv[2], "Ones" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Ones" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Ones */
    Tcl_DStringAppendElement ( &dString, " Routines to allocate memory and set equal to one\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Ones(int rows, int cols);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Ones */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Eye */
    if ( strcmp ( argv[2], "Eye" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Eye" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Eye */
    Tcl_DStringAppendElement ( &dString, " Routines to allocate memory for square matrix and initialize accordingly and set to identity\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Eye(int rowcol);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Eye */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Identity */
    if ( strcmp ( argv[2], "Identity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Identity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Identity */
    Tcl_DStringAppendElement ( &dString, " Sets current matrix=identity matrix (if it is square)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Identity();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Identity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Zero */
    if ( strcmp ( argv[2], "Zero" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Zero" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Zero */
    Tcl_DStringAppendElement ( &dString, " Set all elements = zero\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Zero();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Zero */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Fill */
    if ( strcmp ( argv[2], "Fill" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Fill" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Fill */
    Tcl_DStringAppendElement ( &dString, " Set all elements = a\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Fill(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Fill */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InternalPrint */
    if ( strcmp ( argv[2], "InternalPrint" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InternalPrint" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InternalPrint */
    Tcl_DStringAppendElement ( &dString, " Print Matrix to stderr\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void InternalPrint();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for InternalPrint */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: PrintBW */
    if ( strcmp ( argv[2], "PrintBW" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "PrintBW" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for PrintBW */
    Tcl_DStringAppendElement ( &dString, " Print Matrix to stderr\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void PrintBW();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for PrintBW */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Print */
    if ( strcmp ( argv[2], "Print" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Print" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Print */
    Tcl_DStringAppendElement ( &dString, " Print Matrix to stderr\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Print(const char *message);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Print */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetElement */
    if ( strcmp ( argv[2], "GetElement" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetElement" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetElement */
    Tcl_DStringAppendElement ( &dString, " Get/Set Element in row=i column=j with range checking SAFE\n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetElement(int i, int j);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for GetElement */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetElement */
    if ( strcmp ( argv[2], "SetElement" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetElement" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetElement */
    Tcl_DStringAppendElement ( &dString, " Get/Set Element in row=i column=j with range checking SAFE\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetElement(int i, int j, float v);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for SetElement */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AddToElement */
    if ( strcmp ( argv[2], "AddToElement" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AddToElement" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddToElement */
    Tcl_DStringAppendElement ( &dString, " Get/Set Element in row=i column=j with range checking SAFE\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void AddToElement(int i, int j, float v);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for AddToElement */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Scale */
    if ( strcmp ( argv[2], "Scale" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Scale" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Scale */
    Tcl_DStringAppendElement ( &dString, " Multiply All elements in matrix by s\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Scale(float s);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Scale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Add */
    if ( strcmp ( argv[2], "Add" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Add" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Add */
    Tcl_DStringAppendElement ( &dString, " Static methods: a+b=results, a*b=results \n" );
    Tcl_DStringAppendElement ( &dString, "static void Add(vtkpxSparseMatrix *a, vtkpxSparseMatrix *b, vtkpxSparseMatrix *result);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Add */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Add */
    if ( strcmp ( argv[2], "Add" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Add" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Add */
    Tcl_DStringAppendElement ( &dString, " Static methods: a+b=results, a*b=results \n" );
    Tcl_DStringAppendElement ( &dString, "static void Add(float wa, vtkpxSparseMatrix *a, float wb, vtkpxSparseMatrix *b, vtkpxSparseMatrix *result);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Add */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Multiply */
    if ( strcmp ( argv[2], "Multiply" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Multiply" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Multiply */
    Tcl_DStringAppendElement ( &dString, " Static methods: a+b=results, a*b=results \n" );
    Tcl_DStringAppendElement ( &dString, "static void Multiply(vtkpxSparseMatrix *a, vtkpxMatrix *b, vtkpxMatrix *result);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for Multiply */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MultiplyThisWith */
    if ( strcmp ( argv[2], "MultiplyThisWith" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MultiplyThisWith" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MultiplyThisWith */
    Tcl_DStringAppendElement ( &dString, " this*b=c\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void MultiplyThisWith(vtkpxMatrix *b, vtkpxMatrix *c);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for MultiplyThisWith */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MapPoints */
    if ( strcmp ( argv[2], "MapPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MapPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MapPoints */
    Tcl_DStringAppendElement ( &dString, " Stuff Needed for RPM\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void MapPoints(vtkpxMatrix *Y, vtkpxMatrix *VY);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for MapPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NormalizeMixtureMatrix */
    if ( strcmp ( argv[2], "NormalizeMixtureMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NormalizeMixtureMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NormalizeMixtureMatrix */
    Tcl_DStringAppendElement ( &dString, " Stuff Needed for RPM\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void NormalizeMixtureMatrix(vtkpxMatrix *M_Outliers_Row);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for NormalizeMixtureMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NormalizeRPMMatrix */
    if ( strcmp ( argv[2], "NormalizeRPMMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NormalizeRPMMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NormalizeRPMMatrix */
    Tcl_DStringAppendElement ( &dString, " Stuff Needed for RPM\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void NormalizeRPMMatrix(vtkpxMatrix *M_Outliers_Row, vtkpxMatrix *M_Outliers_Column);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSparseMatrix" );
    /* Closing for NormalizeRPMMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxAbstractMatrixCppCommand(static_cast<vtkpxAbstractMatrix *>(op),interp,argc,argv) == TCL_OK)
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
