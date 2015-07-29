// tcl wrapper for vtkpxSplineSource object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxSplineSource.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxSplineSourceNewCommand()
{
  vtkpxSplineSource *temp = vtkpxSplineSource::New();
  return static_cast<ClientData>(temp);
}

int vtkPolyDataSourceCppCommand(vtkPolyDataSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxSplineSourceCppCommand(vtkpxSplineSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxSplineSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxSplineSourceCppCommand(static_cast<vtkpxSplineSource *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxSplineSourceCppCommand(vtkpxSplineSource *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxSplineSource",argv[1]))
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
    vtkpxSplineSource  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineSource");
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
    vtkpxSplineSource  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineSource");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxSplineSource  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxSplineSource");
    return TCL_OK;
    }
    }
  if ((!strcmp("SetEllipse",argv[1]))&&(argc == 9))
    {
    int      temp0;
    float    temp1;
    float    temp2;
    float    temp3;
    float    temp4;
    float    temp5;
    int      temp6;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
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
    if (Tcl_GetInt(interp,argv[8],&tempi) != TCL_OK) error = 1;
    temp6 = tempi;
    if (!error)
    {
    op->SetEllipse(temp0,temp1,temp2,temp3,temp4,temp5,temp6);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Copy",argv[1]))&&(argc == 3))
    {
    vtkpxSplineSource  *temp0;
    error = 0;

    temp0 = (vtkpxSplineSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxSplineSource"),interp,error));
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
  if ((!strcmp("GetResolution",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetResolution();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetResolution",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetResolution(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumPoints",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumPoints();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("HasSnake",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->HasSnake();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetZlevel",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetZlevel();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetZlevel",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->SetZlevel(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("ExportToPoints",argv[1]))&&(argc == 3))
    {
    vtkPoints  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    if (!error)
    {
    temp20 = (op)->ExportToPoints(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ExportKnotPointsToPoints",argv[1]))&&(argc == 3))
    {
    vtkPoints  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    if (!error)
    {
    temp20 = (op)->ExportKnotPointsToPoints(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FitPoints",argv[1]))&&(argc == 7))
    {
    vtkPoints  *temp0;
    float    temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->FitPoints(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("FitPoints",argv[1]))&&(argc == 5))
    {
    vtkPoints  *temp0;
    float    temp1;
    int      temp2;
    int      temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (!error)
    {
    temp20 = (op)->FitPoints(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetControlPoints",argv[1]))&&(argc == 4))
    {
    vtkPoints  *temp0;
    int      temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->SetControlPoints(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetControlPoints",argv[1]))&&(argc == 3))
    {
    vtkPoints  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkPoints *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkPoints"),interp,error));
    if (!error)
    {
    temp20 = (op)->SetControlPoints(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("ExportToBaseCurve",argv[1]))&&(argc == 3))
    {
    vtkpxBaseCurve  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkpxBaseCurve *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxBaseCurve"),interp,error));
    if (!error)
    {
    temp20 = (op)->ExportToBaseCurve(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SetFromBaseCurve",argv[1]))&&(argc == 3))
    {
    vtkpxBaseCurve  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkpxBaseCurve *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxBaseCurve"),interp,error));
    if (!error)
    {
    temp20 = (op)->SetFromBaseCurve(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
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
    int      temp1;
    float    temp2;
    float    temp3;
    int      temp4;
    int      temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
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
  if ((!strcmp("GetImageSpacing",argv[1]))&&(argc == 2))
    {
    double  *temp20;
    temp20 = (op)->GetImageSpacing();
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
  if ((!strcmp("SetImageSpacing",argv[1]))&&(argc == 5))
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
    op->SetImageSpacing(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetImageOrigin",argv[1]))&&(argc == 2))
    {
    double  *temp20;
    temp20 = (op)->GetImageOrigin();
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
  if ((!strcmp("SetImageOrigin",argv[1]))&&(argc == 5))
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
    op->SetImageOrigin(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("Transform",argv[1]))&&(argc == 3))
    {
    vtkTransform  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkTransform *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkTransform"),interp,error));
    if (!error)
    {
    temp20 = (op)->Transform(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateSnake",argv[1]))&&(argc == 10))
    {
    vtkImageData  *temp0;
    int      temp1;
    int      temp2;
    float    temp3;
    float    temp4;
    float    temp5;
    float    temp6;
    int      temp7;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (Tcl_GetDouble(interp,argv[8],&tempd) != TCL_OK) error = 1;
    temp6 = tempd;
    if (Tcl_GetInt(interp,argv[9],&tempi) != TCL_OK) error = 1;
    temp7 = tempi;
    if (!error)
    {
    op->CreateSnake(temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("CreateSnake",argv[1]))&&(argc == 7))
    {
    vtkImageData  *temp0;
    int      temp1;
    int      temp2;
    float    temp3;
    float    temp4;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (!error)
    {
    op->CreateSnake(temp0,temp1,temp2,temp3,temp4);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("UpdateSnake",argv[1]))&&(argc == 4))
    {
    float    temp0;
    float    temp1;
    float    temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->UpdateSnake(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("UpdateSnake",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->UpdateSnake();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetArea",argv[1]))&&(argc == 2))
    {
    float    temp20;
    temp20 = (op)->GetArea();
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetVOIArea",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    int      temp1;
    int      temp2;
    float    temp3;
    float    temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetDouble(interp,argv[5],&tempd) != TCL_OK) error = 1;
    temp3 = tempd;
    if (!error)
    {
    temp20 = (op)->GetVOIArea(temp0,temp1,temp2,temp3);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("InsertKnot",argv[1]))&&(argc == 4))
    {
    float    temp0;
    float    temp1;
    int      temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->InsertKnot(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("InsertKnot",argv[1]))&&(argc == 5))
    {
    float    temp0;
    float    temp1;
    float    temp2;
    int      temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
    if (!error)
    {
    temp20 = (op)->InsertKnot(temp0,temp1,temp2);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("AddToObjectMap",argv[1]))&&(argc == 6))
    {
    vtkImageData  *temp0;
    int      temp1;
    float    temp2;
    int      temp3;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (Tcl_GetDouble(interp,argv[4],&tempd) != TCL_OK) error = 1;
    temp2 = tempd;
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
  if ((!strcmp("ThresholdAddToObjectMap",argv[1]))&&(argc == 10))
    {
    vtkImageData  *temp0;
    vtkImageData  *temp1;
    int      temp2;
    int      temp3;
    float    temp4;
    int      temp5;
    float    temp6;
    float    temp7;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    temp1 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetDouble(interp,argv[6],&tempd) != TCL_OK) error = 1;
    temp4 = tempd;
    if (Tcl_GetInt(interp,argv[7],&tempi) != TCL_OK) error = 1;
    temp5 = tempi;
    if (Tcl_GetDouble(interp,argv[8],&tempd) != TCL_OK) error = 1;
    temp6 = tempd;
    if (Tcl_GetDouble(interp,argv[9],&tempd) != TCL_OK) error = 1;
    temp7 = tempd;
    if (!error)
    {
    temp20 = (op)->ThresholdAddToObjectMap(temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("AddToDistanceMap",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    int      temp1;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetInt(interp,argv[3],&tempi) != TCL_OK) error = 1;
    temp1 = tempi;
    if (!error)
    {
    temp20 = (op)->AddToDistanceMap(temp0,temp1);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
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

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxSplineSourceCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkPolyDataSourceCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxSplineSource:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetEllipse\t with 7 args\n",NULL);
    Tcl_AppendResult(interp,"  Copy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetResolution\n",NULL);
    Tcl_AppendResult(interp,"  SetResolution\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumPoints\n",NULL);
    Tcl_AppendResult(interp,"  HasSnake\n",NULL);
    Tcl_AppendResult(interp,"  GetZlevel\n",NULL);
    Tcl_AppendResult(interp,"  SetZlevel\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ExportToPoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ExportKnotPointsToPoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  FitPoints\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  FitPoints\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  SetControlPoints\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  SetControlPoints\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ExportToBaseCurve\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetFromBaseCurve\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Resample\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Resample\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Resample\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  ChangeSpacing\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  GetImageSpacing\n",NULL);
    Tcl_AppendResult(interp,"  SetImageSpacing\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  GetImageOrigin\n",NULL);
    Tcl_AppendResult(interp,"  SetImageOrigin\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  Transform\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  CreateSnake\t with 8 args\n",NULL);
    Tcl_AppendResult(interp,"  CreateSnake\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  UpdateSnake\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  UpdateSnake\n",NULL);
    Tcl_AppendResult(interp,"  GetArea\n",NULL);
    Tcl_AppendResult(interp,"  GetVOIArea\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  InsertKnot\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  InsertKnot\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  AddToObjectMap\t with 4 args\n",NULL);
    Tcl_AppendResult(interp,"  ThresholdAddToObjectMap\t with 8 args\n",NULL);
    Tcl_AppendResult(interp,"  AddToDistanceMap\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  OptimizerFeedback\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateGradient\t with 3 args\n",NULL);
    Tcl_AppendResult(interp,"  CalculateFunction\t with 2 args\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "SetEllipse" );
    Tcl_DStringAppendElement ( &dString, "Copy" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "GetResolution" );
    Tcl_DStringAppendElement ( &dString, "SetResolution" );
    Tcl_DStringAppendElement ( &dString, "GetNumPoints" );
    Tcl_DStringAppendElement ( &dString, "HasSnake" );
    Tcl_DStringAppendElement ( &dString, "GetZlevel" );
    Tcl_DStringAppendElement ( &dString, "SetZlevel" );
    Tcl_DStringAppendElement ( &dString, "ExportToPoints" );
    Tcl_DStringAppendElement ( &dString, "ExportKnotPointsToPoints" );
    Tcl_DStringAppendElement ( &dString, "FitPoints" );
    Tcl_DStringAppendElement ( &dString, "FitPoints" );
    Tcl_DStringAppendElement ( &dString, "SetControlPoints" );
    Tcl_DStringAppendElement ( &dString, "SetControlPoints" );
    Tcl_DStringAppendElement ( &dString, "ExportToBaseCurve" );
    Tcl_DStringAppendElement ( &dString, "SetFromBaseCurve" );
    Tcl_DStringAppendElement ( &dString, "Resample" );
    Tcl_DStringAppendElement ( &dString, "Resample" );
    Tcl_DStringAppendElement ( &dString, "Resample" );
    Tcl_DStringAppendElement ( &dString, "ChangeSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetImageSpacing" );
    Tcl_DStringAppendElement ( &dString, "SetImageSpacing" );
    Tcl_DStringAppendElement ( &dString, "GetImageOrigin" );
    Tcl_DStringAppendElement ( &dString, "SetImageOrigin" );
    Tcl_DStringAppendElement ( &dString, "Transform" );
    Tcl_DStringAppendElement ( &dString, "CreateSnake" );
    Tcl_DStringAppendElement ( &dString, "CreateSnake" );
    Tcl_DStringAppendElement ( &dString, "UpdateSnake" );
    Tcl_DStringAppendElement ( &dString, "UpdateSnake" );
    Tcl_DStringAppendElement ( &dString, "GetArea" );
    Tcl_DStringAppendElement ( &dString, "GetVOIArea" );
    Tcl_DStringAppendElement ( &dString, "InsertKnot" );
    Tcl_DStringAppendElement ( &dString, "InsertKnot" );
    Tcl_DStringAppendElement ( &dString, "AddToObjectMap" );
    Tcl_DStringAppendElement ( &dString, "ThresholdAddToObjectMap" );
    Tcl_DStringAppendElement ( &dString, "AddToDistanceMap" );
    Tcl_DStringAppendElement ( &dString, "OptimizerFeedback" );
    Tcl_DStringAppendElement ( &dString, "CalculateGradient" );
    Tcl_DStringAppendElement ( &dString, "CalculateFunction" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxSplineSource *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for SafeDownCast */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetEllipse */
    if ( strcmp ( argv[2], "SetEllipse" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetEllipse" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetEllipse */
    Tcl_DStringAppendElement ( &dString, " Initialization -- Set as Ellipse and Copy Other \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetEllipse(int np, float radiusx=10.0, float radiusy=5.0, float x=50.0, float y=50.0, float z=0.0, int dim=2);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for SetEllipse */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Copy */
    Tcl_DStringAppendElement ( &dString, " Initialization -- Set as Ellipse and Copy Other \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Copy(vtkpxSplineSource *other);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, " Input/Output to standard .spl format \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Load(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, " Input/Output to standard .spl format \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Save(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for Save */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetResolution */
    if ( strcmp ( argv[2], "GetResolution" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetResolution" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetResolution */
    Tcl_DStringAppendElement ( &dString, " Display Parameter \n" );
    Tcl_DStringAppendElement ( &dString, "float GetResolution();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for GetResolution */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetResolution */
    if ( strcmp ( argv[2], "SetResolution" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetResolution" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetResolution */
    Tcl_DStringAppendElement ( &dString, " Display Parameter \n" );
    Tcl_DStringAppendElement ( &dString, "void SetResolution(float a);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for SetResolution */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumPoints */
    if ( strcmp ( argv[2], "GetNumPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumPoints */
    Tcl_DStringAppendElement ( &dString, " Information regarding spline \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumPoints();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for GetNumPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: HasSnake */
    if ( strcmp ( argv[2], "HasSnake" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "HasSnake" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for HasSnake */
    Tcl_DStringAppendElement ( &dString, " Information regarding spline \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int HasSnake();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for HasSnake */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetZlevel */
    if ( strcmp ( argv[2], "GetZlevel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetZlevel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetZlevel */
    Tcl_DStringAppendElement ( &dString, " Zlevel is constant z-coordinate or slice level \n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetZlevel();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for GetZlevel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetZlevel */
    if ( strcmp ( argv[2], "SetZlevel" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetZlevel" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetZlevel */
    Tcl_DStringAppendElement ( &dString, " Zlevel is constant z-coordinate or slice level \n" );
    Tcl_DStringAppendElement ( &dString, "virtual void SetZlevel(float lev);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for SetZlevel */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ExportToPoints */
    if ( strcmp ( argv[2], "ExportToPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ExportToPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ExportToPoints */
    Tcl_DStringAppendElement ( &dString, " Interface to vtkPoints \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ExportToPoints(vtkPoints *points);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for ExportToPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ExportKnotPointsToPoints */
    if ( strcmp ( argv[2], "ExportKnotPointsToPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ExportKnotPointsToPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ExportKnotPointsToPoints */
    Tcl_DStringAppendElement ( &dString, " Interface to vtkPoints \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ExportKnotPointsToPoints(vtkPoints *points);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for ExportKnotPointsToPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FitPoints */
    if ( strcmp ( argv[2], "FitPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FitPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FitPoints */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int FitPoints(vtkPoints *points, float smooth, int numpoints, int closed, int iterations);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for FitPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: FitPoints */
    if ( strcmp ( argv[2], "FitPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "FitPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for FitPoints */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int FitPoints(vtkPoints *points, float smooth=2.0, int numpoints=-1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for FitPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetControlPoints */
    if ( strcmp ( argv[2], "SetControlPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetControlPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetControlPoints */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int SetControlPoints(vtkPoints *points, int np);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for SetControlPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetControlPoints */
    if ( strcmp ( argv[2], "SetControlPoints" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetControlPoints" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkPoints" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetControlPoints */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int SetControlPoints(vtkPoints *points);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for SetControlPoints */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ExportToBaseCurve */
    if ( strcmp ( argv[2], "ExportToBaseCurve" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ExportToBaseCurve" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseCurve" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ExportToBaseCurve */
    Tcl_DStringAppendElement ( &dString, " Interface to vtkpxBaseCurve\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ExportToBaseCurve(vtkpxBaseCurve *baseCurve);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for ExportToBaseCurve */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetFromBaseCurve */
    if ( strcmp ( argv[2], "SetFromBaseCurve" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetFromBaseCurve" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxBaseCurve" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetFromBaseCurve */
    Tcl_DStringAppendElement ( &dString, " Interface to vtkpxBaseCurve\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int SetFromBaseCurve(vtkpxBaseCurve *baseCurve);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for SetFromBaseCurve */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ChangeSpacing */
    Tcl_DStringAppendElement ( &dString, " Resample and Smooth Spline \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ChangeSpacing(float smooth=0.5, int npoints=-1, float step=-1.0, float csmooth=0.0, int iterations=50);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for ChangeSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageSpacing */
    if ( strcmp ( argv[2], "GetImageSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageSpacing */
    Tcl_DStringAppendElement ( &dString, " Scale factors to scale from voxels (used for snake stuff) to mm \n" );
    Tcl_DStringAppendElement ( &dString, "double *GetImageSpacing();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for GetImageSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetImageSpacing */
    if ( strcmp ( argv[2], "SetImageSpacing" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetImageSpacing" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetImageSpacing */
    Tcl_DStringAppendElement ( &dString, " Scale factors to scale from voxels (used for snake stuff) to mm \n" );
    Tcl_DStringAppendElement ( &dString, "void SetImageSpacing(double a\\[3\\]);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for SetImageSpacing */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetImageOrigin */
    if ( strcmp ( argv[2], "GetImageOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetImageOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetImageOrigin */
    Tcl_DStringAppendElement ( &dString, " Shift factor to translate from voxels (used for snake stuff) to mm \n" );
    Tcl_DStringAppendElement ( &dString, "double *GetImageOrigin();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for GetImageOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetImageOrigin */
    if ( strcmp ( argv[2], "SetImageOrigin" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetImageOrigin" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetImageOrigin */
    Tcl_DStringAppendElement ( &dString, " Shift factor to translate from voxels (used for snake stuff) to mm \n" );
    Tcl_DStringAppendElement ( &dString, "void SetImageOrigin(double a\\[3\\]);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for SetImageOrigin */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Transform */
    if ( strcmp ( argv[2], "Transform" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Transform" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkTransform" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Transform */
    Tcl_DStringAppendElement ( &dString, " Apply Transformation to BSpline\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Transform(vtkTransform *Transform);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for Transform */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateSnake */
    if ( strcmp ( argv[2], "CreateSnake" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateSnake" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateSnake */
    Tcl_DStringAppendElement ( &dString, " Snake Stuff -- crude segmentation algorithm\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void CreateSnake(vtkImageData *image, int level, int frame, float img_coeff, float smooth_coeff, float timestep, float sigma, int niter);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for CreateSnake */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateSnake */
    if ( strcmp ( argv[2], "CreateSnake" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateSnake" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateSnake */
    Tcl_DStringAppendElement ( &dString, " Snake Stuff -- crude segmentation algorithm\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void CreateSnake(vtkImageData *image, int level, int frame, float img_coeff=5.0, float smooth_coeff=0.5);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for CreateSnake */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UpdateSnake */
    if ( strcmp ( argv[2], "UpdateSnake" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UpdateSnake" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UpdateSnake */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual float UpdateSnake(float img_coeff, float smooth_coeff);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for UpdateSnake */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: UpdateSnake */
    if ( strcmp ( argv[2], "UpdateSnake" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "UpdateSnake" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for UpdateSnake */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual float UpdateSnake();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for UpdateSnake */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetArea */
    if ( strcmp ( argv[2], "GetArea" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetArea" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetArea */
    Tcl_DStringAppendElement ( &dString, " VOI Stuff area, mean intensity area>0 std of mean intensity \n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetArea();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for GetArea */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetVOIArea */
    if ( strcmp ( argv[2], "GetVOIArea" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetVOIArea" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetVOIArea */
    Tcl_DStringAppendElement ( &dString, " VOI Stuff area, mean intensity area>0 std of mean intensity \n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetVOIArea(vtkImageData *image, int level, int frame, float threshold=0.1);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for GetVOIArea */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InsertKnot */
    if ( strcmp ( argv[2], "InsertKnot" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InsertKnot" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InsertKnot */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int InsertKnot(float x, float y);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for InsertKnot */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: InsertKnot */
    if ( strcmp ( argv[2], "InsertKnot" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "InsertKnot" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for InsertKnot */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int InsertKnot(float x, float y, float mindist);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for InsertKnot */

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
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddToObjectMap */
    Tcl_DStringAppendElement ( &dString, " Add To Objectmap\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int AddToObjectMap(vtkImageData *objectmap, int level=0, float value=1.0, int add=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for AddToObjectMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ThresholdAddToObjectMap */
    if ( strcmp ( argv[2], "ThresholdAddToObjectMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ThresholdAddToObjectMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ThresholdAddToObjectMap */
    Tcl_DStringAppendElement ( &dString, " Add To Objectmap\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ThresholdAddToObjectMap(vtkImageData *objectmap, vtkImageData *img, int level, int imagelevel, float value=1.0, int thresholdmode=0, float min=0.0, float max=0.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for ThresholdAddToObjectMap */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: AddToDistanceMap */
    if ( strcmp ( argv[2], "AddToDistanceMap" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "AddToDistanceMap" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for AddToDistanceMap */
    Tcl_DStringAppendElement ( &dString, " Add To Objectmap\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int AddToDistanceMap(vtkImageData *distancemap, int level=0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for AddToDistanceMap */

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
    Tcl_DStringAppendElement ( &dString, " Optimization Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "static void OptimizerFeedback(vtkObject *self, const char *line, double val);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, " Optimization Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "static double CalculateGradient(vtkObject *self, vtkDoubleArray *position, vtkDoubleArray *output);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
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
    Tcl_DStringAppendElement ( &dString, " Optimization Stuff\n" );
    Tcl_DStringAppendElement ( &dString, "static double CalculateFunction(vtkObject *self, vtkDoubleArray *position);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxSplineSource" );
    /* Closing for CalculateFunction */

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
