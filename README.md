# Spatial_indexing Using R Trees
## Table of Content
- [Demo](#demo)
- [Screen Shots](#screen-shots)
- [Overview](#overview)
- [Motivation](#motivation)
- [Deep into R Trees](#deep-into-r-trees)
- [Technical Aspect](#technical-aspect)
- Features
- Installation
- Algorithm
- Runtime Complexities
- Directory Tree
- Technology and Tools
- To Do
- [Reference](#references)
- Contact
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
