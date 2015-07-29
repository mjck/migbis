
#ifndef __vtkmpjImageExtractComponents_h
#define __vtkmpjImageExtractComponents_h


#include "vtkThreadedImageAlgorithm.h"

class vtkmpjImageExtractComponents : public vtkThreadedImageAlgorithm
{
public:
  static vtkmpjImageExtractComponents *New();
  vtkTypeMacro(vtkmpjImageExtractComponents,vtkThreadedImageAlgorithm);
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
  vtkmpjImageExtractComponents();
  ~vtkmpjImageExtractComponents() {};

  int StartFrame;
  int EndFrame;

  virtual int RequestInformation (vtkInformation *, vtkInformationVector**,
                                  vtkInformationVector *);
  
  void ThreadedExecute (vtkImageData *inData, vtkImageData *outData, 
                       int ext[6], int id);
private:
  vtkmpjImageExtractComponents(const vtkmpjImageExtractComponents&);  // Not implemented.
  void operator=(const vtkmpjImageExtractComponents&);  // Not implemented.
};

#endif










