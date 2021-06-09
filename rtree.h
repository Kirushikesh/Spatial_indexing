#ifndef RTREE_H_INCLUDED
#define RTREE_H_INCLUDED

#include<iostream>
#include<cmath>
#include<vector>

using namespace std;

const int M=3;

class coordinate
{
    public:

    //stores coordinates of the points
    int x,y;

    coordinate()
    {}

    coordinate(const coordinate &another)
    {
        x=another.x;
        y=another.y;
    }

    coordinate(int a,int b)
    {
        x=a;
        y=b;
    }

};

class mbb
{
    public:

    coordinate *bottom;
    coordinate *top;

    mbb(int bottomleftx,int bottomlefty,int toprightx,int toprighty)
    {
        bottom=new coordinate(bottomleftx,bottomlefty);
        top=new coordinate(toprightx,toprighty);
    }

    mbb(coordinate *b,coordinate *t)
    {
        bottom=new coordinate(*b);
        top=new coordinate(*t);
    }

    mbb(const mbb &ano)
    {
        bottom=new coordinate(*ano.bottom);
        top=new coordinate(*ano.top);
    }

    void print_mbb()
    {
        cout<<"Bottom Left is ( "<<bottom->x<<" , "<<bottom->y<<" )";
        cout<<" And the Top Right is ( "<<top->x<<" , "<<top->y<<" )\n";
    }

    //calculates the area of the bounding box
    int area()
    {
        return (top->x-bottom->x)*(top->y-bottom->y);
    }

    //calculate the overlapping area btwn 2 objects
    //gives the accurate results only when both are boxes
    int overlapping_area(mbb* ano)
    {
        int x_dist=min(top->x,ano->top->x)-max(bottom->x,ano->bottom->x);
        int y_dist=min(top->y,ano->top->y)-max(bottom->y,ano->bottom->y);
        int area=0;
        if(x_dist>0 && y_dist>0)
            area=x_dist*y_dist;
        return area;
    }

    //returns the min area enlargement when we try to add the box within this box
    int area_enlargement(mbb *ano)
    {
        int totalarea=(max(top->x,ano->top->x)-min(bottom->x,ano->bottom->x))*((max(top->y,ano->top->y)-min(bottom->y,ano->bottom->y)));
        return totalarea+overlapping_area(ano)-area()-ano->area();
    }

    //changes the box coordinate w.r.t the another box which is the child of this.
    void handle_newobj(mbb *ano)
    {
        if(bottom->x>ano->bottom->x)
            bottom->x=ano->bottom->x;
        if(bottom->y>ano->bottom->y)
            bottom->y=ano->bottom->y;
        if(top->x<ano->top->x)
            top->x=ano->top->x;
        if(top->y<ano->top->y)
            top->y=ano->top->y;
    }

    //calculates the mindist btwn this box and a point
    int mindist(coordinate *qi)
    {
        int min=0;
        if(qi->x<bottom->x)
            min+=pow(qi->x-bottom->x,2);
        else if(qi->x>top->x)
            min+=pow(qi->x-top->x,2);
        if(qi->y<bottom->y)
            min+=pow(qi->y-bottom->y,2);
        else if(qi->y>top->y)
            min+=pow(qi->y-top->y,2);
        return min;
    }

    //checks whether a line(box with area 0) intersects a box
    bool intersects(mbb *b)
    {
        if((b->bottom->x>=bottom->x && b->top->x<=top->x) && !(b->bottom->y>top->y || b->top->y<bottom->y))
            return true;
        else if(!(b->bottom->x > top->x || b->top->x < bottom->x) && (b->bottom->y>=bottom->y && b->top->y<=top->y))
            return true;
        return false;
    }
};

//returns boxes index which are having maximum dead space
int *pick_seed(mbb **tot)
{
    int maxdis=-1;
    int i,j,dis;
    int *seed=new int[2];
    for(i=0;i<M;i++)
    {
        for(j=i+1;j<M+1;j++)
        {
            dis=tot[i]->area_enlargement(tot[j]);

            if(dis > maxdis)
            {
                maxdis=dis;
                seed[0]=i;
                seed[1]=j;
            }
        }
    }
    return seed;
}

//returns the boxno and the index of the point in the tot array also take care of minno of nodes
int* pick_next(mbb **tot,mbb *box1,mbb *box2,bool *presence,int no1,int no2)
{
    int i,a1,a2,d1,d2,lambda=M+1-no1-no2,flag=0;
    int *result=new int[2];

    if(no1==(ceil((float)M/2)-lambda))
    {
        result[0]=1;
        flag=1;
    }

    else if(no2==(ceil((float)M/2)-lambda))
    {
        result[0]=2;
        flag=1;
    }

    for(i=0;i<M+1;i++)
    {
        if(presence[i]==false)
        {
            if(flag)
                break;

            d1=box1->area_enlargement(tot[i]);
            d2=box2->area_enlargement(tot[i]);

            if(d1<d2)
                result[0]=1;
            else if(d2<d1)
                result[0]=2;
            else
            {
                a1=box1->area();
                a2=box2->area();
                if(a1<a2)
                    result[0]=1;
                else if(a2<a1)
                    result[0]=2;
                else
                {
                    if(no1<no2)
                        result[0]=1;
                    else
                        result[0]=2;
                }
            }
            break;
        }
    }

    result[1]=i;
    return result;
}

class element
{
    public:

    int key;
    mbb* box;

    element(int k,mbb *c)
    {
        key=k;
        box=c;
    }

    void print_element()
    {
        cout<<"The key is "<<key<<endl;
        box->print_mbb();
    }

};

//adds a new coordinate into the results list based on insertion sort method
void add_knn(element *newpnt,int distance,element** results,int dis[],int no,int k)
{
    int i;
    if(no==k)
        i=no-2;
    else
        i=no-1;
    while(distance<dis[i] && i>=0)
    {
        dis[i+1]=dis[i];
        results[i+1]=results[i];
        i--;
    }
    results[i+1]=newpnt;
    dis[i+1]=distance;
}

class node
{
    public:

    bool isleaf;
    mbb *box;
    element *leafs[M];
    node *child[M];
    int level;
    int no_leafs;
    int no_childs;

    node(bool leaf,int l)
    {
        isleaf=leaf;
        box = NULL;
        for(int i=0;i<M;i++)
        {
            leafs[i]=NULL;
            child[i]=NULL;
        }
        no_leafs=0;
        no_childs=0;
        level=l;
    }

    //insert a new point in the leaf node
    void insert_leaf(element *e)
    {
        if(box==NULL)
            box=new mbb(*e->box);
        leafs[no_leafs]=e;
        box->handle_newobj(e->box);
        no_leafs++;
    }

    //deletes the point in the leaf node at Ex index
    void delete_leaf(int Ex)
    {
        element *temp;
        temp=leafs[Ex];
        leafs[Ex]=NULL;
        delete temp;

        int i;
        for(i=Ex;i<no_leafs-1;i++)
        {
            leafs[i]=leafs[i+1];
        }
        leafs[i]=NULL;
        no_leafs--;
    }

    //insert a new child node in a non leaf node
    void insert_child(node *c)
    {
        if(box==NULL)
            box=new mbb(*c->box);
        child[no_childs]=c;
        box->handle_newobj(c->box);
        no_childs++;
    }

    //Removes the child of the node at Ex index
    void remove_child(int Ex)
    {
        int i;
        child[Ex]=NULL;
        for(i=Ex;i<no_childs;i++)
        {
            child[i]=child[i+1];
        }
        child[i]=NULL;
        no_childs--;
    }

    void print_node()
    {
        cout<<"Node dimensions are\n";
        box->print_mbb();
        cout<<"No of childs are "<<no_childs<<" No of Leafs "<<no_leafs<<endl;
        cout<<" Level of the node is "<<level<<endl;
        if(isleaf==true)
        {
            for(int i=0;i<no_leafs;i++)
                leafs[i]->box->print_mbb();
        }
        else
        {
            for(int i=0;i<no_childs;i++)
                child[i]->box->print_mbb();
        }
    }

    //returns a list of all mbb including next
    mbb** create_list(mbb *next)
    {
        mbb **temp=new mbb*[M+1];
        for(int i=0;i<M;i++)
            temp[i]=child[i]->box;
        temp[M]=next;
        return temp;
    }

    //returns a list of all mbb including next
    mbb** create_list(element *next)
    {
        mbb **temp=new mbb*[M+1];
        for(int i=0;i<M;i++)
            temp[i]=leafs[i]->box;
        temp[M]=next->box;
        return temp;
    }

    //adjust the node mbr after deletion
    void adjust_mbr()
    {
        int minx=999,maxx=-1,miny=999,maxy=-1,i;
        if(isleaf==true)
        {
            for(i=0;i<no_leafs;i++)
            {
                if(leafs[i]->box->bottom->x<minx)
                {
                    minx=leafs[i]->box->bottom->x;
                }
                if(leafs[i]->box->top->x>maxx)
                {
                    maxx=leafs[i]->box->top->x;
                }
                if(leafs[i]->box->bottom->y<miny)
                {
                    miny=leafs[i]->box->bottom->y;
                }
                if(leafs[i]->box->top->y>maxy)
                {
                    maxy=leafs[i]->box->top->y;
                }
            }
        }
        else
        {
            for(i=0;i<no_childs;i++)
            {
                if(child[i]->box->bottom->x<minx)
                {
                    minx=child[i]->box->bottom->x;
                }
                if(child[i]->box->top->x>maxx)
                {
                    maxx=child[i]->box->top->x;
                }
                if(child[i]->box->bottom->y<miny)
                {
                    miny=child[i]->box->bottom->y;
                }
                if(child[i]->box->top->y>maxy)
                {
                    maxy=child[i]->box->top->y;
                }
            }
        }
        box->bottom->x=minx;
        box->bottom->y=miny;
        box->top->x=maxx;
        box->top->y=maxy;
    }

    //searchs the point present within a query box coordinates
    void range_search_helper(mbb *query)
    {
        if(isleaf==false)
        {
            for(int i=0;i<no_childs;i++)
            {
                if(query->overlapping_area(child[i]->box)>0 || query->intersects(child[i]->box))
                    child[i]->range_search_helper(query);
            }
        }
        else
        {
            for(int i=0;i<no_leafs;i++)
            {
                if(query->overlapping_area(leafs[i]->box)>0 || query->intersects(leafs[i]->box) )
                {
                    cout<<"The key is "<<leafs[i]->key<<endl;
                    leafs[i]->box->print_mbb();
                }
            }
        }
    }

    //searchs object which contain coordinate e
    void search_obj(coordinate *e)
    {
        int i;
        if(isleaf==true)
        {
            for(i=0;i<no_leafs;i++)
                if(leafs[i]->box->mindist(e)==0)
                    leafs[i]->print_element();
        }
        else
        {
            for(i=0;i<no_childs;i++)
                if(child[i]->box->mindist(e)==0)
                    child[i]->search_obj(e);
        }
    }

    //finds the knn based on the query point (declared outside)
    int knn_helper(element **results,int dis[],coordinate *query,int cur,int k);

};

//sorts the node based on mindis uses insertion sort
void sort_branchlist(node **branchlist,int dis[],int n)
{
    int i=0, key, j;
    node *temp;
    for (i = 1; i < n; i++)
    {
        temp=branchlist[i];
        key = dis[i];
        j = i - 1;

        while (j >= 0 && dis[j] > key)
        {
            dis[j + 1] = dis[j];
            branchlist[j+1]=branchlist[j];
            j = j - 1;
        }
        dis[j + 1] = key;
        branchlist[j+1]=temp;
    }
}

//finds the knn based on the query point
int node::knn_helper(element **results,int dis[],coordinate *query,int cur,int k)
{
    int i=0,d;
    if(isleaf==true)
    {
        for(i=0;i<no_leafs;i++)
        {
            d=leafs[i]->box->mindist(query);
            if(d<=dis[cur-1] || cur<k)
                add_knn(leafs[i],d,results,dis,cur,k);
                if(cur<k)
                    cur++;
        }
    }
    else
    {
        node **branchlist=new node*[no_childs];
        int mindis[no_childs];

        for(i=0;i<no_childs;i++)
        {
            branchlist[i]=child[i];
            mindis[i]=child[i]->box->mindist(query);
        }

        sort_branchlist(branchlist,mindis,no_childs);
        for(i=0;i<no_childs;i++)
        {
            if(mindis[i]<=dis[cur-1] || cur<k)
            {
                cur=branchlist[i]->knn_helper(results,dis,query,cur,k);
            }
        }
    }
    return cur;
}

//find the same level node which can contain b
node* find_lvl_node(node *cursor,int lvl,mbb *b)
{
    if(cursor->level==lvl)
        return cursor;

    int i,minnode,minarea=9999,temp;

    for(i=0;i<cursor->no_childs;i++)
    {
        temp=cursor->child[i]->box->area_enlargement(b);
        if(minarea>temp)
        {
            minarea=temp;
            minnode=i;
        }
    }

    return find_lvl_node(cursor->child[minnode],lvl,b);
}

//returns the parent of a node
node * findparent(node *cursor,node *child)
{
    node *parent;
    if(cursor->isleaf)
        return NULL;
    for(int i=0;i<cursor->no_childs;i++)
    {
        if(cursor->child[i]==child)
            return cursor;
        else
        {
            parent=findparent(cursor->child[i],child);
            if(parent!=NULL)
                return parent;
        }
    }
    return parent;
}

class rtree
{
    public:
    node *root;

    rtree()
    {
        root=NULL;
    }

    //insert a point in the tree
    void insert(element *co)
    {
        if(root==NULL)
        {
            root=new node(true,0);
            root->insert_leaf(co);
        }
        else
        {
            node *cursor=root;
            node *parent;
            while(cursor->isleaf==false)
            {
                parent=cursor;
                parent->box->handle_newobj(co->box);
                int i,minnode,minarea=9999,temp;

                for(i=0;i<cursor->no_childs;i++)
                {
                    temp=cursor->child[i]->box->area_enlargement(co->box);
                    if(minarea>temp)
                    {
                        minarea=temp;
                        minnode=i;
                    }
                }
                cursor=cursor->child[minnode];
            }
            if(cursor->no_leafs<M)
            {
                cursor->insert_leaf(co);
            }
            else
            {
                int i;
                node *newleaf=new node(true,0);
                mbb **all_leafs=cursor->create_list(co);
                element *virtualelement[M+1];
                for(i=0;i<M;i++)
                {
                    virtualelement[i]=cursor->leafs[i];
                }
                virtualelement[M]=co;

                cursor->no_leafs=0;
                cursor->box=NULL;

                bool presence[M+1];
                for(i=0;i<M+1;i++)
                    presence[i]=false;

                int *seeds=pick_seed(all_leafs);

                cursor->insert_leaf(virtualelement[seeds[0]]);
                newleaf->insert_leaf(virtualelement[seeds[1]]);

                presence[seeds[0]]=true;
                presence[seeds[1]]=true;

                int no1=1,no2=1,*next;
                while(no1+no2<M+1)
                {
                    next=pick_next(all_leafs,cursor->box,newleaf->box,presence,no1,no2);
                    if(next[0]==1)
                    {
                        cursor->insert_leaf(virtualelement[next[1]]);
                        no1++;
                    }
                    else
                    {
                        newleaf->insert_leaf(virtualelement[next[1]]);
                        no2++;
                    }
                    presence[next[1]]=true;
                }

                if(cursor==root)
                {
                    node *newroot=new node(false,1);
                    newroot->insert_child(cursor);
                    newroot->insert_child(newleaf);
                    root=newroot;
                }
                else
                {
                    insert_helper(parent,newleaf);
                }
            }
        }
    }

    //helper of insert function inserts a particular node in another node
    void insert_helper(node *cursor,node *child)
    {
        if(cursor->no_childs<M)
        {
            cursor->insert_child(child);
        }
        else
        {
            int i;
            node *newinternal=new node(false,cursor->level);
            mbb **all_boxes=cursor->create_list(child->box);
            cursor->no_childs=0;
            cursor->box=NULL;
            node *virtualchild[M+1];
            for(i=0;i<M;i++)
            {
                virtualchild[i]=cursor->child[i];
            }
            virtualchild[M]=child;

            bool presence[M+1];
            for(i=0;i<M+1;i++)
                presence[i]=false;
            int *seeds=pick_seed(all_boxes);

            cursor->insert_child(virtualchild[seeds[0]]);
            newinternal->insert_child(virtualchild[seeds[1]]);

            presence[seeds[0]]=true;
            presence[seeds[1]]=true;

            int no1=1,no2=1,*next;
            while(no1+no2<M+1)
            {
                next=pick_next(all_boxes,cursor->box,newinternal->box,presence,no1,no2);
                if(next[0]==1)
                {
                    cursor->insert_child(virtualchild[next[1]]);
                    no1++;
                }
                else
                {
                    newinternal->insert_child(virtualchild[next[1]]);
                    no2++;
                }
                presence[next[1]]=true;
            }

            if(cursor==root)
            {
                node *newroot=new node(false,cursor->level+1);
                newroot->insert_child(cursor);
                newroot->insert_child(newinternal);
                newroot->box=new mbb(cursor->box->bottom->x,cursor->box->bottom->y,cursor->box->top->x,cursor->box->top->y);
                newroot->box->handle_newobj(newinternal->box);
                root=newroot;
            }
            else
            {
                insert_helper(findparent(root,cursor),newinternal);
            }
        }
    }

    //finds all the points inside a box m
    void range_search(mbb *m)
    {
        if(m->overlapping_area(root->box)>0 || m->intersects(root->box))
        {
            root->range_search_helper(m);
        }
        else
        {
            cout<<"No points occur"<<endl;
        }
    }

    //finds knn wrt the k and query point
    void knn(int k,coordinate *query)
    {
        element **results=new element*[k];
        int dis[k],no=0;
        for(int i=0;i<k;i++)
        {
            results[i]=NULL;
            dis[i]=99999;
        }

        no=root->knn_helper(results,dis,query,no,k);
        for(int i=0;i<no;i++)
        {
            results[i]->print_element();
            cout<<" and distance is "<<dis[i]<<endl;
        }
    }

    //deletes a element
    void deletion(element *e)
    {
        int i;
        node *L=find_lvl_node(root,0,e->box),*temp;

        for(i=0;i<L->no_leafs;i++)
            if(L->leafs[i]->key==e->key)
                break;

        L->delete_leaf(i);

        condense_tree(L);

        if(root->no_childs==1)
        {
            temp=root->child[0];
            delete root;
            root=temp;
        }
        else
        {
            root->adjust_mbr();
        }
    }

    //Maps the node L into its appropriate place
    void condense_tree(node *L)
    {
        node *X=L,*parentx,*temp;
        vector<node*> N;
        int Ex,i,j;
        while(X!=root)
        {
            parentx=findparent(root,X);
            for(i=0;i<parentx->no_childs;i++)
                if(X==parentx->child[i])
                    break;
            Ex=i;

            if(X->isleaf==true && X->no_leafs<ceil((float)M/2))
            {
                N.push_back(X);
                parentx->remove_child(Ex);
            }
            else if(X->isleaf==false && X->no_childs<ceil((float)M/2))
            {
                N.push_back(X);
                parentx->remove_child(Ex);
            }
            else
            {
                X->adjust_mbr();
            }
            X=parentx;
        }

        for(i=0;i<(int)N.size();i++)
        {
            if(N[i]->isleaf==true)
            {
                for(j=0;j<N[i]->no_leafs;j++)
                    insert(N[i]->leafs[j]);
            }
            else
            {
                for(j=0;j<N[i]->no_childs;j++)
                {
                    temp=find_lvl_node(root,N[i]->level,N[i]->box);
                    insert_helper(temp,N[i]->child[j]);
                }
            }
        }
    }

    //searchs for a element in the rtree
    void search(element *e)
    {
        int i;
        node *L=find_lvl_node(root,0,e->box);

        for(i=0;i<L->no_leafs;i++)
            if(L->leafs[i]->key==e->key)
                break;

        if(i==L->no_leafs)
        {
            cout<<"Element Not Found\n";
        }
        else
        {
            cout<<"Element Found\n";
            e->print_element();
        }
    }

    //finds the object in which the query point present
    void findobj_wrt_point(coordinate *query)
    {
        root->search_obj(query);
    }

};

#endif // RTREE_H_INCLUDED
