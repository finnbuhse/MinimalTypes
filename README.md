# MinimalTypes

# What is included?
MinimalTypes currently provides 6 types total, with the intension of more to come in the future.
These include:
  - Vector<typename T>: A dynamic array which can contain a type of choice, it has a size of 16 bytes. However, its buffer is allocated elsewhere so its total memory consumption is this plus the size of the buffer. It is also a Plain-Old-Data (POD) type and the advantage as a result is that it is more memory efficient and can be packed contiguously into arrays. Although, due to the restrictions of POD types, this class must be manually allocated with 'initialize()' and deallocated with 'destroy()'.
  - List<typename T>: A dynamic linked list which can contain a type of choice, it has a size of 24 bytes. It is not a POD type.
  - SortedList<typename T>: A dynamic linked list which automatically sorts elements as you append them to it. It also has a size of 24 bytes. It is not a POD type.
  - String<size_t S>: A fixed size string type which can be edited at runtime but cannot change size. It has a size of S and is a POD type. It also automatically constructs and deconstructs.
  - ContiguousMap<typename K, typename T>: A non-POD hash map which utilises indices and a seperate buffer to store contained values contiguously. It has a size of 112 bytes.
  - StaticMap<typename K, typename T, size_t N>: A non-POD hash map which utilises indices and a array to store contained values as contiguously as possible while maintaining the same pointers for the contained values. This is more efficient since there is no memory reallocation (Apart from potentially within internal map). It has a size of 80 bytes plus N * sizeof(T).
  
# How to setup
 1. - Download the library as a ZIP and provide it's include directory to your IDE or copy the files into your project.
 2. - MinimalTypes is effectively a header only library, however to allow for cyclic inclusion and avoid function redefinitions you must create a cpp file. It can have any name.
 3. - Within this file you must then write the following lines of code:
  
    #include "MinimalTypes.h" <- This includes the library so the compiler can match the function definitions within the CPP component to the functions declared in the header file
                                 
    #include MINIMAL_TYPES_INCLUDE <- This includes the addition headers that are not neccessary in the header file but are for the function definitions within the CPP component
  
    MINIMAL_TYPES_CPP <- This defines the neccessary function definitions for the library to work properly
 
And that is the setup complete. You should now be able to indidividually include "Vector.h", "String.h", and "Map.h", or include "MinimalTypes.h" to include all 3 files at once.
