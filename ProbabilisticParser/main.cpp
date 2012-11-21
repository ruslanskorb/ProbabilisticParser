//
//  main.cpp
//  ProbabilisticParser
//
//  Created by Ruslan Skorb on 11/17/12.
//  Copyright (c) 2012 Ruslan Skorb. All rights reserved.
//

#include <cstdio>
#include <vector>
#include <cmath>

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

vector< vector<float> > vectorPSq(vector<float> PS, vector< vector< vector<float> > > PSigns)
{
    vector< vector<float> > PSq;
    
    for (int i = 0; i < PSigns.size(); i++) {
        vector<float> v;
        for (int j = 0; j < PS.size(); j++) {
            float PSqValue = (PSigns[i][j][0] * PS[j]) / ( (PSigns[i][j][0] * PS[j]) + ( PSigns[i][j][1] * (1 - PS[j]) ) );
            v.push_back(PSqValue);
        }
        PSq.push_back(v);
    }
    
    return PSq;
}

vector< vector<float> > vectorPSqInverse(vector<float> PS, vector< vector< vector<float> > > PSigns)
{
    vector< vector<float> > PSqInverse;
    
    for (int i = 0; i < PSigns.size(); i++) {
        vector<float> v;
        for (int j = 0; j < PS.size(); j++) {
            float PSqValue = ( (1 - PSigns[i][j][0]) * PS[j]) / ( (1 - PSigns[i][j][0] * PS[j]) - ( PSigns[i][j][1] * (1 - PS[j]) ) );
            v.push_back(PSqValue);
        }
        PSqInverse.push_back(v);
    }
    
    return PSqInverse;
}

vector< vector<float> > vectorZ(vector< vector<float> > PSq, vector< vector<float> > PSqInverse)
{
    vector< vector<float> > Z;
    
    for (int i = 0; i < PSq.size(); i++) {
        vector<float> v;
        for (int j = 0; j < PSq[i].size(); j++) {
            float zValue = fabs(PSq[i][j] - PSqInverse[i][j]);
            v.push_back(zValue);
        }
        Z.push_back(v);
    }
    
    return Z;
}

vector<float> vectorSumZ(vector< vector<float> > Z)
{
    vector<float> sumZ;
    
    for (int i = 0; i < Z.size(); i++) {
        float sumZValue = 0;
        for (int j = 0; j < Z[i].size(); j++) {
            sumZValue += Z[i][j];
        }
        sumZ.push_back(sumZValue);
    }
    
    return sumZ;
}

int indexOfMaxSumZ(vector<float> vectorSumZ)
{
    float maxSumZ = -1;
    int indexMaxSumZ = -1;
    
    for (int i = 0; i < vectorSumZ.size(); i++) {
        if (vectorSumZ[i] > maxSumZ) {
            maxSumZ = vectorSumZ[i];
            indexMaxSumZ = i;
        }
    }
    
    return indexMaxSumZ;
}

int main(int argc, const char * argv[])
{
    FILE *in = fopen("/Users/ruslan/Developer/ProbabilisticParser/ProbabilisticParser/input.txt", "r");
    vector<float> PS = vectorPSFromFile(in);
    vector< vector< vector<float> > > PSigns = vectorPSignsFromFile(in, (int)(PS.size()));
    fclose(in);
    
	FILE *out = fopen("/Users/ruslan/Developer/ProbabilisticParser/ProbabilisticParser/output.txt", "w");
    
    fprintf(out, "\nVector PS:\n\n");
    outputVectorTo(out, PS);
    
    fprintf(out, "\nVector PSigns:\n\n");
    outputVectorTo(out, PSigns);
    
    fprintf(out, "\nVector PSq:\n\n");
    vector< vector<float> > PSq = vectorPSq(PS, PSigns);
    outputDualVectorTo(out, PSq);
    
    fprintf(out, "\nVector PSqInverse:\n\n");
    vector< vector<float> > PSqInverse = vectorPSqInverse(PS, PSigns);
    outputDualVectorTo(out, PSqInverse);
    
    fprintf(out, "\nVector Z:\n\n");
    vector< vector<float> > Z = vectorZ(PSq, PSqInverse);
    outputDualVectorTo(out, Z);
    
    fprintf(out, "\nVector sum Z:\n\n");
    vector<float> sumZ = vectorSumZ(Z);
    outputVectorTo(out, sumZ);
    
    int indexMaxSumZ = indexOfMaxSumZ(sumZ);
    fprintf(out, "\nIndex of max sum Z: %d\n\n", indexMaxSumZ);
    
    fclose(out);
    
    return 0;
}

