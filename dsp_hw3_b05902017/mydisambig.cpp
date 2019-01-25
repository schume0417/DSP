#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <stdio.h>
#include <string>

#include "Ngram.h"
#include "Vocab.h"

using namespace std;

int main(int argc, char const *argv[]){
	map< string, set<string> > my_map;
	Vocab voc;
	Ngram lm(voc,2);
	FILE *testdata;
	for (int i = 0; i < argc; i++){
		if ( string("-lm") == argv[i]){
			File lmFile( argv[i+1], "r" );
			lm.read(lmFile);
			lmFile.close();
		}
		if ( string("-map") == argv[i] ){
			FILE *fp = fopen(argv[i+1],"r");
			char first[10000];
			string key;	
			string value;
			string tmp;
			int j;
			while((j = fscanf(fp,"%s",first)) != EOF){
				key = string(first);
				//printf("%s\n",key.c_str());
				int pos = 0;
				char second[10000];
				fgets(second,10000,fp);
				value = string(second);
				//printf("%s",value.c_str());
				//printf("%s",key.c_str());
				while(pos != -1){
            		pos = value.find(" ");
            		tmp = value.substr(0, pos);
            		//printf("%s\n",tmp.c_str());
            		my_map[key].insert(tmp);
            		value = value.substr(pos + 1);
        		}
			}
			
			/*
			map< string, set<string> >::iterator it;
			int count = 0;
		    for (it = my_map.begin(); it != my_map.end();it++){
                cout<< it->first << '\n';
                set<string> TmpSet;
                TmpSet = it->second;
           	    set<string>::iterator iter;
				for (iter = TmpSet.begin() ;iter != TmpSet.end();iter ++)
				  	cout<<*iter;
					cout<<'\n';
			}
			*/
			
			
		}	
		if ( string("-text") == argv[i] ){
			//File text(argv[i+1], "r");
			testdata = fopen(argv[i+1],"r");
		}
	}

	/*
	File testdata(argv[2],"r");
	char *line = NULL;
	while (line = testdata.getline()){
		printf("%s\n", line);
	}
	*/

	char line[10000];
	int countnum = 0;
	while (fgets(line,10000,testdata)){
		//if (countnum > 0)
		//	break;  //debug
		vector<string> text_line;
		//printf("%s\n", line);
    	string tmp;
    	string tmpline = string(line);
    	int pos;
    	while((pos = tmpline.find(" ")) != -1){
       		tmpline.erase(pos, 1);
    	}
    	//printf("%s\n",tmpline.c_str());
    	for(int i = 0 ; i < tmpline.size() ; i += 2){
       		tmp = tmpline.substr(i, 2);
       		text_line.push_back(tmp);
       	//	printf("%s",tmp.c_str());
    	}
    	vector< set<string> > possible_road;
		map< string, set<string> >::iterator it;
		set <string> s {"<s>"};
		set <string> s_end {"</s>"};
		possible_road.push_back(s);
		for (int i = 0 ; i < text_line.size()-1; i++){
			set<string> tmp;
			//printf("%s\n",text_line.at(i).c_str());
			it = my_map.find(text_line.at(i));
			//cout<< it->second << '\n';
			if (it != my_map.end() && (it->second).size() > 2){
				//printf("%s\n",text_line[i]);
		 		tmp = (it->second);
				//printf("%d\n",tmp.size());
			}
			else{
				tmp.insert(text_line.at(i));
				//printf("!!! = %d !!!%d\n",i,tmp.size());
			}
			//set<string>::iterator iter;
			//for (iter = tmp.begin() ;iter != tmp.end();iter ++){
			//	cout<<*iter<<' ';
			//	cout<<'\n';
			//}
			possible_road.push_back(tmp);
		}
		possible_road.push_back(s_end);
		//printf("%d\n",text_line.size());
		//printf("%d\n",countnum);
		countnum++;
		//printf("%d\n",possible_road.size());
		vector< pair< vector<string>,double> > start;
		set<string>::iterator iter;
		for (iter = possible_road[0].begin(); iter != possible_road[0].end() ; iter++){
			vector<string> node;
			node.push_back((*iter));
			double zero = 0;
			start.push_back(make_pair(node,zero));
		}
		//printf("success\n");
	    int in = -1;

		for (int i = 1; i < possible_road.size() ; i++){
			/*
			double maxProb = -1000.0;
    		int Index = 0;
    		for(int j = 0 ; j < start.size() ; j++){
        		if(start.at(j).second > maxProb){
            		Index = j;
            		maxProb = start.at(j).second;
        		}
    		}
			vector<string> best = start.at(Index).first;
			//printf("%d",Index);
			cout << "<s>";
    		for(int j = 0 ; j < best.size() ; j++){
       			cout << " " << best.at(j);
   	 		}
    		cout << " </s>" << endl;
			*/

			//printf("%d\n",i);
			vector< pair< vector<string>,double> > road;
			set<string> tmp;
			tmp = possible_road.at(i);

			vector<string> best;
			double finalmax= -1000;
			//printf("%s\n",tmp);
			int cou = 0;
			//int max_index = 0;
			for (iter = tmp.begin(); iter != tmp.end(); iter++ ){
			    //printf("sad");
				string now_tmp;
				now_tmp = *iter;
				//printf("%s",now_tmp);
				pair < vector<string>, double> bigram;	
				double max = -1000;
				int max_index = 0;
				//printf("start_size = %d\n",start.size());
				for (int j = 0; j < start.size(); j++){
					vector<string>::iterator iterat;
					iterat = start.at(j).first.end();
        			iterat -= 1;
					string first;
					first = (*iterat);
					//if (countnum == 44){	
					//printf("%s\n",first.c_str());
					//}
					double old;
					old = start.at(j).second;
					double new_prob;	
					VocabIndex id1 = voc.getIndex(first.c_str());
					VocabIndex id2 = voc.getIndex(now_tmp.c_str());
					if (id1 == Vocab_None)
						id1 = voc.getIndex(Vocab_Unknown);
					if (id2 == Vocab_None)
						id2 = voc.getIndex(Vocab_Unknown);
					VocabIndex con[] = {id1, Vocab_None};
					new_prob = lm.wordProb(id2, con);
					//if (countnum == 8){	
					//	printf("%s\n",first.c_str());
					//	printf("%f\n",new_prob);
					//	printf("%f\n",old);
					//}
					if ( (new_prob + old) > max){
 						//if (countnum == 22){	
						//printf("%s\n",first.c_str());
						//printf("%f\n",new_prob);
						//printf("%f\n",old);
						//}
						max = new_prob + old;
						max_index = j;
						//printf("changed!!! %d\n",max_index);
					}
					//printf("!!! %d\n",max_index);
				}
				//printf("max_index = %d\n",max_index);
				//if (countnum == 34){	
					//printf("max_index = %d\n", max_index);
				//	printf("%s\n",now_tmp.c_str());
				//	printf("%f\n",max);
				//}
				bigram = start.at(max_index);
				in = max_index;
				bigram.first.push_back(now_tmp);
				bigram.second = max;
				road.push_back(bigram);
				//printf("%d\n",cou);
				cou ++;
				//if (max > finalmax && i == possible_road.size()-2 && countnum ==22){
				//	best = bigram.first;
				//	for (int k =0 ;k< best.size();k++){
				//	cout << best.at(k);
				//	}

				//}

			//	printf("%s\n",now_tmp.c_str());
			}
			start.clear();
			start = road;
		}
		double maxProb = -1000.0;
    	int Index = 0;
    	for(int j = 0 ; j < start.size() ; j++){
       		if(start.at(j).second > maxProb){
           		Index = j;
           		maxProb = start.at(j).second;
        	}
    	}
	
		vector<string> best = start.at(Index).first;
		//printf("%d\n",best.size());
    	for(int j = 0 ; j < best.size() -1 ; j++){
       		cout << best.at(j)<< " ";
   	 	}
    	cout << "</s>" <<  endl;
	}

	return 0;
}

