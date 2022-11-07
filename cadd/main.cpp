#include <iostream>
#include <Gl/glut.h>
#include <half_edge_structure.h>
#define PI 3.1415926536
#include <GLFW/glfw3.h>

using namespace std;
double xrot = 0.0;
double yrot = 0.0;
double xrotr = 0.0;
double yrotr = 0.0;
bool displaymode = true;
int xp, yp;
bool bdown = false;
BOOL FaceToDraw = true;
BOOL VertToDraw = FALSE;
GLUtesselator* obj = gluNewTess();;
static BOOL shapeMode = FALSE;
int Interactive() {
	cout << "************************三维CAD作业**********************" << endl;
	cout << "本程序提供两个预设DEMO,DEMO1是利用欧拉操作实现带通孔立方体。" << endl;
	cout << "DEMO2是先利用欧拉操作实现带通孔的二维平面，然后利用SWEEP扫掠生成模型" << endl;
	cout << "输入1则生成DEMO1，输入2则生成DEMO2，输入3则生成用户自定义模型，用户自定义需要用户在USER函数中添加" << endl;
	int num;
	cin >> num;
	return num;
}


void GLInit(){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	obj = gluNewTess();
	gluTessCallback(obj, GLU_TESS_BEGIN, (void (CALLBACK*)())glBegin);
	gluTessCallback(obj, GLU_TESS_END, (void (CALLBACK*)())glEnd);
	gluTessCallback(obj, GLU_TESS_VERTEX, (void (CALLBACK*)())glVertex4dv);
	//gluTessCallback(obj, GLU_TESS_ERROR, (void (CALLBACK*)())ErrorCallback);
}


void DEMO1()
{
	ClearSolidList();
	Solid* solid = mvfs(1, 1, 1, -1, -1, 1);
	AddToSolidList(solid);
	smev(1, 1, 1, 2, 1, -1, 1);
	smev(1, 1, 2, 3, 1, 1, 1);
	smev(1, 1, 3, 4, -1, 1, 1);
	smef(1, 1, 4, 1, 2);
	smev(1, 2, 1, 5, -1, -1, -1);
	smev(1, 2, 2, 6, 1, -1, -1);
	smev(1, 2, 3, 7, 1, 1, -1);
	smev(1, 2, 4, 8, -1, 1, -1);
	smef(1, 2, 7, 8, 3);
	smef(1, 3, 6, 7, 4);
	smef(1, 4, 5, 6, 5);
	smef(1, 5, 8, 5, 6);
	smev(1, 2, 4, 9, -0.5, 1.0, 0.5);
	smev(1, 2, 9, 10, 0.5, 1.0, 0.5);
	smev(1, 2, 10, 11, 0.5, 1.0, -0.5);
	smev(1, 2, 11, 12, -0.5, 1.0, -0.5);
	smef(1, 2, 12, 9, 7);
	skemr(1, 7, 4, 9);
	smev(1, 2, 9, 13, -0.5, -1.0, 0.5);
	smev(1, 2, 10, 14, 0.5, -1.0, 0.5);
	smev(1, 2, 11, 15, 0.5, -1.0, -0.5);
	smev(1, 2, 12, 16, -0.5, -1.0, -0.5);
	smef(1, 2, 13, 14, 8);
	smef(1, 2, 14, 15, 9);
	smef(1, 2, 15, 16, 10);
	smef(1, 2, 13, 16, 11);
	kfmrh(1, 4, 11);
	solidlist->Print();
}
void DEMO2()
{
	ClearSolidList();
	Solid* solid = mvfs(1, 1, 1, -1.6, -1, 1.6);
	AddToSolidList(solid);
	smev(1, 1, 1, 2, 1.6, -1, 1.6);
	smev(1, 1, 2, 3, 1.6, -1, -1.6);
	smev(1, 1, 3, 4, -1.6, -1, -1.6);
	smef(1, 1, 4, 1, 2);
	smev(1, 1, 1, 5, -1.2, -1, 1.2);
	smev(1, 1, 5, 6, -0.4, -1, 1.2);
	smev(1, 1, 6, 7, -0.4, -1, 0.4);
	smev(1, 1, 7, 8, -1.2, -1, 0.4);
	smef(1, 1, 8, 5, 3);
	skemr(1, 3, 1, 5);
	smev(1, 3, 1, 9, 0.4, -1, 1.2);
	smev(1, 3, 9, 10, 1.2, -1, 1.2);
	smev(1, 3, 10, 11, 1.2, -1, 0.4);
	smev(1, 3, 11, 12, 0.4, -1, 0.4);
	smef(1, 3, 12, 9, 4);
	skemr(1, 4, 1, 9);
	smev(1, 4, 1, 13, 0.4, -1, -0.4);
	smev(1, 4, 13, 14, 1.2, -1, -0.4);
	smev(1, 4, 14, 15, 1.2, -1, -1.2);
	smev(1, 4, 15, 16, 0.4, -1, -1.2);
	smef(1, 4, 16, 13, 5);
	skemr(1, 5, 1, 13);
	smev(1, 5, 1, 17, -1.2, -1, -0.4);
	smev(1, 5, 17, 18, -0.4, -1, -0.4);
	smev(1, 5, 18, 19, -0.4, -1, -1.2);
	smev(1, 5, 19, 20, -1.2, -1, -1.2);
	smef(1, 5, 20, 17, 6);
	skemr(1, 6, 1, 17);
	kfmrh(1, 2, 1);
	kfmrh(1, 2, 3);
	kfmrh(1, 2, 5);
	kfmrh(1, 2, 4);
	sweep(1, 2, 0, 2, 0);
	solidlist->Print();
	
}
void USER() {

}

void DrawFace(ID Fsid, ID Ffid)
{
	Solid* solid;
	Face* face;
	Loop* lp;
	HalfEdge* he;
	static double change = 0.0;
	if ((solid = GetSolidFromID(Fsid)) == NULL)
	{
		MessageBox(NULL, TEXT("Can't Find The Solid"), TEXT("ERROR:Display Face"), MB_ICONERROR);
		FaceToDraw = FALSE;
		return;
	}
	if ((face = findface(solid, Ffid)) == NULL)
	{
		MessageBox(NULL, TEXT("Face  Not Find In solid"), TEXT("ERROR:Display Face"), MB_ICONERROR);
		FaceToDraw = FALSE;
		return;
	}
	lp = face->floopout;
	he = lp->helist;
	//glColor3f(0.2, 0.6, 0.8);蓝色
	glColor3f(0.9, 0.3, 0.2);
	gluTessBeginPolygon(obj, NULL);
	gluTessBeginContour(obj);
	while (he)
	{
		gluTessVertex(obj, he->vtx->coord, he->vtx->coord);
		he = he->next;
		if (he == lp->helist)
		{
			break;
		}
	}
	gluTessEndContour(obj);
	lp = face->floops;
	while (lp)
	{
		gluTessBeginContour(obj);
		he = lp->helist;
		while (he)
		{
			gluTessVertex(obj, he->vtx->coord, he->vtx->coord);
			he = he->next;
			if (he == lp->helist)
			{
				break;
			}
		}
		gluTessEndContour(obj);
		lp = lp->next;
		if (lp == face->floops)
		{
			break;
		}
	}
	gluTessEndPolygon(obj);
}
void DrawVertex(ID sid, ID vid)
{
	Solid* solid;
	Vertex* v;
	static double change = 0;
	if ((solid = GetSolidFromID(sid)) == NULL)
	{
		MessageBox(NULL, TEXT("Can't Find The Solid"), TEXT("ERROR:Display Vertex"), MB_ICONERROR);
		return;
	}
	if ((v = findvertex(solid, vid)) == NULL)
	{
		MessageBox(NULL, TEXT("Vertex Not Found In Solid."), TEXT("ERROR:Display Vertex"), MB_ICONERROR);
		return;
	}
	glColor3d(1.0, 1.0, 0.0);
	glPointSize(abs(sin(change)) * 10.0);
	change += 0.05;
	glBegin(GL_POINTS);
	glVertex3d(v->coord[0], v->coord[1], v->coord[2]);
	glEnd();
}
void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();
	glTranslatef(-0.0, 0.0, -10.0);
	glRotatef(xrot + xrotr, 1.0, 0.0, 0.0);
	glRotatef(yrot + yrotr, 0.0, 1.0, 0.0);
	Solid* tmp = solidlist;
	Vertex* v;
	Edge* e;
	Face* face;
	Loop* lp;
	HalfEdge* he;
	if (shapeMode) {
		//draw faces
		glColor3f(1.0, 0.0, 1.0);
		while (tmp)
		{
			face = tmp->facelist;
			while (face)
			{
				lp = face->floopout;
				he = lp->helist;
				gluTessBeginPolygon(obj, NULL);
				gluTessBeginContour(obj);
				while (he)
				{
					gluTessVertex(obj, he->vtx->coord, he->vtx->coord);
					he = he->next;
					if (he == lp->helist)
					{
						break;
					}
				}
				gluTessEndContour(obj);
				lp = face->floops;
				while (lp)
				{
					gluTessBeginContour(obj);
					he = lp->helist;
					while (he)
					{
						gluTessVertex(obj, he->vtx->coord, he->vtx->coord);
						he = he->next;
						if (he == lp->helist)
						{
							break;
						}
					}
					gluTessEndContour(obj);
					lp = lp->next;
					if (lp == face->floops)
					{
						break;
					}
				}
				face = face->next;
				gluTessEndPolygon(obj);
				if (face == tmp->facelist)
				{
					break;
				}
			}
			tmp = tmp->next;
			if (tmp == solidlist)
			{
				break;
			}

		}
	}
	if (FaceToDraw) {
		tmp = solidlist;
		while (tmp) {
			int globsid = tmp->sID;
			face = tmp->facelist;
			while (face) {
				int globfid = face->fID;
				DrawFace(globsid, globfid);
				face = face->next;
				if (face == tmp->facelist) {
					break;
				}
			}
			tmp = tmp->next;
			if (tmp == solidlist) {
				break;
			}
		}
	}
	

	//if (FaceToDraw)DrawFace(globsid, globfid);
	//draw lines
	glColor3f(1.0, 1.0, 1.0);
	tmp = solidlist;
	glBegin(GL_LINES);
	while (tmp)
	{
		e = tmp->edgelist;
		while (e)
		{
			glVertex3d(e->lhe->vtx->coord[0], e->lhe->vtx->coord[1], e->lhe->vtx->coord[2]);
			glVertex3d(e->rhe->vtx->coord[0], e->rhe->vtx->coord[1], e->rhe->vtx->coord[2]);
			e = e->next;
			if (e == tmp->edgelist)
			{
				break;
			}
		}
		tmp = tmp->next;
		if (tmp == solidlist)
		{
			break;
		}
	}
	glEnd();
	//display vertices
	tmp = solidlist;
	glPointSize(4);
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POINTS);
	while (tmp)
	{
		v = tmp->vtxlist;
		while (v)
		{
			glVertex3d(v->coord[0], v->coord[1], v->coord[2]);
			v = v->next;
			if (v == tmp->vtxlist)
			{
				break;
			}
		}
		tmp = tmp->next;
		if (tmp == solidlist)
		{
			break;
		}
	}
	glEnd();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (double)w / (double)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			bdown = true;
			xp = x;
			yp = y;
		}
		else
		{
			bdown = false;
			xrot += xrotr;
			yrot += yrotr;
		}
	}
}
void move(int x, int y)
{
	int deltx, delty;
	if (bdown)
	{
		deltx = x - xp;
		delty = y - yp;
		yrotr = ((double)deltx / 10.0 * 180.0 / PI * 0.04);
		xrotr = ((double)delty / 10.0 * 180.0 / PI * 0.04);
	}
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	glutCreateWindow("XDD的程序");
	GLInit();
	int num = Interactive();
	if (num == 1) DEMO1();
	else if (num == 2) DEMO2();
	else if (num == 3) USER();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(move);
	glutMainLoop();

	return 0;
}