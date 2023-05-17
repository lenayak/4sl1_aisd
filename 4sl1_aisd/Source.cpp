#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

class Tree
{
private:
	struct Node
	{
		int data;
		Node* left, * right;;
		Node(int value)
		{
			data = value;
			left = right = nullptr;
		}
	};
	Node* root;
	void deleteTree(Node* root)
	{
		if (root)
		{
			deleteTree(root->left);
			deleteTree(root->right);
			delete root;
		}
	}
	void printTree(const Node* root, int height) const
	{
		if (root)
		{
			printTree(root->right, height + 1);
			for (int i = 0; i < height; i++) cout << "     ";
			cout << root->data << endl;
			printTree(root->left, height + 1);
		}
	}
	void copy(Node* root)
	{
		if (root != nullptr)
		{
			this->insert(root->data);
			copy(root->left);
			copy(root->right);
		}
	}
	Node* find(Node* root, int key)
	{
		if (root == nullptr)
			return nullptr;
		if (key < root->data)
			return find(root->left, key);
		else if (key > root->data)
			return find(root->right, key);
		else return root;
	}
public:
	Tree(int value = NULL)
	{
		root = new Node(value);
	}
	Tree(const Tree& object)
	{
		if (!object.root)
		{
			root = nullptr;
		}
		copy(object.root);
		//Node* tmp = object.root;
	}
	~Tree()
	{
		deleteTree(root);
	}
	void print() const
	{
		printTree(root, 1);
	}
	void clear()
	{
		deleteTree(root);
		root = nullptr;
	}
	bool contains(int key) const     /*   проверка на наличие   */
	{
		Node* tmp = root;
		while (tmp && tmp->data != key)
		{
			if (tmp->data > key)
				tmp = tmp->left;
			else
				tmp = tmp->right;
		}
		return tmp;
	}
	bool insert(int key)  /*  вставка  */
	{
		if (!root)
		{
			root = new Node(key);
			return true;
		}
		Node* tmp = root;
		while (tmp)
		{
			if (tmp->data == key)
				return false;
			if (tmp->data > key && !tmp->left)
			{
				tmp->left = new Node(key);
				return true;
			}
			if (tmp->data < key && !tmp->right)
			{
				tmp->right = new Node(key);
				return true;
			}
			if (tmp->data > key)
				tmp = tmp->left;
			else
				tmp = tmp->right;
		}
		return true;
	}
	bool erase(int key)           /*  удаление элемента  */
	{
		Node* tmp = root;
		Node* parent = nullptr;
		while (tmp && tmp->data != key)
		{
			parent = tmp;
			if (tmp->data > key)
				tmp = tmp->left;
			else
				tmp = tmp->right;
		}
		if (!tmp)
			return false;
		if (!tmp->left)
		{
			if (parent && parent->left == tmp)
				parent->left = tmp->right;
			if (parent && parent->right == tmp)
				parent->right = tmp->right;
			delete tmp;
			return true;
		}
		if (!tmp->right)
		{
			if (parent && parent->left == tmp)
				parent->left = tmp->left;
			if (parent && parent->right == tmp)
				parent->right = tmp->left;
			delete tmp;
			return true;
		}
		Node* replace = tmp->right;
		while (replace && replace->left)
			replace = replace->left;
		int value = replace->data;
		erase(value);
		tmp->data = value;
		return true;
	}
	Tree& operator=(const Tree& object)
	{
		if (&object == this)
		{
			return *this;
			this->clear();
			copy(object.root);
			return *this;
		}
	}
};

vector<int> repeated(const vector<int>& original)     /*   вариант 3    */
{
	if (original.size() == 0)
		throw logic_error("empty vector");
	vector<int> result;
	for (int i = 0; i < original.size(); i++)
	{
		bool flag = true;
		for (int j = 0; j < original.size(); j++)
		{
			if (i == j)
				continue;
			else if (original[i] == original[j])
				flag = false;
		}
		if (!flag)
		{
			bool check = false;
			for (int k = 0; k < result.size(); k++)
			{
				if (result[k] == original[i])
					check = true;
			}
			if (!check)
				result.push_back(original[i]);
		}
	}
	return result;
}

int lcg(int& value)       /*  генератор случайных чисел  */
{
	value = (1021 * value + 24631) % 116640;
	return value;
}

void createTree(Tree& object, int amount, int& value)
{
	for (int i = 0; i < amount; i++)
	{
		object.insert(lcg(value));
	}
}

void createVector(vector<int> object, int amount, int& value)
{
	for (int i = 0; i < amount; i++)
	{
		object.push_back(lcg(value));
	}
}

void timePrint(chrono::duration<long long, nano> time, const char* dataStruct)
{
	cout << endl << dataStruct << endl;
	cout << "nanoseconds: " << chrono::duration_cast<chrono::nanoseconds>(time).count() << " ns" << endl;
	cout << "microseconds: " << chrono::duration_cast<chrono::microseconds>(time).count() << " ms" << endl;
	cout << "milliseconds: " << chrono::duration_cast<chrono::milliseconds>(time).count() << " mls" << endl;
	cout << "seconds: " << chrono::duration_cast<chrono::seconds>(time).count() << " sec" << endl;
}

void timeCompare(chrono::duration<long long, nano> timeTree, chrono::duration<long long, nano> timeVector)
{
	auto timeTreeTmp = chrono::duration_cast<chrono::nanoseconds>(timeTree).count();
	auto timeVectorTmp = chrono::duration_cast<chrono::nanoseconds>(timeVector).count();
	if (timeTreeTmp == timeVectorTmp)
		cout << "Execution time is the SAME" << endl;
	else if (timeTreeTmp < timeVectorTmp)
		cout << "Execution time for TREE structure is FASTER" << endl;
	else
		cout << "Execution time for VECTOR structure is FASTER" << endl;
}

void createTreeAndVector(Tree& object1, vector<int> object2, int amount, int& valueTree, int& valueVector)
{
	auto timetmp = chrono::steady_clock::now();
	chrono::duration<long long, nano> timeTree = timetmp - timetmp, timeVector = timetmp - timetmp;
	for (int i = 0; i < 100; i++)
	{
		auto startTree = chrono::steady_clock::now();
		createTree(object1, amount, valueTree);
		auto endTree = chrono::steady_clock::now();
		auto startVector = chrono::steady_clock::now();
		createVector(object2, amount, valueVector);
		auto endVector = chrono::steady_clock::now();
		timeTree += endTree - startTree;
		timeVector += endVector - startVector;
		if (i != 99)
		{
			object1.clear();
			object2.clear();
		}
	}
	cout << endl << "AVERAGE TIME TO FILL STRUCTURES WITH " << amount << " VALUES:" << endl;
	timePrint(timeTree / 100, "TREE");
	timePrint(timeVector / 100, "VECTOR");
	cout << endl << "Fill time comparison:" << endl;
	timeCompare(timeTree / 100, timeVector / 100);
}

void searchTreeAndVector(const Tree& object1, const vector<int>& object2, int& value, int amount)
{
	auto timetmp = chrono::steady_clock::now();
	chrono::duration<long long, nano> timeTree = timetmp - timetmp, timeVector = timetmp - timetmp;
	for (int i = 0; i < 1000; i++)
	{
		lcg(value);
		auto startTree = chrono::steady_clock::now();
		object1.contains(value);
		auto endTree = chrono::steady_clock::now();
		auto startVector = chrono::steady_clock::now();
		bool tmp = find(object2.begin(), object2.end(), value) != object2.end();
		auto endVector = chrono::steady_clock::now();
		timeTree = endTree - startTree;
		timeVector = endVector - startVector;
	}
	cout << endl << "AVERAGE LOOKUP TIME IN A STRUCTURES WITH " << amount << " VALUES:" << endl;
	cout << "Tree - " << chrono::duration_cast<chrono::nanoseconds>(timeTree / 1000).count() << endl;
	cout << "Vector - " << chrono::duration_cast<chrono::nanoseconds>(timeVector / 1000).count() << endl;
	timeCompare(timeTree / 1000, timeVector / 1000);
}

void addAndRemoveTreeAndVector(Tree& object1, vector<int>& object2, int& remove, int& add, int amount)
{
	auto timetmp = chrono::steady_clock::now();
	chrono::duration<long long, nano> timeTreeAdd = timetmp - timetmp, timeTreeRemove = timetmp - timetmp,
		timeVectorAdd = timetmp - timetmp, timeVectorRemove = timetmp - timetmp;
	for (int i = 0; i < 1000; ++i)
	{
		lcg(remove);
		lcg(add);
		auto startTreeRemove = chrono::steady_clock::now();
		object1.erase(remove);
		auto endTreeRemove = chrono::steady_clock::now();
		auto startTreeAdd = chrono::steady_clock::now();
		object1.insert(add);
		auto endTreeAdd = chrono::steady_clock::now();
		auto startVectorRemove = chrono::steady_clock::now();
		for (int i = 0; i < object2.size(); i++)
		{
			if (object2[i] == remove)
			{
				object2.erase(object2.begin() + i);
				break;
			}
		}
		auto endVectorRemove = chrono::steady_clock::now();
		auto startVectorAdd = chrono::steady_clock::now();
		object2.push_back(add);
		auto endVectorAdd = chrono::steady_clock::now();
		timeTreeAdd += endTreeAdd - startTreeAdd;
		timeVectorAdd += endVectorAdd - startVectorAdd;
		timeTreeRemove += endTreeRemove - startTreeRemove;
		timeVectorRemove += endVectorRemove - startVectorRemove;
	}
	cout << endl << "AVERAGE TIME TO REMOVE A RANDOM VALUE IN A STRUCTURES WITH " << amount << " VALUES:" << endl;
	cout << "Tree - " << chrono::duration_cast<chrono::nanoseconds>(timeTreeRemove / 1000).count() << endl;
	cout << "Vector - " << chrono::duration_cast<chrono::nanoseconds>(timeVectorRemove / 1000).count() << endl;
	timeCompare(timeTreeRemove / 1000, timeVectorRemove / 1000);
	cout << endl << "AVERAGE TIME TO ADD A RANDOM VALUE IN A STRUCTURES WITH " << amount << " VALUES:" << endl;
	cout << "Tree - " << chrono::duration_cast<chrono::nanoseconds>(timeTreeAdd / 1000).count() << endl;
	cout << "Vector - " << chrono::duration_cast<chrono::nanoseconds>(timeVectorAdd / 1000).count() << endl;
	timeCompare(timeTreeAdd / 1000, timeVectorAdd / 1000);
}

int main()
{
	Tree tree;
	vector<int> v, number{ 1000, 10000, 100000 };
	int x = 0, y = 0, z = 0, d = 0;
	for (int i : number)
	{
		createTreeAndVector(tree, v, i, x, y);
		tree.clear();
		v.clear();
	}
	for (int k : number)
	{
		x = 0;
		y = 0;
		z = 0;
		d = 0;
		createTree(tree, k, x);
		createVector(v, k, y);
		searchTreeAndVector(tree, v, z, k);
		addAndRemoveTreeAndVector(tree, v, d, z, k);
		cout << v.size() << endl;
		tree.clear();
		v.clear();
	}
}