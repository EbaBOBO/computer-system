Project 3: Caching I/O
======================

<!-- TODO: Fill this out. -->

## Design Overview:

## Collaborators:
Yiran Zhou
## Conceptual Questions:
1.Come up with a real world analogy to explain caching to someone who does not know what a computer is.

Cache likes a refrigerator and memory likes a farm. When we want some foods from the farm, it's unconvenient for us to go to the farm to get the food every time. So we can take some foods and store them in the refrigerator so that we can get it directly at home. We don't need to go to the farm when we want to have foods.

2.What are the benefits of having a standard file I/O API provided by the operating system? How does this help programmers who might not know what hardware their programs will be running on?
Having a standard file I/O API can reduce the time of calling the system call, which can save our time a lot. With the standard file I/O, we can know where the data comes from and goes. Programmers can know the preocesses of how to read and write the data into the disk clearly. 

3.Read about the catchphrase “Everything is a file” that is central to the design of UNIX-like operating systems (e.g., Linux, macOS, Android). Why might this complicate caching I/O?

Everything is a file means a great range of input/output resources are simple streams of bytes exposed through the filesystem name space. When we want to open a file, a file descriptor is created. The file path will become the addressing system and the file descriptor will become the byte stream  I/O interface. Caching I/O aimes at convenient programmers to reduce the time of the system call. But if everything is a file, we need to design different structure and caching to store different files with diffrent features, which will be extremely complicated. 

## How long did it take to complete Caching I/O?
15 hours