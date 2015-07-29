// tcl wrapper for vtkpxShapeTracking object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxShapeTracking.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxShapeTrackingNewCommand()
{
  vtkpxShapeTracking *temp = vtkpxShapeTracking::New();
  return static_cast<ClientData>(temp);
}

int vtkPolyDataSourceCppCommand(vtkPolyDataSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxShapeTrackingCppCommand(vtkpxShapeTracking *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxShapeTrackingCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxShapeTrackingCppCommand(static_cast<vtkpxShapeTracking *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxShapeTrackingCppCommand(vtkpxShapeTracking *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxShapeTracking",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkPolyDataSourceCppCommand(static_cast<vtkPolyDataSource *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkPolyDataSource"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxShapeTracking  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxShapeTracking");
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
    vtkpxShapeTracking  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxShapeTracking");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxShapeTracking  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxShapeTracking");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetReferenceSurface",argv[1]))&&(argc == 3))
    {
    vtkPolyData  *temp0;
    error = 0;

    temp0 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPolyData"),interp,error));
    if (!error)
    {
    op->SetReferenceSurface(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetReferenceSurface",argv[1]))&&(argc == 2))
    {
    vtkPolyData  *temp20;
    temp20 = (op)->GetReferenceSurface();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
  if ((!strcmp("SetTargetSurface",argv[1]))&&(argc == 3))
    {
    vtkPolyData  *temp0;
    error = 0;

    temp0 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPolyData"),interp,error));
    if (!error)
    {
    op->SetTargetSurface(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTargetSurface",argv[1]))&&(argc == 2))
    {
    vtkPolyData  *temp20;
    temp20 = (op)->GetTargetSurface();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
  if ((!strcmp("Load",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->Load(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Save",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->Save(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToNearestNeighbor",argv[1]))&&(argc == 2))
    {
    op->SetModeToNearestNeighbor();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToSymmetricNeighbor",argv[1]))&&(argc == 2))
    {
    op->SetModeToSymmetricNeighbor();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToAffinePlusNN",argv[1]))&&(argc == 2))
    {
    op->SetModeToAffinePlusNN();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToAffinePlusSNN",argv[1]))&&(argc == 2))
    {
    op->SetModeToAffinePlusSNN();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetInitialMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetInitialMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetInitialModeToRigidBody",argv[1]))&&(argc == 2))
    {
    op->SetInitialModeToRigidBody();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetInitialModeToSimilarity",argv[1]))&&(argc == 2))
    {
    op->SetInitialModeToSimilarity();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetInitialModeToAffine",argv[1]))&&(argc == 2))
    {
    op->SetInitialModeToAffine();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("GetInitialMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInitialMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSearchRadius",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetSearchRadius(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetSearchRadiusMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSearchRadiusMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSearchRadiusMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSearchRadiusMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetSearchRadius",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetSearchRadius();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetInterpolateIterations",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetInterpolateIterations(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInterpolateIterationsMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInterpolateIterationsMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInterpolateIterationsMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInterpolateIterationsMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInterpolateIterations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetInterpolateIterations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetInterpolateThreshold",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetInterpolateThreshold(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetInterpolateThresholdMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInterpolateThresholdMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInterpolateThresholdMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInterpolateThresholdMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetInterpolateThreshold",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetInterpolateThreshold();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetDisplacement",argv[1]))&&(argc == 4))
    {
    int      temp0;
    int      temp1;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->GetDisplacement(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetConfidence",argv[1]))&&(argc == 3))
    {
    int      temp0;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->GetConfidence(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDisplacements",argv[1]))&&(argc == 2))
    {
    vtkFloatArray  *temp20;
    temp20 = (op)->GetDisplacements();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkFloatArray");
    return TCL_OK;
    }
  if ((!strcmp("OldStyleShapeTracking",argv[1]))&&(argc == 7))
    {
    vtkpxTriangulatedStackSource  *temp0;
    vtkpxTriangulatedStackSource  *temp1;
    int      temp2;
    vtkFloatArray  *temp3;
    char    *temp4;
    error = 0;

    temp0 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    temp1 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    temp3 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkFloatArray"),interp,error));
    temp4 = argv[6];
    if (!error)
    {
    op->OldStyleShapeTracking(temp0,temp1,temp2,temp3,temp4);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("OldStyleShapeTracking",argv[1]))&&(argc == 6))
    {
    vtkpxTriangulatedStackSource  *temp0;
    vtkpxTriangulatedStackSource  *temp1;
    vtkFloatArray  *temp2;
    int      temp3;
    error = 0;

    temp0 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    temp1 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    temp2 = (vtkFloatArray *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkFloatArray"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    op->OldStyleShapeTracking(temp0,temp1,temp2,temp3);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("OldStyleShapeTracking",argv[1]))&&(argc == 7))
    {
    vtkpxTriangulatedStackSource  *temp0;
    vtkpxTriangulatedStackSource  *temp1;
    char    *temp2;
    int      temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    temp1 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    temp2 = argv[4];
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->OldStyleShapeTracking(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("OldStyleShapeTracking",argv[1]))&&(argc == 7))
    {
    char    *temp0;
    char    *temp1;
    char    *temp2;
    int      temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    temp1 = argv[3];
    temp2 = argv[4];
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->OldStyleShapeTracking(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxShapeTrackingCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkPolyDataSourceCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxShapeTracking:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetReferenceSurface\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetReferenceSurface\n",NULL);
    Tcl_AppendResult(interp,"  SetTargetSurface\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTargetSurface\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetMode\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToNearestNeighbor\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToSymmetricNeighbor\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToAffinePlusNN\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToAffinePlusSNN\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialModeToRigidBody\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialModeToSimilarity\n",NULL);
    Tcl_AppendResult(interp,"  SetInitialModeToAffine\n",NULL);
    Tcl_AppendResult(interp,"  GetInitialMode\n",NULL);
    Tcl_AppendResult(interp,"  SetSearchRadius\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetSearchRadiusMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSearchRadiusMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetSearchRadius\n",NULL);
    Tcl_AppendResult(interp,"  SetInterpolateIterations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolateIterationsMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolateIterationsMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolateIterations\n",NULL);
    Tcl_AppendResult(interp,"  SetInterpolateThreshold\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolateThresholdMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolateThresholdMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetInterpolateThreshold\n",NULL);
    Tcl_AppendResult(interp,"  GetDisplacement\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetConfidence\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDisplacements\n",NULL);
    Tcl_AppendResult(interp,"  OldStyleShapeTracking\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  OldStyleShapeTracking\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  OldStyleShapeTracking\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  OldStyleShapeTracking\t with 5 args\n",NULL);
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
    vtkPolyDataSourceCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetReferenceSurface" );
    Tcl_DStringAppendElement ( &dString, "GetReferenceSurface" );
    Tcl_DStringAppendElement ( &dString, "SetTargetSurface" );
    Tcl_DStringAppendElement ( &dString, "GetTargetSurface" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "SetMode" );
    Tcl_DStringAppendElement ( &dString, "GetModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetMode" );
    Tcl_DStringAppendElement ( &dString, "SetModeToNearestNeighbor" );
    Tcl_DStringAppendElement ( &dString, "SetModeToSymmetricNeighbor" );
    Tcl_DStringAppendElement ( &dString, "SetModeToAffinePlusNN" );
    Tcl_DStringAppendElement ( &dString, "SetModeToAffinePlusSNN" );
    Tcl_DStringAppendElement ( &dString, "SetInitialMode" );
    Tcl_DStringAppendElement ( &dString, "SetInitialModeToRigidBody" );
    Tcl_DStringAppendElement ( &dString, "SetInitialModeToSimilarity" );
    Tcl_DStringAppendElement ( &dString, "SetInitialModeToAffine" );
    Tcl_DStringAppendElement ( &dString, "GetInitialMode" );
    Tcl_DStringAppendElement ( &dString, "SetSearchRadius" );
    Tcl_DStringAppendElement ( &dString, "GetSearchRadiusMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetSearchRadiusMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetSearchRadius" );
    Tcl_DStringAppendElement ( &dString, "SetInterpolateIterations" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateIterationsMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateIterationsMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateIterations" );
    Tcl_DStringAppendElement ( &dString, "SetInterpolateThreshold" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateThresholdMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateThresholdMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateThreshold" );
    Tcl_DStringAppendElement ( &dString, "GetDisplacement" );
    Tcl_DStringAppendElement ( &dString, "GetConfidence" );
    Tcl_DStringAppendElement ( &dString, "GetDisplacements" );
    Tcl_DStringAppendElement ( &dString, "OldStyleShapeTracking" );
    Tcl_DStringAppendElement ( &dString, "OldStyleShapeTracking" );
    Tcl_DStringAppendElement ( &dString, "OldStyleShapeTracking" );
    Tcl_DStringAppendElement ( &dString, "OldStyleShapeTracking" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkPolyDataSourceCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxShapeTracking *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetReferenceSurface */
    if ( strcmp ( argv[2], "SetReferenceSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetReferenceSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetReferenceSurface */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetReferenceSurface(vtkPolyData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetReferenceSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetReferenceSurface */
    if ( strcmp ( argv[2], "GetReferenceSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetReferenceSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetReferenceSurface */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData *GetReferenceSurface();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetReferenceSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTargetSurface */
    if ( strcmp ( argv[2], "SetTargetSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTargetSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTargetSurface */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetTargetSurface(vtkPolyData *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetTargetSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTargetSurface */
    if ( strcmp ( argv[2], "GetTargetSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTargetSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTargetSurface */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData *GetTargetSurface();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetTargetSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Load */
    if ( strcmp ( argv[2], "Load" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Load" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Load */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int Load(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for Load */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Save */
    if ( strcmp ( argv[2], "Save" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Save" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Save */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int Save(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for Save */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetMode */
    if ( strcmp ( argv[2], "SetMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetMode */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetModeMinValue */
    if ( strcmp ( argv[2], "GetModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetModeMinValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetModeMaxValue */
    if ( strcmp ( argv[2], "GetModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetModeMaxValue */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetMode */
    if ( strcmp ( argv[2], "GetMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetMode */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToNearestNeighbor */
    if ( strcmp ( argv[2], "SetModeToNearestNeighbor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToNearestNeighbor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToNearestNeighbor */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToNearestNeighbor();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetModeToNearestNeighbor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToSymmetricNeighbor */
    if ( strcmp ( argv[2], "SetModeToSymmetricNeighbor" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToSymmetricNeighbor" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToSymmetricNeighbor */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToSymmetricNeighbor();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetModeToSymmetricNeighbor */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToAffinePlusNN */
    if ( strcmp ( argv[2], "SetModeToAffinePlusNN" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToAffinePlusNN" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToAffinePlusNN */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToAffinePlusNN();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetModeToAffinePlusNN */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToAffinePlusSNN */
    if ( strcmp ( argv[2], "SetModeToAffinePlusSNN" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToAffinePlusSNN" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToAffinePlusSNN */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToAffinePlusSNN();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetModeToAffinePlusSNN */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialMode */
    if ( strcmp ( argv[2], "SetInitialMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialMode */
    Tcl_DStringAppendElement ( &dString, " Set Mode for Affine Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialMode(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetInitialMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialModeToRigidBody */
    if ( strcmp ( argv[2], "SetInitialModeToRigidBody" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialModeToRigidBody" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialModeToRigidBody */
    Tcl_DStringAppendElement ( &dString, " Set Mode for Affine Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialModeToRigidBody();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetInitialModeToRigidBody */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialModeToSimilarity */
    if ( strcmp ( argv[2], "SetInitialModeToSimilarity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialModeToSimilarity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialModeToSimilarity */
    Tcl_DStringAppendElement ( &dString, " Set Mode for Affine Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialModeToSimilarity();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetInitialModeToSimilarity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInitialModeToAffine */
    if ( strcmp ( argv[2], "SetInitialModeToAffine" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInitialModeToAffine" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInitialModeToAffine */
    Tcl_DStringAppendElement ( &dString, " Set Mode for Affine Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInitialModeToAffine();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetInitialModeToAffine */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInitialMode */
    if ( strcmp ( argv[2], "GetInitialMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInitialMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInitialMode */
    Tcl_DStringAppendElement ( &dString, " Set Mode for Affine Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInitialMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetInitialMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSearchRadius */
    if ( strcmp ( argv[2], "SetSearchRadius" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSearchRadius" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSearchRadius */
    Tcl_DStringAppendElement ( &dString, " Window Size for Shape Match\n" );
    Tcl_DStringAppendElement ( &dString, "void SetSearchRadius(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetSearchRadius */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSearchRadiusMinValue */
    if ( strcmp ( argv[2], "GetSearchRadiusMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSearchRadiusMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSearchRadiusMinValue */
    Tcl_DStringAppendElement ( &dString, " Window Size for Shape Match\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSearchRadiusMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetSearchRadiusMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSearchRadiusMaxValue */
    if ( strcmp ( argv[2], "GetSearchRadiusMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSearchRadiusMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSearchRadiusMaxValue */
    Tcl_DStringAppendElement ( &dString, " Window Size for Shape Match\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSearchRadiusMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetSearchRadiusMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetSearchRadius */
    if ( strcmp ( argv[2], "GetSearchRadius" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetSearchRadius" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetSearchRadius */
    Tcl_DStringAppendElement ( &dString, " Window Size for Shape Match\n" );
    Tcl_DStringAppendElement ( &dString, "int GetSearchRadius();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetSearchRadius */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInterpolateIterations */
    if ( strcmp ( argv[2], "SetInterpolateIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInterpolateIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInterpolateIterations */
    Tcl_DStringAppendElement ( &dString, " Number of Iterations for Symmetric Nearest Neighbor Interpolation\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInterpolateIterations(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetInterpolateIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolateIterationsMinValue */
    if ( strcmp ( argv[2], "GetInterpolateIterationsMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateIterationsMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolateIterationsMinValue */
    Tcl_DStringAppendElement ( &dString, " Number of Iterations for Symmetric Nearest Neighbor Interpolation\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInterpolateIterationsMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetInterpolateIterationsMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolateIterationsMaxValue */
    if ( strcmp ( argv[2], "GetInterpolateIterationsMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateIterationsMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolateIterationsMaxValue */
    Tcl_DStringAppendElement ( &dString, " Number of Iterations for Symmetric Nearest Neighbor Interpolation\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInterpolateIterationsMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetInterpolateIterationsMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolateIterations */
    if ( strcmp ( argv[2], "GetInterpolateIterations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateIterations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolateIterations */
    Tcl_DStringAppendElement ( &dString, " Number of Iterations for Symmetric Nearest Neighbor Interpolation\n" );
    Tcl_DStringAppendElement ( &dString, "int GetInterpolateIterations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetInterpolateIterations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetInterpolateThreshold */
    if ( strcmp ( argv[2], "SetInterpolateThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetInterpolateThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetInterpolateThreshold */
    Tcl_DStringAppendElement ( &dString, " Threshold for Symmetric Nearest Neighbor Interpolation Convergence\n" );
    Tcl_DStringAppendElement ( &dString, "void SetInterpolateThreshold(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for SetInterpolateThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolateThresholdMinValue */
    if ( strcmp ( argv[2], "GetInterpolateThresholdMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateThresholdMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolateThresholdMinValue */
    Tcl_DStringAppendElement ( &dString, " Threshold for Symmetric Nearest Neighbor Interpolation Convergence\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInterpolateThresholdMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetInterpolateThresholdMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolateThresholdMaxValue */
    if ( strcmp ( argv[2], "GetInterpolateThresholdMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateThresholdMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolateThresholdMaxValue */
    Tcl_DStringAppendElement ( &dString, " Threshold for Symmetric Nearest Neighbor Interpolation Convergence\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInterpolateThresholdMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetInterpolateThresholdMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetInterpolateThreshold */
    if ( strcmp ( argv[2], "GetInterpolateThreshold" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetInterpolateThreshold" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetInterpolateThreshold */
    Tcl_DStringAppendElement ( &dString, " Threshold for Symmetric Nearest Neighbor Interpolation Convergence\n" );
    Tcl_DStringAppendElement ( &dString, "float GetInterpolateThreshold();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetInterpolateThreshold */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDisplacement */
    if ( strcmp ( argv[2], "GetDisplacement" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDisplacement" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDisplacement */
    Tcl_DStringAppendElement ( &dString, " Access Fem Data\n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetDisplacement(int node, int comp);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetDisplacement */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetConfidence */
    if ( strcmp ( argv[2], "GetConfidence" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetConfidence" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetConfidence */
    Tcl_DStringAppendElement ( &dString, " Access Fem Data\n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetConfidence(int node);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetConfidence */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDisplacements */
    if ( strcmp ( argv[2], "GetDisplacements" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDisplacements" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDisplacements */
    Tcl_DStringAppendElement ( &dString, " Get Displacements as Scalars\n" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray *GetDisplacements();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for GetDisplacements */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OldStyleShapeTracking */
    if ( strcmp ( argv[2], "OldStyleShapeTracking" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OldStyleShapeTracking" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OldStyleShapeTracking */
    Tcl_DStringAppendElement ( &dString, " Old Style Shape Tracking\n" );
    Tcl_DStringAppendElement ( &dString, "static void OldStyleShapeTracking(vtkpxTriangulatedStackSource *st1, vtkpxTriangulatedStackSource *st2, int searchwindow, vtkFloatArray *displacements, const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for OldStyleShapeTracking */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OldStyleShapeTracking */
    if ( strcmp ( argv[2], "OldStyleShapeTracking" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OldStyleShapeTracking" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringAppendElement ( &dString, "vtkFloatArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OldStyleShapeTracking */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static void OldStyleShapeTracking(vtkpxTriangulatedStackSource *st1, vtkpxTriangulatedStackSource *st2, vtkFloatArray *displ, int searchwindow=3);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for OldStyleShapeTracking */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OldStyleShapeTracking */
    if ( strcmp ( argv[2], "OldStyleShapeTracking" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OldStyleShapeTracking" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OldStyleShapeTracking */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int OldStyleShapeTracking(vtkpxTriangulatedStackSource *st1, vtkpxTriangulatedStackSource *st2, const char *filename, int searchwindow=3, int subdivide=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for OldStyleShapeTracking */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OldStyleShapeTracking */
    if ( strcmp ( argv[2], "OldStyleShapeTracking" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OldStyleShapeTracking" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OldStyleShapeTracking */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static int OldStyleShapeTracking(const char *name1, const char *name2, char *outfilename, int searchwindow=3, int subdivide=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxShapeTracking" );
    /* Closing for OldStyleShapeTracking */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkPolyDataSourceCppCommand(static_cast<vtkPolyDataSource *>(op),interp,argc,argv) == TCL_OK)
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
