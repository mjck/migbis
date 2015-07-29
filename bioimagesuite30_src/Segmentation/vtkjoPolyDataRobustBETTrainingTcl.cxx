// tcl wrapper for vtkjoPolyDataRobustBETTraining object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkjoPolyDataRobustBETTraining.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkjoPolyDataRobustBETTrainingNewCommand()
{
  vtkjoPolyDataRobustBETTraining *temp = vtkjoPolyDataRobustBETTraining::New();
  return static_cast<ClientData>(temp);
}

int vtkPolyDataAlgorithmCppCommand(vtkPolyDataAlgorithm *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkjoPolyDataRobustBETTrainingCppCommand(vtkjoPolyDataRobustBETTraining *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkjoPolyDataRobustBETTrainingCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkjoPolyDataRobustBETTrainingCppCommand(static_cast<vtkjoPolyDataRobustBETTraining *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkjoPolyDataRobustBETTrainingCppCommand(vtkjoPolyDataRobustBETTraining *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkjoPolyDataRobustBETTraining",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkPolyDataAlgorithmCppCommand(static_cast<vtkPolyDataAlgorithm *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkPolyDataAlgorithm"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkjoPolyDataRobustBETTraining  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkjoPolyDataRobustBETTraining");
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
    vtkjoPolyDataRobustBETTraining  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkjoPolyDataRobustBETTraining");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkjoPolyDataRobustBETTraining  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkjoPolyDataRobustBETTraining");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetSrcImage",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetSrcImage(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("AddInput",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->AddInput(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInputImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetInputImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetOutputAsImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetOutputAsImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("GetMin",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    double   temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->GetMin(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMax",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    double   temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->GetMax(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetMinIntensitySearchDist",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMinIntensitySearchDist(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMinIntensitySearchDist",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMinIntensitySearchDist();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetMaxIntensitySearchDist",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetMaxIntensitySearchDist(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetMaxIntensitySearchDist",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetMaxIntensitySearchDist();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOutMinIntensitySearchDist",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetOutMinIntensitySearchDist(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutMinIntensitySearchDist",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetOutMinIntensitySearchDist();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOutMaxIntensitySearchDist",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetOutMaxIntensitySearchDist(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutMaxIntensitySearchDist",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetOutMaxIntensitySearchDist();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetLocalThresholdConst",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetLocalThresholdConst(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLocalThresholdConstMinValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLocalThresholdConstMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLocalThresholdConstMaxValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLocalThresholdConstMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetLocalThresholdConst",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetLocalThresholdConst();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNormalizeInputs",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetNormalizeInputs(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNormalizeInputsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNormalizeInputsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNormalizeInputsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNormalizeInputsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNormalizeInputs",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNormalizeInputs();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("NormalizeInputsOn",argv[1]))&&(argc == 2))
    {
    op->NormalizeInputsOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("NormalizeInputsOff",argv[1]))&&(argc == 2))
    {
    op->NormalizeInputsOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetSearchOutwards",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetSearchOutwards(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSearchOutwardsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSearchOutwardsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSearchOutwardsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSearchOutwardsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSearchOutwards",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSearchOutwards();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SearchOutwardsOn",argv[1]))&&(argc == 2))
    {
    op->SearchOutwardsOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SearchOutwardsOff",argv[1]))&&(argc == 2))
    {
    op->SearchOutwardsOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkjoPolyDataRobustBETTrainingCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkPolyDataAlgorithmCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkjoPolyDataRobustBETTraining:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetSrcImage\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  AddInput\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInputImage\n",NULL);
    Tcl_AppendResult(interp,"  GetOutputAsImage\n",NULL);
    Tcl_AppendResult(interp,"  GetMin\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMax\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetMinIntensitySearchDist\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMinIntensitySearchDist\n",NULL);
    Tcl_AppendResult(interp,"  SetMaxIntensitySearchDist\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetMaxIntensitySearchDist\n",NULL);
    Tcl_AppendResult(interp,"  SetOutMinIntensitySearchDist\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutMinIntensitySearchDist\n",NULL);
    Tcl_AppendResult(interp,"  SetOutMaxIntensitySearchDist\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetOutMaxIntensitySearchDist\n",NULL);
    Tcl_AppendResult(interp,"  SetLocalThresholdConst\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetLocalThresholdConstMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLocalThresholdConstMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetLocalThresholdConst\n",NULL);
    Tcl_AppendResult(interp,"  SetNormalizeInputs\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNormalizeInputsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetNormalizeInputsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetNormalizeInputs\n",NULL);
    Tcl_AppendResult(interp,"  NormalizeInputsOn\n",NULL);
    Tcl_AppendResult(interp,"  NormalizeInputsOff\n",NULL);
    Tcl_AppendResult(interp,"  SetSearchOutwards\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSearchOutwardsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSearchOutwardsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSearchOutwards\n",NULL);
    Tcl_AppendResult(interp,"  SearchOutwardsOn\n",NULL);
    Tcl_AppendResult(interp,"  SearchOutwardsOff\n",NULL);
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
    vtkPolyDataAlgorithmCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetSrcImage" );
    Tcl_DStringAppendElement ( &dString, "AddInput" );
    Tcl_DStringAppendElement ( &dString, "GetInputImage" );
    Tcl_DStringAppendElement ( &dString, "GetOutputAsImage" );
    Tcl_DStringAppendElement ( &dString, "GetMin" );
    Tcl_DStringAppendElement ( &dString, "GetMax" );
    Tcl_DStringAppendElement ( &dString, "SetMinIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "SetMaxIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "SetOutMinIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "GetOutMinIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "SetOutMaxIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "GetOutMaxIntensitySearchDist" );
    Tcl_DStringAppendElement ( &dString, "SetLocalThresholdConst" );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConstMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConstMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConst" );
    Tcl_DStringAppendElement ( &dString, "SetNormalizeInputs" );
    Tcl_DStringAppendElement ( &dString, "GetNormalizeInputsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetNormalizeInputsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetNormalizeInputs" );
    Tcl_DStringAppendElement ( &dString, "NormalizeInputsOn" );
    Tcl_DStringAppendElement ( &dString, "NormalizeInputsOff" );
    Tcl_DStringAppendElement ( &dString, "SetSearchOutwards" );
    Tcl_DStringAppendElement ( &dString, "GetSearchOutwardsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSearchOutwardsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetSearchOutwards" );
    Tcl_DStringAppendElement ( &dString, "SearchOutwardsOn" );
    Tcl_DStringAppendElement ( &dString, "SearchOutwardsOff" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkPolyDataAlgorithmCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkjoPolyDataRobustBETTraining *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
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
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSrcImage */
    if ( strcmp ( argv[2], "SetSrcImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSrcImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSrcImage */
    Tcl_DStringAppendElement ( &dString, " Get/Set the source image.\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetSrcImage(vtkImageData *image);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SetSrcImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AddInput */
    if ( strcmp ( argv[2], "AddInput" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AddInput" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddInput */
    Tcl_DStringAppendElement ( &dString, " Add source image to the stack of training images.  The added\n image will be appended as a component to the end of the\n current image stack.  Use the NormalizeInputs flag to turn\n normalization on/off.\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void AddInput(vtkImageData *image);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for AddInput */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInputImage */
    if ( strcmp ( argv[2], "GetInputImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInputImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInputImage */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkImageData *GetInputImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetInputImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutputAsImage */
    if ( strcmp ( argv[2], "GetOutputAsImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutputAsImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutputAsImage */
    Tcl_DStringAppendElement ( &dString, " Get the calculated information as an image.\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkImageData *GetOutputAsImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetOutputAsImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMin */
    if ( strcmp ( argv[2], "GetMin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMin */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual double GetMin(const char *arrayName);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetMin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMax */
    if ( strcmp ( argv[2], "GetMax" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMax" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMax */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual double GetMax(const char *arrayName);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetMax */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMinIntensitySearchDist */
    if ( strcmp ( argv[2], "SetMinIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMinIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMinIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MinSearchDist - the distance searched for the minimum intensity\n from a surface point (d1 in the BET paper).  Default d1 = 7mm.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMinIntensitySearchDist(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SetMinIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMinIntensitySearchDist */
    if ( strcmp ( argv[2], "GetMinIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMinIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMinIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MinSearchDist - the distance searched for the minimum intensity\n from a surface point (d1 in the BET paper).  Default d1 = 7mm.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMinIntensitySearchDist();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetMinIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMaxIntensitySearchDist */
    if ( strcmp ( argv[2], "SetMaxIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMaxIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMaxIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MaxSearchDist - the distance searched for the maximum intensity\n from a surface point (d2 in the BET paper).  Default d2 = 0.5*d1.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetMaxIntensitySearchDist(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SetMaxIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMaxIntensitySearchDist */
    if ( strcmp ( argv[2], "GetMaxIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMaxIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMaxIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MaxSearchDist - the distance searched for the maximum intensity\n from a surface point (d2 in the BET paper).  Default d2 = 0.5*d1.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetMaxIntensitySearchDist();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetMaxIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutMinIntensitySearchDist */
    if ( strcmp ( argv[2], "SetOutMinIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutMinIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutMinIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MinSearchDist - the distance searched for the minimum intensity\n from a surface point (d1 in the BET paper).  Default d1 = 7mm.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetOutMinIntensitySearchDist(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SetOutMinIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutMinIntensitySearchDist */
    if ( strcmp ( argv[2], "GetOutMinIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutMinIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutMinIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MinSearchDist - the distance searched for the minimum intensity\n from a surface point (d1 in the BET paper).  Default d1 = 7mm.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetOutMinIntensitySearchDist();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetOutMinIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutMaxIntensitySearchDist */
    if ( strcmp ( argv[2], "SetOutMaxIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutMaxIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutMaxIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MaxSearchDist - the distance searched for the maximum intensity\n from a surface point (d2 in the BET paper).  Default d2 = 0.5*d1.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetOutMaxIntensitySearchDist(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SetOutMaxIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutMaxIntensitySearchDist */
    if ( strcmp ( argv[2], "GetOutMaxIntensitySearchDist" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutMaxIntensitySearchDist" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutMaxIntensitySearchDist */
    Tcl_DStringAppendElement ( &dString, " MaxSearchDist - the distance searched for the maximum intensity\n from a surface point (d2 in the BET paper).  Default d2 = 0.5*d1.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetOutMaxIntensitySearchDist();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetOutMaxIntensitySearchDist */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLocalThresholdConst */
    if ( strcmp ( argv[2], "SetLocalThresholdConst" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLocalThresholdConst" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLocalThresholdConst */
    Tcl_DStringAppendElement ( &dString, " LocalThresholdConst - fractional constant (b_t in the BET paper)\n used when computing the locally appropriate intensity threshold t_l.\n Default b_t = 0.5;\n" );
    Tcl_DStringAppendElement ( &dString, "void SetLocalThresholdConst(double);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SetLocalThresholdConst */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLocalThresholdConstMinValue */
    if ( strcmp ( argv[2], "GetLocalThresholdConstMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConstMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLocalThresholdConstMinValue */
    Tcl_DStringAppendElement ( &dString, " LocalThresholdConst - fractional constant (b_t in the BET paper)\n used when computing the locally appropriate intensity threshold t_l.\n Default b_t = 0.5;\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLocalThresholdConstMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetLocalThresholdConstMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLocalThresholdConstMaxValue */
    if ( strcmp ( argv[2], "GetLocalThresholdConstMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConstMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLocalThresholdConstMaxValue */
    Tcl_DStringAppendElement ( &dString, " LocalThresholdConst - fractional constant (b_t in the BET paper)\n used when computing the locally appropriate intensity threshold t_l.\n Default b_t = 0.5;\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLocalThresholdConstMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetLocalThresholdConstMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLocalThresholdConst */
    if ( strcmp ( argv[2], "GetLocalThresholdConst" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLocalThresholdConst" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLocalThresholdConst */
    Tcl_DStringAppendElement ( &dString, " LocalThresholdConst - fractional constant (b_t in the BET paper)\n used when computing the locally appropriate intensity threshold t_l.\n Default b_t = 0.5;\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLocalThresholdConst();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetLocalThresholdConst */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNormalizeInputs */
    if ( strcmp ( argv[2], "SetNormalizeInputs" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNormalizeInputs" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNormalizeInputs */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "void SetNormalizeInputs(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SetNormalizeInputs */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNormalizeInputsMinValue */
    if ( strcmp ( argv[2], "GetNormalizeInputsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNormalizeInputsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNormalizeInputsMinValue */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNormalizeInputsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetNormalizeInputsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNormalizeInputsMaxValue */
    if ( strcmp ( argv[2], "GetNormalizeInputsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNormalizeInputsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNormalizeInputsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNormalizeInputsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetNormalizeInputsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNormalizeInputs */
    if ( strcmp ( argv[2], "GetNormalizeInputs" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNormalizeInputs" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNormalizeInputs */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "int GetNormalizeInputs();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetNormalizeInputs */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NormalizeInputsOn */
    if ( strcmp ( argv[2], "NormalizeInputsOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NormalizeInputsOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NormalizeInputsOn */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "void NormalizeInputsOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for NormalizeInputsOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: NormalizeInputsOff */
    if ( strcmp ( argv[2], "NormalizeInputsOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "NormalizeInputsOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for NormalizeInputsOff */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "void NormalizeInputsOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for NormalizeInputsOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSearchOutwards */
    if ( strcmp ( argv[2], "SetSearchOutwards" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSearchOutwards" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSearchOutwards */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSearchOutwards(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SetSearchOutwards */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSearchOutwardsMinValue */
    if ( strcmp ( argv[2], "GetSearchOutwardsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSearchOutwardsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSearchOutwardsMinValue */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSearchOutwardsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetSearchOutwardsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSearchOutwardsMaxValue */
    if ( strcmp ( argv[2], "GetSearchOutwardsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSearchOutwardsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSearchOutwardsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSearchOutwardsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetSearchOutwardsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSearchOutwards */
    if ( strcmp ( argv[2], "GetSearchOutwards" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSearchOutwards" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSearchOutwards */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSearchOutwards();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for GetSearchOutwards */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SearchOutwardsOn */
    if ( strcmp ( argv[2], "SearchOutwardsOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SearchOutwardsOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SearchOutwardsOn */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "void SearchOutwardsOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SearchOutwardsOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SearchOutwardsOff */
    if ( strcmp ( argv[2], "SearchOutwardsOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SearchOutwardsOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SearchOutwardsOff */
    Tcl_DStringAppendElement ( &dString, " Normalize the input images as they are added to the input image\n stack (default is On).\n" );
    Tcl_DStringAppendElement ( &dString, "void SearchOutwardsOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkjoPolyDataRobustBETTraining" );
    /* Closing for SearchOutwardsOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkPolyDataAlgorithmCppCommand(static_cast<vtkPolyDataAlgorithm *>(op),interp,argc,argv) == TCL_OK)
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
