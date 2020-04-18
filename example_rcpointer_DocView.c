/* file: example_rcpointer_DocView.c
 * Example of model-view-controller pattern using rcpointer.
 * Relationship of Doc:View is 1:Many, so the Doc object pointer is shared.
 *
 * Build:
 *   gcc -Wall -ggdb3 -O0 -o example_rcpointer_DocView `pkg-config --cflags --libs glib-2.0` example_rcpointer_DocView.c
 * Debug with valgrind:
 *   G_DEBUG=always-malloc valgrind --leak-check=full --track-origins=yes ./example_rcpointer_DocView |& tee valgrind.log
 *
 * Author; Thomas E. Hilinski <https://github.com/tehilinski>
 * Copyright (c) 2020 Thomas E. Hilinski. All rights reserved.
 * License: Apache License version 2.0
 *          URL: http://www.apache.org/licenses/LICENSE-2.0
 */

#include "rcpointer.h"
#include <stdio.h>

/*  --------------------------------------------------------------------------
 *  A document (aka model) contains the information for the viewer.
 */
typedef struct Doc
{
    gchar* fileName;
} Doc;

Doc* Doc_new ( gchar* const fileName )
{
    Doc* const doc = g_new0( Doc, 1 );
    if ( fileName )
        doc->fileName = g_strdup( fileName );
    else
        doc->fileName = g_strdup( "Untitled" );
    return doc;
}

Doc* Doc_delete ( Doc* doc )
{
    if ( doc )
    {
        if ( doc->fileName )
            g_free( doc->fileName );
        g_free( doc );
        doc = 0;
    }
    return doc;
}

void Doc_print ( Doc const * const doc )
{
    printf( " Doc: doc->fileName = %s\n", doc->fileName );
}

/*  --------------------------------------------------------------------------
 *  A viewer to display information from the document.
 */
typedef struct View
{
    rcpointer* doc;     /* shared Doc* */
    gpointer widget;    /* widget to display doc */
} View;

View* View_new ( rcpointer* const doc )
{
    View* view = g_new0( View, 1 );
    view->doc = rcp_copy( doc );
    /* todo: need a widget-maker function */
    view->widget = 0;
    return view;
}

View* View_delete( View* view )
{
    if ( view )
    {
        rcp_delete( view->doc );
        if ( view->widget )
            g_free( view->widget );
        g_free( view );
        view = 0;
    }
    return view;
}

void View_print ( View const * const view )
{
    printf( "View: doc->fileName = %s\n",
	    ( (Doc*) rcp_get( view->doc ) )->fileName );
}

/*  --------------------------------------------------------------------------
 *  Associate a document with a viewer.
 */
typedef struct DocView
{
    rcpointer* doc;     /* shared Doc* */
    View*      view;    /* pointer is owed here */
} DocView;

DocView* DocView_new (
    rcpointer* const doc,
    View* const      view)
{
    DocView* docview = g_new0( DocView, 1 );
    docview->doc  = rcp_copy( doc );
    docview->view = view;
    return docview;
}

DocView* DocView_delete ( DocView* docview )
{
    docview->doc  = rcp_delete( docview->doc );
    docview->view = View_delete( docview->view );
    g_free( docview );
    docview = 0;
    return docview;
}

void DocView_print ( DocView const * const docview )
{
    Doc_print( rcp_get( docview->doc ) );
    View_print( docview->view );
}

/*  --------------------------------------------------------------------------
 *  main is the controller
 */
int main ()
{
    /* container of DocView that knows how to delete elements */
    GPtrArray* arrayDV = g_ptr_array_new_with_free_func(
                            (GDestroyNotify) DocView_delete );

    /* shared document */
    rcpointer* doc = rcp_new( Doc_new( "Everything_Explained.txt" ),
                              Doc_delete );

    /* make some DocView associations and add to array */
    DocView* docview1 =  DocView_new( doc, View_new( doc ) );
    g_ptr_array_add( arrayDV, (gpointer) docview1 );
    DocView* docview2 =  DocView_new( doc, View_new( doc ) );
    g_ptr_array_add( arrayDV, (gpointer) docview2 );

    /* display each DocView */
    g_ptr_array_foreach( arrayDV, (GFunc) DocView_print, NULL );

    /* all done! delete the array contents and the array object */
    g_ptr_array_free( arrayDV, TRUE );
    rcp_delete( doc );

    return 0;
}
