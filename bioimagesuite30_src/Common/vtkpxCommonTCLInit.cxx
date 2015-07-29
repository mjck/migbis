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

int vtkbisNIFTIExtensionCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisNIFTIExtensionNewCommand();
int vtkbisNIFTIExtensionsListCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisNIFTIExtensionsListNewCommand();
int vtkbisImageHeaderCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisImageHeaderNewCommand();
int vtkpxAnalyzeImageSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAnalyzeImageSourceNewCommand();
int vtkpxAnalyzeImageSourceOldCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAnalyzeImageSourceOldNewCommand();
int vtkpxAnalyzeImageWriterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAnalyzeImageWriterNewCommand();
int vtkpxPickerPrismSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxPickerPrismSourceNewCommand();
int vtkpxSignaLXImageSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxSignaLXImageSourceNewCommand();
int vtkpxEpilepsyCSIReaderCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxEpilepsyCSIReaderNewCommand();
int vtkafBVImageReaderCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkafBVImageReaderNewCommand();
int vtkafBVImageWriterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkafBVImageWriterNewCommand();
int vtkpxUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxUtilNewCommand();
int vtkbisTestUtilCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisTestUtilNewCommand();
int vtkpxDataArrayUndoStackCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxDataArrayUndoStackNewCommand();
int vtkpxMathCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxMathNewCommand();
int vtkpxOptimizerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxOptimizerNewCommand();
int vtkpxTestOptimizerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxTestOptimizerNewCommand();
int vtkpxMatrixCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxMatrixNewCommand();
int vtkpxBSplineCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxBSplineNewCommand();
int vtkpxImageComponentsToFramesCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageComponentsToFramesNewCommand();
int vtkpxStructuredPointsToStructuredPointsFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxStructuredPointsToStructuredPointsFilterNewCommand();
int vtkpxImageExtractCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageExtractNewCommand();
int vtkpxAppendComponentsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxAppendComponentsNewCommand();
int vtkpxImageExtractVOICommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageExtractVOINewCommand();
int vtkpxImageReplaceSliceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxImageReplaceSliceNewCommand();
int vtkpxCrossHairSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxCrossHairSourceNewCommand();
int vtkbisCylinderSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisCylinderSourceNewCommand();
int vtkpxBaseCurveCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxBaseCurveNewCommand();
int vtkpxTensorBSplineTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxTensorBSplineTransformNewCommand();
int vtkokImageAccumulateCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkokImageAccumulateNewCommand();
int vtkbisImageResliceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisImageResliceNewCommand();
int vtkbisImageResampleCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisImageResampleNewCommand();
int vtkbisImageReOrientCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisImageReOrientNewCommand();
int vtkjoSphereSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkjoSphereSourceNewCommand();
int vtkjoColoredSphereSourceCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkjoColoredSphereSourceNewCommand();
int vtkjoStatisticsCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkjoStatisticsNewCommand();
int vtkjoROCFilterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkjoROCFilterNewCommand();
int vtkpxOpenIGTLinkClientCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxOpenIGTLinkClientNewCommand();
int vtkpxOpenIGTLinkServerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxOpenIGTLinkServerNewCommand();
int vtkbisGDCMImageReaderCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisGDCMImageReaderNewCommand();
int vtkbisDICOMReaderCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisDICOMReaderNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkpxcommontcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkpxcommontcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkpxcommontcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkpxcommontcl_Init(interp);
}


int VTK_EXPORT Vtkpxcommontcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkbisNIFTIExtension"), vtkbisNIFTIExtensionNewCommand,
                  vtkbisNIFTIExtensionCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisNIFTIExtensionsList"), vtkbisNIFTIExtensionsListNewCommand,
                  vtkbisNIFTIExtensionsListCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisImageHeader"), vtkbisImageHeaderNewCommand,
                  vtkbisImageHeaderCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAnalyzeImageSource"), vtkpxAnalyzeImageSourceNewCommand,
                  vtkpxAnalyzeImageSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAnalyzeImageSourceOld"), vtkpxAnalyzeImageSourceOldNewCommand,
                  vtkpxAnalyzeImageSourceOldCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAnalyzeImageWriter"), vtkpxAnalyzeImageWriterNewCommand,
                  vtkpxAnalyzeImageWriterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxPickerPrismSource"), vtkpxPickerPrismSourceNewCommand,
                  vtkpxPickerPrismSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxSignaLXImageSource"), vtkpxSignaLXImageSourceNewCommand,
                  vtkpxSignaLXImageSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxEpilepsyCSIReader"), vtkpxEpilepsyCSIReaderNewCommand,
                  vtkpxEpilepsyCSIReaderCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkafBVImageReader"), vtkafBVImageReaderNewCommand,
                  vtkafBVImageReaderCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkafBVImageWriter"), vtkafBVImageWriterNewCommand,
                  vtkafBVImageWriterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxUtil"), vtkpxUtilNewCommand,
                  vtkpxUtilCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisTestUtil"), vtkbisTestUtilNewCommand,
                  vtkbisTestUtilCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxDataArrayUndoStack"), vtkpxDataArrayUndoStackNewCommand,
                  vtkpxDataArrayUndoStackCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxMath"), vtkpxMathNewCommand,
                  vtkpxMathCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxOptimizer"), vtkpxOptimizerNewCommand,
                  vtkpxOptimizerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxTestOptimizer"), vtkpxTestOptimizerNewCommand,
                  vtkpxTestOptimizerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxMatrix"), vtkpxMatrixNewCommand,
                  vtkpxMatrixCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxBSpline"), vtkpxBSplineNewCommand,
                  vtkpxBSplineCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageComponentsToFrames"), vtkpxImageComponentsToFramesNewCommand,
                  vtkpxImageComponentsToFramesCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxStructuredPointsToStructuredPointsFilter"), vtkpxStructuredPointsToStructuredPointsFilterNewCommand,
                  vtkpxStructuredPointsToStructuredPointsFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageExtract"), vtkpxImageExtractNewCommand,
                  vtkpxImageExtractCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxAppendComponents"), vtkpxAppendComponentsNewCommand,
                  vtkpxAppendComponentsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageExtractVOI"), vtkpxImageExtractVOINewCommand,
                  vtkpxImageExtractVOICommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxImageReplaceSlice"), vtkpxImageReplaceSliceNewCommand,
                  vtkpxImageReplaceSliceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxCrossHairSource"), vtkpxCrossHairSourceNewCommand,
                  vtkpxCrossHairSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisCylinderSource"), vtkbisCylinderSourceNewCommand,
                  vtkbisCylinderSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxBaseCurve"), vtkpxBaseCurveNewCommand,
                  vtkpxBaseCurveCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxTensorBSplineTransform"), vtkpxTensorBSplineTransformNewCommand,
                  vtkpxTensorBSplineTransformCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkokImageAccumulate"), vtkokImageAccumulateNewCommand,
                  vtkokImageAccumulateCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisImageReslice"), vtkbisImageResliceNewCommand,
                  vtkbisImageResliceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisImageResample"), vtkbisImageResampleNewCommand,
                  vtkbisImageResampleCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisImageReOrient"), vtkbisImageReOrientNewCommand,
                  vtkbisImageReOrientCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkjoSphereSource"), vtkjoSphereSourceNewCommand,
                  vtkjoSphereSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkjoColoredSphereSource"), vtkjoColoredSphereSourceNewCommand,
                  vtkjoColoredSphereSourceCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkjoStatistics"), vtkjoStatisticsNewCommand,
                  vtkjoStatisticsCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkjoROCFilter"), vtkjoROCFilterNewCommand,
                  vtkjoROCFilterCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxOpenIGTLinkClient"), vtkpxOpenIGTLinkClientNewCommand,
                  vtkpxOpenIGTLinkClientCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxOpenIGTLinkServer"), vtkpxOpenIGTLinkServerNewCommand,
                  vtkpxOpenIGTLinkServerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisGDCMImageReader"), vtkbisGDCMImageReaderNewCommand,
                  vtkbisGDCMImageReaderCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisDICOMReader"), vtkbisDICOMReaderNewCommand,
                  vtkbisDICOMReaderCommand);
  char pkgName[]="vtkpxCommonTCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
