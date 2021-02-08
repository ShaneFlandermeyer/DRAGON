/* -*- c++ -*- */

#define DRAGON_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "dragon_swig_doc.i"

%{
#include "dragon/math/difference.h"
#include "dragon/math/dti.h"
#include "dragon/vector-utils/oversample_vector.h"
#include "dragon/math/complex_exponential.h"
#include "dragon/waveform/cpm.h"
#include "dragon/waveform/pulse_shaper.h"
#include "dragon/waveform/pcfm_mod.h"
#include "dragon/waveform/phase_code.h"
#include "dragon/waveform/phase_code_generator.h"
#include "dragon/radar/moving_target_sim.h"
#include "dragon/waveform/pdu_phase_code_generator.h"
#include "dragon/constants.h"
#include "dragon/waveform/pdu_lfm.h"
#include "dragon/math/db.h"
#include "dragon/pdu-utils/pdu_to_stream.h"
#include "dragon/math/convolve.h"
#include "dragon/vector-utils/pad_vector.h"
#include "dragon/vector-utils/vector_functions.h"
#include "dragon/math/convolve_fft_hier.h"
#include "dragon/cl_db.h"
#include "dragon/lfm_source.h"
%}
%include "dragon/math/difference.h"
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, difference_cc, difference<gr_complex>);
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, difference_ff, difference<float>);

%include "dragon/math/dti.h"
GR_SWIG_BLOCK_MAGIC2(dragon, dti);
%include "dragon/vector-utils/oversample_vector.h"
GR_SWIG_BLOCK_MAGIC2(dragon, oversample_vector);
%include "dragon/math/complex_exponential.h"
GR_SWIG_BLOCK_MAGIC2(dragon, complex_exponential);


%include "dragon/waveform/cpm.h"
%include "dragon/waveform/pulse_shaper.h"
GR_SWIG_BLOCK_MAGIC2(dragon, pulse_shaper);

%include "dragon/waveform/pcfm_mod.h"
GR_SWIG_BLOCK_MAGIC2(dragon, pcfm_mod);
%include "dragon/waveform/phase_code.h"

%include "dragon/waveform/phase_code_generator.h"
GR_SWIG_BLOCK_MAGIC2(dragon, phase_code_generator);
%include "dragon/radar/moving_target_sim.h"
GR_SWIG_BLOCK_MAGIC2(dragon, moving_target_sim);
%include "dragon/waveform/pdu_phase_code_generator.h"
GR_SWIG_BLOCK_MAGIC2(dragon, pdu_phase_code_generator);

%include "dragon/constants.h"

%include "dragon/waveform/pdu_lfm.h"
GR_SWIG_BLOCK_MAGIC2(dragon, pdu_lfm);


%include "dragon/math/db.h"
GR_SWIG_BLOCK_MAGIC2(dragon, db);

%include "dragon/pdu-utils/pdu_to_stream.h"
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, pdu_to_stream_b, pdu_to_stream<unsigned char>);
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, pdu_to_stream_s, pdu_to_stream<short>);
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, pdu_to_stream_f, pdu_to_stream<float>);
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, pdu_to_stream_c, pdu_to_stream<gr_complex>);
%include "dragon/math/convolve.h"
GR_SWIG_BLOCK_MAGIC2(dragon, convolve);
%include "dragon/vector-utils/pad_vector.h"
GR_SWIG_BLOCK_MAGIC2(dragon, pad_vector);
%include "dragon/vector-utils/vector_functions.h"
%include "dragon/math/convolve_fft_hier.h"
GR_SWIG_BLOCK_MAGIC2(dragon, convolve_fft_hier);
%include "dragon/cl_db.h"
GR_SWIG_BLOCK_MAGIC2(dragon, cl_db);
%include "dragon/lfm_source.h"
GR_SWIG_BLOCK_MAGIC2(dragon, lfm_source);
