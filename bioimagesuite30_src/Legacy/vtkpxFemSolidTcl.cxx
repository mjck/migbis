// tcl wrapper for vtkpxFemSolid object
//
#define VTK_WRAPPING_CXX
#define VTK_STREAMS_FWD_ONLY
#include "vtkSystemIncludes.h"
#include "vtkpxFemSolid.h"

#include "vtkTclUtil.h"
#include "vtkStdString.h"
#include <stdexcept>
#include <vtksys/ios/sstream>

ClientData vtkpxFemSolidNewCommand()
{
  vtkpxFemSolid *temp = vtkpxFemSolid::New();
  return static_cast<ClientData>(temp);
}

int vtkPolyDataSourceCppCommand(vtkPolyDataSource *op, Tcl_Interp *interp,
             int argc, char *argv[]);
int VTKTCL_EXPORT vtkpxFemSolidCppCommand(vtkpxFemSolid *op, Tcl_Interp *interp,
             int argc, char *argv[]);

int VTKTCL_EXPORT vtkpxFemSolidCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[])
{
  if ((argc == 2)&&(!strcmp("Delete",argv[1]))&& !vtkTclInDelete(interp))
    {
    Tcl_DeleteCommand(interp,argv[0]);
    return TCL_OK;
    }
   return vtkpxFemSolidCppCommand(static_cast<vtkpxFemSolid *>(static_cast<vtkTclCommandArgStruct *>(cd)->Pointer),interp, argc, argv);
}

int VTKTCL_EXPORT vtkpxFemSolidCppCommand(vtkpxFemSolid *op, Tcl_Interp *interp,
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
      if (!strcmp("vtkpxFemSolid",argv[1]))
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
    vtkpxFemSolid  *temp20;
    temp20 = (op)->New();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxFemSolid");
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
    vtkpxFemSolid  *temp20;
    temp20 = (op)->NewInstance();
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxFemSolid");
    return TCL_OK;
    }
  if ((!strcmp("SafeDownCast",argv[1]))&&(argc == 3))
    {
    vtkObject  *temp0;
    vtkpxFemSolid  *temp20;
    error = 0;

    temp0 = (vtkObject *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkObject"),interp,error));
    if (!error)
    {
    temp20 = (op)->SafeDownCast(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkpxFemSolid");
    return TCL_OK;
    }
    }
  if ((!strcmp("Copy",argv[1]))&&(argc == 3))
    {
    vtkpxFemSolid  *temp0;
    error = 0;

    temp0 = (vtkpxFemSolid *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxFemSolid"),interp,error));
    if (!error)
    {
    op->Copy(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateHexahedralSolid",argv[1]))&&(argc == 7))
    {
    vtkpxTriangulatedStackSource  *temp0;
    vtkpxTriangulatedStackSource  *temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    int      temp20;
    error = 0;

    temp0 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    temp1 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (!error)
    {
    temp20 = (op)->GenerateHexahedralSolid(temp0,temp1,temp2,temp3,temp4);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateHexahedralSolid",argv[1]))&&(argc == 9))
    {
    vtkpxTriangulatedStackSource  *temp0;
    vtkpxTriangulatedStackSource  *temp1;
    int      temp2;
    int      temp3;
    int      temp4;
    float    temp5;
    int      temp6;
    int      temp20;
    error = 0;

    temp0 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    temp1 = (vtkpxTriangulatedStackSource *)(vtkTclGetPointerFromObject(argv[3],const_cast<char *>("vtkpxTriangulatedStackSource"),interp,error));
    if (Tcl_GetInt(interp,argv[4],&tempi) != TCL_OK) error = 1;
    temp2 = tempi;
    if (Tcl_GetInt(interp,argv[5],&tempi) != TCL_OK) error = 1;
    temp3 = tempi;
    if (Tcl_GetInt(interp,argv[6],&tempi) != TCL_OK) error = 1;
    temp4 = tempi;
    if (Tcl_GetDouble(interp,argv[7],&tempd) != TCL_OK) error = 1;
    temp5 = tempd;
    if (Tcl_GetInt(interp,argv[8],&tempi) != TCL_OK) error = 1;
    temp6 = tempi;
    if (!error)
    {
    temp20 = (op)->GenerateHexahedralSolid(temp0,temp1,temp2,temp3,temp4,temp5,temp6);
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
  if ((!strcmp("SetColorMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetColorMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetColorModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetColorModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetColorModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetColorModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetColorMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetColorMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetStrainMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetStrainMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetStrainModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetStrainModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetStrainModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetStrainModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetStrainMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetStrainMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("SetWarpMode",argv[1]))&&(argc == 3))
    {
    int      temp0;
    error = 0;

    if (Tcl_GetInt(interp,argv[2],&tempi) != TCL_OK) error = 1;
    temp0 = tempi;
    if (!error)
    {
    op->SetWarpMode(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetWarpModeMinValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetWarpModeMinValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetWarpModeMaxValue",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetWarpModeMaxValue();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetWarpMode",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetWarpMode();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("CreateFemData",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->CreateFemData();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("LoadFemData",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->LoadFemData(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("SaveFemData",argv[1]))&&(argc == 3))
    {
    char    *temp0;
    int      temp20;
    error = 0;

    temp0 = argv[2];
    if (!error)
    {
    temp20 = (op)->SaveFemData(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
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
  if ((!strcmp("GetStrain",argv[1]))&&(argc == 4))
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
    temp20 = (op)->GetStrain(temp0,temp1);
    char tempResult[1024];
    Tcl_PrintDouble(interp,temp20,tempResult);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("Scale",argv[1]))&&(argc == 3))
    {
    float    temp0;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    op->Scale(temp0);
    Tcl_ResetResult(interp);
    return TCL_OK;
    }
    }
  if ((!strcmp("GetNumberOfNodes",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfNodes();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("GetNumberOfElements",argv[1]))&&(argc == 2))
    {
    int      temp20;
    temp20 = (op)->GetNumberOfElements();
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
  if ((!strcmp("ReorientStrainsToFiberOrientation",argv[1]))&&(argc == 3))
    {
    vtkImageData  *temp0;
    int      temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (!error)
    {
    temp20 = (op)->ReorientStrainsToFiberOrientation(temp0);
    char tempResult[1024];
    sprintf(tempResult,"%i",temp20);
    Tcl_SetResult(interp, tempResult, TCL_VOLATILE);
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateFibreMaps",argv[1]))&&(argc == 4))
    {
    vtkImageData  *temp0;
    double   temp1;
    vtkPolyData  *temp20;
    error = 0;

    temp0 = (vtkImageData *)(vtkTclGetPointerFromObject(argv[2],const_cast<char *>("vtkImageData"),interp,error));
    if (Tcl_GetDouble(interp,argv[3],&tempd) != TCL_OK) error = 1;
    temp1 = tempd;
    if (!error)
    {
    temp20 = (op)->GenerateFibreMaps(temp0,temp1);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
    }
  if ((!strcmp("GenerateInternalFibreMaps",argv[1]))&&(argc == 3))
    {
    double   temp0;
    vtkPolyData  *temp20;
    error = 0;

    if (Tcl_GetDouble(interp,argv[2],&tempd) != TCL_OK) error = 1;
    temp0 = tempd;
    if (!error)
    {
    temp20 = (op)->GenerateInternalFibreMaps(temp0);
      vtkTclGetObjectFromPointer(interp,(void *)(temp20),"vtkPolyData");
    return TCL_OK;
    }
    }

  if (!strcmp("ListInstances",argv[1]))
    {
    vtkTclListInstances(interp,(ClientData)(vtkpxFemSolidCommand));
    return TCL_OK;
    }

  if (!strcmp("ListMethods",argv[1]))
    {
    vtkPolyDataSourceCppCommand(op,interp,argc,argv);
    Tcl_AppendResult(interp,"Methods from vtkpxFemSolid:\n",NULL);
    Tcl_AppendResult(interp,"  GetSuperClassName\n",NULL);
    Tcl_AppendResult(interp,"  New\n",NULL);
    Tcl_AppendResult(interp,"  GetClassName\n",NULL);
    Tcl_AppendResult(interp,"  IsA\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  NewInstance\n",NULL);
    Tcl_AppendResult(interp,"  SafeDownCast\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Copy\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GenerateHexahedralSolid\t with 5 args\n",NULL);
    Tcl_AppendResult(interp,"  GenerateHexahedralSolid\t with 7 args\n",NULL);
    Tcl_AppendResult(interp,"  Load\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  Save\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SetColorMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetColorModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetColorModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetColorMode\n",NULL);
    Tcl_AppendResult(interp,"  SetStrainMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetStrainModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetStrainModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetStrainMode\n",NULL);
    Tcl_AppendResult(interp,"  SetWarpMode\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetWarpModeMinValue\n",NULL);
    Tcl_AppendResult(interp,"  GetWarpModeMaxValue\n",NULL);
    Tcl_AppendResult(interp,"  GetWarpMode\n",NULL);
    Tcl_AppendResult(interp,"  CreateFemData\n",NULL);
    Tcl_AppendResult(interp,"  LoadFemData\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  SaveFemData\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetDisplacement\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GetStrain\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  Scale\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfNodes\n",NULL);
    Tcl_AppendResult(interp,"  GetNumberOfElements\n",NULL);
    Tcl_AppendResult(interp,"  ReorientStrainsToFiberOrientation\t with 1 arg\n",NULL);
    Tcl_AppendResult(interp,"  GenerateFibreMaps\t with 2 args\n",NULL);
    Tcl_AppendResult(interp,"  GenerateInternalFibreMaps\t with 1 arg\n",NULL);
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
    Tcl_DStringAppendElement ( &dString, "Copy" );
    Tcl_DStringAppendElement ( &dString, "GenerateHexahedralSolid" );
    Tcl_DStringAppendElement ( &dString, "GenerateHexahedralSolid" );
    Tcl_DStringAppendElement ( &dString, "Load" );
    Tcl_DStringAppendElement ( &dString, "Save" );
    Tcl_DStringAppendElement ( &dString, "SetColorMode" );
    Tcl_DStringAppendElement ( &dString, "GetColorModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetColorModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetColorMode" );
    Tcl_DStringAppendElement ( &dString, "SetStrainMode" );
    Tcl_DStringAppendElement ( &dString, "GetStrainModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetStrainModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetStrainMode" );
    Tcl_DStringAppendElement ( &dString, "SetWarpMode" );
    Tcl_DStringAppendElement ( &dString, "GetWarpModeMinValue" );
    Tcl_DStringAppendElement ( &dString, "GetWarpModeMaxValue" );
    Tcl_DStringAppendElement ( &dString, "GetWarpMode" );
    Tcl_DStringAppendElement ( &dString, "CreateFemData" );
    Tcl_DStringAppendElement ( &dString, "LoadFemData" );
    Tcl_DStringAppendElement ( &dString, "SaveFemData" );
    Tcl_DStringAppendElement ( &dString, "GetDisplacement" );
    Tcl_DStringAppendElement ( &dString, "GetStrain" );
    Tcl_DStringAppendElement ( &dString, "Scale" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfNodes" );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfElements" );
    Tcl_DStringAppendElement ( &dString, "ReorientStrainsToFiberOrientation" );
    Tcl_DStringAppendElement ( &dString, "GenerateFibreMaps" );
    Tcl_DStringAppendElement ( &dString, "GenerateInternalFibreMaps" );
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
    Tcl_DStringAppendElement ( &dString, "static vtkpxFemSolid *New();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid *NewInstance();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
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
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid *SafeDownCast(vtkObject* o);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for SafeDownCast */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Copy */
    Tcl_DStringAppendElement ( &dString, " Initialization\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Copy(vtkpxFemSolid *other);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for Copy */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateHexahedralSolid */
    if ( strcmp ( argv[2], "GenerateHexahedralSolid" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateHexahedralSolid" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateHexahedralSolid */
    Tcl_DStringAppendElement ( &dString, " Create Mesh \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int GenerateHexahedralSolid(vtkpxTriangulatedStackSource *endo, vtkpxTriangulatedStackSource *epi, int nodes, int totalnumstacks, int bias);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GenerateHexahedralSolid */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateHexahedralSolid */
    if ( strcmp ( argv[2], "GenerateHexahedralSolid" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateHexahedralSolid" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringAppendElement ( &dString, "vtkpxTriangulatedStackSource" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateHexahedralSolid */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int GenerateHexahedralSolid(vtkpxTriangulatedStackSource *endo, vtkpxTriangulatedStackSource *epi, int nodes, int totalnumstacks, int bias, float sample, int skipslices);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GenerateHexahedralSolid */

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
    Tcl_DStringAppendElement ( &dString, " I/O Code -- save as .sld file \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Load(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
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
    Tcl_DStringAppendElement ( &dString, " I/O Code -- save as .sld file \n" );
    Tcl_DStringAppendElement ( &dString, "virtual int Save(const char *fname);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for Save */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetColorMode */
    if ( strcmp ( argv[2], "SetColorMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetColorMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetColorMode */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "void SetColorMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for SetColorMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetColorModeMinValue */
    if ( strcmp ( argv[2], "GetColorModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetColorModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetColorModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "int GetColorModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetColorModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetColorModeMaxValue */
    if ( strcmp ( argv[2], "GetColorModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetColorModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetColorModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "int GetColorModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetColorModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetColorMode */
    if ( strcmp ( argv[2], "GetColorMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetColorMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetColorMode */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "int GetColorMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetColorMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetStrainMode */
    if ( strcmp ( argv[2], "SetStrainMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetStrainMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetStrainMode */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "void SetStrainMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for SetStrainMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetStrainModeMinValue */
    if ( strcmp ( argv[2], "GetStrainModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetStrainModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetStrainModeMinValue */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "int GetStrainModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetStrainModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetStrainModeMaxValue */
    if ( strcmp ( argv[2], "GetStrainModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetStrainModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetStrainModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "int GetStrainModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetStrainModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetStrainMode */
    if ( strcmp ( argv[2], "GetStrainMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetStrainMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetStrainMode */
    Tcl_DStringAppendElement ( &dString, " Modes for Strain Value / Color Mode \n" );
    Tcl_DStringAppendElement ( &dString, "int GetStrainMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetStrainMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SetWarpMode */
    if ( strcmp ( argv[2], "SetWarpMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SetWarpMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SetWarpMode */
    Tcl_DStringAppendElement ( &dString, " If Warp=on transform mesh using internal displacements \n" );
    Tcl_DStringAppendElement ( &dString, "void SetWarpMode(int);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for SetWarpMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWarpModeMinValue */
    if ( strcmp ( argv[2], "GetWarpModeMinValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWarpModeMinValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWarpModeMinValue */
    Tcl_DStringAppendElement ( &dString, " If Warp=on transform mesh using internal displacements \n" );
    Tcl_DStringAppendElement ( &dString, "int GetWarpModeMinValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetWarpModeMinValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWarpModeMaxValue */
    if ( strcmp ( argv[2], "GetWarpModeMaxValue" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWarpModeMaxValue" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWarpModeMaxValue */
    Tcl_DStringAppendElement ( &dString, " If Warp=on transform mesh using internal displacements \n" );
    Tcl_DStringAppendElement ( &dString, "int GetWarpModeMaxValue();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetWarpModeMaxValue */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetWarpMode */
    if ( strcmp ( argv[2], "GetWarpMode" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetWarpMode" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetWarpMode */
    Tcl_DStringAppendElement ( &dString, " If Warp=on transform mesh using internal displacements \n" );
    Tcl_DStringAppendElement ( &dString, "int GetWarpMode();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetWarpMode */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: CreateFemData */
    if ( strcmp ( argv[2], "CreateFemData" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "CreateFemData" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for CreateFemData */
    Tcl_DStringAppendElement ( &dString, " Access Fem Data\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int CreateFemData();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for CreateFemData */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: LoadFemData */
    if ( strcmp ( argv[2], "LoadFemData" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "LoadFemData" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for LoadFemData */
    Tcl_DStringAppendElement ( &dString, " Access Fem Data\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int LoadFemData(const char *line);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for LoadFemData */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: SaveFemData */
    if ( strcmp ( argv[2], "SaveFemData" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "SaveFemData" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "string" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for SaveFemData */
    Tcl_DStringAppendElement ( &dString, " Access Fem Data\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int SaveFemData(const char *line);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for SaveFemData */

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
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetDisplacement */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetStrain */
    if ( strcmp ( argv[2], "GetStrain" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetStrain" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringAppendElement ( &dString, "int" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetStrain */
    Tcl_DStringAppendElement ( &dString, " Access Fem Data\n" );
    Tcl_DStringAppendElement ( &dString, "virtual float GetStrain(int node, int comp);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetStrain */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: Scale */
    if ( strcmp ( argv[2], "Scale" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "Scale" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for Scale */
    Tcl_DStringAppendElement ( &dString, " Scale Points\n" );
    Tcl_DStringAppendElement ( &dString, "virtual void Scale(float s);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for Scale */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfNodes */
    if ( strcmp ( argv[2], "GetNumberOfNodes" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfNodes" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfNodes */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumberOfNodes();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetNumberOfNodes */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GetNumberOfElements */
    if ( strcmp ( argv[2], "GetNumberOfElements" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GetNumberOfElements" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GetNumberOfElements */
    Tcl_DStringAppendElement ( &dString, "" );
    Tcl_DStringAppendElement ( &dString, "virtual int GetNumberOfElements();" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GetNumberOfElements */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: ReorientStrainsToFiberOrientation */
    if ( strcmp ( argv[2], "ReorientStrainsToFiberOrientation" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "ReorientStrainsToFiberOrientation" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for ReorientStrainsToFiberOrientation */
    Tcl_DStringAppendElement ( &dString, " Final Issues\n" );
    Tcl_DStringAppendElement ( &dString, "virtual int ReorientStrainsToFiberOrientation(vtkImageData *img);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for ReorientStrainsToFiberOrientation */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateFibreMaps */
    if ( strcmp ( argv[2], "GenerateFibreMaps" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateFibreMaps" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "vtkImageData" );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateFibreMaps */
    Tcl_DStringAppendElement ( &dString, " Final Issues\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkPolyData *GenerateFibreMaps(vtkImageData *img, double scale=2.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GenerateFibreMaps */

    Tcl_DStringResult ( interp, &dString );
    Tcl_DStringFree ( &dString );
    return TCL_OK;
    }
    /* Starting function: GenerateInternalFibreMaps */
    if ( strcmp ( argv[2], "GenerateInternalFibreMaps" ) == 0 ) {
    Tcl_DStringInit ( &dString );
    Tcl_DStringAppendElement ( &dString, "GenerateInternalFibreMaps" );
    /* Arguments */
    Tcl_DStringStartSublist ( &dString );
    Tcl_DStringAppendElement ( &dString, "float" );
    Tcl_DStringEndSublist ( &dString );
    /* Documentation for GenerateInternalFibreMaps */
    Tcl_DStringAppendElement ( &dString, " Final Issues\n" );
    Tcl_DStringAppendElement ( &dString, "virtual vtkPolyData *GenerateInternalFibreMaps(double scale=2.0);" );
    Tcl_DStringAppendElement ( &dString, "vtkpxFemSolid" );
    /* Closing for GenerateInternalFibreMaps */

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
