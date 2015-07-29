// tcl wrapper for vtkpxSplineStackSource object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxSplineStackSource.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxSplineStackSourceNewCommand()
{
  vtkpxSplineStackSource *temp = vtkpxSplineStackSource::New();
  return static_cast<ClientData>(temp);
}

int vtkPolyDataSourceCppCommand(vtkPolyDataSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxSplineStackSourceCppCommand(vtkpxSplineStackSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxSplineStackSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxSplineStackSourceCppCommand(static_cast<vtkpxSplineStackSource *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxSplineStackSourceCppCommand(vtkpxSplineStackSource *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxSplineStackSource",argv[1]))
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
    vtkpxSplineStackSource  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineStackSource");
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
    vtkpxSplineStackSource  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineStackSource");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxSplineStackSource  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineStackSource");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetEllipticalCylinder",argv[1]))&&(argc == 10))
    {
    int      temp0;
    int      temp1;
    float    temp2;
    float    temp3;
    float    temp4;
    float    temp5;
    float    temp6;
    float    temp7;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
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
    if (!error)
    {
    op->SetEllipticalCylinder(temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetLizzardCylinder",argv[1]))&&(argc == 15))
    {
    int      temp0;
    int      temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    int      temp5;
    float    temp6;
    float    temp7;
    float    temp8;
    float    temp9;
    float    temp10;
    float    temp11;
    float    temp12;
    int      temp20;
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
    if (Tcl_GetDouble(interp,argv[8],&tempd) != TCL_OK) error = 1;
    temp6 = tempd;
    if (Tcl_GetDouble(interp,argv[9],&tempd) != TCL_OK) error = 1;
    temp7 = tempd;
    if (Tcl_GetDouble(interp,argv[10],&tempd) != TCL_OK) error = 1;
    temp8 = tempd;
    if (Tcl_GetDouble(interp,argv[11],&tempd) != TCL_OK) error = 1;
    temp9 = tempd;
    if (Tcl_GetDouble(interp,argv[12],&tempd) != TCL_OK) error = 1;
    temp10 = tempd;
    if (Tcl_GetDouble(interp,argv[13],&tempd) != TCL_OK) error = 1;
    temp11 = tempd;
    if (Tcl_GetDouble(interp,argv[14],&tempd) != TCL_OK) error = 1;
    temp12 = tempd;
    if (!error)
    {
    temp20 = (op)->SetLizzardCylinder(temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8,temp9,temp10,temp11,temp12);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Copy",argv[1]))&&(argc == 4))
    {
    vtkpxSplineStackSource  *temp0;
    int      temp1;
    error = 0;

    temp0 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    op->Copy(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Copy",argv[1]))&&(argc == 3))
    {
    vtkpxSplineStackSource  *temp0;
    error = 0;

    temp0 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    if (!error)
    {
    op->Copy(temp0);
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
  if ((!strcmp("GetDrawMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetDrawMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetDrawMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetDrawMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetResolutionU",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetResolutionU();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetResolutionU",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetResolutionU(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetZDisplayOffset",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetZDisplayOffset();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetZDisplayOffset",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetZDisplayOffset(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetResolutionV",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetResolutionV();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetResolutionV",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetResolutionV(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPolarMiddle",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetPolarMiddle();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPolarMiddle",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetPolarMiddle(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetPolarMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetPolarMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetPolarMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetPolarMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetWholeSurface",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetWholeSurface();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetWholeSurface",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetWholeSurface(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetRedrawSurface",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetRedrawSurface(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetRedrawSurface",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetRedrawSurface();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTrimBottom",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTrimBottom();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetTrimTop",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetTrimTop();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetTrimBottom",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetTrimBottom(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetTrimTop",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetTrimTop(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumSplines",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumSplines();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetZSpacing",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetZSpacing();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetSpline",argv[1]))&&(argc == 4))
    {
    vtkpxSplineSource  *temp0;
    int      temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkpxSplineSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSplineSource"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->SetSpline(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetSplineFromSplineStack",argv[1]))&&(argc == 4))
    {
    vtkpxSplineSource  *temp0;
    int      temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkpxSplineSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSplineSource"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->SetSplineFromSplineStack(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetApex",argv[1]))&&(argc == 5))
    {
    float    temp0;
    float    temp1;
    float    temp2;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    op->SetApex(temp0,temp1,temp2);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("EnableApex",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->EnableApex(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ApexEnabled",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->ApexEnabled();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("AutoSetApex",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->AutoSetApex(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Resample",argv[1]))&&(argc == 4))
    {
    float    temp0;
    int      temp1;
    int      temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->Resample(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Resample",argv[1]))&&(argc == 3))
    {
    int      temp0;
    int      temp20;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    temp20 = (op)->Resample(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Resample",argv[1]))&&(argc == 3))
    {
    float    temp0;
    int      temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    temp20 = (op)->Resample(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ChangeSpacing",argv[1]))&&(argc == 7))
    {
    float    temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    int      temp4;
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
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->ChangeSpacing(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ScaleShiftZ",argv[1]))&&(argc == 4))
    {
    float    temp0;
    float    temp1;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    op->ScaleShiftZ(temp0,temp1);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ScaleShiftXY",argv[1]))&&(argc == 6))
    {
    float    temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (!error)
    {
    op->ScaleShiftXY(temp0,temp1,temp2,temp3);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("AdjustNumberOfSplines",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->AdjustNumberOfSplines(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetVolume",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetVolume();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetVOIVolume",argv[1]))&&(argc == 5))
    {
    vtkImageData  *temp0;
    int      temp1;
    float    temp2;
    float    temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->GetVOIVolume(temp0,temp1,temp2);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("AddToObjectMap",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    float    temp1;
    int      temp2;
    int      temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->AddToObjectMap(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateObjectMap",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    int      temp1;
    int      temp2;
    int      temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (!error)
    {
    temp20 = (op)->CreateObjectMap(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateBoundedObjectMap",argv[1]))&&(argc == 8))
    {
    vtkpxSplineStackSource  *temp0;
    vtkImageData  *temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    int      temp5;
    int      temp20;
    error = 0;

    temp0 = (vtkpxSplineStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSplineStackSource"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
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
    temp20 = (op)->CreateBoundedObjectMap(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FillDistanceMap",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->FillDistanceMap(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FlipX",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->FlipX(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("LoadAndFitContourStack",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    op->LoadAndFitContourStack(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("UpSample",argv[1]))&&(argc == 2))
    {
    op->UpSample();
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
  if ((!strcmp("CreateSurfaceFromImage",argv[1]))&&(argc == 8))
    {
    vtkImageData  *temp0;
    int      temp1;
    int      temp2;
    int      temp3;
    float    temp4;
    float    temp5;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (!error)
    {
    temp20 = (op)->CreateSurfaceFromImage(temp0,temp1,temp2,temp3,temp4,temp5);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("PreprocessBinaryImage",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    int      temp1;
    vtkImageData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->PreprocessBinaryImage(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkImageData");
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxSplineStackSourceCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkPolyDataSourceCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxSplineStackSource:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetEllipticalCylinder\t with 8 args\n",NULL);
    Tcl_AppendResult(interp,"  SetLizzardCylinder\t with 13 args\n",NULL);
    Tcl_AppendResult(interp,"  Copy\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Copy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDrawMode\n",NULL);
    Tcl_AppendResult(interp,"  SetDrawMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetResolutionU\n",NULL);
    Tcl_AppendResult(interp,"  SetResolutionU\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetZDisplayOffset\n",NULL);
    Tcl_AppendResult(interp,"  SetZDisplayOffset\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetResolutionV\n",NULL);
    Tcl_AppendResult(interp,"  SetResolutionV\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPolarMiddle\n",NULL);
    Tcl_AppendResult(interp,"  SetPolarMiddle\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetPolarMode\n",NULL);
    Tcl_AppendResult(interp,"  SetPolarMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetWholeSurface\n",NULL);
    Tcl_AppendResult(interp,"  SetWholeSurface\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetRedrawSurface\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetRedrawSurface\n",NULL);
    Tcl_AppendResult(interp,"  GetTrimBottom\n",NULL);
    Tcl_AppendResult(interp,"  GetTrimTop\n",NULL);
    Tcl_AppendResult(interp,"  SetTrimBottom\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetTrimTop\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumSplines\n",NULL);
    Tcl_AppendResult(interp,"  GetZSpacing\n",NULL);
    Tcl_AppendResult(interp,"  SetSpline\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetSplineFromSplineStack\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetApex\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  EnableApex\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ApexEnabled\n",NULL);
    Tcl_AppendResult(interp,"  AutoSetApex\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Resample\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Resample\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Resample\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ChangeSpacing\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  ScaleShiftZ\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  ScaleShiftXY\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  AdjustNumberOfSplines\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetVolume\n",NULL);
    Tcl_AppendResult(interp,"  GetVOIVolume\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  AddToObjectMap\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateObjectMap\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateBoundedObjectMap\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  FillDistanceMap\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  FlipX\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  LoadAndFitContourStack\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  UpSample\n",NULL);
    Tcl_AppendResult(interp,"  CreateSurfaceFromImage\t with 6 args\n",NULL);
    Tcl_AppendResult(interp,"  PreprocessBinaryImage\t with 2 args\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetEllipticalCylinder" );
    Tcl_DStringAppendElement ( &dString, "SetLizzardCylinder" );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "GetDrawMode" );
    Tcl_DStringAppendElement ( &dString, "SetDrawMode" );
    Tcl_DStringAppendElement ( &dString, "GetResolutionU" );
    Tcl_DStringAppendElement ( &dString, "SetResolutionU" );
    Tcl_DStringAppendElement ( &dString, "GetZDisplayOffset" );
    Tcl_DStringAppendElement ( &dString, "SetZDisplayOffset" );
    Tcl_DStringAppendElement ( &dString, "GetResolutionV" );
    Tcl_DStringAppendElement ( &dString, "SetResolutionV" );
    Tcl_DStringAppendElement ( &dString, "GetPolarMiddle" );
    Tcl_DStringAppendElement ( &dString, "SetPolarMiddle" );
    Tcl_DStringAppendElement ( &dString, "GetPolarMode" );
    Tcl_DStringAppendElement ( &dString, "SetPolarMode" );
    Tcl_DStringAppendElement ( &dString, "GetWholeSurface" );
    Tcl_DStringAppendElement ( &dString, "SetWholeSurface" );
    Tcl_DStringAppendElement ( &dString, "SetRedrawSurface" );
    Tcl_DStringAppendElement ( &dString, "GetRedrawSurface" );
    Tcl_DStringAppendElement ( &dString, "GetTrimBottom" );
    Tcl_DStringAppendElement ( &dString, "GetTrimTop" );
    Tcl_DStringAppendElement ( &dString, "SetTrimBottom" );
    Tcl_DStringAppendElement ( &dString, "SetTrimTop" );
    Tcl_DStringAppendElement ( &dString, "GetNumSplines" );
    Tcl_DStringAppendElement ( &dString, "GetZSpacing" );
    Tcl_DStringAppendElement ( &dString, "SetSpline" );
    Tcl_DStringAppendElement ( &dString, "SetSplineFromSplineStack" );
    Tcl_DStringAppendElement ( &dString, "SetApex" );
    Tcl_DStringAppendElement ( &dString, "EnableApex" );
    Tcl_DStringAppendElement ( &dString, "ApexEnabled" );
    Tcl_DStringAppendElement ( &dString, "AutoSetApex" );
    Tcl_DStringAppendElement ( &dString, "Resample" );
    Tcl_DStringAppendElement ( &dString, "Resample" );
    Tcl_DStringAppendElement ( &dString, "Resample" );
    Tcl_DStringAppendElement ( &dString, "ChangeSpacing" );
    Tcl_DStringAppendElement ( &dString, "ScaleShiftZ" );
    Tcl_DStringAppendElement ( &dString, "ScaleShiftXY" );
    Tcl_DStringAppendElement ( &dString, "AdjustNumberOfSplines" );
    Tcl_DStringAppendElement ( &dString, "GetVolume" );
    Tcl_DStringAppendElement ( &dString, "GetVOIVolume" );
    Tcl_DStringAppendElement ( &dString, "AddToObjectMap" );
    Tcl_DStringAppendElement ( &dString, "CreateObjectMap" );
    Tcl_DStringAppendElement ( &dString, "CreateBoundedObjectMap" );
    Tcl_DStringAppendElement ( &dString, "FillDistanceMap" );
    Tcl_DStringAppendElement ( &dString, "FlipX" );
    Tcl_DStringAppendElement ( &dString, "LoadAndFitContourStack" );
    Tcl_DStringAppendElement ( &dString, "UpSample" );
    Tcl_DStringAppendElement ( &dString, "CreateSurfaceFromImage" );
    Tcl_DStringAppendElement ( &dString, "PreprocessBinaryImage" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxSplineStackSource *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetEllipticalCylinder */
    if ( strcmp ( argv[2], "SetEllipticalCylinder" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetEllipticalCylinder" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetEllipticalCylinder */
    Tcl_DStringAppendElement ( &dString, " Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetEllipticalCylinder(int np, int numslices, float radiusx=10.0, float radiusy=5.0, float x=50.0, float y=50.0, float zspacing=1.0, float bottomz=0.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetEllipticalCylinder */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetLizzardCylinder */
    if ( strcmp ( argv[2], "SetLizzardCylinder" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetLizzardCylinder" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetLizzardCylinder */
    Tcl_DStringAppendElement ( &dString, " Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int SetLizzardCylinder(int np, int numslices, int bottomtrim, int lizzardslices, int blendslices, int thinslices, float minradius, float maxradius, float yscale=1.2, float x=50.0, float y=50.0, float zspacing=1.0, float bottomz=0.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetLizzardCylinder */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Copy */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void Copy(vtkpxSplineStackSource *other, int ignoretrim);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for Copy */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Copy */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void Copy(vtkpxSplineStackSource *other);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for Copy */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for Save */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetDrawMode */
    if ( strcmp ( argv[2], "GetDrawMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetDrawMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetDrawMode */
    Tcl_DStringAppendElement ( &dString, " Display Parameter \n" );
    Tcl_DStringAppendElement ( &dString, "int GetDrawMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetDrawMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetDrawMode */
    if ( strcmp ( argv[2], "SetDrawMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetDrawMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetDrawMode */
    Tcl_DStringAppendElement ( &dString, " Display Parameter \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetDrawMode(int md);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetDrawMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetResolutionU */
    if ( strcmp ( argv[2], "GetResolutionU" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetResolutionU" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetResolutionU */
    Tcl_DStringAppendElement ( &dString, " Sampling Rate in-plane \n" );
    Tcl_DStringAppendElement ( &dString, "float GetResolutionU();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetResolutionU */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetResolutionU */
    if ( strcmp ( argv[2], "SetResolutionU" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetResolutionU" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetResolutionU */
    Tcl_DStringAppendElement ( &dString, " Sampling Rate in-plane \n" );
    Tcl_DStringAppendElement ( &dString, "void SetResolutionU(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetResolutionU */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetZDisplayOffset */
    if ( strcmp ( argv[2], "GetZDisplayOffset" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetZDisplayOffset" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetZDisplayOffset */
    Tcl_DStringAppendElement ( &dString, " Display Offset for top & bottom slices to make them visible\n" );
    Tcl_DStringAppendElement ( &dString, "float GetZDisplayOffset();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetZDisplayOffset */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetZDisplayOffset */
    if ( strcmp ( argv[2], "SetZDisplayOffset" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetZDisplayOffset" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetZDisplayOffset */
    Tcl_DStringAppendElement ( &dString, " Display Offset for top & bottom slices to make them visible\n" );
    Tcl_DStringAppendElement ( &dString, "void SetZDisplayOffset(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetZDisplayOffset */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetResolutionV */
    if ( strcmp ( argv[2], "GetResolutionV" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetResolutionV" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetResolutionV */
    Tcl_DStringAppendElement ( &dString, " Sampling Rate out-of-plane\n" );
    Tcl_DStringAppendElement ( &dString, "float GetResolutionV();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetResolutionV */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetResolutionV */
    if ( strcmp ( argv[2], "SetResolutionV" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetResolutionV" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetResolutionV */
    Tcl_DStringAppendElement ( &dString, " Sampling Rate out-of-plane\n" );
    Tcl_DStringAppendElement ( &dString, "void SetResolutionV(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetResolutionV */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPolarMiddle */
    if ( strcmp ( argv[2], "GetPolarMiddle" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPolarMiddle" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPolarMiddle */
    Tcl_DStringAppendElement ( &dString, " Center of rotation for polar (3D ultrasound) spline \n" );
    Tcl_DStringAppendElement ( &dString, "float GetPolarMiddle();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetPolarMiddle */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPolarMiddle */
    if ( strcmp ( argv[2], "SetPolarMiddle" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPolarMiddle" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPolarMiddle */
    Tcl_DStringAppendElement ( &dString, " Center of rotation for polar (3D ultrasound) spline \n" );
    Tcl_DStringAppendElement ( &dString, "void SetPolarMiddle(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetPolarMiddle */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetPolarMode */
    if ( strcmp ( argv[2], "GetPolarMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetPolarMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetPolarMode */
    Tcl_DStringAppendElement ( &dString, " Set/Get Polar Mode \n" );
    Tcl_DStringAppendElement ( &dString, "int GetPolarMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetPolarMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetPolarMode */
    if ( strcmp ( argv[2], "SetPolarMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetPolarMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetPolarMode */
    Tcl_DStringAppendElement ( &dString, " Set/Get Polar Mode \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetPolarMode(int pm);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetPolarMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWholeSurface */
    if ( strcmp ( argv[2], "GetWholeSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWholeSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWholeSurface */
    Tcl_DStringAppendElement ( &dString, " Update Whole Surface \n" );
    Tcl_DStringAppendElement ( &dString, "int GetWholeSurface();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetWholeSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetWholeSurface */
    if ( strcmp ( argv[2], "SetWholeSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetWholeSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetWholeSurface */
    Tcl_DStringAppendElement ( &dString, " Update Whole Surface \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetWholeSurface(int ws);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetWholeSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetRedrawSurface */
    if ( strcmp ( argv[2], "SetRedrawSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetRedrawSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetRedrawSurface */
    Tcl_DStringAppendElement ( &dString, " Force Re-Draw of Surface \n" );
    Tcl_DStringAppendElement ( &dString, "void SetRedrawSurface(int a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetRedrawSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetRedrawSurface */
    if ( strcmp ( argv[2], "GetRedrawSurface" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetRedrawSurface" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetRedrawSurface */
    Tcl_DStringAppendElement ( &dString, " Force Re-Draw of Surface \n" );
    Tcl_DStringAppendElement ( &dString, "int GetRedrawSurface();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetRedrawSurface */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTrimBottom */
    if ( strcmp ( argv[2], "GetTrimBottom" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTrimBottom" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTrimBottom */
    Tcl_DStringAppendElement ( &dString, " Trims are extent of spline in image numsplines = numslices \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetTrimBottom();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetTrimBottom */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetTrimTop */
    if ( strcmp ( argv[2], "GetTrimTop" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetTrimTop" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetTrimTop */
    Tcl_DStringAppendElement ( &dString, " Trims are extent of spline in image numsplines = numslices \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetTrimTop();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetTrimTop */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTrimBottom */
    if ( strcmp ( argv[2], "SetTrimBottom" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTrimBottom" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTrimBottom */
    Tcl_DStringAppendElement ( &dString, " Trims are extent of spline in image numsplines = numslices \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTrimBottom(int tb);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetTrimBottom */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetTrimTop */
    if ( strcmp ( argv[2], "SetTrimTop" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetTrimTop" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetTrimTop */
    Tcl_DStringAppendElement ( &dString, " Trims are extent of spline in image numsplines = numslices \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetTrimTop(int tt);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetTrimTop */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumSplines */
    if ( strcmp ( argv[2], "GetNumSplines" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumSplines" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumSplines */
    Tcl_DStringAppendElement ( &dString, " Trims are extent of spline in image numsplines = numslices \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumSplines();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetNumSplines */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetZSpacing */
    if ( strcmp ( argv[2], "GetZSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetZSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetZSpacing */
    Tcl_DStringAppendElement ( &dString, " Trims are extent of spline in image numsplines = numslices \n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetZSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetZSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSpline */
    if ( strcmp ( argv[2], "SetSpline" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSpline" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSpline */
    Tcl_DStringAppendElement ( &dString, " Set Spline \n" );
    Tcl_DStringAppendElement ( &dString, "int SetSpline(vtkpxSplineSource *source, int ind);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetSpline */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetSplineFromSplineStack */
    if ( strcmp ( argv[2], "SetSplineFromSplineStack" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetSplineFromSplineStack" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetSplineFromSplineStack */
    Tcl_DStringAppendElement ( &dString, " Set Spline \n" );
    Tcl_DStringAppendElement ( &dString, "int SetSplineFromSplineStack(vtkpxSplineSource *source, int ind);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetSplineFromSplineStack */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetApex */
    if ( strcmp ( argv[2], "SetApex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetApex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetApex */
    Tcl_DStringAppendElement ( &dString, " Cardiac Related Isuues, to set Apex \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetApex(float x, float y, float z);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for SetApex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: EnableApex */
    if ( strcmp ( argv[2], "EnableApex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "EnableApex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for EnableApex */
    Tcl_DStringAppendElement ( &dString, " Cardiac Related Isuues, to set Apex \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void EnableApex(int enable=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for EnableApex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ApexEnabled */
    if ( strcmp ( argv[2], "ApexEnabled" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ApexEnabled" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ApexEnabled */
    Tcl_DStringAppendElement ( &dString, " Cardiac Related Isuues, to set Apex \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ApexEnabled();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for ApexEnabled */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AutoSetApex */
    if ( strcmp ( argv[2], "AutoSetApex" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AutoSetApex" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AutoSetApex */
    Tcl_DStringAppendElement ( &dString, " Cardiac Related Isuues, to set Apex \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void AutoSetApex(int active=1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for AutoSetApex */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Resample */
    if ( strcmp ( argv[2], "Resample" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Resample" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Resample */
    Tcl_DStringAppendElement ( &dString, " Resample and Smooth Spline \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Resample(float ds_step, int npoints);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for Resample */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Resample */
    if ( strcmp ( argv[2], "Resample" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Resample" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Resample */
    Tcl_DStringAppendElement ( &dString, " Resample and Smooth Spline \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Resample(int npoints);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for Resample */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Resample */
    if ( strcmp ( argv[2], "Resample" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Resample" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Resample */
    Tcl_DStringAppendElement ( &dString, " Resample and Smooth Spline \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Resample(float ds);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for Resample */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ChangeSpacing */
    if ( strcmp ( argv[2], "ChangeSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ChangeSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ChangeSpacing */
    Tcl_DStringAppendElement ( &dString, " Resample and Smooth Spline \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ChangeSpacing(float smooth=0.5, float npoints=-1.0, float step=0.05, float csmooth=0.0, int iterations=50);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for ChangeSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScaleShiftZ */
    if ( strcmp ( argv[2], "ScaleShiftZ" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScaleShiftZ" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScaleShiftZ */
    Tcl_DStringAppendElement ( &dString, " Shift And Scale Z-Coordinate \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void ScaleShiftZ(float shift, float scale);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for ScaleShiftZ */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ScaleShiftXY */
    if ( strcmp ( argv[2], "ScaleShiftXY" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ScaleShiftXY" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ScaleShiftXY */
    Tcl_DStringAppendElement ( &dString, " Shift And Scale Z-Coordinate \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void ScaleShiftXY(float shiftx, float shifty, float scalex, float scaley);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for ScaleShiftXY */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AdjustNumberOfSplines */
    if ( strcmp ( argv[2], "AdjustNumberOfSplines" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AdjustNumberOfSplines" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AdjustNumberOfSplines */
    Tcl_DStringAppendElement ( &dString, " Shift And Scale Z-Coordinate \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void AdjustNumberOfSplines(int newnumber);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for AdjustNumberOfSplines */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetVolume */
    if ( strcmp ( argv[2], "GetVolume" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetVolume" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetVolume */
    Tcl_DStringAppendElement ( &dString, " VOI Stuff area, mean intensity area>0 std of mean intensity \n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetVolume();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetVolume */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetVOIVolume */
    if ( strcmp ( argv[2], "GetVOIVolume" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetVOIVolume" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetVOIVolume */
    Tcl_DStringAppendElement ( &dString, " VOI Stuff area, mean intensity area>0 std of mean intensity \n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetVOIVolume(vtkImageData *image, int frame, float threshold=0.1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for GetVOIVolume */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AddToObjectMap */
    if ( strcmp ( argv[2], "AddToObjectMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AddToObjectMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddToObjectMap */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int AddToObjectMap(vtkImageData *objectmap, float value=1.0, int add=0, int background=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for AddToObjectMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateObjectMap */
    if ( strcmp ( argv[2], "CreateObjectMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateObjectMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateObjectMap */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int CreateObjectMap(vtkImageData *objectmap, int inside=2, int outside=1, int offslice=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for CreateObjectMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateBoundedObjectMap */
    if ( strcmp ( argv[2], "CreateBoundedObjectMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateBoundedObjectMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateBoundedObjectMap */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int CreateBoundedObjectMap(vtkpxSplineStackSource *bounding, vtkImageData *objectmap, int inside_below=2, int inside_above=3, int outside=1, int offslice=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for CreateBoundedObjectMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FillDistanceMap */
    if ( strcmp ( argv[2], "FillDistanceMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FillDistanceMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FillDistanceMap */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int FillDistanceMap(vtkImageData *distancemap);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for FillDistanceMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FlipX */
    if ( strcmp ( argv[2], "FlipX" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FlipX" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FlipX */
    Tcl_DStringAppendElement ( &dString, " Flip Left\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int FlipX(vtkImageData *image);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for FlipX */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LoadAndFitContourStack */
    if ( strcmp ( argv[2], "LoadAndFitContourStack" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LoadAndFitContourStack" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LoadAndFitContourStack */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual void LoadAndFitContourStack(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for LoadAndFitContourStack */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UpSample */
    if ( strcmp ( argv[2], "UpSample" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UpSample" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UpSample */
    Tcl_DStringAppendElement ( &dString, " Upsample, i.e. from interpoated tracings add in-between slice\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void UpSample();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for UpSample */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateSurfaceFromImage */
    if ( strcmp ( argv[2], "CreateSurfaceFromImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateSurfaceFromImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateSurfaceFromImage */
    Tcl_DStringAppendElement ( &dString, " \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int CreateSurfaceFromImage(vtkImageData *img, int minslice, int maxslice, int frame, float curvesmooth=1.0, float sigma=1.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for CreateSurfaceFromImage */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: PreprocessBinaryImage */
    if ( strcmp ( argv[2], "PreprocessBinaryImage" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "PreprocessBinaryImage" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for PreprocessBinaryImage */
    Tcl_DStringAppendElement ( &dString, " A few static methods for Zak\n" );
    Tcl_DStringAppendElement ( &dString, "static vtkImageData *PreprocessBinaryImage(vtkImageData *img, int frame);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineStackSource" );
    /* Closing for PreprocessBinaryImage */

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
