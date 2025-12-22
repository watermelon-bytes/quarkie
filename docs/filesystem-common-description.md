### Description of the Quarkie systems common mechanisms

The Quarkie filesystem is designed to be simple and fast (see KISS). It uses trees to store directories and files descriptors called `nodes`. *Each node can describe both folder and file*.
Since `struct node` has a fixed size, Quarkie ecosystem manages metadata space via bitmaps. Also, since we know that the space is supposed to be allocated for a single `node` only, Quarkie filesystem doesn't have to store every chunk size (i.e. we guarantee that each request to the bitmap wants a fixed well-known amount of memory). So the bitmap has to be able to define and switch 1 bit in its descriptors.

The information about what node belongs to (file or directory) is kept in bit 0 in the `attributes` field. Also, `attributes` contains all metadata about an object a node describes.

The main idea that the filesystem was designed with is that a node can be used to describe both file and directory. 
A `node` has at least two pointers:
- **pointer to the next `node` inside same directory** and *if this* `node` *represents a directory*, then it holds another pointer to the head of linked list (another `node`) that represents the directory's content. Thus, directories can be stored recursively.
- **pointer to the parental directory**, i.e. each file and each folder has a parent, so that we can move through nodes in all directions (as downwards as upwards).

Let's consider this example where directory A contains files B, C and another directory D with file E. So:
1. All *A*'s, *B*'s and *C*'s field "`parent`" points to *A*
2. *A* points to *B* (*A*.head = address of *B*)
3. *B*.next = *C* --> *C*.next = *D* --> *D*.next = *NULL* --> ✋
4. Since *D* is a directory, *D*.child = *E*. 
Note that the same bits a directory node uses to hold pointer to the first child, are used by file to store information about the space belonging to it.
