// tcl wrapper for vtkpxSimilarityTransform object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxSimilarityTransform.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxSimilarityTransformNewCommand()
{
  vtkpxSimilarityTransform *temp = vtkpxSimilarityTransform::New();
  return static_cast<ClientData>(temp);
}

int vtkMatrixToLinearTransformCppCommand(vtkMatrixToLinearTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxSimilarityTransformCppCommand(vtkpxSimilarityTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxSimilarityTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxSimilarityTransformCppCommand(static_cast<vtkpxSimilarityTransform *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxSimilarityTransformCppCommand(vtkpxSimilarityTransform *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxSimilarityTransform",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkMatrixToLinearTransformCppCommand(static_cast<vtkMatrixToLinearTransform *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkMatrixToLinearTransform"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxSimilarityTransform  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSimilarityTransform");
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
    vtkpxSimilarityTransform  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSimilarityTransform");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxSimilarityTransform  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSimilarityTransform");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetParameters",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->SetParameters(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetParameters",argv[1]))&&(argc == 4))
    {
    vtkDoubleArray  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->GetParameters(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetModeToRotationXYOnly",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationXYOnly();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationOnly",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationOnly();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationScale",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationScale();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationTranslation",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationTranslation();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationScaleTranslation",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationScaleTranslation();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationOrigin",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationOrigin();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationScaleOrigin",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationScaleOrigin();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToRotationTranlationOrigin",argv[1]))&&(argc == 2))
    {
    op->SetModeToRotationTranlationOrigin();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToAll",argv[1]))&&(argc == 2))
    {
    op->SetModeToAll();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToZRotationOnly",argv[1]))&&(argc == 2))
    {
    op->SetModeToZRotationOnly();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("SetModeToFixedParameters",argv[1]))&&(argc == 2))
    {
    op->SetModeToFixedParameters();
    Tcl_ResetResult(interp);
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
  if ((!strcmp("Put",argv[1]))&&(argc == 4))
    {
    int      temp0;
    float    temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    op->Put(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Set",argv[1]))&&(argc == 4))
    {
    int      temp0;
    float    temp1;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    op->Set(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Get",argv[1]))&&(argc == 3))
    {
    int      temp0;
    float    temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->Get(temp0);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFreezeUpdates",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetFreezeUpdates(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFreezeUpdatesMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFreezeUpdatesMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFreezeUpdatesMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFreezeUpdatesMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFreezeUpdates",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFreezeUpdates();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
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
  if ((!strcmp("Identity",argv[1]))&&(argc == 2))
    {
    op->Identity();
    Tcl_ResetResult(interp);
    return TCL_OK;
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
  if ((!strcmp("Initialize",argv[1]))&&(argc == 2))
    {
    op->Initialize();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("Copy",argv[1]))&&(argc == 3))
    {
    vtkpxSimilarityTransform  *temp0;
    error = 0;

    temp0 = (vtkpxSimilarityTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSimilarityTransform"),interp,error));
    if (!error)
    {
    op->Copy(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("CopyParameters",argv[1]))&&(argc == 3))
    {
    vtkpxSimilarityTransform  *temp0;
    error = 0;

    temp0 = (vtkpxSimilarityTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSimilarityTransform"),interp,error));
    if (!error)
    {
    op->CopyParameters(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ApproximatePoints",argv[1]))&&(argc == 5))
    {
    vtkPoints  *temp0;
    vtkPoints  *temp1;
    vtkpxMatrix  *temp2;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    temp1 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkPoints"),interp,error));
    temp2 = (vtkpxMatrix *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkpxMatrix"),interp,error));
    if (!error)
    {
    op->ApproximatePoints(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("UpdateMatrix",argv[1]))&&(argc == 2))
    {
    op->UpdateMatrix();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("CreateBlendedCylinder",argv[1]))&&(argc == 5))
    {
    vtkPolyData  *temp0;
    float    temp1;
    int      temp2;
    vtkPolyData  *temp20;
    error = 0;

    temp0 = (vtkPolyData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPolyData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->CreateBlendedCylinder(temp0,temp1,temp2);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
    }
  if ((!strcmp("GetLocalZAxis",argv[1]))&&(argc == 2))
    {
    float   *temp20;
    temp20 = (op)->GetLocalZAxis();
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
  if ((!strcmp("GetLocalXAxis",argv[1]))&&(argc == 2))
    {
    float   *temp20;
    temp20 = (op)->GetLocalXAxis();
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
  if ((!strcmp("GetOrigin",argv[1]))&&(argc == 2))
    {
    float   *temp20;
    temp20 = (op)->GetOrigin();
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
  if ((!strcmp("GetRotationAxis",argv[1]))&&(argc == 2))
    {
    float   *temp20;
    temp20 = (op)->GetRotationAxis();
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
  if ((!strcmp("GetRotationAngle",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetRotationAngle();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTranslationScale",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetTranslationScale();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetTranslationScale",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetTranslationScale(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetTranslationScaleMinValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetTranslationScaleMinValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTranslationScaleMaxValue",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetTranslationScaleMaxValue();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxSimilarityTransformCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkMatrixToLinearTransformCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxSimilarityTransform:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetParameters\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetParameters\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationXYOnly\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationOnly\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationScale\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationTranslation\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationScaleTranslation\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationOrigin\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationScaleOrigin\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToRotationTranlationOrigin\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToAll\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToZRotationOnly\n",NULL);
    Tcl_AppendResult(interp,"  SetModeToFixedParameters\n",NULL);
    Tcl_AppendResult(interp,"  GetMode\n",NULL);
    Tcl_AppendResult(interp,"  SetMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Put\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Set\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Get\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetFreezeUpdates\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFreezeUpdatesMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFreezeUpdatesMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFreezeUpdates\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Identity\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfParameters\n",NULL);
    Tcl_AppendResult(interp,"  Initialize\n",NULL);
    Tcl_AppendResult(interp,"  Copy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  CopyParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ApproximatePoints\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  UpdateMatrix\n",NULL);
    Tcl_AppendResult(interp,"  CreateBlendedCylinder\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetLocalZAxis\n",NULL);
    Tcl_AppendResult(interp,"  GetLocalXAxis\n",NULL);
    Tcl_AppendResult(interp,"  GetOrigin\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationAxis\n",NULL);
    Tcl_AppendResult(interp,"  GetRotationAngle\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationScale\n",NULL);
    Tcl_AppendResult(interp,"  SetTranslationScale\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationScaleMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetTranslationScaleMaxValue\n",NULL);
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
    vtkMatrixToLinearTransformCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetParameters" );
    Tcl_DStringAppendElement ( &dString, "GetParameters" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationXYOnly" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationOnly" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationScale" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationTranslation" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationScaleTranslation" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationOrigin" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationScaleOrigin" );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationTranlationOrigin" );
    Tcl_DStringAppendElement ( &dString, "SetModeToAll" );
    Tcl_DStringAppendElement ( &dString, "SetModeToZRotationOnly" );
    Tcl_DStringAppendElement ( &dString, "SetModeToFixedParameters" );
    Tcl_DStringAppendElement ( &dString, "GetMode" );
    Tcl_DStringAppendElement ( &dString, "SetMode" );
    Tcl_DStringAppendElement ( &dString, "Put" );
    Tcl_DStringAppendElement ( &dString, "Set" );
    Tcl_DStringAppendElement ( &dString, "Get" );
    Tcl_DStringAppendElement ( &dString, "SetFreezeUpdates" );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdatesMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdatesMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdates" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "Identity" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfParameters" );
    Tcl_DStringAppendElement ( &dString, "Initialize" );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    Tcl_DStringAppendElement ( &dString, "CopyParameters" );
    Tcl_DStringAppendElement ( &dString, "ApproximatePoints" );
    Tcl_DStringAppendElement ( &dString, "UpdateMatrix" );
    Tcl_DStringAppendElement ( &dString, "CreateBlendedCylinder" );
    Tcl_DStringAppendElement ( &dString, "GetLocalZAxis" );
    Tcl_DStringAppendElement ( &dString, "GetLocalXAxis" );
    Tcl_DStringAppendElement ( &dString, "GetOrigin" );
    Tcl_DStringAppendElement ( &dString, "GetRotationAxis" );
    Tcl_DStringAppendElement ( &dString, "GetRotationAngle" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationScale" );
    Tcl_DStringAppendElement ( &dString, "SetTranslationScale" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationScaleMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetTranslationScaleMaxValue" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkMatrixToLinearTransformCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxSimilarityTransform *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetParameters */
    if ( strcmp ( argv[2], "SetParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetParameters */
    Tcl_DStringAppendElement ( &dString, " Set the parameters from array\n This is key for Optimization Classes \n" );
    Tcl_DStringAppendElement ( &dString, "void SetParameters(vtkDoubleArray *parameters, int InputOffset=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetParameters */
    if ( strcmp ( argv[2], "GetParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkDoubleArray" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetParameters */
    Tcl_DStringAppendElement ( &dString, " Set the parameters from array\n This is key for Optimization Classes \n" );
    Tcl_DStringAppendElement ( &dString, "void GetParameters(vtkDoubleArray *parameters, int InputOffset=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationXYOnly */
    if ( strcmp ( argv[2], "SetModeToRotationXYOnly" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationXYOnly" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationXYOnly */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationXYOnly();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToRotationXYOnly */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationOnly */
    if ( strcmp ( argv[2], "SetModeToRotationOnly" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationOnly" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationOnly */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationOnly();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToRotationOnly */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationScale */
    if ( strcmp ( argv[2], "SetModeToRotationScale" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationScale" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationScale */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationScale();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToRotationScale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationTranslation */
    if ( strcmp ( argv[2], "SetModeToRotationTranslation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationTranslation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationTranslation */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationTranslation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToRotationTranslation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationScaleTranslation */
    if ( strcmp ( argv[2], "SetModeToRotationScaleTranslation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationScaleTranslation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationScaleTranslation */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationScaleTranslation();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToRotationScaleTranslation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationOrigin */
    if ( strcmp ( argv[2], "SetModeToRotationOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationOrigin */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationOrigin();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToRotationOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationScaleOrigin */
    if ( strcmp ( argv[2], "SetModeToRotationScaleOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationScaleOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationScaleOrigin */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationScaleOrigin();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToRotationScaleOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToRotationTranlationOrigin */
    if ( strcmp ( argv[2], "SetModeToRotationTranlationOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToRotationTranlationOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToRotationTranlationOrigin */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToRotationTranlationOrigin();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToRotationTranlationOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToAll */
    if ( strcmp ( argv[2], "SetModeToAll" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToAll" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToAll */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToAll();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToAll */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToZRotationOnly */
    if ( strcmp ( argv[2], "SetModeToZRotationOnly" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToZRotationOnly" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToZRotationOnly */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToZRotationOnly();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToZRotationOnly */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetModeToFixedParameters */
    if ( strcmp ( argv[2], "SetModeToFixedParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetModeToFixedParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetModeToFixedParameters */
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetModeToFixedParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetModeToFixedParameters */

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
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "int GetMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetMode */

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
    Tcl_DStringAppendElement ( &dString, " Mode determines how Set/Get Parameters Operate, all other elements are active but fixed\n i.e. fixed scale DOES not mean no scaling\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetMode(int m);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Put */
    if ( strcmp ( argv[2], "Put" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Put" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Put */
    Tcl_DStringAppendElement ( &dString, " Puts a transformation parameter (transformation matrix is updated)\n" );
    Tcl_DStringAppendElement ( &dString, "void Put(int, float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for Put */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Set */
    if ( strcmp ( argv[2], "Set" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Set" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Set */
    Tcl_DStringAppendElement ( &dString, " Puts a transformation parameter (transformation matrix is updated)\n" );
    Tcl_DStringAppendElement ( &dString, "void Set(int a, float v);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for Set */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Get */
    if ( strcmp ( argv[2], "Get" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Get" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Get */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "float Get(int) const;" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for Get */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFreezeUpdates */
    if ( strcmp ( argv[2], "SetFreezeUpdates" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFreezeUpdates" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFreezeUpdates */
    Tcl_DStringAppendElement ( &dString, " Stop Auto Updates\n" );
    Tcl_DStringAppendElement ( &dString, "void SetFreezeUpdates(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetFreezeUpdates */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFreezeUpdatesMinValue */
    if ( strcmp ( argv[2], "GetFreezeUpdatesMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdatesMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFreezeUpdatesMinValue */
    Tcl_DStringAppendElement ( &dString, " Stop Auto Updates\n" );
    Tcl_DStringAppendElement ( &dString, "int GetFreezeUpdatesMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetFreezeUpdatesMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFreezeUpdatesMaxValue */
    if ( strcmp ( argv[2], "GetFreezeUpdatesMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdatesMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFreezeUpdatesMaxValue */
    Tcl_DStringAppendElement ( &dString, " Stop Auto Updates\n" );
    Tcl_DStringAppendElement ( &dString, "int GetFreezeUpdatesMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetFreezeUpdatesMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFreezeUpdates */
    if ( strcmp ( argv[2], "GetFreezeUpdates" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdates" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFreezeUpdates */
    Tcl_DStringAppendElement ( &dString, " Stop Auto Updates\n" );
    Tcl_DStringAppendElement ( &dString, "int GetFreezeUpdates();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetFreezeUpdates */

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
    Tcl_DStringAppendElement ( &dString, " Load / Save Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Load(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for Save */

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
    Tcl_DStringAppendElement ( &dString, " Set To Identity\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Identity();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for Identity */

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
    Tcl_DStringAppendElement ( &dString, " Number Of Parameters\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumberOfParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetNumberOfParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Initialize */
    if ( strcmp ( argv[2], "Initialize" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Initialize" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Initialize */
    Tcl_DStringAppendElement ( &dString, " Set Initialize\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Initialize();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for Initialize */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Copy */
    if ( strcmp ( argv[2], "Copy" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Copy */
    Tcl_DStringAppendElement ( &dString, " Copy\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Copy(vtkpxSimilarityTransform *other);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for Copy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CopyParameters */
    if ( strcmp ( argv[2], "CopyParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CopyParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CopyParameters */
    Tcl_DStringAppendElement ( &dString, " Copy\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void CopyParameters(vtkpxSimilarityTransform *other);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for CopyParameters */

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
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ApproximatePoints */
    Tcl_DStringAppendElement ( &dString, " Point Fitting\n" );
    Tcl_DStringAppendElement ( &dString, "void ApproximatePoints(vtkPoints *SourceLandmarks, vtkPoints *TargetLandmarks, vtkpxMatrix *Weights);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for ApproximatePoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UpdateMatrix */
    if ( strcmp ( argv[2], "UpdateMatrix" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UpdateMatrix" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UpdateMatrix */
    Tcl_DStringAppendElement ( &dString, " Forces Parameter Updates\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void UpdateMatrix();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for UpdateMatrix */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateBlendedCylinder */
    if ( strcmp ( argv[2], "CreateBlendedCylinder" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateBlendedCylinder" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPolyData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateBlendedCylinder */
    Tcl_DStringAppendElement ( &dString, " BlendedCylinder\n mode=0 no-blend, 1=symmetric blend, 2=causal blend\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkPolyData *CreateBlendedCylinder(vtkPolyData *input, float angle, int mode);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for CreateBlendedCylinder */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLocalZAxis */
    if ( strcmp ( argv[2], "GetLocalZAxis" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLocalZAxis" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLocalZAxis */
    Tcl_DStringAppendElement ( &dString, " Z-Axis : can \n" );
    Tcl_DStringAppendElement ( &dString, "float *GetLocalZAxis();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetLocalZAxis */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetLocalXAxis */
    if ( strcmp ( argv[2], "GetLocalXAxis" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetLocalXAxis" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetLocalXAxis */
    Tcl_DStringAppendElement ( &dString, " Z-Axis : can \n" );
    Tcl_DStringAppendElement ( &dString, "float *GetLocalXAxis();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetLocalXAxis */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetOrigin */
    if ( strcmp ( argv[2], "GetOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetOrigin */
    Tcl_DStringAppendElement ( &dString, " Access Internal Numbers\n" );
    Tcl_DStringAppendElement ( &dString, "float *GetOrigin();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationAxis */
    if ( strcmp ( argv[2], "GetRotationAxis" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationAxis" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationAxis */
    Tcl_DStringAppendElement ( &dString, " Access Internal Numbers\n" );
    Tcl_DStringAppendElement ( &dString, "float *GetRotationAxis();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetRotationAxis */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRotationAngle */
    if ( strcmp ( argv[2], "GetRotationAngle" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRotationAngle" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRotationAngle */
    Tcl_DStringAppendElement ( &dString, " Access Internal Numbers\n" );
    Tcl_DStringAppendElement ( &dString, "float GetRotationAngle();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetRotationAngle */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationScale */
    if ( strcmp ( argv[2], "GetTranslationScale" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationScale" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationScale */
    Tcl_DStringAppendElement ( &dString, " Translation Scale == size of voxel in mm\n" );
    Tcl_DStringAppendElement ( &dString, "float GetTranslationScale();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetTranslationScale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTranslationScale */
    if ( strcmp ( argv[2], "SetTranslationScale" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTranslationScale" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTranslationScale */
    Tcl_DStringAppendElement ( &dString, " Translation Scale == size of voxel in mm\n" );
    Tcl_DStringAppendElement ( &dString, "void SetTranslationScale(float);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for SetTranslationScale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationScaleMinValue */
    if ( strcmp ( argv[2], "GetTranslationScaleMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationScaleMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationScaleMinValue */
    Tcl_DStringAppendElement ( &dString, " Translation Scale == size of voxel in mm\n" );
    Tcl_DStringAppendElement ( &dString, "float GetTranslationScaleMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetTranslationScaleMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTranslationScaleMaxValue */
    if ( strcmp ( argv[2], "GetTranslationScaleMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTranslationScaleMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTranslationScaleMaxValue */
    Tcl_DStringAppendElement ( &dString, " Translation Scale == size of voxel in mm\n" );
    Tcl_DStringAppendElement ( &dString, "float GetTranslationScaleMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSimilarityTransform" );
    /* Closing for GetTranslationScaleMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkMatrixToLinearTransformCppCommand(static_cast<vtkMatrixToLinearTransform *>(op),interp,argc,argv) == TCL_OK)
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
