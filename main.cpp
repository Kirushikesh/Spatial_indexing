#include<iostream>
#include "rtree.h"
#include "opengl.h"

using namespace std;

int main(int argc,char *argv[])
{
    rtree tree;
    element p[]={element(1,coordinate(100,130)),element(2,coordinate(130,100)),element(3,coordinate(180,200)),
                 element(4,coordinate(160,275)),element(5,coordinate(150,300)),element(6,coordinate(200,290)),
                 element(8,coordinate(250,280)),element(7,coordinate(220,260)),element(9,coordinate(230,175)),
                 element(10,coordinate(300,220)),element(11,coordinate(240,120)),element(12,coordinate(255,140))};
    for(int i=0;i<12;i++)
        tree.insert(&p[i]);
    //tree.deletion(&element(1,coordinate(0,0)));
    //tree.deletion(&element(5,coordinate(4,4)));
    tree.root->print_node();
    //tree.root->child[0]->print_node();
    //tree.root->child[1]->print_node();
    //tree.search(&element(5,coordinate(4,5)));
    //mbb *m=new mbb(4,3,5,7);
    //tree.knn(3,&coordinate(4,6));
    //tree.range_search(&mbb(0,0,2,4));
    Visualize_tree(tree.root,argc,argv);

    return 1;
}

// Add OpenGL for visualization.
