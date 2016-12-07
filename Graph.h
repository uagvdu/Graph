#include<queue>
#include<string>
#include<iostream>
using namespace std;
#include<vector>
#include<assert.h>
/*
ͼ����һ�ַ����Խṹ���ɶ��㼯��(vertex)�������Ĺ�ϵ������ɵ�һ�����ݽṹ�� ��**���������Ŷ�����Ϣ 
  ���ݽṹ����������Դ洢���ݶ���
��: �޷���ʾ�ߣ������Ա�ʾ���㣬����������ȥ��ʾ�ߡ�

�ٽӾ��󣺾����ڲ���ŵ���Ȩ��
step1 : ���������ݴ���������У���������
step2 : ���Ȩ��

*/
namespace Matrix
{

	template <class V, class K >//V ������ֵ��K ����Ȩ��
	class GraphMatrix
	{
	public:
		GraphMatrix(V *a, size_t sz, bool isDirected = false, K key = K())
			: _v(a, a + sz)//����ҿ�
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

		void DFS(const V& value) //������printf��һ����ǰ���������ݽṹ��������������������ݼ��Ͻ������
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
	/*	void _DFS(bool *tab,size_t index)    ����ͼ�ı�����
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

		void _BFS(queue<size_t>& q, bool *tab) //����ͼ������ķ�ʽ������ͼ���ڽӾ�����ڽӱ�������ʵ�ֵķ�ʽ��
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
		K** _matrix;//����
		bool _isDirected;
	};

}


namespace Labels
{
	template<typename V,typename K>
	class GraphLink
	{
		struct Node  //������ͼ��ʵ���ϱߣ�ÿһ���ڵ㶼��Ȩ�أ�Ҳ���Ǳߣ�ͨ�����εõ���ʼĩ�˵�ı�
		{
			Node *_next;
			K _key;  //Ȩ��
			V _dst;
			V _src;
			Node(V src, V dst, const K& key = K())
				:_next(NULL)
				, _key(key) 
				, _dst(dst)//ĩ��
				, _src(src)//ʼ��
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
				tmp->_next = _tables[index2];/*���ֻ����Ӹ�ÿ���ڵ����Ȩ�ؾͲ��ö�дtmp������Ļ�����������ͼ�����������㶼���� ʼ��  */
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
				tab[i] = false; //�������
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

		void BFS(const V& value)  //������Ĳ��Ǳߣ����Ƕ˵�
		{
			bool *tab = new bool[_sz];
			for (int i = 0; i < _sz; i++)
			{
				tab[i] = false;
			}
			 
			size_t index = Getindex(value);
			queue<size_t> q;
			q.push(index);
			tab[index] = true;//����ֻҪѹ����о���true
			_BFS(q, tab);

			for (int i = 0; i < _sz; i++)
			{ 
				if (i == index)
					i++;
				if (tab[i] == false)
				{	
					q.push(i);
					tab[index] = true;//����ֻҪѹ����о���true
					_BFS(q, tab);
				}		
			}
			cout << endl;
		}
		//��С������

		void Print()
		{
			for (int i = 0; i < _sz; i++)
			{
				while (_tables[i] == NULL)/*�������ͼʱ���еĶ���ֻ��Ϊĩ�ˣ���������������ö��㣬��Ĵ�С�ֺ�����һ�£������tables�ڲ��е�Ԫ��ΪNULL*/
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
					size_t curIndex = Getindex(cur->_dst);//��һ�����У���_src��һ��
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
				if (_tables[index]== NULL)			//tables ����ŵ���Ȩ�صĽڵ�
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
	string a[] = { "����", "����", "�Ϻ�", "�Ͼ�", "����" };
	Matrix::GraphMatrix<string, int > gm(a, 5,true);
	gm.AddEdge("����","����",100);
	gm.AddEdge("�Ϻ�", "����",200);
	gm.AddEdge("�Ͼ�", "����", 300);
	gm.AddEdge("����", "�Ϻ�", 400);	
	gm.AddEdge("����", "�Ϻ�", 500);
	cout << "DFS:  ";
	gm.DFS("����");
	cout << "BFS:  ";
	gm.BFS("����");

	cout << "---------------" << endl;
	gm.Print();

}

void GraphTextTables()
{
	string a[] = { "����", "����", "�Ϻ�", "�Ͼ�", "����" };
	Labels::GraphLink<string, int> gm(a, 5,true);//����ͼ ����ͨͼ
	gm.AddEdge("����", "����", 100);
	gm.AddEdge("�Ϻ�", "����", 200);
	gm.AddEdge("����", "�Ϻ�", 200);
	gm.AddEdge("����", "�Ͼ�", 300);
	gm.AddEdge("����", "����", 400);
	gm.AddEdge("����", "�Ϻ�", 500);
	cout <<"DFS:" << " ";
	gm.DFS("����",5);
	cout << "BFS:" << " ";
	gm.BFS("����");
	cout << "------------ "<<endl;
	gm.Print();
}


