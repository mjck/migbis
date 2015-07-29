// tcl wrapper for vtkbisDijkstraGraphGeodesicPath object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkbisDijkstraGraphGeodesicPath.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkbisDijkstraGraphGeodesicPathNewCommand()
{
  vtkbisDijkstraGraphGeodesicPath *temp = vtkbisDijkstraGraphGeodesicPath::New();
  return static_cast<ClientData>(temp);
}

int vtkGraphGeodesicPathCppCommand(vtkGraphGeodesicPath *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkbisDijkstraGraphGeodesicPathCppCommand(vtkbisDijkstraGraphGeodesicPath *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkbisDijkstraGraphGeodesicPathCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkbisDijkstraGraphGeodesicPathCppCommand(static_cast<vtkbisDijkstraGraphGeodesicPath *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkbisDijkstraGraphGeodesicPathCppCommand(vtkbisDijkstraGraphGeodesicPath *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkbisDijkstraGraphGeodesicPath",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkGraphGeodesicPathCppCommand(static_cast<vtkGraphGeodesicPath *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkGraphGeodesicPath"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkbisDijkstraGraphGeodesicPath  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisDijkstraGraphGeodesicPath");
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
    vtkbisDijkstraGraphGeodesicPath  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisDijkstraGraphGeodesicPath");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkbisDijkstraGraphGeodesicPath  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisDijkstraGraphGeodesicPath");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetIdList",argv[1]))&&(argc == 2))
    {
    vtkIdList  *temp20;
    temp20 = (op)->GetIdList();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkIdList");
    return TCL_OK;
    }
  if ((!strcmp("Getd",argv[1]))&&(argc == 2))
    {
    vtkFloatArray  *temp20;
    temp20 = (op)->Getd();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
  if ((!strcmp("SetStopWhenEndReached",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetStopWhenEndReached(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetStopWhenEndReached",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetStopWhenEndReached();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("StopWhenEndReachedOn",argv[1]))&&(argc == 2))
    {
    op->StopWhenEndReachedOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("StopWhenEndReachedOff",argv[1]))&&(argc == 2))
    {
    op->StopWhenEndReachedOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetUseScalarWeights",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetUseScalarWeights(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUseScalarWeights",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseScalarWeights();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("UseScalarWeightsOn",argv[1]))&&(argc == 2))
    {
    op->UseScalarWeightsOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("UseScalarWeightsOff",argv[1]))&&(argc == 2))
    {
    op->UseScalarWeightsOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetGeodesicLength",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetGeodesicLength();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkbisDijkstraGraphGeodesicPathCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkGraphGeodesicPathCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkbisDijkstraGraphGeodesicPath:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetIdList\n",NULL);
    Tcl_AppendResult(interp,"  Getd\n",NULL);
    Tcl_AppendResult(interp,"  SetStopWhenEndReached\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetStopWhenEndReached\n",NULL);
    Tcl_AppendResult(interp,"  StopWhenEndReachedOn\n",NULL);
    Tcl_AppendResult(interp,"  StopWhenEndReachedOff\n",NULL);
    Tcl_AppendResult(interp,"  SetUseScalarWeights\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUseScalarWeights\n",NULL);
    Tcl_AppendResult(interp,"  UseScalarWeightsOn\n",NULL);
    Tcl_AppendResult(interp,"  UseScalarWeightsOff\n",NULL);
    Tcl_AppendResult(interp,"  GetGeodesicLength\n",NULL);
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
    vtkGraphGeodesicPathCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "GetIdList" );
    Tcl_DStringAppendElement ( &dString, "Getd" );
    Tcl_DStringAppendElement ( &dString, "SetStopWhenEndReached" );
    Tcl_DStringAppendElement ( &dString, "GetStopWhenEndReached" );
    Tcl_DStringAppendElement ( &dString, "StopWhenEndReachedOn" );
    Tcl_DStringAppendElement ( &dString, "StopWhenEndReachedOff" );
    Tcl_DStringAppendElement ( &dString, "SetUseScalarWeights" );
    Tcl_DStringAppendElement ( &dString, "GetUseScalarWeights" );
    Tcl_DStringAppendElement ( &dString, "UseScalarWeightsOn" );
    Tcl_DStringAppendElement ( &dString, "UseScalarWeightsOff" );
    Tcl_DStringAppendElement ( &dString, "GetGeodesicLength" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkGraphGeodesicPathCppCommand(op,interp,argc,argv);
    if ( SuperClassStatus == TCL_OK ) { return TCL_OK; }
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, " Instantiate the class\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkbisDijkstraGraphGeodesicPath *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
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
    Tcl_DStringAppendElement ( &dString, " Instantiate the class\n" );
    Tcl_DStringAppendElement ( &dString, "const char *GetClassName();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
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
    Tcl_DStringAppendElement ( &dString, " Instantiate the class\n" );
    Tcl_DStringAppendElement ( &dString, "int IsA(const char *name);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
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
    Tcl_DStringAppendElement ( &dString, " Instantiate the class\n" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
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
    Tcl_DStringAppendElement ( &dString, " Instantiate the class\n" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIdList */
    if ( strcmp ( argv[2], "GetIdList" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIdList" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIdList */
    Tcl_DStringAppendElement ( &dString, " The vertex ids (of the input polydata) on the shortest path\n" );
    Tcl_DStringAppendElement ( &dString, "vtkIdList *GetIdList();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for GetIdList */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Getd */
    if ( strcmp ( argv[2], "Getd" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Getd" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Getd */
    Tcl_DStringAppendElement ( &dString, " Get the summed weight for all vertices\n" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray *Getd();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for Getd */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetStopWhenEndReached */
    if ( strcmp ( argv[2], "SetStopWhenEndReached" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetStopWhenEndReached" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetStopWhenEndReached */
    Tcl_DStringAppendElement ( &dString, " Stop when the end vertex is reached\n or calculate shortest path to all vertices\n" );
    Tcl_DStringAppendElement ( &dString, "void SetStopWhenEndReached(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for SetStopWhenEndReached */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetStopWhenEndReached */
    if ( strcmp ( argv[2], "GetStopWhenEndReached" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetStopWhenEndReached" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetStopWhenEndReached */
    Tcl_DStringAppendElement ( &dString, " Stop when the end vertex is reached\n or calculate shortest path to all vertices\n" );
    Tcl_DStringAppendElement ( &dString, "int GetStopWhenEndReached();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for GetStopWhenEndReached */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: StopWhenEndReachedOn */
    if ( strcmp ( argv[2], "StopWhenEndReachedOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "StopWhenEndReachedOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for StopWhenEndReachedOn */
    Tcl_DStringAppendElement ( &dString, " Stop when the end vertex is reached\n or calculate shortest path to all vertices\n" );
    Tcl_DStringAppendElement ( &dString, "void StopWhenEndReachedOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for StopWhenEndReachedOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: StopWhenEndReachedOff */
    if ( strcmp ( argv[2], "StopWhenEndReachedOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "StopWhenEndReachedOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for StopWhenEndReachedOff */
    Tcl_DStringAppendElement ( &dString, " Stop when the end vertex is reached\n or calculate shortest path to all vertices\n" );
    Tcl_DStringAppendElement ( &dString, "void StopWhenEndReachedOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for StopWhenEndReachedOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUseScalarWeights */
    if ( strcmp ( argv[2], "SetUseScalarWeights" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUseScalarWeights" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUseScalarWeights */
    Tcl_DStringAppendElement ( &dString, " Use scalar values in the edge weight (experimental)\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUseScalarWeights(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for SetUseScalarWeights */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseScalarWeights */
    if ( strcmp ( argv[2], "GetUseScalarWeights" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseScalarWeights" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseScalarWeights */
    Tcl_DStringAppendElement ( &dString, " Use scalar values in the edge weight (experimental)\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseScalarWeights();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for GetUseScalarWeights */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseScalarWeightsOn */
    if ( strcmp ( argv[2], "UseScalarWeightsOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseScalarWeightsOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseScalarWeightsOn */
    Tcl_DStringAppendElement ( &dString, " Use scalar values in the edge weight (experimental)\n" );
    Tcl_DStringAppendElement ( &dString, "void UseScalarWeightsOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for UseScalarWeightsOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseScalarWeightsOff */
    if ( strcmp ( argv[2], "UseScalarWeightsOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseScalarWeightsOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseScalarWeightsOff */
    Tcl_DStringAppendElement ( &dString, " Use scalar values in the edge weight (experimental)\n" );
    Tcl_DStringAppendElement ( &dString, "void UseScalarWeightsOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for UseScalarWeightsOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetGeodesicLength */
    if ( strcmp ( argv[2], "GetGeodesicLength" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetGeodesicLength" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetGeodesicLength */
    Tcl_DStringAppendElement ( &dString, " TODO: Get the total geodesic length.\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double GetGeodesicLength();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisDijkstraGraphGeodesicPath" );
    /* Closing for GetGeodesicLength */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkGraphGeodesicPathCppCommand(static_cast<vtkGraphGeodesicPath *>(op),interp,argc,argv) == TCL_OK)
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
