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

#ifndef INCLUDED_DRAGON_PHASE_CODE_H
#define INCLUDED_DRAGON_PHASE_CODE_H

#include "../api.h"
#include <vector>

namespace gr {
namespace dragon {

/*!
 * \brief Return the phase values for a phase code of a given type and length
 *
 */
class DRAGON_API phase_code {
 public:
  enum code_type {P4, BARKER};

  /*!
   * \brief Return the phase values for a phase code
   *
   * \details
   *
   * @param type
   * @param code_len
   * @return
   */
  static std::vector<float> get_code(code_type type, unsigned code_len);


};

} // namespace dragon
} // namespace gr

#endif /* INCLUDED_DRAGON_PHASE_CODE_H */
