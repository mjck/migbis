// tcl wrapper for vtkbisROIMean object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkbisROIMean.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkbisROIMeanNewCommand()
{
  vtkbisROIMean *temp = vtkbisROIMean::New();
  return static_cast<ClientData>(temp);
}

int vtkSimpleImageToImageFilterCppCommand(vtkSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkbisROIMeanCppCommand(vtkbisROIMean *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkbisROIMeanCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkbisROIMeanCppCommand(static_cast<vtkbisROIMean *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkbisROIMeanCppCommand(vtkbisROIMean *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkbisROIMean",argv[1]))
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
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkbisROIMean  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisROIMean");
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
    vtkbisROIMean  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisROIMean");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkbisROIMean  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkbisROIMean");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetImageROI",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetImageROI(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetImageROI",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetImageROI();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetCentroids",argv[1]))&&(argc == 2))
    {
    vtkPoints  *temp20;
    temp20 = (op)->GetCentroids();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPoints");
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
  if ((!strcmp("SetFileName2",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetFileName2(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFileName2",argv[1]))&&(argc == 2))
    {
    const char    *temp20;
    temp20 = (op)->GetFileName2();
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
  if ((!strcmp("SetDoText",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetDoText(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDoTextMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDoTextMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDoTextMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDoTextMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDoText",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDoText();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetAddRegion",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetAddRegion(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAddRegionMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetAddRegionMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAddRegionMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetAddRegionMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAddRegion",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetAddRegion();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfRegions",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfRegions();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
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

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkbisROIMeanCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkbisROIMean:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetImageROI\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetImageROI\n",NULL);
    Tcl_AppendResult(interp,"  GetCentroids\n",NULL);
    Tcl_AppendResult(interp,"  SetFileName\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFileName\n",NULL);
    Tcl_AppendResult(interp,"  SetFileName2\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFileName2\n",NULL);
    Tcl_AppendResult(interp,"  SetDoText\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDoTextMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDoTextMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetDoText\n",NULL);
    Tcl_AppendResult(interp,"  SetAddRegion\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAddRegionMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAddRegionMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAddRegion\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfRegions\n",NULL);
    Tcl_AppendResult(interp,"  SetImageMask\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetImageMask\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetImageROI" );
    Tcl_DStringAppendElement ( &dString, "GetImageROI" );
    Tcl_DStringAppendElement ( &dString, "GetCentroids" );
    Tcl_DStringAppendElement ( &dString, "SetFileName" );
    Tcl_DStringAppendElement ( &dString, "GetFileName" );
    Tcl_DStringAppendElement ( &dString, "SetFileName2" );
    Tcl_DStringAppendElement ( &dString, "GetFileName2" );
    Tcl_DStringAppendElement ( &dString, "SetDoText" );
    Tcl_DStringAppendElement ( &dString, "GetDoTextMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetDoTextMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetDoText" );
    Tcl_DStringAppendElement ( &dString, "SetAddRegion" );
    Tcl_DStringAppendElement ( &dString, "GetAddRegionMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetAddRegionMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetAddRegion" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfRegions" );
    Tcl_DStringAppendElement ( &dString, "SetImageMask" );
    Tcl_DStringAppendElement ( &dString, "GetImageMask" );
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
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkbisROIMean *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetImageROI */
    if ( strcmp ( argv[2], "SetImageROI" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetImageROI" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetImageROI */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetImageROI(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for SetImageROI */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageROI */
    if ( strcmp ( argv[2], "GetImageROI" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageROI" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageROI */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetImageROI();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetImageROI */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCentroids */
    if ( strcmp ( argv[2], "GetCentroids" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCentroids" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCentroids */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints *GetCentroids();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetCentroids */

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
    Tcl_DStringAppendElement ( &dString, "virtual void SetFileName(const char *filename);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
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
    Tcl_DStringAppendElement ( &dString, "virtual const char *GetFileName();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetFileName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFileName2 */
    if ( strcmp ( argv[2], "SetFileName2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFileName2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFileName2 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetFileName2(const char *filename2);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for SetFileName2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFileName2 */
    if ( strcmp ( argv[2], "GetFileName2" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFileName2" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFileName2 */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual const char *GetFileName2();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetFileName2 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDoText */
    if ( strcmp ( argv[2], "SetDoText" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDoText" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDoText */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetDoText(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for SetDoText */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDoTextMinValue */
    if ( strcmp ( argv[2], "GetDoTextMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDoTextMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDoTextMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetDoTextMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetDoTextMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDoTextMaxValue */
    if ( strcmp ( argv[2], "GetDoTextMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDoTextMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDoTextMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetDoTextMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetDoTextMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDoText */
    if ( strcmp ( argv[2], "GetDoText" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDoText" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDoText */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetDoText();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetDoText */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAddRegion */
    if ( strcmp ( argv[2], "SetAddRegion" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAddRegion" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAddRegion */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetAddRegion(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for SetAddRegion */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAddRegionMinValue */
    if ( strcmp ( argv[2], "GetAddRegionMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAddRegionMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAddRegionMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetAddRegionMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetAddRegionMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAddRegionMaxValue */
    if ( strcmp ( argv[2], "GetAddRegionMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAddRegionMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAddRegionMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetAddRegionMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetAddRegionMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAddRegion */
    if ( strcmp ( argv[2], "GetAddRegion" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAddRegion" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAddRegion */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetAddRegion();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetAddRegion */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfRegions */
    if ( strcmp ( argv[2], "GetNumberOfRegions" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfRegions" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfRegions */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfRegions();" );
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetNumberOfRegions */

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
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
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
    Tcl_DStringAppendElement ( &dString, "vtkbisROIMean" );
    /* Closing for GetImageMask */

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
