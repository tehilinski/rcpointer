# rcpointer
This implements a reference-counted shared pointer for C using glib types.

Wraps a C pointer in a struct with reference counting.
Requires a destructor function for the pointer's object.

Glib types are used for convenience.

See the example C files for usage.

This was inspired by a need to share an allocated object
among multiple instances of a struct, each of which
have different lifetimes.

## Functions

Allocate a new rcpointer that owns the raw pointer.
```
rcpointer* rcp_new( gpointer obj, pointer destructor );
```

Delete the rcpointer, and destroy the raw object if reference count is zero.
```
rcpointer* rcp_delete( rcpointer* ptr );
```

Copy the rcpointer.
```
rcpointer* rcp_copy( rcpointer* const ptr );
```

Get the raw pointer to the object owned by the rcpointer.
```
gpointer rcp_get( rcpointer* const ptr );
```

## Examples

The header comment in each example file gives the compilation command
for gcc, and the command for running the example using valgrind to
show that no memory issues occur.

[example_rcpointer.c](example_rcpointer.c)

Simple usage of rcpointer.
Displays how a single raw pointer is contained in multiple rcpointer objects.

[example_rcpointer_DocView.c](example_rcpointer_DocView.c)

Example of model-view-controller pattern using rcpointer.
Relationship of model:view is 1:many, so the model object pointer is shared.
Here, the model is a document, and the view is an unspecified widget
that, if implemented, will display a view of the document's contents.
