/* -*- c++ -*- */

#define DRAGON_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "dragon_swig_doc.i"

%{
#include "dragon/difference.h"
#include "dragon/phase_code_generator.h"
#include "dragon/dti.h"
#include "dragon/oversample_vector.h"
#include "dragon/complex_exponential.h"
#include "dragon/shaping_filt.h"
#include "dragon/cpm.h"
#include "dragon/pulse_shaper.h"
%}

%include "dragon/difference.h"
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, difference_cc, difference<gr_complex>);
GR_SWIG_BLOCK_MAGIC2_TMPL(dragon, difference_ff, difference<float>);
%include "dragon/phase_code_generator.h"
GR_SWIG_BLOCK_MAGIC2(dragon, phase_code_generator);
%include "dragon/dti.h"
GR_SWIG_BLOCK_MAGIC2(dragon, dti);
%include "dragon/oversample_vector.h"
GR_SWIG_BLOCK_MAGIC2(dragon, oversample_vector);
%include "dragon/complex_exponential.h"
GR_SWIG_BLOCK_MAGIC2(dragon, complex_exponential);
%include "dragon/shaping_filt.h"
GR_SWIG_BLOCK_MAGIC2(dragon, shaping_filt);

%include "dragon/cpm.h"
%include "dragon/pulse_shaper.h"
GR_SWIG_BLOCK_MAGIC2(dragon, pulse_shaper);
