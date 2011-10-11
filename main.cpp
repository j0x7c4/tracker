#include "tracker.h";
int main ( ) {
  Tracker tracker;
  tracker.Init();
  tracker.OpenDepthCamera();
  tracker.StartGetDepthStreamThread();
  tracker.StartImageProcessThread();
  tracker.StartImageShowThread();
  getchar();
  return 0;
}
////#include<Windows.h>
////#include<iostream>
////#include<MSR_NuiApi.h>
////#include<cv.h>
////#include<highgui.h>
////
////
////
////
////
////int main () 
////{
////	//Event handle
////	HANDLE depthframe_event = NULL;
////	HANDLE colorframe_event;
////	//HANDLE skeletonframe_event;
////	HANDLE nui_process_stop_event;
////
////	HANDLE depth_stream;
////	HANDLE color_stream;
////	//Result Handle
////	HRESULT hr;
////	const NUI_IMAGE_FRAME* image_frame = NULL;
////	//CV windows initilization
////	//cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
////    //cvNamedWindow("Depth", CV_WINDOW_AUTOSIZE);
////    //cvNamedWindow("Grey", CV_WINDOW_AUTOSIZE);
////
////	IplImage *rgb32;
////    IplImage *grey;
////    IplImage *depth32;
////
////	
////	
////	//Start: Initialize your kinect with depth camera
////	hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH);
////	if ( FAILED(hr) )
////	{
////		printf("Fail to open device!\n");
////		exit(1);
////	}
////	//hr = NuiSkeletonTrackingEnable( skeletonframe_event, 0 );
////	//if( FAILED( hr ) ) printf("TrackingEnable verkackt \n");
////	
////	//hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480,0, 2, colorframe_event, &color_stream );
////	//if( FAILED( hr ) ) printf("VideoStreamHandle verkackt \n");
////
////	//depthframe_event = CreateEvent(NULL, TRUE, FALSE, NULL);
////	hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH,NUI_IMAGE_RESOLUTION_320x240,0,2,depthframe_event,	&depth_stream);
////	if( FAILED( hr ) ) printf("DepthStreamHandle verkackt \n");
////
////	//PBYTE rgb32_data = (PBYTE) malloc(640*480*3*sizeof(BYTE));
////   
////	while ( 0 )
////	{
////		
////		hr = NuiImageStreamGetNextFrame(depth_stream,0,&image_frame);
////
////		if ( hr == E_NUI_FRAME_NO_DATA ) continue;
////		else if ( FAILED(hr) ) break;
////		NUI_IMAGE_FRAME* buffer;
////		//memcpy(buffer,image_frame,sizeof(*image_frame));
////		NuiImageBuffer* texture_pointer = image_frame->pFrameTexture;
////		KINECT_LOCKED_RECT locked_rect;
////		texture_pointer->LockRect(0, &locked_rect, NULL, 0);
////
////		//ReadDepthLockedRect(locked_rect,320,240,depth32_data);
////		
////		//cvSetData(depth32, depth32_data, depth32->widthStep);
////		BYTE * p_buffer=(BYTE*)locked_rect.pBits;
////		//USHORT* p_buffer_run = (USHORT*)p_buffer;
////
////		for(int row=0; row<240; row++)
////		{
////			for(int col=0; col<320; col++)
////			{
////				int depth_idx= (row*320+col)*2;
////				low = (int)p_buffer[depth_idx];
////				high = (int)p_buffer[depth_idx+1];
////				int value = low + (high<<8);
////				if ( value <900 )
////				{
////					cvSet2D(depth32, row, col, cvScalar(255, 0, 0));
////				}
////				else if ( value>2000 )
////				{
////					cvSet2D(depth32, row, col, cvScalar(0, 255, 0));
////				}
////				else
////				{
////					cvSet2D(depth32, row, col, cvScalar(0, 0, 255));
////				}
////			}
////		}
////		texture_pointer->UnlockRect(0);
////		cvShowImage("Depth", depth32);
////		cvWaitKey(33);
////		NuiImageStreamReleaseFrame(depth_stream,image_frame);
////		
////	}
////	cvReleaseImage(&depth32);
////
////	
////	
////	return 0;
////}
//#include<windows.h>
//#include<iostream>
//#include<list>
//#include<string>
//#include <tchar.h>
//#include<MSR_NuiApi.h>
//#include<cv.h>
//#include<highgui.h>
//using namespace std;
//
//HANDLE mutex_handle = NULL;
//HANDLE image_read_ready_event_handle = NULL;
//HANDLE image_write_ready_event_handle = NULL;
//HANDLE thread_handle = NULL;
//HANDLE depth_image_ready_event_handle = NULL;
//list<IplImage> messages;
//bool is_running = true;
//
//
//HANDLE color_stream;
////Result Handle
//HRESULT hr;
//
//
//
//IplImage *rgb32;
//IplImage *grey;
//IplImage *depth32;
//
//DWORD WINAPI ShowImageFrame ( LPVOID thread_parameter ) {
//  DWORD result;
//  
//  while ( is_running ) {
//	//cout<<"in "<<GetCurrentThreadId()<<endl;
//    list<IplImage> message_list(messages);
//	result = WaitForSingleObject(image_read_ready_event_handle,
//		                         INFINITE);
//	if ( result != WAIT_OBJECT_0 && result != WAIT_ABANDONED ) {
//	  cout<<"wait error!"<<endl;
//	  return -1;
//	}
//	result = WaitForSingleObject(mutex_handle,
//		                         INFINITE);
//	
//	if ( result != WAIT_OBJECT_0 && result != WAIT_ABANDONED ) {
//	  cout<<"wait error!"<<endl;
//	  return -2;
//	}
//	messages.clear();
//	ReleaseMutex(mutex_handle);
//
//	for ( list<IplImage>::iterator iter = message_list.begin();
//		iter != message_list.end();
//		iter++ ) {
//	  cvShowImage("Depth",&(*iter));
//	  cout<<"show_image\n"<<endl;
//	}
//	SetEvent(image_write_ready_event_handle);
//	cout<<"show_image_done!\n"<<endl;
//	ResetEvent(image_read_ready_event_handle);
//  }
//  return 0;
//}
//
//
//int _tmain ( int argc, _TCHAR* argv[]){
//  HANDLE depth_stream = NULL;
//  const NUI_IMAGE_FRAME* image_frame = NULL;
//  mutex_handle = CreateMutex(NULL,FALSE,NULL);
//  image_read_ready_event_handle = CreateEvent(NULL,FALSE,FALSE,NULL); 
//  image_write_ready_event_handle = CreateEvent(NULL,FALSE,FALSE,NULL); 
//  is_running = true;
//  thread_handle = CreateThread(NULL,0,
//     (LPTHREAD_START_ROUTINE)ShowImageFrame, NULL, 0, NULL);
//
//  cvNamedWindow("Depth", CV_WINDOW_AUTOSIZE);
//
//  //Start: Initialize your kinect with depth camera
//  hr = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH);
//  if ( FAILED(hr) ) {
//    printf("Fail to open device!\n");
//    exit(1);
//  }
//  PBYTE depth32_data = (PBYTE) malloc(320*240*3*sizeof(BYTE));
//
//  depth32 = cvCreateImage(cvSize(320, 240), IPL_DEPTH_8U, 3);
//  int low,high;
//
//  hr = NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH,NUI_IMAGE_RESOLUTION_320x240,0,2,depth_stream,	&depth_stream);
//  if( FAILED( hr ) ) printf("DepthStreamHandle verkackt \n");
//
//  while ( is_running ) {
//	hr = NuiImageStreamGetNextFrame(depth_stream,200,&image_frame);
//
//    if ( hr == E_NUI_FRAME_NO_DATA ) 
//      continue;
//	else if ( FAILED(hr) ) break;
//	NUI_IMAGE_FRAME* buffer;
//	
//    NuiImageBuffer* texture_pointer = image_frame->pFrameTexture;
//    KINECT_LOCKED_RECT locked_rect;
//    texture_pointer->LockRect(0, &locked_rect, NULL, 0);
//	BYTE * p_buffer=(BYTE*)locked_rect.pBits;
//	//USHORT* p_buffer_run = (USHORT*)p_buffer;
//    for(int row=0; row<240; row++) {
//      for(int col=0; col<320; col++) {
//        int depth_idx= (row*320+col)*2;
//        low = (int)p_buffer[depth_idx];
//        high = (int)p_buffer[depth_idx+1];
//		int value = low + (high<<8);
//		if ( value <900 ) {
//          cvSet2D(depth32, row, col, cvScalar(255, 0, 0));
//		}
//        else if ( value>2000 ) {
//          cvSet2D(depth32, row, col, cvScalar(0, 255, 0));
//        }
//        else {
//          cvSet2D(depth32, row, col, cvScalar(0, 0, 255));
//        }
// 	  }
//    }
//    texture_pointer->UnlockRect(0);
//	DWORD result = WaitForSingleObject(mutex_handle,INFINITE);
//	if ( result != WAIT_OBJECT_0 && result != WAIT_ABANDONED ) {
//	  cout<<"Wait error!"<<endl;
//	  return -1;
//	}
//	messages.push_back(*depth32);
//	ReleaseMutex(mutex_handle);
//	
//	
//	NuiImageStreamReleaseFrame(depth_stream,image_frame);
//	//cvWaitKey(1000000);
//	result = WaitForSingleObject(image_write_ready_event_handle,
//		                         INFINITE);
//	if ( result != WAIT_OBJECT_0 ) {
//	  cout<<"wait error!"<<endl;
//	  return -1;
//	}
//  }
//  cvReleaseImage(&depth32);
//  CloseHandle(mutex_handle);
//  CloseHandle(image_read_ready_event_handle);
//  CloseHandle(image_write_ready_event_handle);
//  CloseHandle(thread_handle);
//
//  //End: Shut down the device
//  NuiShutdown();
//
//  return 0;
//}
//
////#include <Windows.h>
////#include <MSR_NuiApi.h>
////#include <cv.h>
////#include <highgui.h>
////#include <iostream>
////using namespace std;
////
////int main()
////{
////	HRESULT result;
////	HANDLE m_pDepthStreamHandle = NULL;
////	HANDLE m_hNextDepthFrameEvent = NULL;
////	const NUI_IMAGE_FRAME * pDepthFrame = NULL;
////	NuiImageBuffer * pDepthTexture = NULL;
////    KINECT_LOCKED_RECT LockedDepthRect;
////
////	HANDLE m_pVideoStreamHandle = NULL;
////	HANDLE m_hNextVideoFrameEvent = NULL;
////	const NUI_IMAGE_FRAME * pVideoFrame = NULL;
////	NuiImageBuffer * pVideoTexture = NULL;
////    KINECT_LOCKED_RECT LockedVideoRect;
////
////	result = NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH);
////	if(result!=0)
////	{
////		cout << "Kinect initial error" << endl;
////	}
////	else
////	{
////		cout << "Kinect initial success" << endl;
////	}
////
////	result = NuiImageStreamOpen( NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, 0, 4, m_hNextVideoFrameEvent, &m_pVideoStreamHandle);
////	if(result!=0)
////	{
////		cout << "video stream open error" << endl;
////	}
////	else
////	{
////		cout << "video stream open success" << endl;
////	}
////
////	result = NuiImageStreamOpen( NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_320x240, 0, 4, m_hNextDepthFrameEvent, &m_pDepthStreamHandle);
////	if(result!=0)
////	{
////		cout << "depth stream open error" << endl;
////	}
////	else
////	{
////		cout << "depth stream open success" << endl;
////	}
////
////	cvNamedWindow( "depth_img" );
////	cvNamedWindow( "video_img" );
////	IplImage *depth_img = cvCreateImage(cvSize(320, 240), 8, 1);
////	IplImage *video_img = cvCreateImage(cvSize(640, 480), 8, 3);
////
////	while(true)
////	{
////		result = NuiImageStreamGetNextFrame( m_pDepthStreamHandle, 0, &pDepthFrame );
////		while(result!=0)
////		{
////			result = NuiImageStreamGetNextFrame( m_pDepthStreamHandle, 0, &pDepthFrame );
////			cout << "get depth frame error" << endl;
////		}
////		cout << "get depth frame success" << endl;
////
////		result = NuiImageStreamGetNextFrame( m_pVideoStreamHandle, 0, &pVideoFrame );
////		while(result!=0)
////		{
////			result = NuiImageStreamGetNextFrame( m_pVideoStreamHandle, 0, &pVideoFrame );
////			cout << "get video frame error" << endl;
////		}
////		cout << "get video frame success" << endl;
////
////		pDepthTexture = pDepthFrame->pFrameTexture;
////		pVideoTexture = pVideoFrame->pFrameTexture;
////
////		pDepthTexture->LockRect( 0, &LockedDepthRect, NULL, 0 );
////		pVideoTexture->LockRect( 0, &LockedVideoRect, NULL, 0 );
////
////		BYTE * pDepthBuffer = (BYTE*) LockedDepthRect.pBits;
////		BYTE * pVideoBuffer = (BYTE*) LockedVideoRect.pBits;
////
////		int depth_idx, video_idx;
////		int low, high, value;
////		int r, g, b;
////		for(int row=0; row<240; row++)
////		{
////			for(int col=0; col<320; col++)
////			{
////				depth_idx = (row*320+col)*2;
////				low = (int)pDepthBuffer[depth_idx];
////				high = (int)pDepthBuffer[depth_idx+1];
////				value = low + (high<<8);
////				value = (value/9000.0)*255;
////				cvSet2D(depth_img, row, col, cvScalar(value, 0, 0));
////			}
////		}
////
////		for(int row=0; row<480; row++)
////		{
////			for(int col=0; col<640; col++)
////			{
////				video_idx = (row*640+col)*4;
////				r = (int)pVideoBuffer[video_idx];
////				g = (int)pVideoBuffer[video_idx+1];
////				b = (int)pVideoBuffer[video_idx+2];
////				cvSet2D(video_img, row, col, cvScalar(r, g, b));
////			}
////		}
////
////		pDepthTexture->UnlockRect(0);
////		pVideoTexture->UnlockRect(0);
////
////		NuiImageStreamReleaseFrame( m_pDepthStreamHandle, pDepthFrame );
////		NuiImageStreamReleaseFrame( m_pVideoStreamHandle, pVideoFrame );
////
////		cvShowImage( "depth_img", depth_img);
////		cvShowImage( "video_img", video_img);
////
////		char c = cvWaitKey(33);
////		if(c==27)	break;
////	}
////
////	
////	cvReleaseImage(&depth_img);
////	cvDestroyWindow("depth_img");
////	cvReleaseImage(&video_img);
////	cvDestroyWindow("video_img");
////
////	NuiShutdown( );
////	return 0;
////}
