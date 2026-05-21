#ifndef FRAME_H
#define FRAME_H

#include <vector>
#include <string>

using namespace std;

vector<string> generateTarget(int N, int M, char edgeValue);
int solveFrameProblem(vector<string>& field, vector<vector<string>>& steps, char& chosenEdge);

#endif