#include "tracker.h"
Tracker::Tracker() {
  //Event Handle
  next_color_frame_event_ = NULL;
  next_depth_frame_event_ = NULL;
  //Stream Handle
  depth_stream_ = NULL;
  color_stream_ = NULL;
  //Mutex Handle
  read_mutex_ = NULL;
  write_mutex_ = NULL;
  //Thread Handle
  read_depth_frame_thread_ = NULL;
  read_color_frame_thread_ = NULL;
  process_thread_ = NULL;
  write_thread_ = NULL;
  //The image needs to be tracker
  image_process_buffer_.clear(); 
  //The image read from camera
  image_read_buffer_.clear();
  //The image needs to be showed
  image_write_buffer_.clear();
}
void Tracker::Init( ) {
  //Initialize Kinect
  HRESULT hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH);
  if ( FAILED(hr) ) {
    printf("Fail to open device!\n");
    exit(1);
  }
  //read_mutex_ = CreateMutex(NULL,FALSE,(LPCWSTR)"READ_MUTEX");
  //write_mutex_ = CreateMutex(NULL,FALSE,(LPCWSTR)"WRITE_MUTEX");
  depth_buffer_full_event_ = CreateEvent(NULL,FALSE,FALSE,NULL);
  depth_buffer_empty_event_ = CreateEvent(NULL,FALSE,TRUE,NULL);
  write_buffer_full_event_ = CreateEvent(NULL,FALSE,FALSE,NULL);
  write_buffer_empty_event_ = CreateEvent(NULL,FALSE,TRUE,NULL);
  is_running = true;
}
Tracker::~Tracker() {
  //Close Kincet
  NuiShutdown();
  CloseHandle(read_mutex_);
  CloseHandle(write_mutex_);
}

void Tracker::OpenDepthCamera( ) {
  next_depth_frame_event_ = CreateEvent(NULL,FALSE,FALSE,NULL);
  HRESULT hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH,
                                                            NUI_IMAGE_RESOLUTION_320x240,
                                                            0,2,next_depth_frame_event_,	&depth_stream_);
  if( FAILED( hr ) ) 
    printf("DepthStream Can't open\n");
}

void Tracker::OpenColorCamera( ) {
  next_depth_frame_event_ = CreateEvent(NULL,FALSE,FALSE,NULL);
  HRESULT hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR,
                                                            NUI_IMAGE_RESOLUTION_640x480,
                                                            0,2,next_color_frame_event_,	&color_stream_);
  if( FAILED( hr ) ) 
    printf("ColorStream Can't open\n");
}

void Tracker::GetDepthStream ( ) {
  //read_depth_frame_thread_ =CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)_GetDepthStream, NULL, 0, NULL);
  HRESULT hr;
  const NUI_IMAGE_FRAME* image_frame = NULL;
  int low,high;
  KINECT_LOCKED_RECT locked_rect;
  BYTE * p_buffer = NULL;
  DWORD result;
  NUI_IMAGE_FRAME* buffer;
  NuiImageBuffer* texture_pointer;
  ImageDataList read_buffer;
  while ( is_running ) {
    //store 100 frames for batch processing
    int buffer_left_space = 5;
    /*result = WaitForSingleObject(next_color_frame_event_,INFINITE);
    if ( result != WAIT_OBJECT_0 && result != WAIT_ABANDONED ) {
	    cout<<"Wait error!"<<endl;
	    continue;
	  }*/
    
    while ( buffer_left_space ) {
      hr = NuiImageStreamGetNextFrame(depth_stream_,200,&image_frame);
      if ( hr == E_NUI_FRAME_NO_DATA ) 
        continue;
	    else if ( FAILED(hr) ) break;
      //cout<<"Get Depth Frame!"<<endl;
      BYTE* data = (BYTE*)malloc(320*240*3*sizeof(BYTE));
      texture_pointer = image_frame->pFrameTexture;
      texture_pointer->LockRect(0, &locked_rect, NULL, 0);
	    ReadDepthLockedRect(locked_rect,320,240,data);
      texture_pointer->UnlockRect(0);
      read_buffer.push_back(data);
	    NuiImageStreamReleaseFrame(depth_stream_,image_frame);
      buffer_left_space--;
    }
    result = WaitForSingleObject(depth_buffer_empty_event_,INFINITE);
    if ( result != WAIT_OBJECT_0 && result != WAIT_ABANDONED ) {
	    cout<<"Wait error!"<<endl;
	    return;
	  }
    for ( ImageDataList::iterator iter = read_buffer.begin(); iter != read_buffer.end(); iter++ ) {
      image_read_buffer_.push_back(*iter);
    }
    SetEvent(depth_buffer_full_event_);
    //ReleaseMutex(read_mutex_);
    read_buffer.clear();
  }
 /* CloseHandle(mutex_handle);
  CloseHandle(image_read_ready_event_handle);
  CloseHandle(image_write_ready_event_handle);
  CloseHandle(thread_handle);*/
  return;
}

RGBQUAD NuiShortToQuadDepth( USHORT s ) {
  USHORT real_depth = (s & 0xfff8) >> 4;
  
  //cout<<"Depth: "<<real_depth<<endl;
  USHORT player = s & 7;

  // transform 13-bit depth information into an 8-bit intensity appropriate
  // for display (we disregard information in most significant bit)
  BYTE l = 255 - (BYTE)(256*real_depth/0x0fff);

  RGBQUAD q;
  q.rgbRed = q.rgbBlue = q.rgbGreen = 0;

  if ( real_depth < 900 ) {
    q.rgbBlue = 255;
  }
  else
    if ( real_depth > 2000 ) {
      q.rgbGreen = 255;
    }
    else {
      q.rgbRed = 255;
    }
  return q;
  /*switch( player ) {
  case 0:
    q.rgbRed = l / 2;
    q.rgbBlue = l / 2;
    q.rgbGreen = l / 2;
    break;
  case 1:
    q.rgbRed = l;
    break;
  case 2:
    q.rgbGreen = l;
    break;
  case 3:
    q.rgbRed = l / 4;
    q.rgbGreen = l;
    q.rgbBlue = l;
    break;
  case 4:
    q.rgbRed = l;
    q.rgbGreen = l;
    q.rgbBlue = l / 4;
    break;
  case 5:
    q.rgbRed = l;
    q.rgbGreen = l / 4;
    q.rgbBlue = l;
    break;
  case 6:
    q.rgbRed = l / 2;
    q.rgbGreen = l / 2;
    q.rgbBlue = l;
    break;
  case 7:
    q.rgbRed = 255 - ( l / 2 );
    q.rgbGreen = 255 - ( l / 2 );
    q.rgbBlue = 255 - ( l / 2 );
  }
  return q;*/
}

void ReadDepthLockedRect(KINECT_LOCKED_RECT& locked_rect, int w, int h, BYTE * data) {
	int distance;
	int index = 0;
	int blue_idx=0, green_idx = 1, red_idx =2;
	if( locked_rect.Pitch != 0 ) {
		BYTE * p_buffer=(BYTE*)locked_rect.pBits;
		for( int y = 0 ; y < h ; y++ ) {
			for( int x = 0 ; x < w ; x++ ) {
				index = (y*w+x)*3;
				*(data+index + blue_idx) =0;
				*(data+index + green_idx) = 0;
				*(data+index + red_idx) = 0;
        distance = int( *(p_buffer) | (*(p_buffer+1))<<8 );
        if ( distance < 1500 && distance > 1000 ) {
          *(data+index + blue_idx) =255;
				  *(data+index + green_idx) = 255;
				  *(data+index + red_idx) = 255;
        }
        else {
          *(data+index + blue_idx) =0;
				  *(data+index + green_idx) = 0;
				  *(data+index + red_idx) = 0;
          //cout<<distance<<endl;
        }
        p_buffer+=2;
			}
		}
	}
}

static DWORD WINAPI _GetDepthStream( LPVOID thread_parameter ) {
  Tracker * tracker = (Tracker*)thread_parameter;
  tracker->GetDepthStream();
  return 0;
}

void Tracker::StartGetDepthStreamThread ( ) {
  //read_depth_frame_thread_ =CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)_GetDepthStream, NULL, 0, NULL);
  read_depth_frame_thread_ = CreateThread(NULL, 0, _GetDepthStream, this, NULL, 0);
}


void Tracker::ImageProcess( ) {
  //process_thread_ =CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)_ImageProcess, NULL, 0, NULL);
  HRESULT result;
  
  while ( is_running ) {
    result = WaitForSingleObject(depth_buffer_full_event_,INFINITE);
    if ( result != WAIT_OBJECT_0 && result != WAIT_ABANDONED ) {
	    cout<<"Wait error!"<<endl;
	    return;
	  }
    ImageDataList buffer(image_read_buffer_);
    image_read_buffer_.clear();
    SetEvent(depth_buffer_empty_event_);
    //ReleaseMutex(read_mutex_);

    /* 
      To-do
      Image Process, etc. Detection, Tracking and ...
    */

    //trans procced data to write_buffer
    result = WaitForSingleObject(write_buffer_empty_event_,INFINITE);
    if ( result != WAIT_OBJECT_0 && result != WAIT_ABANDONED ) {
	    cout<<"Wait error!"<<endl;
	    return;
	  }
    for ( ImageDataList::iterator iter = buffer.begin(); iter!=buffer.end(); iter++ ) {
      image_write_buffer_.push_back(*iter);
    }
    SetEvent(write_buffer_full_event_);
  }
  return;
}

static DWORD WINAPI _ImageProcess( LPVOID thread_parameter ) {
  Tracker * tracker = (Tracker*)thread_parameter;
  tracker->ImageProcess();
  return 0;
}

void Tracker::StartImageProcessThread ( ) {
  //read_depth_frame_thread_ =CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)_GetDepthStream, NULL, 0, NULL);
  process_thread_ = CreateThread(NULL, 0, _ImageProcess, this, NULL, 0);
}

void Tracker::ImageShow( ) {
  cvNamedWindow("DepthImage", CV_WINDOW_AUTOSIZE);
  //write_thread_ =CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)_ImageShow, NULL, 0, NULL);
  HRESULT result;
  IplImage *image = cvCreateImageHeader(cvSize(320, 240), IPL_DEPTH_8U, 3);
  while ( is_running ) {
    result = WaitForSingleObject(write_buffer_full_event_,INFINITE);
    for ( ImageDataList::iterator iter = image_write_buffer_.begin(); iter != image_write_buffer_.end() ; iter++ ) {
      cvSetData(image,(BYTE*)(*iter),image->widthStep);
      cvShowImage("DepthImage",image);
      cvWaitKey(1);
    }
    SetEvent(write_buffer_empty_event_);
    //clear data
    for ( ImageDataList::iterator iter = image_write_buffer_.begin(); iter != image_write_buffer_.end() ; iter++ ) {
      delete *iter;
    }
    image_write_buffer_.clear();
    //ReleaseMutex(write_mutex_);
  }
  cvReleaseImage(&image);
  return;
}

static DWORD WINAPI _ImageShow( LPVOID thread_parameter ) {
  Tracker * tracker = (Tracker*)thread_parameter;
  tracker->ImageShow();
  return 0;
}

void Tracker::StartImageShowThread ( ) {
  //read_depth_frame_thread_ =CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)_GetDepthStream, NULL, 0, NULL);
  write_thread_ = CreateThread(NULL, 0, _ImageShow, this, NULL, 0);
}