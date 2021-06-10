# Spatial_indexing Using R Trees
## Table of Content
- [Demo](#demo)
- [Screen Shots](#screen-shots)
- [Overview](#overview)
- [Motivation](#motivation)
- [Deep into R Trees](#deep-into-r-trees)
- [Technical Aspect](#technical-aspect)
- [Installation](#installation)
- [Run](#run)
- [Runtime Complexities](#runtime-complexities)
- [Directory Tree](#directory-tree)
- [Technology and Tools](#technology-and-tools)
- [To Do](#to-do)
- [Reference](#references)
- [Contact](#contact)
## Demo
## Screen Shots
## Overview
A spatial database is a database that is optimized for storing and querying data that represents objects defined in a geometric space. Most spatial databases allow the representation of simple geometric objects such as points, lines and polygons. Such databases are finding increasing use in applications in environmental monitoring, space, urban planning, resource management, geographic information systems (GIS).

A spatial query is a special type of database query supported by spatial databases that allow for the use of geometry data types such as points, lines and polygons and that these queries consider the spatial relationship between these geometries. 

Spatial indices are used by spatial databases (databases which store information related to objects in space) to optimize spatial queries. Conventional index types do not efficiently handle spatial queries such as how far two points differ, or whether points fall within a spatial area of interest. Common spatial index methods include:

- Quad Trees
- R Trees
- Geohash
- R+ Trees
- and manymore.
## Motivation
R-trees are tree data structures used for spatial access methods, i.e., for indexing multi-dimensional information such as geographical coordinates, rectangles or polygons.
The R-tree was found significant use in both theoretical and applied contexts.  

A common real-world usage for an R-tree might be to store spatial objects such as restaurant locations or the polygons that typical maps are made of: streets, buildings, outlines of lakes, coastlines, etc. and then find answers quickly to queries such as "Find all museums within 2 km of my current location", "retrieve all road segments within 2 km of my location" (to display them in a navigation system) or "find the nearest gas station" (although not taking roads into account). The R-tree can also accelerate nearest neighbor search for various distance metrics, including great-circle distance.

For Visualisation we use OpenGl. OpenGL is a cross-language, cross-platform application programming interface for rendering 2D and 3D vector graphics.
## Deep into R trees
![image](https://user-images.githubusercontent.com/49152921/121508047-d491c380-ca02-11eb-9cc4-e7ba3501d42d.png)

The key idea of the data structure is to group nearby objects and represent them with their minimum bounding rectangle in the next higher level of the tree, the "R" in R-tree is for rectangle.  Since all objects lie within this bounding rectangle, a query that does not intersect the bounding rectangle also cannot intersect any of the contained objects. 

![image](https://user-images.githubusercontent.com/49152921/121508174-f4c18280-ca02-11eb-9123-eceefa600577.png)

At the non leaf level, the node has a minimum bounding box enclosing all its child nodes mbr's.

![image](https://user-images.githubusercontent.com/49152921/121508321-1884c880-ca03-11eb-8051-b60956942624.png)

At the leaf level, the leaf node points to the actual objects in the database enclosed with in the mbr, at higher levels the aggregation includes an increasing number of objects. This can also be seen as an increasingly coarse approximation of the data set.

![Screenshot (103)](https://user-images.githubusercontent.com/49152921/121506044-00ac4500-ca01-11eb-8727-f99f6403ebb5.png)

Similar to the B-tree, the R-tree is also a balanced search tree (so all leaf nodes are at the same depth), organizes the data in pages, and is designed for storage on disk (as used in databases). Each page can contain a maximum number of entries, often denoted as M. It also guarantees a minimum fill (except for the root node).

The key idea of searching algorithms on R trees is to use the bounding boxes to decide whether or not to search inside a subtree. In this way, most of the nodes in the tree are never read during a search. R-trees do not guarantee good worst-case performance, but generally perform well with real-world data.
## Technical Aspect
In this project we can insert object like point, line, circle and, polygon. The data to be inserted is stored in a csv file(dataset.csv). In the file each object has a unique id and points of the object and if needed we can also add descriptions for each object.
The queries include
- Given a rectangular region finding all the objects that overlaps the box.
- Given a query point and a K value finding K Nearest Objects to that point.
- Searching for an object
- Given a query point finding the object which contains the point.
Deleting a object from the R trees and finally visualising the R trees using OpenGL.

The project was divided into 3 files rtrees.h which holds the code for the Tree, opengl.h which holds the code of OpenGl for visualisation and main.cpp which is used to integrate the both.
## Installation
The code was written using C++. If you don't have IDE that supports C++ please install one. After that please download OpenGL and follow the instructions to install using this [link](https://users.cs.jmu.edu/bernstdh/web/common/help/cpp_mingw-glut-setup.php). Finally Clone the repository and move to next sub heading.
## Run
First Lets start with the dataset which consist of keyid, type, no of points and, the corresponding coordinates. With a total of rows each row denotes a object.

![image](https://user-images.githubusercontent.com/49152921/121520720-debabe80-ca10-11eb-8fbc-c7394013a34a.png)

Dont Worry about the header files they are just few lines of codes to perform the following tasks.
### Creating a tree
First Create a tree object of class rtree. Done by 
```
rtree tree;
```
### Insertion
Now we are inserting an element(object) into the R trees. The Element class consist of a keyid and the mbr enclosing the object. Since the object may be of any size we are not storing the details of the object.
```
//element constructor takes the parameters int and mbb*. Here b is a pointer to the mbb of the object.
element *e=new element(key,b);    
//takes the parameters element*.
tree.insert(e);     
```
The insertion algorithm uses the Guttman's Quadratic Split Algorithm for splitting the node when an overflow occurs.
### Printing the Node Details
Every node has a function print_node which used to return the particular node details. For ex:
```
tree.root->print_node();
```
### Searching for a Object
To check whether a object is present in the tree or not. Use
```
// Search function takes the parameter element*. Here e is the element pointer which points to the query element.
tree.search(e);
```
### Finding the object wrt the point
To find the object which contains the query point use the following code
```
// findobj_wrt_point takes a parameter coordinate*. Where the coordinate object consist of x and y coordinate values.
tree.findobj_wrt_point(q);
```
### Range Search
To find all the object that lies or overlaps in the query box. Use the following code
```
//range_search function takes the input mbb*. Where mbb object consist of query boxes bottomleftx, bottomlefty, toprightx and, toprighty. 
//here b is a pointer points to a mbb object.
tree.range_search(b);
```
### KNN Search
To find all the k objects given a query point. Use the following code
```
//knn function takes int,coordinate* as the input.
//here k is the K no of neighbours and query is a pointer to the coordinate object.
tree.knn(k,query);
```
### Deletion
For deleting a object from the R tree. Use the following code
```
//deletion function takes the parameter element*.
//here e is the pointer points to the object which is to be deleted.
tree.deletion(e);
```
To understand much detail about how each of these algorithms work internally refer the references I shared.
### Visualisation
To visualise the R tree using the opengl. Call this function
```
//Visualize_tree takes the parameter node*,int,char**
//here tree.root is the root of the rtree, argc and argv are the arguments of the main function.
Visualize_tree(tree.root,argc,argv);
```
The keys used for visualisation is UP, DOWN, LEFT, Down Arrows for moving the objects. And the Key W and S for Zoom in and out.
Also keep in mind to call this visualize function at the last because it is called it never returns.
## Runtime Complexities
Search: average complexity is ![image](https://user-images.githubusercontent.com/49152921/121526043-a5854d00-ca16-11eb-8886-4c27e4c68491.png)

Insert: average complexity is ![image](https://user-images.githubusercontent.com/49152921/121526074-addd8800-ca16-11eb-9d57-abb20abf17e9.png)
## Directory Tree
![image](https://user-images.githubusercontent.com/49152921/121527695-4de7e100-ca18-11eb-8002-e21fa8c443ec.png)

## Technology and Tools
- C++
- OpenGL
## To Do
- Fixing some bugs(if found).
- Giving more features while visualising the tree.
- Adding more complex queries.
## References
- https://en.wikipedia.org/wiki/R-tree
- http://www.cs.umd.edu/~hjs/pubs/kim.pdf
- https://en.wikipedia.org/wiki/Spatial_database
- http://www.bowdoin.edu/~ltoma/teaching/cs340/spring08/Papers/Rtree-chap1.pdf
- https://www.cse.cuhk.edu.hk/~taoyf/course/infs4205/lec/rtree.pdf
- https://github.com/th-10/R-Tree-cpp
- http://www.mathcs.emory.edu/~cheung/Courses/554/Syllabus/3-index/R-tree.html
- http://postgis.org/support/rtree.pdf
- https://www.geeksforgeeks.org/insertion-in-a-b-tree/
- http://www09.sigmod.org/disc/disc99/disc/record/issues/9809/cheung.pdf
- http://postgis.refractions.net/support/nearestneighbor.pdf
- https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/
## Contact
If you found any bug or like to raise a question feel free to contact me through [LinkedIn](https://www.linkedin.com/in/kirushikesh-d-b-10a75a169/). If you feel this project helped you and like to encourage me for more kinds of stuff like this please endorse my skills in my [LinkedIn](https://www.linkedin.com/in/kirushikesh-d-b-10a75a169/) thanks in advance!!!.
