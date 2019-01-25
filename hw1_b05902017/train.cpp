#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hmm.h"


void train(HMM *hmm,const char *modelname,int iteration){
	FILE *fp;
	fp = fopen(modelname,"r");
	char seq_model[10000][100]; 
	int  seq[10000][100];   // 10000x50
	int length = 0;
	for (int i = 0; i < 10000; i++){
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
		}
	} 
	length = strlen(seq_model[0]);
	
for (int iter = 0; iter < iteration; iter++){  // iteration



	double gamma_all[6][50] ={0};
	double elpslon_all[6][6] = {0};
	double observe[6][6] = {0};

	for (int n = 0; n < 10000; n++){  // change to 10000 sample
		// ======================compute alpha======================
		double alpha[6][50] = {0};
		for (int j = 0; j < 6; j++){
			alpha[j][0] = hmm->initial[j] * hmm->observation[seq[n][0]][j];
		}	
		for (int i = 1; i < 50; i++){ 
			for (int j = 0; j < 6 ; j++){
				double all = 0;
				for (int k = 0; k < 6 ; k++){
					all += (alpha[k][i-1] * hmm->transition[k][j]);
				}
				alpha[j][i] = all * hmm->observation[seq[n][i]][j];
			}
		}

		/*
		for (int i = 0; i < 50; i++){
			printf("%c",seq_model[0][i]);
		}
		printf("\n");
		for (int i = 0; i < 50; i++){
			printf("%d", seq[0][i]);
		}
		printf("\n");

		for (int i = 0; i < 6; i++){
			for (int j = 0; j < 6; j++){
				printf("%f ",alpha[i][j]);
			}
			printf("\n");
		}
		*/

		// ======================compute beta======================
		double beta[6][50] = {0};
		for (int j = 0; j < 6; j++){
			beta[j][49] = 1;
		}
		for (int i = 48; i >=0 ; i--){
			for (int j = 0; j < 6; j++){
				double all = 0;
				for (int k = 0; k < 6; k++){
					all += (hmm->transition[j][k] * hmm->observation[seq[n][i+1]][k] * beta[k][i+1]);
				}
				beta[j][i] = all;
			}
		}

		// ======================compute gamma======================
		double gamma[6][50] = {0};
		for (int i = 0; i < 50; i++){
			for (int j = 0; j < 6; j++){
				double all = 0;
				for (int k = 0; k < 6; k++){
					all += (alpha[k][i] * beta[k][i]);
				} 
				gamma[j][i] = (alpha[j][i] * beta[j][i]) / all;
			}
		}

		// ======================compute elpslon======================
		double elpslon[49][6][6] = {0};
		for (int t = 0; t < 49; t++){
			double all = 0;
			for (int i = 0; i < 6; i++){
				for (int j = 0; j < 6; j++ ){
					elpslon[t][i][j] = alpha[i][t] *  hmm -> transition[i][j] * hmm -> observation[seq[n][t+1]][j] * beta[j][t+1];
					all += elpslon[t][i][j];
				}
			}
			for (int i = 0; i < 6; i++){
				for (int j = 0; j < 6; j++){
					elpslon[t][i][j] /= all;
				}
			}
		}


		// ======================accumulate======================
		for (int i = 0; i < 6; i++ ){
			for (int j = 0 ; j < 50; j++){
				gamma_all[i][j] += gamma[i][j];
			}
		}

		for (int t = 0; t < 49; t++ ){
			for (int i = 0; i < 6 ; i++){
				for (int j = 0; j < 6 ; j++){
					elpslon_all[i][j] += elpslon[t][i][j];
				}
			}
		}

		for (int i = 0; i < 50; i++){
			for (int j = 0; j < 6; j++){
				observe[seq[n][i]][j] += gamma[j][i];
			}
		}
	}
		// ======================re-estimate======================

		for (int i = 0; i < 6; i++){
			hmm -> initial[i] = gamma_all[i][0] / 10000;
		}
		double gamma_final[6] = {0};
		for (int i = 0; i < 6; i++ ){
			for (int j = 0; j < 49; j++){
				gamma_final[i] += gamma_all[i][j];
			}
		}

		for (int i = 0; i < 6; i++){
			for (int j = 0; j < 6; j++){
				hmm -> transition[i][j] = elpslon_all[i][j] / gamma_final[i];
			}
		}

		for (int i = 0; i < 6; i++){
			gamma_final[i] += gamma_all[i][49];
		}

		for (int i = 0; i < 6; i++){
			for (int j = 0; j < 6; j++){
				hmm -> observation[i][j] = observe[i][j] / gamma_final[j];
			}
		}

}

	/*
		printf("initial\n");
		double all = 0;
		for (int i = 0; i < 6; i++){
			printf("%f ",hmm -> initial[i]);
			all += hmm -> initial[i];
		}
		printf("%f ", all);
		printf("\n");
		printf("transition\n");
		for (int i = 0; i < 6; i++){
			all = 0;
			for (int j = 0; j < 6; j++){
				printf("%f ",hmm -> transition[i][j]);
				all += hmm -> transition[i][j];
			}
			printf("%f ",all );
			printf("\n");
		}
		printf("observation\n");
		for (int i = 0; i < 6; i++){
			all = 0;
			for (int j = 0; j < 6; j++){
				printf("%f ",hmm -> observation[i][j]);
				all += hmm -> observation[j][i];
			}
			printf("%f ",all );
			printf("\n");
		}
	*/

	return;
}


int main(int argc, char const *argv[])
{
	//printf("program started\n");
	int iteration = atoi(argv[1]);
	HMM hmm;
	loadHMM(&hmm,argv[2]);  //model_init.txt
	//seq_model_01.txt argv[3]
	//model_01.txt argv[4]
	//printf("%s\n",hmm.model_name);
	//printf("%d\n",hmm.state_num);
	//printf("%d\n",hmm.observ_num);
	train(&hmm,argv[3],iteration);

	FILE *fp;
	fp = fopen(argv[4],"wb");
	dumpHMM(fp,&hmm); 
	fclose(fp);
	

	return 0;
}