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

    //computes the squared euclidean distance between this and another point
    int squ_euclidean(coordinate *ano)
    {
        return pow(ano->x-x,2)+pow(ano->y-y,2);
    }

};

//adds a new coordinate into the results list based on insertion sort method
void add_knn(coordinate *newpnt,int distance,coordinate** results,int dis[],int no,int k)
{
    if(no==k)
    {
        int i=no-2;
        while(distance<dis[i] and i>=0)
        {
            dis[i+1]=dis[i];
            results[i+1]=results[i];
            i--;
        }
        results[i+1]=newpnt;
        dis[i+1]=distance;
    }
    else
    {
        int i=no-1;
        while(distance<dis[i] and i>=0)
        {
            dis[i+1]=dis[i];
            results[i+1]=results[i];
            i--;
        }
        results[i+1]=newpnt;
        dis[i+1]=distance;
    }
}

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

    //calculates the area of the bounding box
    int area()
    {
        return (top->x-bottom->x)*(top->y-bottom->y);
    }

    //calculate the overlapping area btwn 2 boxes
    int overlapping_area(mbb* ano)
    {
        int x_dist=min(top->x,ano->top->x)-max(bottom->x,ano->bottom->x);
        int y_dist=min(top->y,ano->top->y)-max(bottom->y,ano->bottom->y);
        int area=0;
        if(x_dist>0 && y_dist>0)
            area=x_dist*y_dist;
        return area;
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

    //returns the area enlargement when we try to add the box within this box
    int area_enlargement(mbb *ano)
    {
        int totalarea=(max(top->x,ano->top->x)-min(bottom->x,ano->bottom->x))*((max(top->y,ano->top->y)-min(bottom->y,ano->bottom->y)));
        return totalarea+overlapping_area(ano)-area()-ano->area();
    }

    //determine whether this rectangle touch with other rectangle
    bool touch_box(mbb *box1)
    {
        //checks for touches from the top
        if(bottom->y==box1->top->y)
        {
            if(!(top->x<box1->bottom->x or bottom->x>box1->top->x))
                return true;
        }
        //checks for touches from the left
        if(box1->bottom->x==top->x)
        {
            if(!(top->y<box1->bottom->y or bottom->y>box1->top->y))
                return true;
        }
        //checks for touches from the right
        if(box1->top->x==bottom->x)
        {
            if(!(top->y<box1->bottom->y or bottom->y>box1->top->y))
                return true;
        }
        //checks for touches from the bottom
        if(top->y==box1->bottom->y)
        {
            if(!(top->x<box1->bottom->x or bottom->x>box1->top->x))
                return true;
        }
        return false;
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
};

//convert a point to box
mbb *point_to_box(coordinate *co)
{
    mbb *newbox=new mbb(co,co);
    return newbox;
}

//convert a box to coordinate
coordinate* box_to_point(mbb *mo)
{
    coordinate *newco= new coordinate(mo->bottom->x,mo->bottom->y);
    return newco;
}

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
            box=new mbb(co,co);
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

    //searchs the point present within a query box coordinates
    void range_search_helper(mbb *query)
    {
        if(isleaf==false)
        {
            for(int i=0;i<no_childs;i++)
            {
                if(child[i]->box->overlapping_area(query)>0 or query->touch_box(child[i]->box))
                    child[i]->range_search_helper(query);
            }
        }
        else
        {
            for(int i=0;i<no_points;i++)
            {
                if(points[i]->x >=query->bottom->x and points[i]->x <=query->top->x)
                {
                    if(points[i]->y >=query->bottom->y and points[i]->y <=query->top->y)
                    {
                        cout<<"( "<<points[i]->x<<" , "<<points[i]->y<<" ) ,";
                    }
                }
            }
        }
    }

    //finds the knn based on the query point (declared outside)
    int knn_helper(coordinate **results,int dis[],coordinate *query,int cur,int k);

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
int node::knn_helper(coordinate **results,int dis[],coordinate *query,int cur,int k)
{
    int i=0,d;
    if(isleaf==true)
    {
        for(i=0;i<no_points;i++)
        {
            d=query->squ_euclidean(points[i]);
            if(d<=dis[cur-1])
                add_knn(points[i],d,results,dis,cur,k);
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
            if(mindis[i]<=dis[cur-1] or cur==0)
            {
                cur=branchlist[i]->knn_helper(results,dis,query,cur,k);
            }
        }
    }
    return cur;
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

    //helper of insert function inserts a particular node in another node
    void insert_helper(node *cursor,node *child)
    {
        if(cursor->no_childs<M)
        {
            cursor->insert_child(child,child->isleaf);
        }
        else
        {
            int i;
            node *newinternal=new node(false);
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
            for(int i=0;i<M+1;i++)
                presence[i]=false;

            int *seeds=pick_seed(all_boxes);
            cursor->insert_child(virtualchild[seeds[0]],virtualchild[seeds[0]]->isleaf);
            newinternal->insert_child(virtualchild[seeds[1]],virtualchild[seeds[1]]->isleaf);

            presence[seeds[0]]=true;
            presence[seeds[1]]=true;

            int no1=1,no2=1,*next;
            while(no1+no2<M+1)
            {
                next=pick_next(all_boxes,cursor->box,newinternal->box,presence,no1,no2);
                if(next[0]==1)
                {
                    cursor->insert_child(virtualchild[next[1]],virtualchild[next[1]]->isleaf);
                    no1++;
                }
                else
                {
                    newinternal->insert_child(virtualchild[next[1]],virtualchild[next[1]]->isleaf);
                    no2++;
                }
                presence[next[1]]=true;
            }

            if(cursor==root)
            {
                node *newroot=new node(false);
                newroot->insert_child(cursor,false);
                newroot->insert_child(newinternal,false);
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

    //finds all the points inside a box m
    void range_search(mbb *m)
    {
        if(root->box->overlapping_area(m)>0 or root->no_points==1 or m->touch_box(root->box))
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
        coordinate **results=new coordinate*[k];
        int dis[k],no=0;
        for(int i=0;i<k;i++)
        {
            results[i]=NULL;
            dis[i]=99999;
        }

        no=root->knn_helper(results,dis,query,no,k);
        for(int i=0;i<no;i++)
        {
            cout<<"Point is ("<<results[i]->x<<" , "<<results[i]->y<<" ) and distance is "<<dis[i]<<endl;
        }
    }
};

int main()
{
    rtree tree;
    coordinate p[]={coordinate(10,10),coordinate(20,50),coordinate(30,30),coordinate(40,60),coordinate(50,60),coordinate(0,10),
                    coordinate(10,20),coordinate(30,60),coordinate(5,5)};
    for(int i=0;i<9;i++)
        tree.insert(&p[i]);
    cout<<tree.root->box->bottom->x<<tree.root->box->bottom->y;
    cout<<tree.root->box->top->x<<tree.root->box->top->y<<endl;

    mbb *m=new mbb(40,40,60,60);
    tree.knn(3,&coordinate(10,10));
    tree.range_search(&mbb(0,0,20,20));
    //cout<<tree.root->child[0]->child[0]->no_childs<<tree.root->child[0]->child[0]->no_points<<endl;
    //cout<<tree.root->child[0]->child[0]->box->bottom->x<<tree.root->child[0]->child[0]->box->bottom->y;
    //cout<<tree.root->child[0]->child[0]->box->top->x<<tree.root->child[0]->child[0]->box->top->y;
    return 1;
}
//Add complex objects in r trees.
