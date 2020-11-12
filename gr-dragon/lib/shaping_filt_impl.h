/* -*- c++ -*- */
/*
 * Copyright 2020 gr-dragon author.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_DRAGON_SHAPING_FILT_IMPL_H
#define INCLUDED_DRAGON_SHAPING_FILT_IMPL_H

#include <dragon/shaping_filt.h>

namespace gr {
  namespace dragon {

    class shaping_filt_impl : public shaping_filt
    {
     private:
      std::string d_filt_type;
      int d_filt_len;
      int d_vlen;

     public:
      shaping_filt_impl(std::string filt_type, int filt_len, int vlen);
      ~shaping_filt_impl();

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_SHAPING_FILT_IMPL_H */

