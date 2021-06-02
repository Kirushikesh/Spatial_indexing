#include<iostream>
#include<cmath>
using namespace std;

const int M=3;

class coordinate
{
    public:

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

    float euclidean_dist(coordinate *another)
    {
        return sqrt(pow((x-another->x),2)+pow((y-another->y),2));
    }
};

int* pick_seed(coordinate **tot)
{
    int maxdis=0;
    int i,j,dis;
    int *seed=new int[2];

    for(i=0;i<M;i++)
    {
        for(j=i+1;j<M+1;j++)
        {
            dis=tot[i]->euclidean_dist(tot[j]);
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

    int new_point_loc(coordinate *co)
    {
        int x=co->x,y=co->y;
        if((x>=bottom[0] and x<=top[0]) and (y>=bottom[1] and y<=top[1]))
           return 0;
        else if(x<bottom[0] and y<bottom[1])
            return 1;
        else if((x>=bottom[0] and x<=top[0]) and y<bottom[1])
            return 2;
        else if(x>top[0] and y<bottom[1])
            return 3;
        else if(x>top[0] and (y>=bottom[1] and y<=top[1]))
            return 4;
        else if(x>top[0] and y>top[1])
            return 5;
        else if((x>=bottom[0] and x<=top[0]) and y>top[1])
            return 6;
        else if(x<bottom[0] and y>top[1])
            return 7;
        else
            return 8;
    }

    int area()
    {
        return (top[0]-bottom[0])*(top[1]-bottom[1]);
    }

    int area_enlargement(coordinate *newco)
    {
        int x=newco->x,y=newco->y;
        int check=new_point_loc(newco);
        switch(check)
        {
            case 1:
                return ((x-top[0])*(y-top[1]))-area();
            case 2:
                return ((bottom[0]-top[0])*(y-top[1]))-area();
            case 3:
                return ((bottom[0]-x)*(y-top[1]))-area();
            case 4:
                return ((bottom[0]-x)*(bottom[1]-top[1]))-area();
            case 5:
                return ((bottom[0]-x)*(bottom[1]-y))-area();
            case 6:
                return ((bottom[0]-top[0])*(bottom[1]-y))-area();
            case 7:
                return ((x-top[0])*(bottom[1]-y))-area();
            case 8:
                return ((x-top[0])*(bottom[1]-top[1]))-area();
            default:
                return 0;
        }
    }

    void calculate_new_mbb(mbb *ano)
    {
        if(bottom[0]>ano->bottom[0])
            bottom[0]=ano->bottom[0];
        if(bottom[1]>ano->bottom[1])
            bottom[1]=ano->bottom[1];
        if(top[0]<ano->top[0])
            top[0]=ano->top[0];
        if(top[1]<ano->top[1])
            top[1]=ano->top[1];
        //cout<<bottom[0]<<bottom[1]<<top[0]<<top[1]<<endl;
    }

    coordinate* box_midpoint()
    {
        return coordinate((bottom[0]+top[0]/2),(bottom[1]+top[1]/2));
    }
};

int* pick_next(coordinate **tot,mbb *box1,mbb *box2,bool *presence,int no1,int no2)
{
    int i,a1,a2,d1,d2;
    int *result=new int[2];

    for(i=0;i<M+1;i++)
    {
        if(presence[i]==false)
        {
            //cout<<"point "<<i<<"is observed now\n";
            d1=box1->area_enlargement(tot[i]);
            d2=box2->area_enlargement(tot[i]);
            //cout<<"d1 and d2 is "<<d1<<" "<<d2<<endl;
            if(d1<d2)
                result[0]=1;
            else if(d2<d1)
                result[0]=2;
            else
            {
                a1=box1->area();
                a2=box2->area();
                //cout<<"a1 and a2 is "<<a1<<" "<<a2<<endl;
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
    //cout<<"result is "<<result[0]<<'\t'<<result[1]<<endl;
    //cout<<"going to exit the pick next function\n";
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

    void insert_point(coordinate *co)
    {
        if(box==NULL)
            box=new mbb(co->x,co->y,co->x,co->y);
        points[no_points]=new coordinate(*co);
        handle_newpoint(co);
        no_points++;
    }

    void handle_newpoint(coordinate *co)
    {
        int x=co->x,y=co->y;
        int check=box->new_point_loc(co);
        switch(check)
        {
            case 1:
                box->bottom[0]=x;
                box->bottom[1]=y;
                break;
            case 2:
                box->bottom[1]=y;
                break;
            case 3:
                box->bottom[1]=y;
                box->top[0]=x;
                break;
            case 4:
                box->top[0]=x;
                break;
            case 5:
                box->top[0]=x;
                box->top[1]=y;
                break;
            case 6:
                box->top[1]=y;
                break;
            case 7:
                box->bottom[0]=x;
                box->top[1]=y;
                break;
            case 8:
                box->bottom[0]=x;
                break;
            default:
                // means the point is inside the bounding box
                break;
        }
    }

    coordinate** create_list(coordinate *next)
    {
        coordinate **temp=new coordinate*[M+1];
        for(int i=0;i<M;i++)
            temp[i]=points[i];
        temp[M]=next;
        return temp;
    }

    void insert_child(node *c)
    {
        child[no_childs]=new node(false);
        child[no_childs]=c;
        no_childs++;
    }

    void splitchild(int index,coordinate* co)
    {
        coordinate **all_points=child[0]->create_list(co);
        delete child[0];
        no_childs-=1;
        child[0]=NULL;

        bool presence[M+1];
        for(int i=0;i<M+1;i++)
            presence[i]=false;

        int *seeds=pick_seed(all_points);

        //cout<<endl<<all_points[seeds[0]]->x<<'\t'<<all_points[seeds[0]]->y<<endl;
        //cout<<all_points[seeds[1]]->x<<'\t'<<all_points[seeds[1]]->y<<endl;

        node *node1=new node(true),*node2=new node(true);
        node1->insert_point(all_points[seeds[0]]);
        node2->insert_point(all_points[seeds[1]]);

        presence[seeds[0]]=true;
        presence[seeds[1]]=true;

        int lambda=M-1,no1=1,no2=1,*next;
        while(lambda>0)
        {
            //cout<<no1<<no2<<endl;
            if(no1==(ceil((float)M/2)-lambda))
            {
                for(int i=0;i<M+1;i++)
                    if(presence[i]==false)
                        node1->insert_point(all_points[i]);
                break;
            }
            else if(no2==(ceil((float)M/2)-lambda))
            {
                for(int i=0;i<M+1;i++)
                    if(presence[i]==false)
                        node2->insert_point(all_points[i]);
                break;
            }
            else
            {
                next=pick_next(all_points,node1->box,node2->box,presence,no1,no2);
                //cout<<"next is "<<next[0]<<'\t'<<next[1]<<endl;
                if(next[0]==1)
                {
                    node1->insert_point(all_points[next[1]]);
                    no1++;
                }
                else
                {
                    node2->insert_point(all_points[next[1]]);
                    no2++;
                }
                presence[next[1]]=true;
                lambda--;
            }
        }
        //cout<<node1->no_points<<node2->no_points<<endl;
        insert_child(node1);
        insert_child(node2);
        box=new mbb(node1->box->bottom[0],node1->box->bottom[1],node1->box->top[0],node1->box->top[1]);
        box->calculate_new_mbb(node2->box);
    }

    coordinate** create_box_list(mbb *ano)
    {
        coordinate **temp=new coordinate*[M+1];
        for(int i=0;i<M;i++)
            temp[i]=child[i]->box->box_midpoint();
        temp[M]=ano->box_midpoint();
        return temp;
    }
};

class rtree
{
    public:

    node *root;

    node *insert_rec(node *root,coordinate *co)
    {
        //cout<<"inside rec function"<<endl;
        if(root->isleaf==true)
        {
            // The current node is a leaf node
            if(root->no_points<M)
            {
                cout<<"inside if"<<endl;
                root->insert_point(co);
                //cout<<"no of points present is "<<root->no_points<<endl;
                //cout<<root->box->bottom[0]<<root->box->bottom[1];
                //cout<<root->box->top[0]<<root->box->top[1];
                return root;
            }
            else
            {
                cout<<"inside else";
                node *n=new node(false);
                n->insert_child(root);
                n->splitchild(0,co);
                //cout<<endl<<n->child[0]->box->area()<<endl;
                //cout<<n->child[1]->box->area()<<endl;
                //cout<<endl<<n->child[0]->box->bottom[0]<<n->child[0]->box->bottom[1];
                //cout<<n->child[0]->box->top[0]<<n->child[0]->box->top[1]<<endl;

                //cout<<endl<<n->child[1]->box->bottom[0]<<n->child[1]->box->bottom[1];
                //cout<<n->child[1]->box->top[0]<<n->child[1]->box->top[1]<<endl;
                return n;
            }
        }
        else
        {
            cout<<"\nvery outer else\n";
            //cout<<root->no_childs<<endl;
            int i,minnode,minarea=999,temp;
            for(i=0;i<root->no_childs;i++)
            {
                temp=root->child[i]->box->area_enlargement(co);
                if(minarea>temp)
                {
                    minarea=temp;
                    minnode=i;
                }
            }
            root->child[minnode]=insert_rec(root->child[minnode],co);
            //cout<<root->child[0]->no_points<<endl;
            //cout<<root->child[1]->no_points<<endl;
            root->box->calculate_new_mbb(root->child[minnode]->box);
            //cout<<root->child[0]->box->area()<<root->child[1]->box->area();
            return root;
        }
    }

    public:
    rtree()
    {
        root=NULL;
    }

    void insert(coordinate *p)
    {
        if(root==NULL)
            root=new node(true);
        insert_rec(root,p);
    }

    void insert_new(coordinate *co)
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
                parent->handle_newpoint(co);
                int i,minnode,minarea=999,temp;

                for(i=0;i<cursor->no_childs;i++)
                {
                    temp=cursor->child[i]->box->area_enlargement(co);
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
                coordinate **all_points=cursor->create_list(co);
                cursor->no_points=0;
                cursor->box=NULL;

                bool presence[M+1];
                for(int i=0;i<M+1;i++)
                    presence[i]=false;

                int *seeds=pick_seed(all_points);
                cursor->insert_point(all_points[seeds[0]]);
                newleaf->insert_point(all_points[seeds[1]]);

                presence[seeds[0]]=true;
                presence[seeds[1]]=true;

                int lambda=M-1,no1=1,no2=1,*next;
                while(lambda>0)
                {
                    //cout<<no1<<no2<<endl;
                    if(no1==(ceil((float)M/2)-lambda))
                    {
                        for(int i=0;i<M+1;i++)
                            if(presence[i]==false)
                                cursor->insert_point(all_points[i]);
                        break;
                    }
                    else if(no2==(ceil((float)M/2)-lambda))
                    {
                        for(int i=0;i<M+1;i++)
                            if(presence[i]==false)
                                newleaf->insert_point(all_points[i]);
                        break;
                    }
                    else
                    {
                        next=pick_next(all_points,cursor->box,newleaf->box,presence,no1,no2);
                        //cout<<"next is "<<next[0]<<'\t'<<next[1]<<endl;
                        if(next[0]==1)
                        {
                            cursor->insert_point(all_points[next[1]]);
                            no1++;
                        }
                        else
                        {
                            newleaf->insert_point(all_points[next[1]]);
                            no2++;
                        }
                        presence[next[1]]=true;
                        lambda--;
                    }
                }

                if(cursor==root)
                {
                    node *newroot=new node(false);
                    newroot->insert_child(cursor);
                    newroot->insert_child(newleaf);
                    newroot->box=new mbb(cursor->box->bottom[0],cursor->box->bottom[1],cursor->box->top[0],cursor->box->top[1]);
                    newroot->box->calculate_new_mbb(newleaf->box);
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
    {
        if(cursor->no_childs<M)
        {
            cursor->insert_child(child);
            cursor->box->calculate_new_mbb(child->box);
        }
        else
        {
            node *newinternal=new node(false);
            coordinate **all_points=cursor->create_box_list(child->box);
            cursor->no_childs=0;
            cursor->box=NULL;

            bool presence[M+1];
            for(int i=0;i<M+1;i++)
                presence[i]=false;

            int *seeds=pick_seed(all_points);
            cursor->insert_point(all_points[seeds[0]]);
            newleaf->insert_point(all_points[seeds[1]]);

            presence[seeds[0]]=true;
            presence[seeds[1]]=true;
        }
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

