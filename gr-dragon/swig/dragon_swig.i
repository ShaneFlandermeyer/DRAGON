/* -*- c++ -*- */

#define DRAGON_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "dragon_swig_doc.i"

%{
#include "dragon/difference.h"
#include "dragon/dti.h"
#include "dragon/oversample_vector.h"
#include "dragon/complex_exponential.h"
#include "dragon/cpm.h"
#include "dragon/pulse_shaper.h"
#include "dragon/pcfm_mod.h"
#include "dragon/phase_code.h"
#include "dragon/phase_code_generator.h"
#include "dragon/moving_target_sim.h"
#include "dragon/pdu_phase_code_generator.h"
#include "dragon/lfm_pdu.h"
#include "dragon/pdu_constants.h"
#include "dragon/pdu_to_stream.h"
%}
%include "dragon/difference.h"
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, difference_cc, difference<gr_complex>);
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, difference_ff, difference<float>);

%include "dragon/dti.h"
GR_SWIG_BLOCK_MAGIC2(dragon, dti);
%include "dragon/oversample_vector.h"
GR_SWIG_BLOCK_MAGIC2(dragon, oversample_vector);
%include "dragon/complex_exponential.h"
GR_SWIG_BLOCK_MAGIC2(dragon, complex_exponential);


%include "dragon/cpm.h"
%include "dragon/pulse_shaper.h"
GR_SWIG_BLOCK_MAGIC2(dragon, pulse_shaper);

%include "dragon/pcfm_mod.h"
GR_SWIG_BLOCK_MAGIC2(dragon, pcfm_mod);
%include "dragon/phase_code.h"

%include "dragon/phase_code_generator.h"
GR_SWIG_BLOCK_MAGIC2(dragon, phase_code_generator);
%include "dragon/moving_target_sim.h"
GR_SWIG_BLOCK_MAGIC2(dragon, moving_target_sim);
%include "dragon/pdu_phase_code_generator.h"
GR_SWIG_BLOCK_MAGIC2(dragon, pdu_phase_code_generator);

%include "dragon/lfm_pdu.h"
GR_SWIG_BLOCK_MAGIC2(dragon, lfm_pdu);


%include "dragon/pdu_constants.h"

// pdu_to_bursts
%include "dragon/pdu_to_stream.h"
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, pdu_to_stream_b, pdu_to_stream<unsigned char>);
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, pdu_to_stream_s, pdu_to_stream<short>);
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, pdu_to_stream_f, pdu_to_stream<float>);
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, pdu_to_stream_c, pdu_to_stream<gr_complex>);
//
//%include "dragon/pdu_to_stream.h"
//GR_SWIG_BLOCK_MAGIC2(dragon, pdu_to_stream);
