# WSNFramework
Wireless Sensor Network (WSN) Framework project is a C++ Visual Studio 2010 solution that implements a few routing algorithms in 3D geographic
graphs with the goal of comparing their performance. This was my Master's project which I defended a few years ago. The main two algorithms
implemented are: Greedy-Random-Greedy (with and without the addition of a Virtual Network to lower path length) and Trace Routing. A new
algorithm which combines elements from both algorithms is also implemented. Two main libraries are used to help the implementation: 
CGAL - a computational geometry algorithms library used to compute some geometric calculations, and Google Test used as the driving tool to 
run tests with different parameters. All performance data is stored in SQlite.

Near Future Plans: When I first started the project, CGAL was not available in C++11. In addition, as the defense time drew nearer, my code
became sloppier. I am planning to re-write the entire project in at least C++11 and possible add more algorithms.
