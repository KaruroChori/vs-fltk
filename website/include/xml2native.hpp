#pragma once
/*
    Compile an xml file representing a component into a .so library which can just be loaded as a native one.
    Extra speed for a decrease in portability.
    Still, the code generated is C, so can just use TCC to make magic.
    [QUESTION] What if we target directly something like qbe? 
               It can optimize code much better compared to tcc and we can have the tcc assembler as the last step.
*/