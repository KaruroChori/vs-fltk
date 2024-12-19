/*
Policies are a bit of a mess because of the many variables involved, the need to track env variables and different default profiles for each class.  
Basically, getting bugs in there is way too simple and I would prefer not to.  

Codegen from a fully explicit and annotated schema would signficiatly help in making policies safer by being bug-free.  
It would also avoid very expensive refactorings if at some point the architecture requires changes.

The overall structure for codegen is as it is right now, starting from some json/xml schema of it. Probably json and an xml is generated like we do for components.
*/