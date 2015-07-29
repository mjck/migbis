#include "vtkTclUtil.h"
#include "vtkVersion.h"
#define VTK_TCL_TO_STRING(x) VTK_TCL_TO_STRING0(x)
#define VTK_TCL_TO_STRING0(x) #x
extern "C"
{
#if (TCL_MAJOR_VERSION == 8) && (TCL_MINOR_VERSION >= 4)
  typedef int (*vtkTclCommandType)(ClientData, Tcl_Interp *,int, CONST84 char *[]);
#else
  typedef int (*vtkTclCommandType)(ClientData, Tcl_Interp *,int, char *[]);
#endif
}

int vtkpxSurfaceUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxSurfaceUtilNewCommand();
int vtkpxAppendAndLabelPolyDataCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAppendAndLabelPolyDataNewCommand();
int vtkpxAddDisplacementVectorToPolyDataCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAddDisplacementVectorToPolyDataNewCommand();
int vtkpxColorMapUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxColorMapUtilNewCommand();
int vtkpxCurveEditUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxCurveEditUtilNewCommand();
int vtkpxEnclosedPipelineCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxEnclosedPipelineNewCommand();
int vtkpxImageSliceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageSliceNewCommand();
int vtkpx2DImageSliceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpx2DImageSliceNewCommand();
int vtkpxMapActivationToPolyDataCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxMapActivationToPolyDataNewCommand();
int vtkpxNormalShrinkPolyDataCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxNormalShrinkPolyDataNewCommand();
int vtkpxOrthoImageSliceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxOrthoImageSliceNewCommand();
int vtkpxVolumeCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxVolumeNewCommand();
int vtkpxObliqueImageSliceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxObliqueImageSliceNewCommand();
int vtkpxCameraInfoCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxCameraInfoNewCommand();
int vtkEllipseSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkEllipseSourceNewCommand();
int vtkpxElectrodeGridToSurfaceFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxElectrodeGridToSurfaceFilterNewCommand();
int vtkpxElectrodeSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxElectrodeSourceNewCommand();
int vtkpxElectrodeGridSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxElectrodeGridSourceNewCommand();
int vtkpxElectrodeMultiGridSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxElectrodeMultiGridSourceNewCommand();
int vtkpxPredictElectrodeStripCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxPredictElectrodeStripNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkpxgraphicstcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkpxgraphicstcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkpxgraphicstcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkpxgraphicstcl_Init(interp);
}


int VTK_EXPORT Vtkpxgraphicstcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkpxSurfaceUtil"), vtkpxSurfaceUtilNewCommand,
                  vtkpxSurfaceUtilCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAppendAndLabelPolyData"), vtkpxAppendAndLabelPolyDataNewCommand,
                  vtkpxAppendAndLabelPolyDataCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAddDisplacementVectorToPolyData"), vtkpxAddDisplacementVectorToPolyDataNewCommand,
                  vtkpxAddDisplacementVectorToPolyDataCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxColorMapUtil"), vtkpxColorMapUtilNewCommand,
                  vtkpxColorMapUtilCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxCurveEditUtil"), vtkpxCurveEditUtilNewCommand,
                  vtkpxCurveEditUtilCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxEnclosedPipeline"), vtkpxEnclosedPipelineNewCommand,
                  vtkpxEnclosedPipelineCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageSlice"), vtkpxImageSliceNewCommand,
                  vtkpxImageSliceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpx2DImageSlice"), vtkpx2DImageSliceNewCommand,
                  vtkpx2DImageSliceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxMapActivationToPolyData"), vtkpxMapActivationToPolyDataNewCommand,
                  vtkpxMapActivationToPolyDataCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxNormalShrinkPolyData"), vtkpxNormalShrinkPolyDataNewCommand,
                  vtkpxNormalShrinkPolyDataCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxOrthoImageSlice"), vtkpxOrthoImageSliceNewCommand,
                  vtkpxOrthoImageSliceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxVolume"), vtkpxVolumeNewCommand,
                  vtkpxVolumeCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxObliqueImageSlice"), vtkpxObliqueImageSliceNewCommand,
                  vtkpxObliqueImageSliceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxCameraInfo"), vtkpxCameraInfoNewCommand,
                  vtkpxCameraInfoCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkEllipseSource"), vtkEllipseSourceNewCommand,
                  vtkEllipseSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxElectrodeGridToSurfaceFilter"), vtkpxElectrodeGridToSurfaceFilterNewCommand,
                  vtkpxElectrodeGridToSurfaceFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxElectrodeSource"), vtkpxElectrodeSourceNewCommand,
                  vtkpxElectrodeSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxElectrodeGridSource"), vtkpxElectrodeGridSourceNewCommand,
                  vtkpxElectrodeGridSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxElectrodeMultiGridSource"), vtkpxElectrodeMultiGridSourceNewCommand,
                  vtkpxElectrodeMultiGridSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxPredictElectrodeStrip"), vtkpxPredictElectrodeStripNewCommand,
                  vtkpxPredictElectrodeStripCommand);
  char pkgName[]="vtkpxGraphicsTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
