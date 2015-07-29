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

int vtkbisImageTimeSeriesNormalizeCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisImageTimeSeriesNormalizeNewCommand();
int vtkbisImageTemporalSmoothingCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisImageTemporalSmoothingNewCommand();
int vtkbisMedianTemporalSmoothingCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisMedianTemporalSmoothingNewCommand();
int vtkbisRemoveTemporalDriftCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisRemoveTemporalDriftNewCommand();
int vtkbisRemoveGlobalTemporalDriftCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisRemoveGlobalTemporalDriftNewCommand();
int vtkbisRemoveSliceMeanCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisRemoveSliceMeanNewCommand();
int vtkbisRemoveVolumeMeanCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisRemoveVolumeMeanNewCommand();
int vtkbisTimeSeriesMeanCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisTimeSeriesMeanNewCommand();
int vtkbisTimeSeriesStatCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisTimeSeriesStatNewCommand();
int vtkbisTimeSeriesCorrelationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisTimeSeriesCorrelationNewCommand();
int vtkbisROIMeanCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisROIMeanNewCommand();
int vtkbisROIMean_rtCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisROIMean_rtNewCommand();
int vtkbisROICorrelationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisROICorrelationNewCommand();
int vtkbisConnectivityUtilityCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisConnectivityUtilityNewCommand();
int vtkbisCsfWmMotionCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisCsfWmMotionCorrectionNewCommand();
int vtkbisTaskBasedSeedCorrelationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisTaskBasedSeedCorrelationNewCommand();
int vtkbisDisplayROIMeanCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisDisplayROIMeanNewCommand();
int vtkbisIntrinsicConnMapCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisIntrinsicConnMapNewCommand();
int vtkbisIpsilateralConnMapCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisIpsilateralConnMapNewCommand();
int vtkbisDijkstraGraphGeodesicPathCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisDijkstraGraphGeodesicPathNewCommand();
int vtkbisfmriToolBoxCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisfmriToolBoxNewCommand();
int vtkbisfmriToolBoxOptimizeCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisfmriToolBoxOptimizeNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkbisconnectivitytcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkbisconnectivitytcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkbisconnectivitytcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkbisconnectivitytcl_Init(interp);
}


int VTK_EXPORT Vtkbisconnectivitytcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkbisImageTimeSeriesNormalize"), vtkbisImageTimeSeriesNormalizeNewCommand,
                  vtkbisImageTimeSeriesNormalizeCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisImageTemporalSmoothing"), vtkbisImageTemporalSmoothingNewCommand,
                  vtkbisImageTemporalSmoothingCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisMedianTemporalSmoothing"), vtkbisMedianTemporalSmoothingNewCommand,
                  vtkbisMedianTemporalSmoothingCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisRemoveTemporalDrift"), vtkbisRemoveTemporalDriftNewCommand,
                  vtkbisRemoveTemporalDriftCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisRemoveGlobalTemporalDrift"), vtkbisRemoveGlobalTemporalDriftNewCommand,
                  vtkbisRemoveGlobalTemporalDriftCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisRemoveSliceMean"), vtkbisRemoveSliceMeanNewCommand,
                  vtkbisRemoveSliceMeanCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisRemoveVolumeMean"), vtkbisRemoveVolumeMeanNewCommand,
                  vtkbisRemoveVolumeMeanCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisTimeSeriesMean"), vtkbisTimeSeriesMeanNewCommand,
                  vtkbisTimeSeriesMeanCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisTimeSeriesStat"), vtkbisTimeSeriesStatNewCommand,
                  vtkbisTimeSeriesStatCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisTimeSeriesCorrelation"), vtkbisTimeSeriesCorrelationNewCommand,
                  vtkbisTimeSeriesCorrelationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisROIMean"), vtkbisROIMeanNewCommand,
                  vtkbisROIMeanCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisROIMean_rt"), vtkbisROIMean_rtNewCommand,
                  vtkbisROIMean_rtCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisROICorrelation"), vtkbisROICorrelationNewCommand,
                  vtkbisROICorrelationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisConnectivityUtility"), vtkbisConnectivityUtilityNewCommand,
                  vtkbisConnectivityUtilityCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisCsfWmMotionCorrection"), vtkbisCsfWmMotionCorrectionNewCommand,
                  vtkbisCsfWmMotionCorrectionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisTaskBasedSeedCorrelation"), vtkbisTaskBasedSeedCorrelationNewCommand,
                  vtkbisTaskBasedSeedCorrelationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisDisplayROIMean"), vtkbisDisplayROIMeanNewCommand,
                  vtkbisDisplayROIMeanCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisIntrinsicConnMap"), vtkbisIntrinsicConnMapNewCommand,
                  vtkbisIntrinsicConnMapCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisIpsilateralConnMap"), vtkbisIpsilateralConnMapNewCommand,
                  vtkbisIpsilateralConnMapCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisDijkstraGraphGeodesicPath"), vtkbisDijkstraGraphGeodesicPathNewCommand,
                  vtkbisDijkstraGraphGeodesicPathCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisfmriToolBox"), vtkbisfmriToolBoxNewCommand,
                  vtkbisfmriToolBoxCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisfmriToolBoxOptimize"), vtkbisfmriToolBoxOptimizeNewCommand,
                  vtkbisfmriToolBoxOptimizeCommand);
  char pkgName[]="vtkbisConnectivityTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
