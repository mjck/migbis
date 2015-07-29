// tcl wrapper for vtkpxPolySimilarityTransform object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxPolySimilarityTransform.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxPolySimilarityTransformNewCommand()
{
  vtkpxPolySimilarityTransform *temp = vtkpxPolySimilarityTransform::New();
  return static_cast<ClientData>(temp);
}

int vtkpxPolyAbstractTransformCppCommand(vtkpxPolyAbstractTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxPolySimilarityTransformCppCommand(vtkpxPolySimilarityTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxPolySimilarityTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxPolySimilarityTransformCppCommand(static_cast<vtkpxPolySimilarityTransform *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxPolySimilarityTransformCppCommand(vtkpxPolySimilarityTransform *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxPolySimilarityTransform",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkpxPolyAbstractTransformCppCommand(static_cast<vtkpxPolyAbstractTransform *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkpxPolyAbstractTransform"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
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
    vtkpxPolySimilarityTransform  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxPolySimilarityTransform");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxPolySimilarityTransform  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxPolySimilarityTransform");
    return TCL_OK;
    }
    }
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxPolySimilarityTransform  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxPolySimilarityTransform");
    return TCL_OK;
    }
  if ((!strcmp("SetRegionLabel",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    op->SetRegionLabel(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetTransform",argv[1]))&&(argc == 4))
    {
    int      temp0;
    vtkpxArticulatedJointTransform  *temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    temp1 = (vtkpxArticulatedJointTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxArticulatedJointTransform"),interp,error));
    if (!error)
    {
    op->SetTransform(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTransform",argv[1]))&&(argc == 3))
    {
    int      temp0;
    vtkpxArticulatedJointTransform  *temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->GetTransform(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxArticulatedJointTransform");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfTransformations",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfTransformations();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetWholeBodyPreset",argv[1]))&&(argc == 9))
    {
    int      temp0;
    int      temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    int      temp5;
    int      temp6;
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
    if (Tcl_GetInt(interp,argv[8],&tempi) != TCL_OK) error = 1;
    temp6 = tempi;
    if (!error)
    {
    op->SetWholeBodyPreset(temp0,temp1,temp2,temp3,temp4,temp5,temp6);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetCylinderPreset",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetCylinderPreset(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
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
  if ((!strcmp("GetNumberOfParameters",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfParameters();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetCurrentParameters",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->SetCurrentParameters(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetCurrentParameters",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->GetCurrentParameters(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("OptimizerFeedback",argv[1]))&&(argc == 5))
    {
    vtkObject  *temp0;
    char    *temp1;
    double   temp2;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    temp1 = argv[3];
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->OptimizerFeedback(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("CalculateGradient",argv[1]))&&(argc == 5))
    {
    vtkObject  *temp0;
    vtkDoubleArray  *temp1;
    vtkDoubleArray  *temp2;
    double   temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    temp1 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkDoubleArray"),interp,error));
    temp2 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->CalculateGradient(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CalculateFunction",argv[1]))&&(argc == 4))
    {
    vtkObject  *temp0;
    vtkDoubleArray  *temp1;
    double   temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    temp1 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->CalculateFunction(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("OptimizerGradient",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    vtkDoubleArray  *temp1;
    double   temp20;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    temp1 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->OptimizerGradient(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("OptimizerValue",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    double   temp20;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    temp20 = (op)->OptimizerValue(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FixOrigins",argv[1]))&&(argc == 3))
    {
    double   temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->FixOrigins(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetOriginAxes",argv[1]))&&(argc == 3))
    {
    vtkPoints  *temp0;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    if (!error)
    {
    op->SetOriginAxes(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("DrawAxis",argv[1]))&&(argc == 4))
    {
    float    temp0;
    float    temp1;
    vtkPolyData  *temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->DrawAxis(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
    }
  if ((!strcmp("ApproximatePoints",argv[1]))&&(argc == 8))
    {
    vtkPoints  *temp0;
    vtkPoints  *temp1;
    vtkpxMatrix  *temp2;
    int      temp3;
    float    temp4;
    float    temp5;
    double   temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    temp1 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPoints"),interp,error));
    temp2 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (!error)
    {
    temp20 = (op)->ApproximatePoints(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("InitializeRearLimbPresetRotations",argv[1]))&&(argc == 3))
    {
    vtkPoints  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    if (!error)
    {
    temp20 = (op)->InitializeRearLimbPresetRotations(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("InitializeRearLimbPresetRotations",argv[1]))&&(argc == 4))
    {
    vtkPoints  *temp0;
    vtkLinearTransform  *temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    temp1 = (vtkLinearTransform *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkLinearTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->InitializeRearLimbPresetRotations(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateTestPoints",argv[1]))&&(argc == 2))
    {
    vtkPoints  *temp20;
    temp20 = (op)->CreateTestPoints();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPoints");
    return TCL_OK;
    }
  if ((!strcmp("Identity",argv[1]))&&(argc == 2))
    {
    op->Identity();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetRegionLabelImageName",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->SetRegionLabelImageName(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRegionLabelImageName",argv[1]))&&(argc == 2))
    {
    char    *temp20;
    temp20 = (op)->GetRegionLabelImageName();
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
  if ((!strcmp("UpdateThinPlateSplineTransform",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->UpdateThinPlateSplineTransform();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("CreateThinPlateSplineTransform",argv[1]))&&(argc == 6))
    {
    int      temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (!error)
    {
    temp20 = (op)->CreateThinPlateSplineTransform(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateSamplePoints",argv[1]))&&(argc == 6))
    {
    int      temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    vtkPoints  *temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (!error)
    {
    temp20 = (op)->CreateSamplePoints(temp0,temp1,temp2,temp3);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPoints");
    return TCL_OK;
    }
    }
  if ((!strcmp("InitializeComboTransform",argv[1]))&&(argc == 6))
    {
    vtkpxComboTransform  *temp0;
    int      temp1;
    int      temp2;
    double   temp3;
    error = 0;

    temp0 = (vtkpxComboTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxComboTransform"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (!error)
    {
    op->InitializeComboTransform(temp0,temp1,temp2,temp3);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxPolySimilarityTransformCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkpxPolyAbstractTransformCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxPolySimilarityTransform:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  SetRegionLabel\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetTransform\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetTransform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfTransformations\n",NULL);
    Tcl_AppendResult(interp,"  SetWholeBodyPreset\t with 7 args\n",NULL);
    Tcl_AppendResult(interp,"  SetCylinderPreset\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfParameters\n",NULL);
    Tcl_AppendResult(interp,"  SetCurrentParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetCurrentParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerFeedback\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateGradient\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateFunction\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerGradient\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerValue\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  FixOrigins\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetOriginAxes\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  DrawAxis\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ApproximatePoints\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  InitializeRearLimbPresetRotations\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  InitializeRearLimbPresetRotations\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateTestPoints\n",NULL);
    Tcl_AppendResult(interp,"  Identity\n",NULL);
    Tcl_AppendResult(interp,"  SetRegionLabelImageName\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRegionLabelImageName\n",NULL);
    Tcl_AppendResult(interp,"  UpdateThinPlateSplineTransform\n",NULL);
    Tcl_AppendResult(interp,"  CreateThinPlateSplineTransform\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateSamplePoints\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  InitializeComboTransform\t with 4 args\n",NULL);
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
    vtkpxPolyAbstractTransformCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "SetRegionLabel" );
    Tcl_DStringAppendElement ( &dString, "SetTransform" );
    Tcl_DStringAppendElement ( &dString, "GetTransform" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfTransformations" );
    Tcl_DStringAppendElement ( &dString, "SetWholeBodyPreset" );
    Tcl_DStringAppendElement ( &dString, "SetCylinderPreset" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfParameters" );
    Tcl_DStringAppendElement ( &dString, "SetCurrentParameters" );
    Tcl_DStringAppendElement ( &dString, "GetCurrentParameters" );
    Tcl_DStringAppendElement ( &dString, "OptimizerFeedback" );
    Tcl_DStringAppendElement ( &dString, "CalculateGradient" );
    Tcl_DStringAppendElement ( &dString, "CalculateFunction" );
    Tcl_DStringAppendElement ( &dString, "OptimizerGradient" );
    Tcl_DStringAppendElement ( &dString, "OptimizerValue" );
    Tcl_DStringAppendElement ( &dString, "FixOrigins" );
    Tcl_DStringAppendElement ( &dString, "SetOriginAxes" );
    Tcl_DStringAppendElement ( &dString, "DrawAxis" );
    Tcl_DStringAppendElement ( &dString, "ApproximatePoints" );
    Tcl_DStringAppendElement ( &dString, "InitializeRearLimbPresetRotations" );
    Tcl_DStringAppendElement ( &dString, "InitializeRearLimbPresetRotations" );
    Tcl_DStringAppendElement ( &dString, "CreateTestPoints" );
    Tcl_DStringAppendElement ( &dString, "Identity" );
    Tcl_DStringAppendElement ( &dString, "SetRegionLabelImageName" );
    Tcl_DStringAppendElement ( &dString, "GetRegionLabelImageName" );
    Tcl_DStringAppendElement ( &dString, "UpdateThinPlateSplineTransform" );
    Tcl_DStringAppendElement ( &dString, "CreateThinPlateSplineTransform" );
    Tcl_DStringAppendElement ( &dString, "CreateSamplePoints" );
    Tcl_DStringAppendElement ( &dString, "InitializeComboTransform" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkpxPolyAbstractTransformCppCommand(op,interp,argc,argv);
    if ( SuperClassStatus == TCL_OK ) { return TCL_OK; }
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
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: New */
    if ( strcmp ( argv[2], "New" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "New" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for New */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "static vtkpxPolySimilarityTransform *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for New */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRegionLabel */
    if ( strcmp ( argv[2], "SetRegionLabel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRegionLabel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRegionLabel */
    Tcl_DStringAppendElement ( &dString, " Set Label Image\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetRegionLabel(vtkImageData *h);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for SetRegionLabel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTransform */
    if ( strcmp ( argv[2], "SetTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkpxArticulatedJointTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTransform */
    Tcl_DStringAppendElement ( &dString, " Get Linear Transform\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTransform(int i, vtkpxArticulatedJointTransform *tr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for SetTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTransform */
    if ( strcmp ( argv[2], "GetTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTransform */
    Tcl_DStringAppendElement ( &dString, " Get Linear Transform\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkpxArticulatedJointTransform *GetTransform(int i);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for GetTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfTransformations */
    if ( strcmp ( argv[2], "GetNumberOfTransformations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfTransformations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfTransformations */
    Tcl_DStringAppendElement ( &dString, " Get Linear Transform\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumberOfTransformations();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for GetNumberOfTransformations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetWholeBodyPreset */
    if ( strcmp ( argv[2], "SetWholeBodyPreset" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetWholeBodyPreset" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetWholeBodyPreset */
    Tcl_DStringAppendElement ( &dString, " This is a preset\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetWholeBodyPreset(int doscale, int hasfeet, int hashindlimbs, int hasforelimbs, int hashead, int taildivisions, int spinedivisions);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for SetWholeBodyPreset */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCylinderPreset */
    if ( strcmp ( argv[2], "SetCylinderPreset" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCylinderPreset" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCylinderPreset */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetCylinderPreset(int numparts=2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for SetCylinderPreset */

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
    Tcl_DStringAppendElement ( &dString, " Save & Load Transform into .tal file\n" );
    Tcl_DStringAppendElement ( &dString, "int Load(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
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
    Tcl_DStringAppendElement ( &dString, " Save & Load Transform into .tal file\n" );
    Tcl_DStringAppendElement ( &dString, "int Save(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for Save */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfParameters */
    if ( strcmp ( argv[2], "GetNumberOfParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfParameters */
    Tcl_DStringAppendElement ( &dString, " Optimization Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumberOfParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for GetNumberOfParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetCurrentParameters */
    if ( strcmp ( argv[2], "SetCurrentParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetCurrentParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetCurrentParameters */
    Tcl_DStringAppendElement ( &dString, " Optimization Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetCurrentParameters(vtkDoubleArray *param);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for SetCurrentParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetCurrentParameters */
    if ( strcmp ( argv[2], "GetCurrentParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetCurrentParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetCurrentParameters */
    Tcl_DStringAppendElement ( &dString, " Optimization Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void GetCurrentParameters(vtkDoubleArray *param);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for GetCurrentParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OptimizerFeedback */
    if ( strcmp ( argv[2], "OptimizerFeedback" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OptimizerFeedback" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OptimizerFeedback */
    Tcl_DStringAppendElement ( &dString, " These are needed for the Optimizer \n" );
    Tcl_DStringAppendElement ( &dString, "static void OptimizerFeedback(vtkObject *self, const char *line, double val);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for OptimizerFeedback */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CalculateGradient */
    if ( strcmp ( argv[2], "CalculateGradient" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CalculateGradient" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CalculateGradient */
    Tcl_DStringAppendElement ( &dString, " These are needed for the Optimizer \n" );
    Tcl_DStringAppendElement ( &dString, "static double CalculateGradient(vtkObject *self, vtkDoubleArray *position, vtkDoubleArray *output);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for CalculateGradient */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CalculateFunction */
    if ( strcmp ( argv[2], "CalculateFunction" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CalculateFunction" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkObject" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CalculateFunction */
    Tcl_DStringAppendElement ( &dString, " These are needed for the Optimizer \n" );
    Tcl_DStringAppendElement ( &dString, "static double CalculateFunction(vtkObject *self, vtkDoubleArray *position);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for CalculateFunction */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OptimizerGradient */
    if ( strcmp ( argv[2], "OptimizerGradient" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OptimizerGradient" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OptimizerGradient */
    Tcl_DStringAppendElement ( &dString, " Compute Gradients etc\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double OptimizerGradient(vtkDoubleArray *params, vtkDoubleArray *grad);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for OptimizerGradient */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: OptimizerValue */
    if ( strcmp ( argv[2], "OptimizerValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "OptimizerValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for OptimizerValue */
    Tcl_DStringAppendElement ( &dString, " Compute Gradients etc\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double OptimizerValue(vtkDoubleArray *params);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for OptimizerValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FixOrigins */
    if ( strcmp ( argv[2], "FixOrigins" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FixOrigins" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FixOrigins */
    Tcl_DStringAppendElement ( &dString, " Fix Origins\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void FixOrigins(double gap=2.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for FixOrigins */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetOriginAxes */
    if ( strcmp ( argv[2], "SetOriginAxes" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetOriginAxes" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetOriginAxes */
    Tcl_DStringAppendElement ( &dString, " Fix Origins\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetOriginAxes(vtkPoints *points);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for SetOriginAxes */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: DrawAxis */
    if ( strcmp ( argv[2], "DrawAxis" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "DrawAxis" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for DrawAxis */
    Tcl_DStringAppendElement ( &dString, " Fix Origins\n" );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData *DrawAxis(float scale=2.0, float length=100.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for DrawAxis */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ApproximatePoints */
    if ( strcmp ( argv[2], "ApproximatePoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ApproximatePoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkpxMatrix" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ApproximatePoints */
    Tcl_DStringAppendElement ( &dString, " Approximate Points\n" );
    Tcl_DStringAppendElement ( &dString, "virtual double ApproximatePoints(vtkPoints *SourceLandmarks, vtkPoints *TargetLandmarks, vtkpxMatrix *Weights, int numsteps=1, float beginstepsize=1.0, float tolerance=0.01);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for ApproximatePoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InitializeRearLimbPresetRotations */
    if ( strcmp ( argv[2], "InitializeRearLimbPresetRotations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InitializeRearLimbPresetRotations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InitializeRearLimbPresetRotations */
    Tcl_DStringAppendElement ( &dString, " Initialize Rotations\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int InitializeRearLimbPresetRotations(vtkPoints *points);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for InitializeRearLimbPresetRotations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InitializeRearLimbPresetRotations */
    if ( strcmp ( argv[2], "InitializeRearLimbPresetRotations" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InitializeRearLimbPresetRotations" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "vtkLinearTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InitializeRearLimbPresetRotations */
    Tcl_DStringAppendElement ( &dString, " Initialize Rotations\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int InitializeRearLimbPresetRotations(vtkPoints *points, vtkLinearTransform *tr);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for InitializeRearLimbPresetRotations */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateTestPoints */
    if ( strcmp ( argv[2], "CreateTestPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateTestPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateTestPoints */
    Tcl_DStringAppendElement ( &dString, " Initialize Rotations\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkPoints *CreateTestPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for CreateTestPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Identity */
    if ( strcmp ( argv[2], "Identity" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Identity" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Identity */
    Tcl_DStringAppendElement ( &dString, " Identity\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Identity();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for Identity */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRegionLabelImageName */
    if ( strcmp ( argv[2], "SetRegionLabelImageName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRegionLabelImageName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRegionLabelImageName */
    Tcl_DStringAppendElement ( &dString, " Specify name of the region label image\n" );
    Tcl_DStringAppendElement ( &dString, "void SetRegionLabelImageName(char *);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for SetRegionLabelImageName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRegionLabelImageName */
    if ( strcmp ( argv[2], "GetRegionLabelImageName" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRegionLabelImageName" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRegionLabelImageName */
    Tcl_DStringAppendElement ( &dString, " Specify name of the region label image\n" );
    Tcl_DStringAppendElement ( &dString, "char *GetRegionLabelImageName();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for GetRegionLabelImageName */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UpdateThinPlateSplineTransform */
    if ( strcmp ( argv[2], "UpdateThinPlateSplineTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UpdateThinPlateSplineTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UpdateThinPlateSplineTransform */
    Tcl_DStringAppendElement ( &dString, " Create Displacement Field\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int UpdateThinPlateSplineTransform();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for UpdateThinPlateSplineTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateThinPlateSplineTransform */
    if ( strcmp ( argv[2], "CreateThinPlateSplineTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateThinPlateSplineTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateThinPlateSplineTransform */
    Tcl_DStringAppendElement ( &dString, " Create Displacement Field\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int CreateThinPlateSplineTransform(int numberofcontrolpoints=150, float smooth=2.0, float ratio=1.25, float startsample=-1.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for CreateThinPlateSplineTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateSamplePoints */
    if ( strcmp ( argv[2], "CreateSamplePoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateSamplePoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateSamplePoints */
    Tcl_DStringAppendElement ( &dString, " Create Displacement Field\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkPoints *CreateSamplePoints(int numberofpoints=1500, float smooth=1.5, float ratio=1.25, float startsample=-1.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for CreateSamplePoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InitializeComboTransform */
    if ( strcmp ( argv[2], "InitializeComboTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InitializeComboTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxComboTransform" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InitializeComboTransform */
    Tcl_DStringAppendElement ( &dString, " Initialize from a Poly Similarity Transform\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void InitializeComboTransform(vtkpxComboTransform *combo, int numpoints, int numsteps, double smoothness=0.0001);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxPolySimilarityTransform" );
    /* Closing for InitializeComboTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkpxPolyAbstractTransformCppCommand(static_cast<vtkpxPolyAbstractTransform *>(op),interp,argc,argv) == TCL_OK)
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
