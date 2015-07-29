// tcl wrapper for vtkmpjFastMarchingImageFilter object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkmpjFastMarchingImageFilter.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkmpjFastMarchingImageFilterNewCommand()
{
  vtkmpjFastMarchingImageFilter *temp = vtkmpjFastMarchingImageFilter::New();
  return static_cast<ClientData>(temp);
}

int vtkpxSimpleImageToImageFilterCppCommand(vtkpxSimpleImageToImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkmpjFastMarchingImageFilterCppCommand(vtkmpjFastMarchingImageFilter *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkmpjFastMarchingImageFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkmpjFastMarchingImageFilterCppCommand(static_cast<vtkmpjFastMarchingImageFilter *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkmpjFastMarchingImageFilterCppCommand(vtkmpjFastMarchingImageFilter *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkmpjFastMarchingImageFilter",argv[1]))
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
    vtkmpjFastMarchingImageFilter  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjFastMarchingImageFilter");
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
    vtkmpjFastMarchingImageFilter  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjFastMarchingImageFilter");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkmpjFastMarchingImageFilter  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkmpjFastMarchingImageFilter");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetAlivePoints",argv[1]))&&(argc == 3))
    {
    vtkDataArray  *temp0;
    error = 0;

    temp0 = (vtkDataArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDataArray"),interp,error));
    if (!error)
    {
    op->SetAlivePoints(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetAlivePoints",argv[1]))&&(argc == 2))
    {
    vtkDataArray  *temp20;
    temp20 = (op)->GetAlivePoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDataArray");
    return TCL_OK;
    }
  if ((!strcmp("SetTrialPoints",argv[1]))&&(argc == 3))
    {
    vtkDataArray  *temp0;
    error = 0;

    temp0 = (vtkDataArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDataArray"),interp,error));
    if (!error)
    {
    op->SetTrialPoints(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTrialPoints",argv[1]))&&(argc == 2))
    {
    vtkDataArray  *temp20;
    temp20 = (op)->GetTrialPoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDataArray");
    return TCL_OK;
    }
  if ((!strcmp("GetLabelImage",argv[1]))&&(argc == 2))
    {
    vtkImageData  *temp20;
    temp20 = (op)->GetLabelImage();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
  if ((!strcmp("SetSpeedConstant",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetSpeedConstant(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSpeedConstant",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetSpeedConstant();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNormalizationFactor",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetNormalizationFactor();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetNormalizationFactor",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetNormalizationFactor(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetStoppingValue",argv[1]))&&(argc == 2))
    {
    double   temp20;
    temp20 = (op)->GetStoppingValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetStoppingValue",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetStoppingValue(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCollectPoints",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetCollectPoints();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCollectPoints",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetCollectPoints(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("CollectPointsOn",argv[1]))&&(argc == 2))
    {
    op->CollectPointsOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("CollectPointsOff",argv[1]))&&(argc == 2))
    {
    op->CollectPointsOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetProcessedPoints",argv[1]))&&(argc == 2))
    {
    vtkDoubleArray  *temp20;
    temp20 = (op)->GetProcessedPoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkDoubleArray");
    return TCL_OK;
    }
  if ((!strcmp("SetOutputExtent",argv[1]))&&(argc == 8))
    {
    int      temp0;
    int      temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    int      temp5;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (!error)
    {
    op->SetOutputExtent(temp0,temp1,temp2,temp3,temp4,temp5);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetOutputSpacing",argv[1]))&&(argc == 5))
    {
    double   temp0;
    double   temp1;
    double   temp2;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->SetOutputSpacing(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOutputSpacing",argv[1]))&&(argc == 2))
    {
    double  *temp20;
    temp20 = (op)->GetOutputSpacing();
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
  if ((!strcmp("SetOutputSpacing",argv[1]))&&(argc == 5))
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
    op->SetOutputSpacing(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetOverrideOutputInformation",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetOverrideOutputInformation();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetOverrideOutputInformation",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetOverrideOutputInformation(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("OverrideOutputInformationOn",argv[1]))&&(argc == 2))
    {
    op->OverrideOutputInformationOn();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("OverrideOutputInformationOff",argv[1]))&&(argc == 2))
    {
    op->OverrideOutputInformationOff();
    Tcl_ResetResult(interp);
    return TCL_OK;
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

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkmpjFastMarchingImageFilterCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxSimpleImageToImageFilterCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkmpjFastMarchingImageFilter:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetAlivePoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetAlivePoints\n",NULL);
    Tcl_AppendResult(interp,"  SetTrialPoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTrialPoints\n",NULL);
    Tcl_AppendResult(interp,"  GetLabelImage\n",NULL);
    Tcl_AppendResult(interp,"  SetSpeedConstant\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSpeedConstant\n",NULL);
    Tcl_AppendResult(interp,"  GetNormalizationFactor\n",NULL);
    Tcl_AppendResult(interp,"  SetNormalizationFactor\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetStoppingValue\n",NULL);
    Tcl_AppendResult(interp,"  SetStoppingValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCollectPoints\n",NULL);
    Tcl_AppendResult(interp,"  SetCollectPoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  CollectPointsOn\n",NULL);
    Tcl_AppendResult(interp,"  CollectPointsOff\n",NULL);
    Tcl_AppendResult(interp,"  GetProcessedPoints\n",NULL);
    Tcl_AppendResult(interp,"  SetOutputExtent\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  SetOutputSpacing\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetOutputSpacing\n",NULL);
    Tcl_AppendResult(interp,"  SetOutputSpacing\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetOverrideOutputInformation\n",NULL);
    Tcl_AppendResult(interp,"  SetOverrideOutputInformation\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  OverrideOutputInformationOn\n",NULL);
    Tcl_AppendResult(interp,"  OverrideOutputInformationOff\n",NULL);
    Tcl_AppendResult(interp,"  GetLargeValue\n",NULL);
    Tcl_AppendResult(interp,"  SetLargeValue\t with 1 arg\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetAlivePoints" );
    Tcl_DStringAppendElement ( &dString, "GetAlivePoints" );
    Tcl_DStringAppendElement ( &dString, "SetTrialPoints" );
    Tcl_DStringAppendElement ( &dString, "GetTrialPoints" );
    Tcl_DStringAppendElement ( &dString, "GetLabelImage" );
    Tcl_DStringAppendElement ( &dString, "SetSpeedConstant" );
    Tcl_DStringAppendElement ( &dString, "GetSpeedConstant" );
    Tcl_DStringAppendElement ( &dString, "GetNormalizationFactor" );
    Tcl_DStringAppendElement ( &dString, "SetNormalizationFactor" );
    Tcl_DStringAppendElement ( &dString, "GetStoppingValue" );
    Tcl_DStringAppendElement ( &dString, "SetStoppingValue" );
    Tcl_DStringAppendElement ( &dString, "GetCollectPoints" );
    Tcl_DStringAppendElement ( &dString, "SetCollectPoints" );
    Tcl_DStringAppendElement ( &dString, "CollectPointsOn" );
    Tcl_DStringAppendElement ( &dString, "CollectPointsOff" );
    Tcl_DStringAppendElement ( &dString, "GetProcessedPoints" );
    Tcl_DStringAppendElement ( &dString, "SetOutputExtent" );
    Tcl_DStringAppendElement ( &dString, "SetOutputSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetOutputSpacing" );
    Tcl_DStringAppendElement ( &dString, "SetOutputSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetOverrideOutputInformation" );
    Tcl_DStringAppendElement ( &dString, "SetOverrideOutputInformation" );
    Tcl_DStringAppendElement ( &dString, "OverrideOutputInformationOn" );
    Tcl_DStringAppendElement ( &dString, "OverrideOutputInformationOff" );
    Tcl_DStringAppendElement ( &dString, "GetLargeValue" );
    Tcl_DStringAppendElement ( &dString, "SetLargeValue" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkmpjFastMarchingImageFilter *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
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
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetAlivePoints */
    if ( strcmp ( argv[2], "SetAlivePoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetAlivePoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDataArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetAlivePoints */
    Tcl_DStringAppendElement ( &dString, " Set/Get the container of Alive Points representing the initial front\n" );
    Tcl_DStringAppendElement ( &dString, "void SetAlivePoints(vtkDataArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetAlivePoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetAlivePoints */
    if ( strcmp ( argv[2], "GetAlivePoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetAlivePoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetAlivePoints */
    Tcl_DStringAppendElement ( &dString, " Set/Get the container of Alive Points representing the initial front\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDataArray *GetAlivePoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetAlivePoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTrialPoints */
    if ( strcmp ( argv[2], "SetTrialPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTrialPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDataArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTrialPoints */
    Tcl_DStringAppendElement ( &dString, " Set/Get the container of Trial Points representing the initial front\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTrialPoints(vtkDataArray *);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetTrialPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTrialPoints */
    if ( strcmp ( argv[2], "GetTrialPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTrialPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTrialPoints */
    Tcl_DStringAppendElement ( &dString, " Set/Get the container of Trial Points representing the initial front\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDataArray *GetTrialPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetTrialPoints */

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
    Tcl_DStringAppendElement ( &dString, " Get label image\n" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData *GetLabelImage();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetLabelImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSpeedConstant */
    if ( strcmp ( argv[2], "SetSpeedConstant" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSpeedConstant" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSpeedConstant */
    Tcl_DStringAppendElement ( &dString, " Set the Speed Constant. If the Speed Image is NULL,\n the SpeedConstant value is used for the whole level set.\n By default, the SpeedConstant is set to 1.0.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSpeedConstant(double value);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetSpeedConstant */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSpeedConstant */
    if ( strcmp ( argv[2], "GetSpeedConstant" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSpeedConstant" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSpeedConstant */
    Tcl_DStringAppendElement ( &dString, " Get the Speed Constant\n" );
    Tcl_DStringAppendElement ( &dString, "double GetSpeedConstant();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetSpeedConstant */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNormalizationFactor */
    if ( strcmp ( argv[2], "GetNormalizationFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNormalizationFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNormalizationFactor */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "double GetNormalizationFactor();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetNormalizationFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetNormalizationFactor */
    if ( strcmp ( argv[2], "SetNormalizationFactor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetNormalizationFactor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetNormalizationFactor */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetNormalizationFactor(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetNormalizationFactor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetStoppingValue */
    if ( strcmp ( argv[2], "GetStoppingValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetStoppingValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetStoppingValue */
    Tcl_DStringAppendElement ( &dString, " Set/Get the Fast Marching algorithm Stopping Value. The Fast Marching\n algorithm is terminated when the value of the smallest trial point\n is greater than the stopping value.\n" );
    Tcl_DStringAppendElement ( &dString, "double GetStoppingValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetStoppingValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetStoppingValue */
    if ( strcmp ( argv[2], "SetStoppingValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetStoppingValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetStoppingValue */
    Tcl_DStringAppendElement ( &dString, " Set/Get the Fast Marching algorithm Stopping Value. The Fast Marching\n algorithm is terminated when the value of the smallest trial point\n is greater than the stopping value.\n" );
    Tcl_DStringAppendElement ( &dString, "void SetStoppingValue(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetStoppingValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCollectPoints */
    if ( strcmp ( argv[2], "GetCollectPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCollectPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCollectPoints */
    Tcl_DStringAppendElement ( &dString, " Set/Get the Collect Points flag. Instrument the algorithm to collect\n a container of all nodes which it has visited. Useful for\n creating Narrowbands for level set algorithms that supports \n narrow banding. \n" );
    Tcl_DStringAppendElement ( &dString, "int GetCollectPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetCollectPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCollectPoints */
    if ( strcmp ( argv[2], "SetCollectPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCollectPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCollectPoints */
    Tcl_DStringAppendElement ( &dString, " Set/Get the Collect Points flag. Instrument the algorithm to collect\n a container of all nodes which it has visited. Useful for\n creating Narrowbands for level set algorithms that supports \n narrow banding. \n" );
    Tcl_DStringAppendElement ( &dString, "void SetCollectPoints(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetCollectPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CollectPointsOn */
    if ( strcmp ( argv[2], "CollectPointsOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CollectPointsOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CollectPointsOn */
    Tcl_DStringAppendElement ( &dString, " Set/Get the Collect Points flag. Instrument the algorithm to collect\n a container of all nodes which it has visited. Useful for\n creating Narrowbands for level set algorithms that supports \n narrow banding. \n" );
    Tcl_DStringAppendElement ( &dString, "void CollectPointsOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for CollectPointsOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CollectPointsOff */
    if ( strcmp ( argv[2], "CollectPointsOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CollectPointsOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CollectPointsOff */
    Tcl_DStringAppendElement ( &dString, " Set/Get the Collect Points flag. Instrument the algorithm to collect\n a container of all nodes which it has visited. Useful for\n creating Narrowbands for level set algorithms that supports \n narrow banding. \n" );
    Tcl_DStringAppendElement ( &dString, "void CollectPointsOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for CollectPointsOff */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetProcessedPoints */
    if ( strcmp ( argv[2], "GetProcessedPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetProcessedPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetProcessedPoints */
    Tcl_DStringAppendElement ( &dString, " Get the container of Processed Points. If the CollectPoints flag\n is set, the algorithm collects a container of all processed nodes.\n This is useful for defining creating Narrowbands for level\n set algorithms that supports narrow banding.\n" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray *GetProcessedPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetProcessedPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutputExtent */
    if ( strcmp ( argv[2], "SetOutputExtent" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutputExtent" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutputExtent */
    Tcl_DStringAppendElement ( &dString, " The output extents and spacing is computed as follows.\n If the speed image is NULL or if the OverrideOutputInformation is true, \n the output information is set from user specified parameters. These \n parameters can be specified using methods SetOutputExtent(), SetOutputSpacing().\n Else if the speed image is not NULL, the output information\n is copied from the input speed image. \n" );
    Tcl_DStringAppendElement ( &dString, "void SetOutputExtent(int minX, int maxX, int minY, int maxY, int minZ, int maxZ);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetOutputExtent */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutputSpacing */
    if ( strcmp ( argv[2], "SetOutputSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutputSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutputSpacing */
    Tcl_DStringAppendElement ( &dString, " The output extents and spacing is computed as follows.\n If the speed image is NULL or if the OverrideOutputInformation is true, \n the output information is set from user specified parameters. These \n parameters can be specified using methods SetOutputExtent(), SetOutputSpacing().\n Else if the speed image is not NULL, the output information\n is copied from the input speed image. \n" );
    Tcl_DStringAppendElement ( &dString, "void SetOutputSpacing(double si, double sj, double sk);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetOutputSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOutputSpacing */
    if ( strcmp ( argv[2], "GetOutputSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOutputSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOutputSpacing */
    Tcl_DStringAppendElement ( &dString, " The output extents and spacing is computed as follows.\n If the speed image is NULL or if the OverrideOutputInformation is true, \n the output information is set from user specified parameters. These \n parameters can be specified using methods SetOutputExtent(), SetOutputSpacing().\n Else if the speed image is not NULL, the output information\n is copied from the input speed image. \n" );
    Tcl_DStringAppendElement ( &dString, "double *GetOutputSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetOutputSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOutputSpacing */
    if ( strcmp ( argv[2], "SetOutputSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOutputSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOutputSpacing */
    Tcl_DStringAppendElement ( &dString, " The output extents and spacing is computed as follows.\n If the speed image is NULL or if the OverrideOutputInformation is true, \n the output information is set from user specified parameters. These \n parameters can be specified using methods SetOutputExtent(), SetOutputSpacing().\n Else if the speed image is not NULL, the output information\n is copied from the input speed image. \n" );
    Tcl_DStringAppendElement ( &dString, "void SetOutputSpacing(double a\\[3\\]);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetOutputSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOverrideOutputInformation */
    if ( strcmp ( argv[2], "GetOverrideOutputInformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOverrideOutputInformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOverrideOutputInformation */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetOverrideOutputInformation();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetOverrideOutputInformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOverrideOutputInformation */
    if ( strcmp ( argv[2], "SetOverrideOutputInformation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOverrideOutputInformation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOverrideOutputInformation */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetOverrideOutputInformation(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetOverrideOutputInformation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OverrideOutputInformationOn */
    if ( strcmp ( argv[2], "OverrideOutputInformationOn" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OverrideOutputInformationOn" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OverrideOutputInformationOn */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void OverrideOutputInformationOn();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for OverrideOutputInformationOn */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OverrideOutputInformationOff */
    if ( strcmp ( argv[2], "OverrideOutputInformationOff" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OverrideOutputInformationOff" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OverrideOutputInformationOff */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void OverrideOutputInformationOff();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for OverrideOutputInformationOff */

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
    Tcl_DStringAppendElement ( &dString, " Set/Get value for infinity\n" );
    Tcl_DStringAppendElement ( &dString, "double GetLargeValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for GetLargeValue */

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
    Tcl_DStringAppendElement ( &dString, " Set/Get value for infinity\n" );
    Tcl_DStringAppendElement ( &dString, "void SetLargeValue(double a);" );
    Tcl_DStringAppendElement ( &dString, "vtkmpjFastMarchingImageFilter" );
    /* Closing for SetLargeValue */

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
