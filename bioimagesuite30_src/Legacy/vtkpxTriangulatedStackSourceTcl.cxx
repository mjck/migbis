// tcl wrapper for vtkpxTriangulatedStackSource object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxTriangulatedStackSource.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxTriangulatedStackSourceNewCommand()
{
  vtkpxTriangulatedStackSource *temp = vtkpxTriangulatedStackSource::New();
  return static_cast<ClientData>(temp);
}

int vtkPolyDataSourceCppCommand(vtkPolyDataSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxTriangulatedStackSourceCppCommand(vtkpxTriangulatedStackSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxTriangulatedStackSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxTriangulatedStackSourceCppCommand(static_cast<vtkpxTriangulatedStackSource *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxTriangulatedStackSourceCppCommand(vtkpxTriangulatedStackSource *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxTriangulatedStackSource",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkPolyDataSourceCppCommand(static_cast<vtkPolyDataSource *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkPolyDataSource"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxTriangulatedStackSource  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxTriangulatedStackSource");
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
    vtkpxTriangulatedStackSource  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxTriangulatedStackSource");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxTriangulatedStackSource  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxTriangulatedStackSource");
    return TCL_OK;
    }
    }
  if ((!strcmp("Copy",argv[1]))&&(argc == 3))
    {
    vtkpxTriangulatedStackSource  *temp0;
    error = 0;

    temp0 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    if (!error)
    {
    op->Copy(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFromSplineStackSource",argv[1]))&&(argc == 6))
    {
    vtkpxSplineStackSource  *temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    error = 0;

    temp0 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (!error)
    {
    op->SetFromSplineStackSource(temp0,temp1,temp2,temp3);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFromSplineStackSource2",argv[1]))&&(argc == 7))
    {
    vtkpxSplineStackSource  *temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    float    temp4;
    error = 0;

    temp0 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (!error)
    {
    op->SetFromSplineStackSource2(temp0,temp1,temp2,temp3,temp4);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFromPolyData",argv[1]))&&(argc == 3))
    {
    vtkPolyData  *temp0;
    error = 0;

    temp0 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPolyData"),interp,error));
    if (!error)
    {
    op->SetFromPolyData(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeCurvatures",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->ComputeCurvatures(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("NonShrinkSmooth",argv[1]))&&(argc == 5))
    {
    float    temp0;
    float    temp1;
    int      temp2;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    op->NonShrinkSmooth(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
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
  if ((!strcmp("LoadAndFitContourStack",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->LoadAndFitContourStack(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetColorMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetColorMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetColorModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetColorModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetColorModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetColorModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetColorMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetColorMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetColorGamma",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetColorGamma(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetColorGammaMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetColorGammaMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetColorGammaMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetColorGammaMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetColorGamma",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetColorGamma();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxTriangulatedStackSourceCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkPolyDataSourceCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxTriangulatedStackSource:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Copy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetFromSplineStackSource\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  SetFromSplineStackSource2\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  SetFromPolyData\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ComputeCurvatures\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NonShrinkSmooth\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Scale\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  LoadAndFitContourStack\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetColorMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetColorModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetColorModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetColorMode\n",NULL);
    Tcl_AppendResult(interp,"  SetColorGamma\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetColorGammaMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetColorGammaMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetColorGamma\n",NULL);
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
    vtkPolyDataSourceCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    Tcl_DStringAppendElement ( &dString, "SetFromSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "SetFromSplineStackSource2" );
    Tcl_DStringAppendElement ( &dString, "SetFromPolyData" );
    Tcl_DStringAppendElement ( &dString, "ComputeCurvatures" );
    Tcl_DStringAppendElement ( &dString, "NonShrinkSmooth" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "Scale" );
    Tcl_DStringAppendElement ( &dString, "LoadAndFitContourStack" );
    Tcl_DStringAppendElement ( &dString, "SetColorMode" );
    Tcl_DStringAppendElement ( &dString, "GetColorModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetColorModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetColorMode" );
    Tcl_DStringAppendElement ( &dString, "SetColorGamma" );
    Tcl_DStringAppendElement ( &dString, "GetColorGammaMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetColorGammaMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetColorGamma" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkPolyDataSourceCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxTriangulatedStackSource *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for SafeDownCast */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Copy */
    Tcl_DStringAppendElement ( &dString, " Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Copy(vtkpxTriangulatedStackSource *other);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for Copy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFromSplineStackSource */
    if ( strcmp ( argv[2], "SetFromSplineStackSource" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFromSplineStackSource" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFromSplineStackSource */
    Tcl_DStringAppendElement ( &dString, " Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetFromSplineStackSource(vtkpxSplineStackSource *sur, float dist, float zmin=-1.0, float zmax=1000.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for SetFromSplineStackSource */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFromSplineStackSource2 */
    if ( strcmp ( argv[2], "SetFromSplineStackSource2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFromSplineStackSource2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFromSplineStackSource2 */
    Tcl_DStringAppendElement ( &dString, " Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetFromSplineStackSource2(vtkpxSplineStackSource *sur, float dist, float zdist, float zmin=-1.0, float zmax=1000.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for SetFromSplineStackSource2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFromPolyData */
    if ( strcmp ( argv[2], "SetFromPolyData" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFromPolyData" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFromPolyData */
    Tcl_DStringAppendElement ( &dString, " Set From Ordinary Poly Data ( Useful for computing curvatures)\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetFromPolyData(vtkPolyData *polydata);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for SetFromPolyData */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeCurvatures */
    if ( strcmp ( argv[2], "ComputeCurvatures" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeCurvatures" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeCurvatures */
    Tcl_DStringAppendElement ( &dString, " Compute Curvatures\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void ComputeCurvatures(int scale);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for ComputeCurvatures */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NonShrinkSmooth */
    if ( strcmp ( argv[2], "NonShrinkSmooth" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NonShrinkSmooth" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NonShrinkSmooth */
    Tcl_DStringAppendElement ( &dString, " Compute Curvatures\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void NonShrinkSmooth(float alpha, float beta, int iterations);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for NonShrinkSmooth */

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
    Tcl_DStringAppendElement ( &dString, " I/O Code\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Load(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
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
    Tcl_DStringAppendElement ( &dString, " I/O Code\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Save(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for Save */

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
    Tcl_DStringAppendElement ( &dString, " Scale all in-plane points\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Scale(float scale);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for Scale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LoadAndFitContourStack */
    if ( strcmp ( argv[2], "LoadAndFitContourStack" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LoadAndFitContourStack" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LoadAndFitContourStack */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void LoadAndFitContourStack(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for LoadAndFitContourStack */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetColorMode */
    if ( strcmp ( argv[2], "SetColorMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetColorMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetColorMode */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "void SetColorMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for SetColorMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetColorModeMinValue */
    if ( strcmp ( argv[2], "GetColorModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetColorModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetColorModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "int GetColorModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for GetColorModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetColorModeMaxValue */
    if ( strcmp ( argv[2], "GetColorModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetColorModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetColorModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "int GetColorModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for GetColorModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetColorMode */
    if ( strcmp ( argv[2], "GetColorMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetColorMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetColorMode */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "int GetColorMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for GetColorMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetColorGamma */
    if ( strcmp ( argv[2], "SetColorGamma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetColorGamma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetColorGamma */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Gamma \n" );
    Tcl_DStringAppendElement ( &dString, "void SetColorGamma(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for SetColorGamma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetColorGammaMinValue */
    if ( strcmp ( argv[2], "GetColorGammaMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetColorGammaMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetColorGammaMinValue */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Gamma \n" );
    Tcl_DStringAppendElement ( &dString, "float GetColorGammaMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for GetColorGammaMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetColorGammaMaxValue */
    if ( strcmp ( argv[2], "GetColorGammaMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetColorGammaMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetColorGammaMaxValue */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Gamma \n" );
    Tcl_DStringAppendElement ( &dString, "float GetColorGammaMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for GetColorGammaMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetColorGamma */
    if ( strcmp ( argv[2], "GetColorGamma" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetColorGamma" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetColorGamma */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Gamma \n" );
    Tcl_DStringAppendElement ( &dString, "float GetColorGamma();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    /* Closing for GetColorGamma */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkPolyDataSourceCppCommand(static_cast<vtkPolyDataSource *>(op),interp,argc,argv) == TCL_OK)
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
