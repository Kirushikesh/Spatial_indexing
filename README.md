# Spatial_indexing Using R Trees
## Table of Content
- [Demo](#demo)
- [Screen Shots](#screen-shots)
- [Overview](#overview)
- [Motivation](#motivation)
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

This project involves implementing spatial indexing for spatial queries using R Trees and gives us a visualisation of R trees using OpenGL.
## Motivation
R-trees are tree data structures used for spatial access methods, i.e., for indexing multi-dimensional information such as geographical coordinates, rectangles or polygons.
The R-tree was found significant use in both theoretical and applied contexts.  

A common real-world usage for an R-tree might be to store spatial objects such as restaurant locations or the polygons that typical maps are made of: streets, buildings, outlines of lakes, coastlines, etc. and then find answers quickly to queries such as "Find all museums within 2 km of my current location", "retrieve all road segments within 2 km of my location" (to display them in a navigation system) or "find the nearest gas station" (although not taking roads into account). The R-tree can also accelerate nearest neighbor search[4] for various distance metrics, including great-circle distance.

Visualisation helps us to understand the Rtrees better using OpenGL framework.
## Technical Aspect
This project was developed using R Trees and OpenGL. The inputs for the R trees was taken from a dataset and inserted into the rtrees which will be disscussed in the later section and different types of queries are raised. Finally deleting the objects and visualisation.
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
