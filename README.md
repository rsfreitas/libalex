# libalex

ALlegro EXtensions Library

Libalex is a library to manipulate Allegro DIALOGs trough UI resource files,
in JSON format. The user may define all objects and behavior from a dialog
without write a single line of code.

The library allows that outside functions may be assigned to an object and
called within an event.

## License

LGPLv2

## Dependencies

* Allegro library 4.4.2
* libcollections

## TODO

* Rename GRC to some other more clear extension that best matches our goal
  with the library.

* Add more examples

* Add support to plugins from libcollections.

* Add reference count to the exported objects.

* Add a storage structure to keep objects info, like the text from a button,
  with the goal of centering all this info in this structure and be able to
  release everything else.

* Include some check while using a virtual keyboard without an edit object.

* Change all exported data to opaque types.

* Convert all string manipulation to use *cstring_t* from libcollections.

* Framegrabber support, for live capture, with multilive support also.

* Test objects of 'icon' and 'image' type.

* Create an object like a task manager.

* When using a *digital_clock*, create a way that all DIALOGs created after
  may also have this object. Or update the clock inside a thread, so it may
  continue work outside your DIALOG.

* Load the 'info' block even when the gfx mode is not been loaded again.

* Try to make the virtual keyboard available only when accessing objects of
  edit type.

* Implement a way that the user does not need to create a function to
  fill a listbox object.

* Create some objects as dialog boxes, like a mainwindow (to act as a desktop),
  default dialogs (with minimize and maximize buttons).

* Include field into the GRC file to set a default return value from a callback
  function call when there is no callback function.

* Add support for changing font using this library.

* Maybe remove the *al_grc_prepare_dialog* and pass all Allegro initialization
  to the *al_grc_prepare_dialog*.

