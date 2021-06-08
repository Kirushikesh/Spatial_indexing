#include<iostream>
#include "rtree.h"

using namespace std;

int main(int argc,char *argv[])
{
    rtree tree;
    element p[]={element(1,coordinate(100,170)),element(2,coordinate(170,100)),element(3,coordinate(250,250)),
                 element(4,coordinate(225,275)),element(5,coordinate(190,300)),element(6,coordinate(275,290)),
                 element(8,coordinate(310,280)),element(7,coordinate(290,260)),element(9,coordinate(300,225)),
                 element(10,coordinate(330,250)),element(11,coordinate(315,120)),element(12,coordinate(325,150))};
    for(int i=0;i<12;i++)
        tree.insert(&p[i]);
    //tree.deletion(&element(1,coordinate(0,0)));
    //tree.deletion(&element(5,coordinate(4,4)));
    tree.root->print_node();
    //tree.root->child[0]->print_node();
    //tree.search(&element(5,coordinate(4,5)));
    //mbb *m=new mbb(4,3,5,7);
    //tree.knn(3,&coordinate(4,6));
    //tree.range_search(&mbb(0,0,2,4));
    Visualize_tree(tree.root,argc,argv);

    return 1;
}

// Add OpenGL for visualization.
