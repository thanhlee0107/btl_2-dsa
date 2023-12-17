#include "main.h"
long long MAXSIZE;
//////
const int mod = 1e9+7;


//////////Huffman Tree
// Huffman tree node abstract base class
class Node{
	public:
	Node*left;
	Node*right;
	int weight;
	int height;
	char c;
	Node(int weight, char c='\0',Node*left=nullptr,Node*right=nullptr) : weight(weight),left(left),right(right),c(c),height(1){}
};
//
int max(int a, int b)  
{  
    return (a > b)? a : b;  
}  

int height(Node *N) 
{ 
	if (!N) 
		return 0; 
	return N->height; 
}
int getBalance(Node *N) 
{ 
	if (!N) 
		return 0; 
	return height(N->left) - height(N->right); 
} 
Node* rotateLeft(Node*subroot){
		Node*mid=subroot->right;
		Node*end=mid->left;
		mid->left=subroot;
		subroot->right=end;
		subroot->height = max(height(subroot->left), height(subroot->right)) + 1; 
		mid->height = max(height(mid->left), height(mid->right)) + 1;
		//cout<<"da chon"<<endl;
		return mid;
}
Node* rotateRight(Node*subroot){
		Node*mid=subroot->left;
		Node*end=mid->right;
		mid->right=subroot;
		subroot->left=end;
		subroot->height = max(height(subroot->left), height(subroot->right)) + 1; 
		mid->height = max(height(mid->left), height(mid->right)) + 1;
		return mid;
}
Node*balanceNode(int& count,Node*root){
	//if(count>=3) return root;//chuwa ngi ra
	int balance=getBalance(root);
	//left left or left left&&left right
	root->height = 1 + max(height(root->left), height(root->right)); 

	if(balance>1&&height(root->left->left)>=height(root->left->right)){
		count++;
		//cout<<1<<endl;
		return rotateRight(root);
	}
	//right right or right right&&right left
	else if(balance<-1&&height(root->right->left)<=height(root->right->right)){
		count++;
		//cout<<2<<endl;
		return rotateLeft(root);
	}
	//left of right
	else if(balance<-1&&height(root->right->left)>height(root->right->right)){
		count++;
		root->right=rotateRight(root->right);
		//cout<<3<<endl;
		if(count>=3) return root;
		count++;
		return rotateLeft(root);
	}
	//right of left
	else if(balance>1&&height(root->left->right)>height(root->left->left)){
		count++;
		root->left=rotateLeft(root->left);
		//cout<<4<<endl;
		if(count>=3) return root;
		count++;
		return rotateRight(root);
	}
	return root;

}
Node*balancetree(int& count,Node*root){
	if(count>=3) return root;
	root=balanceNode(count,root);
	if(count<3&&root->left) root->left=balancetree(count,root->left);
	if(count<3&&root->right)root->right=balancetree(count,root->right);
	return root;
}
//build huff
int found(vector<Node*>temp,int value){
    for(int i=0;i<(int)temp.size();i++){
        if(temp[i]->weight<=value) return i;
    }
    return -1;
}
//build
//Node(int weight, char c='\0',Node*left=nullptr,Node*right=nullptr) : weight(weight), height(height),left(left),right(right),c(c){}

Node*buildHuffman(vector<pair<char,int>>fre){
	vector<Node*> huff;
	for(auto it=fre.begin();it!=fre.end();it++){
		Node*temp=new Node(it->second,it->first,nullptr,nullptr);
		huff.push_back(temp);
	}
	while(huff.size()>1){
		Node*pleft=huff.back();
		//cout<<pleft->weight<<" "<<pleft->height<<endl;
		huff.pop_back();
		Node*pright=huff.back();
		//cout<<pright->weight<<" "<<pright->height<<endl;
		huff.pop_back();
		int count=0;
		Node* newhuff= new Node(pright->weight+pleft->weight,'\0',pleft,pright);
		newhuff=balancetree(count,newhuff);
		//cout<<"newhuff: "<<newhuff->weight<<" "<<newhuff->height<<endl;
		int pos=found(huff,newhuff->weight);
		if(pos==-1) huff.push_back(newhuff);
		else huff.insert(huff.begin()+pos,newhuff);
	}
	return huff.front();
}
//
string findHuffmanCode(Node* root, char target, string str = "") {
    if (root == nullptr) {
        return "";
    }

    // Nếu đây là một nút lá và chứa ký tự muốn tìm
    if (root->c == target) {
        return str;
    }

    string sleft = findHuffmanCode(root->left, target, str + "0");
    if (!sleft.empty()) {
        return sleft;
    }

    string sright = findHuffmanCode(root->right, target, str + "1");
    if (!sright.empty()) {
        return sright;
    }

    return "";
}


long long binary(vector<pair<char,int>> caesar, Node*huffman){
	string bin="";
	for(int i=(int)caesar.size()-1;i>=0;i--){
		bin+=findHuffmanCode(huffman,caesar[(unsigned)i].first,"");
		if(bin.size()>=10)break;
	}
	//cout<<"chuyen binary"<<endl;
	long long sum=0;
	if(bin.size()>10){
		for(int i=0;i<10;i++){
			if(bin[(unsigned)i+bin.size()-10]=='1'){
				sum+=pow(2,i);
			}
		}
	}
	else{
		for(int i=0;i<(int)bin.size();i++){
			if(bin[i]=='1'){
				sum+=pow(2,i);
			}
		}
	}

	return sum;
}
/// ////////////////build

class gojo{
	public:
	long long maxsize;
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
		vector<long long> customer;
		int count;
		BST(){
			count=0;
			root=nullptr;
		}
		node*add(long long result,node*&subroot){
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
		void insert(long long result){
			root=add(result,root);
			customer.push_back(result);
			count++;
		}
		node*findMin(node*subroot){
			 while (subroot->left) {
            	subroot = subroot->left;
        	}
        	return subroot;
		}
		void del(long long val,node*&subroot){
			if (!subroot) {
            	return;
        	}

			if (val < subroot->data) {
				del(val, root->left);
			}
			else if (val > subroot->data) {
				del(val, subroot->right);
			}
			else {
				// Node with the value to delete found
				if (!subroot->left) {
					node* temp = subroot->right;
					delete subroot;
					subroot = temp;
				}
				else if (!subroot->right) {
					node* temp = subroot->left;
					delete subroot;
					subroot = temp;
				}
				else {
					// Node with two children, find in-order successor
					node* temp = findMin(subroot->right);
					subroot->data = temp->data;
					del(temp->data, subroot->right);
				}
        	}
		}
		void remove(long long Y){
				if(Y<customer.size()){
					while(Y--){
					del(customer.front(),root);
					customer.erase(customer.begin());
					}
				}
				else{
					clear(root);
				}
			
		}
		//TINH SO HOAN VI POSTFIX
		void PascalTriangle(int n,vector<vector<int>>&dp){
			for(int i=0;i<=n;i++){
				dp[i] =  vector<int>(i+1,1); //initially all 1 
				//Now Apply combination logic of adding prev_row and prev_col 
				for(int j=1;j<i;j++){
					dp[i][j] = (dp[i-1][j-1] + dp[i-1][j])%mod;
				}
			}
		}
			int dfs(vector<long long>&nums,vector<vector<int>>&dp){
				vector<long long>leftSubTree , rightSubTree ; 
				int n = nums.size() ; 

				if(n<3)  return 1;

				for(int i=1;i<n;i++){
					if(nums[i]>=nums[0]) rightSubTree.push_back(nums[i]);
					if(nums[i]<nums[0]) leftSubTree.push_back(nums[i]);
				} 

				int left = dfs(leftSubTree,dp)%mod ; 
				int right = dfs(rightSubTree,dp)%mod ; 
				int ele_atLeft = leftSubTree.size() ; 

				// n-1Cele_atLeft * left * right 
				return (1ll * (1ll * dp[n-1][ele_atLeft]*1ll * left)%mod * right)%mod ; 
			}
			long long numOfWays(vector<long long>& nums) {
				
				int n = nums.size() ; 
				vector<vector<int>>dp(n+1) ; 
				PascalTriangle(n,dp);
				long long ways = dfs(nums,dp)-1; 
				return ways;
			}
		//

		//
		void print(node*subroot){
			if(!subroot) return;
			print(subroot->left);			
			cout<<subroot->data<<endl;
			print(subroot->right);
		}
		void clear(node*subroot){
			if(!subroot) return;
			clear(subroot->left);
			clear(subroot->right);
			delete subroot;
			subroot=nullptr;
		}
		~BST(){
			clear(root);
		}	
		
	};
	BST**hash;
	gojo(int size){
		hash=new BST*[size+1];
		this->maxsize=size;
		for(int i=1;i<=maxsize;i++){
			hash[i]=new BST();
		}
	}
	void kick(){
		for(long long i=1;i<=maxsize;i++){
			long long y=hash[i]->numOfWays(hash[i]->customer);
			y=y%maxsize;
			hash[i]->remove(y);
		}
	}
	void printInorder(long long num){
			if(num>maxsize||hash[num]->count==0){
				//do nothing
			}
			else{
				hash[num]->print(hash[num]->root);
			}
		}
	void add(long long result){
		//cout<<"gojjo add"<<endl;
		long long id=result%maxsize+1;//id from 1 to maxsize
		//cout<<"id gojjo:"<<id<<endl;
		hash[id]->insert(result);
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
	long long maxsize;
	int time;
	sukuna(long long max){
		maxsize=max;
		time=0;
	}
	class area{
		public:
		queue<long long>customer;
		long long id;//khac voi vi tri heap, m nham r
		int sum;
		int timeIn;
		area(){
			sum=0;
			timeIn=0;
		}
		void insert(long long result,int timec){
			customer.push(result);
			sum++;
			timeIn =timec;
		}
	};
	vector<area> MinHeap;
	int findId(long long id){//find position of id in Minheap
		for (unsigned i = 0; i < MinHeap.size(); i++)
		{
			if(MinHeap[i].id==id) return (int)i;
		}
		return -1;
		
	}
	void reheapDown(vector<area> &minHeap, int numberOfElements, int index){
		int left=index*2+1;
		int right=index*2+2;
		if(left>=numberOfElements) return;
		if(right>=numberOfElements){
			if(minHeap[index].sum>minHeap[left].sum){
				swap(minHeap[index],minHeap[left]);
			}
			else if(minHeap[index].sum==minHeap[left].sum){
				if(minHeap[index].timeIn>minHeap[left].timeIn){
					swap(minHeap[index],minHeap[left]);
				}
			}
		}
		else{
			if(minHeap[left].sum>minHeap[right].sum){
				if(minHeap[index].sum>minHeap[right].sum){
					swap(minHeap[index],minHeap[right]);
					reheapDown(minHeap,numberOfElements,right);
				}
				else if(minHeap[index].sum==minHeap[right].sum){
						if(minHeap[index].timeIn>minHeap[right].timeIn){
							swap(minHeap[index],minHeap[right]);
							reheapDown(minHeap,numberOfElements,right);
						}
				}
			}
			else if(minHeap[left].sum<minHeap[right].sum){
				if(minHeap[index].sum>minHeap[left].sum){
					swap(minHeap[index],minHeap[left]);
					reheapDown(minHeap,numberOfElements,left);
				}
				else if(minHeap[index].sum==minHeap[left].sum){
						if(minHeap[index].timeIn>minHeap[left].timeIn){
							swap(minHeap[index],minHeap[left]);
							reheapDown(minHeap,numberOfElements,left);
						}
				}
			}
			else{// sum==
				int child = minHeap[right].timeIn>minHeap[left].timeIn ? left : right;
					if(minHeap[index].sum>minHeap[child].sum){
						swap(minHeap[index],minHeap[child]);
						reheapDown(minHeap,numberOfElements,child);
					}
					else if(minHeap[index].sum==minHeap[child].sum){
							if(minHeap[index].timeIn>minHeap[child].timeIn){
								swap(minHeap[index],minHeap[child]);
								reheapDown(minHeap,numberOfElements,child);
							}
					}
			}
			
		}
	}
	//chua fix reheapup


	//
	void reheapUp(vector<area> &minHeap, int numberOfElements, int index){
		if(!index) return;
		int parent= (index-1)/2;
		if(minHeap[parent].sum>minHeap[index].sum){
			swap(minHeap[parent],minHeap[index]);
			reheapUp(minHeap,numberOfElements,parent);
		}
		
	}
	void remove(vector<area> &nums){
		int s=(int)nums.size();
		nums[0]=nums[s-1];
		int i=0;
		nums.erase(nums.begin()+s-1);
		reheapDown(nums,s-1,i);
	}

	void cut(long long num){
		if((unsigned)num>MinHeap[0].sum){
			while(!MinHeap[0].customer.empty()){
				cout<<MinHeap[0].customer.front()<<"-"<<MinHeap[0].id<<endl;
				MinHeap[0].customer.pop();
			}			
			remove(MinHeap);
		}
		else{
			while(num--){
				cout<<MinHeap[0].customer.front()<<"-"<<MinHeap[0].id<<endl;
				MinHeap[0].customer.pop();
				num>MinHeap[0].sum--;
			}
		}
	}
	void add(long long result){
		//cout<<"success"<<endl;
		long long id=result%maxsize+1;
		time++;
		int position=findId(id);
		if(position!=-1){//area da co
			MinHeap[(unsigned)position].insert(result,time);
		}
		else{//chua co area ->dua vao minheap
			area temp;
			temp.insert(result,time);
			temp.id=id;
			MinHeap.push_back(temp);
			reheapUp(MinHeap,MinHeap.size(),MinHeap.size()-1);
		}
		int N=(int)MinHeap.size();
		for(int i=N/2-1;i>=0;i--){
			reheapDown(MinHeap,N,i);
		}
	}
	void printPreorder(long long num){//can than thu tu vector
		
			print(MinHeap,0,num);
		
	}
	void print(vector<area>Minheap,int index,long long num){
		if(index>=MinHeap.size()) return ;
		long long wide= num<=Minheap[index].sum? num:Minheap[index].sum;
		queue<long long> temp=MinHeap[index].customer;
		for(long long i=0;i<wide;i++){
			cout<<Minheap[index].id<<"-"<<temp.front()<<endl;
			temp.pop();

		}
			print(Minheap,index*2+1,num);
			print(Minheap,index*2+2,num);
		
	}
};
//

stack<Node*>huff;
	gojo* thay5;
	sukuna* catdoi;
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
	//   cout<<"before encode"<<endl;
	//   for (auto& it : code) {
    //     cout << it.first << ' ' << it.second << endl;
    //  }
	//encode caesar code
	 //cout<<"after encode"<<endl;
	if(code.size()<3) return;
	 vector<pair<char,int>> list_X=encode(code);
	 vector<pair<char,int>> temp=list_X;
	 reverse(temp.begin(), temp.end());
	 Node*huf=buildHuffman(temp);
	//   	for (auto& it : list_X) {
    //       cout << it.first << ' ' << it.second << endl;
    //   }
	//xu li dua vao huffman

	long long result=binary(list_X,huf);
	//ra dc result
	if(result%2==0){
		thay5->add(result);
	}
	else{
		catdoi->add(result);
	}
	huff.push(huf);
}
void keiteiken(long long num,sukuna*cut){
	//cout<<num<<endl;
	cut->cut(num);
	//cout<<"keiteiken_func"<<endl;
}
//decode

void printHuf(Node*huf){
	if(!huf) return;
	printHuf(huf->left);
	if(huf->c!='\0'){
		cout<<huf->c<<endl;
	}
	else{
		cout<<huf->weight<<endl;
	}
	printHuf(huf->right);
}
void hand(){
	printHuf(huff.top());

}
void kokusen(gojo* thay){
	//cout<<"kokusen"<<endl;
	thay->kick();
}
void limitless(long long num,gojo* thay){
	thay->printInorder(num);
	//cout<<"thay5"<<endl;
}
void cleave(long long num,sukuna* catdoi2){
	catdoi2->printPreorder(num);
	//cout<<"sukumn"<<endl;
}



///////// read file

void simulate(string filename)
{	//chua doc file->doc file r lam, thuc hien cac class
	ifstream ss(filename);
	string str, maxsize, name, num;
	
	while(ss >> str)
	{ 	//cout<<str<<endl;
		if(str == "MAXSIZE")
		{
			ss >> maxsize;
			MAXSIZE = stoll(maxsize); 
			thay5=new gojo(MAXSIZE);
			catdoi=new sukuna(MAXSIZE);
    	}
        else if(str == "LAPSE") // RED <NAME> <ENERGY>
        {
            ss >> name;
            lapse(name);
    	}
		else if(str=="KEITEIKEN"){
			ss>>num;
			keiteiken(stoll(num),catdoi);

		}
    	else if(str=="HAND"){
			hand();
		}
		else if(str=="LIMITLESS"){
			ss>>num;
			limitless(stoll(num),thay5);
		}
		else if(str=="CLEAVE"){
			ss>>num;
			cleave(stoll(num),catdoi);
		}
		else if(str=="KOKUSEN"){

		}
    }
	delete thay5;
	delete catdoi;
	return;
}
//////////////////////////////////////////////////////////////////////////////
