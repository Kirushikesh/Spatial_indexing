#ifndef OPENGL_H_INCLUDED
#define OPENGL_H_INCLUDED

node *mainroot=NULL;

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
                glColor3f(0,1,0);
                glBegin(GL_POINTS);
                    glVertex2f(temp->leafs[i]->box->bottom->x,temp->leafs[i]->box->bottom->y);
                    //glVertex2f(100+temp->leafs[i]->box->top->x,100+temp->leafs[i]->box->bottom->y);
                    //glVertex2f(100+temp->leafs[i]->box->top->x,100+temp->leafs[i]->box->top->y);
                    //glVertex2f(100+temp->leafs[i]->box->bottom->x,100+temp->leafs[i]->box->top->y);
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
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
	{
	    case 'w':
            glTranslatef(0.0, 0.0, 5.0);
            break;
        case 's':
            glTranslatef(0.0, 0.0, -5.0);
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

    gluPerspective(80.0, width / height, 1.0, 600.0);

	glTranslatef(-250.0, -250.0, -300.0);

	glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        cout<<"Left is clicked\n";
    }
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
    glutMouseFunc(mouse);
    glutDisplayFunc(myDisplay);
    myInit();
    glutReshapeFunc(resize);
    glutMainLoop();
}

#endif // OPENGL_H_INCLUDED
