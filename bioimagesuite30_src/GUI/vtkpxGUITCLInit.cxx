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

int vtkpxGUIComponentCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIComponentNewCommand();
int vtkbisGUINIFTIHeaderEditorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkbisGUINIFTIHeaderEditorNewCommand();
int vtkpxGUIRendererCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIRendererNewCommand();
int vtkpxGUI4DRendererCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUI4DRendererNewCommand();
int vtkpxGUI2DImageViewerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUI2DImageViewerNewCommand();
int vtkpxGUIMosaicViewerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIMosaicViewerNewCommand();
int vtkpxGUIObjectmapMosaicViewerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIObjectmapMosaicViewerNewCommand();
int vtkpxGUIMultipleMosaicViewerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIMultipleMosaicViewerNewCommand();
int vtkpxGUICurveImageEditorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUICurveImageEditorNewCommand();
int vtkpxGUIContourStackEditorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIContourStackEditorNewCommand();
int vtkpxGUIOrthogonalViewerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIOrthogonalViewerNewCommand();
int vtkpxGUIObjectmapOrthogonalViewerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIObjectmapOrthogonalViewerNewCommand();
int vtkpxGUIOrthogonal4DViewerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIOrthogonal4DViewerNewCommand();
int vtkpxGUI2DSplineObjectmapViewerCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUI2DSplineObjectmapViewerNewCommand();
int vtkpxGUISplineEditorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUISplineEditorNewCommand();
int vtkpxGUIObjectmapMosaicEditorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIObjectmapMosaicEditorNewCommand();
int vtkpxGUIObjectmapOrthogonalEditorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIObjectmapOrthogonalEditorNewCommand();
int vtkpxGUI2DImageEditorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUI2DImageEditorNewCommand();
int vtkpxGUIAnalyzeHeaderEditorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIAnalyzeHeaderEditorNewCommand();
int vtkpxGUIColormapEditorCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIColormapEditorNewCommand();
int vtkpxGUIBaseCurveControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIBaseCurveControlNewCommand();
int vtkpxGUIPolyDataControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIPolyDataControlNewCommand();
int vtkpxGUIObjectmapSplineControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIObjectmapSplineControlNewCommand();
int vtkpxGUISplineControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUISplineControlNewCommand();
int vtkpxGUISplineStackControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUISplineStackControlNewCommand();
int vtkpxGUIObjectmapSplineStackControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIObjectmapSplineStackControlNewCommand();
int vtkpxGUI4DSplineStackControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUI4DSplineStackControlNewCommand();
int vtkpxGUITriangulatedStackControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUITriangulatedStackControlNewCommand();
int vtkpxGUI4DTriangulatedStackControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUI4DTriangulatedStackControlNewCommand();
int vtkpxGUIFemPostControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIFemPostControlNewCommand();
int vtkpxGUIVolumeControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIVolumeControlNewCommand();
int vtkpxGUIObliqueImageSliceControlCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxGUIObliqueImageSliceControlNewCommand();
int vtkpxTalairachTransformCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkpxTalairachTransformNewCommand();
int vtkmpjX3DExporterCommand(ClientData cd, Tcl_Interp *interp,
             int argc, char *argv[]);
ClientData vtkmpjX3DExporterNewCommand();

extern Tcl_HashTable vtkInstanceLookup;
extern Tcl_HashTable vtkPointerLookup;
extern Tcl_HashTable vtkCommandLookup;
extern void vtkTclDeleteObjectFromHash(void *);
extern void vtkTclListInstances(Tcl_Interp *interp, ClientData arg);


extern "C" {int VTK_EXPORT Vtkpxguitcl_SafeInit(Tcl_Interp *interp);}

extern "C" {int VTK_EXPORT Vtkpxguitcl_Init(Tcl_Interp *interp);}

extern void vtkTclGenericDeleteObject(ClientData cd);


int VTK_EXPORT Vtkpxguitcl_SafeInit(Tcl_Interp *interp)
{
  return Vtkpxguitcl_Init(interp);
}


int VTK_EXPORT Vtkpxguitcl_Init(Tcl_Interp *interp)
{

  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIComponent"), vtkpxGUIComponentNewCommand,
                  vtkpxGUIComponentCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkbisGUINIFTIHeaderEditor"), vtkbisGUINIFTIHeaderEditorNewCommand,
                  vtkbisGUINIFTIHeaderEditorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIRenderer"), vtkpxGUIRendererNewCommand,
                  vtkpxGUIRendererCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUI4DRenderer"), vtkpxGUI4DRendererNewCommand,
                  vtkpxGUI4DRendererCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUI2DImageViewer"), vtkpxGUI2DImageViewerNewCommand,
                  vtkpxGUI2DImageViewerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIMosaicViewer"), vtkpxGUIMosaicViewerNewCommand,
                  vtkpxGUIMosaicViewerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIObjectmapMosaicViewer"), vtkpxGUIObjectmapMosaicViewerNewCommand,
                  vtkpxGUIObjectmapMosaicViewerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIMultipleMosaicViewer"), vtkpxGUIMultipleMosaicViewerNewCommand,
                  vtkpxGUIMultipleMosaicViewerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUICurveImageEditor"), vtkpxGUICurveImageEditorNewCommand,
                  vtkpxGUICurveImageEditorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIContourStackEditor"), vtkpxGUIContourStackEditorNewCommand,
                  vtkpxGUIContourStackEditorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIOrthogonalViewer"), vtkpxGUIOrthogonalViewerNewCommand,
                  vtkpxGUIOrthogonalViewerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIObjectmapOrthogonalViewer"), vtkpxGUIObjectmapOrthogonalViewerNewCommand,
                  vtkpxGUIObjectmapOrthogonalViewerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIOrthogonal4DViewer"), vtkpxGUIOrthogonal4DViewerNewCommand,
                  vtkpxGUIOrthogonal4DViewerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUI2DSplineObjectmapViewer"), vtkpxGUI2DSplineObjectmapViewerNewCommand,
                  vtkpxGUI2DSplineObjectmapViewerCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUISplineEditor"), vtkpxGUISplineEditorNewCommand,
                  vtkpxGUISplineEditorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIObjectmapMosaicEditor"), vtkpxGUIObjectmapMosaicEditorNewCommand,
                  vtkpxGUIObjectmapMosaicEditorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIObjectmapOrthogonalEditor"), vtkpxGUIObjectmapOrthogonalEditorNewCommand,
                  vtkpxGUIObjectmapOrthogonalEditorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUI2DImageEditor"), vtkpxGUI2DImageEditorNewCommand,
                  vtkpxGUI2DImageEditorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIAnalyzeHeaderEditor"), vtkpxGUIAnalyzeHeaderEditorNewCommand,
                  vtkpxGUIAnalyzeHeaderEditorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIColormapEditor"), vtkpxGUIColormapEditorNewCommand,
                  vtkpxGUIColormapEditorCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIBaseCurveControl"), vtkpxGUIBaseCurveControlNewCommand,
                  vtkpxGUIBaseCurveControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIPolyDataControl"), vtkpxGUIPolyDataControlNewCommand,
                  vtkpxGUIPolyDataControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIObjectmapSplineControl"), vtkpxGUIObjectmapSplineControlNewCommand,
                  vtkpxGUIObjectmapSplineControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUISplineControl"), vtkpxGUISplineControlNewCommand,
                  vtkpxGUISplineControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUISplineStackControl"), vtkpxGUISplineStackControlNewCommand,
                  vtkpxGUISplineStackControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIObjectmapSplineStackControl"), vtkpxGUIObjectmapSplineStackControlNewCommand,
                  vtkpxGUIObjectmapSplineStackControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUI4DSplineStackControl"), vtkpxGUI4DSplineStackControlNewCommand,
                  vtkpxGUI4DSplineStackControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUITriangulatedStackControl"), vtkpxGUITriangulatedStackControlNewCommand,
                  vtkpxGUITriangulatedStackControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUI4DTriangulatedStackControl"), vtkpxGUI4DTriangulatedStackControlNewCommand,
                  vtkpxGUI4DTriangulatedStackControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIFemPostControl"), vtkpxGUIFemPostControlNewCommand,
                  vtkpxGUIFemPostControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIVolumeControl"), vtkpxGUIVolumeControlNewCommand,
                  vtkpxGUIVolumeControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxGUIObliqueImageSliceControl"), vtkpxGUIObliqueImageSliceControlNewCommand,
                  vtkpxGUIObliqueImageSliceControlCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkpxTalairachTransform"), vtkpxTalairachTransformNewCommand,
                  vtkpxTalairachTransformCommand);
  vtkTclCreateNew(interp,const_cast<char *>("vtkmpjX3DExporter"), vtkmpjX3DExporterNewCommand,
                  vtkmpjX3DExporterCommand);
  char pkgName[]="vtkpxGUITCL";
  char pkgVers[]=VTK_TCL_TO_STRING(VTK_MAJOR_VERSION) "." VTK_TCL_TO_STRING(VTK_MINOR_VERSION);
  Tcl_PkgProvide(interp, pkgName, pkgVers);
  return TCL_OK;
}
