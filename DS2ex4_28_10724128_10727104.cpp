// 10724128 吳宇哲 10727104 王子威
// 【工具】–【編譯選項】–【編譯器】勾選【編譯時加入以下命令】
// 加入【-std=c++11】 ，即可運作。

# include<fstream>
# include<string.h>
# include<iostream>
# include<vector>
# include<cstdlib>
# include<iomanip>
# include<stack>
#include<algorithm>
#include<math.h>
#include<queue>

using namespace std;


struct Data {
    char putID[10] ; // 發訊
    char getID[10] ; // 收訊
    float weight ;
};// 讀取資料用的結構

struct ListData{

    char ID[10] ;
    float weight = 0 ;

} ;
struct List{

    char ID[10] ;
    vector<ListData> idList ;

} ;

struct BFSData{
    char ID[10] ;
    vector<char*> BFSNode ;

} ;

bool sortID( const List &a, const List &b ) {
        if (strcmp(a.ID , b.ID) < 0 )
            return true;
        else
            return false;
    }

bool sortIDBFS( char* &a, char* &b ) {
        if (strcmp(a , b) < 0 )
            return true;
        else
            return false;
    }

bool sortWeight( const ListData &a, const ListData &b ) {
        int aID = stoi(a.ID) ;
        int bID = stoi(b.ID) ;
        if (a.weight > b.weight)
            return true;
        else if ( a.weight == b.weight && aID < bID)
            return true ;
        else
            return false;
}

class AdjacencyList {
	private:
    	vector< Data> list ; // 讀黨存取
    	vector< List> adjList ; // Adjacency List
    	string inputString ;
    	int nodeCount ;
    public:
        void reset() {
            list.clear() ;
            adjList.clear() ;
        }

		void readBin ( ) {
		    fstream theFile;
		    cout << endl << "Input a file number (e.g., 401, 402, ...): " ;
		    cin >> inputString ;
		    string fileName = "pairs" + inputString + ".bin" ;
  			int  dataNum ;
  			Data temp ;
            theFile.open( fileName.c_str(), ios::in | ios::binary ) ;
  			if ( theFile.is_open() ) { // 檢查檔案是否打開
				theFile.seekg( 0, theFile.end ) ;
                dataNum = theFile.tellg() / sizeof( temp ); // 讀取檔案大小
                nodeCount = dataNum ;
                theFile.seekg( 0, theFile.beg ) ;
                //cout << dataNum << endl ;
                for ( int i = 0 ; i < dataNum ; i ++ ) {
                    theFile.read( (char*)&temp, sizeof(temp) ) ;
                    list.push_back( temp ) ;
                    //cout << temp.putID << "     "<< temp.getID << "    " <<temp.weight << endl ;
                } // end for

    			theFile.close();
            }
            else {
                cout << "### The file does not exist! ###" <<endl ;
            }
		}


        void adjacency_list ( ) {
            ListData temp ;
            bool check1 = true ; // if have putID == true
            bool check2 = true ; // if have getID == true
            for ( int i = 0 ; i < list.size() ; i++ ) {
                check1 = true ;
                check2 = true ;
                for ( int j = 0 ; j < adjList.size() ; j++ ) {
                    if ( strcmp( adjList.at(j).ID , list.at(i).putID) == 0 ) {
                        strcpy( temp.ID , list.at(i).getID ) ;
                        temp.weight = list.at(i).weight ;
                        adjList.at(j).idList.push_back( temp) ;
                        check1 = false ;
                    } // if have same id push back

                    if ( strcmp( adjList.at(j).ID , list.at(i).getID) == 0 )
                        check2 = false ;
                }  // Check for the same ID


                if ( check1 ) {
                    List insertList ; // Add a new node for List
                    strcpy( insertList.ID , list.at(i).putID ) ;
                    adjList.push_back( insertList ) ;

                    strcpy( temp.ID , list.at(i).getID ) ;
                    temp.weight = list.at(i).weight ;
                    adjList.at( adjList.size()-1 ).idList.push_back( temp) ;

                } // if not have putID

                if ( check2 ) {
                    List insertList ; // Add a new node for List
                    strcpy( insertList.ID , list.at(i).getID ) ;
                    adjList.push_back( insertList ) ;
                } // if not have getID

            } // input all data (for)

            sort(adjList.begin(), adjList.end(), sortID);
            for ( int j = 0 ; j < adjList.size() ; j++ )
                sort(adjList.at(j).idList.begin(), adjList.at(j).idList.end(), sortWeight);

        }

        void write() {
            fstream newFile;
            int lineCount = 0 ;
            newFile.open("pairs"+inputString+".adj", ios::out);
            newFile << "<<< There are " << adjList.size() << " IDs in total. >>>"  ;
            for ( int j = 0 ; j < adjList.size() ; j++ ) {
                newFile << endl << "[" << setw(3) << j+1 << "] "<< adjList.at(j).ID << ": " ;
                lineCount = 0 ;
                newFile << endl  ;
                for ( int k = 0 ; k < adjList.at(j).idList.size() ; k++ ) {
                    lineCount++ ;
                    newFile <<"\t(" <<  setw(2) << lineCount << ") " << adjList.at(j).idList.at(k).ID << ", "<<setw(5)<<adjList.at(j).idList.at(k).weight  ;

                    if ( lineCount % 10 ==0 ) {
                        newFile << endl ;
                    }
                }

            }

            newFile << endl << "<<< There are " << nodeCount << " nodes in total. >>>" << endl ;
            newFile.close() ;
            cout << "done!" << endl ;
        }


	void BFStraverse() { // 優先寬度走訪
        vector<BFSData> BFS ;
        queue<char*> theQueue ;
        vector<int> haveName ; // 檢查是否已存在
        for ( int i = 0 ; i < adjList.size() ; i ++ ) {
            haveName.clear() ;
            BFSData temp ;
            strcpy( temp.ID, adjList.at(i).ID ) ;
            haveName.push_back( atoi( adjList.at(i).ID ) ) ;
            int size = 0 ;
            while ( size < adjList.at(i).idList.size()) { //將發迅人放入柱列
                char* idTemp = new char[10] ;
                strcpy( idTemp, adjList.at(i).idList.at(size).ID ) ;
                theQueue.push( idTemp ) ;
                haveName.push_back( atoi( idTemp ) ) ;
                size ++ ;
            }


            while ( !theQueue.empty() ) {
                char* idTemp = new char[10] ;
                strcpy( idTemp, theQueue.front() ) ; // 拿最前面的質比較
                temp.BFSNode.push_back( idTemp ) ; //將毒入的質放入TEMP
                for ( int i = 0 ; i < adjList.size() ; i ++ ) { //逐一檢查相鄰串列的值，是否能夠放入
                        if ( strcmp( idTemp, adjList.at(i).ID ) == 0 ) {
                            for ( int j = 0 ; j < adjList.at(i).idList.size() ; j ++ ) {
                            char* idTemp2 = new char[10] ;
                            strcpy( idTemp2, adjList.at(i).idList.at(j).ID ) ;
                            bool haveNum = false;
                            for ( int k = 0 ; k < haveName.size() ; k++ ) { //是否已經走過
                                if ( haveName.at(k) == atoi( idTemp2 ) ) {
                                    haveNum = true ;
                                }
                            }
                            if ( !haveNum ) {  //沒走過的話放入
                                theQueue.push( idTemp2 ) ;
                                haveName.push_back( atoi( idTemp2 ) ) ;
                            } // end if
                        } // end inner for
                        break ;
                        } // if
                } // for
                theQueue.pop() ; // 將處理完的值移除
            } // end while

            BFS.push_back( temp ) ; //放入串列
            haveName.clear() ;
        } // end outer for

        for( int i = 0 ; i < BFS.size() ; i++) {
            sort(BFS.at(i).BFSNode.begin(), BFS.at(i).BFSNode.end(), sortIDBFS);
        }

        writeBFS( BFS ) ;
	} // BFS



    void writeBFS( vector<BFSData> adjList ) {

        fstream newFile ;
        string filename = "pairs" + inputString + ".cnt" ;
        newFile.open( filename, ios::out ) ;
        newFile << "<<< There are " << adjList.size() << " IDs in total. >>>"  <<endl;
        for ( int i = 0 ; i < adjList.size() ; i ++ ) {
            newFile << "[" << setw(3)<< i + 1 << "]" << adjList.at(i).ID << "\t" << "Power : " << adjList.at(i).BFSNode.size() << endl ;
            for ( int j = 0 ; j < adjList.at(i).BFSNode.size() ; j ++ ) {
                newFile << "\t"<< "(" << setw(2 )<< j + 1 << ") " << adjList.at(i).BFSNode.at(j)  ;
                if ( j %10 == 9 && j!= 0)
                    newFile << endl ;
            } //end inner for
            newFile << endl ;
        } // end outer for
        newFile.close() ;
        cout << "done!!" << endl ;
    } // writeBFS



};

int main(void) {
	int command = -1;
	AdjacencyList adjList ;
    bool check = false ;
	while ( command != 0 ) {
		cout << endl;
		cout << "***** Influence on a graph  ****\n"
			 << "* 0. QUIT                      *\n"
			 << "* 1. Build adjacency lists     *\n"
			 << "* 2. Compute influence values  *\n"
			 << "********************************\n\n"
			 << "Please input your command :  " ;
		cin >> command;
		if ( command == 1 ) {
            adjList.reset() ;
            adjList.readBin() ;
            adjList.adjacency_list() ;
            adjList.write() ;
			cout << endl ;
			check = true ;
		}
		else if ( command == 2 ) {
            if ( check ) {
                adjList.BFStraverse() ;
                check = false ;
            }
            else {
                cout << "### There is no graph and choose 1 first. ###" << endl ;
            }

		}
		else if ( command == 3 ) {
            if ( check ) {
                //adjList.BFStraverse2() ;
                check = false ;
            }
            else {
                cout << "### There is no graph and choose 1 first. ###" << endl ;
            }

		}

		else if ( command != 0 ) {
			cout << endl << "Error !!! 請輸入正確的指令!!!\n\n" ;
		}
	}

	cout << endl << "結束!" << endl;
}
