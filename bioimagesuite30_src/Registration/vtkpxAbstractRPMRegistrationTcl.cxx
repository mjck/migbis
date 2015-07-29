// tcl wrapper for vtkpxAbstractRPMRegistration object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxAbstractRPMRegistration.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxAbstractRPMRegistrationNewCommand()
{
  vtkpxAbstractRPMRegistration *temp = vtkpxAbstractRPMRegistration::New();
  return static_cast<ClientData>(temp);
}

int vtkpxRPMCorrespondenceFinderCppCommand(vtkpxRPMCorrespondenceFinder *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxAbstractRPMRegistrationCppCommand(vtkpxAbstractRPMRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxAbstractRPMRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxAbstractRPMRegistrationCppCommand(static_cast<vtkpxAbstractRPMRegistration *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxAbstractRPMRegistrationCppCommand(vtkpxAbstractRPMRegistration *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxAbstractRPMRegistration",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxRPMCorrespondenceFinderCppCommand(static_cast<vtkpxRPMCorrespondenceFinder *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxRPMCorrespondenceFinder"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxAbstractRPMRegistration  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxAbstractRPMRegistration");
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
    vtkpxAbstractRPMRegistration  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxAbstractRPMRegistration");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxAbstractRPMRegistration  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxAbstractRPMRegistration");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCurrentTransformation",argv[1]))&&(argc == 2))
    {
    vtkAbstractTransform  *temp20;
    temp20 = (op)->GetCurrentTransformation();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkAbstractTransform");
    return TCL_OK;
    }
  if ((!strcmp("SetInitialTemperature",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetInitialTemperature(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInitialTemperatureMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialTemperatureMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInitialTemperatureMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialTemperatureMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInitialTemperature",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialTemperature();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetFinalTemperature",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetFinalTemperature(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFinalTemperatureMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetFinalTemperatureMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFinalTemperatureMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetFinalTemperatureMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFinalTemperature",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetFinalTemperature();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetAnnealRate",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetAnnealRate(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAnnealRateMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetAnnealRateMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAnnealRateMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetAnnealRateMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetAnnealRate",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetAnnealRate();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetInitialSmoothnessFactor",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetInitialSmoothnessFactor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInitialSmoothnessFactorMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialSmoothnessFactorMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInitialSmoothnessFactorMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialSmoothnessFactorMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInitialSmoothnessFactor",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInitialSmoothnessFactor();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetFinalSmoothnessFactor",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetFinalSmoothnessFactor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFinalSmoothnessFactorMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetFinalSmoothnessFactorMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFinalSmoothnessFactorMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetFinalSmoothnessFactorMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFinalSmoothnessFactor",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetFinalSmoothnessFactor();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSymmetryWeight",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetSymmetryWeight(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSymmetryWeightMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSymmetryWeightMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSymmetryWeightMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSymmetryWeightMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSymmetryWeight",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetSymmetryWeight();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetUseSymmetricCorrespondence",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetUseSymmetricCorrespondence(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetUseSymmetricCorrespondenceMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseSymmetricCorrespondenceMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseSymmetricCorrespondenceMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseSymmetricCorrespondenceMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetUseSymmetricCorrespondence",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetUseSymmetricCorrespondence();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("UseSymmetricCorrespondenceOn",argv[1]))&&(argc == 2))
    {
    op->UseSymmetricCorrespondenceOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("UseSymmetricCorrespondenceOff",argv[1]))&&(argc == 2))
    {
    op->UseSymmetricCorrespondenceOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("Run",argv[1]))&&(argc == 2))
    {
    op->Run();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxAbstractRPMRegistrationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxRPMCorrespondenceFinderCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxAbstractRPMRegistration:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCurrentTransformation\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialTemperature\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialTemperatureMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialTemperatureMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialTemperature\n",NULL);
    Tcl_AppendResult(interp,"  SetFinalTemperature\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFinalTemperatureMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFinalTemperatureMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFinalTemperature\n",NULL);
    Tcl_AppendResult(interp,"  SetAnnealRate\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAnnealRateMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAnnealRateMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetAnnealRate\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialSmoothnessFactor\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialSmoothnessFactorMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialSmoothnessFactorMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialSmoothnessFactor\n",NULL);
    Tcl_AppendResult(interp,"  SetFinalSmoothnessFactor\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFinalSmoothnessFactorMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFinalSmoothnessFactorMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFinalSmoothnessFactor\n",NULL);
    Tcl_AppendResult(interp,"  SetSymmetryWeight\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSymmetryWeightMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSymmetryWeightMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSymmetryWeight\n",NULL);
    Tcl_AppendResult(interp,"  SetUseSymmetricCorrespondence\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetUseSymmetricCorrespondenceMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseSymmetricCorrespondenceMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetUseSymmetricCorrespondence\n",NULL);
    Tcl_AppendResult(interp,"  UseSymmetricCorrespondenceOn\n",NULL);
    Tcl_AppendResult(interp,"  UseSymmetricCorrespondenceOff\n",NULL);
    Tcl_AppendResult(interp,"  Run\n",NULL);
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
    vtkpxRPMCorrespondenceFinderCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "GetCurrentTransformation" );
    Tcl_DStringAppendElement ( &dString, "SetInitialTemperature" );
    Tcl_DStringAppendElement ( &dString, "GetInitialTemperatureMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetInitialTemperatureMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetInitialTemperature" );
    Tcl_DStringAppendElement ( &dString, "SetFinalTemperature" );
    Tcl_DStringAppendElement ( &dString, "GetFinalTemperatureMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetFinalTemperatureMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetFinalTemperature" );
    Tcl_DStringAppendElement ( &dString, "SetAnnealRate" );
    Tcl_DStringAppendElement ( &dString, "GetAnnealRateMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetAnnealRateMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetAnnealRate" );
    Tcl_DStringAppendElement ( &dString, "SetInitialSmoothnessFactor" );
    Tcl_DStringAppendElement ( &dString, "GetInitialSmoothnessFactorMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetInitialSmoothnessFactorMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetInitialSmoothnessFactor" );
    Tcl_DStringAppendElement ( &dString, "SetFinalSmoothnessFactor" );
    Tcl_DStringAppendElement ( &dString, "GetFinalSmoothnessFactorMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetFinalSmoothnessFactorMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetFinalSmoothnessFactor" );
    Tcl_DStringAppendElement ( &dString, "SetSymmetryWeight" );
    Tcl_DStringAppendElement ( &dString, "GetSymmetryWeightMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSymmetryWeightMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetSymmetryWeight" );
    Tcl_DStringAppendElement ( &dString, "SetUseSymmetricCorrespondence" );
    Tcl_DStringAppendElement ( &dString, "GetUseSymmetricCorrespondenceMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseSymmetricCorrespondenceMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetUseSymmetricCorrespondence" );
    Tcl_DStringAppendElement ( &dString, "UseSymmetricCorrespondenceOn" );
    Tcl_DStringAppendElement ( &dString, "UseSymmetricCorrespondenceOff" );
    Tcl_DStringAppendElement ( &dString, "Run" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxRPMCorrespondenceFinderCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxAbstractRPMRegistration *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCurrentTransformation */
    if ( strcmp ( argv[2], "GetCurrentTransformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCurrentTransformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCurrentTransformation */
    Tcl_DStringAppendElement ( &dString, " Get Current Transformation -- needed for displaying updates \n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkAbstractTransform *GetCurrentTransformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetCurrentTransformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialTemperature */
    if ( strcmp ( argv[2], "SetInitialTemperature" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialTemperature" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialTemperature */
    Tcl_DStringAppendElement ( &dString, " Initial Temperature for deterministic annealing\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialTemperature(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for SetInitialTemperature */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialTemperatureMinValue */
    if ( strcmp ( argv[2], "GetInitialTemperatureMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialTemperatureMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialTemperatureMinValue */
    Tcl_DStringAppendElement ( &dString, " Initial Temperature for deterministic annealing\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialTemperatureMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetInitialTemperatureMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialTemperatureMaxValue */
    if ( strcmp ( argv[2], "GetInitialTemperatureMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialTemperatureMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialTemperatureMaxValue */
    Tcl_DStringAppendElement ( &dString, " Initial Temperature for deterministic annealing\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialTemperatureMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetInitialTemperatureMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialTemperature */
    if ( strcmp ( argv[2], "GetInitialTemperature" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialTemperature" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialTemperature */
    Tcl_DStringAppendElement ( &dString, " Initial Temperature for deterministic annealing\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialTemperature();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetInitialTemperature */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFinalTemperature */
    if ( strcmp ( argv[2], "SetFinalTemperature" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFinalTemperature" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFinalTemperature */
    Tcl_DStringAppendElement ( &dString, " Final Temperature for deterministic annealing\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFinalTemperature(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for SetFinalTemperature */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFinalTemperatureMinValue */
    if ( strcmp ( argv[2], "GetFinalTemperatureMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFinalTemperatureMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFinalTemperatureMinValue */
    Tcl_DStringAppendElement ( &dString, " Final Temperature for deterministic annealing\n" );
    Tcl_DStringAppendElement ( &dString, "float GetFinalTemperatureMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetFinalTemperatureMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFinalTemperatureMaxValue */
    if ( strcmp ( argv[2], "GetFinalTemperatureMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFinalTemperatureMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFinalTemperatureMaxValue */
    Tcl_DStringAppendElement ( &dString, " Final Temperature for deterministic annealing\n" );
    Tcl_DStringAppendElement ( &dString, "float GetFinalTemperatureMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetFinalTemperatureMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFinalTemperature */
    if ( strcmp ( argv[2], "GetFinalTemperature" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFinalTemperature" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFinalTemperature */
    Tcl_DStringAppendElement ( &dString, " Final Temperature for deterministic annealing\n" );
    Tcl_DStringAppendElement ( &dString, "float GetFinalTemperature();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetFinalTemperature */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAnnealRate */
    if ( strcmp ( argv[2], "SetAnnealRate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAnnealRate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAnnealRate */
    Tcl_DStringAppendElement ( &dString, " AnnealRate for annealing \n" );
    Tcl_DStringAppendElement ( &dString, "void SetAnnealRate(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for SetAnnealRate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAnnealRateMinValue */
    if ( strcmp ( argv[2], "GetAnnealRateMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAnnealRateMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAnnealRateMinValue */
    Tcl_DStringAppendElement ( &dString, " AnnealRate for annealing \n" );
    Tcl_DStringAppendElement ( &dString, "float GetAnnealRateMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetAnnealRateMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAnnealRateMaxValue */
    if ( strcmp ( argv[2], "GetAnnealRateMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAnnealRateMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAnnealRateMaxValue */
    Tcl_DStringAppendElement ( &dString, " AnnealRate for annealing \n" );
    Tcl_DStringAppendElement ( &dString, "float GetAnnealRateMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetAnnealRateMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAnnealRate */
    if ( strcmp ( argv[2], "GetAnnealRate" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAnnealRate" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAnnealRate */
    Tcl_DStringAppendElement ( &dString, " AnnealRate for annealing \n" );
    Tcl_DStringAppendElement ( &dString, "float GetAnnealRate();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetAnnealRate */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialSmoothnessFactor */
    if ( strcmp ( argv[2], "SetInitialSmoothnessFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialSmoothnessFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialSmoothnessFactor */
    Tcl_DStringAppendElement ( &dString, " Smoothness Factors\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialSmoothnessFactor(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for SetInitialSmoothnessFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialSmoothnessFactorMinValue */
    if ( strcmp ( argv[2], "GetInitialSmoothnessFactorMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialSmoothnessFactorMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialSmoothnessFactorMinValue */
    Tcl_DStringAppendElement ( &dString, " Smoothness Factors\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialSmoothnessFactorMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetInitialSmoothnessFactorMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialSmoothnessFactorMaxValue */
    if ( strcmp ( argv[2], "GetInitialSmoothnessFactorMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialSmoothnessFactorMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialSmoothnessFactorMaxValue */
    Tcl_DStringAppendElement ( &dString, " Smoothness Factors\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialSmoothnessFactorMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetInitialSmoothnessFactorMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialSmoothnessFactor */
    if ( strcmp ( argv[2], "GetInitialSmoothnessFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialSmoothnessFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialSmoothnessFactor */
    Tcl_DStringAppendElement ( &dString, " Smoothness Factors\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInitialSmoothnessFactor();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetInitialSmoothnessFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFinalSmoothnessFactor */
    if ( strcmp ( argv[2], "SetFinalSmoothnessFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFinalSmoothnessFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFinalSmoothnessFactor */
    Tcl_DStringAppendElement ( &dString, " Smoothness Factors\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFinalSmoothnessFactor(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for SetFinalSmoothnessFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFinalSmoothnessFactorMinValue */
    if ( strcmp ( argv[2], "GetFinalSmoothnessFactorMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFinalSmoothnessFactorMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFinalSmoothnessFactorMinValue */
    Tcl_DStringAppendElement ( &dString, " Smoothness Factors\n" );
    Tcl_DStringAppendElement ( &dString, "float GetFinalSmoothnessFactorMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetFinalSmoothnessFactorMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFinalSmoothnessFactorMaxValue */
    if ( strcmp ( argv[2], "GetFinalSmoothnessFactorMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFinalSmoothnessFactorMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFinalSmoothnessFactorMaxValue */
    Tcl_DStringAppendElement ( &dString, " Smoothness Factors\n" );
    Tcl_DStringAppendElement ( &dString, "float GetFinalSmoothnessFactorMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetFinalSmoothnessFactorMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFinalSmoothnessFactor */
    if ( strcmp ( argv[2], "GetFinalSmoothnessFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFinalSmoothnessFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFinalSmoothnessFactor */
    Tcl_DStringAppendElement ( &dString, " Smoothness Factors\n" );
    Tcl_DStringAppendElement ( &dString, "float GetFinalSmoothnessFactor();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetFinalSmoothnessFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSymmetryWeight */
    if ( strcmp ( argv[2], "SetSymmetryWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSymmetryWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSymmetryWeight */
    Tcl_DStringAppendElement ( &dString, " Symmetry Weight for exchanging parameters\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSymmetryWeight(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for SetSymmetryWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSymmetryWeightMinValue */
    if ( strcmp ( argv[2], "GetSymmetryWeightMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSymmetryWeightMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSymmetryWeightMinValue */
    Tcl_DStringAppendElement ( &dString, " Symmetry Weight for exchanging parameters\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSymmetryWeightMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetSymmetryWeightMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSymmetryWeightMaxValue */
    if ( strcmp ( argv[2], "GetSymmetryWeightMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSymmetryWeightMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSymmetryWeightMaxValue */
    Tcl_DStringAppendElement ( &dString, " Symmetry Weight for exchanging parameters\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSymmetryWeightMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetSymmetryWeightMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSymmetryWeight */
    if ( strcmp ( argv[2], "GetSymmetryWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSymmetryWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSymmetryWeight */
    Tcl_DStringAppendElement ( &dString, " Symmetry Weight for exchanging parameters\n" );
    Tcl_DStringAppendElement ( &dString, "float GetSymmetryWeight();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetSymmetryWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetUseSymmetricCorrespondence */
    if ( strcmp ( argv[2], "SetUseSymmetricCorrespondence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetUseSymmetricCorrespondence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetUseSymmetricCorrespondence */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "void SetUseSymmetricCorrespondence(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for SetUseSymmetricCorrespondence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseSymmetricCorrespondenceMinValue */
    if ( strcmp ( argv[2], "GetUseSymmetricCorrespondenceMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseSymmetricCorrespondenceMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseSymmetricCorrespondenceMinValue */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseSymmetricCorrespondenceMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetUseSymmetricCorrespondenceMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseSymmetricCorrespondenceMaxValue */
    if ( strcmp ( argv[2], "GetUseSymmetricCorrespondenceMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseSymmetricCorrespondenceMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseSymmetricCorrespondenceMaxValue */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseSymmetricCorrespondenceMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetUseSymmetricCorrespondenceMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetUseSymmetricCorrespondence */
    if ( strcmp ( argv[2], "GetUseSymmetricCorrespondence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetUseSymmetricCorrespondence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetUseSymmetricCorrespondence */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "int GetUseSymmetricCorrespondence();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for GetUseSymmetricCorrespondence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseSymmetricCorrespondenceOn */
    if ( strcmp ( argv[2], "UseSymmetricCorrespondenceOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseSymmetricCorrespondenceOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseSymmetricCorrespondenceOn */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "void UseSymmetricCorrespondenceOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for UseSymmetricCorrespondenceOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UseSymmetricCorrespondenceOff */
    if ( strcmp ( argv[2], "UseSymmetricCorrespondenceOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UseSymmetricCorrespondenceOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UseSymmetricCorrespondenceOff */
    Tcl_DStringAppendElement ( &dString, " UseInitialTransform\n" );
    Tcl_DStringAppendElement ( &dString, "void UseSymmetricCorrespondenceOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for UseSymmetricCorrespondenceOff */

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
    Tcl_DStringAppendElement ( &dString, " Execute The Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Run();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxAbstractRPMRegistration" );
    /* Closing for Run */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxRPMCorrespondenceFinderCppCommand(static_cast<vtkpxRPMCorrespondenceFinder *>(op),interp,argc,argv) == TCL_OK)
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
