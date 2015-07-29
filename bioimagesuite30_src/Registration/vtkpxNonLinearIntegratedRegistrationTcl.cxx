// tcl wrapper for vtkpxNonLinearIntegratedRegistration object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxNonLinearIntegratedRegistration.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxNonLinearIntegratedRegistrationNewCommand()
{
  vtkpxNonLinearIntegratedRegistration *temp = vtkpxNonLinearIntegratedRegistration::New();
  return static_cast<ClientData>(temp);
}

int vtkpxNonLinearRegistrationCppCommand(vtkpxNonLinearRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxNonLinearIntegratedRegistrationCppCommand(vtkpxNonLinearIntegratedRegistration *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxNonLinearIntegratedRegistrationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxNonLinearIntegratedRegistrationCppCommand(static_cast<vtkpxNonLinearIntegratedRegistration *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxNonLinearIntegratedRegistrationCppCommand(vtkpxNonLinearIntegratedRegistration *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxNonLinearIntegratedRegistration",argv[1]))
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
    vtkpxNonLinearIntegratedRegistration  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxNonLinearIntegratedRegistration");
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
    vtkpxNonLinearIntegratedRegistration  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxNonLinearIntegratedRegistration");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxNonLinearIntegratedRegistration  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxNonLinearIntegratedRegistration");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRPMEstimator",argv[1]))&&(argc == 2))
    {
    vtkpxRPMCorrespondenceFinder  *temp20;
    temp20 = (op)->GetRPMEstimator();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxRPMCorrespondenceFinder");
    return TCL_OK;
    }
  if ((!strcmp("GetPointsWeight",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPointsWeight();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPointsWeight",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetPointsWeight(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPointsWeightMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPointsWeightMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetPointsWeightMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPointsWeightMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ComputeRegistration",argv[1]))&&(argc == 3))
    {
    int      temp0;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->ComputeRegistration(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetWholeImage",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetWholeImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetWholeImage",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetWholeImage();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("WholeImageOn",argv[1]))&&(argc == 2))
    {
    op->WholeImageOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("WholeImageOff",argv[1]))&&(argc == 2))
    {
    op->WholeImageOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetMarginSize",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMarginSize(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMarginSize",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMarginSize();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetFixedCorrespondences",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetFixedCorrespondences(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFixedCorrespondences",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFixedCorrespondences();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("FixedCorrespondencesOn",argv[1]))&&(argc == 2))
    {
    op->FixedCorrespondencesOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("FixedCorrespondencesOff",argv[1]))&&(argc == 2))
    {
    op->FixedCorrespondencesOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxNonLinearIntegratedRegistrationCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxNonLinearRegistrationCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxNonLinearIntegratedRegistration:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRPMEstimator\n",NULL);
    Tcl_AppendResult(interp,"  GetPointsWeight\n",NULL);
    Tcl_AppendResult(interp,"  SetPointsWeight\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPointsWeightMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetPointsWeightMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  ComputeRegistration\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetWholeImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetWholeImage\n",NULL);
    Tcl_AppendResult(interp,"  WholeImageOn\n",NULL);
    Tcl_AppendResult(interp,"  WholeImageOff\n",NULL);
    Tcl_AppendResult(interp,"  SetMarginSize\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMarginSize\n",NULL);
    Tcl_AppendResult(interp,"  SetFixedCorrespondences\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFixedCorrespondences\n",NULL);
    Tcl_AppendResult(interp,"  FixedCorrespondencesOn\n",NULL);
    Tcl_AppendResult(interp,"  FixedCorrespondencesOff\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "GetRPMEstimator" );
    Tcl_DStringAppendElement ( &dString, "GetPointsWeight" );
    Tcl_DStringAppendElement ( &dString, "SetPointsWeight" );
    Tcl_DStringAppendElement ( &dString, "GetPointsWeightMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetPointsWeightMaxValue" );
    Tcl_DStringAppendElement ( &dString, "ComputeRegistration" );
    Tcl_DStringAppendElement ( &dString, "SetWholeImage" );
    Tcl_DStringAppendElement ( &dString, "GetWholeImage" );
    Tcl_DStringAppendElement ( &dString, "WholeImageOn" );
    Tcl_DStringAppendElement ( &dString, "WholeImageOff" );
    Tcl_DStringAppendElement ( &dString, "SetMarginSize" );
    Tcl_DStringAppendElement ( &dString, "GetMarginSize" );
    Tcl_DStringAppendElement ( &dString, "SetFixedCorrespondences" );
    Tcl_DStringAppendElement ( &dString, "GetFixedCorrespondences" );
    Tcl_DStringAppendElement ( &dString, "FixedCorrespondencesOn" );
    Tcl_DStringAppendElement ( &dString, "FixedCorrespondencesOff" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxNonLinearIntegratedRegistration *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRPMEstimator */
    if ( strcmp ( argv[2], "GetRPMEstimator" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRPMEstimator" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRPMEstimator */
    Tcl_DStringAppendElement ( &dString, " Get Internal Transformation Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "vtkpxRPMCorrespondenceFinder *GetRPMEstimator();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for GetRPMEstimator */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPointsWeight */
    if ( strcmp ( argv[2], "GetPointsWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPointsWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPointsWeight */
    Tcl_DStringAppendElement ( &dString, " Relative Weight of Intensity to Points \n" );
    Tcl_DStringAppendElement ( &dString, "float GetPointsWeight();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for GetPointsWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPointsWeight */
    if ( strcmp ( argv[2], "SetPointsWeight" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPointsWeight" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPointsWeight */
    Tcl_DStringAppendElement ( &dString, " Relative Weight of Intensity to Points \n" );
    Tcl_DStringAppendElement ( &dString, "void SetPointsWeight(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for SetPointsWeight */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPointsWeightMinValue */
    if ( strcmp ( argv[2], "GetPointsWeightMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPointsWeightMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPointsWeightMinValue */
    Tcl_DStringAppendElement ( &dString, " Relative Weight of Intensity to Points \n" );
    Tcl_DStringAppendElement ( &dString, "float GetPointsWeightMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for GetPointsWeightMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPointsWeightMaxValue */
    if ( strcmp ( argv[2], "GetPointsWeightMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPointsWeightMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPointsWeightMaxValue */
    Tcl_DStringAppendElement ( &dString, " Relative Weight of Intensity to Points \n" );
    Tcl_DStringAppendElement ( &dString, "float GetPointsWeightMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for GetPointsWeightMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ComputeRegistration */
    if ( strcmp ( argv[2], "ComputeRegistration" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ComputeRegistration" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ComputeRegistration */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int ComputeRegistration(int level);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for ComputeRegistration */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetWholeImage */
    if ( strcmp ( argv[2], "SetWholeImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetWholeImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetWholeImage */
    Tcl_DStringAppendElement ( &dString, " Whole Image -- if set to 0 only register on part of the image covered by points \n" );
    Tcl_DStringAppendElement ( &dString, "void SetWholeImage(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for SetWholeImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWholeImage */
    if ( strcmp ( argv[2], "GetWholeImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWholeImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWholeImage */
    Tcl_DStringAppendElement ( &dString, " Whole Image -- if set to 0 only register on part of the image covered by points \n" );
    Tcl_DStringAppendElement ( &dString, "int GetWholeImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for GetWholeImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: WholeImageOn */
    if ( strcmp ( argv[2], "WholeImageOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "WholeImageOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for WholeImageOn */
    Tcl_DStringAppendElement ( &dString, " Whole Image -- if set to 0 only register on part of the image covered by points \n" );
    Tcl_DStringAppendElement ( &dString, "void WholeImageOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for WholeImageOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: WholeImageOff */
    if ( strcmp ( argv[2], "WholeImageOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "WholeImageOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for WholeImageOff */
    Tcl_DStringAppendElement ( &dString, " Whole Image -- if set to 0 only register on part of the image covered by points \n" );
    Tcl_DStringAppendElement ( &dString, "void WholeImageOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for WholeImageOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMarginSize */
    if ( strcmp ( argv[2], "SetMarginSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMarginSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMarginSize */
    Tcl_DStringAppendElement ( &dString, " If WholeImage==0 use this to bound the region to be registered on\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMarginSize(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for SetMarginSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMarginSize */
    if ( strcmp ( argv[2], "GetMarginSize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMarginSize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMarginSize */
    Tcl_DStringAppendElement ( &dString, " If WholeImage==0 use this to bound the region to be registered on\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMarginSize();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for GetMarginSize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFixedCorrespondences */
    if ( strcmp ( argv[2], "SetFixedCorrespondences" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFixedCorrespondences" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFixedCorrespondences */
    Tcl_DStringAppendElement ( &dString, " FixedCorrespondencesk -- if 1 run RPM only once at the startof the registration\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFixedCorrespondences(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for SetFixedCorrespondences */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFixedCorrespondences */
    if ( strcmp ( argv[2], "GetFixedCorrespondences" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFixedCorrespondences" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFixedCorrespondences */
    Tcl_DStringAppendElement ( &dString, " FixedCorrespondencesk -- if 1 run RPM only once at the startof the registration\n" );
    Tcl_DStringAppendElement ( &dString, "int GetFixedCorrespondences();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for GetFixedCorrespondences */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FixedCorrespondencesOn */
    if ( strcmp ( argv[2], "FixedCorrespondencesOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FixedCorrespondencesOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FixedCorrespondencesOn */
    Tcl_DStringAppendElement ( &dString, " FixedCorrespondencesk -- if 1 run RPM only once at the startof the registration\n" );
    Tcl_DStringAppendElement ( &dString, "void FixedCorrespondencesOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for FixedCorrespondencesOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FixedCorrespondencesOff */
    if ( strcmp ( argv[2], "FixedCorrespondencesOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FixedCorrespondencesOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FixedCorrespondencesOff */
    Tcl_DStringAppendElement ( &dString, " FixedCorrespondencesk -- if 1 run RPM only once at the startof the registration\n" );
    Tcl_DStringAppendElement ( &dString, "void FixedCorrespondencesOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxNonLinearIntegratedRegistration" );
    /* Closing for FixedCorrespondencesOff */

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
