// tcl wrapper for vtkmpjLaxConnectivity object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkmpjLaxConnectivity.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkmpjLaxConnectivityNewCommand()
{
  vtkmpjLaxConnectivity *temp = vtkmpjLaxConnectivity::New();
  return static_cast<ClientData>(temp);
}

int vtkpxSimpleImageToImageFilterCppCommand(vtkpxSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkmpjLaxConnectivityCppCommand(vtkmpjLaxConnectivity *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkmpjLaxConnectivityCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkmpjLaxConnectivityCppCommand(static_cast<vtkmpjLaxConnectivity *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkmpjLaxConnectivityCppCommand(vtkmpjLaxConnectivity *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkmpjLaxConnectivity",argv[1]))
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
    vtkmpjLaxConnectivity  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjLaxConnectivity");
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
    vtkmpjLaxConnectivity  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjLaxConnectivity");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkmpjLaxConnectivity  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjLaxConnectivity");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetKnownPoints",argv[1]))&&(argc == 3))
    {
    vtkFloatArray  *temp0;
    error = 0;

    temp0 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkFloatArray"),interp,error));
    if (!error)
    {
    op->SetKnownPoints(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetKnownPoints",argv[1]))&&(argc == 2))
    {
    vtkFloatArray  *temp20;
    temp20 = (op)->GetKnownPoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfIterations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfIterations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNumberOfIterations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNumberOfIterations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLabelImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetLabelImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetLabelImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetLabelImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetSolution",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetSolution(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetConvergenceImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetConvergenceImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetSymmetricTensor",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetSymmetricTensor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSymmetricTensor",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSymmetricTensor();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SymmetricTensorOn",argv[1]))&&(argc == 2))
    {
    op->SymmetricTensorOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SymmetricTensorOff",argv[1]))&&(argc == 2))
    {
    op->SymmetricTensorOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetStopOnMaximumError",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetStopOnMaximumError(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetStopOnMaximumError",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetStopOnMaximumError();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("StopOnMaximumErrorOn",argv[1]))&&(argc == 2))
    {
    op->StopOnMaximumErrorOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("StopOnMaximumErrorOff",argv[1]))&&(argc == 2))
    {
    op->StopOnMaximumErrorOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetMaximumError",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetMaximumError();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMaximumError",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMaximumError(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetErrorValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetErrorValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetErrorValue",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetErrorValue(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetIterationNumber",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetIterationNumber();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetIterationNumber",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetIterationNumber(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSpacing",argv[1]))&&(argc == 2))
    {
    float   *temp20;
    temp20 = (op)->GetSpacing();
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
  if ((!strcmp("SetSpacing",argv[1]))&&(argc == 5))
    {
    float    temp0;
    float    temp1;
    float    temp2;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->SetSpacing(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetSpacing",argv[1]))&&(argc == 5))
    {
    float  temp0[3];
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0[0] = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp0[1] = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp0[2] = tempd;
    if (!error)
    {
    op->SetSpacing(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInfinity",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInfinity();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetInfinity",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetInfinity(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkmpjLaxConnectivityCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkmpjLaxConnectivity:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetKnownPoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetKnownPoints\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfIterations\n",NULL);
    Tcl_AppendResult(interp,"  SetNumberOfIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLabelImage\n",NULL);
    Tcl_AppendResult(interp,"  SetLabelImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetSolution\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetConvergenceImage\n",NULL);
    Tcl_AppendResult(interp,"  SetSymmetricTensor\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSymmetricTensor\n",NULL);
    Tcl_AppendResult(interp,"  SymmetricTensorOn\n",NULL);
    Tcl_AppendResult(interp,"  SymmetricTensorOff\n",NULL);
    Tcl_AppendResult(interp,"  SetStopOnMaximumError\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetStopOnMaximumError\n",NULL);
    Tcl_AppendResult(interp,"  StopOnMaximumErrorOn\n",NULL);
    Tcl_AppendResult(interp,"  StopOnMaximumErrorOff\n",NULL);
    Tcl_AppendResult(interp,"  GetMaximumError\n",NULL);
    Tcl_AppendResult(interp,"  SetMaximumError\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetErrorValue\n",NULL);
    Tcl_AppendResult(interp,"  SetErrorValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetIterationNumber\n",NULL);
    Tcl_AppendResult(interp,"  SetIterationNumber\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSpacing\n",NULL);
    Tcl_AppendResult(interp,"  SetSpacing\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  SetSpacing\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetInfinity\n",NULL);
    Tcl_AppendResult(interp,"  SetInfinity\t with 1 arg\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetKnownPoints" );
    Tcl_DStringAppendElement ( &dString, "GetKnownPoints" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfIterations" );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfIterations" );
    Tcl_DStringAppendElement ( &dString, "GetLabelImage" );
    Tcl_DStringAppendElement ( &dString, "SetLabelImage" );
    Tcl_DStringAppendElement ( &dString, "SetSolution" );
    Tcl_DStringAppendElement ( &dString, "GetConvergenceImage" );
    Tcl_DStringAppendElement ( &dString, "SetSymmetricTensor" );
    Tcl_DStringAppendElement ( &dString, "GetSymmetricTensor" );
    Tcl_DStringAppendElement ( &dString, "SymmetricTensorOn" );
    Tcl_DStringAppendElement ( &dString, "SymmetricTensorOff" );
    Tcl_DStringAppendElement ( &dString, "SetStopOnMaximumError" );
    Tcl_DStringAppendElement ( &dString, "GetStopOnMaximumError" );
    Tcl_DStringAppendElement ( &dString, "StopOnMaximumErrorOn" );
    Tcl_DStringAppendElement ( &dString, "StopOnMaximumErrorOff" );
    Tcl_DStringAppendElement ( &dString, "GetMaximumError" );
    Tcl_DStringAppendElement ( &dString, "SetMaximumError" );
    Tcl_DStringAppendElement ( &dString, "GetErrorValue" );
    Tcl_DStringAppendElement ( &dString, "SetErrorValue" );
    Tcl_DStringAppendElement ( &dString, "GetIterationNumber" );
    Tcl_DStringAppendElement ( &dString, "SetIterationNumber" );
    Tcl_DStringAppendElement ( &dString, "GetSpacing" );
    Tcl_DStringAppendElement ( &dString, "SetSpacing" );
    Tcl_DStringAppendElement ( &dString, "SetSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetInfinity" );
    Tcl_DStringAppendElement ( &dString, "SetInfinity" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkmpjLaxConnectivity *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetKnownPoints */
    if ( strcmp ( argv[2], "SetKnownPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetKnownPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetKnownPoints */
    Tcl_DStringAppendElement ( &dString, " Set/Get boundary conditions for the propagation. You can also\n set the boundary points in the label image. See below.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetKnownPoints(vtkFloatArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetKnownPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetKnownPoints */
    if ( strcmp ( argv[2], "GetKnownPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetKnownPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetKnownPoints */
    Tcl_DStringAppendElement ( &dString, " Set/Get boundary conditions for the propagation. You can also\n set the boundary points in the label image. See below.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray *GetKnownPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetKnownPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfIterations */
    if ( strcmp ( argv[2], "GetNumberOfIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfIterations */
    Tcl_DStringAppendElement ( &dString, " Get/Set total number of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNumberOfIterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetNumberOfIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNumberOfIterations */
    if ( strcmp ( argv[2], "SetNumberOfIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNumberOfIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNumberOfIterations */
    Tcl_DStringAppendElement ( &dString, " Get/Set total number of Iterations\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNumberOfIterations(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetNumberOfIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLabelImage */
    if ( strcmp ( argv[2], "GetLabelImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLabelImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLabelImage */
    Tcl_DStringAppendElement ( &dString, " Set/Get the label image. If KnownPoints is not set, then\n LabelImage must have values >0 for initialized (boundary) points,\n and 0 for points on the grid to be calculated. \n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetLabelImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetLabelImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLabelImage */
    if ( strcmp ( argv[2], "SetLabelImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLabelImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLabelImage */
    Tcl_DStringAppendElement ( &dString, " Set/Get the label image. If KnownPoints is not set, then\n LabelImage must have values >0 for initialized (boundary) points,\n and 0 for points on the grid to be calculated. \n" );
    Tcl_DStringAppendElement ( &dString, "void SetLabelImage(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetLabelImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSolution */
    if ( strcmp ( argv[2], "SetSolution" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSolution" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSolution */
    Tcl_DStringAppendElement ( &dString, " Set initial solution for boundary points.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSolution(vtkImageData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetSolution */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetConvergenceImage */
    if ( strcmp ( argv[2], "GetConvergenceImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetConvergenceImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetConvergenceImage */
    Tcl_DStringAppendElement ( &dString, " Get convergence image\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetConvergenceImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetConvergenceImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSymmetricTensor */
    if ( strcmp ( argv[2], "SetSymmetricTensor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSymmetricTensor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSymmetricTensor */
    Tcl_DStringAppendElement ( &dString, " Set/Get tensor symmetry flag\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSymmetricTensor(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetSymmetricTensor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSymmetricTensor */
    if ( strcmp ( argv[2], "GetSymmetricTensor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSymmetricTensor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSymmetricTensor */
    Tcl_DStringAppendElement ( &dString, " Set/Get tensor symmetry flag\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSymmetricTensor();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetSymmetricTensor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SymmetricTensorOn */
    if ( strcmp ( argv[2], "SymmetricTensorOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SymmetricTensorOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SymmetricTensorOn */
    Tcl_DStringAppendElement ( &dString, " Set/Get tensor symmetry flag\n" );
    Tcl_DStringAppendElement ( &dString, "void SymmetricTensorOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SymmetricTensorOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SymmetricTensorOff */
    if ( strcmp ( argv[2], "SymmetricTensorOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SymmetricTensorOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SymmetricTensorOff */
    Tcl_DStringAppendElement ( &dString, " Set/Get tensor symmetry flag\n" );
    Tcl_DStringAppendElement ( &dString, "void SymmetricTensorOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SymmetricTensorOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetStopOnMaximumError */
    if ( strcmp ( argv[2], "SetStopOnMaximumError" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetStopOnMaximumError" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetStopOnMaximumError */
    Tcl_DStringAppendElement ( &dString, " Enables/disables stopping at maximum error\n" );
    Tcl_DStringAppendElement ( &dString, "void SetStopOnMaximumError(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetStopOnMaximumError */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetStopOnMaximumError */
    if ( strcmp ( argv[2], "GetStopOnMaximumError" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetStopOnMaximumError" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetStopOnMaximumError */
    Tcl_DStringAppendElement ( &dString, " Enables/disables stopping at maximum error\n" );
    Tcl_DStringAppendElement ( &dString, "int GetStopOnMaximumError();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetStopOnMaximumError */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: StopOnMaximumErrorOn */
    if ( strcmp ( argv[2], "StopOnMaximumErrorOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "StopOnMaximumErrorOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for StopOnMaximumErrorOn */
    Tcl_DStringAppendElement ( &dString, " Enables/disables stopping at maximum error\n" );
    Tcl_DStringAppendElement ( &dString, "void StopOnMaximumErrorOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for StopOnMaximumErrorOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: StopOnMaximumErrorOff */
    if ( strcmp ( argv[2], "StopOnMaximumErrorOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "StopOnMaximumErrorOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for StopOnMaximumErrorOff */
    Tcl_DStringAppendElement ( &dString, " Enables/disables stopping at maximum error\n" );
    Tcl_DStringAppendElement ( &dString, "void StopOnMaximumErrorOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for StopOnMaximumErrorOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaximumError */
    if ( strcmp ( argv[2], "GetMaximumError" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaximumError" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaximumError */
    Tcl_DStringAppendElement ( &dString, " Get/Set maximum error value\n" );
    Tcl_DStringAppendElement ( &dString, "float GetMaximumError();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetMaximumError */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaximumError */
    if ( strcmp ( argv[2], "SetMaximumError" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaximumError" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaximumError */
    Tcl_DStringAppendElement ( &dString, " Get/Set maximum error value\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaximumError(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetMaximumError */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetErrorValue */
    if ( strcmp ( argv[2], "GetErrorValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetErrorValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetErrorValue */
    Tcl_DStringAppendElement ( &dString, " Get/Set error value\n" );
    Tcl_DStringAppendElement ( &dString, "float GetErrorValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetErrorValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetErrorValue */
    if ( strcmp ( argv[2], "SetErrorValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetErrorValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetErrorValue */
    Tcl_DStringAppendElement ( &dString, " Get/Set error value\n" );
    Tcl_DStringAppendElement ( &dString, "void SetErrorValue(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetErrorValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetIterationNumber */
    if ( strcmp ( argv[2], "GetIterationNumber" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetIterationNumber" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetIterationNumber */
    Tcl_DStringAppendElement ( &dString, " Get/Set error value\n" );
    Tcl_DStringAppendElement ( &dString, "int GetIterationNumber();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetIterationNumber */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetIterationNumber */
    if ( strcmp ( argv[2], "SetIterationNumber" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetIterationNumber" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetIterationNumber */
    Tcl_DStringAppendElement ( &dString, " Get/Set error value\n" );
    Tcl_DStringAppendElement ( &dString, "void SetIterationNumber(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetIterationNumber */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSpacing */
    if ( strcmp ( argv[2], "GetSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSpacing */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float *GetSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSpacing */
    if ( strcmp ( argv[2], "SetSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSpacing */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetSpacing(float, float, float);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSpacing */
    if ( strcmp ( argv[2], "SetSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSpacing */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetSpacing(float a\\[3\\]);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInfinity */
    if ( strcmp ( argv[2], "GetInfinity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInfinity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInfinity */
    Tcl_DStringAppendElement ( &dString, " Set/Get infinity value\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInfinity();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for GetInfinity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInfinity */
    if ( strcmp ( argv[2], "SetInfinity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInfinity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInfinity */
    Tcl_DStringAppendElement ( &dString, " Set/Get infinity value\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInfinity(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjLaxConnectivity" );
    /* Closing for SetInfinity */

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
