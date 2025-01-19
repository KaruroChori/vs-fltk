This folder contains all bindings that the embedded scripts need to expose functionality from `vs` to the user.  
Both module and single user scripts share the same bindings, however certain feature might be disabled in module scripts to avoid/mitigate aliasing.  

These are **not** the vs-fltk C bindings. Those are in `/include/cbindings/*` and `/src/cbindings/*`.