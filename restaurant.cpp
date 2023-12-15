#include "main.h"
int MAXSIZE;
//////

//////////Huffman Tree
// Huffman tree node abstract base class
template <typename E> 
class HuffNode {
public:
	virtual ~HuffNode() {} // Base destructor
	virtual int weight() = 0; // Return frequency
	virtual bool isLeaf() = 0; // Determine type
};
template <typename E> // Leaf node subclass
class LeafNode : public HuffNode<E> {
private:
	E it; // Value ->char
	int wgt; // Weight
public:
	LeafNode(const E& val, int freq) // Constructor
	{ it = val; wgt = freq; }
	int weight() { return wgt; }
	E val() { return it; }
	bool isLeaf() { return true; }
};
template <typename E> // Internal node subclass
class IntlNode : public HuffNode<E> {
private:
	HuffNode<E>* lc; // Left child
	HuffNode<E>* rc; // Right child
	int wgt; // Subtree weight
public:
	IntlNode(HuffNode<E>* l, HuffNode<E>* r)
	{ wgt = l->weight() + r->weight(); lc = l; rc = r; }
	int weight() { return wgt; }
	bool isLeaf() { return false; }
	HuffNode<E>* left() const { return lc; }
	void setLeft(HuffNode<E>* b)
	{ lc = (HuffNode<E>*)b; }
	HuffNode<E>* right() const { return rc; }
	void setRight(HuffNode<E>* b)
	{ rc = (HuffNode<E>*)b; }
	~IntlNode(){
		delete lc;
		delete rc;
	}
};
// HuffTree is a template of two parameters: the element
// type being coded and a comparator for two such elements.
template <typename E>
class HuffTree {
private:
	HuffNode<E>* Root; // Tree root
public:
	HuffTree(E& val, int freq) // Leaf constructor
	{ Root = new LeafNode<E>(val, freq); }
	// Internal node constructor
	HuffTree(HuffTree<E>* l, HuffTree<E>* r)
	{ Root = new IntlNode<E>(l->root(), r->root()); }
	~HuffTree() {} // Destructor
	HuffNode<E>* root() { return Root; } // Get root
	int weight() { return Root->weight(); } // Root weight
};
///heapmin
template<typename E>
class heapHuffman{
	public:
		vector<HuffTree<E>*>element;
		int count;
		int capacity;
		heapHuffman(vector<pair<E,int>> caesar,int count){
			this->count=count;
			this->capacity=count;
			for(int i=0;i<count;i++){
				element[i]=new HuffTree<E>(caesar[i].first,caesar[i].second);
			}
		}
		static void reheapDown(vector<HuffTree<E>*>minHeap, int numberOfElements, int index)
		{
			int left=index*2+1;
			int right=index*2+2;
			if(left>=numberOfElements) return;
			if(right>=numberOfElements){
				if(minHeap[index]->wgt>minHeap[left]->wgt){
					swap(minHeap[index],minHeap[left]);
				}
			}
			else{
				int child=minHeap[left]->wgt<=minHeap[right]->wgt?left:right;
				if(minHeap[index]->wgt>minHeap[child]->wgt){
					swap(minHeap[index],minHeap[child]);
					reheapDown(minHeap,numberOfElements,child);
				}
				
			}
		}
		static void reheapUp(vector<HuffTree<E>*>minHeap, int numberOfElements, int index)
		{
			if(!index) return;
			int parent= (index-1)/2;
			if(minHeap[parent]->wgt>minHeap[index]->wgt){
				swap(minHeap[parent],minHeap[index]);
				reheapUp(minHeap,numberOfElements,parent);
			}
		}
		HuffTree<E>* removefirst(){
			HuffTree<E>*temp=element[0];
			element[0]=element[--count];
			reheapDown(element,count,0);
			return temp;
		}
		void balance(){

		}
		void insert(HuffTree<E>*temp){
			element[count++]=temp;
			reheapUp(element,count,count-1);
		}
		~heapHuffman(){
			delete element[0]->root();
			delete[] element;
		}
};
// Build a Huffman tree from a collection of frequencies
template <typename E> 
HuffTree<E>*buildHuff(vector<pair<E,int>> caesar,int count) {
	heapHuffman<E>* forest =new heapHuffman<E>(caesar,count);
	HuffTree<char> *temp1, *temp2, *temp3 = NULL;
	while (forest->size() > 1) {
		temp1 = forest->removefirst(); // Pull first two trees
		temp2 = forest->removefirst(); // off the list
		temp3 = new HuffTree<E>(temp1, temp2);
		forest->insert(temp3); // Put the new tree back on list
		delete temp1; // Must delete the remnants
		delete temp2; // of the trees we created
	}
return temp3;
}
/// ////////////////build

class gojo{
	public:
	int maxsize;
	gojo(int MAXSIZE){
		maxsize=MAXSIZE;
	}
	class BST{
		public:
		class node{
			public:
				int data;
				node*left;
				node*right;
				node(int result){
					this->data=result;
					left=nullptr;
					right=nullptr;
				}
		};
		node*root;
		node*add(int result,node*&subroot){
			if(!subroot){
				subroot=new node(result);
				return subroot;
			}
			if(result<subroot->data){
				subroot->left=add(result,subroot->left);
			}
			else{
				subroot->right=add(result,subroot->right);
			}
			return subroot;
		}
		void insert(int result){
			root=add(result,root);
		}
		node*findMin(node*subroot){
			 while (subroot->left) {
            	subroot = subroot->left;
        	}
        	return subroot;
		}
		void del(int val,node*root){
			if (!root) {
            	return;
        	}

			if (val < root->data) {
				del(val, root->left);
			}
			else if (val > root->data) {
				del(val, root->right);
			}
			else {
				// Node with the value to delete found
				if (!root->left) {
					node* temp = root->right;
					delete root;
					root = temp;
				}
				else if (!root->right) {
					node* temp = root->left;
					delete root;
					root = temp;
				}
				else {
					// Node with two children, find in-order successor
					node* temp = findMin(root->right);
					root->data = temp->data;
					del(temp->data, root->right);
				}
        	}
		}
		void clear(node*root){
			if(!root) return;
			clear(root->left);
			clear(root->right);
			delete root;
			root=nullptr;
		}
		~BST(){
			clear(root);
		}	
		
	};
	BST**hash=new BST*[maxsize+1];
	void add(int result){
		int id=result%maxsize+1;//id from 1 to maxsize
		hash[id]->insert(id);
	}
	void clear(){
		for(int i=0;i<=maxsize;i++){
			delete[] hash[i];
		}
		delete[] hash;
	}
	~gojo(){
		clear();
	}

};
class sukuna {
	public:
	int maxsize;
	static int time;
	class area{
		public:
		queue<int>customer;
		int id;//khac voi vi tri heap, m nham r
		int sum;
		int timeIn;
		area(){
			sum=0;
		}
		void insert(int result){
			customer.push(result);
			sum++;
			timeIn=time;
		}
	};
	vector<area> MinHeap;
	int findId(int id){//find position of id in Minheap
		for (unsigned i = 0; i < MinHeap.size(); i++)
		{
			if(MinHeap[i].id==id) return (int)i;
		}
		return -1;
		
	}
	static void reheapDown(vector<int> &minHeap, int numberOfElements, int index){
		int left=index*2+1;
		int right=index*2+2;
		if(left>=numberOfElements) return;
		if(right>=numberOfElements){
			if(minHeap[index]>minHeap[left]){
				swap(minHeap[index],minHeap[left]);
			}
		}
		else{
			int child=minHeap[left]<=minHeap[right]?left:right;
			if(minHeap[index]>minHeap[child]){
				swap(minHeap[index],minHeap[child]);
				reheapDown(minHeap,numberOfElements,child);
			}
			
		}
	}
	static void reheapUp(vector<int> &minHeap, int numberOfElements, int index){
		if(!index) return;
		int parent= (index-1)/2;
		if(minHeap[parent]>minHeap[index]){
			swap(minHeap[parent],minHeap[index]);
			reheapUp(minHeap,numberOfElements,parent);
		}
	}
	void remove(vector<int> &nums){
		int s=nums.size();
		nums[0]=nums[s-1];
		reheapDown(nums,s,0);
	}
	void add(int result){
		int id=result%maxsize+1;
		time++;
		int position=findId(id);
		if(position!=-1){//area da co
			MinHeap[(unsigned)position].insert(result);
		}
		else{//chua co area ->dua vao minheap
			area temp;
			temp.insert(result);
			temp.id=id;
			MinHeap.push_back(temp);
		}
		int N=MinHeap.size();
		for(int i=N/2-1;i>=0;i--){
			reheapDown(nums,N,i);
		}
	}
};

//////////////////////////////////////////////////////
//compare to sort caesar code
bool comp ( pair<char,int>& l,  pair<char,int>& r)  {
	
    if (l.second != r.second) {
        return l.second < r.second;
    }
	if(l.first>='a'&&r.first<'a'){
		return l.first > r.first;
	}
    return l.first < r.first;
}
//encode caesar code--ki tu nho, f nho->dung trc
vector<pair<char,int>> encode(map<char,int> &code){
	map<char,int> Newmap;
	//co bug ve ki tu->fix nhanh dc
	for(auto it=code.begin();it!=code.end();it++){
		if(Newmap.empty()){
			if(it->first>='a'){
				Newmap.emplace('a'+(it->first-'a'+it->second)%26,it->second);
			}
			else{
				Newmap.emplace('A'+(it->first-'A'+it->second)%26,it->second);
			}
			
		}
		else{
			if(Newmap.find('a'+(it->first-'a'+it->second)%26)!=Newmap.end()&&it->first>='a'){// co kitu giong nhau
				Newmap['a'+(it->first-'a'+it->second)%26]+=it->second;
			}
			
			else if(Newmap.find('A'+(it->first-'A'+it->second)%26)!=Newmap.end()&&it->first>='A'&&it->first<='Z'){//co kitu giong nhau
				Newmap['A'+(it->first-'A'+it->second)%26]+=it->second;
			}
			else {
				if(it->first>='a'){
					Newmap.emplace('a'+(it->first-'a'+it->second)%26,it->second);
				}
				else{
					Newmap.emplace('A'+(it->first-'A'+it->second)%26,it->second);
				}
			}
		}
	}
	//complete newmap
	//transfer vector pair to sort
	vector<pair<char,int>> caesar;
	for(auto &it:Newmap){
		caesar.push_back(it);
	}
	sort(caesar.begin(),caesar.end(),comp);
	return caesar;
}

void lapse(string name){
	// fisrt step, calculate frequency
	map<char,int> code;
	for(unsigned i=0;i<name.size();i++){
		if(i==0){
			//new map, don't have data-> add
			code.emplace(name[i],1);
		}
		else{
			if(code.find(name[i])!=code.end()){
				//if have similar char -> code.second++
				code[name[i]]++;
			}
			else{
				code.emplace(name[i],1);
			}
		}
	}
	cout<<"before encode"<<endl;
	for (auto& it : code) {
        cout << it.first << ' ' << it.second << endl;
    }
	//encode caesar code
	cout<<"after encode"<<endl;
	vector<pair<char,int>> list_X=encode(code);
		for (auto& it : list_X) {
        cout << it.first << ' ' << it.second << endl;
    }
	//xu li dua vao huffman

	int result;
	//ra dc result
	if(result%2==0){

	}
	else{

	}
}
void keiteiken(int num){
	cout<<num<<endl;
	cout<<"keiteiken_func"<<endl;
}
void hand(){
	cout<<"hand_func"<<endl;
}




///////// read file
void simulate(string filename)
{	//chua doc file->doc file r lam, thuc hien cac class
	ifstream ss(filename);
	string str, maxsize, name, num;
	while(ss >> str)
	{ 	cout<<str<<endl;
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoi(maxsize); 
    	}
        else if(str == "LAPSE") // RED <NAME> <ENERGY>
        {
            ss >> name;
            lapse(name);
    	}
		else if(str=="KEITEIKEN"){
			ss>>num;
			keiteiken(stoi(num));

		}
    	else if(str=="HAND"){
			hand();
		}
		else if(str=="LIMITLESS"){
			ss>>num;

		}
		else if(str=="CLEAVE"){
			ss>>num;
		}
		else if(str=="KOKUSEN"){

		}
    }
	return;
}
//////////////////////////////////////////////////////////////////////////////
