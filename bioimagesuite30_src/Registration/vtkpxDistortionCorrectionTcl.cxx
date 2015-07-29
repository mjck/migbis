// tcl wrapper for vtkpxDistortionCorrection object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxDistortionCorrection.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxDistortionCorrectionNewCommand()
{
  vtkpxDistortionCorrection *temp = vtkpxDistortionCorrection::New();
  return static_cast<ClientData>(temp);
}

int vtkpxNonLinearRegistrationCppCommand(vtkpxNonLinearRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxDistortionCorrectionCppCommand(vtkpxDistortionCorrection *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxDistortionCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxDistortionCorrectionCppCommand(static_cast<vtkpxDistortionCorrection *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxDistortionCorrectionCppCommand(vtkpxDistortionCorrection *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxDistortionCorrection",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxNonLinearRegistrationCppCommand(static_cast<vtkpxNonLinearRegistration *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxNonLinearRegistration"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxDistortionCorrection  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxDistortionCorrection");
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
    vtkpxDistortionCorrection  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxDistortionCorrection");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxDistortionCorrection  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxDistortionCorrection");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetUseJacobian",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetUseJacobian(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUseJacobianMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseJacobianMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseJacobianMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseJacobianMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseJacobian",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseJacobian();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("UseJacobianOn",argv[1]))&&(argc == 2))
    {
    op->UseJacobianOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("UseJacobianOff",argv[1]))&&(argc == 2))
    {
    op->UseJacobianOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetUseSignalLossCorrection",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetUseSignalLossCorrection(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUseSignalLossCorrectionMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseSignalLossCorrectionMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseSignalLossCorrectionMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseSignalLossCorrectionMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseSignalLossCorrection",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseSignalLossCorrection();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("UseSignalLossCorrectionOn",argv[1]))&&(argc == 2))
    {
    op->UseSignalLossCorrectionOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("UseSignalLossCorrectionOff",argv[1]))&&(argc == 2))
    {
    op->UseSignalLossCorrectionOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetPhaseEncodeAxis",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetPhaseEncodeAxis(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPhaseEncodeAxisMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPhaseEncodeAxisMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPhaseEncodeAxisMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPhaseEncodeAxisMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPhaseEncodeAxis",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPhaseEncodeAxis();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPhaseEncodeAxisToX",argv[1]))&&(argc == 2))
    {
    op->SetPhaseEncodeAxisToX();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetPhaseEncodeAxisToY",argv[1]))&&(argc == 2))
    {
    op->SetPhaseEncodeAxisToY();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetPhaseEncodeAxisToZ",argv[1]))&&(argc == 2))
    {
    op->SetPhaseEncodeAxisToZ();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("ScaleIntensitiesUsingJacobian",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkAbstractTransform  *temp1;
    vtkImageData  *temp2;
    int      temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->ScaleIntensitiesUsingJacobian(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ScaleIntensitiesUsingJacobianAndSignalLoss",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    vtkAbstractTransform  *temp1;
    vtkImageData  *temp2;
    vtkImageData  *temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkAbstractTransform"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->ScaleIntensitiesUsingJacobianAndSignalLoss(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ComputeSimpleJacobianAndSignalLoss",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    vtkImageData  *temp2;
    vtkAbstractTransform  *temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    temp3 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->ComputeSimpleJacobianAndSignalLoss(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxDistortionCorrectionCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxNonLinearRegistrationCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxDistortionCorrection:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetUseJacobian\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUseJacobianMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseJacobianMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseJacobian\n",NULL);
    Tcl_AppendResult(interp,"  UseJacobianOn\n",NULL);
    Tcl_AppendResult(interp,"  UseJacobianOff\n",NULL);
    Tcl_AppendResult(interp,"  SetUseSignalLossCorrection\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUseSignalLossCorrectionMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseSignalLossCorrectionMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseSignalLossCorrection\n",NULL);
    Tcl_AppendResult(interp,"  UseSignalLossCorrectionOn\n",NULL);
    Tcl_AppendResult(interp,"  UseSignalLossCorrectionOff\n",NULL);
    Tcl_AppendResult(interp,"  SetPhaseEncodeAxis\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPhaseEncodeAxisMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPhaseEncodeAxisMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPhaseEncodeAxis\n",NULL);
    Tcl_AppendResult(interp,"  SetPhaseEncodeAxisToX\n",NULL);
    Tcl_AppendResult(interp,"  SetPhaseEncodeAxisToY\n",NULL);
    Tcl_AppendResult(interp,"  SetPhaseEncodeAxisToZ\n",NULL);
    Tcl_AppendResult(interp,"  ScaleIntensitiesUsingJacobian\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ScaleIntensitiesUsingJacobianAndSignalLoss\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  ComputeSimpleJacobianAndSignalLoss\t with 4 args\n",NULL);
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
    vtkpxNonLinearRegistrationCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetUseJacobian" );
    Tcl_DStringAppendElement ( &dString, "GetUseJacobianMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseJacobianMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseJacobian" );
    Tcl_DStringAppendElement ( &dString, "UseJacobianOn" );
    Tcl_DStringAppendElement ( &dString, "UseJacobianOff" );
    Tcl_DStringAppendElement ( &dString, "SetUseSignalLossCorrection" );
    Tcl_DStringAppendElement ( &dString, "GetUseSignalLossCorrectionMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseSignalLossCorrectionMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseSignalLossCorrection" );
    Tcl_DStringAppendElement ( &dString, "UseSignalLossCorrectionOn" );
    Tcl_DStringAppendElement ( &dString, "UseSignalLossCorrectionOff" );
    Tcl_DStringAppendElement ( &dString, "SetPhaseEncodeAxis" );
    Tcl_DStringAppendElement ( &dString, "GetPhaseEncodeAxisMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetPhaseEncodeAxisMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetPhaseEncodeAxis" );
    Tcl_DStringAppendElement ( &dString, "SetPhaseEncodeAxisToX" );
    Tcl_DStringAppendElement ( &dString, "SetPhaseEncodeAxisToY" );
    Tcl_DStringAppendElement ( &dString, "SetPhaseEncodeAxisToZ" );
    Tcl_DStringAppendElement ( &dString, "ScaleIntensitiesUsingJacobian" );
    Tcl_DStringAppendElement ( &dString, "ScaleIntensitiesUsingJacobianAndSignalLoss" );
    Tcl_DStringAppendElement ( &dString, "ComputeSimpleJacobianAndSignalLoss" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxNonLinearRegistrationCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxDistortionCorrection *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUseJacobian */
    if ( strcmp ( argv[2], "SetUseJacobian" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUseJacobian" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUseJacobian */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUseJacobian(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for SetUseJacobian */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseJacobianMinValue */
    if ( strcmp ( argv[2], "GetUseJacobianMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseJacobianMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseJacobianMinValue */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseJacobianMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for GetUseJacobianMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseJacobianMaxValue */
    if ( strcmp ( argv[2], "GetUseJacobianMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseJacobianMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseJacobianMaxValue */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseJacobianMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for GetUseJacobianMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseJacobian */
    if ( strcmp ( argv[2], "GetUseJacobian" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseJacobian" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseJacobian */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseJacobian();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for GetUseJacobian */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseJacobianOn */
    if ( strcmp ( argv[2], "UseJacobianOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseJacobianOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseJacobianOn */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "void UseJacobianOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for UseJacobianOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseJacobianOff */
    if ( strcmp ( argv[2], "UseJacobianOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseJacobianOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseJacobianOff */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "void UseJacobianOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for UseJacobianOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUseSignalLossCorrection */
    if ( strcmp ( argv[2], "SetUseSignalLossCorrection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUseSignalLossCorrection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUseSignalLossCorrection */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUseSignalLossCorrection(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for SetUseSignalLossCorrection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseSignalLossCorrectionMinValue */
    if ( strcmp ( argv[2], "GetUseSignalLossCorrectionMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseSignalLossCorrectionMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseSignalLossCorrectionMinValue */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseSignalLossCorrectionMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for GetUseSignalLossCorrectionMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseSignalLossCorrectionMaxValue */
    if ( strcmp ( argv[2], "GetUseSignalLossCorrectionMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseSignalLossCorrectionMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseSignalLossCorrectionMaxValue */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseSignalLossCorrectionMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for GetUseSignalLossCorrectionMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseSignalLossCorrection */
    if ( strcmp ( argv[2], "GetUseSignalLossCorrection" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseSignalLossCorrection" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseSignalLossCorrection */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseSignalLossCorrection();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for GetUseSignalLossCorrection */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseSignalLossCorrectionOn */
    if ( strcmp ( argv[2], "UseSignalLossCorrectionOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseSignalLossCorrectionOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseSignalLossCorrectionOn */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "void UseSignalLossCorrectionOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for UseSignalLossCorrectionOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseSignalLossCorrectionOff */
    if ( strcmp ( argv[2], "UseSignalLossCorrectionOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseSignalLossCorrectionOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseSignalLossCorrectionOff */
    Tcl_DStringAppendElement ( &dString, " Use Jacobian Correction\n" );
    Tcl_DStringAppendElement ( &dString, "void UseSignalLossCorrectionOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for UseSignalLossCorrectionOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPhaseEncodeAxis */
    if ( strcmp ( argv[2], "SetPhaseEncodeAxis" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPhaseEncodeAxis" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPhaseEncodeAxis */
    Tcl_DStringAppendElement ( &dString, " Set Phase Encode Axis\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPhaseEncodeAxis(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for SetPhaseEncodeAxis */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPhaseEncodeAxisMinValue */
    if ( strcmp ( argv[2], "GetPhaseEncodeAxisMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPhaseEncodeAxisMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPhaseEncodeAxisMinValue */
    Tcl_DStringAppendElement ( &dString, " Set Phase Encode Axis\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPhaseEncodeAxisMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for GetPhaseEncodeAxisMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPhaseEncodeAxisMaxValue */
    if ( strcmp ( argv[2], "GetPhaseEncodeAxisMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPhaseEncodeAxisMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPhaseEncodeAxisMaxValue */
    Tcl_DStringAppendElement ( &dString, " Set Phase Encode Axis\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPhaseEncodeAxisMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for GetPhaseEncodeAxisMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPhaseEncodeAxis */
    if ( strcmp ( argv[2], "GetPhaseEncodeAxis" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPhaseEncodeAxis" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPhaseEncodeAxis */
    Tcl_DStringAppendElement ( &dString, " Set Phase Encode Axis\n" );
    Tcl_DStringAppendElement ( &dString, "int GetPhaseEncodeAxis();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for GetPhaseEncodeAxis */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPhaseEncodeAxisToX */
    if ( strcmp ( argv[2], "SetPhaseEncodeAxisToX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPhaseEncodeAxisToX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPhaseEncodeAxisToX */
    Tcl_DStringAppendElement ( &dString, " Set Phase Encode Axis\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPhaseEncodeAxisToX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for SetPhaseEncodeAxisToX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPhaseEncodeAxisToY */
    if ( strcmp ( argv[2], "SetPhaseEncodeAxisToY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPhaseEncodeAxisToY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPhaseEncodeAxisToY */
    Tcl_DStringAppendElement ( &dString, " Set Phase Encode Axis\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPhaseEncodeAxisToY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for SetPhaseEncodeAxisToY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPhaseEncodeAxisToZ */
    if ( strcmp ( argv[2], "SetPhaseEncodeAxisToZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPhaseEncodeAxisToZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPhaseEncodeAxisToZ */
    Tcl_DStringAppendElement ( &dString, " Set Phase Encode Axis\n" );
    Tcl_DStringAppendElement ( &dString, "void SetPhaseEncodeAxisToZ();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for SetPhaseEncodeAxisToZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScaleIntensitiesUsingJacobian */
    if ( strcmp ( argv[2], "ScaleIntensitiesUsingJacobian" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScaleIntensitiesUsingJacobian" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScaleIntensitiesUsingJacobian */
    Tcl_DStringAppendElement ( &dString, " Scale Intensities by Computing the Jacobian of the Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "static int ScaleIntensitiesUsingJacobian(vtkImageData *inout, vtkAbstractTransform *tr, vtkImageData *jac, int logmode=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for ScaleIntensitiesUsingJacobian */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScaleIntensitiesUsingJacobianAndSignalLoss */
    if ( strcmp ( argv[2], "ScaleIntensitiesUsingJacobianAndSignalLoss" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScaleIntensitiesUsingJacobianAndSignalLoss" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScaleIntensitiesUsingJacobianAndSignalLoss */
    Tcl_DStringAppendElement ( &dString, " Scale Intensities by Computing the Jacobian of the Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "static int ScaleIntensitiesUsingJacobianAndSignalLoss(vtkImageData *inout, vtkAbstractTransform *tr, vtkImageData *jac, vtkImageData *signalloss, int logmode=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for ScaleIntensitiesUsingJacobianAndSignalLoss */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeSimpleJacobianAndSignalLoss */
    if ( strcmp ( argv[2], "ComputeSimpleJacobianAndSignalLoss" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeSimpleJacobianAndSignalLoss" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeSimpleJacobianAndSignalLoss */
    Tcl_DStringAppendElement ( &dString, " Scale Intensities by Computing the Jacobian of the Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "static int ComputeSimpleJacobianAndSignalLoss(vtkImageData *ref, vtkImageData *jacobian, vtkImageData *signalloss, vtkAbstractTransform *tr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxDistortionCorrection" );
    /* Closing for ComputeSimpleJacobianAndSignalLoss */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxNonLinearRegistrationCppCommand(static_cast<vtkpxNonLinearRegistration *>(op),interp,argc,argv) == TCL_OK)
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
