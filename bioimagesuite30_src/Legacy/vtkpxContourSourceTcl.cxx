// tcl wrapper for vtkpxContourSource object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxContourSource.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxContourSourceNewCommand()
{
  vtkpxContourSource *temp = vtkpxContourSource::New();
  return static_cast<ClientData>(temp);
}

int vtkPolyDataSourceCppCommand(vtkPolyDataSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxContourSourceCppCommand(vtkpxContourSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxContourSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxContourSourceCppCommand(static_cast<vtkpxContourSource *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxContourSourceCppCommand(vtkpxContourSource *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxContourSource",argv[1]))
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
    vtkpxContourSource  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxContourSource");
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
    vtkpxContourSource  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxContourSource");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxContourSource  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxContourSource");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetClosed",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetClosed(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetClosedMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetClosedMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetClosedMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetClosedMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetClosed",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetClosed();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ClosedOn",argv[1]))&&(argc == 2))
    {
    op->ClosedOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ClosedOff",argv[1]))&&(argc == 2))
    {
    op->ClosedOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetPoints",argv[1]))&&(argc == 2))
    {
    vtkPoints  *temp20;
    temp20 = (op)->GetPoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPoints");
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
  if ((!strcmp("Copy",argv[1]))&&(argc == 3))
    {
    vtkpxContourSource  *temp0;
    error = 0;

    temp0 = (vtkpxContourSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxContourSource"),interp,error));
    if (!error)
    {
    op->Copy(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFromSplineStackSource",argv[1]))&&(argc == 5))
    {
    vtkpxSplineStackSource  *temp0;
    int      temp1;
    float    temp2;
    error = 0;

    temp0 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->SetFromSplineStackSource(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("EnsureAnticlockwise",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->EnsureAnticlockwise();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("EnsureClockwise",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->EnsureClockwise();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetDirection",argv[1]))&&(argc == 3))
    {
    int      temp0;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->SetDirection(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetCircle",argv[1]))&&(argc == 6))
    {
    int      temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (!error)
    {
    op->SetCircle(temp0,temp1,temp2,temp3);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("IsInside",argv[1]))&&(argc == 4))
    {
    float    temp0;
    float    temp1;
    int      temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->IsInside(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Intersect",argv[1]))&&(argc == 8))
    {
    float    temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    float    temp4;
    float    temp5;
    int      temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (!error)
    {
    temp20 = (op)->Intersect(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetArea",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetArea();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetContourLength",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetContourLength();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Smooth",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->Smooth(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Sample",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->Sample(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Subsample",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->Subsample(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Equispace",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->Equispace(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Equisample",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->Equisample(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("MapContours",argv[1]))&&(argc == 5))
    {
    vtkpxContourSource  *temp0;
    vtkpxContourSource  *temp1;
    int      temp2;
    vtkpxContourSource  *temp20;
    error = 0;

    temp0 = (vtkpxContourSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxContourSource"),interp,error));
    temp1 = (vtkpxContourSource *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxContourSource"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->MapContours(temp0,temp1,temp2);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxContourSource");
    return TCL_OK;
    }
    }
  if ((!strcmp("CyclicDistance",argv[1]))&&(argc == 5))
    {
    int      temp0;
    int      temp1;
    int      temp2;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->CyclicDistance(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxContourSourceCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkPolyDataSourceCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxContourSource:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetClosed\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetClosedMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetClosedMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetClosed\n",NULL);
    Tcl_AppendResult(interp,"  ClosedOn\n",NULL);
    Tcl_AppendResult(interp,"  ClosedOff\n",NULL);
    Tcl_AppendResult(interp,"  GetPoints\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Copy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetFromSplineStackSource\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  EnsureAnticlockwise\n",NULL);
    Tcl_AppendResult(interp,"  EnsureClockwise\n",NULL);
    Tcl_AppendResult(interp,"  SetDirection\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetCircle\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  IsInside\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Intersect\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  GetArea\n",NULL);
    Tcl_AppendResult(interp,"  GetContourLength\n",NULL);
    Tcl_AppendResult(interp,"  Smooth\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Sample\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Subsample\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Equispace\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Equisample\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  MapContours\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CyclicDistance\t with 3 args\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetClosed" );
    Tcl_DStringAppendElement ( &dString, "GetClosedMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetClosedMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetClosed" );
    Tcl_DStringAppendElement ( &dString, "ClosedOn" );
    Tcl_DStringAppendElement ( &dString, "ClosedOff" );
    Tcl_DStringAppendElement ( &dString, "GetPoints" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    Tcl_DStringAppendElement ( &dString, "SetFromSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "EnsureAnticlockwise" );
    Tcl_DStringAppendElement ( &dString, "EnsureClockwise" );
    Tcl_DStringAppendElement ( &dString, "SetDirection" );
    Tcl_DStringAppendElement ( &dString, "SetCircle" );
    Tcl_DStringAppendElement ( &dString, "IsInside" );
    Tcl_DStringAppendElement ( &dString, "Intersect" );
    Tcl_DStringAppendElement ( &dString, "GetArea" );
    Tcl_DStringAppendElement ( &dString, "GetContourLength" );
    Tcl_DStringAppendElement ( &dString, "Smooth" );
    Tcl_DStringAppendElement ( &dString, "Sample" );
    Tcl_DStringAppendElement ( &dString, "Subsample" );
    Tcl_DStringAppendElement ( &dString, "Equispace" );
    Tcl_DStringAppendElement ( &dString, "Equisample" );
    Tcl_DStringAppendElement ( &dString, "MapContours" );
    Tcl_DStringAppendElement ( &dString, "CyclicDistance" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxContourSource *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetClosed */
    if ( strcmp ( argv[2], "SetClosed" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetClosed" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetClosed */
    Tcl_DStringAppendElement ( &dString, " Is Contour Closed?\n" );
    Tcl_DStringAppendElement ( &dString, "void SetClosed(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for SetClosed */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetClosedMinValue */
    if ( strcmp ( argv[2], "GetClosedMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetClosedMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetClosedMinValue */
    Tcl_DStringAppendElement ( &dString, " Is Contour Closed?\n" );
    Tcl_DStringAppendElement ( &dString, "int GetClosedMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for GetClosedMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetClosedMaxValue */
    if ( strcmp ( argv[2], "GetClosedMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetClosedMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetClosedMaxValue */
    Tcl_DStringAppendElement ( &dString, " Is Contour Closed?\n" );
    Tcl_DStringAppendElement ( &dString, "int GetClosedMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for GetClosedMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetClosed */
    if ( strcmp ( argv[2], "GetClosed" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetClosed" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetClosed */
    Tcl_DStringAppendElement ( &dString, " Is Contour Closed?\n" );
    Tcl_DStringAppendElement ( &dString, "int GetClosed();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for GetClosed */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ClosedOn */
    if ( strcmp ( argv[2], "ClosedOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ClosedOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ClosedOn */
    Tcl_DStringAppendElement ( &dString, " Is Contour Closed?\n" );
    Tcl_DStringAppendElement ( &dString, "void ClosedOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for ClosedOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ClosedOff */
    if ( strcmp ( argv[2], "ClosedOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ClosedOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ClosedOff */
    Tcl_DStringAppendElement ( &dString, " Is Contour Closed?\n" );
    Tcl_DStringAppendElement ( &dString, "void ClosedOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for ClosedOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPoints */
    if ( strcmp ( argv[2], "GetPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPoints */
    Tcl_DStringAppendElement ( &dString, " Get Points\n" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints *GetPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for GetPoints */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Copy */
    Tcl_DStringAppendElement ( &dString, " Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Copy(vtkpxContourSource *other);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
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
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFromSplineStackSource */
    Tcl_DStringAppendElement ( &dString, " Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetFromSplineStackSource(vtkpxSplineStackSource *sur, int level, float dist);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for SetFromSplineStackSource */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: EnsureAnticlockwise */
    if ( strcmp ( argv[2], "EnsureAnticlockwise" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "EnsureAnticlockwise" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for EnsureAnticlockwise */
    Tcl_DStringAppendElement ( &dString, " Orientation Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int EnsureAnticlockwise();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for EnsureAnticlockwise */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: EnsureClockwise */
    if ( strcmp ( argv[2], "EnsureClockwise" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "EnsureClockwise" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for EnsureClockwise */
    Tcl_DStringAppendElement ( &dString, " Orientation Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int EnsureClockwise();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for EnsureClockwise */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDirection */
    if ( strcmp ( argv[2], "SetDirection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDirection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDirection */
    Tcl_DStringAppendElement ( &dString, " Orientation Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int SetDirection(int dir=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for SetDirection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCircle */
    if ( strcmp ( argv[2], "SetCircle" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCircle" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCircle */
    Tcl_DStringAppendElement ( &dString, " Initialize \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetCircle(int np=10, float radius=5.0, float x=50.0, float y=50.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for SetCircle */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: IsInside */
    if ( strcmp ( argv[2], "IsInside" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "IsInside" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for IsInside */
    Tcl_DStringAppendElement ( &dString, " Check Inside/Outside\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int IsInside(float testpointx, float testpointy);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for IsInside */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Intersect */
    if ( strcmp ( argv[2], "Intersect" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Intersect" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Intersect */
    Tcl_DStringAppendElement ( &dString, " Check Inside/Outside\n" );
    Tcl_DStringAppendElement ( &dString, "static int Intersect(float x1, float y1, float x2, float y2, float x, float y);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for Intersect */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetArea */
    if ( strcmp ( argv[2], "GetArea" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetArea" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetArea */
    Tcl_DStringAppendElement ( &dString, " Contour Statistics\n ----------------------\n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetArea();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for GetArea */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetContourLength */
    if ( strcmp ( argv[2], "GetContourLength" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetContourLength" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetContourLength */
    Tcl_DStringAppendElement ( &dString, " Contour Statistics\n ----------------------\n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetContourLength();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for GetContourLength */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Smooth */
    if ( strcmp ( argv[2], "Smooth" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Smooth" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Smooth */
    Tcl_DStringAppendElement ( &dString, " Contour Manipulation\n ------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Smooth(float relaxation);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for Smooth */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Sample */
    if ( strcmp ( argv[2], "Sample" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Sample" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Sample */
    Tcl_DStringAppendElement ( &dString, " Contour Manipulation\n ------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Sample(int width);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for Sample */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Subsample */
    if ( strcmp ( argv[2], "Subsample" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Subsample" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Subsample */
    Tcl_DStringAppendElement ( &dString, " Contour Manipulation\n ------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Subsample(int maxratio);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for Subsample */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Equispace */
    if ( strcmp ( argv[2], "Equispace" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Equispace" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Equispace */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void Equispace(float spacing);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for Equispace */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Equisample */
    if ( strcmp ( argv[2], "Equisample" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Equisample" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Equisample */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void Equisample(int npoints);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for Equisample */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: MapContours */
    if ( strcmp ( argv[2], "MapContours" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "MapContours" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for MapContours */
    Tcl_DStringAppendElement ( &dString, " Symmetric Nearest Neighbor Map of Contours\n ------------------------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkpxContourSource *MapContours(vtkpxContourSource *cnt1, vtkpxContourSource *cntr2, int resample=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for MapContours */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CyclicDistance */
    if ( strcmp ( argv[2], "CyclicDistance" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CyclicDistance" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CyclicDistance */
    Tcl_DStringAppendElement ( &dString, " Symmetric Nearest Neighbor Map of Contours\n ------------------------------------------\n" );
    Tcl_DStringAppendElement ( &dString, "static int CyclicDistance(int p1, int p2, int np);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxContourSource" );
    /* Closing for CyclicDistance */

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
