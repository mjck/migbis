// tcl wrapper for vtkmpjLevelSetNeighborhoodExtractor object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkmpjLevelSetNeighborhoodExtractor.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkmpjLevelSetNeighborhoodExtractorNewCommand()
{
  vtkmpjLevelSetNeighborhoodExtractor *temp = vtkmpjLevelSetNeighborhoodExtractor::New();
  return static_cast<ClientData>(temp);
}

int vtkpxSimpleImageToImageFilterCppCommand(vtkpxSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkmpjLevelSetNeighborhoodExtractorCppCommand(vtkmpjLevelSetNeighborhoodExtractor *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkmpjLevelSetNeighborhoodExtractorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkmpjLevelSetNeighborhoodExtractorCppCommand(static_cast<vtkmpjLevelSetNeighborhoodExtractor *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkmpjLevelSetNeighborhoodExtractorCppCommand(vtkmpjLevelSetNeighborhoodExtractor *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkmpjLevelSetNeighborhoodExtractor",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxSimpleImageToImageFilterCppCommand(static_cast<vtkpxSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxSimpleImageToImageFilter"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkmpjLevelSetNeighborhoodExtractor  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjLevelSetNeighborhoodExtractor");
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
    vtkmpjLevelSetNeighborhoodExtractor  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjLevelSetNeighborhoodExtractor");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkmpjLevelSetNeighborhoodExtractor  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjLevelSetNeighborhoodExtractor");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetInputLevelSet",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetInputLevelSet(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetLevelSetValue",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLevelSetValue(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLevelSetValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLevelSetValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNarrowBandwidth",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetNarrowBandwidth(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNarrowBandwidth",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetNarrowBandwidth();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNarrowBanding",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNarrowBanding();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNarrowBanding",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNarrowBanding(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("NarrowBandingOn",argv[1]))&&(argc == 2))
    {
    op->NarrowBandingOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("NarrowBandingOff",argv[1]))&&(argc == 2))
    {
    op->NarrowBandingOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetInputNarrowBand",argv[1]))&&(argc == 3))
    {
    vtkDataArray  *temp0;
    error = 0;

    temp0 = (vtkDataArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDataArray"),interp,error));
    if (!error)
    {
    op->SetInputNarrowBand(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInputNarrowBand",argv[1]))&&(argc == 2))
    {
    vtkDataArray  *temp20;
    temp20 = (op)->GetInputNarrowBand();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDataArray");
    return TCL_OK;
    }
  if ((!strcmp("GetInsidePoints",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetInsidePoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
    return TCL_OK;
    }
  if ((!strcmp("GetOutsidePoints",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetOutsidePoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
    return TCL_OK;
    }
  if ((!strcmp("SetLargeValue",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLargeValue(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLargeValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLargeValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("Locate",argv[1]))&&(argc == 2))
    {
    op->Locate();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkmpjLevelSetNeighborhoodExtractorCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkmpjLevelSetNeighborhoodExtractor:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetInputLevelSet\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetLevelSetValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLevelSetValue\n",NULL);
    Tcl_AppendResult(interp,"  SetNarrowBandwidth\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNarrowBandwidth\n",NULL);
    Tcl_AppendResult(interp,"  GetNarrowBanding\n",NULL);
    Tcl_AppendResult(interp,"  SetNarrowBanding\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NarrowBandingOn\n",NULL);
    Tcl_AppendResult(interp,"  NarrowBandingOff\n",NULL);
    Tcl_AppendResult(interp,"  SetInputNarrowBand\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInputNarrowBand\n",NULL);
    Tcl_AppendResult(interp,"  GetInsidePoints\n",NULL);
    Tcl_AppendResult(interp,"  GetOutsidePoints\n",NULL);
    Tcl_AppendResult(interp,"  SetLargeValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLargeValue\n",NULL);
    Tcl_AppendResult(interp,"  Locate\n",NULL);
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
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetInputLevelSet" );
    Tcl_DStringAppendElement ( &dString, "SetLevelSetValue" );
    Tcl_DStringAppendElement ( &dString, "GetLevelSetValue" );
    Tcl_DStringAppendElement ( &dString, "SetNarrowBandwidth" );
    Tcl_DStringAppendElement ( &dString, "GetNarrowBandwidth" );
    Tcl_DStringAppendElement ( &dString, "GetNarrowBanding" );
    Tcl_DStringAppendElement ( &dString, "SetNarrowBanding" );
    Tcl_DStringAppendElement ( &dString, "NarrowBandingOn" );
    Tcl_DStringAppendElement ( &dString, "NarrowBandingOff" );
    Tcl_DStringAppendElement ( &dString, "SetInputNarrowBand" );
    Tcl_DStringAppendElement ( &dString, "GetInputNarrowBand" );
    Tcl_DStringAppendElement ( &dString, "GetInsidePoints" );
    Tcl_DStringAppendElement ( &dString, "GetOutsidePoints" );
    Tcl_DStringAppendElement ( &dString, "SetLargeValue" );
    Tcl_DStringAppendElement ( &dString, "GetLargeValue" );
    Tcl_DStringAppendElement ( &dString, "Locate" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkmpjLevelSetNeighborhoodExtractor *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInputLevelSet */
    if ( strcmp ( argv[2], "SetInputLevelSet" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInputLevelSet" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInputLevelSet */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetInputLevelSet(vtkImageData *ptr);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for SetInputLevelSet */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLevelSetValue */
    if ( strcmp ( argv[2], "SetLevelSetValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLevelSetValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLevelSetValue */
    Tcl_DStringAppendElement ( &dString, " Set/Get Set the value of the level set to be located. Default is 0.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetLevelSetValue(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for SetLevelSetValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLevelSetValue */
    if ( strcmp ( argv[2], "GetLevelSetValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLevelSetValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLevelSetValue */
    Tcl_DStringAppendElement ( &dString, " Set/Get Set the value of the level set to be located. Default is 0.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLevelSetValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for GetLevelSetValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNarrowBandwidth */
    if ( strcmp ( argv[2], "SetNarrowBandwidth" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNarrowBandwidth" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNarrowBandwidth */
    Tcl_DStringAppendElement ( &dString, " Set/Get the narrow band width. Default is 12.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNarrowBandwidth(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for SetNarrowBandwidth */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNarrowBandwidth */
    if ( strcmp ( argv[2], "GetNarrowBandwidth" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNarrowBandwidth" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNarrowBandwidth */
    Tcl_DStringAppendElement ( &dString, " Set/Get the narrow band width. Default is 12.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetNarrowBandwidth();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for GetNarrowBandwidth */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNarrowBanding */
    if ( strcmp ( argv[2], "GetNarrowBanding" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNarrowBanding" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNarrowBanding */
    Tcl_DStringAppendElement ( &dString, " Set/Get the narrowbanding flag. \n" );
    Tcl_DStringAppendElement ( &dString, "int GetNarrowBanding();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for GetNarrowBanding */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNarrowBanding */
    if ( strcmp ( argv[2], "SetNarrowBanding" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNarrowBanding" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNarrowBanding */
    Tcl_DStringAppendElement ( &dString, " Set/Get the narrowbanding flag. \n" );
    Tcl_DStringAppendElement ( &dString, "void SetNarrowBanding(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for SetNarrowBanding */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NarrowBandingOn */
    if ( strcmp ( argv[2], "NarrowBandingOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NarrowBandingOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NarrowBandingOn */
    Tcl_DStringAppendElement ( &dString, " Set/Get the narrowbanding flag. \n" );
    Tcl_DStringAppendElement ( &dString, "void NarrowBandingOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for NarrowBandingOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NarrowBandingOff */
    if ( strcmp ( argv[2], "NarrowBandingOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NarrowBandingOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NarrowBandingOff */
    Tcl_DStringAppendElement ( &dString, " Set/Get the narrowbanding flag. \n" );
    Tcl_DStringAppendElement ( &dString, "void NarrowBandingOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for NarrowBandingOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInputNarrowBand */
    if ( strcmp ( argv[2], "SetInputNarrowBand" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInputNarrowBand" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDataArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInputNarrowBand */
    Tcl_DStringAppendElement ( &dString, " Set/Get the input narrowband. \n" );
    Tcl_DStringAppendElement ( &dString, "void SetInputNarrowBand(vtkDataArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for SetInputNarrowBand */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInputNarrowBand */
    if ( strcmp ( argv[2], "GetInputNarrowBand" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInputNarrowBand" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInputNarrowBand */
    Tcl_DStringAppendElement ( &dString, " Set/Get the input narrowband. \n" );
    Tcl_DStringAppendElement ( &dString, "vtkDataArray *GetInputNarrowBand();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for GetInputNarrowBand */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInsidePoints */
    if ( strcmp ( argv[2], "GetInsidePoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInsidePoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInsidePoints */
    Tcl_DStringAppendElement ( &dString, " Get the container of inside points.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetInsidePoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for GetInsidePoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutsidePoints */
    if ( strcmp ( argv[2], "GetOutsidePoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutsidePoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutsidePoints */
    Tcl_DStringAppendElement ( &dString, " Get the container of outside points.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetOutsidePoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for GetOutsidePoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLargeValue */
    if ( strcmp ( argv[2], "SetLargeValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLargeValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLargeValue */
    Tcl_DStringAppendElement ( &dString, " Set/Get Set value for infinity\n" );
    Tcl_DStringAppendElement ( &dString, "void SetLargeValue(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for SetLargeValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLargeValue */
    if ( strcmp ( argv[2], "GetLargeValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLargeValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLargeValue */
    Tcl_DStringAppendElement ( &dString, " Set/Get Set value for infinity\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLargeValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for GetLargeValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Locate */
    if ( strcmp ( argv[2], "Locate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Locate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Locate */
    Tcl_DStringAppendElement ( &dString, " Locate the level set. This method evokes the level set\n location algorithm\n" );
    Tcl_DStringAppendElement ( &dString, "void Locate();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLevelSetNeighborhoodExtractor" );
    /* Closing for Locate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxSimpleImageToImageFilterCppCommand(static_cast<vtkpxSimpleImageToImageFilter *>(op),interp,argc,argv) == TCL_OK)
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
