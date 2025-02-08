#include <iostream>
#include <stdlib.h>
#include <iomanip> 
#include <vector>
#include <string>
#define size 50
#define V 11
#define V1 6
using namespace std;
struct user
{
    string name;
    string product;
    int quantity;
    double bill;

    user() {}

    user(string name, string product, int quantity, double bill)
    {
        this->name = name;
        this->product = product;
        this->quantity = quantity;
        this->bill = bill;
    }
};

struct deliveryUser
{
    user nUser;
    string address;
    double deliveryCharges;
    int distanceDelivery;
    deliveryUser *next = NULL;

    deliveryUser(string name, string product, int quantity, double bill, string address, double deliveryCharges, int distanceDelivery)
    {
        this->nUser = user(name, product, quantity, bill);
        this->address = address;
        this->deliveryCharges = deliveryCharges;
        this->distanceDelivery = distanceDelivery;
    }
};

struct Shop
{
    string name;
    string *product;
    int *price;
    string address;
    deliveryUser *nextdeliveryUser;
};

Shop* shop=NULL;
string addressF;
int dist[V];
int distanceP[V1];
int optionDelivery = -999;
int optionDelivery1 = -999;
deliveryUser *currentdeliveryUser = NULL;

struct takeAway
{
    string name;
    string product;
    int quantity;
    double bill;
    int orderId;
    takeAway *left;
    takeAway *right;

    takeAway(string name, string product, int quantity, double bill, int orderId)
    {
        this->name=name;
		this->product = product;
        this->quantity = quantity;
        this->bill = bill;
        this->orderId=orderId;
        this->left = NULL;
        this->right = NULL;
    }
};

takeAway *root = NULL; 
void display(takeAway *root)
{
	cout <<"\n------- Take-Away Order  ---------"<< endl;
	cout <<"\n----------------------------------"<< endl;
    cout << "Take away Customer name :" << root->name << endl;
    cout << "Product :" << root->product << endl;
    cout << "Quantity : " << root->quantity << endl;
    cout << "Bill : " << root->bill << endl;
    cout << "Order ID: " << root->orderId << endl;
    cout <<"-----------------------------------\n"<< endl;
}

takeAway *displayTakeAway(takeAway *root)
{
  
    if  (root)
	{
        displayTakeAway(root->left);
        display(root);
        displayTakeAway(root->right);
    }
     return root;
}
int isEmpty(takeAway *root)
{
    return root == NULL;
}

// Height 
int height(takeAway* root) {
	if (root == NULL)
      return -1;
    else {
      int leftH = height(root -> left);
      int rightH = height(root -> right);

    return (1+max(leftH , rightH));
	}
}
int balanceFactor(takeAway *root)
{
    if (!root)
        return 0;

    return height(root->left) - height(root->right);
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

takeAway *search(takeAway *root, int id)
{
    if (root == NULL)
    {
        return NULL;
    }
    else if (root->orderId == id)
    {
        return root;
    }
    else if (root->orderId < id)
    {
        return search(root->right, id);
    }
    else if (root->orderId > id)
    {
        return search(root->left, id);
    }

    return root;
}
takeAway *LLRotation(takeAway *root)
{
    takeAway *x = root->left;
    takeAway *temp = x->right;

    x->right = root;
    root->left = temp;

    root = x;

    return x;
}
takeAway *RRRotation(takeAway *root)
{

    takeAway *x = root->right;
    takeAway *temp = x->left;

    x->left = root;
    root->right = temp;

    root = x;

    return x;
}
takeAway *LRRotation(takeAway *root)
{
    root->left = RRRotation(root->left);
    return LLRotation(root);
}
takeAway *RLRotation(takeAway *root)
{
    root->right = LLRotation(root->right);
    return RRRotation(root);
}
//  INSERTION
takeAway *insertion(string name, int quantity, string product, double bill, int orderId, takeAway *root)
{
    takeAway *newNode = new takeAway(name,  product, quantity, bill, orderId);

    if (root == NULL)
    {
        root = newNode;
    }

    else if (root->orderId > newNode->orderId)
    {
        root->left = insertion(name, quantity, product, bill, orderId, root->left);
    }
    else if (root->orderId < newNode->orderId)
    {
        root->right = insertion(name, quantity, product, bill, orderId, root->right);
    }

    else
    {
        cout << "No duplicates Values are Allowed " << endl;
        return root;
    }

    int bf = balanceFactor(root);

    if (bf == 2)
    {
        if (root->left->orderId > newNode->orderId)
        {
            return LLRotation(root);
        }

        if (root->left->orderId < newNode->orderId)
        {
            return LRRotation(root);
        }
    }
    else if (bf == -2)
    {
        if (root->right->orderId < newNode->orderId)
        {
            return RRRotation(root);
        }
        if (root->right->orderId > newNode->orderId)
        {
            return RLRotation(root);
        }
    }
    return root; 
}

takeAway * minValue(takeAway *root) {
    takeAway *current = root;
    while (current-> left != NULL) {
      current = current -> left;
    }
    return current;
  }

takeAway *deleteNode(takeAway *root, int orderId)
{
    if (root == NULL)
        return root;
    else if (orderId < root->orderId)
        root->left = deleteNode(root->left, orderId);
    else if (orderId > root->orderId)
        root->right = deleteNode(root->right, orderId);
    else {
      if (root-> left == NULL) {
        takeAway* temp = root-> right;
        delete root;
        return temp;
      } 
	  else if (root-> right == NULL) {
        takeAway* temp = root-> left;
        delete root;
        return temp;
      } 
      
	  else {
        takeAway * temp = minValue(root-> right);
        root-> orderId = temp -> orderId;
        root-> right = deleteNode(root-> right, temp-> orderId );
      }
    }

    int balance = balanceFactor(root);

    if (balance > 1 && orderId < root->left->orderId)
        return LLRotation(root);

    if (balance > 1 && orderId > root->left->orderId)
    {
        root->left = LLRotation(root->left);
        return LRRotation(root);
    }

    if (balance < -1 && orderId > root->right->orderId)
        return RRRotation(root);

    if (balance < -1 && orderId < root->right->orderId)
    {
        return RLRotation(root);
    }
    return root;
}
void placeOrderHomeDeliveryCustomer(string name, string product, int quantity, double bill, string address, int deliveryCharges, int distanceDelivery)
{

    currentdeliveryUser=new deliveryUser(name, product, quantity, bill, address, deliveryCharges, distanceDelivery);
    
    if (shop->nextdeliveryUser == NULL)
    {
        shop->nextdeliveryUser= currentdeliveryUser;
    }
    else
    {
        deliveryUser *temp = shop->nextdeliveryUser;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = currentdeliveryUser;
        currentdeliveryUser->next = NULL;
    }
    
   	cout << "\n-------------------- The Details of Order --------------------";
    cout << "\n Customer Name : " << name << "\n Order name : " << product << "\n Quantity : " << quantity<<"\n Address : " << address ;
    cout << "\n Distance in km : " << distanceDelivery <<"\n Delivery charges : " << deliveryCharges <<"\n Total bill : " << bill<<endl;
}

void displayAllOrdersHomeDeliveryCustomers()
{
    if (shop->nextdeliveryUser == NULL)
    {
        cout << "/nNo Home Delivery Order" << endl;
    }
    else
    {
        deliveryUser *traversal = shop->nextdeliveryUser;
        while (traversal != NULL)
        {
        	cout << "--------------  Home Delivery Order  ----------------" << endl;
            cout << "-----------------------------------------------------" << endl;
            cout << "Home Delivery Customer : " << traversal->nUser.name << endl;
            cout << "Product Name : " << traversal->nUser.product << endl;
            cout << "Quantity : " << traversal->nUser.quantity << endl;
            cout << "Delivery Distance : " << traversal->deliveryCharges << "KM"<<endl;
            cout << "Delivery Charges : " << traversal->distanceDelivery << endl;
            cout << "Bill : " << traversal->nUser.bill << " RS/_" << endl;
            cout << "Delivery Address : " << traversal->address << endl;
            cout << "-----------------------------------------------------" << endl;

            traversal = traversal->next;
        }
    }   
}
int minDistance(int dist[], bool visited[])
{
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (visited[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}
void dijkstra(int graph[V][V], int dist[], int src)
{
    bool visited[V]; 

    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX, visited[i] = false;

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, visited);

        visited[u] = true;
        
        for (int v = 0; v < V; v++)

            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
                	dist[v] = dist[u] + graph[u][v];
    }
}
int minKey(int distanceP[], bool visitedP[])
{
	int min = INT_MAX, min_index;

	for (int v = 0; v < V1; v++)
		if (visitedP[v] == false && distanceP[v] < min)
			min = distanceP[v], min_index = v;

	return min_index;
}

void printMST(int parent[], int graph[V1][V1])
{
	cout << "Edge \tWeight\n";
	for (int i = 1; i < V1; i++)
		cout << parent[i] << " - " << i << " \t"
			<< graph[i][parent[i]] << " \n";
}

void prims(int graph[V1][V1], int distanceP[])
{
	int parent[V1];
	bool visitedP[V1];
	for (int i = 0; i < V1; i++)
	distanceP[i] = INT_MAX, visitedP[i] = false;
	distanceP[0] = 0;
	parent[0] = -1; 

	for (int count = 0; count < V1 - 1; count++) {
		int u = minKey(distanceP, visitedP);
		visitedP[u] = true;
		for (int v = 0; v < V1; v++)
			if (graph[u][v] && visitedP[v] == false && graph[u][v] < distanceP[v])
				parent[v] = u, distanceP[v] = graph[u][v];
	}
}
string CityName[] = {"WareHouse", "Lalitpur","Kathmandu", "Chitwan", "Bhaktapur", "-", "-", "-", "-", "-", "-"};
string Lalitpur[]={"Lalitpur", "Badikhel", "Chandanpur", "Dalchoki", "Gotikhel", "Lamatar"};
string Kathmandu[]={"Kathmandu", "Lazimpat" ,"Dillibazar", "Bagbajar", "Baneshwor", "Jamal"};
string Chitwan[]={"Chitwan", "Bairiya", "Dahakhani", "Jutpani", "Kumroj", "Mugling"};
string Bhaktapur[]={"Bhaktapur", "Bageshwari", "Duwakot", "Jhaukhel", "Sipadol", "Tathali"};
int graph[V][V]={   {0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3},   
					{4, 0, 0, 0, 0, 0, 0, 6, 0, 0, 2},	 
					{0, 0, 0, 0, 9, 4, 0, 0, 0, 0, 11},	 
					{0, 0, 0, 0, 1, 0, 0, 0, 16, 5, 0},	 
					{0, 0, 9, 1, 0, 8, 13, 0, 10, 6, 0},	
					{0, 0, 4, 0, 8, 0, 0, 3, 17, 0, 7},	 
					{0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 7},	 
					{0, 6, 0, 0, 0, 3, 0, 0, 5, 0, 0},	 
					{0, 0, 0, 16, 10, 17, 0, 5, 0, 0, 0},
					{0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0},	 
					{3, 2, 11, 0, 0, 7, 7, 0, 0, 0, 0}  };			
int KtmGraph[V1][V1]={ 	{ 0, 4, 4, 0, 0, 0 },
						{ 4, 0, 2, 0, 0, 0 },  
						{ 4, 2, 0, 3, 2, 4 },	
						{ 0, 0, 3, 0, 0, 3 },
						{ 0, 0, 2, 0, 0, 3 },	
						{ 0, 0, 4, 3, 3, 0 } };	
int ChitGraph[V1][V1]={ 	{ 0, 4, 0, 2, 0, 0 },
						{ 4, 0, 3, 1, 0, 6 },	
						{ 0, 3, 0, 4, 0, 7 },	
						{ 2, 1, 4, 0, 9, 0 },	
						{ 0, 0, 0, 9, 0, 5 },
						{ 0, 6, 7, 0, 5, 0 } };	
int BhaGraph[V1][V1]={ 	{ 0, 9, 15, 6, 0, 0 },
						{ 9, 0, 7, 0, 0, 8 },
						{ 15, 7, 0, 8, 9, 5 },
						{ 6, 0, 8, 0, 11, 0 },
						{ 0, 0, 9, 11, 0, 0 },
						{ 0, 8, 5, 0, 0, 0 } };							
int LatGraph[V1][V1]={ { 0, 6, 3, 0, 0, 1 },
                        { 6, 0, 0, 2, 0, 5 }, 
						{ 3, 0, 0, 0, 3, 5 }, 
						{ 0, 2, 0, 0, 6, 4 },
						{ 0, 0, 3, 6, 0, 6 },
						{ 1, 5, 5, 4, 6, 0 } }; 

int city(){
	int opt;
	cout << "\nThe delivery is available for following Areas : \n" << endl;
	cout<<"\n---------------------------------";
	cout<<"\n |CITY CODE |        City       |";
	cout<<"\n---------------------------------";
    cout<<"\n |     1    |    Lalitpur       |"<<endl;
    cout<<"\n |     2    |    Kathmandu      |"<<endl;
	cout<<"\n |     3    |    Chituwan       |"<<endl;
    cout<<"\n |     4    |    Bhaktapur      |"<<endl;
    cout<<"----------------------------------\n"<<endl;
	cout << "Enter your option :" << endl;
    cin >> opt;
    return opt;
}

int Chit(){
	int opt;
	cout << "\nThe delivery is available for following Areas in Chitwan: " << endl;
    cout<<"\n---------------------------------";
	cout<<"\n |CITY CODE |        AREA       |";
	cout<<"\n---------------------------------";
    cout<<"\n |     1    |    Bhairiya       |"<<endl;
    cout<<"\n |     2    |    Dahakhani      |"<<endl;
    cout<<"\n |     3    |    Jutpani        |"<<endl;
	cout<<"\n |     4    |    Kumroj         |"<<endl;
    cout<<"\n |     5    |    Mugling        |"<<endl;
    cout<<"----------------------------------\n"<<endl;
    cout << "Enter your option :" << endl;
    cin >> opt;
    return opt;                              
}
int Lat(){
	int opt;
	cout << "\nThe delivery is available for following Areas in Lalitpur: " << endl;
	cout<<"\n---------------------------------";
	cout<<"\n |CITY CODE |        AREA       |";
	cout<<"\n---------------------------------";
    cout<<"\n |     1    |    Badikhel       |"<<endl;
    cout<<"\n |     2    |    Chandanpur     |"<<endl;
    cout<<"\n |     3    |    Dalchoki       |"<<endl;
	cout<<"\n |     4    |    Gotikhel       |"<<endl;
    cout<<"\n |     5    |    Lamatar        |"<<endl;
    cout<<"---------------------------------\n"<<endl;
    cout << "Enter your option :\n" << endl;
    cin >> opt;
    return opt;                        
}
int Bha(){
	int opt;
	cout << "\nThe delivery is available for following Areas in Bhaktapur: " << endl;
	cout<<"\n---------------------------------";
	cout<<"\n |CITY CODE |        AREA       |";
	cout<<"\n---------------------------------";
    cout<<"\n |     1    |    Bageshwari     |"<<endl;
    cout<<"\n |     2    |    Duwakot        |"<<endl;
    cout<<"\n |     3    |    Jaukhel        |"<<endl;
	cout<<"\n |     4    |    Sipadol        |"<<endl;
    cout<<"\n |     5    |    Tathali        |"<<endl;
    cout<<"---------------------------------\n"<<endl;
    cout << "Enter your option :" << endl;
    cin >> opt;
    return opt;                             
}
int Ktm(){
	int opt;
	cout << "\nThe delivery is available for following Areas in Kathmandu: " << endl;
	cout<<"\n---------------------------------";
	cout<<"\n |CITY CODE |        AREA       |";
	cout<<"\n---------------------------------";
    cout<<"\n |     1    |    Lazimpat       |"<<endl;
    cout<<"\n |     2    |    Dillibazar     |"<<endl;
    cout<<"\n |     3    |    Bagbajar       |"<<endl;
	cout<<"\n |     4    |    Baneshwor      |"<<endl;
    cout<<"\n |     5    |    Jamal          |"<<endl;
    cout<<"---------------------------------\n"<<endl;
    
    cout << "Enter your option :" << endl;
    cin >> opt;
    return opt;                         
}
int FurthurArea(int option){
	int opt;
	switch(option){
			case 1: 
			{
				prims(LatGraph,distanceP);
				opt=Lat();
				addressF=Lalitpur[opt];
			
			}
			break;
			case 2:
			{
				prims(KtmGraph,distanceP);
				opt=Ktm();
				addressF=Kathmandu[opt];	
			}
			break;
			
			case 3:
			{
				prims(ChitGraph,distanceP);
				opt=Chit();
				addressF=Chitwan[opt];
			}
			break;
			
			case 4:
			{
				prims(BhaGraph,distanceP);
				opt=Bha();
				addressF=Bhaktapur[opt];
			}
			break;
			
		}
	return opt;
}
int main()
{
	shop =new Shop;
	shop->name=" E-Stationary ";
	shop->address="Kathmandu, Dillibazar";
	shop->product=new string[11]{"","Pack of Pencils -2B ", "Pack of Pen -Black", "Pack of Pen -Blue",
								 "Eraser Set ", "Highlighter Set","Stapler Pin Set  ",
								  "Scale- 5 pcs  ", "Scissors  ", "Fevicol",
								"A4 Paper -5pcs "};														
	shop->price=new int[11]{0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
	int choice = -99;
	do
    {
        cout << endl;
        cout << "------------------------" << shop->name << "------------------------" << endl;
        cout << "We are located at : " << shop->address << endl;
        cout << endl;
        cout << endl;
        cout << "------------------------  FEATURES  ------------------------" << endl;
        cout << endl;
        cout << "1. Display the product and prices" << endl;
		cout << "2. Place order for Take-Away from Warehouse" << endl;
        cout << "3. Place order for Home Delivery " << endl;
        cout << "4. Display all Delivery Orders and takeaway orders" << endl;
        cout << "5. EXIT " << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
        string name;
        int quantity;
        int productNumber;
        double bill;
        string address;
        int orderId;
        int distance;
		
        switch (choice)
        {
        case 1:
		{
		cout << "\n---------------------------------------------------------------------\n";
        cout << "|  SN  |       ITEM NAME                |          ORIGINAL PRICE    |\n";
        cout << "---------------------------------------------------------------------\n";
        string items[11] = {"", "Pack of Pencils -2B", "Pack of Pen -Black", "Pack of Pen -Blue","Eraser Set", "Highlighter Set", "Stapler Pin Set", "Scale- 5 pcs","Scissors", "Fevicol", "A4 Paper -5pcs"};
        int prices[11] = {0, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
        for (int i = 1; i <= 10; i++) {
        cout << "| " << setw(5) << left << i << "| "  // SN
             << setw(31) << left << items[i] << "| "  // Item Name 
             << setw(27) << left << prices[i] << "|" << endl; // Price 
        }
        cout << "---------------------------------------------------------------------\n";
		}
		break ;		
        case 2:                                   
        { 
			cout << "Enter the name of the customer: ";
            cin >> name;
			cout << "Enter the product Number: ";
            cin >> productNumber;
            cout << "Enter the quantity of the product: ";
            cin >> quantity;
            cout << "Enter orderID of one or two letters: ";
            cin >> orderId;

            bill = quantity * shop->price[productNumber];
            root=insertion(name, quantity, shop->product[productNumber], bill, orderId, root);
            cout <<"\n------------------------------------"<< endl;
            cout << "**** Your order has been placed *****" << endl;
            cout << "Total amount : " << bill << endl;
			cout << "order ID :" << orderId << endl;
            cout << "You can get your order from Warehouse " << endl;
            cout <<"-------------------------------------\n"<< endl; 
        }
        break;
        case 3:
        {                                               
            dijkstra(graph,dist,0);
            do
            {
                optionDelivery=city();   //Return The index of city that has been selected

            } while (!(optionDelivery >= 0 && optionDelivery <= 10));
            
            if(optionDelivery==1 || optionDelivery==2 || optionDelivery==3 || optionDelivery==4 )
			{
            	do
            	{
            		optionDelivery1=FurthurArea(optionDelivery);  //Return index of area in a city
            		
				} while(!(optionDelivery1 >=0 && optionDelivery1 <=5));
				
				address= CityName[optionDelivery] +" " +addressF;
				distance=dist[optionDelivery] + distanceP[optionDelivery1];	
			}
			else 
			{
				address = CityName[optionDelivery];
				distance=dist[optionDelivery];
				
			}
            cout << "Enter Customer's Name': ";
            cin >> name;
            cout << "Enter Product's Number: ";
            cin >> productNumber;
            cout << "Enter the quantity you want to purchase: ";
            cin >> quantity;

            int deliveryChargesPerKM = 10;
            int deliveryCharges = deliveryChargesPerKM * distance;
            bill = quantity * shop->price[productNumber] + deliveryCharges;

            placeOrderHomeDeliveryCustomer(name, shop->product[productNumber], quantity, bill, address, deliveryCharges, distance );
        }
        break;
		case 4 :
		{
            displayAllOrdersHomeDeliveryCustomers();
            displayTakeAway(root);
		}
        break;
    	}
    } while (choice <=4);
    cout << "Thank you for visiting our store " << endl;
    cout<<" :)";
	return 0;
}


