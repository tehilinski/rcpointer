/* file: example_rcpointer.c
 * test of rcpointer
 *
 * Build:
 *   gcc -Wall -ggdb3 -O0 -o example_rcpointer `pkg-config --cflags --libs glib-2.0` example_rcpointer.c
 * Debug with valgrind:
 *   G_DEBUG=always-malloc valgrind --leak-check=full --track-origins=yes ./example_rcpointer |& tee valgrind.log
 *
 * Author; Thomas E. Hilinski <tom.hilinski@gmail.com>
 * Copyright (c) 2020 Thomas E. Hilinski. All rights reserved.
 * License: Apache License version 2.0
 *          URL: http://www.apache.org/licenses/LICENSE-2.0
 */

#include "rcpointer.h"
#include <stdio.h>

typedef struct Document
{
    gchar*   fileName;
    gboolean changed;

} Document;

Document* Document_constructor (
    gchar const * const docFilename)
{
    Document* const pGD = g_new0( Document, 1 );
    pGD->fileName = g_strdup( (gchar*) docFilename );
    pGD->changed  = FALSE;
    return pGD;
}

Document* Document_destructor ( Document* pGD )
{
    if ( pGD )
    {
	g_free( pGD->fileName );	pGD->fileName = 0;
	g_free( pGD );			pGD = 0;
    }
    return pGD;
}

void Document_display ( Document const * const pGD )
{
    if ( pGD )
    {
	printf( "document file name: %s\n   modified? %s\n",
		pGD->fileName,
		( pGD->changed ? "yes" : "no" ) );
    }
}


void DisplayPtr ( rcpointer const * const p )
{
    printf( "pointer count = %d\n",  (p ? p->count : 0) );
}

int main ()
{
    printf( "\nTest of rcpointer\n" );

    printf( "--- make initial pointer:\n" );
    rcpointer* ptr1 = rcp_new(
			Document_constructor("doc1.txt"),
			Document_destructor );
    DisplayPtr( ptr1 );
    Document_display( rcp_get( ptr1 ) );

    printf( "--- copy pointer:\n" );
    rcpointer* ptr2 = rcp_copy( ptr1 );
    DisplayPtr( ptr2 );
    Document_display( rcp_get( ptr2 ) );

    printf( "--- delete pointer 1:\n" );
    ptr1 = rcp_delete( ptr1 );
    DisplayPtr( ptr1 );
    Document_display( rcp_get( ptr1 ) );

    printf( "--- delete pointer 2:\n" );
    ptr2 = rcp_delete( ptr2 );
    DisplayPtr( ptr2 );
    if ( ptr2 )
	Document_display( rcp_get( ptr2 ) );

    printf( "\n   all done!\n\n" );
    return 0;
}
