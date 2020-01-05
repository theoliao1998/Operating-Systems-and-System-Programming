/*
 * Copyright © 2019 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SPOOLER_TWO_CV_H
#define SPOOLER_TWO_CV_H

#include "spooler.h"

/*
 * Function to initialize any fields added in PART 4. You SHOULD NOT
 * initialize anything you added in PART 1, PART 2, or PART 3.
 */
void spool_init_cv2(spool_t* sp);

/*
 * Function used to implement insertion in a thread safe manner. It
 * should use spool_insert_unchecked once it confirms there is room
 * in the spooler. This function should always insert the item.
 */
void spool_insert_cv2(spool_t* sp, char* item);

/*
 * Function used to implement removal in a thread safe manner. It
 * should use spool_remove_unchecked once it confirms there is data
 * in the spooler. This function should only return NULL if a shutdown
 * has been issued and otherwise must return an element.
 */
char* spool_remove_cv2(spool_t* sp);

#endif

