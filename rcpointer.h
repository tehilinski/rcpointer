/** @file rcpointer.h
 *
 * Reference counted shared pointer.
 *
 * This design is not thread-safe.
 * Uses glib's gpointer type.
 * Thanks to many sources on the web and C++ std::shared_ptr for ideas.
 *
 * @author     Thomas E. Hilinski <https://github.com/tehilinski>
 * @copyright  {
 * Copyright (c) 2020 Thomas E. Hilinski. All rights reserved.
 * License: Apache License version 2.0
 *          URL: http://www.apache.org/licenses/LICENSE-2.0
 * }
 */

#ifndef INC_rcpointer_h
#define INC_rcpointer_h

#include <glib.h>
G_BEGIN_DECLS


typedef struct rcpointer
{
    gpointer data;			/* pointer to the owned object */
    gpointer ( *free ) ( gpointer );	/* destructor for the owned object */
    guint count;			/* ownership counter */
} rcpointer;


/** Get the raw pointer to the object owned by the rcpointer.
 *  @param ptr	The rcpointer object
 *  @return	Pointer to the owned object as a gpointer, or NULL
 */
static inline gpointer rcp_get( rcpointer* const ptr )
{
    if ( ptr )
	return ptr->data;
    else
	return NULL;
}

/** Copy the pointer
 *  @param ptr	The rcpointer object
 *  @return	A new rcpointer object referring to the original's owned object
 */
static inline rcpointer* rcp_copy( rcpointer* const ptr )
{
    if ( ptr )
	ptr->count++;
    return ptr;
}

/** Allocate a new pointer for this object
 *  @param
 *  @param
 *  @return
 */
static inline rcpointer* rcp_new(
    gpointer obj,			/**< obj is owned by returned rcpointer in data */
    gpointer destructor )		/**< function to free obj; argument and return = obj pointer */
{
    rcpointer* rcp = g_new0( rcpointer, 1 );
    rcp->data = obj;
    rcp->free = destructor;
    rcp->count = 1;
    return rcp;
}

/** Delete the copy of the pointer or  destroy the object if reference count is zero
 *  @param	The rcpointer object
 *  @return
 */
static inline rcpointer* rcp_delete( rcpointer* ptr )
{
    if ( ptr )
    {
	if ( --(ptr->count) == 0 )
	{
	    ptr->data = ptr->free( ptr->data );
	    g_free( ptr );
	    ptr = 0;
	}
    }
    return ptr;
}


G_END_DECLS
#endif /* INC_rcpointer_h */
