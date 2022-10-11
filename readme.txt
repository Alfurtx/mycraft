=== MyCraft ===

Minecraft clone, written in C++ using OpenGL.

== Design and ideas ==

For the world, instead of creating and setting up a VAO, VBO, and IBO for every chunk,
I'm going to create WORLD_CHUNK_SIZE amounts of them and just keep constantly reusing them
for every new chunk. This also benefits on the fact that chunks are very much the same in terms
of setting up things in OpenGL, so the only major change is in the buffer of data that's passed to it.
