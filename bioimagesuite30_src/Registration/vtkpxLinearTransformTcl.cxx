// tcl wrapper for vtkpxLinearTransform object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxLinearTransform.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxLinearTransformNewCommand()
{
  vtkpxLinearTransform *temp = vtkpxLinearTransform::New();
  return static_cast<ClientData>(temp);
}

int vtkTransformCppCommand(vtkTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxLinearTransformCppCommand(vtkpxLinearTransform *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxLinearTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxLinearTransformCppCommand(static_cast<vtkpxLinearTransform *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxLinearTransformCppCommand(vtkpxLinearTransform *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxLinearTransform",argv[1]))
        {
        argv[2] = static_cast<char *>(static_cast<void *>(op));
        return TCL_OK;
        }
      if (vtkTransformCppCommand(static_cast<vtkTransform *>(op),interp,argc,argv) == TCL_OK)
        {
        return TCL_OK;
        }
      }
    return TCL_ERROR;
    }

  if (!strcmp("GetSuperClassName",argv[1]))
    {
    Tcl_SetResult(interp,const_cast<char *>("vtkTransform"), TCL_VOLATILE);
    return TCL_OK;
    }

  try
    {
  if ((!strcmp("New",argv[1]))&&(argc == 2))
    {
    vtkpxLinearTransform  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearTransform");
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
    vtkpxLinearTransform  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearTransform");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxLinearTransform  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxLinearTransform");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetParameters",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->SetParameters(temp0);
    Tcl_ResetResult(interp);
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
  if ((!strcmp("GetParameters",argv[1]))&&(argc == 3))
    {
    vtkDoubleArray  *temp0;
    error = 0;

    temp0 = (vtkDoubleArray *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkDoubleArray"),interp,error));
    if (!error)
    {
    op->GetParameters(temp0);
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
  if ((!strcmp("SetFlipX",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetFlipX(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFlipY",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetFlipY(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFlipZ",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetFlipZ(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetFlipX",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFlipX();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFlipY",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFlipY();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetFlipZ",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetFlipZ();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
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
  if ((!strcmp("SetDistortionMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetDistortionMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetDistortionMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDistortionMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
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
  if ((!strcmp("Inverse",argv[1]))&&(argc == 2))
    {
    op->Inverse();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("UpdateParameters",argv[1]))&&(argc == 2))
    {
    op->UpdateParameters();
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
  if ((!strcmp("Identity",argv[1]))&&(argc == 2))
    {
    op->Identity();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("Copy",argv[1]))&&(argc == 3))
    {
    vtkpxLinearTransform  *temp0;
    error = 0;

    temp0 = (vtkpxLinearTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxLinearTransform"),interp,error));
    if (!error)
    {
    op->Copy(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ManualSetParameters",argv[1]))&&(argc == 16))
    {
    float    temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    float    temp4;
    float    temp5;
    float    temp6;
    float    temp7;
    float    temp8;
    vtkImageData  *temp9;
    int      temp10;
    vtkImageData  *temp11;
    int      temp12;
    vtkTransform  *temp13;
    int      temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (Tcl_GetDouble(interp,argv[8],&tempd) != TCL_OK) error = 1;
    temp6 = tempd;
    if (Tcl_GetDouble(interp,argv[9],&tempd) != TCL_OK) error = 1;
    temp7 = tempd;
    if (Tcl_GetDouble(interp,argv[10],&tempd) != TCL_OK) error = 1;
    temp8 = tempd;
    temp9 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[11],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[12],&tempi) != TCL_OK) error = 1;
    temp10 = tempi;
    temp11 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[13],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[14],&tempi) != TCL_OK) error = 1;
    temp12 = tempi;
    temp13 = (vtkTransform *)(vtkTclGetPointerFromObject(argv[15],const_cast<char *>("vtkTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->ManualSetParameters(temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,temp10,temp11,temp12,temp13);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ExtractParameters",argv[1]))&&(argc == 8))
    {
    vtkAbstractTransform  *temp0;
    int      temp1;
    vtkImageData  *temp2;
    int      temp3;
    vtkImageData  *temp4;
    int      temp5;
    int      temp20;
    error = 0;

    temp0 = (vtkAbstractTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkAbstractTransform"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    temp2 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[4],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    temp4 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[6],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (!error)
    {
    temp20 = (op)->ExtractParameters(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateUpdateTransform",argv[1]))&&(argc == 7))
    {
    vtkTransform  *temp0;
    vtkImageData  *temp1;
    int      temp2;
    vtkImageData  *temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkTransform"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    temp3 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[5],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->GenerateUpdateTransform(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxLinearTransformCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkTransformCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxLinearTransform:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetParameters\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetParameters\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetParameters\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Put\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Set\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Get\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetFlipX\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetFlipY\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetFlipZ\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFlipX\n",NULL);
    Tcl_AppendResult(interp,"  GetFlipY\n",NULL);
    Tcl_AppendResult(interp,"  GetFlipZ\n",NULL);
    Tcl_AppendResult(interp,"  SetFreezeUpdates\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetFreezeUpdatesMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFreezeUpdatesMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetFreezeUpdates\n",NULL);
    Tcl_AppendResult(interp,"  SetDistortionMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDistortionMode\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Inverse\n",NULL);
    Tcl_AppendResult(interp,"  UpdateParameters\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfParameters\n",NULL);
    Tcl_AppendResult(interp,"  Identity\n",NULL);
    Tcl_AppendResult(interp,"  Copy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ManualSetParameters\t with 14 args\n",NULL);
    Tcl_AppendResult(interp,"  ExtractParameters\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  GenerateUpdateTransform\t with 5 args\n",NULL);
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
    vtkTransformCppCommand(op,interp,argc,argv);
    Tcl_DStringGetResult ( interp, &dStringParent );
    Tcl_DStringAppend ( &dString, Tcl_DStringValue ( &dStringParent ), -1 );
    Tcl_DStringAppendElement ( &dString, "New" );
    Tcl_DStringAppendElement ( &dString, "GetClassName" );
    Tcl_DStringAppendElement ( &dString, "IsA" );
    Tcl_DStringAppendElement ( &dString, "NewInstance" );
    Tcl_DStringAppendElement ( &dString, "SafeDownCast" );
    Tcl_DStringAppendElement ( &dString, "SetParameters" );
    Tcl_DStringAppendElement ( &dString, "SetParameters" );
    Tcl_DStringAppendElement ( &dString, "GetParameters" );
    Tcl_DStringAppendElement ( &dString, "GetParameters" );
    Tcl_DStringAppendElement ( &dString, "Put" );
    Tcl_DStringAppendElement ( &dString, "Set" );
    Tcl_DStringAppendElement ( &dString, "Get" );
    Tcl_DStringAppendElement ( &dString, "SetFlipX" );
    Tcl_DStringAppendElement ( &dString, "SetFlipY" );
    Tcl_DStringAppendElement ( &dString, "SetFlipZ" );
    Tcl_DStringAppendElement ( &dString, "GetFlipX" );
    Tcl_DStringAppendElement ( &dString, "GetFlipY" );
    Tcl_DStringAppendElement ( &dString, "GetFlipZ" );
    Tcl_DStringAppendElement ( &dString, "SetFreezeUpdates" );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdatesMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdatesMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetFreezeUpdates" );
    Tcl_DStringAppendElement ( &dString, "SetDistortionMode" );
    Tcl_DStringAppendElement ( &dString, "GetDistortionMode" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Inverse" );
    Tcl_DStringAppendElement ( &dString, "UpdateParameters" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfParameters" );
    Tcl_DStringAppendElement ( &dString, "Identity" );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    Tcl_DStringAppendElement ( &dString, "ManualSetParameters" );
    Tcl_DStringAppendElement ( &dString, "ExtractParameters" );
    Tcl_DStringAppendElement ( &dString, "GenerateUpdateTransform" );
  Tcl_DStringResult ( interp, &dString );
  Tcl_DStringFree ( &dString );
  Tcl_DStringFree ( &dStringParent );
    return TCL_OK;
    }
    if(argc==3) {
      Tcl_DString dString;
      int SuperClassStatus;
    SuperClassStatus = vtkTransformCppCommand(op,interp,argc,argv);
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxLinearTransform *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetParameters */
    Tcl_DStringAppendElement ( &dString, " Set the parameters\n" );
    Tcl_DStringAppendElement ( &dString, "void SetParameters(vtkDoubleArray *parameters);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for SetParameters */

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
    Tcl_DStringAppendElement ( &dString, " Set the parameters\n" );
    Tcl_DStringAppendElement ( &dString, "void SetParameters(vtkDoubleArray *parameters, int frame);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetParameters */
    Tcl_DStringAppendElement ( &dString, " Set the parameters\n" );
    Tcl_DStringAppendElement ( &dString, "void GetParameters(vtkDoubleArray *parameters);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for GetParameters */

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
    Tcl_DStringAppendElement ( &dString, " Set the parameters\n" );
    Tcl_DStringAppendElement ( &dString, "void GetParameters(vtkDoubleArray *parameters, int frame);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for GetParameters */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for Get */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFlipX */
    if ( strcmp ( argv[2], "SetFlipX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFlipX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFlipX */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetFlipX(int f);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for SetFlipX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFlipY */
    if ( strcmp ( argv[2], "SetFlipY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFlipY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFlipY */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetFlipY(int f);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for SetFlipY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFlipZ */
    if ( strcmp ( argv[2], "SetFlipZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFlipZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFlipZ */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetFlipZ(int f);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for SetFlipZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFlipX */
    if ( strcmp ( argv[2], "GetFlipX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFlipX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFlipX */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetFlipX();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for GetFlipX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFlipY */
    if ( strcmp ( argv[2], "GetFlipY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFlipY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFlipY */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetFlipY();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for GetFlipY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetFlipZ */
    if ( strcmp ( argv[2], "GetFlipZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetFlipZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetFlipZ */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetFlipZ();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for GetFlipZ */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for GetFreezeUpdates */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDistortionMode */
    if ( strcmp ( argv[2], "SetDistortionMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDistortionMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDistortionMode */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "void SetDistortionMode(int m);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for SetDistortionMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDistortionMode */
    if ( strcmp ( argv[2], "GetDistortionMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDistortionMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDistortionMode */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GetDistortionMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for GetDistortionMode */

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
    Tcl_DStringAppendElement ( &dString, " Load / Save Transformation\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Save(char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for Save */

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
    Tcl_DStringAppendElement ( &dString, "virtual int Load(char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for Load */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Inverse */
    if ( strcmp ( argv[2], "Inverse" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Inverse" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Inverse */
    Tcl_DStringAppendElement ( &dString, " Invert the transformation.  This will also set a flag so that\n the transformation will use the inverse of its Input, if an Input\n has been set.\n" );
    Tcl_DStringAppendElement ( &dString, "void Inverse();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for Inverse */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UpdateParameters */
    if ( strcmp ( argv[2], "UpdateParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UpdateParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UpdateParameters */
    Tcl_DStringAppendElement ( &dString, " Invert the transformation.  This will also set a flag so that\n the transformation will use the inverse of its Input, if an Input\n has been set.\n" );
    Tcl_DStringAppendElement ( &dString, "void UpdateParameters();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for UpdateParameters */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for GetNumberOfParameters */

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
    Tcl_DStringAppendElement ( &dString, " Set Identity\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Identity();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for Identity */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Copy */
    Tcl_DStringAppendElement ( &dString, " Copy\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Copy(vtkpxLinearTransform *other);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for Copy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ManualSetParameters */
    if ( strcmp ( argv[2], "ManualSetParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ManualSetParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ManualSetParameters */
    Tcl_DStringAppendElement ( &dString, " This is key code for setting/exporting transformation given an input\n and images of different orientations\n" );
    Tcl_DStringAppendElement ( &dString, "int ManualSetParameters(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz, vtkImageData *reference, int ref_orientation, vtkImageData *transform, int trn_orientation, vtkTransform *combined);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for ManualSetParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ExtractParameters */
    if ( strcmp ( argv[2], "ExtractParameters" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ExtractParameters" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkAbstractTransform" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ExtractParameters */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int ExtractParameters(vtkAbstractTransform *tr, int nparam, vtkImageData *reference, int ref_orientation, vtkImageData *transform, int trn_orientation);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for ExtractParameters */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateUpdateTransform */
    if ( strcmp ( argv[2], "GenerateUpdateTransform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateUpdateTransform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkTransform" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateUpdateTransform */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "int GenerateUpdateTransform(vtkTransform *output, vtkImageData *reference, int ref_orientation, vtkImageData *transform, int trn_orientation);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxLinearTransform" );
    /* Closing for GenerateUpdateTransform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
   Tcl_SetResult ( interp, const_cast<char*>("Could not find method"), TCL_VOLATILE ); 
   return TCL_ERROR;
   }
 }

  if (vtkTransformCppCommand(static_cast<vtkTransform *>(op),interp,argc,argv) == TCL_OK)
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
