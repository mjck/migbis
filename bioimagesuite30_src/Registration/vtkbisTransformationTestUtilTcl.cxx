// tcl wrapper for vtkbisTransformationTestUtil object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkbisTransformationTestUtil.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkbisTransformationTestUtilNewCommand()
{
  vtkbisTransformationTestUtil *temp = vtkbisTransformationTestUtil::New();
  return static_cast<ClientData>(temp);
}

int vtkObjectCppCommand(vtkObject *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkbisTransformationTestUtilCppCommand(vtkbisTransformationTestUtil *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkbisTransformationTestUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkbisTransformationTestUtilCppCommand(static_cast<vtkbisTransformationTestUtil *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkbisTransformationTestUtilCppCommand(vtkbisTransformationTestUtil *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkbisTransformationTestUtil",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkObjectCppCommand(static_cast<vtkObject *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkObject"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkbisTransformationTestUtil  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisTransformationTestUtil");
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
    vtkbisTransformationTestUtil  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisTransformationTestUtil");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkbisTransformationTestUtil  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisTransformationTestUtil");
    return TCL_OK;
    }
    }
  if ((!strcmp("CompareTransformations",argv[1]))&&(argc == 4))
    {
    vtkAbstractTransform  *temp0;
    vtkAbstractTransform  *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp1 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->CompareTransformations(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CompareTransformations",argv[1]))&&(argc == 5))
    {
    vtkAbstractTransform  *temp0;
    vtkAbstractTransform  *temp1;
    double   temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp1 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->CompareTransformations(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CompareHomogeneousTransformations",argv[1]))&&(argc == 5))
    {
    vtkHomogeneousTransform  *temp0;
    vtkHomogeneousTransform  *temp1;
    double   temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkHomogeneousTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkHomogeneousTransform"),interp,error));
    temp1 = (vtkHomogeneousTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkHomogeneousTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->CompareHomogeneousTransformations(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CompareThinPlateSplineTransformations",argv[1]))&&(argc == 5))
    {
    vtkThinPlateSplineTransform  *temp0;
    vtkThinPlateSplineTransform  *temp1;
    double   temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkThinPlateSplineTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkThinPlateSplineTransform"),interp,error));
    temp1 = (vtkThinPlateSplineTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkThinPlateSplineTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->CompareThinPlateSplineTransformations(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CompareGridTransformations",argv[1]))&&(argc == 5))
    {
    vtkGridTransform  *temp0;
    vtkGridTransform  *temp1;
    double   temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkGridTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkGridTransform"),interp,error));
    temp1 = (vtkGridTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkGridTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->CompareGridTransformations(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CompareComboTransformations",argv[1]))&&(argc == 5))
    {
    vtkpxComboTransform  *temp0;
    vtkpxComboTransform  *temp1;
    double   temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkpxComboTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxComboTransform"),interp,error));
    temp1 = (vtkpxComboTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxComboTransform"),interp,error));
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->CompareComboTransformations(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ReturnStatus",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->ReturnStatus();
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
  if ((!strcmp("ClearStatus",argv[1]))&&(argc == 2))
    {
    op->ClearStatus();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkbisTransformationTestUtilCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkObjectCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkbisTransformationTestUtil:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  CompareTransformations\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  CompareTransformations\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CompareHomogeneousTransformations\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CompareThinPlateSplineTransformations\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CompareGridTransformations\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CompareComboTransformations\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  ReturnStatus\n",NULL);
    Tcl_AppendResult(interp,"  ClearStatus\n",NULL);
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
    vtkObjectCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "CompareTransformations" );
    Tcl_DStringAppendElement ( &dString, "CompareTransformations" );
    Tcl_DStringAppendElement ( &dString, "CompareHomogeneousTransformations" );
    Tcl_DStringAppendElement ( &dString, "CompareThinPlateSplineTransformations" );
    Tcl_DStringAppendElement ( &dString, "CompareGridTransformations" );
    Tcl_DStringAppendElement ( &dString, "CompareComboTransformations" );
    Tcl_DStringAppendElement ( &dString, "ReturnStatus" );
    Tcl_DStringAppendElement ( &dString, "ClearStatus" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkObjectCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkbisTransformationTestUtil *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CompareTransformations */
    if ( strcmp ( argv[2], "CompareTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CompareTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CompareTransformations */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int CompareTransformations(vtkAbstractTransform *tr1, vtkAbstractTransform *tr2);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
    /* Closing for CompareTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CompareTransformations */
    if ( strcmp ( argv[2], "CompareTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CompareTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CompareTransformations */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int CompareTransformations(vtkAbstractTransform *tr1, vtkAbstractTransform *tr2, double threshold);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
    /* Closing for CompareTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CompareHomogeneousTransformations */
    if ( strcmp ( argv[2], "CompareHomogeneousTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CompareHomogeneousTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkHomogeneousTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkHomogeneousTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CompareHomogeneousTransformations */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int CompareHomogeneousTransformations(vtkHomogeneousTransform *tr1, vtkHomogeneousTransform *tr2, double threshold);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
    /* Closing for CompareHomogeneousTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CompareThinPlateSplineTransformations */
    if ( strcmp ( argv[2], "CompareThinPlateSplineTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CompareThinPlateSplineTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkThinPlateSplineTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkThinPlateSplineTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CompareThinPlateSplineTransformations */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int CompareThinPlateSplineTransformations(vtkThinPlateSplineTransform *tr1, vtkThinPlateSplineTransform *tr2, double threshold);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
    /* Closing for CompareThinPlateSplineTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CompareGridTransformations */
    if ( strcmp ( argv[2], "CompareGridTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CompareGridTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkGridTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkGridTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CompareGridTransformations */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int CompareGridTransformations(vtkGridTransform *tr1, vtkGridTransform *tr2, double threshold);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
    /* Closing for CompareGridTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CompareComboTransformations */
    if ( strcmp ( argv[2], "CompareComboTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CompareComboTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CompareComboTransformations */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int CompareComboTransformations(vtkpxComboTransform *tr1, vtkpxComboTransform *tr2, double threshold);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
    /* Closing for CompareComboTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ReturnStatus */
    if ( strcmp ( argv[2], "ReturnStatus" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ReturnStatus" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ReturnStatus */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "const char *ReturnStatus();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
    /* Closing for ReturnStatus */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ClearStatus */
    if ( strcmp ( argv[2], "ClearStatus" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ClearStatus" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ClearStatus */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void ClearStatus();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisTransformationTestUtil" );
    /* Closing for ClearStatus */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkObjectCppCommand(static_cast<vtkObject *>(op),interp,argc,argv) == TCL_OK)
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
