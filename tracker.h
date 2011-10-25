#include "stdafx.h"

typedef BYTE* ImageData;
typedef list<ImageData> ImageDataList; 

class Tracker {
private:
  //Event Handle
  HANDLE next_color_frame_event_;
  HANDLE next_depth_frame_event_;
  HANDLE depth_read_buffer_full_event_;
  HANDLE color_read_buffer_full_event_;
  HANDLE depth_read_buffer_empty_event_;
  HANDLE color_read_buffer_empty_event_;
  HANDLE depth_write_buffer_full_event_;
  HANDLE depth_write_buffer_empty_event_;
  HANDLE color_write_buffer_full_event_;
  HANDLE color_write_buffer_empty_event_;
  //Mutex Handle
  HANDLE read_mutex_;
  HANDLE write_mutex_;
  //Thread Handle
  HANDLE read_depth_frame_thread_;
  HANDLE read_color_frame_thread_;
  HANDLE process_thread_;
  HANDLE write_thread_;
  //Stream Handle
  HANDLE depth_stream_;
  HANDLE color_stream_;
  //The image needs to be tracker
  ImageDataList depth_image_process_buffer_;
  ImageDataList color_image_process_buffer_; 
  //The image read from camera
  ImageDataList depth_image_read_buffer_;
  ImageDataList color_image_read_buffer_;
  //The image needs to be showed
  ImageDataList depth_image_write_buffer_;
  ImageDataList color_image_write_buffer_;
  
  //
  bool is_running;

  //Start get frame stream thread
 // DWORD WINAPI _GetDepthStream(LPVOID thread_parameter);
  //DWORD WINAPI _GetColorStream(LPVOID thread_parameter);
  //Process Buffered Image, etc. Detection, Tracking, and...
 // DWORD WINAPI _ImageProcess(LPVOID thread_parameter);
  //Show Image
  //DWORD WINAPI _ImageShow(LPVOID thread_parameter);
public:
  Tracker();
  ~Tracker();
  //Initialize kincet 
  void Init();
  //Open Kincet depth thread
  void OpenDepthCamera();
  void OpenColorCamera();
  //Start get frame stream
  void GetDepthStream();
  void StartGetDepthStreamThread();
  void GetColorStream();
  //Process image
  void DepthImageProcess();
  void StartImageProcessThread();
  //Show image
  void ShowDepthImage();
  void StartImageShowThread();
protected:
};


void ReadDepthLockedRect(KINECT_LOCKED_RECT& locked_rect, int w, int h, BYTE * data);
RGBQUAD NuiShortToQuadDepth( USHORT s );