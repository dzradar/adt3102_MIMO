
//#include <stm32f427xx.h>
#include <arm_math.h>
#include <math.h>
//#include <marc.h>
#include "peak_single_mimo.h"


void sort_vector_descend ( uint32 * input_vec, int num_vec_u16)
{
	int n_u16, nt_u16;
	uint32 max_flt32;
	for (n_u16 = 0; n_u16 < num_vec_u16 - 1; n_u16 ++)
	{
		max_flt32 = input_vec[n_u16];
		for(nt_u16 = n_u16; nt_u16 < num_vec_u16; nt_u16++)
		{
			if (input_vec[nt_u16] > max_flt32 )
			{
				max_flt32 = input_vec[nt_u16];
				input_vec[nt_u16] = input_vec[n_u16];
				input_vec[n_u16] = max_flt32;
			}
		}
	}
}

void noise_estimation_cfar (uint32 * tmp_rv_pflt32, uint32 * cfar_window_flt32, uint32 * rva_range_average_noise_pflt32, int v_u16)
{
	//int v_u16 = 16;
	//int nrx_u16 = 4;

	int tmp_r_s16;	
	int r_u16, n_u16;
	
	float input_r_flt32, first_r_flt32;
	int first_pos_u16, input_pos_u16;

	// do SORT for pos: 0
	// CFAR
	// 1: get N points and order
	// neighbours i +/- 1 are skipped
	r_u16 = 0;

	for(n_u16 = 0; n_u16 < cfar_window_size_u16; n_u16++ )
	{
		if ( (2u*n_u16) < cfar_window_size_u16 )
			tmp_r_s16 = (int)( r_u16 - cfar_window_size_u16/2 - 1 + n_u16);
		else 
			tmp_r_s16 = (int)( r_u16 - cfar_window_size_u16/2 + 2 + n_u16);

		if (tmp_r_s16 < 0)
		{
			tmp_r_s16 = RVA_MAX_NSAMPLES_U16 + tmp_r_s16;
		}
		else if (tmp_r_s16 >= RVA_MAX_NSAMPLES_U16)
		{
			tmp_r_s16 = tmp_r_s16 - RVA_MAX_NSAMPLES_U16;
		}
		cfar_window_flt32[n_u16] = tmp_rv_pflt32[v_u16+ tmp_r_s16*RVA_HISTORY_SIZE_U16];
	}

	// 2: sort the window
	sort_vector_descend ( cfar_window_flt32, cfar_window_size_u16);

	// 3: select threshold
	rva_range_average_noise_pflt32[r_u16] = cfar_window_flt32 [K_threshold_u16];

	// scan the whole input
	for (r_u16=1;r_u16<RVA_MAX_NSAMPLES_U16;r_u16++)
	{
		//**************** sides

		// new value in the window

		n_u16 = cfar_window_size_u16 -1;
		tmp_r_s16 = (int)( r_u16 - cfar_window_size_u16/2 + 2 + n_u16);

		if (tmp_r_s16 < 0)
		{
			tmp_r_s16 = RVA_MAX_NSAMPLES_U16 + tmp_r_s16;
		}
		else if (tmp_r_s16 >= RVA_MAX_NSAMPLES_U16)
		{
			tmp_r_s16 = tmp_r_s16 - RVA_MAX_NSAMPLES_U16;
		}
		input_r_flt32 = tmp_rv_pflt32[v_u16+ tmp_r_s16*RVA_HISTORY_SIZE_U16];

		// get first value in window
		n_u16 = 0;
		tmp_r_s16 = (int)( r_u16 -1 - cfar_window_size_u16/2 - 1 + n_u16);
	
		if (tmp_r_s16 < 0)
		{
			tmp_r_s16 = RVA_MAX_NSAMPLES_U16 + tmp_r_s16;
		}
		else if (tmp_r_s16 >= RVA_MAX_NSAMPLES_U16)
		{
			tmp_r_s16 = tmp_r_s16 - RVA_MAX_NSAMPLES_U16;
		}
		first_r_flt32 = tmp_rv_pflt32[v_u16+ tmp_r_s16*RVA_HISTORY_SIZE_U16];


	
		// replace first value by new value in cfar window
		// find pos of first value in window
		n_u16 = 0;
		while (( first_r_flt32 < cfar_window_flt32[n_u16] ) && (n_u16 < cfar_window_size_u16 ) )
		{
			n_u16 ++;
		}

		first_pos_u16 = n_u16;
		// find pos of new value in window
		n_u16 = 0;
		while ( (input_r_flt32 < cfar_window_flt32[n_u16]) && (n_u16 < cfar_window_size_u16 ))
		{
			n_u16++;
		}
		input_pos_u16 = n_u16 ;


		// replace and reorder

		if ( input_pos_u16 <= first_pos_u16 )
		{
			for ( n_u16 = first_pos_u16  ; n_u16 > input_pos_u16; n_u16 --)
			{
				cfar_window_flt32 [n_u16] = cfar_window_flt32 [n_u16 -1];
			}
			cfar_window_flt32 [n_u16] = input_r_flt32;
		}
		else if (input_pos_u16 > first_pos_u16 )
		{		
			for ( n_u16 = first_pos_u16  ; n_u16 < input_pos_u16 - 1; n_u16 ++)
			{
				cfar_window_flt32 [n_u16] = cfar_window_flt32 [n_u16 +1];
			}
			cfar_window_flt32 [n_u16] = input_r_flt32;
		}

		////*******************  middle 
		// new value in the window

		tmp_r_s16 = (int)( r_u16 - 2);

		if (tmp_r_s16 < 0)
		{
			tmp_r_s16 = RVA_MAX_NSAMPLES_U16 + tmp_r_s16;
		}
		else if (tmp_r_s16 >= RVA_MAX_NSAMPLES_U16)
		{
			tmp_r_s16 = tmp_r_s16 - RVA_MAX_NSAMPLES_U16;
		}
		input_r_flt32 = tmp_rv_pflt32[v_u16+ tmp_r_s16*RVA_HISTORY_SIZE_U16];

		// get first value in window

		tmp_r_s16 = (int)( r_u16 +1);
	
		if (tmp_r_s16 < 0)
		{
			tmp_r_s16 = RVA_MAX_NSAMPLES_U16 + tmp_r_s16;
		}
		else if (tmp_r_s16 >= RVA_MAX_NSAMPLES_U16)
		{
			tmp_r_s16 = tmp_r_s16 - RVA_MAX_NSAMPLES_U16;
		}
		first_r_flt32 = tmp_rv_pflt32[v_u16+ tmp_r_s16*RVA_HISTORY_SIZE_U16];


		// replace first value by new value in cfar window
		// find pos of first value in window
		n_u16 = 0;
		while (( first_r_flt32 < cfar_window_flt32[n_u16] ) && (n_u16 < cfar_window_size_u16 ) )
		{
			n_u16 ++;
		}

		first_pos_u16 = n_u16;
		// find pos of new value in window
		n_u16 = 0;
		while ( (input_r_flt32 < cfar_window_flt32[n_u16]) && (n_u16 < cfar_window_size_u16 ))
		{
			n_u16++;
		}
		input_pos_u16 = n_u16 ;


		// replace and reorder

		if ( input_pos_u16 <= first_pos_u16 )
		{
			for ( n_u16 = first_pos_u16  ; n_u16 > input_pos_u16; n_u16 --)
			{
				cfar_window_flt32 [n_u16] = cfar_window_flt32 [n_u16 -1];
			}
			cfar_window_flt32 [n_u16] = input_r_flt32;
		}
		else if (input_pos_u16 > first_pos_u16 )
		{		
			for ( n_u16 = first_pos_u16  ; n_u16 < input_pos_u16 -1; n_u16 ++)
			{
				cfar_window_flt32 [n_u16] = cfar_window_flt32 [n_u16 +1];
			}
			cfar_window_flt32 [n_u16] = input_r_flt32;
		}

		// 3: select threshold
		rva_range_average_noise_pflt32[r_u16] = cfar_window_flt32 [K_threshold_u16];

	}


}
