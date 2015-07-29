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

int vtkbisImageAverageComponentsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisImageAverageComponentsNewCommand();
int vtkpxBinaryImageMedian3DCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxBinaryImageMedian3DNewCommand();
int vtkpxCreateCheckerBoardImageCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxCreateCheckerBoardImageNewCommand();
int vtkpxCSIStackImagesCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxCSIStackImagesNewCommand();
int vtkpxImageAlgebraCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageAlgebraNewCommand();
int vtkpxImageCylindricalCropCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageCylindricalCropNewCommand();
int vtkpxImageIDFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageIDFilterNewCommand();
int vtkpxImageAddGaussianNoiseCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageAddGaussianNoiseNewCommand();
int vtkpxImageVariableSmoothingCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageVariableSmoothingNewCommand();
int vtkpxImageFeatureExtractCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageFeatureExtractNewCommand();
int vtkpxImageGaussianNoiseSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageGaussianNoiseSourceNewCommand();
int vtkpxImageNormalizeIntensityCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageNormalizeIntensityNewCommand();
int vtkpxImageComputeNormalizedDifferenceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageComputeNormalizedDifferenceNewCommand();
int vtkpxImageComputeNormalizedRatioCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageComputeNormalizedRatioNewCommand();
int vtkpxImageMaskCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageMaskNewCommand();
int vtkpxImageNormalizeIntensityToReferenceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageNormalizeIntensityToReferenceNewCommand();
int vtkpxImageComputeVOIStatsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageComputeVOIStatsNewCommand();
int vtkpxImagePolarToEuclideanCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImagePolarToEuclideanNewCommand();
int vtkpxImageSincInterpolationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageSincInterpolationNewCommand();
int vtkpxImageStitchCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageStitchNewCommand();
int vtkpxImageHistogramEqualizationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageHistogramEqualizationNewCommand();
int vtkpxSimpleImageThresholdCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxSimpleImageThresholdNewCommand();
int vtkpxWeightedSmoothImageCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxWeightedSmoothImageNewCommand();
int vtkpxImageCorrectNonUniformZCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageCorrectNonUniformZNewCommand();
int vtkpxImageAttenuationCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageAttenuationCorrectionNewCommand();
int vtkpxMaximumImagesCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxMaximumImagesNewCommand();
int vtkpxImageThresholdedIntersectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageThresholdedIntersectionNewCommand();
int vtkpxNormalizedSmoothImageCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxNormalizedSmoothImageNewCommand();
int vtkxqImageHessianCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkxqImageHessianNewCommand();
int vtkxqImageVesselEnhancementCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkxqImageVesselEnhancementNewCommand();
int vtkpxImageReorientTensorOrVectorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageReorientTensorOrVectorNewCommand();
int vtkdsSpectUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkdsSpectUtilNewCommand();
int vtkbisObjectmapRegularizationCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisObjectmapRegularizationNewCommand();
int vtkxqImageKMeansVesselnessFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkxqImageKMeansVesselnessFilterNewCommand();
int vtkxqImageODFVesselnessFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkxqImageODFVesselnessFilterNewCommand();
int vtkxqImageFourierConvolutionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkxqImageFourierConvolutionNewCommand();
int vtkpxAverageImagesCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAverageImagesNewCommand();
int vtkpxAddMotionToTimeSeriesCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAddMotionToTimeSeriesNewCommand();
int vtkpxComputeTmapCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxComputeTmapNewCommand();
int vtkpxCreateSyntheticFmriCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxCreateSyntheticFmriNewCommand();
int vtkpxCreateThickSlicesCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxCreateThickSlicesNewCommand();
int vtkpxImageAddPhaseCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageAddPhaseCorrectionNewCommand();
int vtkpxImageRemoveLinearPhaseCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageRemoveLinearPhaseNewCommand();
int vtkpxMergeFmriConventionalCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxMergeFmriConventionalNewCommand();
int vtkpxProcessFmriCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxProcessFmriNewCommand();
int vtkpxProcessRawBrukerFIDImageCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxProcessRawBrukerFIDImageNewCommand();
int vtknrImageClusterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtknrImageClusterNewCommand();
int vtkpxPhaseCorrectCSIDataCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxPhaseCorrectCSIDataNewCommand();
int vtkpxCSIOffsetCorrectionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxCSIOffsetCorrectionNewCommand();
int vtkbisWaverCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisWaverNewCommand();
int vtkbisTimeSeriesNormalizeCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisTimeSeriesNormalizeNewCommand();
int vtkbisComputeGLMCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisComputeGLMNewCommand();
int vtkbisCudaFFTCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisCudaFFTNewCommand();
int vtkbisCudaRFFTCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisCudaRFFTNewCommand();
int vtkbisCudaConvolutionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisCudaConvolutionNewCommand();
int vtkbisCudaEdgeDetectCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisCudaEdgeDetectNewCommand();
int vtkbisCudaHessianCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisCudaHessianNewCommand();
int vtkbisZscoretoPvalueCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisZscoretoPvalueNewCommand();
int vtkbisRtoTmapCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisRtoTmapNewCommand();
int vtkbisFDRCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisFDRNewCommand();
int vtkbisImageUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisImageUtilNewCommand();
int vtkbisRtoPvalueCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisRtoPvalueNewCommand();
int vtkbisSiemensMosaicConverterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisSiemensMosaicConverterNewCommand();
int vtkbisSiemensDWIArrangeCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisSiemensDWIArrangeNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkpximagingtcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkpximagingtcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkpximagingtcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkpximagingtcl_Init(interp);
}


int VTK_EXPORT Vtkpximagingtcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkbisImageAverageComponents"), vtkbisImageAverageComponentsNewCommand,
                  vtkbisImageAverageComponentsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxBinaryImageMedian3D"), vtkpxBinaryImageMedian3DNewCommand,
                  vtkpxBinaryImageMedian3DCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxCreateCheckerBoardImage"), vtkpxCreateCheckerBoardImageNewCommand,
                  vtkpxCreateCheckerBoardImageCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxCSIStackImages"), vtkpxCSIStackImagesNewCommand,
                  vtkpxCSIStackImagesCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageAlgebra"), vtkpxImageAlgebraNewCommand,
                  vtkpxImageAlgebraCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageCylindricalCrop"), vtkpxImageCylindricalCropNewCommand,
                  vtkpxImageCylindricalCropCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageIDFilter"), vtkpxImageIDFilterNewCommand,
                  vtkpxImageIDFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageAddGaussianNoise"), vtkpxImageAddGaussianNoiseNewCommand,
                  vtkpxImageAddGaussianNoiseCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageVariableSmoothing"), vtkpxImageVariableSmoothingNewCommand,
                  vtkpxImageVariableSmoothingCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageFeatureExtract"), vtkpxImageFeatureExtractNewCommand,
                  vtkpxImageFeatureExtractCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageGaussianNoiseSource"), vtkpxImageGaussianNoiseSourceNewCommand,
                  vtkpxImageGaussianNoiseSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageNormalizeIntensity"), vtkpxImageNormalizeIntensityNewCommand,
                  vtkpxImageNormalizeIntensityCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageComputeNormalizedDifference"), vtkpxImageComputeNormalizedDifferenceNewCommand,
                  vtkpxImageComputeNormalizedDifferenceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageComputeNormalizedRatio"), vtkpxImageComputeNormalizedRatioNewCommand,
                  vtkpxImageComputeNormalizedRatioCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageMask"), vtkpxImageMaskNewCommand,
                  vtkpxImageMaskCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageNormalizeIntensityToReference"), vtkpxImageNormalizeIntensityToReferenceNewCommand,
                  vtkpxImageNormalizeIntensityToReferenceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageComputeVOIStats"), vtkpxImageComputeVOIStatsNewCommand,
                  vtkpxImageComputeVOIStatsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImagePolarToEuclidean"), vtkpxImagePolarToEuclideanNewCommand,
                  vtkpxImagePolarToEuclideanCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageSincInterpolation"), vtkpxImageSincInterpolationNewCommand,
                  vtkpxImageSincInterpolationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageStitch"), vtkpxImageStitchNewCommand,
                  vtkpxImageStitchCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageHistogramEqualization"), vtkpxImageHistogramEqualizationNewCommand,
                  vtkpxImageHistogramEqualizationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxSimpleImageThreshold"), vtkpxSimpleImageThresholdNewCommand,
                  vtkpxSimpleImageThresholdCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxWeightedSmoothImage"), vtkpxWeightedSmoothImageNewCommand,
                  vtkpxWeightedSmoothImageCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageCorrectNonUniformZ"), vtkpxImageCorrectNonUniformZNewCommand,
                  vtkpxImageCorrectNonUniformZCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageAttenuationCorrection"), vtkpxImageAttenuationCorrectionNewCommand,
                  vtkpxImageAttenuationCorrectionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxMaximumImages"), vtkpxMaximumImagesNewCommand,
                  vtkpxMaximumImagesCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageThresholdedIntersection"), vtkpxImageThresholdedIntersectionNewCommand,
                  vtkpxImageThresholdedIntersectionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxNormalizedSmoothImage"), vtkpxNormalizedSmoothImageNewCommand,
                  vtkpxNormalizedSmoothImageCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkxqImageHessian"), vtkxqImageHessianNewCommand,
                  vtkxqImageHessianCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkxqImageVesselEnhancement"), vtkxqImageVesselEnhancementNewCommand,
                  vtkxqImageVesselEnhancementCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageReorientTensorOrVector"), vtkpxImageReorientTensorOrVectorNewCommand,
                  vtkpxImageReorientTensorOrVectorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkdsSpectUtil"), vtkdsSpectUtilNewCommand,
                  vtkdsSpectUtilCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisObjectmapRegularization"), vtkbisObjectmapRegularizationNewCommand,
                  vtkbisObjectmapRegularizationCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkxqImageKMeansVesselnessFilter"), vtkxqImageKMeansVesselnessFilterNewCommand,
                  vtkxqImageKMeansVesselnessFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkxqImageODFVesselnessFilter"), vtkxqImageODFVesselnessFilterNewCommand,
                  vtkxqImageODFVesselnessFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkxqImageFourierConvolution"), vtkxqImageFourierConvolutionNewCommand,
                  vtkxqImageFourierConvolutionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAverageImages"), vtkpxAverageImagesNewCommand,
                  vtkpxAverageImagesCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAddMotionToTimeSeries"), vtkpxAddMotionToTimeSeriesNewCommand,
                  vtkpxAddMotionToTimeSeriesCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxComputeTmap"), vtkpxComputeTmapNewCommand,
                  vtkpxComputeTmapCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxCreateSyntheticFmri"), vtkpxCreateSyntheticFmriNewCommand,
                  vtkpxCreateSyntheticFmriCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxCreateThickSlices"), vtkpxCreateThickSlicesNewCommand,
                  vtkpxCreateThickSlicesCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageAddPhaseCorrection"), vtkpxImageAddPhaseCorrectionNewCommand,
                  vtkpxImageAddPhaseCorrectionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageRemoveLinearPhase"), vtkpxImageRemoveLinearPhaseNewCommand,
                  vtkpxImageRemoveLinearPhaseCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxMergeFmriConventional"), vtkpxMergeFmriConventionalNewCommand,
                  vtkpxMergeFmriConventionalCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxProcessFmri"), vtkpxProcessFmriNewCommand,
                  vtkpxProcessFmriCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxProcessRawBrukerFIDImage"), vtkpxProcessRawBrukerFIDImageNewCommand,
                  vtkpxProcessRawBrukerFIDImageCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtknrImageCluster"), vtknrImageClusterNewCommand,
                  vtknrImageClusterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxPhaseCorrectCSIData"), vtkpxPhaseCorrectCSIDataNewCommand,
                  vtkpxPhaseCorrectCSIDataCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxCSIOffsetCorrection"), vtkpxCSIOffsetCorrectionNewCommand,
                  vtkpxCSIOffsetCorrectionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisWaver"), vtkbisWaverNewCommand,
                  vtkbisWaverCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisTimeSeriesNormalize"), vtkbisTimeSeriesNormalizeNewCommand,
                  vtkbisTimeSeriesNormalizeCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisComputeGLM"), vtkbisComputeGLMNewCommand,
                  vtkbisComputeGLMCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisCudaFFT"), vtkbisCudaFFTNewCommand,
                  vtkbisCudaFFTCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisCudaRFFT"), vtkbisCudaRFFTNewCommand,
                  vtkbisCudaRFFTCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisCudaConvolution"), vtkbisCudaConvolutionNewCommand,
                  vtkbisCudaConvolutionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisCudaEdgeDetect"), vtkbisCudaEdgeDetectNewCommand,
                  vtkbisCudaEdgeDetectCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisCudaHessian"), vtkbisCudaHessianNewCommand,
                  vtkbisCudaHessianCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisZscoretoPvalue"), vtkbisZscoretoPvalueNewCommand,
                  vtkbisZscoretoPvalueCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisRtoTmap"), vtkbisRtoTmapNewCommand,
                  vtkbisRtoTmapCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisFDR"), vtkbisFDRNewCommand,
                  vtkbisFDRCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisImageUtil"), vtkbisImageUtilNewCommand,
                  vtkbisImageUtilCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisRtoPvalue"), vtkbisRtoPvalueNewCommand,
                  vtkbisRtoPvalueCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisSiemensMosaicConverter"), vtkbisSiemensMosaicConverterNewCommand,
                  vtkbisSiemensMosaicConverterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisSiemensDWIArrange"), vtkbisSiemensDWIArrangeNewCommand,
                  vtkbisSiemensDWIArrangeCommand);
  char pkgName[]="vtkpxImagingTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
