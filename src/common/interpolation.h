/* --------------------------------------------------------------------------
    This file is part of darktable,
    copyright (c) 2012 Edouard Gomez <ed.gomez@free.fr>

    darktable is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    darktable is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with darktable.  If not, see <http://www.gnu.org/licenses/>.
* ------------------------------------------------------------------------*/

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <xmmintrin.h>

/** Available interpolations */
enum dt_interpolation_type
{
  DT_INTERPOLATION_FIRST=0, /**< Helper for easy iteration on interpolators */
  DT_INTERPOLATION_BILINEAR=DT_INTERPOLATION_FIRST, /**< Bilinear interpolation (aka tent filter) */
  DT_INTERPOLATION_BICUBIC, /**< Bicubic interpolation (with -0.5 parameter) */
  DT_INTERPOLATION_LANCZOS2, /**< Lanczos interpolation (with 2 lobes) */
  DT_INTERPOLATION_LANCZOS3, /**< Lanczos interpolation (with 3 lobes) */
  DT_INTERPOLATION_LAST, /**< Helper for easy iteration on interpolators */
  DT_INTERPOLATION_DEFAULT=DT_INTERPOLATION_BILINEAR,
  DT_INTERPOLATION_USERPREF /**< can be specified so that user setting is chosen */
};

/** Interpolation function */
typedef float (*dt_interpolation_func)(float width, float t);

/** Interpolation function (SSE) */
typedef __m128 (*dt_interpolation_sse_func)(__m128 width, __m128 t);

/** Interpolation structure */
struct dt_interpolation
{
  enum dt_interpolation_type id; /**< Id such as defined by the dt_interpolation_type */
  const char* name; /**< internal name  */
  int width; /**< Half width of its kernel support */
  dt_interpolation_func func; /**< Kernel function */
  dt_interpolation_sse_func funcsse; /**< Kernel function (four params a time) */
};

/** Compute a single interpolated sample.
 *
 * This function computes a single interpolated sample. Implied costs are:
 * <ul>
 * <li>Horizontal filtering kernel computation</li>
 * <li>Vertical  filtering kernel computation</li>
 * <li>Sample computation</li>
 * </ul>
 *
 * @param in Input samples
 * @param itor interpolator to be used
 * @param x X-Coordinate of the requested sample
 * @param y Y-Coordinate of the requested sample
 * @param samplestride Stride in bytes for a sample
 * @param linestride Stride in bytes for complete line
 *
 * @return computed sample
 */
float
dt_interpolation_compute_sample(
  const struct dt_interpolation* itor,
  const float* in,
  const float x,
  const float y,
  const int samplestride,
  const int linestride);

/** Get an interpolator from type
 * @param type Interpolator to search for
 * @return requested interpolator or default if not found (this function can't fail)
 */
const struct dt_interpolation*
dt_interpolation_new(
  enum dt_interpolation_type type);

#endif /* INTERPOLATION_H */

