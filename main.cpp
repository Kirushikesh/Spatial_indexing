#include<iostream>
#include<fstream>
#include<vector>
#include <sstream>
#include "rtree.h"
#include "opengl.h"

using namespace std;

int main(int argc,char *argv[])
{
    rtree tree;
    element *e;
    coordinate *c;
    int key,nop,p1,p2;

    fstream fio;
    string line,word;
    fio.open("D:/packages/3rd sem/spatial_indexing/dataset.csv",ios::in);
    getline(fio,line);
    vector<string> row;

    stringstream geek;
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

        geek << row[3];
        geek >> p1;
        geek.clear();

        geek <<row[4];
        geek >> p2;
        geek.clear();

        cout<<key<<" "<<nop<<" "<<p1<<" "<<p2<<endl;
        c=new coordinate(p1,p2);
        e=new element(key,*c);
        tree.insert(e);
    }

    fio.close();
    Visualize_tree(tree.root,argc,argv);
    return 0;
}
