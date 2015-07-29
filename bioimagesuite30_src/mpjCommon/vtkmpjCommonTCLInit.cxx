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

int vtkmpjFrameExtractCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjFrameExtractNewCommand();
int vtkmpjFrameAppendCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjFrameAppendNewCommand();
int vtkmpjFrameAverageCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjFrameAverageNewCommand();
int vtkmpjFramePermutationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjFramePermutationNewCommand();
int vtkmpjImageInterleavingCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageInterleavingNewCommand();
int vtkmpjImageTwoNormCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageTwoNormNewCommand();
int vtkmpjImageStatisticsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageStatisticsNewCommand();
int vtkmpjPriorityQueueCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjPriorityQueueNewCommand();
int vtkmpjTestOptimizerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTestOptimizerNewCommand();
int vtkmpjImageBoundaryCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageBoundaryNewCommand();
int vtkmpjTensorStreamlineRegionFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorStreamlineRegionFilterNewCommand();
int vtkmpjTensorStreamlineROIFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorStreamlineROIFilterNewCommand();
int vtkmpjTensorStreamlineStatisticsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorStreamlineStatisticsNewCommand();
int vtkmpjStreamlineQuantizationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjStreamlineQuantizationNewCommand();
int vtkmpjImagePointSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImagePointSourceNewCommand();
int vtkmpjImageROISourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageROISourceNewCommand();
int vtkmpjScalarsToFieldDataCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjScalarsToFieldDataNewCommand();
int vtkmpjVectorStreamlineCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjVectorStreamlineNewCommand();
int vtkmpjImageSignalToNoiseRatioCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageSignalToNoiseRatioNewCommand();
int vtkmpjStreamlineDistanceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjStreamlineDistanceNewCommand();
int vtkmpjThresholdPolyDataCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjThresholdPolyDataNewCommand();
int vtkmpjThresholdPolyDataByDistanceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjThresholdPolyDataByDistanceNewCommand();
int vtkmpjVectorStreamlineStatisticsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjVectorStreamlineStatisticsNewCommand();
int vtkmpjImageHessianCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageHessianNewCommand();
int vtkmpjImageVesselEnhancementCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageVesselEnhancementNewCommand();
int vtkmpjImageVesselMaximumCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageVesselMaximumNewCommand();
int vtkmpjDiffusionProfileCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjDiffusionProfileNewCommand();
int vtkmpjVertexSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjVertexSourceNewCommand();
int vtkmpjSphereSubdivisionSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjSphereSubdivisionSourceNewCommand();
int vtkmpjIcosahedronSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjIcosahedronSourceNewCommand();
int vtkmpjImageT2RelaxCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageT2RelaxNewCommand();
int vtkmpjImageNormalizeCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageNormalizeNewCommand();
int vtkmpjTensorStreamlineWriterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorStreamlineWriterNewCommand();
int vtkmpjTensorStreamlineHistogramWriterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorStreamlineHistogramWriterNewCommand();
int vtkmpjStreamlineMapQuantizationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjStreamlineMapQuantizationNewCommand();
int vtkmpjTensorStreamlineAngleFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorStreamlineAngleFilterNewCommand();
int vtkmpjImageExtractComponentsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageExtractComponentsNewCommand();
int vtkmpjImageThresholdConnectivityCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjImageThresholdConnectivityNewCommand();
int vtkmpjInterpolatedTensorFieldCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjInterpolatedTensorFieldNewCommand();
int vtkmpjTensorStreamLineCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorStreamLineNewCommand();
int vtkmpjTensorStreamOldCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjTensorStreamOldNewCommand();
int vtkmpjPolyDataToImageFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjPolyDataToImageFilterNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkmpjcommontcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkmpjcommontcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkmpjcommontcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkmpjcommontcl_Init(interp);
}


int VTK_EXPORT Vtkmpjcommontcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjFrameExtract"), vtkmpjFrameExtractNewCommand,
                  vtkmpjFrameExtractCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjFrameAppend"), vtkmpjFrameAppendNewCommand,
                  vtkmpjFrameAppendCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjFrameAverage"), vtkmpjFrameAverageNewCommand,
                  vtkmpjFrameAverageCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjFramePermutation"), vtkmpjFramePermutationNewCommand,
                  vtkmpjFramePermutationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageInterleaving"), vtkmpjImageInterleavingNewCommand,
                  vtkmpjImageInterleavingCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageTwoNorm"), vtkmpjImageTwoNormNewCommand,
                  vtkmpjImageTwoNormCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageStatistics"), vtkmpjImageStatisticsNewCommand,
                  vtkmpjImageStatisticsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjPriorityQueue"), vtkmpjPriorityQueueNewCommand,
                  vtkmpjPriorityQueueCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTestOptimizer"), vtkmpjTestOptimizerNewCommand,
                  vtkmpjTestOptimizerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageBoundary"), vtkmpjImageBoundaryNewCommand,
                  vtkmpjImageBoundaryCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorStreamlineRegionFilter"), vtkmpjTensorStreamlineRegionFilterNewCommand,
                  vtkmpjTensorStreamlineRegionFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorStreamlineROIFilter"), vtkmpjTensorStreamlineROIFilterNewCommand,
                  vtkmpjTensorStreamlineROIFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorStreamlineStatistics"), vtkmpjTensorStreamlineStatisticsNewCommand,
                  vtkmpjTensorStreamlineStatisticsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjStreamlineQuantization"), vtkmpjStreamlineQuantizationNewCommand,
                  vtkmpjStreamlineQuantizationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImagePointSource"), vtkmpjImagePointSourceNewCommand,
                  vtkmpjImagePointSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageROISource"), vtkmpjImageROISourceNewCommand,
                  vtkmpjImageROISourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjScalarsToFieldData"), vtkmpjScalarsToFieldDataNewCommand,
                  vtkmpjScalarsToFieldDataCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjVectorStreamline"), vtkmpjVectorStreamlineNewCommand,
                  vtkmpjVectorStreamlineCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageSignalToNoiseRatio"), vtkmpjImageSignalToNoiseRatioNewCommand,
                  vtkmpjImageSignalToNoiseRatioCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjStreamlineDistance"), vtkmpjStreamlineDistanceNewCommand,
                  vtkmpjStreamlineDistanceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjThresholdPolyData"), vtkmpjThresholdPolyDataNewCommand,
                  vtkmpjThresholdPolyDataCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjThresholdPolyDataByDistance"), vtkmpjThresholdPolyDataByDistanceNewCommand,
                  vtkmpjThresholdPolyDataByDistanceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjVectorStreamlineStatistics"), vtkmpjVectorStreamlineStatisticsNewCommand,
                  vtkmpjVectorStreamlineStatisticsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageHessian"), vtkmpjImageHessianNewCommand,
                  vtkmpjImageHessianCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageVesselEnhancement"), vtkmpjImageVesselEnhancementNewCommand,
                  vtkmpjImageVesselEnhancementCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageVesselMaximum"), vtkmpjImageVesselMaximumNewCommand,
                  vtkmpjImageVesselMaximumCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjDiffusionProfile"), vtkmpjDiffusionProfileNewCommand,
                  vtkmpjDiffusionProfileCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjVertexSource"), vtkmpjVertexSourceNewCommand,
                  vtkmpjVertexSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjSphereSubdivisionSource"), vtkmpjSphereSubdivisionSourceNewCommand,
                  vtkmpjSphereSubdivisionSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjIcosahedronSource"), vtkmpjIcosahedronSourceNewCommand,
                  vtkmpjIcosahedronSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageT2Relax"), vtkmpjImageT2RelaxNewCommand,
                  vtkmpjImageT2RelaxCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageNormalize"), vtkmpjImageNormalizeNewCommand,
                  vtkmpjImageNormalizeCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorStreamlineWriter"), vtkmpjTensorStreamlineWriterNewCommand,
                  vtkmpjTensorStreamlineWriterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorStreamlineHistogramWriter"), vtkmpjTensorStreamlineHistogramWriterNewCommand,
                  vtkmpjTensorStreamlineHistogramWriterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjStreamlineMapQuantization"), vtkmpjStreamlineMapQuantizationNewCommand,
                  vtkmpjStreamlineMapQuantizationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorStreamlineAngleFilter"), vtkmpjTensorStreamlineAngleFilterNewCommand,
                  vtkmpjTensorStreamlineAngleFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageExtractComponents"), vtkmpjImageExtractComponentsNewCommand,
                  vtkmpjImageExtractComponentsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjImageThresholdConnectivity"), vtkmpjImageThresholdConnectivityNewCommand,
                  vtkmpjImageThresholdConnectivityCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjInterpolatedTensorField"), vtkmpjInterpolatedTensorFieldNewCommand,
                  vtkmpjInterpolatedTensorFieldCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorStreamLine"), vtkmpjTensorStreamLineNewCommand,
                  vtkmpjTensorStreamLineCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjTensorStreamOld"), vtkmpjTensorStreamOldNewCommand,
                  vtkmpjTensorStreamOldCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjPolyDataToImageFilter"), vtkmpjPolyDataToImageFilterNewCommand,
                  vtkmpjPolyDataToImageFilterCommand);
  char pkgName[]="vtkmpjCommonTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
