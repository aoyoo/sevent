#include<vector>
#include "Shared_Ptr.h"
#include "CFilterLevel.h"
using namespace std;

class CFilterLevelObj:public CFilterLevel,public SharedObject
{
public:
	CFilterLevelObj()
	{
	}
	~CFilterLevelObj()
	{
	}

};
typedef Shared_Ptr<CFilterLevelObj> SharedFilterPtr;

class CFilterList
{
public:
	CFilterList()
	{}
	~CFilterList()
	{
		std::cout<<__FILE__<<__LINE__<<std::endl;
		m_vList.clear();
	}

	void Push(SharedFilterPtr filter)
	{
		m_vList.push_back(filter);
	}
private:
	vector<SharedFilterPtr> m_vList;
};

int main(int argc,char *argv[])
{
	CLevel level;
	CFilterList filter_list;
	{
		SharedFilterPtr filter(new CFilterLevelObj());
		SharedFilterPtr filter1(new CFilterLevelObj());

		filter->SetLevel(level);
		filter_list.Push(filter);
		filter_list.Push(filter1);
	}
	getchar();
}



