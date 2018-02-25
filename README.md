<h1>EmbeddedToolKit</h1>
<p>etk is a collection of C++11 classes that a particularly useful in for embedded applications. These classes include wrappers for arrays and C-strings as well as memory pools, smart pointers, ring buffers, a time class (for potentially counting systicks without rollover) and much more. This library exists to help developers write clean, high integrity C++ code without depending on potentially unsafe C functions (such as sprintf). 
<p>All code in this repository is under the MIT license. I don't care what you do with it, just don't sue me. 

<h3>General Design</h3>

etk is a header only library that consists of templates, classes and inline functions that are commonly implemented in embedded software projects. In general, if a template or class could be applied in at least 3 completely different applications, it may be accepted into etk. The purpose of etk is to bring together highly reusable templates, classes and functions into one project where they can be reviewed and maintained, along with their unit tests and documentation.

  
