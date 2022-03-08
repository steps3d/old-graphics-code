//
//
//

#include	"half.h"

uint32_t	mantTable  [2048];
uint32_t	expTable   [64];
uint16_t	offsTable  [64];
uint16_t	baseTable  [512];
uint8_t		shiftTable [512];

static unsigned int convertmantissa(unsigned int i)
{
	uint32_t m = i << 13;		// zero pad mantissa bits
	uint32_t e = 0; 			// zero exponent
	
	while ( !(m & 0x00800000))	 // while not normalized
	{
		e -= 0x00800000;		// decrement exponent (1<<23)
		m <<= 1;				// shift mantissa
	}
	m &= ~0x00800000;			// clear leading 1 bit
	e += 0x38800000;			// adjust bias ((127-14)<<23)

	return m | e;				// return combined number
}

static void	initTables ()
{
	static	bool	inited = false;
	int	i;

	if ( inited )
		return;
		
	inited        = true;
	mantTable [0] = 0;
	
	for ( i = 1; i < 1024; i++ )
		mantTable [i] = convertmantissa ( i );

	for ( i = 1024; i < 2048; i++ )
		mantTable [i] = 0x38000000 + ((i - 1024) << 13);

	expTable [0]  = 0;
	expTable [32] = 0x80000000;

	for ( i = 1; i <= 30; i++ )
		expTable [i] = i << 23;

	for ( int i = 33; i < 63; i++ )
		expTable [i] = 0x80000000 + ((i-32) << 23);

	expTable [31] = 0x47800000;
	expTable [63] = 0xC7800000;

	for ( i = 1; i < 64; i++ )
		offsTable [i] = 1024;

	offsTable [0]  = 0;
	offsTable [32] = 0;

	for ( i = 0; i < 256; i++ )
	{
		int	e = i - 127;
		
		if ( e < -24 )			// Very small numbers map to zero
		{
			baseTable  [i | 0x000] = 0x0000;
			baseTable  [i | 0x100] = 0x8000;
			shiftTable [i | 0x000] = 24;
			shiftTable [i | 0x100] = 24;
		}
		else if ( e < -14 )		// Small numbers map to denorms
		{
			baseTable  [i | 0x000] = (0x0400 >> (-e-14));
			baseTable  [i | 0x100] = (0x0400 >> (-e-14)) | 0x8000;
			shiftTable [i | 0x000] = -e-1;
			shiftTable [i | 0x100] = -e-1;
		}
		else if ( e <= 15 )		// Normal numbers just lose precision
		{
			baseTable  [i |0x000] = ((e+15) << 10);
			baseTable  [i |0x100] = ((e+15) << 10) | 0x8000;
			shiftTable [i |0x000] = 13;
			shiftTable [i |0x100] = 13;
		}
		else if ( e < 128 )		// Large numbers map to Infinity
		{
			baseTable  [i | 0x000] = 0x7C00;
			baseTable  [i | 0x100] = 0xFC00;
			shiftTable [i | 0x000] = 24;
			shiftTable [i | 0x100] = 24;
		}
		else					// Infinity and NaN's stay Infinity and NaN's
		{
			baseTable  [i | 0x000] = 0x7C00;
			baseTable  [i | 0x100] = 0xFC00;
			shiftTable [i | 0x000] = 13;
			shiftTable [i | 0x100] = 13;
		}
	}
}	

void halfToFloat ( const uint16_t * half, uint32_t * f, int count )
{
	initTables ();
	
	for ( int i = 0; i < count; i++ )
	{
		uint16_t	h = half [i];
		
		f [i] = mantTable[offsTable[h >> 10]+(h & 0x3ff)] + expTable[h>>10];
	}
}

void	floatToHalf ( const uint32_t * ft, uint16_t * half, int count )
{
	initTables ();
	
	for ( int i = 0; i < count; i++ )
	{
		uint32_t f = ft [i];
		
		half [i] = baseTable[(f>>23)&0x1ff]+((f&0x007fffff)>>shiftTable[(f>>23)&0x1ff]);
	}
}

/*
uint16_t half_from_float ( uint32_t f )
{
  const uint32_t one                        = 0x00000001;
  const uint32_t f_s_mask                   = 0x80000000;
  const uint32_t f_e_mask                   = 0x7f800000;
  const uint32_t f_m_mask                   = 0x007fffff;
  const uint32_t f_m_hidden_bit             = 0x00800000;
  const uint32_t f_m_round_bit              = 0x00001000;
  const uint32_t f_snan_mask                = 0x7fc00000;
  const uint32_t f_e_pos                    = 0x00000017;
  const uint32_t h_e_pos                    = 0x0000000a;
  const uint32_t h_e_mask                   = 0x00007c00;
  const uint32_t h_snan_mask                = 0x00007e00;
  const uint32_t h_e_mask_value             = 0x0000001f;
  const uint32_t f_h_s_pos_offset           = 0x00000010;
  const uint32_t f_h_bias_offset            = 0x00000070;
  const uint32_t f_h_m_pos_offset           = 0x0000000d;
  const uint32_t h_nan_min                  = 0x00007c01;
  const uint32_t f_h_e_biased_flag          = 0x0000008f;
  const uint32_t f_s                        = f          & f_s_mask;
  const uint32_t f_e                        = f          & f_e_mask;
  const uint16_t h_s                        = f_s        >> f_h_s_pos_offset;
  const uint32_t f_m                        = f          & f_m_mask;
  const uint16_t f_e_amount                 = f_e        >> f_e_pos;
  const uint32_t f_e_half_bias              = f_e_amount - f_h_bias_offset;
  const uint32_t f_snan                     = f          & f_snan_mask;
  const uint32_t f_m_round_mask             = f_m & f_m_round_bit;
  const uint32_t f_m_round_offset           = f_m_round_mask << one;
  const uint32_t f_m_rounded                = f_m + f_m_round_offset;
  const uint32_t f_m_denorm_sa              = one - f_e_half_bias;
  const uint32_t f_m_with_hidden            = f_m_rounded | f_m_hidden_bit;
  const uint32_t f_m_denorm                 = f_m_with_hidden >> f_m_denorm_sa;
  const uint32_t h_m_denorm                 = f_m_denorm      >> f_h_m_pos_offset;
  const uint32_t f_m_rounded_overflow       = f_m_rounded & f_m_hidden_bit;
  const uint32_t m_nan                      = f_m >> f_h_m_pos_offset;
  const uint32_t h_em_nan                   = h_e_mask | m_nan;
  const uint32_t h_e_norm_overflow_offset   = f_e_half_bias + 1;
  const uint32_t h_e_norm_overflow          = h_e_norm_overflow_offset << h_e_pos;
  const uint32_t h_e_norm                   = f_e_half_bias << h_e_pos;
  const uint32_t h_m_norm                   = f_m_rounded >> f_h_m_pos_offset;
  const uint32_t h_em_norm                  = h_e_norm | h_m_norm;
  const uint32_t is_h_ndenorm_msb           = f_h_bias_offset   - f_e_amount;
  const uint32_t is_f_e_flagged_msb         = f_h_e_biased_flag - f_e_half_bias;
  const uint32_t is_h_denorm_msb            = ~is_h_ndenorm_msb;
  const uint32_t is_f_m_eqz_msb             = f_m   - 1
  const uint32_t is_h_nan_eqz_msb           = m_nan - 1;
  const uint32_t is_f_inf_msb               = is_f_e_flagged_msb & is_f_m_eqz_msb;
  const uint32_t is_f_nan_underflow_msb     = is_f_e_flagged_msb & is_h_nan_eqz_msb;
  const uint32_t is_e_overflow_msb          = h_e_mask_value - f_e_half_bias;
  const uint32_t is_h_inf_msb               = is_e_overflow_msb | is_f_inf_msb;
  const uint32_t is_f_nsnan_msb             = f_snan - f_snan_mask;
  const uint32_t is_m_norm_overflow_msb     = -f_m_rounded_overflow;
  const uint32_t is_f_snan_msb              = ~is_f_nsnan_msb;
  const uint32_t h_em_overflow_result       = _uint32_sels( is_m_norm_overflow_msb, h_e_norm_overflow, h_em_norm                 );
  const uint32_t h_em_nan_result            = _uint32_sels( is_f_e_flagged_msb,     h_em_nan,          h_em_overflow_result      );
  const uint32_t h_em_nan_underflow_result  = _uint32_sels( is_f_nan_underflow_msb, h_nan_min,         h_em_nan_result           );
  const uint32_t h_em_inf_result            = _uint32_sels( is_h_inf_msb,           h_e_mask,          h_em_nan_underflow_result );
  const uint32_t h_em_denorm_result         = _uint32_sels( is_h_denorm_msb,        h_m_denorm,        h_em_inf_result           );
  const uint32_t h_em_snan_result           = _uint32_sels( is_f_snan_msb,          h_snan_mask,       h_em_denorm_result        );
  const uint32_t h_result                   = h_s | h_em_snan_result;

  return (uint16_t)(h_result);
}

uint32_t half_to_float ( uint16_t h )
{
  const uint32_t h_e_mask              = 0x00007c00;
  const uint32_t h_m_mask              = 0x000003ff;
  const uint32_t h_s_mask              = 0x00008000;
  const uint32_t h_f_s_pos_offset      = 0x00000010;
  const uint32_t h_f_e_pos_offset      = 0x0000000d;
  const uint32_t h_f_bias_offset       = 0x0001c000;
  const uint32_t f_e_mask              = 0x7f800000;
  const uint32_t f_m_mask              = 0x007fffff;
  const uint32_t h_f_e_denorm_bias     = 0x0000007e;
  const uint32_t h_f_m_denorm_sa_bias  = 0x00000008;
  const uint32_t f_e_pos               = 0x00000017;
  const uint32_t h_e_mask_minus_one    = 0x00007bff;
  const uint32_t h_e                   = h & h_e_mask;
  const uint32_t h_m                   = h & h_m_mask;
  const uint32_t h_s                   = h & h_s_mask;
  const uint32_t h_e_f_bias            = h_e + h_f_bias_offset;
  const uint32_t h_m_nlz               = _uint32_cntlz( h_m );
  const uint32_t f_s                   = h_s << h_f_s_pos_offset;
  const uint32_t f_e                   = h_e_f_bias << h_f_e_pos_offset;
  const uint32_t f_m                   = h_m << h_f_e_pos_offset;
  const uint32_t f_em                  = f_e | f_m;
  const uint32_t h_f_m_sa              = h_m_nlz - h_f_m_denorm_sa_bias;
  const uint32_t f_e_denorm_unpacked   = h_f_e_denorm_bias - h_f_m_sa;
  const uint32_t h_f_m                 = h_m << h_f_m_sa;
  const uint32_t f_m_denorm            = h_f_m & f_m_mask;
  const uint32_t f_e_denorm            = f_e_denorm_unpacked << f_e_pos;
  const uint32_t f_em_denorm           = f_e_denorm | f_m_denorm;
  const uint32_t f_em_nan              = f_e_mask   | f_m;
  const uint32_t is_e_eqz_msb          = h_e - 1;
  const uint32_t is_m_nez_msb          = -h_m;
  const uint32_t is_e_flagged_msb      = h_e_mask_minus_one - h_e;
  const uint32_t is_zero_msb           = is_e_eqz_msb & ~is_m_nez_msb;
  const uint32_t is_inf_msb            = is_e_flagged_msb & ~is_m_nez_msb;
  const uint32_t is_denorm_msb         = is_m_nez_msb     & is_e_eqz_msb;
  const uint32_t is_nan_msb            = is_e_flagged_msb & is_m_nez_msb; 
  const uint32_t is_zero               = _uint32_ext(  is_zero_msb );
  const uint32_t f_zero_result         = f_em & ~is_zero;
  const uint32_t f_denorm_result       = _uint32_sels( is_denorm_msb, f_em_denorm, f_zero_result );
  const uint32_t f_inf_result          = _uint32_sels( is_inf_msb,    f_e_mask,    f_denorm_result );
  const uint32_t f_nan_result          = _uint32_sels( is_nan_msb,    f_em_nan,    f_inf_result    );
  const uint32_t f_result              = f_s | f_nan_result;
 
  return (f_result);
}
*/
