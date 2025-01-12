`vs` is designed to use several types of data storage mechanisms for different entities or applications. None of them is shared across multiple users.

- `memory-storage`, mapped in memory. It is session based, meaning that restarting the application will erase it, but its entries can be promoted to other more permanent types of storage. It is used to preserve name resolutions, scripts (not serialized), xml documents (not serialized) and memory buffered resources like script sources or images. The same name can be used to store different variations of the same entity. For example a name resolution the script source and the final script object.
- `kv-storage`, mapped in the SQLite database. It is used to preserve the state of an application and, more in general, information which is not for deletion over multiple sessions. However, it is possible to specify a deadline for entries to be removed.
- `res-storage`, mapped in the SQLite database. Similar to the previous type of storage, however this one is specifically designed to preserve certain types of resources after promotion from the memory storage. Things like remote content which has been collected once already. Things stored in this storage can be deleted at any point if the current policies on disk usage are violated.
- `secrets`, mapped in the SQLite database. It is used to preserve data which is relevant to a certain application, but also how to share it with others.
- The XML profiles, stored on disk but loaded on memory when running `vs`.
- `logs`, mapped in the SQLite database. Rotated and deletable if disk usage policies are violated.
- `bookmarks`, mapped in the SQLite database. Permanent. They can be both automatically added based on history, or on direct user actions.
- `policies`, mapped in the SQLite database. An extension of the XML profiles, storing detailed policy information for each application which has exceptions.
