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

/*!\file ds.h
 * \author Lucas Abel <www.github.com/uael>
 */
#ifndef  U_DS_H__
# define U_DS_H__

#include "types.h"

#ifndef DS_MIN_CAP
# define DS_MIN_CAP 4
#endif

/*!\def ds_data
 * \param ds Data Structure
 */
#define ds_super(T) \
  size_t cap, size; \
  T *data

typedef struct ds ds_t;

struct ds {
  ds_super(void);
};

/*!\def   ds_cap
 * \brief Get cap of data structure in number of elements. Capacity indicates the size of the allocated
 *        memory block (maximum size of data structure).
 * \param ds Data Structure
 */
#define ds_cap(ds) (ds).cap

/*!\def   ds_size
 * \brief Get number of currently stored elements.
 * \param ds Data Structure
 */
#define ds_size(ds) (ds).size

/*!\def ds_data
 * \param ds Data Structure
 */
#define ds_data(ds) (ds).data

/*!\def ds_at
 * \param ds    Data Structure
 * \param index Index
 */
#define ds_at(ds, index) ds_data(ds)[index]

/*!\def ds_front
 * \param ds Data Structure
 */
#define ds_front(ds) ds_at(ds, 0)

/*!\def ds_empty
 * \param ds Data Structure
 */
#define ds_empty(ds) (ds_size(ds) == 0)

/*!\def ds_back
 * \param ds Data Structure
 */
#define ds_back(ds) ds_at(ds, ds_size(ds) - 1)

/*!\def   ds_growth
 * \brief Reserve storage for minimum given number of elements.
 *        Never reduces storage and does not affect number of
 *        currently stored elements.
 * \param ds    Data structure
 * \param nmin  Minimum given number of elements
 * \param isize Item size
 */
#define ds_growth(ds, nmin, isize) \
  ds_pgrowth((ds_t *) &(ds), (nmin), (isize))

/*!\def   ds_decay
 * \brief Reserve storage for maximum given number of elements.
 *        Reduces storage and affect number of
 *        currently stored elements.
 * \param ds    Data structure
 * \param nmin  Maximum given number of elements
 * \param isize Item size
 */
#define ds_decay(ds, nmax, isize) \
  ds_pdecay((ds_t *) &(ds), (nmax), (isize))

#define ds_grow(ds, n, isize) \
  ds_growth((ds), ds_size(ds) + (n), (isize))

#define ds_shrink(ds, n, isize) \
  ds_decay((ds), ds_size(ds) - (n), (isize))

#define ds_trim(ds, isize) \
  ds_decay((ds), ds_size(ds))

#define foreach(ds, it_name) \
  for (it_name = 0; (it_name) < ds_size(ds); ++(it_name))

U_API size_t ds_pgrowth(ds_t *self, const ssize_t nmin, const size_t isize);
U_API size_t ds_pdecay(ds_t *self, const ssize_t nmax, const size_t isize);

#include "deque.h"
#include "hash.h"
#include "list.h"
#include "stack.h"
#include "vector.h"

#endif /* U_DS_H__ */
