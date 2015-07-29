// tcl wrapper for vtkbisCsfWmMotionCorrection object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkbisCsfWmMotionCorrection.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkbisCsfWmMotionCorrectionNewCommand()
{
  vtkbisCsfWmMotionCorrection *temp = vtkbisCsfWmMotionCorrection::New();
  return static_cast<ClientData>(temp);
}

int vtkSimpleImageToImageFilterCppCommand(vtkSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkbisCsfWmMotionCorrectionCppCommand(vtkbisCsfWmMotionCorrection *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkbisCsfWmMotionCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkbisCsfWmMotionCorrectionCppCommand(static_cast<vtkbisCsfWmMotionCorrection *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkbisCsfWmMotionCorrectionCppCommand(vtkbisCsfWmMotionCorrection *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkbisCsfWmMotionCorrection",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkSimpleImageToImageFilterCppCommand(static_cast<vtkSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkSimpleImageToImageFilter"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
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
    vtkbisCsfWmMotionCorrection  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisCsfWmMotionCorrection");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkbisCsfWmMotionCorrection  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisCsfWmMotionCorrection");
    return TCL_OK;
    }
    }
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkbisCsfWmMotionCorrection  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisCsfWmMotionCorrection");
    return TCL_OK;
    }
  if ((!strcmp("SetImageMask",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetImageMask(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetImageMask",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetImageMask();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetMotionMatrix",argv[1]))&&(argc == 3))
    {
    vtkpxMatrix  *temp0;
    error = 0;

    temp0 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    op->SetMotionMatrix(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMotionMatrix",argv[1]))&&(argc == 2))
    {
    vtkpxMatrix  *temp20;
    temp20 = (op)->GetMotionMatrix();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxMatrix");
    return TCL_OK;
    }
  if ((!strcmp("SetLSQ",argv[1]))&&(argc == 3))
    {
    vtkpxMatrix  *temp0;
    error = 0;

    temp0 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    op->SetLSQ(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLSQ",argv[1]))&&(argc == 2))
    {
    vtkpxMatrix  *temp20;
    temp20 = (op)->GetLSQ();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxMatrix");
    return TCL_OK;
    }
  if ((!strcmp("SetFileName",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetFileName(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFileName",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetFileName();
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
  if ((!strcmp("SetdoMotion",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetdoMotion(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetdoMotionMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetdoMotionMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetdoMotionMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetdoMotionMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetdoMotion",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetdoMotion();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetdoCsfWm",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetdoCsfWm(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetdoCsfWmMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetdoCsfWmMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetdoCsfWmMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetdoCsfWmMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetdoCsfWm",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetdoCsfWm();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkbisCsfWmMotionCorrectionCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkbisCsfWmMotionCorrection:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  SetImageMask\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetImageMask\n",NULL);
    Tcl_AppendResult(interp,"  SetMotionMatrix\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMotionMatrix\n",NULL);
    Tcl_AppendResult(interp,"  SetLSQ\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLSQ\n",NULL);
    Tcl_AppendResult(interp,"  SetFileName\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFileName\n",NULL);
    Tcl_AppendResult(interp,"  SetdoMotion\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetdoMotionMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetdoMotionMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetdoMotion\n",NULL);
    Tcl_AppendResult(interp,"  SetdoCsfWm\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetdoCsfWmMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetdoCsfWmMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetdoCsfWm\n",NULL);
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
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "SetImageMask" );
    Tcl_DStringAppendElement ( &dString, "GetImageMask" );
    Tcl_DStringAppendElement ( &dString, "SetMotionMatrix" );
    Tcl_DStringAppendElement ( &dString, "GetMotionMatrix" );
    Tcl_DStringAppendElement ( &dString, "SetLSQ" );
    Tcl_DStringAppendElement ( &dString, "GetLSQ" );
    Tcl_DStringAppendElement ( &dString, "SetFileName" );
    Tcl_DStringAppendElement ( &dString, "GetFileName" );
    Tcl_DStringAppendElement ( &dString, "SetdoMotion" );
    Tcl_DStringAppendElement ( &dString, "GetdoMotionMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetdoMotionMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetdoMotion" );
    Tcl_DStringAppendElement ( &dString, "SetdoCsfWm" );
    Tcl_DStringAppendElement ( &dString, "GetdoCsfWmMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetdoCsfWmMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetdoCsfWm" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    if ( SuperClassStatus == TCL_OK ) { return TCL_OK; }
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
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkbisCsfWmMotionCorrection *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for New */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetImageMask */
    if ( strcmp ( argv[2], "SetImageMask" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetImageMask" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetImageMask */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetImageMask(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for SetImageMask */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageMask */
    if ( strcmp ( argv[2], "GetImageMask" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageMask" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageMask */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetImageMask();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for GetImageMask */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMotionMatrix */
    if ( strcmp ( argv[2], "SetMotionMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMotionMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMotionMatrix */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetMotionMatrix(vtkpxMatrix *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for SetMotionMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMotionMatrix */
    if ( strcmp ( argv[2], "GetMotionMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMotionMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMotionMatrix */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix *GetMotionMatrix();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for GetMotionMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLSQ */
    if ( strcmp ( argv[2], "SetLSQ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLSQ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLSQ */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetLSQ(vtkpxMatrix *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for SetLSQ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLSQ */
    if ( strcmp ( argv[2], "GetLSQ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLSQ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLSQ */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix *GetLSQ();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for GetLSQ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFileName */
    if ( strcmp ( argv[2], "SetFileName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFileName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFileName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetFileName(const char *filename);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for SetFileName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFileName */
    if ( strcmp ( argv[2], "GetFileName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFileName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFileName */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "const char *GetFileName();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for GetFileName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetdoMotion */
    if ( strcmp ( argv[2], "SetdoMotion" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetdoMotion" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetdoMotion */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetdoMotion(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for SetdoMotion */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetdoMotionMinValue */
    if ( strcmp ( argv[2], "GetdoMotionMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetdoMotionMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetdoMotionMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetdoMotionMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for GetdoMotionMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetdoMotionMaxValue */
    if ( strcmp ( argv[2], "GetdoMotionMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetdoMotionMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetdoMotionMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetdoMotionMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for GetdoMotionMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetdoMotion */
    if ( strcmp ( argv[2], "GetdoMotion" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetdoMotion" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetdoMotion */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetdoMotion();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for GetdoMotion */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetdoCsfWm */
    if ( strcmp ( argv[2], "SetdoCsfWm" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetdoCsfWm" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetdoCsfWm */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetdoCsfWm(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for SetdoCsfWm */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetdoCsfWmMinValue */
    if ( strcmp ( argv[2], "GetdoCsfWmMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetdoCsfWmMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetdoCsfWmMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetdoCsfWmMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for GetdoCsfWmMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetdoCsfWmMaxValue */
    if ( strcmp ( argv[2], "GetdoCsfWmMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetdoCsfWmMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetdoCsfWmMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetdoCsfWmMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for GetdoCsfWmMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetdoCsfWm */
    if ( strcmp ( argv[2], "GetdoCsfWm" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetdoCsfWm" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetdoCsfWm */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetdoCsfWm();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisCsfWmMotionCorrection" );
    /* Closing for GetdoCsfWm */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkSimpleImageToImageFilterCppCommand(static_cast<vtkSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
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
