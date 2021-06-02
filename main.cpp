#include<iostream>
#include<cmath>
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

    int bottom[2];
    int top[2];

    mbb(int bottomleftx,int bottomlefty,int toprightx,int toprighty)
    {
        bottom[0]=bottomleftx;
        bottom[1]=bottomlefty;
        top[0]=toprightx;
        top[1]=toprighty;
    }

    mbb(const mbb &ano)
    {
        bottom[0]=ano.bottom[0];
        bottom[1]=ano.bottom[1];
        top[0]=ano.top[0];
        top[1]=ano.top[1];
    }

    //calculates the area of the bounding box
    int area()
    {
        return (top[0]-bottom[0])*(top[1]-bottom[1]);
    }

    //calculate the overlapping area btwn 2 boxes
    int overlapping_area(mbb* ano)
    {
        int x_dist=min(top[0],ano->top[0])-max(bottom[0],ano->bottom[0]);
        int y_dist=min(top[1],ano->top[1])-max(bottom[1],ano->bottom[1]);
        int area=0;
        if(x_dist>0 && y_dist>0)
            area=x_dist*y_dist;
        return area;
    }

    //changes the box coordinate w.r.t the another box which is the child of this.
    void handle_newobj(mbb *ano)
    {
        if(bottom[0]>ano->bottom[0])
            bottom[0]=ano->bottom[0];
        if(bottom[1]>ano->bottom[1])
            bottom[1]=ano->bottom[1];
        if(top[0]<ano->top[0])
            top[0]=ano->top[0];
        if(top[1]<ano->top[1])
            top[1]=ano->top[1];
    }

    //returns the area enlargement when we try to add the box within this box
    int area_enlargement(mbb *ano)
    {
        int totalarea=(max(top[0],ano->top[0])-min(bottom[0],ano->bottom[0]))*((max(top[1],ano->top[1])-min(bottom[1],ano->bottom[1])));
        return totalarea+overlapping_area(ano)-area()-ano->area();
    }

};

//convert a point to box
mbb *point_to_box(coordinate *co)
{
    mbb *newbox=new mbb(co->x,co->y,co->x,co->y);
    return newbox;
}

//convert a box to coordinate
coordinate* box_to_point(mbb *mo)
{
    coordinate *newco= new coordinate(mo->bottom[0],mo->bottom[1]);
    return newco;
}

//returns boxes index which are having maximum dead space
int *pick_seed(mbb **tot)
{
    //cout<<"inside pick seed";
    int maxdis=-1;
    int i,j,dis;
    int *seed=new int[2];
    for(i=0;i<M;i++)
    {
        for(j=i+1;j<M+1;j++)
        {
            dis=tot[i]->area_enlargement(tot[j]);
            //cout<<"i is "<<i<<" j is "<<j<<" dis is "<<dis<<endl;
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

class node
{
    public:

    bool isleaf;
    mbb *box;
    coordinate *points[M];
    node *child[M];
    int no_points;
    int no_childs;

    node(bool leaf)
    {
        isleaf=leaf;
        box = NULL;
        for(int i=0;i<M;i++)
        {
            points[i]=NULL;
            child[i]=NULL;
        }
        no_points=0;
        no_childs=0;
    }

    //insert a new point in the leaf node
    void insert_point(coordinate *co)
    {
        if(box==NULL)
            box=new mbb(co->x,co->y,co->x,co->y);
        points[no_points]=new coordinate(*co);
        box->handle_newobj(point_to_box(co));
        no_points++;
    }

    //insert a new child node in a non leaf node
    void insert_child(node *c,bool type)
    {
        if(box==NULL)
            box=new mbb(*c->box);
        child[no_childs]=new node(type);
        child[no_childs]=c;
        box->handle_newobj(c->box);
        no_childs++;
    }

    //converts the points to boxes and return it
    mbb** create_list(coordinate *next)
    {
        mbb **temp=new mbb*[M+1];
        for(int i=0;i<M;i++)
            temp[i]=point_to_box(points[i]);
        temp[M]=point_to_box(next);
        return temp;
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

};

class rtree
{
    public:
    node *root;

    rtree()
    {
        root=NULL;
    }

    //insert a point in the tree
    void insert(coordinate *co)
    {
        if(root==NULL)
        {
            root=new node(true);
            root->insert_point(co);
        }
        else
        {
            node *cursor=root;
            node *parent;
            while(cursor->isleaf==false)
            {
                parent=cursor;
                parent->box->handle_newobj(point_to_box(co));
                int i,minnode,minarea=999,temp;

                for(i=0;i<cursor->no_childs;i++)
                {
                    temp=cursor->child[i]->box->area_enlargement(point_to_box(co));
                    if(minarea>temp)
                    {
                        minarea=temp;
                        minnode=i;
                    }
                }
                cursor=cursor->child[minnode];
            }

            if(cursor->no_points<M)
            {
                cursor->insert_point(co);
            }
            else
            {
                node *newleaf=new node(true);
                mbb **all_points=cursor->create_list(co);
                cursor->no_points=0;
                cursor->box=NULL;

                bool presence[M+1];
                for(int i=0;i<M+1;i++)
                    presence[i]=false;

                int *seeds=pick_seed(all_points);
                cursor->insert_point(box_to_point(all_points[seeds[0]]));
                newleaf->insert_point(box_to_point(all_points[seeds[1]]));

                presence[seeds[0]]=true;
                presence[seeds[1]]=true;

                int no1=1,no2=1,*next;
                while(no1+no2<M+1)
                {
                    next=pick_next(all_points,cursor->box,newleaf->box,presence,no1,no2);
                    if(next[0]==1)
                    {
                        cursor->insert_point(box_to_point(all_points[next[1]]));
                        no1++;
                    }
                    else
                    {
                        newleaf->insert_point(box_to_point(all_points[next[1]]));
                        no2++;
                    }
                    presence[next[1]]=true;
                }

                if(cursor==root)
                {
                    node *newroot=new node(false);
                    newroot->insert_child(cursor,true);
                    newroot->insert_child(newleaf,true);
                    root=newroot;
                }
                else
                {
                    insert_helper(parent,newleaf);
                }
            }
        }
    }

    void insert_helper(node *cursor,node *child)
    {/*
        if(cursor->no_childs<M)
        {
            cursor->insert_child(child);
        }
        else
        {
            int i;
            node *newinternal=new node(false);
            coordinate **all_boxes=cursor->create_mid_list(child->box);
            node *virtualchild[M+1];

            for(i=0;i<M;i++)
            {
                virtualchild[i]=cursor->child[i];
            }
            virtualchild[M]=child;

            cursor->no_childs=0;
            cursor->box=NULL;

            bool presence[M+1];
            for(int i=0;i<M+1;i++)
                presence[i]=false;

            int *seeds=pick_seed(all_boxes);
            cursor->insert_child(virtualchild[seeds[0]]);
            newinternal->insert_child(virtualchild[seeds[1]]);

            presence[seeds[0]]=true;
            presence[seeds[1]]=true;

            int no1=1,no2=1,*next;
            while(no1+no2<M+1)
            {
                next=pick_next(all_points,cursor->box,newinternal->box,presence,no1,no2);
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
                node *newroot=new node(false);
                newroot->insert_child(cursor);
                newroot->insert_child(newinternal);
                newroot->box=new mbb(cursor->box->bottom[0],cursor->box->bottom[1],cursor->box->top[0],cursor->box->top[1]);
                newroot->box->handle_newobj(newinternal->box);
                root=newroot;
            }
            else
            {
                insert_helper(findparent(root,cursor),newinternal);
            }
        }*/
    }

    //returns the parent of a node
    node * findparent(node *cursor,node *child)
    {
        node *parent;
        if(cursor->isleaf || cursor->child[0]->isleaf)
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

};

int main()
{
    rtree tree;
    coordinate p[]={coordinate(10,10),coordinate(20,50),coordinate(30,30),coordinate(40,60),coordinate(50,60)};
    tree.insert(&p[0]);
    tree.insert(&p[1]);
    tree.insert(&p[2]);
    tree.insert(&p[3]);
    tree.insert(&p[4]);
    cout<<tree.root->box->bottom[0]<<tree.root->box->bottom[1];
    cout<<tree.root->box->top[0]<<tree.root->box->top[1];
    return 1;
}
