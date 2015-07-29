// tcl wrapper for vtkokKochCoefficientSource object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkokKochCoefficientSource.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkokKochCoefficientSourceNewCommand()
{
  vtkokKochCoefficientSource *temp = vtkokKochCoefficientSource::New();
  return static_cast<ClientData>(temp);
}

int vtkSimpleImageToImageFilterCppCommand(vtkSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkokKochCoefficientSourceCppCommand(vtkokKochCoefficientSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkokKochCoefficientSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkokKochCoefficientSourceCppCommand(static_cast<vtkokKochCoefficientSource *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkokKochCoefficientSourceCppCommand(vtkokKochCoefficientSource *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkokKochCoefficientSource",argv[1]))
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
    vtkokKochCoefficientSource  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkokKochCoefficientSource");
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
    vtkokKochCoefficientSource  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkokKochCoefficientSource");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkokKochCoefficientSource  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkokKochCoefficientSource");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetImageFFT",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetImageFFT();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetImageCoefficient",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetImageCoefficient();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetImageMultiplied",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetImageMultiplied();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetImageRFFT",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetImageRFFT();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetInducedField",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetInducedField();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetCoefficientMap",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetCoefficientMap();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetScaleFactorOfDisplacement",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetScaleFactorOfDisplacement(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetB0",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetB0(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetB0",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetB0();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOutputScalarType",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetOutputScalarType(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutputScalarType",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetOutputScalarType();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMeanDisplacementToZero",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMeanDisplacementToZero(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMeanDisplacementToZero",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMeanDisplacementToZero();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMeanDisplacementToZeroOn",argv[1]))&&(argc == 2))
    {
    op->SetMeanDisplacementToZeroOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMeanDisplacementToZeroOff",argv[1]))&&(argc == 2))
    {
    op->SetMeanDisplacementToZeroOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOutputScalarTypeToFloat",argv[1]))&&(argc == 2))
    {
    op->SetOutputScalarTypeToFloat();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetOutputScalarTypeToDouble",argv[1]))&&(argc == 2))
    {
    op->SetOutputScalarTypeToDouble();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetPadding",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetPadding(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPaddingMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPaddingMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPaddingMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPaddingMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPadding",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPadding();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkokKochCoefficientSourceCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkokKochCoefficientSource:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetImageFFT\n",NULL);
    Tcl_AppendResult(interp,"  GetImageCoefficient\n",NULL);
    Tcl_AppendResult(interp,"  GetImageMultiplied\n",NULL);
    Tcl_AppendResult(interp,"  GetImageRFFT\n",NULL);
    Tcl_AppendResult(interp,"  GetInducedField\n",NULL);
    Tcl_AppendResult(interp,"  GetCoefficientMap\n",NULL);
    Tcl_AppendResult(interp,"  SetScaleFactorOfDisplacement\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetB0\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetB0\n",NULL);
    Tcl_AppendResult(interp,"  SetOutputScalarType\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutputScalarType\n",NULL);
    Tcl_AppendResult(interp,"  SetMeanDisplacementToZero\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMeanDisplacementToZero\n",NULL);
    Tcl_AppendResult(interp,"  SetMeanDisplacementToZeroOn\n",NULL);
    Tcl_AppendResult(interp,"  SetMeanDisplacementToZeroOff\n",NULL);
    Tcl_AppendResult(interp,"  SetOutputScalarTypeToFloat\n",NULL);
    Tcl_AppendResult(interp,"  SetOutputScalarTypeToDouble\n",NULL);
    Tcl_AppendResult(interp,"  SetPadding\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPaddingMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPaddingMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPadding\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "GetImageFFT" );
    Tcl_DStringAppendElement ( &dString, "GetImageCoefficient" );
    Tcl_DStringAppendElement ( &dString, "GetImageMultiplied" );
    Tcl_DStringAppendElement ( &dString, "GetImageRFFT" );
    Tcl_DStringAppendElement ( &dString, "GetInducedField" );
    Tcl_DStringAppendElement ( &dString, "GetCoefficientMap" );
    Tcl_DStringAppendElement ( &dString, "SetScaleFactorOfDisplacement" );
    Tcl_DStringAppendElement ( &dString, "SetB0" );
    Tcl_DStringAppendElement ( &dString, "GetB0" );
    Tcl_DStringAppendElement ( &dString, "SetOutputScalarType" );
    Tcl_DStringAppendElement ( &dString, "GetOutputScalarType" );
    Tcl_DStringAppendElement ( &dString, "SetMeanDisplacementToZero" );
    Tcl_DStringAppendElement ( &dString, "GetMeanDisplacementToZero" );
    Tcl_DStringAppendElement ( &dString, "SetMeanDisplacementToZeroOn" );
    Tcl_DStringAppendElement ( &dString, "SetMeanDisplacementToZeroOff" );
    Tcl_DStringAppendElement ( &dString, "SetOutputScalarTypeToFloat" );
    Tcl_DStringAppendElement ( &dString, "SetOutputScalarTypeToDouble" );
    Tcl_DStringAppendElement ( &dString, "SetPadding" );
    Tcl_DStringAppendElement ( &dString, "GetPaddingMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetPaddingMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetPadding" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkokKochCoefficientSource *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageFFT */
    if ( strcmp ( argv[2], "GetImageFFT" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageFFT" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageFFT */
    Tcl_DStringAppendElement ( &dString, " Chi(kx,ky,kz),fourier transform of the susceptibility field\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetImageFFT();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetImageFFT */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageCoefficient */
    if ( strcmp ( argv[2], "GetImageCoefficient" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageCoefficient" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageCoefficient */
    Tcl_DStringAppendElement ( &dString, " coefficient image Ck(kx,ky,kz) in the fourier domain\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetImageCoefficient();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetImageCoefficient */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageMultiplied */
    if ( strcmp ( argv[2], "GetImageMultiplied" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageMultiplied" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageMultiplied */
    Tcl_DStringAppendElement ( &dString, " Chi(kx,ky,kz) * Ck(kx,ky,kz)\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetImageMultiplied();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetImageMultiplied */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageRFFT */
    if ( strcmp ( argv[2], "GetImageRFFT" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageRFFT" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageRFFT */
    Tcl_DStringAppendElement ( &dString, "Inverse transform of Ck(kx,ky,kz) * Chi(kx,ky,kz)\nwhich equals B_dz(x,y,z)*B0^(-1)\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetImageRFFT();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetImageRFFT */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInducedField */
    if ( strcmp ( argv[2], "GetInducedField" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInducedField" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInducedField */
    Tcl_DStringAppendElement ( &dString, "induced field B_dz(x,y,z)\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetInducedField();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetInducedField */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCoefficientMap */
    if ( strcmp ( argv[2], "GetCoefficientMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCoefficientMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCoefficientMap */
    Tcl_DStringAppendElement ( &dString, " displacement field\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetCoefficientMap();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetCoefficientMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetScaleFactorOfDisplacement */
    if ( strcmp ( argv[2], "SetScaleFactorOfDisplacement" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetScaleFactorOfDisplacement" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetScaleFactorOfDisplacement */
    Tcl_DStringAppendElement ( &dString, " scaling factor to convert from induced field to displacement field\n" );
    Tcl_DStringAppendElement ( &dString, "void SetScaleFactorOfDisplacement(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for SetScaleFactorOfDisplacement */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetB0 */
    if ( strcmp ( argv[2], "SetB0" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetB0" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetB0 */
    Tcl_DStringAppendElement ( &dString, "static magnetic field\n" );
    Tcl_DStringAppendElement ( &dString, "void SetB0(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for SetB0 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetB0 */
    if ( strcmp ( argv[2], "GetB0" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetB0" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetB0 */
    Tcl_DStringAppendElement ( &dString, "static magnetic field\n" );
    Tcl_DStringAppendElement ( &dString, "float GetB0();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetB0 */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutputScalarType */
    if ( strcmp ( argv[2], "SetOutputScalarType" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutputScalarType" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutputScalarType */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOutputScalarType(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for SetOutputScalarType */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutputScalarType */
    if ( strcmp ( argv[2], "GetOutputScalarType" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutputScalarType" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutputScalarType */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetOutputScalarType();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetOutputScalarType */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMeanDisplacementToZero */
    if ( strcmp ( argv[2], "SetMeanDisplacementToZero" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMeanDisplacementToZero" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMeanDisplacementToZero */
    Tcl_DStringAppendElement ( &dString, " If set to 1, average displacement in the BrainVOI will \n be caribrated as zero\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMeanDisplacementToZero(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for SetMeanDisplacementToZero */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMeanDisplacementToZero */
    if ( strcmp ( argv[2], "GetMeanDisplacementToZero" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMeanDisplacementToZero" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMeanDisplacementToZero */
    Tcl_DStringAppendElement ( &dString, " If set to 1, average displacement in the BrainVOI will \n be caribrated as zero\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMeanDisplacementToZero();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetMeanDisplacementToZero */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMeanDisplacementToZeroOn */
    if ( strcmp ( argv[2], "SetMeanDisplacementToZeroOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMeanDisplacementToZeroOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMeanDisplacementToZeroOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetMeanDisplacementToZeroOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for SetMeanDisplacementToZeroOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMeanDisplacementToZeroOff */
    if ( strcmp ( argv[2], "SetMeanDisplacementToZeroOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMeanDisplacementToZeroOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMeanDisplacementToZeroOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetMeanDisplacementToZeroOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for SetMeanDisplacementToZeroOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutputScalarTypeToFloat */
    if ( strcmp ( argv[2], "SetOutputScalarTypeToFloat" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutputScalarTypeToFloat" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutputScalarTypeToFloat */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOutputScalarTypeToFloat();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for SetOutputScalarTypeToFloat */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutputScalarTypeToDouble */
    if ( strcmp ( argv[2], "SetOutputScalarTypeToDouble" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutputScalarTypeToDouble" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutputScalarTypeToDouble */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOutputScalarTypeToDouble();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for SetOutputScalarTypeToDouble */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPadding */
    if ( strcmp ( argv[2], "SetPadding" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPadding" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPadding */
    Tcl_DStringAppendElement ( &dString, " Padding Width --> add voxels of air around image to improve accuracy\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPadding(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for SetPadding */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPaddingMinValue */
    if ( strcmp ( argv[2], "GetPaddingMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPaddingMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPaddingMinValue */
    Tcl_DStringAppendElement ( &dString, " Padding Width --> add voxels of air around image to improve accuracy\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPaddingMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetPaddingMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPaddingMaxValue */
    if ( strcmp ( argv[2], "GetPaddingMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPaddingMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPaddingMaxValue */
    Tcl_DStringAppendElement ( &dString, " Padding Width --> add voxels of air around image to improve accuracy\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPaddingMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetPaddingMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPadding */
    if ( strcmp ( argv[2], "GetPadding" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPadding" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPadding */
    Tcl_DStringAppendElement ( &dString, " Padding Width --> add voxels of air around image to improve accuracy\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPadding();" );
    Tcl_DStringAppendElement ( &dString, "vtkokKochCoefficientSource" );
    /* Closing for GetPadding */

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
