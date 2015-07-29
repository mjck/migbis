// tcl wrapper for vtkmpjFastMarchingMethod object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkmpjFastMarchingMethod.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkmpjFastMarchingMethodNewCommand()
{
  vtkmpjFastMarchingMethod *temp = vtkmpjFastMarchingMethod::New();
  return static_cast<ClientData>(temp);
}

int vtkpxSimpleImageToImageFilterCppCommand(vtkpxSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkmpjFastMarchingMethodCppCommand(vtkmpjFastMarchingMethod *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkmpjFastMarchingMethodCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkmpjFastMarchingMethodCppCommand(static_cast<vtkmpjFastMarchingMethod *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkmpjFastMarchingMethodCppCommand(vtkmpjFastMarchingMethod *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkmpjFastMarchingMethod",argv[1]))
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
    vtkmpjFastMarchingMethod  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjFastMarchingMethod");
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
    vtkmpjFastMarchingMethod  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjFastMarchingMethod");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkmpjFastMarchingMethod  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjFastMarchingMethod");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetSpeedImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetSpeedImage(temp0);
    Tcl_ResetResult(interp);
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
  if ((!strcmp("GetKnownValues",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetKnownValues();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetKnownValues",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetKnownValues(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetKnownValuesMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetKnownValuesMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetKnownValuesMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetKnownValuesMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("KnownValuesOn",argv[1]))&&(argc == 2))
    {
    op->KnownValuesOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("KnownValuesOff",argv[1]))&&(argc == 2))
    {
    op->KnownValuesOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetPropagationTime",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPropagationTime();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPropagationTime",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetPropagationTime(temp0);
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
  if ((!strcmp("GetSpacing",argv[1]))&&(argc == 2))
    {
    double  *temp20;
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
    double temp0[3];
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
  if ((!strcmp("GetSecondOrder",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSecondOrder();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSecondOrder",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetSecondOrder(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SecondOrderOn",argv[1]))&&(argc == 2))
    {
    op->SecondOrderOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SecondOrderOff",argv[1]))&&(argc == 2))
    {
    op->SecondOrderOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetLabelImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetLabelImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetFrontPoints",argv[1]))&&(argc == 2))
    {
    vtkFloatArray  *temp20;
    temp20 = (op)->GetFrontPoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
  if ((!strcmp("GetElapsedTime",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetElapsedTime();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkmpjFastMarchingMethodCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkmpjFastMarchingMethod:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetSpeedImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetKnownPoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetKnownPoints\n",NULL);
    Tcl_AppendResult(interp,"  GetKnownValues\n",NULL);
    Tcl_AppendResult(interp,"  SetKnownValues\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetKnownValuesMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetKnownValuesMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  KnownValuesOn\n",NULL);
    Tcl_AppendResult(interp,"  KnownValuesOff\n",NULL);
    Tcl_AppendResult(interp,"  GetPropagationTime\n",NULL);
    Tcl_AppendResult(interp,"  SetPropagationTime\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInfinity\n",NULL);
    Tcl_AppendResult(interp,"  SetInfinity\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSpacing\n",NULL);
    Tcl_AppendResult(interp,"  SetSpacing\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetSecondOrder\n",NULL);
    Tcl_AppendResult(interp,"  SetSecondOrder\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SecondOrderOn\n",NULL);
    Tcl_AppendResult(interp,"  SecondOrderOff\n",NULL);
    Tcl_AppendResult(interp,"  GetLabelImage\n",NULL);
    Tcl_AppendResult(interp,"  GetFrontPoints\n",NULL);
    Tcl_AppendResult(interp,"  GetElapsedTime\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetSpeedImage" );
    Tcl_DStringAppendElement ( &dString, "SetKnownPoints" );
    Tcl_DStringAppendElement ( &dString, "GetKnownPoints" );
    Tcl_DStringAppendElement ( &dString, "GetKnownValues" );
    Tcl_DStringAppendElement ( &dString, "SetKnownValues" );
    Tcl_DStringAppendElement ( &dString, "GetKnownValuesMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetKnownValuesMaxValue" );
    Tcl_DStringAppendElement ( &dString, "KnownValuesOn" );
    Tcl_DStringAppendElement ( &dString, "KnownValuesOff" );
    Tcl_DStringAppendElement ( &dString, "GetPropagationTime" );
    Tcl_DStringAppendElement ( &dString, "SetPropagationTime" );
    Tcl_DStringAppendElement ( &dString, "GetInfinity" );
    Tcl_DStringAppendElement ( &dString, "SetInfinity" );
    Tcl_DStringAppendElement ( &dString, "GetSpacing" );
    Tcl_DStringAppendElement ( &dString, "SetSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetSecondOrder" );
    Tcl_DStringAppendElement ( &dString, "SetSecondOrder" );
    Tcl_DStringAppendElement ( &dString, "SecondOrderOn" );
    Tcl_DStringAppendElement ( &dString, "SecondOrderOff" );
    Tcl_DStringAppendElement ( &dString, "GetLabelImage" );
    Tcl_DStringAppendElement ( &dString, "GetFrontPoints" );
    Tcl_DStringAppendElement ( &dString, "GetElapsedTime" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkmpjFastMarchingMethod *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSpeedImage */
    if ( strcmp ( argv[2], "SetSpeedImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSpeedImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSpeedImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetSpeedImage(vtkImageData *speed);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for SetSpeedImage */

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
    Tcl_DStringAppendElement ( &dString, " Set/Get boundary conditions for the propagation\015\n" );
    Tcl_DStringAppendElement ( &dString, "void SetKnownPoints(vtkFloatArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
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
    Tcl_DStringAppendElement ( &dString, " Set/Get boundary conditions for the propagation\015\n" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray *GetKnownPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for GetKnownPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetKnownValues */
    if ( strcmp ( argv[2], "GetKnownValues" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetKnownValues" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetKnownValues */
    Tcl_DStringAppendElement ( &dString, " Turn on/off values for known points\015\n" );
    Tcl_DStringAppendElement ( &dString, "int GetKnownValues();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for GetKnownValues */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetKnownValues */
    if ( strcmp ( argv[2], "SetKnownValues" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetKnownValues" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetKnownValues */
    Tcl_DStringAppendElement ( &dString, " Turn on/off values for known points\015\n" );
    Tcl_DStringAppendElement ( &dString, "void SetKnownValues(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for SetKnownValues */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetKnownValuesMinValue */
    if ( strcmp ( argv[2], "GetKnownValuesMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetKnownValuesMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetKnownValuesMinValue */
    Tcl_DStringAppendElement ( &dString, " Turn on/off values for known points\015\n" );
    Tcl_DStringAppendElement ( &dString, "int GetKnownValuesMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for GetKnownValuesMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetKnownValuesMaxValue */
    if ( strcmp ( argv[2], "GetKnownValuesMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetKnownValuesMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetKnownValuesMaxValue */
    Tcl_DStringAppendElement ( &dString, " Turn on/off values for known points\015\n" );
    Tcl_DStringAppendElement ( &dString, "int GetKnownValuesMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for GetKnownValuesMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: KnownValuesOn */
    if ( strcmp ( argv[2], "KnownValuesOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "KnownValuesOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for KnownValuesOn */
    Tcl_DStringAppendElement ( &dString, " Turn on/off values for known points\015\n" );
    Tcl_DStringAppendElement ( &dString, "void KnownValuesOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for KnownValuesOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: KnownValuesOff */
    if ( strcmp ( argv[2], "KnownValuesOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "KnownValuesOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for KnownValuesOff */
    Tcl_DStringAppendElement ( &dString, " Turn on/off values for known points\015\n" );
    Tcl_DStringAppendElement ( &dString, "void KnownValuesOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for KnownValuesOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPropagationTime */
    if ( strcmp ( argv[2], "GetPropagationTime" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPropagationTime" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPropagationTime */
    Tcl_DStringAppendElement ( &dString, " Set/Get maximum propagation time\015\n" );
    Tcl_DStringAppendElement ( &dString, "float GetPropagationTime();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for GetPropagationTime */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPropagationTime */
    if ( strcmp ( argv[2], "SetPropagationTime" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPropagationTime" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPropagationTime */
    Tcl_DStringAppendElement ( &dString, " Set/Get maximum propagation time\015\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPropagationTime(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for SetPropagationTime */

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
    Tcl_DStringAppendElement ( &dString, " Set/Get infinity value\015\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInfinity();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
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
    Tcl_DStringAppendElement ( &dString, " Set/Get infinity value\015\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInfinity(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for SetInfinity */

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
    Tcl_DStringAppendElement ( &dString, " Set/Get grid spacing\015\n" );
    Tcl_DStringAppendElement ( &dString, "double *GetSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
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
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSpacing */
    Tcl_DStringAppendElement ( &dString, " Set/Get grid spacing\015\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSpacing(double a\\[3\\]);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for SetSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSecondOrder */
    if ( strcmp ( argv[2], "GetSecondOrder" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSecondOrder" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSecondOrder */
    Tcl_DStringAppendElement ( &dString, " Set/Get grid accuracy of the solution\015\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSecondOrder();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for GetSecondOrder */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSecondOrder */
    if ( strcmp ( argv[2], "SetSecondOrder" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSecondOrder" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSecondOrder */
    Tcl_DStringAppendElement ( &dString, " Set/Get grid accuracy of the solution\015\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSecondOrder(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for SetSecondOrder */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SecondOrderOn */
    if ( strcmp ( argv[2], "SecondOrderOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SecondOrderOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SecondOrderOn */
    Tcl_DStringAppendElement ( &dString, " Set/Get grid accuracy of the solution\015\n" );
    Tcl_DStringAppendElement ( &dString, "void SecondOrderOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for SecondOrderOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SecondOrderOff */
    if ( strcmp ( argv[2], "SecondOrderOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SecondOrderOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SecondOrderOff */
    Tcl_DStringAppendElement ( &dString, " Set/Get grid accuracy of the solution\015\n" );
    Tcl_DStringAppendElement ( &dString, "void SecondOrderOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for SecondOrderOff */

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
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetLabelImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for GetLabelImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFrontPoints */
    if ( strcmp ( argv[2], "GetFrontPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFrontPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFrontPoints */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray *GetFrontPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for GetFrontPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetElapsedTime */
    if ( strcmp ( argv[2], "GetElapsedTime" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetElapsedTime" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetElapsedTime */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetElapsedTime();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingMethod" );
    /* Closing for GetElapsedTime */

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
