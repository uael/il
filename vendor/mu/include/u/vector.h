/*
 * MIT License
 *
 * Copyright (c) 2017 Lucas Abel <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!\file vector.h
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_VECTOR_H__
# define U_VECTOR_H__

#include "buffer.h"

#define UVEC_MIN_CAPACITY 4

#define vec_of(T) struct { \
    ds_super(T); \
  }

/*!\def   vec_growth
 * \brief Reserve storage for minimum given number of elements.
 *        Never reduces storage and does not affect number of
 *        currently stored elements.
 * \param vector Vector structure
 * \param nmin   Minimum given number of elements
 */
#define vec_growth(vector, nmin) \
  ds_growth(vector, nmin, sizeof(*ds_data(vector)))

/*!\def   vec_decay
 * \brief Reserve storage for maximum given number of elements.
 *        Reduces storage and affect number of
 *        currently stored elements.
 * \param vector Vector structure
 * \param nmin   Maximum given number of elements
 */
#define vec_decay(vector, nmax) \
  ds_decay(vector, nmax, sizeof(*ds_data(vector)))

/*!\def   vec_grow
 * \brief Add elements without initialization.
 *        Allocates new storage if new size is larger than
 *        vector cap. Value of expression is the expected size of the vector.
 * \param vector Vector structure
 * \param nmemb  Number of elements to grow
 */
#define vec_grow(vector, nmemb) \
  ds_grow(vector, (nmemb), sizeof(*ds_data(vector)))

/*!\def   vec_shrink
 * \brief Remove elements without initialization.
 *        Reallocates storage if new size is lower than
 *        vector cap. Value of expression is the expected size of the vector.
 *        Reduces storage and affect number of
 *        currently stored elements.
 * \param vector Vector structure
 * \param nmemb  Number of elements to grow
 */
#define vec_shrink(vector, nmemb) \
  ds_shrink(vector, (nmemb), sizeof(*ds_data(vector)))

/*!\def   vec_dtor
 * \brief Deallocate vector memory and reset vector pointer to zero. Value of expression
 *        is a null pointer.
 * \param vector Vector structure
 */
#define vec_dtor(v) \
    ((ds_size(v) = ds_cap(v) = 0), \
    (ds_data(v) ? free(ds_data(v)) : (void) 0), \
    ds_data(v) = nullptr)

/*!\def   vec_resize
 * \brief Resize to given absolute size without initialization. Sets new size to num and allocate
 *        new storage if new size is larger than vector cap.
 * \param vector Vector structure
 * \param num    New size
 */
#define vec_resize(vector, num) \
  (ds_size(vector) = vec_growth(vector, num))

/*!\def   vec_copy
 * \brief Copy content of one vector to another, setting new destination vector size to source vector
 *        size and allocating more storage if new destination size is larger than destination vector
 *        cap. Value of the expression is the size of the destination vector after copy.
 * \param dst Destination vector
 * \param src Source vector
 */
#define vec_copy(dst, src) \
  ( \
    ( \
      sizeof(*ds_data(src)) == sizeof(*ds_data(dst)) \
    ) ? ( \
      buf_reserve(dst, ds_size(src)), \
      memcpy(ds_data(dst), ds_data(src), (ds_size(src)) * sizeof(*ds_data(src))), \
      (ds_size(dst) = ds_size(src)) \
    ) : \
      vec_clear(dst) \
  )

/*!\def   vec_erase
 * \brief Erase element at given position by preserving order, replace given position by its next.
 *        Position is NOT ranged checked.
 * \param vector Vector structure
 * \param pos    Position
 */
#define vec_erase(vector, pos) \
  (memmove( \
    ds_data(vector) + (pos), ds_data(vector) + (pos) + 1, --ds_size(vector) * sizeof(*ds_data(vector)) \
  ), ds_size(vector))

/*!\def   vec_erase_safe
 * \brief Erase element at given position without preserving order, swap-with-last using assignment.
 *        Position IS ranged checked.
 * \param vector Vector structure
 * \param pos    Position
 */
#define vec_erase_safe(vector, pos) \
  ((pos) < ds_size(vector) ? \
    vec_erase(vector, pos) : \
    ds_size(vector))

/*!\def   vec_erase_range
 * \brief Erase a range of elements at given position and preserve order by memmove remaining elements
 *        in vector. Position and number of elements are NOT ranged checked
 * \param vector Vector structure
 * \param pos    Starting position
 * \param num    Number of elements to erase
 */
#define vec_erase_range(vector, pos, num) \
  (memmove(ds_data(vector) + (pos), ds_data(vector) + (pos) + (num), \
    (ds_size(vector) -= (num)) * sizeof(*ds_data(vector))), \
  ds_size(vector))

/*!\def   vec_erase_range_safe
 * \brief Erase a range of elements at given position and preserve order by memmove remaining elements
 *        in vector. Position and number of elements are ranged checked
 * \param vector Vector structure
 * \param pos    Starting position
 * \param num    Number of elements to erase
 */
#define vec_erase_range_safe(vector, pos, num) \
  ((pos) + (num) < ds_size(vector) ? \
    vec_erase_range(vector, pos, num) : \
    ds_size(vector))

/*!\def   vec_push
 * \brief Add element at end of vector with assignment. Value of expression is new vector pointer.
 * \param vector   Vector structure
 * \param element  New element
 */
#define vec_push(v, x) \
  (vec_grow(v, 1), ds_data(v)[ds_size(v)++] = (x))

/*!\def   vec_push_memcpy
 * \brief Add element at end of vector copying data with memcpy. Value of expression
 *        is new vector pointer.
 * \param vector      Vector structure
 * \param element_ptr Pointer to new element
 */
#define vec_push_memcpy(vector, element_ptr) \
  (vec_grow(vector, 1), memcpy(ds_data(vector) + ds_size(vector)++, (element_ptr), sizeof(*ds_data(vector))))

/*!\def   vec_insert
 * \brief Add element at given position in vector with assignment. Position is NOT range checked.
 *        Existing elements are moved using memmove. Value of expression is new vector pointer.
 * \param vector  Vector structure
 * \param pos     Position
 * \param element New element
 */
#define vec_insert(vector, pos, element) \
  (vec_grow(vector, 1), \
  memmove(ds_data(vector) + (pos) + 1, ds_data(vector) + (pos), \
    sizeof(*ds_data(vector)) * (ds_size(vector)++ - (pos))), \
  ds_at(vector, (pos)) = (element))

/*!\def   vec_insert_memcpy
 * \brief Add element at given position in vector, copy data using memcpy. Position is NOT range
 *        checked.
 *        Existing elements are moved using memmove. Value of expression is new vector pointer.
 * \param vector      Vector structure
 * \param pos         Position
 * \param element_ptr Pointer to new element
 */
#define vec_insert_memcpy(vector, pos, element_ptr) \
  (vec_grow(vector, 1), \
  memmove(ds_data(vector) + (pos) + 1, ds_data(vector) + (pos), \
    sizeof(*(vector)) * (ds_size(vector)++ - (pos))), \
  memcpy(ds_data(vector) + (pos), (element_ptr), sizeof(*ds_data(vector))))

/*!\def   vec_insert_safe
 * \brief Add element at given position in vector with assignment. Position is range checked.
 *        Existing elements are moved using memmove. Value of expression is new vector pointer.
 * \param vector  Vector structure
 * \param pos     Position
 * \param element New element
 */
#define vec_insert_safe(vector, pos, element) \
  ((pos) && (pos) < ds_size(vector) ? (vec_insert(vector, pos, element), 0) : 0)

/*!\def   vec_insert_safe_memcpy
 * \brief Add element at given position in vector, copy data using memcpy. Position is range
 *        checked.
 *        Existing elements are moved using memmove. Value of expression is new vector pointer.
 * \param vector      Vector structure
 * \param pos         Position
 * \param element_ptr Pointer to new element
 */
#define vec_insert_safe_memcpy(vector, pos, element_ptr) \
  ((pos) && (pos) < ds_size(vector) ? (vec_insert_memcpy(vector, pos, element_ptr), 0) : 0)

/*!\def   vec_pop
 * \brief Remove last element. Vector size is NOT validated, will cause undefined behaviour if
 *        called on an empty vector.
 * \param vector Vector structure
 */
#define vec_pop(v) ds_data(v)[--ds_size(v)]

/*!\def   vec_pop_safe
 * \brief Remove last element. Vector size IS validated, safe to call on an empty vector.
 * \param vector Vector structure
 */
#define vec_pop_safe(vector) \
  (ds_size(vector) > 0 ? --ds_size(vector) : 0)

#define vec_unshift(v, x) ( \
    vec_grow(v, 1), \
    memmove( \
      ds_data(v) + 1, \
      ds_data(v), \
      ds_size(v)++ * sizeof(*ds_data(v)) \
    ), \
    ds_data(v)[0] = (x) \
  )

#define vec_shift(v) \
  vec_erase(v, 0)

/*!\def   vec_clear
 * \brief Set size to 0. Does not affect cap or resize vector storage buffer.
 *        Reset all memory.
 * \param vector Vector structure
 */
#define vec_clear(v) ( \
    (ds_data(v) ? memset(ds_data(v), 0, ds_size(v)) : 0), \
    ds_size(v) = 0 \
  )

#endif /* U_VECTOR_H__ */
