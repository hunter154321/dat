#include <stdio.h>
#include <stdint.h>
#include "jlabdec.h"
#include "fa250dec.h"

void
fa250DataDecode(uint32_t data)
{
  static uint32_t type_last = 15;	/* initialize to type FILLER WORD */
  static uint32_t time_last = 0;
  static int new_type = 0;
  int type_current = 0;
  static int pulse_number = 0;
  static int isca = 0;
  generic_data_word_t gword;

  gword.raw = data;

  if(gword.bf.data_type_defining) /* data type defining word */
    {
      new_type = 1;
      type_current = gword.bf.data_type_tag;
    }
  else
    {
      new_type = 0;
      type_current = type_last;
    }

  switch( type_current )
    {
    case 0:		/* BLOCK HEADER */
      {
	if( new_type )
	  {
	    block_header_t d; d.raw = data;

	    printf("%8X - BLOCK HEADER - slot = %d  modID = %d   n_evts = %d   n_blk = %d\n",
		   d.raw,
		   d.bf.slot_number,
		   d.bf.module_ID,
		   d.bf.number_of_events_in_block,
		   d.bf.event_block_number);

	  }
	else
	  {
	    fa250_block_header_2_t d; d.raw = data;

	    printf("%8X - BLOCK HEADER 2 - PL = %d  NSB = %d  NSA = %d\n",
		   d.raw,
		   d.bf.PL,
		   d.bf.NSB,
		   d.bf.NSA);
	  }
	break;
      }

    case 1:		/* BLOCK TRAILER */
      {
	block_trailer_t d; d.raw = data;

	printf("%8X - BLOCK TRAILER - slot = %d   n_words = %d\n",
		     d.raw,
		     d.bf.slot_number,
		     d.bf.words_in_block);
	break;
      }

    case 2:		/* EVENT HEADER */
      {
	fa250_event_header_t d; d.raw = data;

	printf("%8X - EVENT HEADER 1 - trig time = %d   trig num = %d\n",
	       d.raw,
	       d.bf.trigger_time,
	       d.bf.trigger_number);
	break;
      }

    case 3:		/* TRIGGER TIME */
      {
	if( new_type )
	  {
	    fa250_trigger_time_1_t d; d.raw = data;

	    printf("%8X - TRIGGER TIME 1 - time = %08x\n",
		   d.raw,
		   (d.bf.T_C<<24) | (d.bf.T_D<<16) | (d.bf.T_E<<8) | (d.bf.T_F) );

	    time_last = 1;
	  }
	else
	  {
	    fa250_trigger_time_2_t d; d.raw = data;
	    if( time_last == 1 )
	      {
		printf("%8X - TRIGGER TIME 2 - time = %08x\n",
		       d.raw,
		       (d.bf.T_A<<16) | (d.bf.T_B<<8) | (d.bf.T_C) );
	      }
	    else
	      printf("%8X - TRIGGER TIME - (ERROR)\n", data);

	    time_last = 0;
	  }
	break;
      }

    case 4:		/* WINDOW RAW DATA */
      {
	if( new_type )
	  {
	    fa250_window_raw_data_1_t d; d.raw = data;

	    printf("%8X - WINDOW RAW DATA - chan = %d   nsamples = %d\n",
		   d.raw,
		   d.bf.channel_number,
		   d.bf.PTW);
		decodedData[2] = d.bf.channel_number;
	  }
	else
	  {
	    fa250_window_raw_data_n_t d; d.raw = data;

	    printf("%8X - RAW SAMPLES - valid = %d  adc = %4d   valid = %d  adc = %4d\n",
		   d.raw,
		   !d.bf.invalid_1,
		   d.bf.adc_sample_1,
		   !d.bf.invalid_2,
		   d.bf.adc_sample_2);
		decodedData[0] = d.bf.adc_sample_1;
    	decodedData[1] = d.bf.adc_sample_2;
	  }
	break;
      }

    case 5:		/* PEPPo Channel Sums */
      {
	if( new_type )
	  {
	    fa250_peppo_hi_sum_t d; d.raw = data;

	    printf("%8X - PEPPo Hi Sum - chan = %d   hi sum = 0x%x\n",
		   d.raw,
		   d.bf.channel_number,
		   d.bf.hi_sum);
	    break;
	  }
	else
	  {
	    fa250_peppo_lo_sum_t d; d.raw = data;

	    printf("%8X - PEPPo Lo Sum - lo sum = 0x%06x\n",
		   d.raw,
		   d.bf.lo_sum);
	    break;
	  }
      }

    case 7:		/* PULSE INTEGRAL */
      {
	fa250_pulse_integral_t d; d.raw = data;

	printf("%8X - PULSE INTEGRAL - chan = %d   pulse # = %d   quality = %d   integral = %d\n",
	       d.raw,
	       d.bf.channel_number,
	       d.bf.pulse_number,
	       d.bf.quality_factor,
	       d.bf.integral);
	break;
      }

    case 8:		/* PULSE TIME */
      {
	fa250_pulse_time_t d; d.raw = data;

	  printf("%8X - PULSE TIME - chan = %d   pulse # = %d   quality = %d   time = %d\n",
		 d.raw,
		 d.bf.channel_number,
		 d.bf.pulse_number,
		 d.bf.quality_factor,
		 d.bf.time);
	break;
      }

    case 9:		/* PULSE PARAMETERS */
      {
	if( new_type )
	  { /* Channel ID and Pedestal Info */
	    fa250_pulse_parameters_1_t d; d.raw = data;
	    pulse_number  = 0; /* Initialize */

	    printf("%8X - PULSEPARAM 1 - evt = %d   chan = %d   quality = %d   pedsum = %d\n",
		   d.raw,
		   d.bf.event_number_within_block,
		   d.bf.channel_number,
		   d.bf.pedestal_quality,
		   d.bf.pedestal_sum);
	  }
	else
	  {
	    if(data & (1<<30))
	      { /* Word 1: Integral of n-th pulse in window */
		fa250_pulse_parameters_2_t d; d.raw = data;
		pulse_number++;

		printf("%8X - PULSEPARAM 2 - P# = %d  Sum = %d  NSA+ = %d  Ov/Un = %d/%d  #OT = %d\n",
		       d.raw,
		       pulse_number,
		       d.bf.sample_sum,
		       d.bf.NSA_beyond_PTW,
		       d.bf.overflow,
		       d.bf.underflow,
		       d.bf.pulse_samples_over_threshold);
	      }
	    else
	      { /* Word 2: Time of n-th pulse in window */
		fa250_pulse_parameters_3_t d; d.raw = data;

		printf("%8X - PULSEPARAM 3 - CTime = %d  FTime = %d  Peak = %d  NoVp = %d  Q = %d\n",
		       d.raw,
		       d.bf.coarse_time,
		       d.bf.fine_time,
		       d.bf.pulse_peak,
		       d.bf.vpeak_not_found,
		       d.bf.samples_over_threshold);

	      }
	  }

	break;
      }

    case 12:		/* SCALER HEADER */
      if( new_type )
	{
	  fa250_scaler_1_t d; d.raw = data;

	  printf("%8X - SCALER HEADER - data words = %d\n",
		 d.raw,
		 d.bf.number_scaler_words);
	  isca = 1;
	}
      else
	{
	  printf("%8X - SCALER DATA - word = %2d  counter = %d\n",
		 data,
		 isca++,
		 data);
	}
      break;

    case 13:		/* END OF EVENT */
      {
	event_trailer_t d; d.raw = data;

	printf("%8X - END OF EVENT = %d\n",
	       d.raw,
	       d.bf.data_type_tag);
	break;
      }

    case 14:		/* DATA NOT VALID (no data available) */
      {
	data_not_valid_t d; d.raw = data;

	printf("%8X - DATA NOT VALID = %d\n",
	       d.raw,
	       d.bf.data_type_tag);
	break;
      }

    case 15:		/* FILLER WORD */
      {
	filler_word_t d; d.raw = data;

	printf("%8X - FILLER WORD = %d\n",
	       d.raw,
	       d.bf.data_type_tag);
	break;
      }

    case 6:
    case 10:
    case 11:
    default:
      {
	printf("%8X - UNDEFINED TYPE = %d\n",
	       gword.raw,
	       gword.bf.data_type_tag);
	break;
      }

    }

  type_last = type_current;	/* save type of current data word */

}


void ProcessDataAndFillTree(Float_t data[3], TTree& tree) {

    printf("x=%8f, y=%8f, z=%8f\n", data[0], data[1], data[2]);

    tree.Fill();
}