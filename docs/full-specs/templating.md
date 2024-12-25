# Templating

Templating is based on a [vs.templ](https://github.com/lazy-eggplant/vs.templ), a custom XML preprocessor loosely inspired by [xslt](https://www.w3schools.com/xml/xsl_intro.asp).  
Its documentation can be found on its separate repository [here](https://github.com/karurochori/vs-templ).  
Within the context of `vs.fltk`, using templates involves two files:

- the static dataset which contains any structured needed by the final generation. For vs, its root must be `<vs:static-data>` and provide a `template` attribute pointing to the actual component template. The suggested extension is `.vs`, the same as normal components.
- the template itself, containing a `vs` tree structure and templating commands as well.  
  Its conventional extension is `.tvs`. These files should never be used as components on their own.

When importing or using a component which requires processing, it is compiled and replaces the original dataset before the building process in the UI continues.  
You can find an example for a [dataset](../examples/test-templ.vs) & its associated [template](../examples/test-templ.tvs).
