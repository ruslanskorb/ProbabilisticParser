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

bool isAskedQuestion(int indexOfQuestion, vector<int> questions)
{
    bool isAsked = false;
    
    for (int i = 0; i < questions.size(); i++) {
        if (questions[i] == indexOfQuestion) {
            isAsked = true;
            break;
        }
    }
    
    return isAsked;
}

int indexOfMaxSumZ(vector<float> vectorSumZ, vector<int> questions)
{
    float maxSumZ = -1;
    int indexMaxSumZ = -1;
    
    for (int i = 0; i < vectorSumZ.size(); i++) {
        if (vectorSumZ[i] > maxSumZ) {
            if (!isAskedQuestion(i, questions)) {
                maxSumZ = vectorSumZ[i];
                indexMaxSumZ = i;
            }
        }
    }
    
    return indexMaxSumZ;
}

vector<float> PSQuestions(vector< vector< vector<float> > > PSigns, vector<float> firstVPSQuestions, vector< vector<float> > vectorPSQuestions, int question, int answer)
{
    vector<float> vPSQuestions;
    
    if (vectorPSQuestions.size() == 0) {
        vectorPSQuestions.push_back(firstVPSQuestions);
    }
    
    for (int i = 0; i < PSigns[question].size(); i++) {
        float PSQuestionValue;
        
        if (answer == 1) {
            PSQuestionValue = (PSigns[question][i][0] * vectorPSQuestions[vectorPSQuestions.size() - 1][i]) / ( (PSigns[question][i][0] * vectorPSQuestions[vectorPSQuestions.size() - 1][i]) + ( PSigns[question][i][1] * (1 - vectorPSQuestions[vectorPSQuestions.size() - 1][i]) ) );
        }
        else if (answer == -1)
        {
            PSQuestionValue = ( (1 - PSigns[question][i][0]) * vectorPSQuestions[vectorPSQuestions.size() - 1][i]) / ( (1 - PSigns[question][i][0] * vectorPSQuestions[vectorPSQuestions.size() - 1][i]) - ( PSigns[question][i][1] * (1 - vectorPSQuestions[vectorPSQuestions.size() - 1][i]) ) );
        }
        
        vPSQuestions.push_back(PSQuestionValue);
    }
    
    return vPSQuestions;
}

vector<float> PMaxQuestions(vector< vector< vector<float> > > PSigns, vector< vector<float> > vectorPSQuestions, vector< vector<float> > vectorPMaxQuestions, int question)
{
    vector<float> vPMaxQuestions;
    
    if (vectorPMaxQuestions.size() == 0) {
        vectorPMaxQuestions.push_back(vectorPSQuestions[0]);
    }
    
    for (int i = 0; i < PSigns[question].size(); i++) {
        float PMaxQuestionValue;
        
        if (PSigns[question][i][0] > PSigns[question][i][1]) {
            PMaxQuestionValue = (PSigns[question][i][0] * vectorPMaxQuestions[vectorPMaxQuestions.size() - 1][i]) / ( (PSigns[question][i][0] * vectorPMaxQuestions[vectorPMaxQuestions.size() - 1][i]) + ( PSigns[question][i][1] * (1 - vectorPMaxQuestions[vectorPMaxQuestions.size() - 1][i]) ) );
        }
        else
        {
            PMaxQuestionValue = ( (1 - PSigns[question][i][0]) * vectorPMaxQuestions[vectorPMaxQuestions.size() - 1][i]) / ( (1 - PSigns[question][i][0] * vectorPMaxQuestions[vectorPMaxQuestions.size() - 1][i]) - ( PSigns[question][i][1] * (1 - vectorPMaxQuestions[vectorPMaxQuestions.size() - 1][i]) ) );
        }
        
        vPMaxQuestions.push_back(PMaxQuestionValue);
    }
    
    return vPMaxQuestions;
}

vector<float> PMinQuestions(vector< vector< vector<float> > > PSigns, vector< vector<float> > vectorPSQuestions, vector< vector<float> > vectorPMinQuestions, int question)
{
    vector<float> vPMinQuestions;
    
    if (vectorPMinQuestions.size() == 0) {
        vectorPMinQuestions.push_back(vectorPSQuestions[0]);
    }
    
    for (int i = 0; i < PSigns[question].size(); i++) {
        float PMinQuestionValue;
        
        if (PSigns[question][i][0] > PSigns[question][i][1]) {
            PMinQuestionValue = ( (1 - PSigns[question][i][0]) * vectorPMinQuestions[vectorPMinQuestions.size() - 1][i]) / ( (1 - PSigns[question][i][0] * vectorPMinQuestions[vectorPMinQuestions.size() - 1][i]) - ( PSigns[question][i][1] * (1 - vectorPMinQuestions[vectorPMinQuestions.size() - 1][i]) ) );
        }
        else
        {
            PMinQuestionValue = (PSigns[question][i][0] * vectorPMinQuestions[vectorPMinQuestions.size() - 1][i]) / ( (PSigns[question][i][0] * vectorPMinQuestions[vectorPMinQuestions.size() - 1][i]) + ( PSigns[question][i][1] * (1 - vectorPMinQuestions[vectorPMinQuestions.size() - 1][i]) ) );
        }
        
        vPMinQuestions.push_back(PMinQuestionValue);
    }
    
    return vPMinQuestions;
}

int indexOfHypotesisWithMaxPSQuestion(vector<float> vPSQuestions)
{
    float maxPMax = INT32_MIN;
    int index = 0;
    
    for (int i = 0; i < vPSQuestions.size(); i++) {
        if (vPSQuestions[i] > maxPMax) {
            maxPMax = vPSQuestions[i];
            index = i;
        }
    }
    
    return index;
}

int main(int argc, const char * argv[])
{
    FILE *in = fopen("/Users/ruslan/Developer/ProbabilisticParser/ProbabilisticParser/input.txt", "r");
    vector<float> PS = vectorPSFromFile(in);
    vector< vector< vector<float> > > PSigns = vectorPSignsFromFile(in, (int)(PS.size()));
    fclose(in);
    
	FILE *out = fopen("/Users/ruslan/Developer/ProbabilisticParser/ProbabilisticParser/output.txt", "w");
    
    vector<int> questions;
    vector<int> answers;
    vector< vector<float> > vectorPSQuestions;
    vector< vector<float> > vectorPMaxQuestions;
    vector< vector<float> > vectorPMinQuestions;
    
    fprintf(out, "\nVector PS:\n\n");
    outputVectorTo(out, PS);
    
    fprintf(out, "\nVector PSigns:\n\n");
    outputVectorTo(out, PSigns);
    
    vector<float> vPSQuestions = PS;
    vector<float> vPMaxQuestions;
    vector<float> vPMinQuestions;
    
    bool isAcceptedHypothesis = false;
    int indexOfAcceptedHypoteses = -1;
    
    for (int i = 0; i < PSigns.size(); i++) {
        fprintf(out, "\nVector PSq:\n\n");
        vector< vector<float> > PSq = vectorPSq(vPSQuestions, PSigns);
        outputDualVectorTo(out, PSq);
        
        fprintf(out, "\nVector PSqInverse:\n\n");
        vector< vector<float> > PSqInverse = vectorPSqInverse(vPSQuestions, PSigns);
        outputDualVectorTo(out, PSqInverse);
        
        fprintf(out, "\nVector Z:\n\n");
        vector< vector<float> > Z = vectorZ(PSq, PSqInverse);
        outputDualVectorTo(out, Z);
        
        fprintf(out, "\nVector sum Z:\n\n");
        vector<float> sumZ = vectorSumZ(Z);
        outputVectorTo(out, sumZ);
        
        int indexMaxSumZ = indexOfMaxSumZ(sumZ, questions);
        fprintf(out, "\nIndex of max sum Z:\n\n%d\n", indexMaxSumZ);
        
        questions.push_back(indexMaxSumZ);
        
        fprintf(out, "\nVector questions:\n\n");
        outputVectorTo(out, questions);
        
        int answer = INT32_MIN;
        do {
            printf("Please respond to question #%d ('1' - Yes, '0' - I don't know, '-1' - No): ", indexMaxSumZ);
            scanf("%d", &answer);
        } while (answer != 1 && answer != -1 && answer != 0);
        
        answers.push_back(answer);
        
        fprintf(out, "\nVector answers:\n\n");
        outputVectorTo(out, answers);
        
        vPSQuestions = PSQuestions(PSigns, vPSQuestions, vectorPSQuestions, indexMaxSumZ, answer);
        vectorPSQuestions.push_back(vPSQuestions);
        
        vPMaxQuestions = PMaxQuestions(PSigns, vectorPSQuestions, vectorPMaxQuestions, indexMaxSumZ);
        vectorPMaxQuestions.push_back(vPMaxQuestions);
        
        vPMinQuestions = PMinQuestions(PSigns, vectorPSQuestions, vectorPMinQuestions, indexMaxSumZ);
        vectorPMinQuestions.push_back(vPMinQuestions);
        
        for (int j = 0; j < vPMaxQuestions.size(); j++) {
            if (vPMinQuestions[j] > vPSQuestions[j]) {
                break;
            } else if (vPMaxQuestions[j] < vPSQuestions[j]) {
                indexOfAcceptedHypoteses = j;
                isAcceptedHypothesis = true;
                break;
            }
        }
        
        if (isAcceptedHypothesis) {
            break;
        }
    }
    
    fprintf(out, "\nPS questions:\n\n");
    outputDualVectorTo(out, vectorPSQuestions);
    
    fprintf(out, "\nP max questions:\n\n");
    outputDualVectorTo(out, vectorPMaxQuestions);
    
    fprintf(out, "\nP min questions:\n\n");
    outputDualVectorTo(out, vectorPMinQuestions);
    
    if (!isAcceptedHypothesis) {
        indexOfAcceptedHypoteses = indexOfHypotesisWithMaxPSQuestion(vPSQuestions);
        fprintf(out, "\nAccepted hypothesis is number %d with probability %f\n", indexOfAcceptedHypoteses, vPSQuestions[indexOfAcceptedHypoteses]);
    } else {
        fprintf(out, "\nAccepted hypothesis is number %d\n", indexOfAcceptedHypoteses);
    }
    
    fclose(out);
    
    return 0;
}

