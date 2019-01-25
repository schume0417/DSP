#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hmm.h"

int main(int argc, char const *argv[]){
	HMM hmm[5];
	load_models(argv[1],hmm,5);
	FILE *fp;
	fp = fopen(argv[2],"r");
	char seq_model[2500][100]; 
	int  seq[2500][100];
	for (int i = 0; i < 2500; i++){
		fscanf(fp,"%s",seq_model[i]);
		for (int j = 0; j < 50; j++){
			if (seq_model[i][j] == 'A')
				seq[i][j] = 0;
			else if (seq_model[i][j] == 'B')
				seq[i][j] = 1;
			else if (seq_model[i][j] == 'C')
				seq[i][j] = 2;
			else if (seq_model[i][j] == 'D')
				seq[i][j] = 3;
			else if (seq_model[i][j] == 'E')
				seq[i][j] = 4;
			else if (seq_model[i][j] == 'F')
				seq[i][j] = 5;
			//printf("%d",seq[i][j]);
		}
		//printf("\n");
	} 
	fclose(fp);

	fp = fopen(argv[3],"wb");

	for (int n = 0; n < 2500; n++){
		double max = 0;
		int choose_model = 0;
		for (int i = 0; i < 5; i++){
			double v[6][50] = {0};
			//===========================viterbi===========================
			for (int j = 0; j < 6; j++){
				v[j][0] = hmm[i].initial[j] * hmm[i].observation[seq[n][0]][j] ;
			}
			for (int k = 1; k < 50; k++){
				for (int j = 0; j < 6; j++){
					double little_max = 0;
					for (int l = 0; l < 6; l++){
						double now = 0;
						now = v[l][k-1] * hmm[i].transition[l][j];
						if (now > little_max){
							little_max = now;
						}
					}
					v[j][k] = little_max * hmm[i].observation[seq[n][k]][j];
				}
			}
			for (int j = 0; j < 6; j++){
				if (v[j][49] > max){
					max = v[j][49];
					choose_model = i;
				}
			}

		}
		fprintf(fp, "model_0%d.txt %e\n",choose_model+1,max);

	}

	fclose(fp);



	//===========================calculate accuracy===========================



	return 0;
}