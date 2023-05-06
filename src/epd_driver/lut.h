#pragma once

#include "esp_attr.h"
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "epd_driver.h"

// number of bytes needed for one line of EPD pixel data.
#define EPD_LINE_BYTES (epd_display.width / 4)
// maximum number of line bytes, if needed at compile time.
#define EPD_MAX_LINE_BYTES (EPD_WIDTH / 4)

///////////////////////////// Utils /////////////////////////////////////

/*
 * Reorder the output buffer to account for I2S FIFO order.
 */
void reorder_line_buffer(uint32_t *line_data);

typedef struct {
  const uint8_t *data_ptr;
  EpdRect crop_to;
  SemaphoreHandle_t done_smphr;
  SemaphoreHandle_t start_smphr;
  EpdRect area;
  int frame;
  /// index of the waveform mode when using vendor waveforms.
  /// This is not necessarily the mode number if the waveform header
  //only contains a selection of modes!
  int waveform_index;
  /// waveform range when using vendor waveforms
  int waveform_range;
  /// Draw time for the current frame in 1/10ths of us.
  int frame_time;
  const EpdWaveform* waveform;
  enum EpdDrawMode mode;
  enum EpdDrawError error;
  const bool *drawn_lines;
  // Queue of input data lines
  QueueHandle_t* output_queue;

  // Lookup table size.
  size_t conversion_lut_size;
  // Lookup table space.
  uint8_t* conversion_lut;
} OutputParams;


void feed_display(OutputParams *params);
void provide_out(OutputParams *params);


void write_row(uint32_t output_time_dus);
void skip_row(uint8_t pipeline_finish_time);
