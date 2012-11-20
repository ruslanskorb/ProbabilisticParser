//
//  main.cpp
//  ProbabilisticParser
//
//  Created by Ruslan Skorb on 11/17/12.
//  Copyright (c) 2012 Ruslan Skorb. All rights reserved.
//

#include <cstdio>
#include <vector>

using namespace std;

void outputDualVectorTo(FILE* out, vector< vector<float> > v)
{
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            fprintf(out, "%f ", v[i][j]);
        }
        fprintf(out, "\n");
    }
}

void outputDualVectorTo(FILE* out, vector< vector<int> > v)
{
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            fprintf(out, "%d ", v[i][j]);
        }
        fprintf(out, "\n");
    }
}

void outputVectorTo(FILE *out, vector<float> v)
{
    for (int i = 0; i < v.size(); i++) {
        fprintf(out, "%f ", v[i]);
    }
    fprintf(out, "\n");
}

void outputVectorTo(FILE *out, vector<int> v)
{
    for (int i = 0; i < v.size(); i++) {
        fprintf(out, "%d ", v[i]);
    }
    fprintf(out, "\n");
}

void outputVectorTo(FILE *out, vector< vector< vector<float> > > v)
{
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            for (int l = 0; l < v[i][j].size(); l++) {
                fprintf(out, "%f ", v[i][j][l]);
            }
        }
        fprintf(out, "\n");
    }
}

vector<float> vectorPSFromFile(FILE *in)
{
    vector<float> PS;
    
    int psCount = 0;
    fscanf(in, "%d", &psCount);
    
    for (int i = 0; i < psCount; i++) {
        float PSi = 0.0f;
        fscanf(in, "%f", &PSi);
        PS.push_back(PSi);
    }
    
    return PS;
}

vector< vector< vector<float> > > vectorPSignsFromFile(FILE *in, int psCount)
{
    vector< vector< vector<float> > > PSigns;
    
    int pSignsCount = 0;
    fscanf(in, "%d", &pSignsCount);
    
    for (int i = 0; i < pSignsCount; i++) {
        vector< vector<float> > v;
        for (int j = 0; j < psCount; j++) {
            vector<float> innerV;
            float pPlus = 0.0f;
            float pMinus = 0.0f;
            fscanf(in, "%f", &pPlus);
            fscanf(in, "%f", &pMinus);
            innerV.push_back(pPlus);
            innerV.push_back(pMinus);
            v.push_back(innerV);
        }
        PSigns.push_back(v);
    }
    
    return PSigns;
}

int main(int argc, const char * argv[])
{
    FILE *in = fopen("/Users/ruslan/Developer/ProbabilisticParser/ProbabilisticParser/input.txt", "r");
    vector<float> PS = vectorPSFromFile(in);
    vector< vector< vector<float> > > PSigns = vectorPSignsFromFile(in, (int)(PS.size()));
    fclose(in);
    
	FILE *out = fopen("/Users/ruslan/Developer/ProbabilisticParser/ProbabilisticParser/output.txt", "w");
    outputVectorTo(out, PS);
    outputVectorTo(out, PSigns);
    fclose(out);
    
    return 0;
}

