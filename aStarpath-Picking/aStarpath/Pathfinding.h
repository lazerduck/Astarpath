

using namespace std;
struct vector
{
public:
	int X, Y,oldX,oldY;
	float cost,fScore;
	vector()
	{
		X = 0;
		Y = 0;
		oldX = 0;
		oldY = 0;
		cost = 0;
		fScore = 0;
	}
	vector(int x, int y)
	{
		X = x;
		Y = y;
		oldX = 0;
		oldY = 0;
		cost = 0;
		fScore = 0;
	}
	vector(int x, int y, int oldx, int oldy,float oldCost )
	{
		X = x;
		Y = y;
		oldX = oldx;
		oldY = oldy;
		cost = oldCost;
		fScore = 0;
	}
	vector(vector& v)
	{
		X = v.X;
		Y = v.Y;
		oldX = v.oldX;
		oldY = v.oldY;
		cost = v.cost;
		fScore = v.fScore;
	}
	bool operator!=(vector& left)
	{
		if(this->X == left.X && this->Y == left.Y)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	bool operator==(vector& left)
	{
		if(this->X == left.X && this->Y == left.Y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};
vector findStart(Matrix &Room, bool end = false);

class Path
{
protected:
	Matrix& Room;
	Matrix Heuristic;
	int iterations;
	void Neighbor(vector &Current, LinkedList<vector>& open,LinkedList<vector>& closed, int Xoffset, int Yoffset);
public:
	Path(Matrix &room) : Room(room){};
	void Init();
	void CalcHeuristic();
	void FindPath(int sx, int sy, int gx, int gy);
	void CreatePath(int i,LinkedList<vector> &closed);

};
void Path::Init()
{
	iterations = 0;
	Heuristic.SetColumns(Room.GetColumns());
	Heuristic.SetRows(Room.GetRows());
	float* data = new float[Room.ReturnSize()];
	for(int i = 0; i< Heuristic.ReturnSize();i++)
	{
		data[i] = 0;
	}
	Heuristic.SetData(data,Heuristic.ReturnSize());
	delete[]data;
}
void Path::CalcHeuristic()
{
	vector end = findStart(Room, true);
	vector start = findStart(Room, false);
	float dx2 = start.X - end.X;
	float dy2 = start.Y - end.Y;
	for(int i = 0; i < Room.GetRows();i++)
	{
		for(int j = 0; j < Room.GetColumns();j++)
		{
			float dx = abs(i - end.X);
			float dy = abs(j - end.Y);
			float val = dx+dy + (sqrt(2.0)-1.4)*min(dx,dy);
			Heuristic.SetValue(i,j, val);
		}
	}
}
void Path::CreatePath(int i, LinkedList<vector> &closed)
{
	vector temp = closed[closed.Length()-1];
	while(temp.oldX != -1)
	{
		if(Room(temp.X,temp.Y) == 5)
		{
			Room.SetValue(temp.X,temp.Y,6);
		}
		else
		{
			Room.SetValue(temp.X,temp.Y,4);
		}
		vector next (temp.oldX,temp.oldY);
		temp = closed.Contains(next,true);

	}
}
void Path::FindPath(int sx, int sy, int gx, int gy)
{
	bool found = false;
	vector end(gx,gy);
	LinkedList<vector> open;
	LinkedList<vector> closed;
	vector start(sx,sy);
	start.oldX = -1;
	start.oldY = -1;
	start.cost = 0;
	start.fScore = start.cost + Heuristic(start.X,start.Y);
	int lowest = 1000;
	open.Add(start);
	while(!found)
	{
		if(open.Length() == 0)
		{
			found = true;
			break;
		}
		int i = 0;
		float low = 3000;
		for(int j = 0;j<open.Length();j++)
		{
			float total = open[j].cost + Heuristic(open[j].X,open[j].Y);
			if(total< low)
			{
				i = j;
				low = total;
			}
		}
		if(open[i] == end|| Room(open[i].X,open[i].Y) == 4)
		{
			found = true;
			closed.Add(open[i]);
			CreatePath(0,closed);
			break;
		}
		iterations++;
		vector current(open[i]);
		closed.Add(vector(open[i]));
		//Room.SetValue(open[i].X,open[i].Y,Room(open[i].X,open[i].Y)+1);
		open.RemoveAt(i);
		
		Neighbor(current,open,closed,1,0);
		Neighbor(current,open,closed,-1,0);
		Neighbor(current,open,closed,0,1);
		Neighbor(current,open,closed,0,-1);
		Neighbor(current,open,closed,1,1);
		Neighbor(current,open,closed,1,-1);
		Neighbor(current,open,closed,-1,1);
		Neighbor(current,open,closed,-1,-1);

	}
	Room.SetValue(start.X,start.Y,2);
	Room.SetValue(end.X,end.Y,3);
	open.Add(vector(0,0));
}
vector findStart(Matrix &Room, bool end)
{
	int num = 2;
	if(end)
	{
		num = 3;
	}
	for(int i = 0; i < Room.GetRows();i++)
	{
		for(int j = 0; j<Room.GetColumns();j++)
		{
			if(Room.ReturnValue(i,j) == num)
			{
				vector ret(i,j);
				return ret;
			}
		}
	}
}
void Path::Neighbor(vector &Current, LinkedList<vector>& open,LinkedList<vector>& closed, int Xoffset, int Yoffset)
{
	float tenative = Current.cost + sqrt((float)abs(Xoffset)+(float)abs(Yoffset));
	vector next(Current.X + Xoffset,Current.Y+Yoffset,Current.X,Current.Y,tenative);
	if(Room(next.X,next.Y) != 1)
	{
		if(!closed.Contains(next))
		if(!open.Contains(next))
		{
			open.Add(next);
			//Room.SetValue(next.X,next.Y,6);
		}
		else
		{
			int pos = open.ContainsAt(next);
			if(tenative < open[pos].cost)
			{
				open.RemoveAt(pos);
				open.Add(next);
				//Room.SetValue(next.X,next.Y,6);
			}
		}
	}
}