#include<queue>
#include<string>
#include<iostream>
using namespace std;
#include<vector>
#include<assert.h>
/*
图是另一种非线性结构，由顶点集合(vertex)及顶点间的关系集合组成的一种数据结构。 ：**建立数组存放顶点信息 
  数据结构，本身就是以存储数据而生
《: 无法表示边，但可以表示定点，用两个顶点去表示边》

临接矩阵：矩阵内部存放的是权重
step1 : 将顶点数据存放在数组中，建立矩阵
step2 : 添加权重

*/
namespace Matrix
{

	template <class V, class K >//V 代表顶点值，K 代表权重
	class GraphMatrix
	{
	public:
		GraphMatrix(V *a, size_t sz, bool isDirected = false, K key = K())
			: _v(a, a + sz)//左闭右开
			, _sz(sz)
			, _isDirected(isDirected)
		{
			_matrix = new K*[_sz]();
			for (int i = 0; i < _sz; i++)
			{
				_matrix[i] = new K[_sz]();
			}

		}

		~GraphMatrix()
		{
			for (int i = 0; i < _sz; i++)
			{
				if (_matrix[i] != NULL)
				{
					delete[] _matrix[i];
					_matrix[i] = NULL;
				}
			}

			if (_matrix != NULL)
			{
				delete[] _matrix;
				_matrix = NULL;
			}

		}

		size_t Getindex(const V& value)
		{
			for (int i = 0; i < _sz; i++)
			{
				if (_v[i] == value)
				{
					return i;
				}
			}
			assert(NULL);
		}

		void AddEdge(const V& value1, const V& value2, const K& key)
		{
			int index1 = Getindex(value1);
			int index2 = Getindex(value2);
			if (!_isDirected)
			{
				_matrix[index1][index2] = key;
				_matrix[index2][index1] = key;
			}
			else
			{
				_matrix[index1][index2] = key;
			}
		}

		void Print()
		{
			for (int i = 0; i < _sz; i++)
			{
				for (int j = 0; j < _sz; j++)
				{
					if (_matrix[i][j] != K())
						cout << i << " " << j << " " << _v[i] << " " << _v[j] << " " << _matrix[i][j] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}

		void DFS(const V& value) //遍历和printf不一样：前者是以数据结构进行输出，后者是以数据集合进行输出
		{
			size_t index = Getindex(value);
			bool* tab = new bool[_sz];
			for (int i = 0; i < _sz; i++)
			{
				tab[i] = false;
			}
			_DFS(tab, index);

			for (int i = 0; i < _sz; i++)
			{
				if (!tab[i])
					_DFS(tab, i);

			}
		
			cout << endl;
		}

		void BFS(const V& value)
		{
			size_t index = Getindex(value);
			bool* tab = new bool[_sz];
			for (int i = 0; i < _sz; i++)
			{
				tab[i] = false;
			}
			queue<size_t> q;
			q.push(index);
			tab[index] = true;
			_BFS(q,tab);
			for (int i = 0; i < _sz; i++)
			{
				if (!tab[i])
				{
					q.push(i);
					tab[i] = true;
					_BFS(q, tab);
				}

			}
			cout << endl;
		}

	protected:
	/*	void _DFS(bool *tab,size_t index)    无向图的遍历，
		{
			size_t index2;
			if (!tab[index])
			{
				cout << _v[index] << " ";
				tab[index] = true;
				for (int i = 0; i < _sz; i++)
				{
					if (_matrix[index][i] != 0&&tab[i] ==false)
					{
						_DFS(tab, i);
					}
				}

			}
		}*/

		void _DFS(bool *tab, size_t index)
		{
			size_t index2;
			if (!tab[index])
			{
				cout << _v[index] << " ";
				tab[index] = true;
				for (int i = 0; i < _sz; i++)
				{
					if (_matrix[index][i] != 0 && tab[i] == false)
					{
						_DFS(tab, i);
					}
				}

			}
		}

		void _BFS(queue<size_t>& q, bool *tab) //跟着图想遍历的方式，跟着图的邻接矩阵和邻接表想数据实现的方式。
		{
			while (!q.empty())
			{
				size_t index = q.front();
				q.pop();
				cout << _v[index] << " ";

				for (size_t i = 0; i < _sz; i++)
				{
					if (_matrix[index][i] != 0 && tab[i] == false)
					{
						q.push(i);
						tab[i] = true;
					}
				}
			}
		}

	protected:
		vector<V> _v;
		size_t _sz;
		K** _matrix;//矩阵
		bool _isDirected;
	};

}


namespace Labels
{
	template<typename V,typename K>
	class GraphLink
	{
		struct Node  //看似是图，实际上边，每一个节点都是权重，也就是边，通过传参得到有始末端点的边
		{
			Node *_next;
			K _key;  //权重
			V _dst;
			V _src;
			Node(V src, V dst, const K& key = K())
				:_next(NULL)
				, _key(key) 
				, _dst(dst)//末端
				, _src(src)//始端
			{}
		};

		
	public:
		GraphLink(V* a, size_t sz,bool isDirected = false)
			:_v(a, a + sz)
			, _sz(sz)
			, _isDirected(isDirected)
			, _tables(sz)
		{

		}

		size_t Getindex(const V& value)
		{
			for (int i = 0; i < _sz; i++)
			{
				if (_v[i] == value)
				{
					return i;
				}
			}
			assert(NULL);
		}


		void AddEdge(const V& value1, const V& value2, const K& key)
		{
			int index1 = Getindex(value1);
			int index2 = Getindex(value2);
			Node *cur = new Node(value1,value2,key);
			Node *tmp = new Node(value2, value1, key);
			if (!_isDirected)
			{
				cur->_next = _tables[index1];
				_tables[index1] = cur;
				tmp->_next = _tables[index2];/*如果只是添加给每个节点添加权重就不用多写tmp，否则的话，对于无向图任意两个顶点都会是 始端  */
				_tables[index2] = tmp;
			}
			else
			{
				cur->_next = _tables[index1];
				_tables[index1] = cur;
			}
		}

		void DFS(const V& value,const size_t sz)
		{
			 
			bool *tab = new bool[sz] ;
			for (int i = 0; i < _sz; i++)
			{
				tab[i] = false; //标记数组
			}
			size_t index = Getindex(value);
			_DFS(tab, index);
			for (int i = 0; i < _sz; i++)
			{
				if (!tab[i])
					_DFS(tab, i);

			}
			cout<<endl;
		}

		void BFS(const V& value)  //遍历遍的不是边，而是端点
		{
			bool *tab = new bool[_sz];
			for (int i = 0; i < _sz; i++)
			{
				tab[i] = false;
			}
			 
			size_t index = Getindex(value);
			queue<size_t> q;
			q.push(index);
			tab[index] = true;//顶点只要压入队列就置true
			_BFS(q, tab);

			for (int i = 0; i < _sz; i++)
			{ 
				if (i == index)
					i++;
				if (tab[i] == false)
				{	
					q.push(i);
					tab[index] = true;//顶点只要压入队列就置true
					_BFS(q, tab);
				}		
			}
			cout << endl;
		}
		//最小生成树

		void Print()
		{
			for (int i = 0; i < _sz; i++)
			{
				while (_tables[i] == NULL)/*针对有向图时：有的顶点只作为末端，但数组里面包含该顶点，表的大小又和数组一致，会造成tables内部有的元素为NULL*/
				{
					i++;
				}
				Node *cur = _tables[i];
				cout << cur->_src;
				while (cur)
				{
					cout  <<"->" << cur->_key << " " << cur->_dst << " ";
					cur = cur->_next;
				}
				cout << endl;

			}
			cout << endl;
		}
	protected:

		void _BFS(queue<size_t>& q, bool *tab )
		{
			while (!q.empty())
			{
				size_t index = q.front();
				cout << _v[index] << " ";
				q.pop();
				Node *cur = _tables[index];
				while (cur)
				{
					size_t curIndex = Getindex(cur->_dst);//在一条链中，其_src都一样
					if (!tab[curIndex])
					{
						q.push(curIndex);
						tab[curIndex] = true;
					}
					cur = cur->_next;			
				}
			}
		}
		void _DFS( bool *tab, int index)
		{ 
			if (!tab[index])
			{
				cout << _v[index] << " ";
				tab[index] = true;
				//
				if (_tables[index]== NULL)			//tables 里面放的是权重的节点
				{
					return;
				}
				size_t nextIndex = Getindex(_tables[index]->_dst);
				Node *cur = _tables[nextIndex];
				while (cur)
				{
					if (!tab[nextIndex])
					{
						_DFS(tab, nextIndex);
					}
					cur = cur->_next;
					if (cur)
						nextIndex = Getindex(cur->_dst);
				}
				
			}
			
		}

	protected:
		vector<V> _v;
		const size_t _sz;
		bool _isDirected;
		vector<Node*> _tables;
		
		
	};
}
void GraphTextMatrix()
{
	string a[] = { "西安", "北京", "上海", "南京", "杭州" };
	Matrix::GraphMatrix<string, int > gm(a, 5,true);
	gm.AddEdge("西安","北京",100);
	gm.AddEdge("上海", "北京",200);
	gm.AddEdge("南京", "西安", 300);
	gm.AddEdge("杭州", "上海", 400);	
	gm.AddEdge("西安", "上海", 500);
	cout << "DFS:  ";
	gm.DFS("西安");
	cout << "BFS:  ";
	gm.BFS("西安");

	cout << "---------------" << endl;
	gm.Print();

}

void GraphTextTables()
{
	string a[] = { "西安", "北京", "上海", "南京", "杭州" };
	Labels::GraphLink<string, int> gm(a, 5,true);//有向图 非连通图
	gm.AddEdge("西安", "北京", 100);
	gm.AddEdge("上海", "北京", 200);
	gm.AddEdge("北京", "上海", 200);
	gm.AddEdge("西安", "南京", 300);
	gm.AddEdge("杭州", "北京", 400);
	gm.AddEdge("西安", "上海", 500);
	cout <<"DFS:" << " ";
	gm.DFS("西安",5);
	cout << "BFS:" << " ";
	gm.BFS("西安");
	cout << "------------ "<<endl;
	gm.Print();
}


