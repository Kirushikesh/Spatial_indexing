#ifndef OPENGL_H_INCLUDED
#define OPENGL_H_INCLUDED

#include<windows.h>
#include <GL/glut.h>
#include<queue>
#include<fstream>
#include<math.h>
#include<vector>
#include <sstream>

node *mainroot=NULL;

void draw_polygon(int nop,coordinate **co)
{
    if(nop==1)
    {
        glColor3f(0,1,0);
        glBegin(GL_POINTS);
            glVertex2f(co[0]->x,co[0]->y);
        glEnd();
    }
    else if(nop==2)
    {
        glColor3f(0,1,0);
        glBegin(GL_LINES);
            glVertex2f(co[0]->x,co[0]->y);
            glVertex2f(co[1]->x,co[1]->y);
        glEnd();
    }
    else
    {
        glColor3f(0,1,0);
        glBegin(GL_LINE_LOOP);
            for(int i=0;i<nop;i++)
                glVertex2f(co[i]->x,co[i]->y);
        glEnd();
    }
    glFlush();
}

void draw_circle(coordinate *co,int radius)
{
    glColor3f(0,1,0);
    float angle;
    glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 100; i++)
        {
            angle = i*2*(3.14/100);
            glVertex2f(co->x+(cos(angle)*radius),co->y+(sin(angle)*radius));
        }
    glEnd();
    glFlush();
}

void get_object_data()
{
    int nop,p1,p2,i,radius;
    string line,word;
    vector<string> row;
    stringstream geek;
    coordinate **co;

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

            co[(i-3)/2]=new coordinate(p1,p2);
        }

        if(row[1].compare("circle")==0)
        {
            geek<<row[i];
            geek >> radius;
            geek.clear();

            draw_circle(co[0],radius);
        }
        else
        {
            draw_polygon(nop,co);
        }
    }
    fio.close();
}

void myInit()
{
    glClearColor(1.0,1.0,1.0,0.0);
    glColor3f(0.0,0.0,0.0);
    glPointSize(4);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,640.0,0.0,500.0);
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);

    int i;
    node* temp;
    queue<node*> Q;
    Q.push(mainroot);

    while(!Q.empty())
    {
        temp=Q.front();

        glColor3f(0,0,1);
        glBegin(GL_LINE_LOOP);
            glVertex2f(temp->box->bottom->x,temp->box->bottom->y);
            glVertex2f(temp->box->top->x,temp->box->bottom->y);
            glVertex2f(temp->box->top->x,temp->box->top->y);
            glVertex2f(temp->box->bottom->x,temp->box->top->y);
        glEnd();
        Q.pop();

        if(temp->isleaf==true)
        {
            for(i=0;i<temp->no_leafs;i++)
            {
                glColor3f(0,0,1);
                glBegin(GL_LINE_LOOP);
                    glVertex2f(temp->leafs[i]->box->bottom->x,temp->leafs[i]->box->bottom->y);
                    glVertex2f(temp->leafs[i]->box->top->x,temp->leafs[i]->box->bottom->y);
                    glVertex2f(temp->leafs[i]->box->top->x,temp->leafs[i]->box->top->y);
                    glVertex2f(temp->leafs[i]->box->bottom->x,temp->leafs[i]->box->top->y);
                glEnd();
            }
        }
        else
        {
            for(i=0;i<temp->no_childs;i++)
                Q.push(temp->child[i]);
        }
    }

    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(mainroot->box->bottom->x,mainroot->box->bottom->y);
        glVertex2f(mainroot->box->top->x,mainroot->box->bottom->y);
        glVertex2f(mainroot->box->top->x,mainroot->box->top->y);
        glVertex2f(mainroot->box->bottom->x,mainroot->box->top->y);
    glEnd();
    glFlush();
    get_object_data();

}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
	{
	    case 'w':
            glTranslatef(0.0, 0.0, 3.0);
            break;
        case 's':
            glTranslatef(0.0, 0.0, -3.0);
            break;
	}
	glutPostRedisplay();
}

void SpecialInput(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_UP:
            glTranslatef(0.0, -5.0, 0.0);
            break;
        case GLUT_KEY_DOWN:
            glTranslatef(0.0, 5.0, 0.0);
            break;
        case GLUT_KEY_LEFT:
            glTranslatef(5.0, 0.0, 0.0);
            break;
        case GLUT_KEY_RIGHT:
            glTranslatef(-5.0, 0.0, 0.0);
            break;
    }
    glutPostRedisplay();
}

void resize(int width, int height)
{
	if (height == 0) height = 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 0.1, 1000.0);

	glTranslatef(-250.0, -250.0, -300.0);

	glMatrixMode(GL_MODELVIEW);
}

void Visualize_tree(node *root,int argc,char **argv)
{
    mainroot=root;

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(640,500);
    glutInitWindowPosition(100,150);
    glutCreateWindow("Visualization of Rtree");
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialInput);
    glutDisplayFunc(myDisplay);
    myInit();
    glutReshapeFunc(resize);
    glutMainLoop();
}

#endif // OPENGL_H_INCLUDED
