#pragma once
//Part of the testing pipeline. These xml files are read and its operation run on the ui tree.

/*
Actions in a test environment are allowed to access private name with `#` prefix.
```xml
<actions>
    <action.fill target=".../obj1" value="..."/>
    <wait time="1000"/>
    <action.callback target=".../obj2"/>
</actions>
```
*/