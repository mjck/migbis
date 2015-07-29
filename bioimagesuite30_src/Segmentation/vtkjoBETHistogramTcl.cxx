// tcl wrapper for vtkjoBETHistogram object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkjoBETHistogram.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkjoBETHistogramNewCommand()
{
  vtkjoBETHistogram *temp = vtkjoBETHistogram::New();
  return static_cast<ClientData>(temp);
}

int vtkSimpleImageToImageFilterCppCommand(vtkSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkjoBETHistogramCppCommand(vtkjoBETHistogram *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkjoBETHistogramCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkjoBETHistogramCppCommand(static_cast<vtkjoBETHistogram *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkjoBETHistogramCppCommand(vtkjoBETHistogram *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkjoBETHistogram",argv[1]))
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
    vtkjoBETHistogram  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkjoBETHistogram");
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
    vtkjoBETHistogram  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkjoBETHistogram");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkjoBETHistogram  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkjoBETHistogram");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetMinIntensityPercent",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMinIntensityPercent(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMinIntensityPercentMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinIntensityPercentMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinIntensityPercentMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinIntensityPercentMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinIntensityPercent",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinIntensityPercent();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMaxIntensityPercent",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMaxIntensityPercent(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaxIntensityPercentMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxIntensityPercentMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxIntensityPercentMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxIntensityPercentMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxIntensityPercent",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxIntensityPercent();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMinIntensity",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinIntensity();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMaxIntensity",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxIntensity();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetThreshold",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetThreshold();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetBins",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetBins(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetBinsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBinsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBinsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBinsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetBins",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetBins();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetCenterOfGravity",argv[1]))&&(argc == 2))
    {
    double  *temp20;
    temp20 = (op)->GetCenterOfGravity();
    if(temp20)
      {
      char tempResult[1024];
      *tempResult = '\0';
      char converted[1024];
      *converted = '\0';
      Tcl_PrintDouble(interp,temp20[0], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_PrintDouble(interp,temp20[1], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_PrintDouble(interp,temp20[2], converted);
      strcat(tempResult, " ");
      strcat(tempResult, converted);
      Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
      }
    else
      {
      Tcl_SetResult(interp, const_cast<char *>(""), TCL_VOLATILE);
      }
    return TCL_OK;
    }
  if ((!strcmp("GetRadius",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetRadius();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMeanIntensity",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMeanIntensity();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMedianIntensity",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMedianIntensity();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkjoBETHistogramCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkjoBETHistogram:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetMinIntensityPercent\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMinIntensityPercentMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMinIntensityPercentMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMinIntensityPercent\n",NULL);
    Tcl_AppendResult(interp,"  SetMaxIntensityPercent\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxIntensityPercentMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxIntensityPercentMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxIntensityPercent\n",NULL);
    Tcl_AppendResult(interp,"  GetMinIntensity\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxIntensity\n",NULL);
    Tcl_AppendResult(interp,"  GetThreshold\n",NULL);
    Tcl_AppendResult(interp,"  SetBins\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetBinsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBinsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetBins\n",NULL);
    Tcl_AppendResult(interp,"  GetCenterOfGravity\n",NULL);
    Tcl_AppendResult(interp,"  GetRadius\n",NULL);
    Tcl_AppendResult(interp,"  GetMeanIntensity\n",NULL);
    Tcl_AppendResult(interp,"  GetMedianIntensity\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetMinIntensityPercent" );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercentMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercentMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercent" );
    Tcl_DStringAppendElement ( &dString, "SetMaxIntensityPercent" );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercentMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercentMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercent" );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensity" );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensity" );
    Tcl_DStringAppendElement ( &dString, "GetThreshold" );
    Tcl_DStringAppendElement ( &dString, "SetBins" );
    Tcl_DStringAppendElement ( &dString, "GetBinsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetBinsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetBins" );
    Tcl_DStringAppendElement ( &dString, "GetCenterOfGravity" );
    Tcl_DStringAppendElement ( &dString, "GetRadius" );
    Tcl_DStringAppendElement ( &dString, "GetMeanIntensity" );
    Tcl_DStringAppendElement ( &dString, "GetMedianIntensity" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkjoBETHistogram *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMinIntensityPercent */
    if ( strcmp ( argv[2], "SetMinIntensityPercent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMinIntensityPercent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMinIntensityPercent */
    Tcl_DStringAppendElement ( &dString, " MinIntensityPercent - the lower percentage of the cumulative histogram\n used to determine MinIntensity.  We limit the minimum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.02.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMinIntensityPercent(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for SetMinIntensityPercent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinIntensityPercentMinValue */
    if ( strcmp ( argv[2], "GetMinIntensityPercentMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercentMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinIntensityPercentMinValue */
    Tcl_DStringAppendElement ( &dString, " MinIntensityPercent - the lower percentage of the cumulative histogram\n used to determine MinIntensity.  We limit the minimum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.02.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinIntensityPercentMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetMinIntensityPercentMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinIntensityPercentMaxValue */
    if ( strcmp ( argv[2], "GetMinIntensityPercentMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercentMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinIntensityPercentMaxValue */
    Tcl_DStringAppendElement ( &dString, " MinIntensityPercent - the lower percentage of the cumulative histogram\n used to determine MinIntensity.  We limit the minimum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.02.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinIntensityPercentMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetMinIntensityPercentMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinIntensityPercent */
    if ( strcmp ( argv[2], "GetMinIntensityPercent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensityPercent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinIntensityPercent */
    Tcl_DStringAppendElement ( &dString, " MinIntensityPercent - the lower percentage of the cumulative histogram\n used to determine MinIntensity.  We limit the minimum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.02.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinIntensityPercent();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetMinIntensityPercent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaxIntensityPercent */
    if ( strcmp ( argv[2], "SetMaxIntensityPercent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaxIntensityPercent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaxIntensityPercent */
    Tcl_DStringAppendElement ( &dString, " MaxIntensityPercent - the upper percentage of the cumulative histogram\n used to determine MaxIntensity.  We limit the maximum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.98.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaxIntensityPercent(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for SetMaxIntensityPercent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxIntensityPercentMinValue */
    if ( strcmp ( argv[2], "GetMaxIntensityPercentMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercentMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxIntensityPercentMinValue */
    Tcl_DStringAppendElement ( &dString, " MaxIntensityPercent - the upper percentage of the cumulative histogram\n used to determine MaxIntensity.  We limit the maximum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.98.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxIntensityPercentMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetMaxIntensityPercentMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxIntensityPercentMaxValue */
    if ( strcmp ( argv[2], "GetMaxIntensityPercentMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercentMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxIntensityPercentMaxValue */
    Tcl_DStringAppendElement ( &dString, " MaxIntensityPercent - the upper percentage of the cumulative histogram\n used to determine MaxIntensity.  We limit the maximum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.98.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxIntensityPercentMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetMaxIntensityPercentMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxIntensityPercent */
    if ( strcmp ( argv[2], "GetMaxIntensityPercent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensityPercent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxIntensityPercent */
    Tcl_DStringAppendElement ( &dString, " MaxIntensityPercent - the upper percentage of the cumulative histogram\n used to determine MaxIntensity.  We limit the maximum intensity\n percentage to be between 0.0 and 1.0.  The default is 0.98.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxIntensityPercent();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetMaxIntensityPercent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinIntensity */
    if ( strcmp ( argv[2], "GetMinIntensity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinIntensity */
    Tcl_DStringAppendElement ( &dString, " MinIntensity - the robust minimum intensity below which lies\n MinIntensityPercent percent of the cumulative histogram.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinIntensity();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetMinIntensity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxIntensity */
    if ( strcmp ( argv[2], "GetMaxIntensity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxIntensity */
    Tcl_DStringAppendElement ( &dString, " MaxIntensity - the robust maximum intensity above which lies\n MaxIntensityPercent percent of the cumulative histogram.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxIntensity();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetMaxIntensity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetThreshold */
    if ( strcmp ( argv[2], "GetThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetThreshold */
    Tcl_DStringAppendElement ( &dString, " Threshold - set to be 10% of the way between MinIntensity\n and MaxIntensity.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetThreshold();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetBins */
    if ( strcmp ( argv[2], "SetBins" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetBins" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetBins */
    Tcl_DStringAppendElement ( &dString, " Bins - the number of histogram bins.  We limit the number of bins to be\n between 1 and 512.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetBins(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for SetBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBinsMinValue */
    if ( strcmp ( argv[2], "GetBinsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBinsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBinsMinValue */
    Tcl_DStringAppendElement ( &dString, " Bins - the number of histogram bins.  We limit the number of bins to be\n between 1 and 512.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBinsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetBinsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBinsMaxValue */
    if ( strcmp ( argv[2], "GetBinsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBinsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBinsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Bins - the number of histogram bins.  We limit the number of bins to be\n between 1 and 512.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBinsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetBinsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetBins */
    if ( strcmp ( argv[2], "GetBins" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetBins" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetBins */
    Tcl_DStringAppendElement ( &dString, " Bins - the number of histogram bins.  We limit the number of bins to be\n between 1 and 512.\n" );
    Tcl_DStringAppendElement ( &dString, "int GetBins();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetBins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCenterOfGravity */
    if ( strcmp ( argv[2], "GetCenterOfGravity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCenterOfGravity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCenterOfGravity */
    Tcl_DStringAppendElement ( &dString, " Center Of Gravity - the center of gravity (COG) of the brain/head in the\n image.  Only voxels with intensity greater than Threshold and less than\n MaxIntensity are used to compute this.\n" );
    Tcl_DStringAppendElement ( &dString, "double *GetCenterOfGravity();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetCenterOfGravity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRadius */
    if ( strcmp ( argv[2], "GetRadius" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRadius" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRadius */
    Tcl_DStringAppendElement ( &dString, " Radius - the estimated \"mean' radius of the brain/head volume.  All voxels\n with intensity greater than Threshold are counted.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetRadius();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetRadius */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMeanIntensity */
    if ( strcmp ( argv[2], "GetMeanIntensity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMeanIntensity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMeanIntensity */
    Tcl_DStringAppendElement ( &dString, " MeanIntensity - the mean intensity within a sphere of the estimated\n radius and centered on the COG.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMeanIntensity();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetMeanIntensity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMedianIntensity */
    if ( strcmp ( argv[2], "GetMedianIntensity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMedianIntensity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMedianIntensity */
    Tcl_DStringAppendElement ( &dString, " MedianIntensity - the median intensity within a sphere of the estimated\n radius and centered on the COG.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMedianIntensity();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoBETHistogram" );
    /* Closing for GetMedianIntensity */

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
