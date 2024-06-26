#ifndef __FA250DEC__
#define __FA250DEC__
#include <stdint.h>
#include <TROOT.h>
#include <TFile.h>
#include <TH1F.h>
#include <TTree.h>
#include <TString.h>

/* 0: BLOCK HEADER */
typedef struct
{
  uint32_t NSA:9;
  uint32_t NSB:9;
  uint32_t PL:11;
  uint32_t undef:2;
  uint32_t data_type_defining:1;
} fa250_block_header_2;

typedef union
{
  uint32_t raw;
  fa250_block_header_2 bf;
} fa250_block_header_2_t;

/* 2: EVENT HEADER */
typedef struct
{
  uint32_t trigger_number:12;
  uint32_t trigger_time:10;
  uint32_t slot_number:5;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_event_header;

typedef union
{
  uint32_t raw;
  fa250_event_header bf;
} fa250_event_header_t;

/* 3: TRIGGER TIME */
typedef struct
{
  uint32_t T_F:8;
  uint32_t T_E:8;
  uint32_t T_D:8;
  uint32_t T_C:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_trigger_time_1;

typedef union
{
  uint32_t raw;
  fa250_trigger_time_1 bf;
} fa250_trigger_time_1_t;

typedef struct
{
  uint32_t T_C:8;
  uint32_t T_B:8;
  uint32_t T_A:8;
  uint32_t undef:3;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_trigger_time_2;

typedef union
{
  uint32_t raw;
  fa250_trigger_time_2 bf;
} fa250_trigger_time_2_t;

/* 4: WINDOW RAW DATA */
typedef struct
{
  uint32_t PTW:9;
  uint32_t undef:14;
  uint32_t channel_number:4;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_window_raw_data_1;

typedef union
{
  uint32_t raw;
  fa250_window_raw_data_1 bf;
} fa250_window_raw_data_1_t;

typedef struct
{
  uint32_t adc_sample_2:13;
  uint32_t invalid_2:1;
  uint32_t undef2:2;
  uint32_t adc_sample_1:13;
  uint32_t invalid_1:1;
  uint32_t undef1:1;
  uint32_t data_type_defining:1;
} fa250_window_raw_data_n;

typedef union
{
  uint32_t raw;
  fa250_window_raw_data_n bf;
} fa250_window_raw_data_n_t;

/* 5: PEPPo Channel Sums  */
typedef struct
{
  uint32_t hi_sum:16;
  uint32_t _blank0:7;
  uint32_t channel_number:4;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_peppo_hi_sum;

typedef union
{
  uint32_t raw;
  fa250_peppo_hi_sum bf;
} fa250_peppo_hi_sum_t;

typedef struct
{
  uint32_t lo_sum:24;
  uint32_t _blank0:7;
  uint32_t data_type_defining:1;
} fa250_peppo_lo_sum;

typedef union
{
  uint32_t raw;
  fa250_peppo_lo_sum bf;
} fa250_peppo_lo_sum_t;

/* 7: PULSE INTEGRAL */
typedef struct
{
  uint32_t integral:19;
  uint32_t quality_factor:2;
  uint32_t pulse_number:2;
  uint32_t channel_number:4;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_pulse_integral;

typedef union
{
  uint32_t raw;
  fa250_pulse_integral bf;
} fa250_pulse_integral_t;

/* 8: PULSE TIME */
typedef struct
{
  uint32_t time:16;
  uint32_t blank:3;
  uint32_t quality_factor:2;
  uint32_t pulse_number:2;
  uint32_t channel_number:4;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_pulse_time;

typedef union
{
  uint32_t raw;
  fa250_pulse_time bf;
} fa250_pulse_time_t;


/* 9: PULSE PARAMETERS */
typedef struct
{
  uint32_t pedestal_sum:14;
  uint32_t pedestal_quality:1;
  uint32_t channel_number:4;
  uint32_t event_number_within_block:8;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_pulse_parameters_1;

typedef union
{
  uint32_t raw;
  fa250_pulse_parameters_1 bf;
} fa250_pulse_parameters_1_t;

typedef struct
{
  uint32_t pulse_samples_over_threshold:9;
  uint32_t underflow:1;
  uint32_t overflow:1;
  uint32_t NSA_beyond_PTW:1;
  uint32_t sample_sum:18;
  uint32_t parameter_type_tag:1;
  uint32_t data_type_defining:1;
} fa250_pulse_parameters_2;

typedef union
{
  uint32_t raw;
  fa250_pulse_parameters_2 bf;
} fa250_pulse_parameters_2_t;


typedef struct
{
  uint32_t samples_over_threshold:1;
  uint32_t vpeak_not_found:1;
  uint32_t vpeak_beyond:1;
  uint32_t pulse_peak:12;
  uint32_t fine_time:6;
  uint32_t coarse_time:9;
  uint32_t parameter_type_tag:1;
  uint32_t data_type_defining:1;
} fa250_pulse_parameters_3;

typedef union
{
  uint32_t raw;
  fa250_pulse_parameters_3 bf;
} fa250_pulse_parameters_3_t;


/* 12: SCALER */
typedef struct
{
  uint32_t number_scaler_words:6;
  uint32_t undef:21;
  uint32_t data_type_tag:4;
  uint32_t data_type_defining:1;
} fa250_scaler_1;

typedef union
{
  uint32_t raw;
  fa250_scaler_1 bf;
} fa250_scaler_1_t;


typedef struct
{
  uint32_t data:32;
} fa250_scaler_2;

typedef union
{
  uint32_t raw;
  fa250_scaler_2 bf;
} fa250_scaler_2_t;

void  fa250DataDecode(uint32_t data);

void  ProcessDataAndFillTree(Float_t data[3], TTree& tree);

extern int decodedData[3];

#endif /* __FA250DEC__ */