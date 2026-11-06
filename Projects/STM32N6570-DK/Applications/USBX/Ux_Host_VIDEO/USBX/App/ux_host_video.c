/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_host_video.c
  * @author  MCD Application Team
  * @brief   USBX host applicative file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "ux_host_video.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_usbx_host.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern UX_HOST_CLASS_VIDEO     *video;
extern TX_SEMAPHORE video_transfer_semaphore;
video_streaming_stateTypeDef video_streaming_state;
UX_HOST_CLASS_VIDEO_TRANSFER_REQUEST video_transfer_request;
ULONG max_payload_size;
UCHAR *frame_buffer;
UINT start_of_image = 0;
UCHAR *image_buffer;
UINT image_buffer_size;
UINT image_number;
ULONG video_buffer_size;
UINT buffer_offset;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* USER CODE BEGIN 1 */

/**
  * @brief  Function implementing video_thread_entry.
  * @param  thread_input: not used
  * @retval none
  */
VOID video_thread_entry(ULONG thread_input)
{

  UINT status = UX_SUCCESS;

  ULONG format = 0;
  ULONG frameWidth = 0;
  ULONG frameHeight = 0;
  ULONG frameInterval = 0;
  ULONG *frameIntervals ;
  uint8_t IntervalsIndex = 0;
  UX_HOST_CLASS_VIDEO_PARAMETER_FRAME_DATA frame_parameter;
  UX_HOST_CLASS_VIDEO_PARAMETER_FORMAT_DATA format_parameter;
  format_parameter.ux_host_class_video_parameter_format_requested = 1;

  UX_PARAMETER_NOT_USED(thread_input);

  while(1)
  {
    /* start if the video device is connected */
    if ((video != NULL) && (video -> ux_host_class_video_state == UX_HOST_CLASS_INSTANCE_LIVE))
    {
      switch (video_streaming_state)
      {
      case VIDEO_IDLE :

        /* Get first format data */
        status = _ux_host_class_video_format_data_get(video, &format_parameter);
        if (status != UX_SUCCESS)
        {
          Error_Handler();
        }

        /* Get the frame data */
        frame_parameter.ux_host_class_video_parameter_frame_requested = 1;
        frame_parameter.ux_host_class_video_parameter_frame_subtype = format_parameter.ux_host_class_video_parameter_format_subtype;
        status = _ux_host_class_video_frame_data_get(video, &frame_parameter);
        if (status != UX_SUCCESS)
        {
          Error_Handler();
        }

        /* Use the retrieved parameters values */
        format = format_parameter.ux_host_class_video_parameter_format_subtype;
        frameWidth = frame_parameter.ux_host_class_video_parameter_frame_width;
        frameHeight = frame_parameter.ux_host_class_video_parameter_frame_height;
        frameIntervals = (ULONG *)frame_parameter.ux_host_class_video_parameter_frame_intervals;
        frameInterval = frameIntervals[IntervalsIndex];

        /* set parameters to Retrieve supported format data and frame data */
        status = ux_host_class_video_frame_parameters_set(video, format, frameWidth, frameHeight, frameInterval);
        if (status != UX_SUCCESS)
        {
          Error_Handler();
        }

        /* Get video max payload size */
        max_payload_size = ux_host_class_video_max_payload_get(video);

        /* Update video demo state machine */
        video_streaming_state = VIDEO_START;

        break;

      case VIDEO_START :

        /* start the video */
        if (ux_host_class_video_start(video) != UX_SUCCESS)
        {
          /* try next format */
          format_parameter.ux_host_class_video_parameter_format_requested++;
          video_streaming_state = VIDEO_IDLE;
          break;
        }

        frame_buffer = ux_utility_memory_allocate(UX_SAFE_ALIGN, UX_CACHE_SAFE_MEMORY, max_payload_size);
        if (frame_buffer == UX_NULL)
        {
          Error_Handler();
        }
        video_buffer_size = max_payload_size * 70U;

        /* Allocate buffer for format data */
        image_buffer = ux_utility_memory_allocate(UX_SAFE_ALIGN, UX_CACHE_SAFE_MEMORY, video_buffer_size);
        if (image_buffer == UX_NULL)
        {
          ux_utility_memory_free(frame_buffer);
          ux_utility_memory_free(image_buffer);

          /* try next FrameInterval to reduce the resolution */
          IntervalsIndex ++;

          if (IntervalsIndex > frame_parameter.ux_host_class_video_parameter_frame_interval_type)
          {
            Error_Handler();
          }

          video_streaming_state = VIDEO_IDLE;
          break;
        }
        image_buffer_size = 0;

        /* reset frame buffer memory */
        ux_utility_memory_set(frame_buffer, 0, max_payload_size);
        ux_utility_memory_set(image_buffer, 0, video_buffer_size);


        /* Update video demo state machine */
        video_streaming_state = VIDEO_READ;

        break;

      case VIDEO_READ :

        /* Prepare the video transfer_request */
        video_transfer_request.ux_host_class_video_transfer_request_data_pointer = frame_buffer;
        video_transfer_request.ux_host_class_video_transfer_request_requested_length = max_payload_size;
        video_transfer_request.ux_host_class_video_transfer_request_class_instance = video;
        video_transfer_request.ux_host_class_video_transfer_request_completion_function = video_transfer_request_completion;

        /* read the transfer request */
        ux_host_class_video_read(video, &video_transfer_request);

        tx_semaphore_get(&video_transfer_semaphore, TX_WAIT_FOREVER);

        break;

      case VIDEO_WAIT :

      default:

        /* Sleep thread for 10 ms */
        tx_thread_sleep(MS_TO_TICK(10));

        break;
      }
    }
    else
    {
      /* Sleep thread for 10 ms */
      tx_thread_sleep(MS_TO_TICK(10));
    }
  }
}

VOID video_transfer_request_completion(UX_HOST_CLASS_VIDEO_TRANSFER_REQUEST *video_transfer_request)
{
  UINT buffer_index = 0;

  tx_semaphore_put(&video_transfer_semaphore);

  buffer_index = start_of_image ? buffer_offset : 0;

  while (buffer_index <max_payload_size)
  {
    /* The structure of each frame is SOI at 0xFFD8 and EOI at 0xFFD9*/
    if (!start_of_image && frame_buffer[buffer_index] == 0xFF && frame_buffer[buffer_index + 1] == 0xD8)
    {
      start_of_image = 1;
      image_buffer_size = 0;
      image_number++;
      buffer_offset = buffer_index;
      USBH_UsrLog("\n frame number %d ", image_number);
    }
    if (start_of_image )
    {
      /* overflow check*/
      if (image_buffer_size < video_buffer_size)
      {
        image_buffer[image_buffer_size++] = frame_buffer[buffer_index];

        /* end of image*/
        if (frame_buffer[buffer_index] == 0xFF && frame_buffer[buffer_index + 1] == 0xD9)
        {
          image_buffer[image_buffer_size++] = frame_buffer[buffer_index + 1];
          USBH_UsrLog("\n frame size = %d ", image_buffer_size);
          start_of_image = 0;
        }
      }
      else
      {
        image_buffer_size = 0;
      }
    }
    buffer_index++;
  }
}


/* USER CODE END 1 */
