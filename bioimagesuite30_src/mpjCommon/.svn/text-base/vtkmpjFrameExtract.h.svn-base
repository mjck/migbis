
#ifndef __vtkmpjFrameExtract_h
#define __vtkmpjFrameExtract_h


#include "vtkThreadedImageAlgorithm.h"

class vtkmpjFrameExtract : public vtkThreadedImageAlgorithm
{
public:
  static vtkmpjFrameExtract *New();
  vtkTypeMacro(vtkmpjFrameExtract,vtkThreadedImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set subset of frames to extract
  vtkSetMacro(StartFrame, int);
  vtkGetMacro(StartFrame, int);
  vtkSetMacro(EndFrame, int);
  vtkGetMacro(EndFrame, int);

  // Description:
  // Set subset of frames to extract
  void SetFrameInterval(int start, int end);

  // Convenience method for extracting only one frame
  void SetFrameNumber(int frame);

protected:
  vtkmpjFrameExtract();
  ~vtkmpjFrameExtract() {};

  int StartFrame;
  int EndFrame;

  virtual int RequestInformation (vtkInformation *, vtkInformationVector**,
                                  vtkInformationVector *);
  
  void ThreadedExecute (vtkImageData *inData, vtkImageData *outData, 
                       int ext[6], int id);
private:
  vtkmpjFrameExtract(const vtkmpjFrameExtract&);  // Not implemented.
  void operator=(const vtkmpjFrameExtract&);  // Not implemented.
};

#endif










