#include<iostream>
#include<fstream>
#include<vector>
#include <sstream>
#include "rtree.h"
#include "opengl.h"

using namespace std;

mbb* convert_to_box(int nop,coordinate **co)
{
    int minx=9999,miny=9999,maxx=-1,maxy=-1,i;
    for(i=0;i<nop;i++)
    {
        if(co[i]->x<minx)
            minx=co[i]->x;
        if(co[i]->x>maxx)
            maxx=co[i]->x;
        if(co[i]->y<miny)
            miny=co[i]->y;
        if(co[i]->y>maxy)
            maxy=co[i]->y;
    }
    mbb* temp=new mbb(minx,miny,maxx,maxy);
    return temp;
}

int main(int argc,char *argv[])
{
    int key,nop,p1,p2,i;
    string line,word;
    vector<string> row;
    stringstream geek;

    coordinate **co;
    mbb *b;
    element *e;

    rtree tree;
    fstream fio;

    fio.open("D:/packages/3rd sem/spatial_indexing/dataset.csv",ios::in);
    getline(fio,line);

    while(fio)
    {
        row.clear();

        getline(fio,line);
        stringstream s(line);
        if(line=="")
            break;

        while(getline(s, word, ','))
        {
            row.push_back(word);
        }

        geek << row[0];
        geek >> key;
        geek.clear();

        geek << row[2];
        geek >> nop;
        geek.clear();

        co=new coordinate*[nop];

        for(i=3;i<3+2*(nop);i+=2)
        {
            geek << row[i];
            geek >> p1;
            geek.clear();

            geek << row[i+1];
            geek >> p2;
            geek.clear();

            co[i-3]=new coordinate(p1,p2);
        }

        b=convert_to_box(nop,co);
        e=new element(key,b);
        tree.insert(e);
    }
    tree.root->print_node();
    Visualize_tree(tree.root,argc,argv);
    fio.close();
    return 0;
}
