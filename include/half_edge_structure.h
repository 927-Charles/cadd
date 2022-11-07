#ifndef _HALF_EDGE_STRUCTURE_H
#define _HALF_EDGE_STRUCTURE_H
#define PLUSH -1
#define MINUS  1
#define mate(he) (((he) == (he)->edge->lhe)?(he)->edge->rhe:(he)->edge->lhe)
#include<cstdio>
#include<vector>
#include<iostream>
using namespace std;
typedef double Vec4f[4];
typedef int ID;
static int cur_ids = 1;
static int cur_idf = 1;
static int cur_idv = 1;
static int GenIDs()
{
	return cur_ids++;
}
static int GenIDf()
{
	return cur_idf++;
}
static int GenIDv()
{
	return cur_idv++;
}
//构建半边数据结构
class Vertex;
class HalfEdge;
class Loop;
class Edge;
class Face;
class Solid;
class Vertex
{
public:
	Vertex(ID id, double x, double y, double z)
	{
		this->vID = id;
		coord[0] = x; coord[1] = y; coord[2] = z;
		coord[3] = 1.0;
		this->prev = this;
		this->next = this;
		this->he = NULL;
	}
	ID  vID;
	Vec4f coord;
	Vertex* prev;
	Vertex* next;
	HalfEdge* he;
};
class HalfEdge
{
public:
	HalfEdge(Vertex* v)
	{
		this->vtx = v;
		this->prev = this;
		this->next = this;
		this->edge = NULL;
		this->loop = NULL;
	}
	HalfEdge* prev;
	HalfEdge* next;
	Vertex* vtx;
	Loop* loop;
	Edge* edge;
};
class Edge
{
public:
	Edge()
	{
		this->prev = this;
		this->next = this;
		this->lhe = NULL;
		this->rhe = NULL;
	}
	void ClearHalfEdge()
	{
		delete lhe;
		delete rhe;
	}
	Edge* prev;
	Edge* next;
	HalfEdge* lhe;
	HalfEdge* rhe;
};
class Loop
{
public:
	Loop()
	{
		this->prev = this;
		this->next = this;
		this->helist = NULL;
		this->face = NULL;
	}
	Loop* prev;
	Loop* next;
	HalfEdge* helist;
	Face* face;
};
class Face
{
public:
	Face(ID id)
	{
		this->fID = id;
		this->prev = this;
		this->next = this;
		this->solid = NULL;
		this->floopout = NULL;
		this->floops = NULL;
	}
	void ClearLoops()
	{
		Loop* lastl;
		if (floopout != NULL)
		{
			delete floopout;
		}
		if (floops == NULL)
		{
			return;
		}
		else
		{
			lastl = floops->prev;
			while (lastl != floops)
			{
				lastl = lastl->prev;
				delete lastl->next;
			}
			delete lastl;
			floops = NULL;
		}
	}
	void AddToLoops(Loop* tp)
	{
		if (this->floops == NULL)
		{
			this->floops = tp;
		}
		else
		{
			tp->next = floops;
			tp->prev = this->floops->prev;
			this->floops->prev->next = tp;
			this->floops->prev = tp;
		}
	}
	ID fID;
	Face* prev;
	Face* next;
	Solid* solid;
	Loop* floopout;
	Loop* floops;
};
class Solid
{
public:
	Solid(ID id)
	{
		this->sID = id;
		this->prev = this;
		this->next = this;
		this->vtxlist = NULL;
		this->facelist = NULL;
		this->edgelist = NULL;
	}
	void AddVtx(Vertex* v)
	{
		if (this->vtxlist == NULL)
		{
			this->vtxlist = v;
		}
		else
		{
			v->next = this->vtxlist;
			v->prev = this->vtxlist->prev;
			this->vtxlist->prev->next = v;
			this->vtxlist->prev = v;
		}
	}
	void AddFace(Face* f)
	{
		if (this->facelist == NULL)
		{
			this->facelist = f;
		}
		else
		{
			f->next = this->facelist;
			f->prev = this->facelist->prev;
			this->facelist->prev->next = f;
			this->facelist->prev = f;
		}
	}
	void AddEdge(Edge* e)
	{
		if (this->edgelist == NULL)
		{
			this->edgelist = e;
		}
		else
		{
			e->next = this->edgelist;
			e->prev = this->edgelist->prev;
			this->edgelist->prev->next = e;
			this->edgelist->prev = e;
		}
	}
	Vertex* DelVtx(Vertex* v)
	{
		Vertex* tmp;
		tmp = this->vtxlist;
		while (tmp)
		{
			if (tmp == v)
			{
				tmp->next->prev = tmp->prev;
				tmp->prev->next = tmp->next;
				return tmp;
			}
			tmp = tmp->next;
			if (tmp == this->vtxlist)
			{
				break;
			}
		}
		return NULL;
	}
	Face* DelFace(Face* f)
	{
		Face* tmp;
		tmp = this->facelist;
		while (tmp)
		{
			if (tmp == f)
			{
				tmp->next->prev = tmp->prev;
				tmp->prev->next = tmp->next;
				if (f == this->facelist)
				{
					this->facelist = tmp->next;
				}
				return tmp;
			}
			tmp = tmp->next;
			if (tmp == this->facelist)
			{
				break;
			}
		}
		return NULL;
	}
	Edge* DelEdge(Edge* e)
	{
		Edge* tmp;
		tmp = this->edgelist;
		while (tmp)
		{
			if (tmp == e)
			{
				tmp->next->prev = tmp->prev;
				tmp->prev->next = tmp->next;
				return tmp;
			}
			tmp = tmp->next;
			if (tmp == this->edgelist)
			{
				break;
			}
		}
		return NULL;
	}
	void ClearFaceList()
	{
		Face* lastf;
		if (facelist == NULL)
		{
			return;
		}
		else
		{
			lastf = facelist->prev;
			while (lastf != facelist)
			{
				lastf = lastf->prev;
				lastf->next->ClearLoops();
				delete lastf->next;
			}
			lastf->ClearLoops();
			delete lastf;
			facelist = NULL;
		}
	}
	void ClearEdgeList()
	{
		Edge* laste;
		if (edgelist == NULL)
		{
			return;
		}
		else
		{
			laste = edgelist->prev;
			while (laste != edgelist)
			{
				laste = laste->prev;
				laste->next->ClearHalfEdge();
				delete laste->next;
			}
			laste->ClearHalfEdge();
			delete laste;
			edgelist = NULL;
		}
	}
	void ClearVertexList()
	{
		Vertex* lastv;
		if (vtxlist == NULL)
		{
			return;
		}
		else
		{
			lastv = vtxlist->prev;
			while (lastv != vtxlist)
			{
				lastv = lastv->prev;
				delete lastv->next;
			}
			delete lastv;
			vtxlist = NULL;
		}
	}
	void Print()
	{
 		printf("Solid sID:%d\n", this->sID);
		Face* f = this->facelist;
		Loop* lp;
		HalfEdge* he;
		while (f)
		{
			printf("  Face fID:%d\n", f->fID);
			lp = f->floopout;
			printf("    The out Loop:\n");
			he = lp->helist;
			printf("\t");
			while (he)
			{
				printf("<%d>", he->vtx->vID);
				he = he->next;
				if (he == lp->helist)
				{
					break;
				}
			}
			printf("\n");
			lp = f->floops;
			printf("    The inner Loops:\n");
			while (lp)
			{
				he = lp->helist;
				printf("  the loops:\n");
				printf("\t");
				while (he)
				{
					printf("<%d>", he->vtx->vID);
					he = he->next;
					if (he == lp->helist)
					{
						break;
					}
				}
				printf("\n");
				lp = lp->next;
				if (lp == f->floops)
				{
					break;
				}
			}
			printf("\n");
			f = f->next;
			if (f == this->facelist)
			{
				break;
			}
		}
	}
	void wirepaint()
	{
		glBegin(GL_LINES);
		Edge* tmp;
		tmp = this->edgelist;
		while (tmp)
		{
			glVertex3d(tmp->lhe->vtx->coord[0], tmp->lhe->vtx->coord[1], tmp->lhe->vtx->coord[2]);
			glVertex3d(tmp->rhe->vtx->coord[0], tmp->rhe->vtx->coord[1], tmp->rhe->vtx->coord[2]);
			tmp = tmp->next;
			if (tmp == this->edgelist)
			{
				break;
			}
		}
		glEnd();
	}
	void solidpaint()
	{
		Face* tmp;
		Loop* lp;
		GLfloat colors[][3] = { {1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0},
		{0.0,1.0,1.0},{1.0,0.0,1.0},{1.0,1.0,0.0} };
		int Coptions;
		tmp = this->facelist;
		HalfEdge* he;
		Coptions = 0;
		while (tmp)
		{
			glBegin(GL_POLYGON);
			lp = tmp->floopout;
			he = lp->helist;
			Coptions = Coptions % 6;
			glColor3fv(colors[Coptions]);
			while (he)
			{
				glVertex3d(he->vtx->coord[0], he->vtx->coord[1], he->vtx->coord[2]);
				he = he->next;
				if (he == lp->helist)
				{
					break;
				}
			}
			Coptions++;
			glEnd();
			tmp = tmp->next;
			if (tmp == this->facelist)
			{
				break;
			}
		}
	}
	ID sID;
	Solid* prev;
	Solid* next;
	Vertex* vtxlist;
	Face* facelist;
	Edge* edgelist;
};
static Solid* solidlist = NULL;
static ID maxs;
static ID maxf;
static ID maxv;
//实现对于基本数据结构的查找、删除
static void  AddToSolidList(Solid* s)
{
	if (solidlist == NULL)
	{
		solidlist = s;
	}
	else
	{
		s->next = solidlist;
		s->prev = solidlist->prev;
		solidlist->prev->next = s;
		solidlist->prev = s;
	}
}
static void ClearSolidList()
{
	Solid* lasts;
	if (solidlist == NULL)
	{
		return;
	}
	else
	{
		lasts = solidlist->prev;
		while (lasts != solidlist)
		{
			lasts = lasts->prev;
			lasts->next->ClearEdgeList();
			lasts->next->ClearFaceList();
			lasts->next->ClearVertexList();
			delete lasts->next;
		}
		lasts->ClearVertexList();
		lasts->ClearEdgeList();
		lasts->ClearFaceList();
		delete lasts;
		solidlist = NULL;
	}
}
static void getmaxnames(Solid* s)
{
	Vertex* v;
	Face* f;
	v = s->vtxlist;
	maxv = 0;
	while (v)
	{
		if (v->vID > maxv)
		{
			maxv = v->vID;
		}
		v = v->next;
		if (v == s->vtxlist)
		{
			break;
		}
	}
	f = s->facelist;
	maxf = 0;
	while (f)
	{
		if (f->fID > maxf)
		{
			maxf = f->fID;
		}
		f = f->next;
		if (f == s->facelist)
		{
			break;
		}
	}
}
static Solid* GetSolidFromID(ID n)
{
	Solid* solid;
	if (solidlist == NULL)
	{
		return NULL;
	}
	else
	{
		solid = solidlist;
		while (solid)
		{
			if (solid->sID == n)
			{
				return solid;
			}
			solid = solid->next;
			if (solid == solidlist)
			{
				break;
			}
		}
		return NULL;
	}
}
static Face* findface(Solid* s, ID fn)
{
	Face* f;
	f = s->facelist;
	while (f)
	{
		if (f->fID == fn)
		{
			return f;
		}
		f = f->next;
		if (f == s->facelist)
		{
			break;
		}
	}
	return NULL;
}
static Vertex* findvertex(Solid* s, ID vn)
{
	Vertex* v;
	v = s->vtxlist;
	while (v)
	{
		if (v->vID == vn)
		{
			return v;
		}
		v = v->next;
		if (v == s->vtxlist)
		{
			break;
		}
	}
	return NULL;
}
static HalfEdge* findHalfEdge(Face* f, ID v1, ID v2)
{
	Loop* lp;
	HalfEdge* he;
	lp = f->floopout;
	he = lp->helist;
	while (he)
	{
		if ((he->vtx->vID == v1) && (he->next->vtx->vID == v2))
		{
			return he;
		}
		he = he->next;
		if (he == lp->helist)
		{
			break;
		}
	}
	lp = f->floops;
	while (lp)
	{
		he = lp->helist;
		while (he)
		{
			if ((he->vtx->vID == v1) && (he->next->vtx->vID == v2))
			{
				return he;
			}
			he = he->next;
			if (he == lp->helist)
			{
				break;
			}
		}
		lp = lp->next;
		if (lp == f->floops)
		{
			break;
		}
	}
	return NULL;
}
static HalfEdge* AddHalfEdge(Edge* newedge, Vertex* v, HalfEdge* position, int sign)
{
	HalfEdge* he;
	if (position->edge == NULL)
	{
		he = position;
	}
	else
	{
		he = new HalfEdge(v);
		position->prev->next = he;
		he->prev = position->prev;
		position->prev = he;
		he->next = position;
	}
	he->edge = newedge;
	he->vtx = v;
	he->loop = position->loop;
	if (sign == PLUSH)
	{
		newedge->lhe = he;
	}
	else
	{
		newedge->rhe = he;
	}
	return he;
}
static HalfEdge* DelHalfEdge(HalfEdge* he)
{
	HalfEdge* tmp;
	if (he->edge == NULL)
	{
		delete he;
		return NULL;
	}
	else if (he->next == he)
	{
		he->edge = NULL;
		return he;
	}
	else
	{
		he->prev->next = he->next;
		he->next->prev = he->prev;
		tmp = he->prev;
		delete he;
		return tmp;
	}
}
//实现5种欧拉操作
//mvfs，生成一个点，一个面，一个体，一般用于开始一步
static Solid* mvfs(ID sid, ID fid, ID vid, double x, double y, double z)
{
	Solid* tmp;
	if ((tmp = GetSolidFromID(sid)) != NULL)
	{
		MessageBox(NULL, TEXT("This Solid ID Is Already Attached To An Existing Solid."), TEXT("ERROR:Create Solid"), MB_ICONERROR);
	}
	Vertex* vp = new Vertex(vid, x, y, z);
	Solid* s = new Solid(sid);
	Face* f = new Face(fid);
	Loop* l = new Loop();
	HalfEdge* he = new HalfEdge(vp);
	s->AddFace(f);
	s->AddVtx(vp);
	f->solid = s;
	f->floopout = l;
	l->helist = he;
	l->face = f;
	he->loop = l;
	return s;
}
//mev，生成一条边和一个点
static void lmev(Solid* s, HalfEdge* he1, HalfEdge* he2, ID vid, double x, double y, double z)
{
	HalfEdge* he;
	Vertex* newvertex;
	Edge* newedge;
	newedge = new Edge();
	newvertex = new Vertex(vid, x, y, z);
	s->AddVtx(newvertex);
	s->AddEdge(newedge);
	he = he1;
	while (he != he2)
	{
		he->vtx = newvertex;
		he = mate(he);
	}
	AddHalfEdge(newedge, he2->vtx, he1, MINUS);
	AddHalfEdge(newedge, newvertex, he2, PLUSH);
	newvertex->he = he2->prev;
	he2->vtx->he = he2;
}
static bool mev(ID sid, ID f1, ID f2, ID v1, ID v2, ID v3, ID v4, double x, double y, double z)
{
	Solid* oldsolid;
	Face* oldface1, * oldface2;
	HalfEdge* he1, * he2;
	if ((oldsolid = GetSolidFromID(sid)) == NULL)
	{
		fprintf(stderr, "mev:solid %d not found\n", sid);
		return false;
	}
	if ((oldface1 = findface(oldsolid, f1)) == NULL)
	{
		fprintf(stderr, "mev:face %d not found in solid %d.\n", f1, sid);
		return false;
	}
	if ((oldface2 = findface(oldsolid, f2)) == NULL)
	{
		fprintf(stderr, "mev:face %d not found in solid %d.\n", f2, sid);
		return false;
	}
	if ((he1 = findHalfEdge(oldface1, v1, v2)) == NULL)
	{
		fprintf(stderr, "mev:edge %d-%d not found in face %d.\n", v1, v2, f1);
		return false;
	}
	if ((he2 = findHalfEdge(oldface2, v1, v3)) == NULL)
	{
		fprintf(stderr, "mev:edge %d-%d not found in face %d.\n", v1, v3, f2);
		return false;
	}
	lmev(oldsolid, he1, he2, v4, x, y, z);
	return true;
}
static bool smev(ID sid, ID f1, ID v1, ID v4, double x, double y, double z)
{
	Solid* oldsolid;
	Face* oldface;
	HalfEdge* he;
	Loop* lp;
	Vertex* v;
	bool exist = true;
	if ((oldsolid = GetSolidFromID(sid)) == NULL)
	{
		MessageBox(NULL, TEXT("No Such Solid Exists."), TEXT("ERROR:Create Edge"), MB_ICONERROR);
		return false;
	}
	if ((oldface = findface(oldsolid, f1)) == NULL)
	{
		MessageBox(NULL, TEXT("Face  Not Find In solid."), TEXT("ERROR:Create Edge"), MB_ICONERROR);
		return false;
	}
	if ((v = findvertex(oldsolid, v1)) == NULL)
	{
		MessageBox(NULL, TEXT("Source Vertex Not Find In solid."), TEXT("ERROR:Create Edge"), MB_ICONERROR);
		return false;
	}
	if ((v = findvertex(oldsolid, v4)) != NULL)
	{
		MessageBox(NULL, TEXT("New Vertex To Be Created Exists"), TEXT("ERROR:Create Edge"), MB_ICONERROR);
		return false;
	}
	lp = oldface->floopout;
	he = lp->helist;
	while (he)
	{
		if (he->vtx->vID == v1)
		{
			break;
		}
		he = he->next;
		if (he == lp->helist)
		{
			exist = false;
			break;
		}
	}
	if (!exist)
	{
		fprintf(stderr, "ERROR:operation failed.\n");
		return false;
	}
	lmev(oldsolid, he, he, v4, x, y, z);
	return true;
}
//mef，生成一条边和一个面，新面与旧面通过新边连接
static Face* lmef(Solid* s, HalfEdge* he1, HalfEdge* he2, ID fn)
{
	Face* fnew = new Face(fn);
	Loop* lnew = new Loop();
	Edge* enew = new Edge();
	Face* fold = he1->loop->face;
	//modify
	//fnew->floops = fold->floops;
	//fold->floops = NULL;
	//end modify
	s->AddEdge(enew);
	s->AddFace(fnew);
	HalfEdge* he, * nhe1, * nhe2;
	lnew->face = fnew;
	fnew->floopout = lnew;
	he = he1;
	while (he != he2)
	{
		he->loop = lnew;
		he = he->next;
	}
	nhe2 = AddHalfEdge(enew, he1->vtx, he2, PLUSH);
	nhe1 = AddHalfEdge(enew, he2->vtx, he1, MINUS);

	nhe1->prev->next = nhe2;
	nhe2->prev->next = nhe1;
	he = nhe1->prev;
	nhe1->prev = nhe2->prev;
	nhe2->prev = he;

	lnew->helist = nhe1;
	he2->loop->helist = nhe2;

	return fnew;
}
static bool mef(ID s, ID f1, ID v1, ID v2, ID v3, ID v4, ID f2)
{
	Solid* solid = GetSolidFromID(s);
	Face* face1 = findface(solid, f1);
	HalfEdge* he1, * he2;
	if (s == NULL)
	{
		fprintf(stderr, "ERROR:Solid %d not found.\n", s);
		return false;
	}
	if (face1 == NULL)
	{
		fprintf(stderr, "ERROR:face %d not found in Solid %d.\n", f1, s);
		return false;
	}
	if ((he1 = findHalfEdge(face1, v1, v2)) == NULL)
	{
		fprintf(stderr, "ERROR:Edge %d-%d not found in face %d.\n", v1, v2, f1);
		return false;
	}
	if ((he2 = findHalfEdge(face1, v3, v4)) == NULL)
	{
		fprintf(stderr, "ERROR:Edge %d-%d not found in face %d.\n", v3, v4, f1);
		return false;
	}
	lmef(solid, he1, he2, f2);
	return true;
}
static bool smef(ID s, ID f1, ID v1, ID v3, ID f2)
{
	Solid* solid;
	Face* face1;
	Loop* lp;
	HalfEdge* he;
	HalfEdge* he1=NULL, * he2=NULL;
	Vertex* v;
	if ((solid = GetSolidFromID(s)) == NULL)
	{
		MessageBox(NULL, TEXT("No Such Solid Exists."), TEXT("ERROR:Create Face"), MB_ICONERROR);
		return false;
	}
	if ((face1 = findface(solid, f2)) != NULL)
	{
		MessageBox(NULL, TEXT("Face To Create Exists,You Should Change The New Face ID"), TEXT("ERROR:Create Face"), MB_ICONERROR);
		return false;
	}
	if ((face1 = findface(solid, f1)) == NULL)
	{
		MessageBox(NULL, TEXT("Face  Not Find In solid."), TEXT("ERROR:Create Face"), MB_ICONERROR);
		return false;
	}
	if ((v = findvertex(solid, v1)) == NULL)
	{
		MessageBox(NULL, TEXT("Source Vertex Not Find In solid."), TEXT("ERROR:Create Face"), MB_ICONERROR);
		return false;
	}
	if ((v = findvertex(solid, v3)) == NULL)
	{
		MessageBox(NULL, TEXT("Target Vertex Not Find In Solid."), TEXT("ERROR:Create Face"), MB_ICONERROR);
		return false;
	}
	lp = face1->floopout;
	he = lp->helist;
	while (he)
	{
		if (he->vtx->vID == v1)
		{
			he1 = he;
			break;
		}
		he = he->next;
		if (he == lp->helist)
		{
			return false;
		}
	}
	he = lp->helist;
	while (he)
	{
		if (he->vtx->vID == v3)
		{
			he2 = he;
			break;
		}
		he = he->next;
		if (he == lp->helist)
		{
			return false;
		}
	}
	lmef(solid, he1, he2, f2);
	return true;
}
//kemr，删除一边，生成一个环
static void lkemr(Solid* s, HalfEdge* h1, HalfEdge* h2)
{
	HalfEdge* h3, * h4;
	Loop* oldloop;
	Loop* newloop;
	Edge* killedge;
	oldloop = h1->loop;
	newloop = new Loop();
	//modify
	oldloop->face->floops = mate(h1->next)->loop->face->floops;
	mate(h1->next)->loop->face->floops = NULL;
	//end modify
	oldloop->face->AddToLoops(newloop);
	//oldloop->face->floops = newloop;
	killedge = h1->edge;
	newloop->face = oldloop->face;

	h3 = h1->next;
	h1->next = h2->next;
	h2->next->prev = h1;
	h2->next = h3;
	h3->prev = h2;

	h4 = h2;
	do
	{
		h4->loop = newloop;
	} while ((h4 = h4->next) != h2);

	oldloop->helist = h3 = DelHalfEdge(h1);
	newloop->helist = h4 = DelHalfEdge(h2);

	h3->vtx->he = (h3->edge) ? h3 : (HalfEdge*)NULL;
	h4->vtx->he = (h4->edge) ? h4 : (HalfEdge*)NULL;

	delete s->DelEdge(killedge);
}
static bool skemr(ID sn, ID fn, ID v1, ID v2)
{
	Solid* solid = GetSolidFromID(sn);
	Face* face;
	Loop* lp;
	HalfEdge* he, * he1 = NULL, * he2 = NULL;
	Vertex* v;
	if (solid == NULL)
	{
		MessageBox(NULL, TEXT("No Such Solid Exists."), TEXT("ERROR:Create Ring"), MB_ICONERROR);
		return false;
	}
	if ((face = findface(solid, fn)) == NULL)
	{
		MessageBox(NULL, TEXT("Face  Not Find In solid"), TEXT("ERROR:Create Ring"), MB_ICONERROR);
		return false;
	}
	if ((v = findvertex(solid, v1)) == NULL)
	{
		MessageBox(NULL, TEXT("Source Vertex Not Find In solid."), TEXT("ERROR:Create Ring"), MB_ICONERROR);
		return false;
	}
	if ((v = findvertex(solid, v2)) == NULL)
	{
		MessageBox(NULL, TEXT("Target Vertex Not Find In Solid."), TEXT("ERROR:Create Ring"), MB_ICONERROR);
		return false;
	}
	lp = face->floopout;
	he = lp->helist;
	while (he)
	{
		if ((he->vtx->vID == v1) && (he->next->vtx->vID == v2))
		{
			he1 = he;
			break;
		}
		he = he->next;
		if (he == lp->helist)
		{
			fprintf(stderr, "kemr:error.\n");
			return false;
		}
	}
	he = lp->helist;
	while (he)
	{
		if ((he->vtx->vID == v2) && (he->next->vtx->vID == v1))
		{
			he2 = he;
			break;
		}
		he = he->next;
		if (he == lp->helist)
		{
			fprintf(stderr, "kemr:error\n");
			return false;
		}
	}
	lkemr(solid, he1, he2);
	return true;
}
//kfmrh,删除一面，产生一个内环和通孔
static void kfmrh(ID sid, ID fn, ID ftodel)
{
	Solid* solid;
	Face* face1;
	Face* face2;
	Loop* lptmp;
	if ((solid = GetSolidFromID(sid)) == NULL)
	{
		MessageBox(NULL, TEXT("Solid Not Found."), TEXT("ERROR:Create Hole"), MB_ICONERROR);
		return;
	}
	if ((face1 = findface(solid, fn)) == NULL)
	{
		MessageBox(NULL, TEXT("Face Not Found In Solid."), TEXT("ERROR:Create Hole"), MB_ICONERROR);
		return;
	}
	if ((face2 = findface(solid, ftodel)) == NULL)
	{
		MessageBox(NULL, TEXT("Face Not Found In Solid."), TEXT("ERROR:Create Hole"), MB_ICONERROR);
		return;
	}
	lptmp = face2->floopout;
	face1->AddToLoops(lptmp);
	if (face2->floops != NULL)
	{
		lptmp = face2->floops->prev;
		while (lptmp != face2->floops)
		{
			lptmp = lptmp->prev;
			face1->AddToLoops(lptmp->next);
		}
		face1->AddToLoops(lptmp);

	}
	delete solid->DelFace(face2);
}
//扫成函数，face为基面，dx，dy，dz为扫成方向
static void sweep(ID sid, ID fid, double vx, double vy, double vz)
{
	/*Solid *tmp = GetSolidFromID(sid);
	Face  *face = findface(tmp,fid);
	Loop  *lp;
	HalfEdge *first,*scan;
	Vertex *v;*/
	Solid* tmp;
	Face* face;
	Loop* lp;
	HalfEdge* first, * scan;
	Vertex* v;
	if ((tmp = GetSolidFromID(sid)) == NULL)
	{
		MessageBox(NULL, TEXT("Can't Find The Solid"), TEXT("ERROR:Display Face"), MB_ICONERROR);
		return;
	}
	if ((face = findface(tmp, fid)) == NULL)
	{
		MessageBox(NULL, TEXT("Face  Not Find In solid"), TEXT("ERROR:Display Face"), MB_ICONERROR);
		return;
	}
	getmaxnames(tmp);
	lp = face->floopout;
	while (lp)
	{
		first = lp->helist;
		scan = first->next;
		v = scan->vtx;
		lmev(tmp, scan, scan, ++maxv,
			v->coord[0] + vx,
			v->coord[1] + vy,
			v->coord[2] + vz);
		while (scan != first)
		{
			v = scan->next->vtx;
			lmev(tmp, scan->next, scan->next, ++maxv,
				v->coord[0] + vx,
				v->coord[1] + vy,
				v->coord[2] + vz);
			lmef(tmp, scan->prev, scan->next->next, ++maxf);
			scan = (mate(scan->next))->next;
		}
		lmef(tmp, scan->prev, scan->next->next, ++maxf);
		if (lp == face->floopout)
		{
			lp = face->floops;
		}
		else
		{
			lp = lp->next;
			if (lp == face->floops)
			{
				break;
			}
		}
	}

}



#endif