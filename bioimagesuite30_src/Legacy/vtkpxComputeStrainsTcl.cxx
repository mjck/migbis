// tcl wrapper for vtkpxComputeStrains object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxComputeStrains.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxComputeStrainsNewCommand()
{
  vtkpxComputeStrains *temp = vtkpxComputeStrains::New();
  return static_cast<ClientData>(temp);
}

int vtkProcessObjectCppCommand(vtkProcessObject *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxComputeStrainsCppCommand(vtkpxComputeStrains *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxComputeStrainsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxComputeStrainsCppCommand(static_cast<vtkpxComputeStrains *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxComputeStrainsCppCommand(vtkpxComputeStrains *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxComputeStrains",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkProcessObjectCppCommand(static_cast<vtkProcessObject *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkProcessObject"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxComputeStrains  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxComputeStrains");
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
    vtkpxComputeStrains  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxComputeStrains");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxComputeStrains  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxComputeStrains");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetBaseSolid",argv[1]))&&(argc == 3))
    {
    vtkpxFemSolid  *temp0;
    error = 0;

    temp0 = (vtkpxFemSolid *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxFemSolid"),interp,error));
    if (!error)
    {
    op->SetBaseSolid(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBaseSolid",argv[1]))&&(argc == 2))
    {
    vtkpxFemSolid  *temp20;
    temp20 = (op)->GetBaseSolid();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxFemSolid");
    return TCL_OK;
    }
  if ((!strcmp("SetTransformation",argv[1]))&&(argc == 3))
    {
    vtkAbstractTransform  *temp0;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    op->SetTransformation(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTransformation",argv[1]))&&(argc == 2))
    {
    vtkAbstractTransform  *temp20;
    temp20 = (op)->GetTransformation();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
    return TCL_OK;
    }
  if ((!strcmp("SetCoordinateFrame",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetCoordinateFrame(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCoordinateFrameMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCoordinateFrameMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCoordinateFrameMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCoordinateFrameMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCoordinateFrame",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCoordinateFrame();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCoordinateFrameToXYZ",argv[1]))&&(argc == 2))
    {
    op->SetCoordinateFrameToXYZ();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetCoordinateFrameToRadial",argv[1]))&&(argc == 2))
    {
    op->SetCoordinateFrameToRadial();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetCoordinateFrameToFibres",argv[1]))&&(argc == 2))
    {
    op->SetCoordinateFrameToFibres();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SaveOutput",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->SaveOutput(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Run",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->Run();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Run",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->Run(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateStudyFile",argv[1]))&&(argc == 5))
    {
    char    *temp0;
    char    *temp1;
    int      temp2;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    temp1 = argv[3];
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->GenerateStudyFile(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxComputeStrainsCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkProcessObjectCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxComputeStrains:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetBaseSolid\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBaseSolid\n",NULL);
    Tcl_AppendResult(interp,"  SetTransformation\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTransformation\n",NULL);
    Tcl_AppendResult(interp,"  SetCoordinateFrame\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCoordinateFrameMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCoordinateFrameMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetCoordinateFrame\n",NULL);
    Tcl_AppendResult(interp,"  SetCoordinateFrameToXYZ\n",NULL);
    Tcl_AppendResult(interp,"  SetCoordinateFrameToRadial\n",NULL);
    Tcl_AppendResult(interp,"  SetCoordinateFrameToFibres\n",NULL);
    Tcl_AppendResult(interp,"  SaveOutput\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Run\n",NULL);
    Tcl_AppendResult(interp,"  Run\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GenerateStudyFile\t with 3 args\n",NULL);
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
    vtkProcessObjectCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetBaseSolid" );
    Tcl_DStringAppendElement ( &dString, "GetBaseSolid" );
    Tcl_DStringAppendElement ( &dString, "SetTransformation" );
    Tcl_DStringAppendElement ( &dString, "GetTransformation" );
    Tcl_DStringAppendElement ( &dString, "SetCoordinateFrame" );
    Tcl_DStringAppendElement ( &dString, "GetCoordinateFrameMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetCoordinateFrameMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetCoordinateFrame" );
    Tcl_DStringAppendElement ( &dString, "SetCoordinateFrameToXYZ" );
    Tcl_DStringAppendElement ( &dString, "SetCoordinateFrameToRadial" );
    Tcl_DStringAppendElement ( &dString, "SetCoordinateFrameToFibres" );
    Tcl_DStringAppendElement ( &dString, "SaveOutput" );
    Tcl_DStringAppendElement ( &dString, "Run" );
    Tcl_DStringAppendElement ( &dString, "Run" );
    Tcl_DStringAppendElement ( &dString, "GenerateStudyFile" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkProcessObjectCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxComputeStrains *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBaseSolid */
    if ( strcmp ( argv[2], "SetBaseSolid" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBaseSolid" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBaseSolid */
    Tcl_DStringAppendElement ( &dString, " Set Solid\n" );
    Tcl_DStringAppendElement ( &dString, "void SetBaseSolid(vtkpxFemSolid *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for SetBaseSolid */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBaseSolid */
    if ( strcmp ( argv[2], "GetBaseSolid" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBaseSolid" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBaseSolid */
    Tcl_DStringAppendElement ( &dString, " Set Solid\n" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid *GetBaseSolid();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for GetBaseSolid */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransformation */
    if ( strcmp ( argv[2], "SetTransformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransformation */
    Tcl_DStringAppendElement ( &dString, " Set Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTransformation(vtkAbstractTransform *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for SetTransformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransformation */
    if ( strcmp ( argv[2], "GetTransformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransformation */
    Tcl_DStringAppendElement ( &dString, " Set Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform *GetTransformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for GetTransformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCoordinateFrame */
    if ( strcmp ( argv[2], "SetCoordinateFrame" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCoordinateFrame" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCoordinateFrame */
    Tcl_DStringAppendElement ( &dString, " Set/Get Coordinate Frame\n" );
    Tcl_DStringAppendElement ( &dString, "void SetCoordinateFrame(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for SetCoordinateFrame */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCoordinateFrameMinValue */
    if ( strcmp ( argv[2], "GetCoordinateFrameMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCoordinateFrameMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCoordinateFrameMinValue */
    Tcl_DStringAppendElement ( &dString, " Set/Get Coordinate Frame\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCoordinateFrameMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for GetCoordinateFrameMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCoordinateFrameMaxValue */
    if ( strcmp ( argv[2], "GetCoordinateFrameMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCoordinateFrameMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCoordinateFrameMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set/Get Coordinate Frame\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCoordinateFrameMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for GetCoordinateFrameMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCoordinateFrame */
    if ( strcmp ( argv[2], "GetCoordinateFrame" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCoordinateFrame" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCoordinateFrame */
    Tcl_DStringAppendElement ( &dString, " Set/Get Coordinate Frame\n" );
    Tcl_DStringAppendElement ( &dString, "int GetCoordinateFrame();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for GetCoordinateFrame */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCoordinateFrameToXYZ */
    if ( strcmp ( argv[2], "SetCoordinateFrameToXYZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCoordinateFrameToXYZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCoordinateFrameToXYZ */
    Tcl_DStringAppendElement ( &dString, " Set/Get Coordinate Frame\n" );
    Tcl_DStringAppendElement ( &dString, "void SetCoordinateFrameToXYZ();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for SetCoordinateFrameToXYZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCoordinateFrameToRadial */
    if ( strcmp ( argv[2], "SetCoordinateFrameToRadial" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCoordinateFrameToRadial" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCoordinateFrameToRadial */
    Tcl_DStringAppendElement ( &dString, " Set/Get Coordinate Frame\n" );
    Tcl_DStringAppendElement ( &dString, "void SetCoordinateFrameToRadial();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for SetCoordinateFrameToRadial */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCoordinateFrameToFibres */
    if ( strcmp ( argv[2], "SetCoordinateFrameToFibres" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCoordinateFrameToFibres" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCoordinateFrameToFibres */
    Tcl_DStringAppendElement ( &dString, " Set/Get Coordinate Frame\n" );
    Tcl_DStringAppendElement ( &dString, "void SetCoordinateFrameToFibres();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for SetCoordinateFrameToFibres */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaveOutput */
    if ( strcmp ( argv[2], "SaveOutput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaveOutput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaveOutput */
    Tcl_DStringAppendElement ( &dString, " I/O Code -- save as .sld file \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int SaveOutput(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for SaveOutput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Run */
    if ( strcmp ( argv[2], "Run" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Run" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Run */
    Tcl_DStringAppendElement ( &dString, " Generate Output\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Run();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for Run */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Run */
    if ( strcmp ( argv[2], "Run" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Run" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Run */
    Tcl_DStringAppendElement ( &dString, " Generate Output Image Instead\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Run(vtkImageData *in, vtkImageData *out);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for Run */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateStudyFile */
    if ( strcmp ( argv[2], "GenerateStudyFile" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateStudyFile" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateStudyFile */
    Tcl_DStringAppendElement ( &dString, " Generate .stat file\n" );
    Tcl_DStringAppendElement ( &dString, "static int GenerateStudyFile(const char *namestem, const char *sldname, int nframes);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxComputeStrains" );
    /* Closing for GenerateStudyFile */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkProcessObjectCppCommand(static_cast<vtkProcessObject *>(op),interp,argc,argv) == TCL_OK)
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
