#include <set>
#include<iostream>
#include<iterator>
#include<algorithm>
#include<chrono>
#include<ctime>
#include<stack>
#include<vector>
#include<map>
using namespace std;
typedef  long long ll;
vector<int>v;
int q;
class RBT {

public:
	enum  clr { red, black };
	struct node {
		node * left, *right;
		node *par;
		clr color;
		int value;
		int vcnt;
		int cnt;
		node() :value(0), left(nullptr), right(nullptr),color(red),vcnt(0),cnt(0) {
			
		}
		~node() {
			
			//cout << "delete" << value << endl;
		}
	};
	node * root, *nil;
public:
	RBT() {
		nil = new node();
		nil->par = nil;
		nil->color = black;
		root=nil;
	}
	~RBT() {
		stack<node*> st;
		st.push(root);
		while (!st.empty()) {
			node * tp = st.top();
			st.pop();
			if (tp->left != nil)st.push(tp->left);
			if (tp->right != nil)st.push(tp->right);
			delete tp;
		}
	}
	node * grandparent(node * self) {
		if (self == nullptr)return nullptr;
		return self->par->par;
		
	}
	node * uncle(node * self) {
		if (self == nullptr)
			return nullptr;
		node * fa=self->par;
		node * gp=grandparent(self);
		if (gp->left == fa) {
			return gp->right;
		}
		else return gp->left;
	}
	void rotate_left(node * self) {
		/*if (self->par == nil) {
			root = self;
			return;
		}*/
		//node * gp = grandparent(self);
		node * fa = self->par;
		node * y = self->right;

		self->right = y->left;
		if (self->right != nil) {
			self->right->par = self;
		}

		y->left = self;
		self->par = y;
		y->par = fa;
		if (root == self)root = y;
		
		if (fa != nil) {
			if (fa->left == self) {
				fa->left = y;
			}
			else fa->right = y;
		}
		self->cnt = self->left->cnt + self->right->cnt + self->vcnt;
		y->cnt = y->left->cnt + y->right->cnt + y->vcnt;
	}
	void rotate_right(node * self) {
		node *fa = self->par;
		node *y = self->left;
		self->left = y->right;
		if (self->left != nil) {
			self->left->par = self;
		}
		y->right = self;
		self->par = y;
		y->par = fa;

		if (root==self)root = y;
		if (fa != nil) {
			if (fa->left == self) {
				fa->left = y;
			}
			else fa->right = y;
		}
		self->cnt = self->left->cnt + self->right->cnt + self->vcnt;
		y->cnt = y->left->cnt + y->right->cnt + y->vcnt;
	}
	void insert_up(node * self) {
		/*if (self->color == black)return;
		
		if(self->par == nil) {
			self->color=black;
			root = self;
		}*/
		if (self->color == red && (self->left->color == red || self->right->color == red)) {
			cout << "deg" << endl;
		}
		else if(self->par->color==red){
			if (uncle(self)->color == red) {
				self->par->color = uncle(self)->color = black;
				grandparent(self)->color = red;
				insert_up(grandparent(self));
			}
			else {
				if (grandparent(self)->left == self->par) {
					if (self->par->right == self) {
						rotate_left(self->par);
						insert_up(self->left);
					}
					else {
						self->par->color = black;
						grandparent(self)->color = red;
						rotate_right(grandparent(self));
						//
					}
				}
				else if (grandparent(self)->right == self->par) {
					// right letf
					if (self->par->left == self) {
						rotate_right(self->par);
						insert_up(self->right);
					}
					else {
						self->par->color = black;
						grandparent(self)->color = red;
						rotate_left(grandparent(self));
					}
				}
			}
		}
	}
	int insert(int num) {
		node * y =nil;
		
		//z->par;

		node * x = root;
		while (x != nil) {
			x->cnt++;
			y = x;
			if (x->value == num) {
				x->vcnt++;
				return 0;
			}
			if (x->value < num) {
				x = x->right;
			}
			else {
				x = x->left;
			}
		}
		node * z(new node());
		z->value = num;
		z->left = nil;
		z->right = nil;
		z->color = red;
		z->par = y;
		z->cnt = z->vcnt = 1;
		if (y == nil) {
			root = z;
		}
		else {
			if (y->value < z->value) {
				y->right = z;
			}
			else {
				y->left = z;
			}
		}
		
		insert_up(z);
		root->color = black;
		return z->value;
	}
	int  In_Order_Traversal(node * p,int dep=0) {
		if (p == nil)return dep-1;
		int mx=In_Order_Traversal(p->left,dep+1);
		v.push_back(p -> value);
		//cout << p->value << " ";
		mx=max(mx,In_Order_Traversal(p->right, dep + 1));
		return mx;
	}
	node * findnode(int x) {
		auto p = root;
		while (p != nil) {
			if (p->value == x) {
				return p;
			}
			if (p->value > x) {
				p=p->left;
			}
			else {
				p = p->right;
			}
		}
		return nullptr;

	}
	
	node* findmin(node* p) {
		p = p->right;
		while (p->left != nil)p = p->left;
		return p;
	}
	void transplant(node* u, node* v) {
		if (root == u) {
			root = v;
		}
		else if (u->par->left == u) {
			u->par->left = v;
		}
		else u->par->right = v;
		v->par = u->par;
	}
	void delete_up(node *self) {
		if (self == root||self->color==red) {
			self->color = black;
			return;
		}
		if (self == self->par->left) {
			node * bro = self->par->right;
			if (bro->color == red) {
				bro->color = black;
				self->par->color = red;
				rotate_left(self->par);
				return delete_up(self);
			}
			if (bro->left->color == black && bro->right->color == black) {
				bro->color = red;
				return delete_up(self->par);
			}
			if(bro->left->color == red && bro->right->color == black) {
				bro->left->color = black;
				bro->color = red;
				rotate_right(bro);
				return delete_up(self);
			}
			if (bro->right->color == red) {
				swap(self->par->color, bro->color);
				bro->right->color = black;
				rotate_left(self->par);
				return delete_up(root);
			}
		}else if (self == self->par->right) {
			node * bro = self->par->left;
			if (bro->color == red) {
				bro->color = black;
				self->par->color = red;
				rotate_right(self->par);
				return delete_up(self);
			}
			if (bro->left->color == black && bro->right->color == black) {
				bro->color = red;
				return delete_up(self->par);
			}
			if (bro->right->color == red && bro->left ->color == black) {
				bro->color = red;
				bro->right->color = black;
				rotate_left(bro);
				return delete_up(self);
			}
			if (bro->left->color == red) {
				swap(self->par->color, bro->color);
				bro->left->color = black;
				rotate_right(self->par);
				return delete_up(root);
			}
		}
	}
	void del(int num) {
		if (root == nil)return;
		node *delnode = root;
		stack<node*>st;
		while (delnode != nil && delnode->value != num) {
			st.push(delnode);
			if (num < delnode->value) {
				delnode = delnode->left;
			}
			else delnode = delnode->right;
		}
		
		if (delnode == nil)return;
		if (delnode->vcnt > 1) {
			delnode->vcnt--;
			delnode->cnt--;
			while (!st.empty()) {
				st.top()->cnt--;
				st.pop();
			}
			return;
		}
		else {
			delnode->cnt--;
			while (!st.empty()) {
				st.top()->cnt--;
				st.pop();
			}
		}
		//cout << delnode->value << endl;
		if (delnode->left != nil && delnode->right != nil) {
			node* nd = findmin(delnode);
			delnode->value = nd->value;
			delnode->vcnt = nd->vcnt;
			node* x = nd;
			while (x!= delnode) {
				x->cnt -= delnode->vcnt;
				x = x->par;
			}
			delnode = nd;
		}
	//	cout << delnode->value << endl;
		node *x= nullptr;
		if (delnode->left == nil) {
			x = delnode->right;
		}
		else if(delnode->right==nil){
			x = delnode->left;
		}
		transplant(delnode, x);

		if (delnode->color == black) {
			 delete_up(x);
		}
		delete delnode;
	}
	int find(int x) {
		node *p = root;
		int kth = 0;
		while (p != nil) {
			if (p->value == x)return kth + p->left->cnt + 1;
			if (p->value > x) {
				p = p->left;
			}
			else {
				kth += p->left->cnt + p->vcnt;
				p = p->right;
			}
		}

	}
	int kth(int x) {
		node *p = root;
		while (p != nil) {
			if (p->left->cnt >= x) {
				p = p->left;
			}
			else if (p->left->cnt + p->vcnt >= x) {
				return p->value;
			}
			else {
				x -= p->left->cnt + p->vcnt;
				p = p->right;
			}
		}
		return -1;
	}
	int pre(int x) {
		node *p = root;
		node *pnode=nullptr;
		while (p!=nil) {
			if (p->value >= x) {
				p = p->left;
			}
			else {
				pnode = p;
				p = p->right;
			}
		}
		
		if (pnode!= nil)return pnode->value;
		return -1;
	}
	int nxt(int x) {
		node *p = root;
		node *pnode = nullptr;
		while (p != nil) {
			if (p->value <= x) {
				p = p->right;
			}
			else {
				pnode = p;
				p = p->left;
			}
		}
		
		if (pnode != nil)return pnode->value;
		return -1;
	}
};

int main()
{
	//auto nw = chrono::system_clock::now();
	less<int>;
	RBT x;
	int n,num;
	cin >> n;
	while (n--) {
		int p;
		cin >> p;
		cin >> num;
		if (p == 1) {
			x.insert(num);
		}
		else if (p == 2) {
			x.del(num);
		}
		else if (p == 3) {
			cout << x.find(num) << endl;
		}
		else if (p == 4) {
			cout << x.kth(num) << endl;
		}
		else if (p == 5) {
			cout << x.pre(num)<<endl;
		}
		else cout << x.nxt(num) << endl;
	}
	/*
	for (int i = 0; i < 10000000; i++) {
		q++;
	//	cout << num << ",";
		//cout << num << endl;
		x.insert(i);
	}
	for (int i = 3000000; i >= 2000000; i--) {
		x.del(i);
	}
	cout<<"dep"<<x.In_Order_Traversal(x.root);
	int pr = -1;
	for (auto w : v) {
		if (w <= pr) {
			cout << "??" << endl;
		}
		pr = w;
	}*/
//	if (x.root->color == RBT::clr::red)cout << "ys" << endl;
//	cout<<x.find(4);
	//shared_ptr<RBT::node>sp = x.uncle(x.find(2));
	//cout << endl;
 //    auto ed = chrono::system_clock::now();
//	std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(ed - nw);
//	cout << time_span.count() << endl;
	return 0;
}

