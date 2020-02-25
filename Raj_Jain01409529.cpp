/*
	First name	Raj
	Last name		Jain
	Student ID	01409529
	Username		RKJ17
*/

//IMPORTANT: before submitting your source code, please make sure to comment your main function (needed for automated testing)
#include <iostream>
#include <string>
#include <map>
#include <vector> //or #include <list>
#include <queue>
// do not use using namespace std

// do not alter the struct declaration
struct hufftreenode {
	char character;
	int frequency;
	hufftreenode* left;
	hufftreenode* right;
};

// do not alter the typedef
typedef hufftreenode* hufftreeptr;

// you can define and use any additional *structures* if you might need to

// YOUR CODE HERE FOR ADDIIONAL STRUCTS DECLARATION, IF ANY

struct CompareHuffNode {
  bool operator()(hufftreeptr p1, hufftreeptr p2){
    return p1->frequency > p2->frequency;
    }
};

/////////////////////////////////////////
// Functions declarations (prototypes) //
/////////////////////////////////////////

// do not alter these functions declarations

//utility function
bool valid_hufftree(const hufftreeptr hufftree);
 // returns true if the input hufftree is a valid Huffman tree
 // i.e. all the terminal nodes (leaves) have characters,
 // all the non-leaf nodes have two sub-trees each,
 // and the occurrence frequency of a non-leaf node equals
 // the sum of the occurrence frequencies of its two sub-trees.

std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable);
// encodes an input message (a string of characters) into an encoded message (string of bits) using the Huffman coding
// builds the Huffman tree and its associated encoding table (i.e. the association between the characters in the tree leaves and their encodings (string of bits))

std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree);
// decodes an encoded message (a string of bits) into the original message (a string of characters) using the Huffman tree built during the encoding

// you can define and use any additional *functions* if you might need to

void preorderencode(hufftreeptr ptr, std::string code, std::map< std::string,char>& hufftable);
void preorder(hufftreeptr ptr, std::string code, std::map<char, std::string>& hufftable);
bool leafNode(hufftreeptr hufftree);
bool fulltree(hufftreeptr ptr);
int sum(hufftreeptr ptr);

///////////////////
// main function //
///////////////////

//IMPORTANT (Reminder): before submitting your source code, please make sure to comment your main function (needed for automated testing)
/*int main() {

	// YOUR CODE HERE FOR TESTING YOUR PROGRAM

	// an example of basic encoding/decoding
	std::string message, encodedmessage, decodedmessage;
	hufftreeptr hufftree;
	std::map<char, int> freqtable;
	std::map<char, std::string> hufftable;

	message = "go go gophers";

	// 1) encoding (compression)
	encodedmessage = huffencode(message, freqtable, hufftree, hufftable);
	// freqtable should be as in Table 1.
	// hufftree might be as in Figure 2 (remember, the Huffman coding tree IS NOT UNIQUE. If we swap two siblings in a Huffman tree, it is still a Huffman tree of the same message.)
	// hufftable should correspond to hufftree, and might be as in Table 2 <' ',"100">, <'g',"00">, <'o',"01">, <'p',"1110">, <'h',"1101">, <'e',"101">, <'r',"1111">, <'s',"1100">.
	// encodedmessage might then be the 37-bits string "0001100000110000011110110110111111100" (i.e. "00.01.100.00.01.100.00.01.1110.1101.101.1111.1100")

	if(valid_hufftree(hufftree))  {
		std::cout << "valid Huffman tree." << std::endl;
	}
	else {
		std::cout << "not valid Huffman tree." << std::endl;
	}

	// 2) decoding (uncompression)
	decodedmessage = huffdecode(encodedmessage, hufftree);
	// should be "go go gophers"

	if(decodedmessage == message) {
		std::cout << "decoding OK." << std::endl;
	}
	else {
		std::cout << "decoding not OK." << std::endl;
	}

	return 0;
}*/



/////////////////////////////////////////////
// Functions definitions (implementations) //
/////////////////////////////////////////////

// YOUR CODE HERE FOR THE FUNCTIONS IMPLEMENTATIONS

bool valid_hufftree(const hufftreeptr hufftree){

	if(!leafNode(hufftree))
	return 0;

	if(!fulltree(hufftree))
	return 0;

	if(sum(hufftree) == 0)
	return 0;

  return 1;
}

bool leafNode(hufftreeptr hufftree){

  if(hufftree == NULL)
  return false;

  if(hufftree->left == NULL && hufftree->right == NULL){

    if(hufftree->character > 0)

		return true;

		else
    return false;
	}

    else{
      bool left = leafNode(hufftree->left);
      bool  right = leafNode(hufftree->right);
      return left&&right;
    }

	}

bool fulltree(hufftreeptr ptr){

  if (ptr == NULL)
  return 0;

	if(ptr->left == NULL && ptr->right == NULL)
  return 1;

	else{
    bool left = fulltree(ptr->left);
    bool  right = fulltree(ptr->right);
    return left&&right;
	}

}

int sum(hufftreeptr ptr){

  if(ptr == NULL)
  return 0;
  if(ptr->left == NULL && ptr->right == NULL)
  return ptr->frequency;

  else{

    int sumleft = sum(ptr->left);
    int sumright = sum(ptr->right);

		int total = sumleft + sumright;

    if(total!=0 && total == ptr->frequency)
    return total;

		else
    return 0;
	}
}

std::string huffencode(const std::string& message, std::map<char, int>& freqtable, hufftreeptr& hufftree, std::map<char, std::string>& hufftable){

  int counter[128]={0};

	for(int i = 0; i < message.length(); i++){
		counter[message[i]]++;
	}

	std::priority_queue<hufftreeptr, std::vector<hufftreeptr>, CompareHuffNode> Q;

	for(int i = 0; i < 128; i++){

    if(counter[i] != 0){

			freqtable.insert(std::pair<char, int>((char)i, counter[i]));
	    hufftreeptr ptr= new hufftreenode;

			ptr->character=(char)i;
	    ptr->frequency=counter[i];

			ptr->left=NULL;
	    ptr->right=NULL;

	    Q.push(ptr);
		}
	}

 while (Q.size()>1){

	 hufftreeptr T1 = Q.top();
   Q.pop();
   hufftreeptr T2 = Q.top();
   Q.pop();

   hufftreeptr r = new hufftreenode;
   r->frequency = T1->frequency + T2->frequency;
   r->left = T1;
   r->right = T2;
   Q.push(r);
 }

 hufftree=Q.top();
 preorder(hufftree,"",hufftable);

 std::string res;
 for(int i = 0; i < message.length(); i++){
   res.append(hufftable[message[i]]);
 }
	return res;

}

void preorder(hufftreeptr ptr, std::string code, std::map<char, std::string>& hufftable){

  if(ptr == NULL)
  return;

  if(ptr->left == NULL && ptr->right == NULL){

		if(code.length() == 0)
		code = "0";

		hufftable.insert(std::pair<char, std::string>(ptr->character, code));
	}

	else{
		std::string leftcode, rightcode;

		leftcode.assign(code);
		leftcode.append("0");
		rightcode.assign(code);
		rightcode.append("1");

		preorder(ptr->left, leftcode, hufftable);
		preorder(ptr->right, rightcode, hufftable);

	}
}

std::string huffdecode(const std::string& encodedmessage, const hufftreeptr& hufftree){

 	std::map< std::string,char> hufftable;
  preorderencode(hufftree, "", hufftable);

  std::string msg="";
  std::string bitstring="";

	for(int i = 0; i < encodedmessage.length(); i++){
		bitstring += encodedmessage[i];
		if(hufftable[bitstring]){
			msg += (hufftable[bitstring]);
			bitstring="";
		}
	}
	return msg;
}

void preorderencode(hufftreeptr ptr, std:: string code,std::map< std::string,char>& hufftable){

	if(ptr == NULL)
	return;

	if( ptr->left == NULL && ptr->right == NULL){
		if(code.length() == 0)
		code = "0";
		hufftable.insert(std::pair< std::string, char>(code, ptr->character));
	}

	else{
		std::string leftcode, rightcode;
		leftcode.assign(code);
		leftcode.append("0");

		rightcode.assign(code);
		rightcode.append("1");

		preorderencode(ptr->left, leftcode, hufftable);
		preorderencode(ptr->right, rightcode, hufftable);
	}

}
